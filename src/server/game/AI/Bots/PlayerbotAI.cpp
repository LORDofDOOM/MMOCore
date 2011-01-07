#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "DBCStores.h"
#include "World.h"
#include "SpellMgr.h"
#include "PlayerbotAI.h"
#include "PlayerbotPriestAI.h"
#include "PlayerbotWarriorAI.h"
#include "PlayerbotShamanAI.h"
#include "PlayerbotRogueAI.h"
#include "PlayerbotPaladinAI.h"
#include "PlayerbotMageAI.h"
#include "PlayerbotDruidAI.h"
#include "PlayerbotWarlockAI.h"
#include "PlayerbotHunterAI.h"
#include "PlayerbotDeathKnightAI.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "WorldPacket.h"
#include "Spell.h"
#include "Unit.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SharedDefines.h"
#include "GossipDef.h"
#include "Config.h"
#include "Group.h"
#include <ctime>



/*
* Packets often compress the GUID (global unique identifier)
* This function extracts the guid from the packet and decompresses it.
* The first word (8 bits) in the packet represents how many words in the following packet(s) are part of
* the guid and what weight they hold. I call it the mask. For example) if mask is 01001001,
* there will be only 3 words. The first word is shifted to the left 0 times,
* the second is shifted 3 times, and the third is shifted 6.
*/
uint64 extractGuid(WorldPacket &packet)
{
    uint8 mask; packet >> mask;
    uint64 guid = 0;
    uint8 bit = 0;
    uint8 testMask = 1;
    while(true)
    {
        if(mask & testMask)
        {
            uint8 word; packet >> word;
            guid += (word << bit);
        }
        if(bit == 7) break;
        ++bit;
        testMask <<= 1;
    }
    return guid;
}

//ChatHandler already implements some useful commands the master can call on bots
//These commands are protected inside the ChatHandler class so this class provides access to the commands
//we'd like to call on our bots
class PlayerbotChatHandler : protected ChatHandler
{
    public:
        explicit PlayerbotChatHandler(Player *pMasterPlayer) : ChatHandler(pMasterPlayer){}

        bool revive(const Player &botPlayer){ return HandleReviveCommand(botPlayer.GetName()); }
        bool teleport(const Player &botPlayer){ return HandleSummonCommand(botPlayer.GetName()); }
        bool teleport(Player &botPlayer, const WorldLocation &loc){ return botPlayer.TeleportTo(loc,TELE_TO_GM_MODE); }
        bool uninvite(const char *str){ return HandlePlayerbotCommand(str); }
        void sysmessage(const char *str){ SendSysMessage(str); }
        bool acceptQuest(const char *str){ return HandleQuestAdd(str); }
        bool abandonQuest(const char *str) { return HandleQuestRemove(str); }
};

PlayerbotAI::PlayerbotAI(Player *const master, Player *const bot): m_master(master), m_bot(bot),
m_ignoreAIUpdatesUntilTime(0), m_combatOrder(ORDERS_NONE), m_ScenarioType(SCENARIO_PVEEASY),
m_TimeDoneEating(0), m_TimeDoneDrinking(0), m_CurrentlyCastingSpellId(0), m_IsFollowingMaster(true),
m_spellIdCommand(0), m_targetGuidCommand(0), m_classAI(0), isLooting(false), m_TimeRessurect(0),
m_FeastSpamTimer(0)
{

    //If the player have a group, it's possible to add the bot.
    if(master->GetGroup())
    {
        Group *m_group = master->GetGroup();
        bool inGroup = false;
        Group::MemberSlotList members = m_group->GetMemberSlots();

        if(!m_group->IsFull() ||
            m_group->IsMember(bot->GetGUID()) )
        {
            //check that bot is not already in the group, ie from a server crash
            Group::MemberSlotList const &groupSlot = master->GetGroup()->GetMemberSlots();
            for(Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
            {
                Player *tPlayer = master->GetObjPlayer(itr->guid);
                if(itr->guid == bot->GetGUID()) inGroup = true;
            }
            if(!inGroup) m_group->AddMember(bot->GetGUID(), bot->GetName());
        } else {
            //group is full so can't add bot
            bot->Say("Group is full!", LANG_UNIVERSAL);
        }
    } else {
        Group *m_group = new Group;
        if(!m_group->Create(master->GetGUID(), master->GetName()))
        {
            delete m_group;
            return;
        }
        sObjectMgr->AddGroup(m_group);
        if(!m_group->IsFull()) m_group->AddMember(bot->GetGUID(), bot->GetName());
    }

    //get class specific AI
    switch(m_bot->getClass())
    {
        case CLASS_PRIEST:
            m_classAI = (PlayerbotClassAI *)new PlayerbotPriestAI(master, m_bot, this);
            break;
        case CLASS_WARRIOR:
            m_classAI = (PlayerbotClassAI *)new PlayerbotWarriorAI(master, m_bot, this);
            break;
        case CLASS_SHAMAN:
            m_classAI = (PlayerbotClassAI *)new PlayerbotShamanAI(master, m_bot, this);
            break;
        case CLASS_ROGUE:
            m_classAI = (PlayerbotClassAI *)new PlayerbotRogueAI(master, m_bot, this);
            break;
        case CLASS_PALADIN:
            m_classAI = (PlayerbotClassAI *)new PlayerbotPaladinAI(master, m_bot, this);
            break;
        case CLASS_MAGE:
            m_classAI = (PlayerbotClassAI *)new PlayerbotMageAI(master, m_bot, this);
            break;
        case CLASS_DRUID:
            m_classAI = (PlayerbotClassAI *)new PlayerbotDruidAI(master, m_bot, this);
            break;
        case CLASS_WARLOCK:
            m_classAI = (PlayerbotClassAI *)new PlayerbotWarlockAI(master, m_bot, this);
            break;
        case CLASS_HUNTER:
            m_classAI = (PlayerbotClassAI *)new PlayerbotHunterAI(master, m_bot, this);
            break;
        case CLASS_DEATH_KNIGHT:
            m_classAI = (PlayerbotClassAI *)new PlayerbotDeathKnightAI(master, m_bot, this);
            break;
    }

    //load config variables
    m_followDistanceMin = sConfig->GetFloatDefault("Bot.FollowDistanceMin", 0.5f);
    m_followDistanceMax = sConfig->GetFloatDefault("Bot.FollowDistanceMax", 3.0f);
    m_playerBotsFly = sConfig->GetIntDefault("Bot.PlayerBotsFly", 0);

    SetQuestNeedItems();
    m_needEmblemList.clear();
    m_needEmblemList[29434] = 200; // Badge of Justice
    m_needEmblemList[40752] = 200; // Emblem of Heroism
    m_needEmblemList[40753] = 200; // Emblem of Valor
    m_needEmblemList[45624] = 200; // Emblem of Conquest
    m_needEmblemList[47241] = 200; // Emblem of Triumph
    m_needEmblemList[49426] = 200; // Emblem of Frost
    m_needEmblemList[44990] = 200; // Champion's Seal
    HandleCommand("help", *m_master);
}
PlayerbotAI::~PlayerbotAI(){}

//finds spell ID for matching substring args
//in priority of full text match, spells not taking reagents, and highest rank
uint32 PlayerbotAI::getSpellId(const char *args, bool master) const
{
    if(!*args) return 0;

    std::string namepart = args;
    std::wstring wnamepart;

    if(!Utf8toWStr(namepart, wnamepart)) return 0;

    //converting string that we try to find to lower case
    wstrToLower(wnamepart);

    int loc = 0;
    if(master) loc = m_master->GetSession()->GetSessionDbcLocale();
    else loc = m_bot->GetSession()->GetSessionDbcLocale();

    uint32 foundSpellId = 0;
    bool foundExactMatch = false;
    bool foundMatchUsesNoReagents = false;

    for(PlayerSpellMap::iterator itr = m_bot->GetSpellMap().begin(); itr != m_bot->GetSpellMap().end(); ++itr)
    {
        uint32 spellId = itr->first;

        if(itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled || IsPassiveSpell(spellId)) continue;

        const SpellEntry *pSpellInfo = sSpellStore.LookupEntry(spellId);
        if(!pSpellInfo) continue;

        const std::string name = pSpellInfo->SpellName[loc];
        if(name.empty() || !Utf8FitTo(name, wnamepart)) continue;

        bool isExactMatch = (name.length() == wnamepart.length()) ? true : false;
        bool usesNoReagents = (pSpellInfo->Reagent[0] <=  0) ? true : false;

        //if we already found a spell
        bool useThisSpell = true;
        if(foundSpellId > 0)
        {
            if(isExactMatch && !foundExactMatch){}
            else if(usesNoReagents && !foundMatchUsesNoReagents){}
            else if(spellId > foundSpellId){}
            else useThisSpell = false;
        }
        if(useThisSpell)
        {
            foundSpellId = spellId;
            foundExactMatch = isExactMatch;
            foundMatchUsesNoReagents = usesNoReagents;
        }
    }
    return foundSpellId;
}

uint32 PlayerbotAI::getSpellIdExact(const char *args, bool includePassive, bool master)
{
    if(!*args) return 0;
    std::string namepart = args;
    int loc = 0;
    if(master) loc = m_master->GetSession()->GetSessionDbcLocale();
    else loc = m_bot->GetSession()->GetSessionDbcLocale();
    uint32 foundSpellId = (uint32) 0;
    bool foundMatchUsesNoReagents = false;

    for(PlayerSpellMap::iterator itr = m_bot->GetSpellMap().begin(); itr != m_bot->GetSpellMap().end(); ++itr)
    {
        uint32 spellId = itr->first;
        if(itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled || ( !includePassive && IsPassiveSpell(spellId))) continue;
        const SpellEntry *pSpellInfo = sSpellStore.LookupEntry(spellId);
        if(!pSpellInfo) continue;
        if(pSpellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL) continue; //This is a learn spell

        const std::string name = pSpellInfo->SpellName[loc];
        if(name.empty()) continue;
        if(strcmp(name.c_str(),namepart.c_str())) continue;
        if(pSpellInfo->Reagent[0] <=  0 && !foundMatchUsesNoReagents){ foundSpellId = spellId; foundMatchUsesNoReagents = true; }
        else if(spellId > foundSpellId) { foundSpellId = spellId; }
    }
    //sLog->outDebug("PBot Class %u - Found in Search - [%u/%s]", m_bot->getClass(), foundSpellId, namepart.c_str());
    if (foundSpellId > 70000) { sLog->outDebug("CRITICAL: PBot Class %u - Weird Spell in Search - [%u/%s]", m_bot->getClass(), foundSpellId, namepart.c_str()); }
    return foundSpellId;
}

// finds quest ID for matching substring args
uint32 PlayerbotAI::getQuestId(const char* args, bool remove) const
{
    if (!*args)
        return 0;

    std::string namepart = args;
    std::wstring wnamepart;

    if (!Utf8toWStr(namepart,wnamepart))
        return 0;

    // converting string that we try to find to lower case
    wstrToLower(wnamepart);
    uint32 questId = 0;
    uint32 foundQuestId = 0;
    bool foundExactMatch = false;
    if (!m_questsSeen.empty() && !remove)
    {
        for (BotQuestsSeen::const_iterator iter = m_questsSeen.begin(); iter != m_questsSeen.end(); ++iter)
        {
            uint32 questId = iter->first;
            const std::string name = iter->second;
            if (name.empty() || !Utf8FitTo(name, wnamepart))
                continue;
            bool isExactMatch = (name.length() == wnamepart.length()) ? true : false;
            // if we already found a quest
            bool useThisQuest = true;
            if (foundQuestId > 0)
            {
                if (isExactMatch && ! foundExactMatch)
                {
                }
                else if (questId > foundQuestId)
                {
                }
                else
                    useThisQuest = false;
            }
            if (useThisQuest)
            {
                foundQuestId = questId;
                foundExactMatch = isExactMatch;
            }
        }
    }
    else if (remove)
    {
        for (QuestStatusMap::iterator iter=m_bot->getQuestStatusMap().begin(); iter!=m_bot->getQuestStatusMap().end(); ++iter)
        {
            const Quest *qInfo = sObjectMgr->GetQuestTemplate(iter->first);
            if (!qInfo) continue;

            uint32 questId = qInfo->GetQuestId();
            const std::string name = qInfo->GetTitle();
            if (name.empty() || !Utf8FitTo(name, wnamepart))
                continue;

            bool isExactMatch = (name.length() == wnamepart.length()) ? true : false;
            // if we already found a quest
            bool useThisQuest = true;
            if (foundQuestId > 0)
            {
                if (isExactMatch && ! foundExactMatch)
                {
                }
                else if (questId > foundQuestId)
                {
                }
                else
                    useThisQuest = false;
            }
            if (useThisQuest)
            {
                foundQuestId = questId;
                foundExactMatch = isExactMatch;
            }
        }
    }
    return foundQuestId;
}


/*
* Send a list of equipment that is in bot's inventor that is currently unequipped.
* This is called when the master is inspecting the bot.
*/
void PlayerbotAI::SendNotEquipList(Player &player)
{
    //find all unequipped items and put them in
    //a vector of dynamically created lists where the vector index is from 0-18
    //and the list contains Item *that can be equipped to that slot
    //Note: each dynamically created list in the vector must be deleted at end
    //so NO EARLY RETURNS!
    //see enum EquipmentSlots in Player.h to see what equipment slot each index in vector
    //is assigned to. (The first is EQUIPMENT_SLOT_HEAD = 0, and last is EQUIPMENT_SLOT_TABARD = 18)

    std::list<Item *> *equip[19];
    for(uint8 i = 0; i < 19; ++i) equip[i] = NULL;

    //list out items in main backpack
    for(uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item *const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if(!pItem)
            continue;

        uint16 dest;
        uint8 msg = m_bot->CanEquipItem(NULL_SLOT, dest, pItem, !pItem->IsBag());
        if(msg != EQUIP_ERR_OK) continue;

        //the dest looks like it includes the old loc in the 8 higher bits
        //so casting it to a uint8 strips them
        uint8 equipSlot = uint8(dest);
        if(!(equipSlot >= 0 && equipSlot < 19)) continue;

        //create a list if one doesn't already exist
        if(equip[equipSlot] == NULL)
            equip[equipSlot] = new std::list<Item *>;

        std::list<Item *> *itemListForEqSlot = equip[equipSlot];

        itemListForEqSlot->push_back(pItem);
    }

    //list out items in other removable backpacks
    for(uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        const Bag *const pBag = (Bag *)m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if(pBag)
        {
            for(uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                Item *const pItem = m_bot->GetItemByPos(bag, slot);
                if(!pItem)
                    continue;

                uint16 equipSlot;
                uint8 msg = m_bot->CanEquipItem(NULL_SLOT, equipSlot, pItem, !pItem->IsBag());
                if(msg != EQUIP_ERR_OK)
                    continue;
                if(!(equipSlot >= 0 && equipSlot < 19))
                    continue;

                //create a list if one doesn't already exist
                if(equip[equipSlot] == NULL)
                    equip[equipSlot] = new std::list<Item *>;

                std::list<Item *> *itemListForEqSlot = equip[equipSlot];
                itemListForEqSlot->push_back(pItem);
            }
        }
    }

    TellMaster("Here's all the items in my inventory that I can equip:");
    ChatHandler ch(m_master);

    const std::string descr[] = { "head", "neck", "shoulders", "body", "chest",
    "waist", "legs", "feet", "wrists", "hands", "finger1", "finger2",
    "trinket1", "trinket2", "back", "mainhand", "offhand", "ranged", "tabard" };

    //now send client all items that can be equipped by slot
    for(uint8 equipSlot = 0; equipSlot < 19; ++equipSlot)
    {
        if(equip[equipSlot] == NULL) continue;
        std::list<Item *> *itemListForEqSlot = equip[equipSlot];
        std::ostringstream out;
        out << descr[equipSlot] << ": ";
        for(std::list<Item *>::iterator it = itemListForEqSlot->begin(); it != itemListForEqSlot->end(); ++it)
        {
            const ItemPrototype *const pItemProto = (*it)->GetProto();
            out << " |cffffffff|Hitem:" << pItemProto->ItemId
            << ":0:0:0:0:0:0:0" << "|h[" << pItemProto->Name1
            << "]|h|r";
        }
        ch.SendSysMessage(out.str().c_str());
        delete itemListForEqSlot; //delete list of Item *
    }
}

void PlayerbotAI::HandleMasterOutgoingPacket(const WorldPacket &packet, WorldSession &masterSession)
{
    // Testing
    const char *oc = LookupOpcodeName(packet.GetOpcode());

    std::ostringstream out;
    out << "HandleMasterOutgoingPacket: " << oc;
    sLog->outError(out.str().c_str());
    // Testing
}

void PlayerbotAI::HandleMasterIncomingPacket(const WorldPacket &packet, WorldSession &masterSession)
{
    switch(packet.GetOpcode())
    {
		case CMSG_SET_SELECTION:
		{
			//sLog->outError("cmsg_set_selection");
			return;
		}

        //If master inspects one of his bots, give the master useful info in chat window
        //such as inventory that can be equipped
        case CMSG_INSPECT:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            uint64 guid; p >> guid;
            Player *const bot = masterSession.GetPlayerBot(guid);
            if(!bot) return;
            bot->GetPlayerbotAI()->SendNotEquipList(*bot);
        }

        case CMSG_PUSHQUESTTOPARTY:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            uint32 quest; p >> quest;
            Player *pPlayer = masterSession.GetPlayer();
            Quest const *pQuest = sObjectMgr->GetQuestTemplate(quest);

            if(pQuest)
                for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                {
                    Player *const bot = it->second;
                    uint64 guid = it->first;
                    uint32 unk1 = 0;

                    WorldPacket data(MSG_QUEST_PUSH_RESULT, (8+4+4));
                    //data << guid;
                    data << pPlayer->GetGUID();
                    data << quest;
                    data << unk1;

                    bot->GetSession()->HandleQuestgiverAcceptQuestOpcode(data);
                    bot->GetPlayerbotAI()->SetQuestNeedItems();
                }
            return;
        }

        //handle emotes from the master
        //case CMSG_EMOTE:
        case CMSG_TEXT_EMOTE:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            uint32 emoteNum;
//            uint64 guid;
            p >> emoteNum;

            switch(emoteNum)
            {
                case TEXTEMOTE_BONK:
                {
                    Player *const pPlayer = masterSession.GetPlayerBot(masterSession.GetPlayer()->GetSelection());
                    if(!pPlayer || !pPlayer->GetPlayerbotAI()) return;
                    PlayerbotAI *const pBot = pPlayer->GetPlayerbotAI();

                    ChatHandler ch(masterSession.GetPlayer());

                    {
                    std::ostringstream out;
                    out << "clock(): " << (getMSTime())
                    << " m_ignoreAIUpdatesUntilTime: " << (pBot->m_ignoreAIUpdatesUntilTime);
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "m_TimeDoneEating: " << pBot->m_TimeDoneEating
                    << " m_TimeDoneDrinking: " << pBot->m_TimeDoneDrinking;
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "m_CurrentlyCastingSpellId: " << pBot->m_CurrentlyCastingSpellId;
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "m_IsFollowingMaster: " << pBot->m_IsFollowingMaster;
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "IsBeingTeleported(): " << pBot->m_bot->IsBeingTeleported();
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    bool tradeActive = (pBot->m_bot->GetTrader()) ? true : false;
                    out << "tradeActive: " << tradeActive;
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "IsCharmed(): " << pBot->m_bot->isCharmed();
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "IsInCombat(): " << pBot->m_bot->isInCombat();
                    ch.SendSysMessage(out.str().c_str());
                    }{
                    std::ostringstream out;
                    out << "isLooting: " << pBot->isLooting;
                    ch.SendSysMessage(out.str().c_str());
                    }
                    {
                    std::ostringstream out;
                    out << "isPulling: " << pBot->GetClassAI()->isPulling();
                    ch.SendSysMessage(out.str().c_str());
                    }
                    return;
                }

                case TEXTEMOTE_EAT:
                case TEXTEMOTE_DRINK:
                {
                    Player *const bot = masterSession.GetPlayerBot(masterSession.GetPlayer()->GetSelection());
                    if(bot) bot->GetPlayerbotAI()->Stay();
                    else {
                        for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                        {
                            Player *const bot = it->second;
                            bot->GetPlayerbotAI()->Feast();
                        }
                    }
                    return;
                }

                //emote to stay
                case TEXTEMOTE_STAND:
                {
                    Player *const bot = masterSession.GetPlayerBot(masterSession.GetPlayer()->GetSelection());
                    if(bot) bot->GetPlayerbotAI()->Stay();
                    else {
                        for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                        {
                            Player *const bot = it->second;
                            bot->GetPlayerbotAI()->Stay();
                        }
                    }
                    return;
                }

                //324 is the followme emote (not defined in enum)
                //if master has bot selected then only bot follows, else all bots follow
                case 324:
                case TEXTEMOTE_WAVE:
                {
                    Player *const bot = masterSession.GetPlayerBot(masterSession.GetPlayer()->GetSelection());
                    if(bot)
                    {
                        bot->GetPlayerbotAI()->Follow(*masterSession.GetPlayer());
                        bot->GetPlayerbotAI()->SetLooting(false);
                    } else {
                        for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                        {
                            Player *const bot = it->second;
                            bot->GetPlayerbotAI()->Follow(*masterSession.GetPlayer());
                            bot->GetPlayerbotAI()->SetLooting(false);
                        }
                    }
                    return;
                }

                default: return;
            }
        } //end CMSG_TEXT_EMOTE

        case CMSG_GROUP_UNINVITE:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            std::string member; p >> member;
            p.clear();

            WorldPacket data(CMSG_GROUP_UNINVITE, 1);
            PlayerbotChatHandler ch(masterSession.GetPlayer());
            std::ostringstream out;
            out << "remove " << member;
            ch.uninvite(out.str().c_str());
            return;
        }

        case CMSG_REPAIR_ITEM:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            uint64 npcGUID;
            p >> npcGUID;

            Object *const pNpc = ObjectAccessor::GetObjectByTypeMask(*masterSession.GetPlayer(), npcGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
            if(!pNpc)
                return;

            //for all master's bots
            for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
            {
                Player *const bot = it->second;
                if(!bot->IsInMap((WorldObject*) pNpc))
                {
                    bot->GetPlayerbotAI()->TellMaster("I'm too far away to repair items!");
                    continue;
                } else {
                    bot->GetPlayerbotAI()->TellMaster("Repairing my items.");
                    bot->DurabilityRepairAll(false, 0.0, false);
                }

            }
            return;
        }

        case CMSG_ACTIVATETAXIEXPRESS:
        {
            WorldPacket incP(packet);
            //for all master's bots
            for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
            {

                Player *const bot = it->second;
                if (!bot->GetPlayerbotAI()->CanBotsFly()) return;

                if(!bot->IsInMap((WorldObject*) masterSession.GetPlayer()))
                {
                    bot->GetPlayerbotAI()->TellMaster("I'm too far away to fly!");
                    continue;
                } else {
                    WorldPacket p;
                   // p << guid << _totalcost << node_count;
                    bot->GetPlayerbotAI()->Stay();  // clear any movement
                    incP.rpos(0);
                    bot->GetSession()->HandleActivateTaxiExpressOpcode(incP);
                }

            }
            return;
        }

        case CMSG_ACTIVATETAXI:
        {
            WorldPacket incP(packet);
            //for all master's bots
            for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
            {
                Player *const bot = it->second;
                if (!bot->GetPlayerbotAI()->CanBotsFly()) return;

                if(!bot->IsInMap((WorldObject*) masterSession.GetPlayer()))
                {
                    bot->GetPlayerbotAI()->TellMaster("I'm too far away to fly!");
                    continue;
                } else {
                    WorldPacket p;
                   // p << guid << nodes[0] << nodes[1];
                    bot->GetPlayerbotAI()->Stay();  // clear any movement
                    incP.rpos(0);
                    bot->GetSession()->HandleActivateTaxiOpcode(incP);
                }

            }
            return;
        }

        // when landing from a flight path
        case CMSG_MOVE_SPLINE_DONE:
        {
            WorldPacket p(packet);
            //p.rpos(0);  // reset reader

            //for all master's bots
            for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
            {
                Player *const bot = it->second;

                if(!bot->IsInMap((WorldObject*) masterSession.GetPlayer()))
                {
                    bot->GetPlayerbotAI()->TellMaster("I'm too far away to land!");
                    continue;
                } else {
                    p.rpos(0);  // reset reader
                    p.appendPackGUID(bot->GetGUID());
                    bot->GetSession()->HandleMoveSplineDoneOpcode(p);
                    uint32 sourcenode = bot->m_taxi.GetTaxiSource();
                    uint32 mountDisplayId = sObjectMgr->GetTaxiMountDisplayId(sourcenode, bot->GetTeam());

                    if (mountDisplayId==0) {
                        bot->CleanupAfterTaxiFlight();
                        bot->GetPlayerbotAI()->Follow(*masterSession.GetPlayer());
                    }
                }

            }
            return;
        }
        case CMSG_LOOT:
            {
                WorldPacket p(packet);
                p.rpos(0); // reset reader
                uint64 cGUID;
                p >> cGUID;

                Player *m_master = masterSession.GetPlayer();
                Creature *cToLoot = m_master->GetMap()->GetCreature(cGUID);
                if (!cToLoot)
                    return;
                /* for all master's bots */
                for (PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin();
                    it != masterSession.GetPlayerBotsEnd(); ++it)
                {
                    Player* const bot = it->second;
                    if (!bot->IsInMap((WorldObject*) cToLoot))
                    {
                        bot->GetPlayerbotAI()->TellMaster("I'm too far away to check for needed Quest Items!");
                        continue;
                    }
                    else
                    {
                        //bot->GetPlayerbotAI()->TellMaster("Checking for needed Quest Items.");
                        bot->GetPlayerbotAI()->AddLootGUID(cGUID);
                        bot->GetPlayerbotAI()->DoLoot();
                    }
                }
                return;

            }
            break;

        case CMSG_GAMEOBJ_USE:
            {
                WorldPacket p(packet);
                p.rpos(0); // reset reader
                uint64 objGUID;
                p >> objGUID;

                Player *m_master = masterSession.GetPlayer();
                GameObject *obj = m_master->GetMap()->GetGameObject( objGUID );
                if( !obj )
                    return;

               for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                {
                    Player* const bot = it->second;
//sLog->outError ("gameobject type = %u", obj->GetGoType());
                    if( obj->GetGoType() == GAMEOBJECT_TYPE_QUESTGIVER )
                    {
                        bot->GetPlayerbotAI()->TurnInQuests( obj );
                    }
                    // add other go types here, i.e.:
                    // GAMEOBJECT_TYPE_CHEST - loot quest items of chest
                }
            }
            break;

        case CMSG_GAMEOBJ_REPORT_USE:
            {
                WorldPacket p(packet);
                p.rpos(0); // reset reader
                uint64 objGUID;
                p >> objGUID;

                Player *m_master = masterSession.GetPlayer();
                GameObject *obj = m_master->GetMap()->GetGameObject( objGUID );
                if( !obj )
                    return;

                //Object* const pNpc = ObjectAccessor::GetObjectByTypeMask(*masterSession.GetPlayer(), npcGUID, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
                //if (!pNpc) return;
            /* For all master's bots */
            for (PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin();
                it != masterSession.GetPlayerBotsEnd(); ++it)
            {
                Player* const bot = it->second;
                if (!bot->IsInMap((WorldObject*) obj))
                {
                    bot->GetPlayerbotAI()->TellMaster("I'm too far away to check for needed Quest Items!");
                    continue;
                }
                else
                {
                    //bot->GetPlayerbotAI()->TellMaster("Checking for needed Quest Items.");
                    bot->GetPlayerbotAI()->AddLootGUID(objGUID);
                    bot->GetPlayerbotAI()->DoLoot();
                }
            }
            return;
        }

        //if master talks to an NPC
        case CMSG_GOSSIP_HELLO:
        case CMSG_QUESTGIVER_HELLO:
            {
                WorldPacket p(packet);
                p.rpos(0); //reset reader
                uint64 npcGUID;
                p >> npcGUID;

                Player *m_master = masterSession.GetPlayer();
                //Object *const pNpc = ObjectAccessor::GetObjectByTypeMask(*masterSession.GetPlayer(), npcGUID, TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT);
                WorldObject* pNpc = ObjectAccessor::GetWorldObject( *m_master, npcGUID );
                if(!pNpc)
                    return;

                // if its a flight master
                if(pNpc->HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_FLIGHTMASTER ))
                {
                    for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                    {
                        Player *const bot = it->second;
                        if (bot->GetSession()->SendLearnNewTaxiNode((Creature*)pNpc))
                            bot->GetPlayerbotAI()->TellMaster("Learned a new path.");
                    }
                    return;
                }

                // for all master's bots
                for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                {
                    Player* const bot = it->second;
                    bot->GetPlayerbotAI()->TurnInQuests( pNpc );
                    bot->GetPlayerbotAI()->SetQuestNeedItems();

                    bot->TalkedToCreature(pNpc->GetEntry(), pNpc->GetGUID());
                }

                return;
        }

        case CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY:
        {
            WorldPacket p(packet);
            p.rpos(0); // reset reader
           if (!masterSession.GetPlayer()->GetSelection()) return;

            //for all master's bots
            for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
            {
                Player* const bot = it->second;
                p.rpos(0); // reset reader
                bot->GetSession()->HandleQuestgiverStatusMultipleQuery(p);
            }


        return;
        }

        // if master accepts a quest, bots should also try to accept quest
        case CMSG_QUESTGIVER_ACCEPT_QUEST:
        {
            WorldPacket p(packet);
            p.rpos(0); // reset reader
            uint64 guid;
            uint32 quest;
            p >> guid >> quest;
            Quest const* qInfo = sObjectMgr->GetQuestTemplate(quest);
            if (qInfo)
            {
                //for all master's bots
                for(PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin(); it != masterSession.GetPlayerBotsEnd(); ++it)
                {
                    Player* const bot = it->second;

                    if (bot->GetQuestStatus(quest) == QUEST_STATUS_COMPLETE)
                        bot->GetPlayerbotAI()->TellMaster("I already completed that quest.");
                    else if (! bot->CanTakeQuest(qInfo, false))
                    {
                        if (! bot->SatisfyQuestStatus(qInfo, false))
                            bot->GetPlayerbotAI()->TellMaster("I already have that quest.");
                        else
                            bot->GetPlayerbotAI()->TellMaster("I can't take that quest.");
                    }
                    else if (! bot->SatisfyQuestLog(false))
                        bot->GetPlayerbotAI()->TellMaster("My quest log is full.");
                    else if (! bot->CanAddQuest(qInfo, false))
                        bot->GetPlayerbotAI()->TellMaster("I can't take that quest because it requires that I take items, but my bags are full!");

                    else
                    {
                        p.rpos(0); // reset reader
                        bot->GetSession()->HandleQuestgiverAcceptQuestOpcode(p);
                        bot->GetPlayerbotAI()->TellMaster("Got the quest.");
                        bot->GetPlayerbotAI()->SetQuestNeedItems();
                    }
                }
            }

            return;
        }

        case CMSG_LIST_INVENTORY:
            {
                uint64 npcGUID;
                WorldPacket p1(packet);
                p1.rpos(0); /* reset reader */
                p1 >> npcGUID;

                Object* const pNpc = ObjectAccessor::GetObjectByTypeMask(*masterSession.GetPlayer(), npcGUID, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
                if (!pNpc) return;

                /* for all master's bots */
                for (PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin();
                    it != masterSession.GetPlayerBotsEnd(); ++it)
                {
                    Player* const bot = it->second;
                    if (!bot->IsInMap((WorldObject*) pNpc))
                    {
                        bot->GetPlayerbotAI()->TellMaster("I'm too far away to sell items!");
                        continue;
                    }
                    else
                    {
                        uint32 TotalCost = 0;
                        uint32 TotalSold = 0;
                        std::ostringstream report;
                        std::ostringstream canSell;
                        canSell << "Items that are not trash and can be sold: ";
                        // list out items in main backpack
                        for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
                        {
                            Item* const item = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
                            if (!item)
                                continue;

                            if (item->CanBeTraded() && item->GetProto()->Quality == ITEM_QUALITY_POOR)
                            {
                                int32 cost = item->GetCount() * item->GetProto()->SellPrice;
                                bot->ModifyMoney(cost);
                                bot->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);
                                bot->AddItemToBuyBackSlot(item);

                                TotalSold = TotalSold + 1;
                                TotalCost = TotalCost + cost;

                                report << "Sold " << item->GetCount() << "x";
                                report << " |cffffffff|Hitem:" << item->GetProto()->ItemId << ":0:0:0:0:0:0:0" << "|h[" << item->GetProto()->Name1 << "]|h|r";
                                report << " for ";

                                uint32 gold = uint32(cost / 10000);
                                cost -= (gold * 10000);
                                uint32 silver = uint32(cost / 100);
                                cost -= (silver * 100);

                                if (gold > 0)
                                {
                                    report << gold << "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
                                }
                                if (silver > 0)
                                {
                                    report << silver << "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
                                }
                                report << cost << "|TInterface\\Icons\\INV_Misc_Coin_05:16|t\n";
                            }
                            else if (item->GetProto()->SellPrice > 0)
                            {
                                canSell << "|cffffffff|Hitem:" << item->GetProto()->ItemId << ":0:0:0:0:0:0:0" << "|h[" << item->GetProto()->Name1 << "]|h|r ";
                            }
                        }

                        for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END;++bag)
                        {
                            const Bag* const pBag = (Bag*) bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
                            if (pBag)
                            {
                                for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
                                {
                                    Item* const item = bot->GetItemByPos(bag, slot);
                                    if (!item)
                                        continue;

                                    if (item->CanBeTraded() && item->GetProto()->Quality == ITEM_QUALITY_POOR)
                                    {
                                        int32 cost = item->GetCount() * item->GetProto()->SellPrice;
                                        bot->ModifyMoney(cost);
                                        bot->MoveItemFromInventory(item->GetBagSlot(), item->GetSlot(), true);
                                        bot->AddItemToBuyBackSlot(item);

                                        TotalSold = TotalSold + 1;
                                        TotalCost = TotalCost + cost;

                                        report << "Sold " << item->GetCount() << "x";
                                        report << " |cffffffff|Hitem:" << item->GetProto()->ItemId << ":0:0:0:0:0:0:0" << "|h[" << item->GetProto()->Name1 << "]|h|r";
                                        report << " for ";

                                        uint32 gold = uint32(cost / 10000);
                                        cost -= (gold * 10000);
                                        uint32 silver = uint32(cost / 100);
                                        cost -= (silver * 100);
                                        if (gold > 0)
                                        {
                                            report << gold << "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
                                        }
                                        if (silver > 0)
                                        {
                                            report << silver << "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
                                        }
                                        report << cost << "|TInterface\\Icons\\INV_Misc_Coin_05:16|t\n";
                                    }
                                    else if (item->GetProto()->SellPrice > 0)
                                    {
                                        canSell << "|cffffffff|Hitem:" << item->GetProto()->ItemId << ":0:0:0:0:0:0:0" << "|h[" << item->GetProto()->Name1 << "]|h|r ";
                                    }
                                }
                            }
                        }
                        if (TotalSold > 0) {
                            report << "Sold total " << TotalSold << " item(s) for ";
                            uint32 gold = uint32(TotalCost / 10000);
                            TotalCost -= (gold * 10000);
                            uint32 silver = uint32(TotalCost / 100);
                            TotalCost -= (silver * 100);
                            if (gold > 0)
                            {
                                report << gold << "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
                            }
                            if (silver > 0)
                            {
                                report << silver << "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
                            }
                            report << TotalCost << "|TInterface\\Icons\\INV_Misc_Coin_05:16|t";
                            bot->GetPlayerbotAI()->TellMaster(report.str());
                        }
                        bot->GetPlayerbotAI()->TellMaster(canSell.str());
                    }
                }
                return;

            }

        case CMSG_AREATRIGGER:
        {
            uint32 Trigger_ID;
            WorldPacket p1(packet);
            p1.rpos(0); /* reset reader */
            p1 >> Trigger_ID;

            /* for all master's bots */
            for (PlayerBotMap::const_iterator it = masterSession.GetPlayerBotsBegin();
                it != masterSession.GetPlayerBotsEnd(); ++it)
            {
                Player* const bot = it->second;

                uint32 quest_id = sObjectMgr->GetQuestForAreaTrigger( Trigger_ID );
                // The conditions that intentionally left unchecked are:
                // Bot is alive or not
                // Bot is in the trigger area or not
                if( quest_id && bot->IsActiveQuest(quest_id) )
                {
                    Quest const* pQuest = sObjectMgr->GetQuestTemplate(quest_id);
                    if( pQuest )
                    {
                        if(bot->GetQuestStatus(quest_id) == QUEST_STATUS_INCOMPLETE)
                        {
                            bot->AreaExploredOrEventHappens( quest_id );
                            bot->GetPlayerbotAI()->TellMaster("Quest area explored");
                        }
                    }
                }

                if(sObjectMgr->IsTavernAreaTrigger(Trigger_ID))
                {
                    // set resting flag we are in the inn
                    bot->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
                    bot->InnEnter(time(NULL), bot->GetMapId() , bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ());
                    bot->SetRestType(REST_TYPE_IN_TAVERN);

                    if(sWorld->IsFFAPvPRealm())
                        bot->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
                }
            }
            return;
        }


        default:
        {
			const char *oc = LookupOpcodeName(packet.GetOpcode());
            ChatHandler ch(masterSession.GetPlayer());
            ch.SendSysMessage(oc);

            std::ostringstream out;
            out << "HandleMasterIncomingPacket: " << oc;
            sLog->outError(out.str().c_str());

            return;
        }

    }
}

//handle outgoing packets the server would send to the client
void PlayerbotAI::HandleBotOutgoingPacket(const WorldPacket &packet)
{
    switch(packet.GetOpcode())
    {
         case SMSG_TRADE_STATUS_EXTENDED:
         {
             //m_bot->GetSession()->SendTradeStatus(TRADE_STATUS_TRADE_ACCEPT);
             return;
         }


        case SMSG_DUEL_WINNER:
            m_bot->HandleEmoteCommand(EMOTE_ONESHOT_APPLAUD);
            return;

        case SMSG_DUEL_COMPLETE:
            SetIgnoreUpdateTime(4);
            m_combatOrder = ORDERS_NONE;
            m_ScenarioType = SCENARIO_PVEEASY;
            m_bot->GetMotionMaster()->Clear(true);
            return;

        case SMSG_DUEL_OUTOFBOUNDS:
            m_bot->HandleEmoteCommand(EMOTE_ONESHOT_CHICKEN);
            return;

        case SMSG_DUEL_REQUESTED:
        {
            SetIgnoreUpdateTime(0);
            WorldPacket p(packet);
            uint64 flagGuid; p >> flagGuid;
            uint64 playerGuid; p >> playerGuid;
            Player *const pPlayer = ObjectAccessor::FindPlayer(playerGuid);
            if(canObeyCommandFrom(*pPlayer))
            {
                m_bot->GetMotionMaster()->Clear(true);
                WorldPacket *const packet = new WorldPacket(CMSG_DUEL_ACCEPTED, 8);
                *packet << flagGuid;
                m_bot->GetSession()->QueuePacket(packet); //queue the packet to get around race condition

                //follow target in casting range
                float angle = rand_norm()*M_PI; //Generates random float between 0 and 3.14
                float dist = (float)(urand((m_followDistanceMin*10), (m_followDistanceMax*10))/10); //Using urand to get a random float is stupid. It takes uint32, not float.
                m_bot->GetMotionMaster()->Clear(true);
                m_bot->GetMotionMaster()->MoveFollow(pPlayer, dist, angle);

                m_bot->SetSelection(playerGuid);
                SetIgnoreUpdateTime(4);
                m_combatOrder = ORDERS_KILL;
                m_ScenarioType = SCENARIO_DUEL;
            }
            return;
        }

        case SMSG_INVENTORY_CHANGE_FAILURE:
            TellMaster("I can't use that.");
            return;

        case SMSG_SPELL_DELAYED:
        {
            //sLog->outDebug("Bot [%u] SMSG_SPELL_DELAYED",m_bot->GetGUIDLow());
            WorldPacket p(packet);
            //uint64 casterGuid = extractGuid(p); //somehow the caster guid is corrupt
            //if(casterGuid != m_bot->GetGUID()) return;
            //uint32 delayTime; p >> delayTime;
            //sLog->outDebug("Bot [%u] caster [%u] Spell Delayed [%u]",m_bot->GetGUIDLow(), casterGuid, delayTime);
            //m_ignoreAIUpdatesUntilTime +=  ((((float)delayTime) / 1000.0f ) + 0.1f) * CLOCKS_PER_SEC;
            if(m_CurrentlyCastingSpellId > 0)
            {
                m_ignoreAIUpdatesUntilTime += 0.5f * 1000; //Until this is handled correctly, assume, delay is the default 0.5 secs
            }
            return;
        }

        case SMSG_SPELL_FAILURE:
        {
            WorldPacket p(packet);
            uint64 casterGuid = extractGuid(p);
            if(casterGuid != m_bot->GetGUID()) return;
            uint32 spellId; p >> spellId;
            if(m_CurrentlyCastingSpellId == spellId)
            {
                SetIgnoreUpdateTime(1);
                m_CurrentlyCastingSpellId = 0;
            }
            return;
        }

        //if a change in speed was detected for the master
        //make sure we have the same mount status
        case SMSG_FORCE_RUN_SPEED_CHANGE:
        {
            WorldPacket p(packet);
            uint64 guid = extractGuid(p);
            //uint64 guid; p >> guid;
            Player *tPlayer = sObjectMgr->GetPlayer(guid);
            if(!tPlayer) return;
            if (!m_master || !m_bot) return;
            if(guid == m_bot->GetGUID()) return;
            if(guid == m_master->GetGUID()) {
                m_bot->GetPlayerbotAI()->UseMount();
                SetIgnoreUpdateTime(2);
            }
            return;
        }

        //handle flying acknowledgement
        case SMSG_MOVE_SET_CAN_FLY:
        {
            WorldPacket p(packet);
            uint64 guid = extractGuid(p);
            if(guid != m_bot->GetGUID()) return;
            m_bot->AddUnitMovementFlag(MOVEMENTFLAG_FLYING);
            //SetSpeed(MOVE_RUN, m_master->GetSpeed(MOVE_FLIGHT) + 0.1f, true);
            return;
        }

        //handle dismount flying acknowledgement
        case SMSG_MOVE_UNSET_CAN_FLY:
        {
            WorldPacket p(packet);
            uint64 guid = extractGuid(p);
            if(guid != m_bot->GetGUID()) return;
            m_bot->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING);
            //SetSpeed(MOVE_RUN, m_master->GetSpeedRate(MOVE_RUN), true);
            return;
        }

        //If the leader role was given to the bot automatically give it to the master
        //if the master is in the group, otherwise leave group
        case SMSG_GROUP_SET_LEADER:
        {
            WorldPacket p(packet);
            std::string name; p >> name;
            if(m_bot->GetGroup() && name == m_bot->GetName())
            {
                if(m_bot->GetGroup()->IsMember(m_master->GetGUID()))
                {
                    p.resize(8);
                    p << m_master->GetGUID();
                    m_bot->GetSession()->HandleGroupSetLeaderOpcode(p);
                } else {
                    p.clear(); //not really needed
                    m_bot->GetSession()->HandleGroupDisbandOpcode(p); //packet not used
                }
            }
            return;
        }

        //If the master leaves the group, then the bot leaves too
        case SMSG_PARTY_COMMAND_RESULT:
        {
            WorldPacket p(packet);
            uint32 operation; p >> operation;
            std::string member; p >> member;
            uint32 result; p >> result;
            p.clear();
            if(operation == PARTY_OP_LEAVE && member == m_master->GetName()) m_bot->GetSession()->HandleGroupDisbandOpcode(p); //packet not used
            return;
        }

        //Automatically accept rez. Useful when bot dies, and a druid does a battle rez.
        case SMSG_RESURRECT_REQUEST:
        {
            WorldPacket p, incP(packet);
            uint8 status = 1;
            uint64 rezzer; incP >> rezzer;
            p << rezzer;
            p << status;
            m_bot->GetPlayerbotAI()->SetLooting(false);
            m_bot->GetSession()->HandleResurrectResponseOpcode(p);
            m_IsFollowingMaster = true;
            m_TimeRessurect = 0;
            return;
        }

        //Handle Group invites (auto accept if master is in group, otherwise decline & send message)
        case SMSG_GROUP_INVITE:
        {
            if(m_bot->GetGroupInvite())
            {
                const Group *const grp = m_bot->GetGroupInvite();
                if(!grp) return;
                Player *const inviter = sObjectMgr->GetPlayer(grp->GetLeaderGUID());
                if(!inviter) return;
                WorldPacket p;
                if(!canObeyCommandFrom(*inviter))
                {
                    std::string buf = "I can't accept your invite unless you first invite my master ";
                    buf += m_master->GetName();
                    buf += ".";
                    SendWhisper(buf, *inviter);
                    m_bot->GetSession()->HandleGroupDeclineOpcode(p); //packet not used
                } else
                    m_bot->GetSession()->HandleGroupAcceptOpcode(p); //packet not used
            }
            return;
        }

        //Handle when another player opens the trade window with the bot
        //also sends list of tradable items bot can trade if bot is allowed to obey commands from
        case SMSG_TRADE_STATUS:
        {
            if(m_bot->GetTrader() == NULL) break;
            WorldPacket p(packet);
            uint32 status; p >> status;
            p.clear();

            if(status == 4) { // TRADE_STATUS_TRADE_ACCEPT
				if (!m_bot->GetTradeData()->IsAccepted() || !m_bot->GetTrader()->GetTradeData()->IsAccepted()) {
                    m_bot->GetSession()->HandleAcceptTradeOpcode(p); //packet not used
                }

            } else if(status == 1) // TRADE_STATUS_BEGIN_TRADE
            {
                m_bot->GetSession()->HandleBeginTradeOpcode(p); //packet not used

                //if(!canObeyCommandFrom(*(m_bot->GetTrader())))
                //{
                    //SendWhisper("I'm not allowed to trade you any of my items, but you are free to give me money or items.", *(m_bot->GetTrader()));
                    //return;
                //}

                //list out items available for trade
                std::ostringstream out;

                //list out items in main backpack
                for(uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
                {
                    const Item *const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
                    if(pItem && pItem->CanBeTraded())
                    {
                        const ItemPrototype *const pItemProto = pItem->GetProto();
                        std::string name = pItemProto->Name1;

                        out << " |cffffffff|Hitem:" << pItemProto->ItemId << ":0:0:0:0:0:0:0" << "|h[" << name << "]|h|r";
                        if(pItem->GetCount() > 1)
                            out << "x" << pItem->GetCount() << ' ';
                    }
                }
                //list out items in other removable backpacks
                for(uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
                {
                    const Bag *const pBag = (Bag *)m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
                    if(pBag)
                    {
                        for(uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
                        {
                            const Item *const pItem = m_bot->GetItemByPos(bag, slot);
                            if(pItem && pItem->CanBeTraded())
                            {
                                const ItemPrototype *const pItemProto = pItem->GetProto();
                                const std::string name = pItemProto->Name1;

                                //item link format: http://www.wowwiki.com/ItemString
                                //itemId, enchantId, jewelId1, jewelId2, jewelId3, jewelId4, suffixId, uniqueId
                                out << " |cffffffff|Hitem:" << pItemProto->ItemId << ":0:0:0:0:0:0:0" << "|h[" << name << "]|h|r";
                                if(pItem->GetCount() > 1)
                                    out << "x" << pItem->GetCount() << ' ';
                            }
                        }
                    }
                }

                //calculate how much money bot has
                uint32 copper = m_bot->GetMoney();
                uint32 gold = uint32(copper / 10000);
                copper -= (gold * 10000);
                uint32 silver = uint32(copper / 100);
                copper -= (silver * 100);

                //send bot the message
                std::ostringstream whisper;
                whisper << "I have |cff00ff00" << gold
                << "|r|cfffffc00g|r|cff00ff00" << silver
                << "|r|cffcdcdcds|r|cff00ff00" << copper
                << "|r|cffffd333c|r" << " and the following items:";
                m_bot->GetPlayerbotAI()->SendWhisper(whisper.str(), *(m_bot->GetTrader()));
                ChatHandler ch(m_bot->GetTrader());
                ch.SendSysMessage(out.str().c_str());
            }
            return;
        }

        case SMSG_SPELL_GO:
        {
            WorldPacket p(packet);
            uint64 castItemGuid = extractGuid(p);
            uint64 casterGuid = extractGuid(p);
            if(casterGuid != m_bot->GetGUID()) return;

            uint32 spellId; p >> spellId;
            uint16 castFlags; p >> castFlags;
            uint32 msTime; p >> msTime;
            uint8 numHit; p >> numHit;
            if(m_CurrentlyCastingSpellId == spellId)
            {
                Spell *const pSpell = m_bot->FindCurrentSpellBySpellId(spellId);
                if(!pSpell) return;
                if(pSpell->IsChannelActive() || pSpell->IsAutoRepeat())
                    SetIgnoreUpdateTime( (((float)GetSpellDuration(pSpell->m_spellInfo) / 1000.0f) + 1.0f) );
                else if(pSpell->IsAutoRepeat())
                    SetIgnoreUpdateTime(6);
                else {
                    SetIgnoreUpdateTime(0.5f);
                    m_CurrentlyCastingSpellId = 0;
                }
            }
            return;
        }

        case SMSG_TEXT_EMOTE:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            uint32 text_emote;
            uint64 guid;
            p >> guid;
            p >> text_emote;

            switch(text_emote)
            {
                case TEXTEMOTE_BOW:
                {
                    //Buff anyone who bows before me. Useful for players not in bot's group
                    Player *pPlayer = sObjectMgr->GetPlayer(guid);

                    Player *const bot =sObjectMgr->GetPlayer(pPlayer->GetSelection());

                    if(bot && bot->GetGUID()==m_bot->GetGUID() &&
                       bot->GetPlayerbotAI()->GetClassAI())
                    {
                        bot->GetPlayerbotAI()->GetClassAI()->BuffPlayer(pPlayer);
                    }
                    return;
                }

                default:
                    m_bot->HandleEmoteCommand(text_emote);
                    return;
            }
            return;
        }

        case MSG_MOVE_TELEPORT_ACK:
            HandleTeleportAck();
            return;

        case SMSG_QUESTGIVER_STATUS_MULTIPLE:
        {
            return;
        }

        // used to communicate between bots
        case SMSG_MESSAGECHAT:
        {
            WorldPacket p(packet);
            p.rpos(0); //reset reader
            uint8 msgtype;
            uint32 language;
            uint64 guid;
            uint32 language2;
            uint64 guid2;
            uint32 textlen;
            std::string msg;

            p>>msgtype; p>>language; p>>guid; p>>language2; p>>guid2; p>>textlen;
            p>>msg;

            Player * fromPlayer = sObjectMgr->GetPlayer(guid);
            if (fromPlayer == NULL) break;
            const std::string text = msg;
            HandleCommand(text, *fromPlayer);
        }

        case SMSG_MONSTER_MOVE:
        case SMSG_UPDATE_WORLD_STATE:
        case SMSG_COMPRESSED_UPDATE_OBJECT:
        case MSG_MOVE_SET_FACING:
        case MSG_MOVE_STOP:
        case MSG_MOVE_HEARTBEAT:
        case MSG_MOVE_STOP_STRAFE:
        case MSG_MOVE_START_STRAFE_LEFT:
        case SMSG_UPDATE_OBJECT:
        case MSG_MOVE_START_FORWARD:
        case SMSG_WEATHER:
        case SMSG_POWER_UPDATE:
        case SMSG_TIME_SYNC_REQ:
        case SMSG_STANDSTATE_UPDATE:
        case SMSG_PERIODICAURALOG:
        case SMSG_AURA_UPDATE:
        return;

// TESTING
        default:
        const char *oc = LookupOpcodeName(packet.GetOpcode());
        TellMaster(oc);
        sLog->outError("SMSG opcode: %s", oc);
  // TESTING
    }
}
void PlayerbotAI::HandleTeleportAck()
{
    SetIgnoreUpdateTime(6);
    m_bot->GetMotionMaster()->Clear(true);
    if(m_bot->IsBeingTeleportedNear())
    {
        WorldPacket p = WorldPacket(MSG_MOVE_TELEPORT_ACK, 8 + 4 + 4);
        p.appendPackGUID(m_bot->GetGUID());
        p << (uint32) 0; //supposed to be flags? not used currently
        p << (uint32) time(0); //time - not currently used
        m_bot->GetSession()->HandleMoveTeleportAck(p);
    }
    else if(m_bot->IsBeingTeleportedFar())
        m_bot->GetSession()->HandleMoveWorldportAckOpcode();
}

uint8 PlayerbotAI::GetHealthPercent(const Unit &target) const
{
    return(static_cast<float>(target.GetHealth()) / target.GetMaxHealth()) * 100;
}

uint8 PlayerbotAI::GetHealthPercent() const
{
    return GetHealthPercent(*m_bot);
}

uint8 PlayerbotAI::GetManaPercent(const Unit &target) const
{
    return(static_cast<float>(target.GetPower(POWER_MANA)) / target.GetMaxPower(POWER_MANA)) * 100;
}

uint8 PlayerbotAI::GetManaPercent() const
{
    return GetManaPercent(*m_bot);
}

uint8 PlayerbotAI::GetBaseManaPercent(const Unit &target) const
{
    if(target.GetPower(POWER_MANA) >= target.GetCreateMana())
        return(100);
    else
        return(static_cast<float>(target.GetPower(POWER_MANA)) / target.GetCreateMana()) * 100;
}

uint8 PlayerbotAI::GetBaseManaPercent() const
{
    return GetBaseManaPercent(*m_bot);
}

uint8 PlayerbotAI::GetRageAmount(const Unit &target) const
{
    return(static_cast<float>(target.GetPower(POWER_RAGE)));
}

uint8 PlayerbotAI::GetRageAmount() const
{
    return GetRageAmount(*m_bot);
}

uint8 PlayerbotAI::GetEnergyAmount(const Unit &target) const
{
    return(static_cast<float>(target.GetPower(POWER_ENERGY)));
}

uint8 PlayerbotAI::GetEnergyAmount() const
{
    return GetEnergyAmount(*m_bot);
}

uint8 PlayerbotAI::GetRunicPower(const Unit &target) const
{
    return(static_cast<float>(target.GetPower(POWER_RUNIC_POWER)));
}

uint8 PlayerbotAI::GetRunicPower() const
{
    return GetRunicPower(*m_bot);
}


typedef std::pair<uint32, uint8> spellEffectPair;
typedef std::multimap< spellEffectPair, Aura*> AuraMap;

bool PlayerbotAI::HasAura(uint32 spellId, const Unit *player) const
{
    for(Unit::AuraMap::const_iterator iter = player->GetOwnedAuras().begin(); iter != player->GetOwnedAuras().end(); ++iter)
    {
        if(iter->second->GetId() == spellId) return true;
    }
    return false;
}
bool PlayerbotAI::HasAura(const char *spellName) const
{
    return HasAura(spellName, m_bot);
}
bool PlayerbotAI::HasAura(const char *spellName, const Unit *player) const
{
    uint32 spellId = getSpellId(spellName);
    return(spellId) ? HasAura(spellId, player) : false;
}

void PlayerbotAI::UseMount() const
{

    if(m_master->IsMounted() && ! m_bot->IsMounted())
    {
// sLog->outError ("PlayerbotAI::UseMount: %s is mounted but %s is not", m_master->GetName(), m_bot->GetName());
        //Player Part
        int32 master_speed1 = 0;
        int32 master_speed2 = 0;
        if(!m_master->GetAuraEffectsByType(SPELL_AURA_MOUNTED).empty())
        {
            master_speed1 = m_master->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetSpellProto()->EffectBasePoints[1];
            master_speed2 = m_master->GetAuraEffectsByType(SPELL_AURA_MOUNTED).front()->GetSpellProto()->EffectBasePoints[2];
        }
//sLog->outError ("master_speed1 = %d", master_speed1);
//sLog->outError ("master_speed2 = %d", master_speed2);
        //Bot Part
        uint32 spellMount = 0;
        for(PlayerSpellMap::iterator itr = m_bot->GetSpellMap().begin(); itr != m_bot->GetSpellMap().end(); ++itr)
        {
            uint32 spellId = itr->first;
            if(itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled || IsPassiveSpell(spellId))
                continue;
            const SpellEntry *pSpellInfo = sSpellStore.LookupEntry(spellId);
            if(!pSpellInfo)
                continue;
            if(pSpellInfo->EffectApplyAuraName[0] == SPELL_AURA_MOUNTED)
            {
                if((pSpellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED)
                    && (pSpellInfo->EffectApplyAuraName[2] == SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                {
                    if((pSpellInfo->EffectBasePoints[1] == master_speed1)
                        && (pSpellInfo->EffectBasePoints[2] == master_speed2))
                    {
                        spellMount = spellId;
                        break;
                    }
                }
                else if((pSpellInfo->EffectApplyAuraName[2] == SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED)
                    && (pSpellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED))
                {
                    if((pSpellInfo->EffectBasePoints[2] == master_speed2)
                        && (pSpellInfo->EffectBasePoints[1] == master_speed1))
                    {
                        spellMount = spellId;
                        break;
                    }
                }
                else if(pSpellInfo->EffectApplyAuraName[1] == SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED)
                {
                    if(pSpellInfo->EffectBasePoints[1] == master_speed1 && master_speed2 <= 0) { spellMount = spellId; break; } //Has no secondary mount aura
                    else if (spellMount == 0) { spellMount = spellId; } // default to first mount in case it doesnt have correct version
                }
            }
        }
//sLog->outError ("spellMount = %u", spellMount);
        if(spellMount > 0) m_bot->GetPlayerbotAI()->CastSpell(spellMount, m_bot);

    }
    else if(!m_master->IsMounted() && m_bot->IsMounted())
    {
        WorldPacket emptyPacket;
        m_bot->GetSession()->HandleCancelMountAuraOpcode(emptyPacket);
    }
} //end UseMount

Item *PlayerbotAI::FindFood() const
{
    // list out items in main backpack
    for (uint8 slot=INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item* const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (pItem)
        {
            const ItemPrototype* const pItemProto = pItem->GetProto();
            if (! pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
            if (pItemProto->Class==ITEM_CLASS_CONSUMABLE &&
                (pItemProto->SubClass==ITEM_SUBCLASS_FOOD ||
                pItemProto->SubClass==ITEM_SUBCLASS_POTION ||
                pItemProto->SubClass==ITEM_SUBCLASS_ELIXIR))
            {
                // if is FOOD
                if (pItemProto->Spells[0].SpellCategory == SPELL_CATEGORY_FOOD)
                    return pItem;
            }
        }
    }
    // list out items in other removable backpacks
    for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        const Bag* const pBag = (Bag*) m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if (pBag)
        {
            for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                Item* const pItem = m_bot->GetItemByPos(bag, slot);
                if (pItem)
                {
                    const ItemPrototype* const pItemProto = pItem->GetProto();
                    if (! pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
                    if (pItemProto->Class==ITEM_CLASS_CONSUMABLE &&
                        (pItemProto->SubClass==ITEM_SUBCLASS_FOOD ||
                        pItemProto->SubClass==ITEM_SUBCLASS_POTION ||
                        pItemProto->SubClass==ITEM_SUBCLASS_ELIXIR))
                    {
                        // if is FOOD
                        if (pItemProto->Spells[0].SpellCategory == SPELL_CATEGORY_FOOD)
                            return pItem;
                    }
                }
            }
        }
    }
    return NULL;
}

Item *PlayerbotAI::FindDrink() const
{
    // list out items in main backpack
    for (uint8 slot=INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item* const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (pItem)
        {
            const ItemPrototype* const pItemProto = pItem->GetProto();
            if (! pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
            if (pItemProto->Class==ITEM_CLASS_CONSUMABLE &&
                (pItemProto->SubClass==ITEM_SUBCLASS_FOOD ||
                pItemProto->SubClass==ITEM_SUBCLASS_POTION ||
                pItemProto->SubClass==ITEM_SUBCLASS_ELIXIR))
            {
                if (pItemProto->Spells[0].SpellCategory == SPELL_CATEGORY_DRINK ||
                    pItemProto->Spells[0].SpellCategory == 4)
                    return pItem;
            }
        }
    }
    // list out items in other removable backpacks
    for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        const Bag* const pBag = (Bag*) m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if (pBag)
        {
            for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                Item* const pItem = m_bot->GetItemByPos(bag, slot);
                if (pItem)
                {
                    const ItemPrototype* const pItemProto = pItem->GetProto();

                    if (! pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
                    if (pItemProto->Class==ITEM_CLASS_CONSUMABLE &&
                        (pItemProto->SubClass==ITEM_SUBCLASS_FOOD ||
                        pItemProto->SubClass==ITEM_SUBCLASS_POTION ||
                        pItemProto->SubClass==ITEM_SUBCLASS_ELIXIR))
                    {
                        // if is WATER
                        if (pItemProto->Spells[0].SpellCategory == SPELL_CATEGORY_DRINK ||
                            pItemProto->Spells[0].SpellCategory == 4)
                            return pItem;
                    }
                }
            }
        }
    }
    return NULL;
}


Item *PlayerbotAI::FindPotion() const
{
    // list out items in main backpack
    for (uint8 slot=INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item* const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if (pItem)
        {
            const ItemPrototype* const pItemProto = pItem->GetProto();
            if (! pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
            if (pItemProto->IsPotion())
            {
                    return pItem;
            }
        }
    }
    // list out items in other removable backpacks
    for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        const Bag* const pBag = (Bag*) m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if (pBag)
        {
            for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                Item* const pItem = m_bot->GetItemByPos(bag, slot);
                if (pItem)
                {
                    const ItemPrototype* const pItemProto = pItem->GetProto();
                    if (! pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
                    if (pItemProto->IsPotion())
                    {
                            return pItem;
                    }
                }
            }
        }
    }
    return NULL;
}



Item *PlayerbotAI::FindBandage() const
{
    //list out items in main backpack
    for(uint8 slot=INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item *const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if(pItem)
        {
            const ItemPrototype *const pItemProto = pItem->GetProto();
            if(!pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
            if(pItemProto->Class == ITEM_CLASS_CONSUMABLE && pItemProto->SubClass == ITEM_SUBCLASS_BANDAGE) return pItem;
        }
    }
    //list out items in other removable backpacks
    for(uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        const Bag *const pBag = (Bag *)m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if(pBag)
        {
            for(uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                Item *const pItem = m_bot->GetItemByPos(bag, slot);
                if(pItem)
                {
                    const ItemPrototype *const pItemProto = pItem->GetProto();
                    if(!pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
                    if(pItemProto->Class == ITEM_CLASS_CONSUMABLE && pItemProto->SubClass == ITEM_SUBCLASS_BANDAGE) return pItem;
                }
            }
        }
    }
    return NULL;
}

// finds poison starting from the front
Item *PlayerbotAI::FindPoisonForward() const
{
    //list out items in main backpack
    for(uint8 slot=INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item *const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if(pItem)
        {
            const ItemPrototype *const pItemProto = pItem->GetProto();
            if(!pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
            if(pItemProto->Class == ITEM_CLASS_CONSUMABLE && pItemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_OTHER) return pItem;
        }
    }
    //list out items in other removable backpacks
    for(uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        const Bag *const pBag = (Bag *)m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if(pBag && pBag->IsBag())
        {
            for(uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                Item *const pItem = m_bot->GetItemByPos(bag, slot);
                if(pItem)
                {
                    const ItemPrototype *const pItemProto = pItem->GetProto();
                    if(!pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
                    if(pItemProto->Class == ITEM_CLASS_CONSUMABLE && pItemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_OTHER) return pItem;
                }
            }
        }
    }
    return NULL;
}

// finds poison starting from the back
Item *PlayerbotAI::FindPoisonBackward() const
{
    //list out items in main backpack
    for(uint8 slot=INVENTORY_SLOT_ITEM_END; slot > INVENTORY_SLOT_ITEM_START; slot--)
    {
        Item *const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if(pItem)
        {
            const ItemPrototype *const pItemProto = pItem->GetProto();
            if(!pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
            if(pItemProto->Class == ITEM_CLASS_GLYPH) continue;
            if(pItemProto->Class == ITEM_CLASS_CONSUMABLE && pItemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_OTHER) return pItem;
        }
    }
    //list out items in other removable backpacks
    for(uint8 bag = INVENTORY_SLOT_BAG_END; bag > INVENTORY_SLOT_BAG_START; --bag)
    {
        const Bag *const pBag = (Bag *)m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if(pBag && pBag->IsBag())
        {
            for(uint8 slot = pBag->GetBagSize(); slot > 0  ; --slot)
            {
                Item *const pItem = m_bot->GetItemByPos(bag, slot);
                if(pItem)
                {
                    const ItemPrototype *const pItemProto = pItem->GetProto();
                    if(!pItemProto || m_bot->CanUseItem(pItemProto)!=EQUIP_ERR_OK) continue;
                    if(pItemProto->Class == ITEM_CLASS_GLYPH) continue;
                    if(pItemProto->Class == ITEM_CLASS_CONSUMABLE && pItemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_OTHER) return pItem;
                }
            }
        }
    }
    return NULL;
}

void PlayerbotAI::InterruptCurrentCastingSpell()
{
    //TellMaster("I'm interrupting my current spell!");
    WorldPacket *const packet = new WorldPacket(CMSG_CANCEL_CAST, 5);
    uint8 counter = 1;
    *packet << counter;
    *packet << m_CurrentlyCastingSpellId;
    m_CurrentlyCastingSpellId = 0;
    m_bot->GetSession()->QueuePacket(packet);
}

void PlayerbotAI::Feast()
{
    //stand up if we are done feasting
    if(!(m_bot->GetHealth() < m_bot->GetMaxHealth() || (m_bot->getPowerType() == POWER_MANA && m_bot->GetPower(POWER_MANA) < m_bot->GetMaxPower(POWER_MANA))))
    {
        m_TimeDoneDrinking = time(0) - 1;
        m_TimeDoneEating = time(0) - 1;
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);
        return;
    }

    //wait 3 seconds before checking if we need to drink more or eat more
    time_t currentTime = time(0);
    SetIgnoreUpdateTime(3);

    //should we drink another
    if(m_bot->getPowerType() == POWER_MANA && currentTime > m_TimeDoneDrinking && ((static_cast<float>(m_bot->GetPower(POWER_MANA)) / m_bot->GetMaxPower(POWER_MANA)) < 0.8))
    {
        Item *pItem = FindDrink();
        if(pItem != NULL && !m_bot->HasSpellCooldown(pItem->GetSpell()))
        {
            UseItem(*pItem);
            m_TimeDoneDrinking = currentTime + 30;
            return;
        } else {

            // find a mage
            if (m_FeastSpamTimer > 0) --m_FeastSpamTimer;
            else {
                Player *mage = GetClassAI()->FindMage(m_bot);
                if (mage != NULL) {
                    SendWhisper("I could use a drink.", *mage);
                }
                TellMaster("I need water.");
                m_FeastSpamTimer=100;
            }
        }
    }

    //should we eat another
    if(currentTime > m_TimeDoneEating && currentTime > m_TimeDoneDrinking  && ((static_cast<float>(m_bot->GetHealth()) / m_bot->GetMaxHealth()) < 0.8))
    {
        Item *pItem = FindFood();
        if(pItem != NULL && !m_bot->HasSpellCooldown(pItem->GetSpell()))
        {
            UseItem(*pItem);
            m_TimeDoneEating = currentTime + 30;
            return;
        }
        //TellMaster("I need food."); //Disabled, tends to be horribly spammy.
    }

    //if we are no longer eating or drinking
    //because we are out of items or we are above 80% in both stats
    if(currentTime > m_TimeDoneEating && currentTime > m_TimeDoneDrinking)
    {
        //TellMaster("I'm ready, let's go.");
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);
    }
}

Unit *PlayerbotAI::getNextTarget(Unit *victim)
{
    Unit *target = NULL;
    AttackerSet m_attackers = victim->getAttackers();
    if(!m_attackers.empty())
    {
        for(AttackerSet::iterator iter = m_attackers.begin(); iter != m_attackers.end(); ++iter)
        {
            if(*iter && m_bot->GetDistance((*iter)) < 30)
            {
                target = *iter;
                break;
            } //end if
        } //end for
    }
    return target;
} //end getNextTarget

//intelligently sets a reasonable combat order for this bot
//based on its class / level / etc
void PlayerbotAI::GetCombatOrders()
{
    if(m_bot->isDead() || isLooting) return;
    Unit *thingToAttack=0;

    // check raid targets icons
   if (!thingToAttack)
    {
        Group *group = m_bot->GetGroup();
        uint64 targetGUID = group->GetTargetWithIconByGroup (m_bot->GetGUID());
        if (targetGUID>0)
        {
            thingToAttack = ObjectAccessor::GetUnit(*m_master, targetGUID);
            if (!thingToAttack || thingToAttack->isDead() || !m_bot->IsHostileTo(thingToAttack)) thingToAttack=0;
//else sLog->outError ("%s is attacking %s", m_bot->GetName(), thingToAttack->GetName());
        }
    }

    //check if someone wants to attack master or me
    if (!thingToAttack) thingToAttack = getNextTarget(m_master);

    if(!thingToAttack)
        thingToAttack = getNextTarget(m_bot);

    //check master's target
    if(!thingToAttack)
    {
        Unit *const pTarget = ObjectAccessor::GetUnit(*m_master, m_master->GetSelection());
        if(pTarget && pTarget->isInCombat() && pTarget->IsHostileTo(m_master))
            thingToAttack = pTarget;
    }

    //last try to find something to attack
    if(!thingToAttack)
    {
        Unit *pUnit = NULL;
        Trinity::NearestHostileUnitInAttackDistanceCheck u_check((Creature*)m_bot, 30.0);
        Trinity::UnitLastSearcher<Trinity::NearestHostileUnitInAttackDistanceCheck> searcher(m_bot, pUnit, u_check);
        m_bot->VisitNearbyObject(30, searcher);
        if(pUnit != NULL && pUnit->isAlive() && pUnit->IsHostileToPlayers()) thingToAttack = pUnit;
    }


    //if the thing to attack is a world invisible trigger, ex Glyph in UBRS,
    //default to master's current victim
    if(!thingToAttack || thingToAttack->GetUInt32Value(UNIT_FIELD_DISPLAYID) == 11686) thingToAttack = m_master->getVictim();

    //if the thing to attack is an invisible trigger ex vazruden in Hellfire Ramparts,
    //default to master's current victim
    if(!thingToAttack || !thingToAttack->IsVisible()) thingToAttack = m_master->getVictim();
    // if the thing to attack is not attackable
    if (!thingToAttack || thingToAttack->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)) thingToAttack = NULL;

    // override all others if ordered to pull
    if (m_bot->GetPlayerbotAI()->GetClassAI()->isPulling()) {
        thingToAttack = ObjectAccessor::GetUnit(*m_master,m_master->GetSelection());
    }

    if(!thingToAttack)
    {
        if(GetClassAI() && !m_bot->isInCombat()) (GetClassAI())->DoNonCombatActions();
        return;
    }

    //wait till it gets closer
    //if(m_bot->GetDistance(thingToAttack) > 30) return;

    //if thing to attack is in a duel, then ignore and don't call updateAI for 6 seconds
    //this method never gets called when the bot is in a duel and this code
    //prevents bot from helping
    if(thingToAttack->GetTypeId() == TYPEID_PLAYER && ((Player*)(thingToAttack))->duel)
    {
       SetIgnoreUpdateTime(6);
        return;
    }

    m_bot->SetSelection(thingToAttack->GetGUID());
    SetIgnoreUpdateTime(1);
    m_combatOrder = ORDERS_KILL;

    if(m_bot->getStandState() != UNIT_STAND_STATE_STAND)
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);
    m_bot->Attack(thingToAttack, true);

    if(thingToAttack->GetTypeId() != TYPEID_PLAYER)
    {
        //add thingToAttack to loot list
        CreatureInfo const *cInfo = ((Creature *)thingToAttack)->GetCreatureInfo();
        if(cInfo && cInfo->lootid) m_lootCreature.push_back(thingToAttack->GetGUID());
    }

    return;
}


void PlayerbotAI::DoNextCombatManeuver()
{
    if(isLooting) return;

    Unit *const pTarget = ObjectAccessor::GetUnit(*m_bot, m_bot->GetSelection());

    //if current order doesn't make sense anymore
    //clear our orders so we can get orders in next update
    if((!pTarget || pTarget->isDead() || !pTarget->IsInWorld() ||
        !m_bot->IsHostileTo(pTarget) || pTarget->IsPolymorphed() || m_bot->isDead()
        || ( !m_master->isInCombat() && !m_bot->isInCombat() && !pTarget->isInCombat()) // The mob probably is in evade mode, stop combat..
        //|| pTarget->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)
        ) &&
        !m_bot->GetPlayerbotAI()->GetClassAI()->isPulling() )
    {
        m_combatOrder = ORDERS_NONE;
        m_bot->SetSelection(0);
        m_bot->GetMotionMaster()->Clear(true);
        m_bot->InterruptNonMeleeSpells(true);
//sLog->outError ("current target doesn't make sense so following");
        Follow(*m_master);
        return;
    }

    if(GetClassAI())
    {
        if(m_bot->HasUnitState(UNIT_STAT_CASTING))
        {
            return;
        }

        GetClassAI()->DoNextCombatManeuver(pTarget);
    }
}

//this is where the AI should go
//GetRandomContactPoint
//GetPower, GetMaxPower
//HasSpellCooldown
//IsAffectedBySpellmod
//isMoving
//HasUnitState(FLAG) FLAG like: UNIT_STAT_ROOT, UNIT_STAT_CONFUSED, UNIT_STAT_STUNNED
//hasAuraType

void PlayerbotAI::UpdateAI(const uint32 p_time)
{
    time_t currentTime = time(0);
    uint32 currentClock = getMSTime();
    m_bot->UpdateZone(m_bot->GetZoneId(), m_bot->GetAreaId());

    if (m_playerBotsFly==0 && m_master->isInFlight())
    {
        if (m_IsFollowingMaster)
        {
            const WorldLocation fakeloc = WorldLocation(35, -0.873190f, 52.920242f, -27.550674f, 1.655620f);
            PlayerbotChatHandler ch(m_master);
            if (! ch.teleport(*m_bot, fakeloc))
            {
                ch.sysmessage(".. could not be teleported ..");
                return;
            }
            m_bot->SendUpdateToPlayer(m_master);
       }
        Stay();
        return;
    }

    if(m_TimeRessurect == 0 && m_bot->isDead())
    {
        m_IsFollowingMaster = false;
        m_TimeRessurect = currentTime + 30;
        return;
    }
    else if(m_TimeRessurect > currentTime && m_bot->isDead())
    {
        return;
    }
    else if((!m_TimeRessurect == 0) && m_TimeRessurect <= currentTime && m_bot->isDead())
    {
        m_IsFollowingMaster = true;
        m_TimeRessurect = 0;
    }
    if(((int64)m_ignoreAIUpdatesUntilTime - (int64)currentClock) > (int64) 30000) { SetIgnoreUpdateTime(2); return; } // Fix Timer overflow and AI freeze (max limit 30 secs)
    if(currentClock < m_ignoreAIUpdatesUntilTime || m_bot->IsBeingTeleported() || m_bot->GetTrader()) return;


    //default updates occur every 1.5 seconds
    SetIgnoreUpdateTime(1.5);

    // prevent cheating
    if (!m_bot->GetGroup())
    {
        m_master->GetSession()->LogoutPlayerBot(m_bot->GetGUID(), false);
        return;
    }

    if(m_bot->isDead()) isLooting = false;

    /*
     * combat checks
     */
    if(m_master <= 0 || ((m_master->isInCombat() || m_bot->isInCombat()) && m_bot->isDead())) return; //You're DEAD, stop thinking.

    //if we are casting a spell then interrupt it
    //make sure any actions that cast a spell set a proper m_ignoreAIUpdatesUntilTime!
    Spell *const pSpell = GetCurrentSpell();
    if(pSpell && !(pSpell->IsChannelActive() || pSpell->IsAutoRepeat())) InterruptCurrentCastingSpell();

    //direct cast command from master
    else if(m_spellIdCommand != 0)
    {
        Unit *pTarget = ObjectAccessor::GetUnit(*m_bot, m_targetGuidCommand);
        if(pTarget != NULL) CastSpell(m_spellIdCommand, pTarget);
        m_spellIdCommand = 0;
        m_targetGuidCommand = 0;
    }

    else if(m_combatOrder != ORDERS_NONE) DoNextCombatManeuver(); //handle combat

    else if (m_bot->GetPlayerbotAI()->GetClassAI()->isPulling())
    {
        GetCombatOrders();
        return;
    }

    //if master is in combat and bot is not, automatically assist master
    //NOTE: combat orders are also set via incoming packets to bot or outgoing packets from master
    else if(m_master->isInCombat() && (!m_bot->isInCombat() || m_combatOrder == ORDERS_NONE) || m_master->isDead()) GetCombatOrders();

    //if bot is in combat but master is not, attack
    else if(m_bot->isInCombat()) GetCombatOrders();

    // if bot is not in combat, but main tank is
    else if (!m_bot->isInCombat()) {
       Unit *tank=m_classAI->FindMainTankInRaid(m_bot);
       if (tank!=NULL && tank->isInCombat()) GetCombatOrders();

    }

    /*
     * Non combat checks
     */

    //are we sitting, if so feast if possible
/* TESTING */    if(m_bot->getStandState() == UNIT_STAND_STATE_SIT) {
/* TESTING */sLog->outError ("%s - sitting so feast", m_bot->GetName());
//
/* TESTING */    }

    //if commanded to follow master and not already following master then follow master
    if(!m_bot->isInCombat() && m_IsFollowingMaster && m_bot->GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE)
    {
        Follow(*m_master);

    //do class specific non combat actions
	} else if(!m_bot->isInCombat() && GetClassAI()) {
        (GetClassAI())->DoNonCombatActions();
    } if(!m_master->isInCombat())
        DoLoot();


    if (m_master->getStandState() == UNIT_STAND_STATE_SIT)
    {
        m_bot->SetStandState(UNIT_STAND_STATE_SIT);
        m_bot->SendUpdateToPlayer(m_master);
        Feast();
    }
    else if (m_TimeDoneDrinking < time(0) && m_TimeDoneEating < time(0)) //Do no interrupt if bot is eating/drinking
    {
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);
        m_bot->SendUpdateToPlayer(m_master);
    }


    //try to catch if he is falling through the world.  This happens
    //when zoning in/out of an instance
    if(m_IsFollowingMaster && m_bot->GetMapId() != m_master->GetMapId() ||
    //m_bot->GetZoneId() != m_master->GetZoneId() ||
    (abs(abs(m_bot->GetPositionX()) - abs(m_master->GetPositionX())) > 90) ||
    (abs(abs(m_bot->GetPositionY()) - abs(m_master->GetPositionY())) > 90) ||
    (abs(abs(m_bot->GetPositionZ()) - abs(m_master->GetPositionZ())) > 50))
    {
//sLog->outError ("%s: %s is too far away so following", m_bot->GetName(), m_master->GetName());
        Follow(*m_master);
    }
}


void PlayerbotAI::KilledMonster(uint32 entry, uint64 guid)
{
   // isLooting = true;

    if(m_master->isAlive() && m_IsFollowingMaster && !m_master->isInCombat())
    {
        if(!DoLoot())
        {
            float angle = rand_norm()*M_PI; //Generates random float between 0 and 3.14
            float dist = (float)(urand((m_followDistanceMin*10), (m_followDistanceMax*10))/10);

            m_bot->GetMotionMaster()->Clear(true);
            m_bot->GetMotionMaster()->MoveFollow(m_master, dist, angle);
        }
    }

    // reset main tank every time we finish combat, just in case the
    // original main tank died and got set to next tank.
    //m_classAI->SetMainTank(NULL);
}

Spell *PlayerbotAI::GetCurrentSpell() const
{
    if(m_CurrentlyCastingSpellId == 0) return NULL;
    Spell *const pSpell = m_bot->FindCurrentSpellBySpellId(m_CurrentlyCastingSpellId);
    return pSpell;
}

void PlayerbotAI::TellMaster(const std::string &text)
{
    SendWhisper(text, *m_master);
}

bool PlayerbotAI::CanBotsFly()
{
    if (m_playerBotsFly==0) return false;
    else return true;
}

void PlayerbotAI::SendWhisper(const std::string &text, Player &player)
{
    WorldPacket data(SMSG_MESSAGECHAT, 200);
    m_bot->BuildPlayerChat(&data, CHAT_MSG_WHISPER_INFORM, text, LANG_UNIVERSAL);
    player.GetSession()->SendPacket(&data);
}

bool PlayerbotAI::canObeyCommandFrom(const Player &player) const
{
    return player.GetSession()->GetAccountId() == m_master->GetSession()->GetAccountId();
}

void PlayerbotAI::SetInFront(const Unit *obj)
{
    if(!m_bot->HasInArc(M_PI, obj))
    {
        m_bot->SetInFront(obj);
        m_bot->SendUpdateToPlayer(m_master);
    }
}

bool PlayerbotAI::CastSpell(const char *args)
{
    uint32 spellId = getSpellId(args);
    return(spellId) ? CastSpell(spellId) : false;
}

bool PlayerbotAI::CastSpell(uint32 spellId, Unit *target, bool checkFirst, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck, bool triggered)
{
    if (!spellId) return false;

    if (!m_bot->HasSpell(spellId)) {
        return false;
    }

    const SpellEntry * pSpellInfo = sSpellStore.LookupEntry(spellId);
    if (!pSpellInfo)
    {
        sLog->outDebug("CRITICAL: PBot Class %u - Non-existing Spell in Cast - [%u]", m_bot->getClass(), spellId);
        std::stringstream ss;
        ss << "Missing spell entry in CastSpell - ";
        ss << spellId;
        TellMaster(ss.str());
        SetIgnoreUpdateTime(1);
        return false;
    }
    return CastSpell(pSpellInfo, target, checkFirst, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck, triggered);
}

bool PlayerbotAI::CastSpell(const SpellEntry * pSpellInfo, Unit *target, bool checkFirst, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck, bool triggered)
{
    if(!m_bot->isAlive()) return false;
    if(!pSpellInfo)
    {
        sLog->outError ("%s: Missing spell entry in CastSpell Direct", m_bot->GetName());
        TellMaster("Missing spell entry in CastSpell");
        SetIgnoreUpdateTime(1);
        return false;
    }
    uint32 spellId = pSpellInfo->Id;
    uint64 oldSel = m_bot->GetSelection();

    // Auto Targeting
    if (!target)
    {
        //NEGATIVE SPELL
        if (sSpellMgr->GetSpellCustomAttr(spellId) & SPELL_ATTR0_CU_NEGATIVE)
        {
            if (m_bot->GetSelection() <= 0) return false;
            else
            {
                target = ObjectAccessor::GetUnit(*m_bot, m_bot->GetSelection());
                if (!target) return false;
            }
        }
        else { target = m_bot; }
    }

    //Make the Checks

    if (!triggered && checkFirst && !CanCast(pSpellInfo, target, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck) ) { return false; }
    if ( m_bot->GetSelection() != target->GetGUID() ) { m_bot->SetSelection(target->GetGUID()); } //if target is different than selection apply it

    m_bot->CastSpell(target, pSpellInfo, triggered); //CAST THE SPELL
    if ( m_bot->GetSelection() != oldSel ) { m_bot->SetSelection(oldSel); } // Restore if target changed to cast

    // Check if the casting started..
    Spell *const pSpell = m_bot->FindCurrentSpellBySpellId(spellId);
    if(!pSpell) return false;

    // Trigger Pseudo Global Cooldown and consider casttime
    float GCD = 1.5f;
    if (m_bot->getPowerType() == POWER_ENERGY) GCD = 1;
    float psCastTime = ((float)pSpell->GetCastTime()) / 1000.0f;
    if (psCastTime - GCD > -0.3f) GCD = 0.3f; //Global cooldown won't be an issiue for casts (0.3 secs is for safe next cast)
    else { GCD -= psCastTime; } //Remaining GCD after cast..
    //float psRecoveryTime = GetSpellRecoveryTime(pSpellInfo) / 1000;
    //sLog->outDebug("Bot [%u] Start Spell [%u] Cast Time [%f]", m_bot->GetGUIDLow(), pSpellInfo->Id, psCastTime);
    m_CurrentlyCastingSpellId = spellId;
    //SetIgnoreUpdateTime(psCastTime + GCD);
    SetIgnoreUpdateTime(psCastTime > GCD ? psCastTime : GCD);
    return true;
}

bool PlayerbotAI::CanCast(uint32 spellId, Unit *target, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck)
{
     //if spellId == 0, it means that the bot is not high enough level to
    //have learned the spell
    if (!spellId) return false;
    const SpellEntry * pSpellInfo = sSpellStore.LookupEntry(spellId);
    if (!pSpellInfo)
    {
        sLog->outDebug("CRITICAL: PBot Class %u - Non-existing Spell in Cast - [%u]", m_bot->getClass(), spellId);
        std::stringstream ss;
        ss << "Missing spell entry in CastSpell - ";
        ss << spellId;
        TellMaster(ss.str());
        SetIgnoreUpdateTime(1);
        return false;
    }
    return CanCast(pSpellInfo, target, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck);
}

bool PlayerbotAI::CanCast(const SpellEntry * pSpellInfo, Unit *target, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck)
{
    if (!pSpellInfo)
    {
        sLog->outDebug("CRITICAL: PBot Class %u - Non-existing Spell in CastCheck - Direct SpellEntry", m_bot->getClass());
        TellMaster("Missing spell entry in CastSpell");
        SetIgnoreUpdateTime(1);
        return false;
    }
    uint32 spellId = pSpellInfo->Id;

    // Auto Targeting
    if (!target)
    {
        //NEGATIVE SPELL
        if (sSpellMgr->GetSpellCustomAttr(spellId) & SPELL_ATTR0_CU_NEGATIVE)
        {
            if (m_bot->GetSelection() <= 0) return false;
            else
            {
                target = ObjectAccessor::GetUnit(*m_bot, m_bot->GetSelection());
                if (!target) return false;
            }
        }
        else { target = m_bot; }
    }

    if (!m_bot->isAlive()) return false;
    if (m_bot->HasSpellCooldown(spellId)) return false;
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) return false;
    if (m_bot->IsMounted()) return false;

    //cast existing aura over again?
    if (!castExistingAura && target->HasAura(spellId, m_bot->GetGUID())) return false;

    //Stances-forms and equipment REQs
    if (!skipEquipStanceCheck)
    {
        uint32 formMask = (GetForm() ? 1 << (GetPlayerBot()->GetShapeshiftForm() - 1) : 0);
        //sLog->outDebug("DEBUG: Spell [%u] - Form [%X] - Need Form [%X] - Not Form [%X]", pSpellInfo->Id, formMask, pSpellInfo->Stances, pSpellInfo->StancesNot );
        if (pSpellInfo->Stances & formMask) { return true; }
        if (pSpellInfo->StancesNot && pSpellInfo->StancesNot & formMask) { return false; }
        if (!m_bot->HasItemFitToSpellRequirements(pSpellInfo)) return false;
    }

    //Power Costs
    const SpellSchoolMask pSpellSchool = GetSpellSchoolMask(pSpellInfo);
    uint32 pPowerCost = CalculatePowerCost(pSpellInfo, m_bot, pSpellSchool);

    if (skipEquipStanceCheck) { if (m_bot->GetPower((Powers)pSpellInfo->powerType) < pPowerCost) return false; } //Power check for Required PowerType (After changind stance, powertype may change, for druids)
    else { if (m_bot->GetPower(m_bot->getPowerType()) < pPowerCost) return false; } //Power check for Current m_bot Power Type

    //Distance / movement checks
    const SpellRangeEntry * pSpellRange = sSpellRangeStore.LookupEntry(pSpellInfo->rangeIndex);
    float curDistance = m_bot->GetDistance(target);
    if (GetSpellCastTime(pSpellInfo) > 0 && m_bot->isMoving()) return false; //Cannot cast while moving

    //The target is immune or not?
    if (target->GetTypeId() != TYPEID_PLAYER) { if ( ((Creature*)target)->IsImmunedToSpell(pSpellInfo)) { return false; } }
    else { if (target->IsImmunedToSpell(pSpellInfo)) { return false; } }

    //target reaction checks (Has problems with dual effect spells like death coil/holy shock)
    if (skipFriendlyCheck)
    {
        if (pSpellRange->maxRangeHostile != 0) { if ( pSpellRange->maxRangeHostile < curDistance || pSpellRange->minRangeHostile > curDistance ) { return false; } } //Assume hostile spell
    }
    else if (IsPositiveSpell(spellId))
    {
        if(! m_bot->IsFriendlyTo(target)) { return false; }
        else if (pSpellRange->maxRangeFriend != 0) { if (pSpellRange->maxRangeFriend < curDistance || pSpellRange->minRangeFriend > curDistance) { return false; } }
        else if (curDistance > MELEE_RANGE) return false;
    }
    else
    {
        if (m_bot->IsFriendlyTo(target))  return false;
        if (!m_bot->HasInArc(M_PI,target)) return false; //target is not in front
        if (pSpellRange->maxRangeHostile != 0) { if ( pSpellRange->maxRangeHostile < curDistance || pSpellRange->minRangeHostile > curDistance ) { return false; } }
        else if (curDistance > MELEE_RANGE) return false; //Out of range - Melee Range
    }

    return true;
}

uint8 PlayerbotAI::GetForm(Unit *pPlayer)
{
    if (!pPlayer) pPlayer = m_bot;
    return (pPlayer->GetUInt32Value(UNIT_FIELD_BYTES_2) & 0xFF000000) >> (4 * 6);
}

//extracts all item ids in format below
//I decided to roll my own extractor rather then use the one in ChatHandler
//because this one works on a const string, and it handles multiple links
//|color|linkType:key:something1:...:somethingN|h[name]|h|r
void PlayerbotAI::extractItemIds(const std::string &text, std::list<uint32> &itemIds) const
{
    uint8 pos = 0;
    while(true)
    {
        int i = text.find("Hitem:", pos);
        if(i == -1) break;
        pos = i + 6;
        int endPos = text.find(':', pos);
        if(endPos == -1) break;
        std::string idC = text.substr(pos, endPos - pos);
        uint32 id = atol(idC.c_str());
        pos = endPos;
        if(id) itemIds.push_back(id);
    }
}

bool PlayerbotAI::extractGOinfo(const std::string& text, uint32 &guid, uint32 &entry, int &mapid, float &x, float &y, float &z) const
{

   //    Link format
   //    |cFFFFFF00|Hfound:" << guid << ':'  << entry << ':' << x << ':' << y << ':' << z  << ':' << mapid << ':' <<  "|h[" << gInfo->name << "]|h|r";

     //    |cFFFFFF00|Hfound:5093:1731:-9295:-270:81.874:0:|h[Copper Vein]|h|r
  uint8 pos = 0;
      // extract GO guid
       int i = text.find("Hfound:", pos); // base H = 11
       if (i == -1) // break if error
            return false;

       pos = i + 7; //start of window in text 11 + 7 = 18
      int endPos = text.find(':', pos); // end of window in text 22
      if (endPos == -1) //break if error
           return false;
       std::string guidC = text.substr(pos, endPos - pos); // get string within window i.e guid 22 - 18 =  4
       guid = atol(guidC.c_str()); // convert ascii to long int
     // extract GO entry
       pos = endPos + 1;
      endPos = text.find(':', pos); // end of window in text
       if (endPos == -1) //break if error
           return false;
      std::string entryC = text.substr(pos, endPos - pos); // get string within window i.e entry
        entry = atol(entryC.c_str()); // convert ascii to float
     // extract GO x
       pos = endPos + 1;
      endPos = text.find(':', pos); // end of window in text
       if (endPos == -1) //break if error
           return false;
       std::string xC = text.substr(pos, endPos - pos); // get string within window i.e x

        x = atof(xC.c_str()); // convert ascii to float
      // extract GO y
       pos = endPos + 1;
      endPos = text.find(':', pos); // end of window in text
       if (endPos == -1) //break if error
           return false;
      std::string yC = text.substr(pos, endPos - pos); // get string within window i.e y
      y = atof(yC.c_str()); // convert ascii to float
     // extract GO z

        pos = endPos + 1;
       endPos = text.find(':', pos); // end of window in text
        if (endPos == -1) //break if error
           return false;

       std::string zC = text.substr(pos, endPos - pos); // get string within window i.e z

        z = atof(zC.c_str()); // convert ascii to float

      //extract GO mapid
       pos = endPos + 1;
       endPos = text.find(':', pos); // end of window in text
       if (endPos == -1) //break if error
           return false;
      std::string mapidC = text.substr(pos, endPos - pos); // get string within window i.e mapid
       mapid = atoi(mapidC.c_str()); // convert ascii to int
        pos = endPos; // end
      return true;
}

//extracts currency in #g#s#c format
uint32 PlayerbotAI::extractMoney(const std::string &text) const
{
    //if user specified money in ##g##s##c format
    std::string acum = "";
    uint32 copper = 0;
    for(uint8 i = 0; i < text.length(); i++)
    {
        if(text[i] == 'g')
        {
            copper += (atol(acum.c_str()) * 100 * 100);
            acum = "";
        }
        else if(text[i] == 'c')
        {
            copper += atol(acum.c_str());
            acum = "";
        }
        else if(text[i] == 's')
        {
            copper += (atol(acum.c_str()) * 100);
            acum = "";
        }
        else if(text[i] == ' ')
        {
            break;
        }
        else if(text[i] >= 48 && text[i] <= 57)
        {
            acum += text[i];
        } else {
            copper = 0;
            break;
        }
    }
    return copper;
}

// finds items in equipment and adds Item* to foundItemList
// also removes found item IDs from itemIdSearchList when found
void PlayerbotAI::findItemsInEquip(std::list<uint32>& itemIdSearchList, std::list<Item*>& foundItemList) const
{
    for( uint8 slot=EQUIPMENT_SLOT_START; itemIdSearchList.size()>0 && slot<EQUIPMENT_SLOT_END; slot++ ) {
        Item* const pItem = m_bot->GetItemByPos( INVENTORY_SLOT_BAG_0, slot );
        if( !pItem )
            continue;

        for (std::list<uint32>::iterator it = itemIdSearchList.begin(); it != itemIdSearchList.end(); ++it)
        {
            if (pItem->GetProto()->ItemId != *it)
                continue;

            foundItemList.push_back(pItem);
            itemIdSearchList.erase(it);
            break;
        }
    }
}


//finds items in inventory and adds Item *to foundItemList
//also removes found item IDs from itemIdSearchList when found
void PlayerbotAI::findItemsInInv(std::list<uint32>& itemIdSearchList, std::list<Item*>& foundItemList) const
{

    //look for items in main bag
    for(uint8 slot = INVENTORY_SLOT_ITEM_START; itemIdSearchList.size() > 0 && slot < INVENTORY_SLOT_ITEM_END; ++slot)
    {
        Item *const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
        if(!pItem) continue;
        for(std::list<uint32>::iterator it = itemIdSearchList.begin(); it != itemIdSearchList.end(); ++it)
        {
            if(pItem->GetProto()->ItemId != *it) continue;
            foundItemList.push_back(pItem);
            itemIdSearchList.erase(it);
            break;
        }
    }

    //for all for items in other bags
    for(uint8 bag = INVENTORY_SLOT_BAG_START; itemIdSearchList.size() > 0 && bag < INVENTORY_SLOT_BAG_END; ++bag)
    {
        Bag *const pBag = (Bag *)m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
        if(!pBag) continue;
        for(uint8 slot = 0; itemIdSearchList.size() > 0 && slot < pBag->GetBagSize(); ++slot)
        {
            Item *const pItem = m_bot->GetItemByPos(bag, slot);
            if(!pItem) continue;
            for(std::list<uint32>::iterator it = itemIdSearchList.begin(); it != itemIdSearchList.end(); ++it)
            {
                if(pItem->GetProto()->ItemId != *it) continue;
                foundItemList.push_back(pItem);
                itemIdSearchList.erase(it);
                break;
            }
        }
    }
}

bool PlayerbotAI::HasPick()
{
    QueryResult result;

    // list out equiped items
    for( uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
    {
        Item* const pItem = m_bot->GetItemByPos( INVENTORY_SLOT_BAG_0, slot );
        if (pItem )
        {
            const ItemPrototype* const pItemProto = pItem->GetProto();

                if (!pItemProto )
                    continue;

            result = WorldDatabase.PQuery("SELECT TotemCategory FROM item_template WHERE entry = '%i'", pItemProto->ItemId);
            if (result)
            {
                Field *fields = result->Fetch();
                uint32 tc = fields[0].GetUInt32();
                // sLog->outDebug("HasPick %u",tc);
                if(tc ==  165 || tc == 167) // pick = 165, hammer = 162 or hammer pick = 167
                    return true;
            }
        }
    }

    // list out items in backpack
    for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
    {
        // sLog->outDebug("[%s's]backpack slot = %u",m_bot->GetName(),slot); // 23 to 38 = 16
        Item* const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot); // 255, 23 to 38
        if (pItem)
        {
            const ItemPrototype* const pItemProto = pItem->GetProto();
            if (!pItemProto )
                continue;
            result = WorldDatabase.PQuery("SELECT TotemCategory FROM item_template WHERE entry = '%i'", pItemProto->ItemId);
            if (result)
            {
                Field *fields = result->Fetch();
                uint32 tc = fields[0].GetUInt32();
                // sLog->outDebug("HasPick %u",tc);
                if(tc ==  165 || tc == 167) // pick = 165, hammer = 162 or hammer pick = 167
                    return true;
            }
        }
    }

    // list out items in other removable backpacks
    for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag) // 20 to 23 = 4
    {
        const Bag* const pBag = (Bag*) m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag); // 255, 20 to 23
        if (pBag)
        {
            for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
            {
                // sLog->outDebug("[%s's]bag[%u] slot = %u",m_bot->GetName(),bag,slot); // 1 to bagsize = ?
                Item* const pItem = m_bot->GetItemByPos(bag, slot); // 20 to 23, 1 to bagsize
                if (pItem)
                {
                    const ItemPrototype* const pItemProto = pItem->GetProto();

                    if (!pItemProto )
                        continue;

                    result = WorldDatabase.PQuery("SELECT TotemCategory FROM item_template WHERE entry = '%i'", pItemProto->ItemId);
                    if (result)
                    {

                        Field *fields = result->Fetch();
                        uint32 tc = fields[0].GetUInt32();
                        // sLog->outDebug("HasPick %u",tc);
                        if(tc ==  165 || tc == 167)
                            return true;
                    }
                }
            }
        }
    }
    std::ostringstream out;
    out << "|cffffffffI do not have a pick!";
    TellMaster( out.str().c_str() );
    return false;
}


//submits packet to use an item
void PlayerbotAI::PoisonWeapon(Item &item, uint32 _spellId, uint32 _target, EquipmentSlots weaponSlot)
{
    uint8 bagIndex = item.GetBagSlot();
    uint8 slot = item.GetSlot();
    uint8 cast_count = 1;
    uint32 spellid = _spellId;
    uint64 item_guid = item.GetGUID();
    uint32 glyphIndex = 0; //??
    uint8 unk_flags = 0; //not 0x02

    uint32 target = _target;
    uint32 targetItemGUID = 0;
    uint8 x = 0;
    Item *weapon=NULL;
    if (_spellId>0){

        targetItemGUID = 16;
        x = 135;
        cast_count = 7;
        weapon = GetPlayerBot()->GetItemByPos( INVENTORY_SLOT_BAG_0, weaponSlot );

    }

    WorldPacket *const packet = new WorldPacket(CMSG_USE_ITEM, 1 + 1 + 1 + 4 + 8 + 4 + 1 + 4);
    *packet << bagIndex << slot << cast_count << spellid << item_guid << glyphIndex << unk_flags << target;
    if (weapon) packet->appendPackGUID(weapon->GetGUID());

    m_bot->GetSession()->QueuePacket(packet); //queue the packet to get around race condition

} // end PoisonWeapon



//submits packet to use an item
void PlayerbotAI::UseItem(Item &item)
{
    uint8 bagIndex = item.GetBagSlot();
    uint8 slot = item.GetSlot();
    uint8 cast_count = 1;
    uint32 spellid = 0; //only used in combat
    uint64 item_guid = item.GetGUID();
    uint32 glyphIndex = 0; //??
    uint8 unk_flags = 0; //not 0x02

    //create target data
    //note other targets are possible but not supported at the moment
    //see SpellCastTargets::read in Spell.cpp to see other options
    //for setting target

    uint32 target = TARGET_FLAG_SELF;

    WorldPacket *const packet = new WorldPacket(CMSG_USE_ITEM, 1 + 1 + 1 + 4 + 8 + 4 + 1);
    *packet << bagIndex << slot << cast_count << spellid << item_guid << glyphIndex << unk_flags << target;
    m_bot->GetSession()->QueuePacket(packet); //queue the packet to get aroundrace condition

} // end UseItem


//submits packet to use an item
void PlayerbotAI::EquipItem(Item &item)
{
    uint8 bagIndex = item.GetBagSlot();
    uint8 slot = item.GetSlot();

    WorldPacket *const packet = new WorldPacket(CMSG_AUTOEQUIP_ITEM, 2);
    *packet << bagIndex << slot;
    m_bot->GetSession()->QueuePacket(packet);
}

// submits packet to trade an item (trade window must already be open)
// default slot is -1 which means trade slots 0 to 5. if slot is set
// to TRADE_SLOT_NONTRADED (which is slot 6) item will be shown in the
// 'Will not be traded' slot.
bool PlayerbotAI::TradeItem(const Item& item, int8 slot)
{
    sLog->outDebug( "[PlayerbotAI::TradeItem]: slot=%d, hasTrader=%d, itemInTrade=%d, itemTradeable=%d",
        slot,
        (m_bot->GetTrader()?1:0),
        (item.IsInTrade()?1:0),
        (item.CanBeTraded()?1:0)
        );

    if (!m_bot->GetTrader() || item.IsInTrade())
        return false;

    int8 tradeSlot = -1;

    if( (slot>=0 && slot<TRADE_SLOT_COUNT) /* && m_bot->GetItem(trade slot)==NULL_SLOT */) {
        tradeSlot = slot;
    } else if (!item.CanBeTraded())
    {
        tradeSlot = (uint8) TRADE_SLOT_NONTRADED;
    }
    else
    {
        for( uint8 i=0; i<TRADE_SLOT_TRADED_COUNT; ++i )
        {
			if (m_bot->GetTradeData()->GetItem(TradeSlots(i)) == NULL){
                tradeSlot = (uint8)i;
                break;
			}
        }
    }

    if( tradeSlot == -1 ) return false;

    WorldPacket* const packet = new WorldPacket(CMSG_SET_TRADE_ITEM, 3);
    *packet << (uint8) tradeSlot << (uint8) item.GetBagSlot()
            << (uint8) item.GetSlot();
    m_bot->GetSession()->QueuePacket(packet);

    return true;
}


//submits packet to trade copper (trade window must be open)
bool PlayerbotAI::TradeCopper(uint32 copper)
{
    if(copper > 0)
    {
        WorldPacket *const packet = new WorldPacket(CMSG_SET_TRADE_GOLD, 4);
        *packet << copper;
        m_bot->GetSession()->QueuePacket(packet);
        return true;
    }
    return false;
}

void PlayerbotAI::Stay()
{
    if (!m_IsFollowingMaster)
        return;

    m_IsFollowingMaster = false;
    m_bot->GetMotionMaster()->Clear(true);
    m_bot->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
}


bool PlayerbotAI::Follow(Player &player)
{
    if(m_master <= 0 || ((m_master->isInCombat() || m_bot->isInCombat()) && m_bot->isDead())) return false; //You're DEAD, stop thinking.
    if(m_master->isDead()) return false;
    if(m_master->IsBeingTeleported() || m_master->isInFlight()) return false;

    if(m_bot->getStandState() == UNIT_STAND_STATE_SIT && (m_TimeDoneDrinking < time(0) && m_TimeDoneEating < time(0))) return false; //Do no interrupt if bot is eating/drinking

    m_IsFollowingMaster = true;

    if(!m_bot->IsStandState()) {
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);
        m_bot->SendUpdateToPlayer(m_master);
    }

    if(!m_bot->isInCombat())
    {
        //if bot is dead and master is alive, revive bot
        if(m_master->isAlive() && !m_bot->isAlive())
        {
            SetIgnoreUpdateTime(6);
            isLooting = false;
            PlayerbotChatHandler ch(m_master);
            if(!ch.revive(*m_bot))
            {
                ch.sysmessage(".. could not be revived ..");
                return false;
            }
        }

        if(!m_bot->isDead() && !m_bot->IsBeingTeleported() && !player.isDead() &&
            (m_bot->GetMapId() != player.GetMapId()
            || m_bot->GetZoneId() != player.GetZoneId()
            || m_bot->GetAreaId() != player.GetAreaId()
            || m_bot->GetPhaseMask() != player.GetPhaseMask())
            || m_bot->GetDistance(player) > 255)
            {
                SetIgnoreUpdateTime(6);
                isLooting = false;
                PlayerbotChatHandler ch(m_master);
                if(!ch.teleport(*m_bot))
                {
                    ch.sysmessage(".. could not be teleported ..");
                    return false;
                }
        }
    }

    if(m_bot->isAlive() && !isLooting)
    {
        float angle = M_PI/2 + rand_norm()*M_PI ; //Generates random float between 90 and 270 degrees
        float dist = (float)(urand((m_followDistanceMin*10), (m_followDistanceMax*10))/10); // Using urand to get a random float is stupid.
        m_bot->GetMotionMaster()->Clear(true);
        m_bot->GetMotionMaster()->MoveFollow(&player, dist, angle);

        return true;
    }
    return false;
}

//handle commands sent through chat channels
void PlayerbotAI::HandleCommand(const std::string &text, Player &fromPlayer)
{
    //ignore any messages from Addons
    if(text.empty() ||
        text.find("X-Perl") != std::wstring::npos ||
        text.find("HealBot") != std::wstring::npos ||
        text.find("LOOT_OPENED") != std::wstring::npos ||
        text.find("CTRA") != std::wstring::npos)
        return;

    //if message is not from a player in the masters account auto reply and ignore
    //if(!canObeyCommandFrom(fromPlayer))
    //{
        //std::string msg = "I can't talk to you. Please speak to my master ";
        //msg += m_master->GetName();
        //msg += ".";
        //m_bot->SendWhisper(msg, *fromPlayer);
        //m_bot->HandleEmoteCommand(EMOTE_ONESHOT_NO);
    //}


    else if (text == "I could use a drink." &&
             m_bot->getClass() == CLASS_MAGE)
    {
            Item const*pItem = FindDrink();
            if (pItem != NULL) {
                WorldPacket *p = new WorldPacket(CMSG_INITIATE_TRADE, 8);
                *p << fromPlayer.GetGUID();
                m_bot->GetSession()-> HandleInitiateTradeOpcode(*p);
                SendWhisper ("Here is a tasty treat for you", fromPlayer);

            }
    }
    // accept food/drink from mage
    else if (text == "Here is a tasty treat for you")
    {
        m_bot->Say ("Thank you for this treat", LANG_UNIVERSAL);
        SendWhisper ("Thank you for this treat.", fromPlayer);
    }
    // trade opened so send items over
    else if (text == "Thank you for this treat")
    {
        if (m_bot->getClass()!=CLASS_MAGE) return;

        Item const*pItem = FindDrink();
        if (pItem==NULL) return;

        bool trade = TradeItem (*pItem,2);
        if (trade) {
            m_bot->Say("Enjoy the refreshing drink.", LANG_UNIVERSAL);

			WorldPacket* const packet = new WorldPacket(CMSG_ACCEPT_TRADE, 3);
			m_bot->GetSession()->QueuePacket(packet);  // packet is not used
        } else {
            m_bot->Say ("I cannot trade with you.", LANG_UNIVERSAL);
        }
    }

    // if in the middle of a trade, and player asks for an item/money
    else if (m_bot->GetTrader() && m_bot->GetTrader()->GetGUID() == fromPlayer.GetGUID() &&
        fromPlayer.GetPlayerbotAI() == NULL)
    {
        uint32 copper = extractMoney(text);
        if (copper > 0)
            TradeCopper(copper);

        std::list<uint32> itemIds;
        extractItemIds(text, itemIds);
        if (itemIds.size() == 0)
            SendWhisper("Show me what item you want by shift clicking the item in the chat window.", fromPlayer);
        else
        {
            std::list<Item*> itemList;
            findItemsInInv(itemIds, itemList);
            findItemsInEquip(itemIds, itemList);
            for (std::list<Item*>::iterator it = itemList.begin(); it != itemList.end(); ++it)
                TradeItem(**it);
        }
    }



    else if(text == "follow" || text == "come")
        Follow(*m_master);

    else if(text == "stay" || text == "stop")
        Stay();

    //handle cast command
    else if(text.size() > 2 && text.substr(0, 2) == "c " ||
        text.size() > 5 && text.substr(0, 5) == "cast ")
        {
            uint32 spellId = 0;
            std::string spellStr = text.substr(text.find(" ") + 1);

            if(spellStr.find("Hspell:"))
            {
                spellStr = spellStr.substr(spellStr.find("|h[") + 3);
                spellStr = spellStr.substr(0, spellStr.find("]"));
            } else
                spellId = (uint32)atol(spellStr.c_str());

            //try and get spell ID by name
            if(spellId == 0) spellId = getSpellId(spellStr.c_str(), true);

            uint64 castOnGuid = fromPlayer.GetSelection();
            if(castOnGuid == 0) castOnGuid = m_bot->GetGUID();
            if(spellId != 0)
            {
                m_spellIdCommand = spellId;
                m_targetGuidCommand = castOnGuid;
            }
    }

    //use items
    else if(text.size() > 2 && text.substr(0, 2) == "u " ||
    text.size() > 4 && text.substr(0, 4) == "use ")
    {
        std::list<uint32> itemIds;
        std::list<Item *> itemList;
        extractItemIds(text, itemIds);
        findItemsInInv(itemIds, itemList);
        for(std::list<Item *>::iterator it = itemList.begin(); it != itemList.end(); ++it) UseItem(**it);
    }

    // poison mainhand weapon
    else if(text.size() > 2 && text.substr(0, 2) == "p " ||
    text.size() >= 8 && text.substr(0, 8) == "poison m")
    {

        Item *poison = FindPoisonForward();
        if(poison == NULL) {
            std::string msg = "No poison found for mainhand.";
            SendWhisper(msg, fromPlayer);
            return;
        }

        PoisonWeapon(*poison, poison->GetProto()->Spells[0].SpellId, TARGET_FLAG_ITEM, EQUIPMENT_SLOT_MAINHAND);
    }

    // poison offhand weapon
    else if(text.size() >= 8 && text.substr(0, 8) == "poison o")
    {

        Item *poison = FindPoisonBackward();
        if(poison == NULL) {
            std::string msg = "No poison found for offhand.";
            SendWhisper(msg, fromPlayer);
            return;
        }

        PoisonWeapon(*poison, poison->GetProto()->Spells[0].SpellId, TARGET_FLAG_ITEM, EQUIPMENT_SLOT_OFFHAND);
    }

    // npcbot commands
    else if(text.size() >= 8 && text.substr(0, 8) == "npcbot a")
    {
        if(m_bot->HaveBot()) {
            SendWhisper("I already have a bot.", fromPlayer);
            return;
        }

        std::string text1 = text.substr(text.find(" ") + 1);
        std::string botClass = text1.substr(text1.find(" ") + 1);


        if (botClass == "priest") m_bot->CreateNPCBot(CLASS_PRIEST);
        else if (botClass == "warrior") m_bot->CreateNPCBot(CLASS_WARRIOR);
        else if (botClass == "druid") m_bot->CreateNPCBot(CLASS_DRUID);
        else if (botClass == "paladin") m_bot->CreateNPCBot(CLASS_PALADIN);
        else if (botClass == "hunter") m_bot->CreateNPCBot(CLASS_HUNTER);
        else if (botClass == "mage") m_bot->CreateNPCBot(CLASS_MAGE);
        else if (botClass == "warlock") m_bot->CreateNPCBot(CLASS_WARLOCK);
        else if (botClass == "shaman") m_bot->CreateNPCBot(CLASS_SHAMAN);
        else if (botClass == "rogue") m_bot->CreateNPCBot(CLASS_ROGUE);
        else {
             SendWhisper("Unknown class", fromPlayer);
             return;
        }

       // m_master->CreateNPCBot(CLASS_PALADIN);
    }
    else if(text.size() >= 8 && text.substr(0, 8) == "npcbot d")
    {
        if(m_bot->HaveBot())
            m_bot->SetBotMustDie();
    }

    //equip items
    else if(text.size() > 2 && text.substr(0, 2) == "e " ||
    text.size() > 6 && text.substr(0, 6) == "equip ")
    {
        std::list<uint32> itemIds;
        std::list<Item *> itemList;
        extractItemIds(text, itemIds);
        findItemsInInv(itemIds, itemList);
         for(std::list<Item *>::iterator it = itemList.begin(); it != itemList.end(); ++it) EquipItem(**it);
    }

    else if(text == "spells")
    {
        int loc = m_master->GetSession()->GetSessionDbcLocale();

        std::ostringstream posOut;
        std::ostringstream negOut;

        const std::string ignoreList = ",Opening,Closing,Stuck,Remove Insignia,Opening - No Text,Grovel,Duel,Honorless Target,";
        std::string alreadySeenList = ",";

        for(PlayerSpellMap::iterator itr = m_bot->GetSpellMap().begin(); itr != m_bot->GetSpellMap().end(); ++itr)
        {
            const uint32 spellId = itr->first;

            if(itr->second->state == PLAYERSPELL_REMOVED || itr->second->disabled || IsPassiveSpell(spellId))
                continue;

            const SpellEntry *const pSpellInfo = sSpellStore.LookupEntry(spellId);
            if(!pSpellInfo)
                continue;

            //|| name.find("Teleport") != -1

            std::string comp = ",";
            comp.append(pSpellInfo->SpellName[loc]);
            comp.append(",");

            if(!(ignoreList.find(comp) == std::string::npos &&
                alreadySeenList.find(comp) == std::string::npos))
                continue;

            alreadySeenList += pSpellInfo->SpellName[loc];
            alreadySeenList += ",";

            if(IsPositiveSpell(spellId))
                posOut << " |cffffffff|Hspell:" << spellId << "|h[" << pSpellInfo->SpellName[loc] << "]|h|r";
            else
                negOut << " |cffffffff|Hspell:" << spellId << "|h[" << pSpellInfo->SpellName[loc] << "]|h|r";
        }

        ChatHandler ch(&fromPlayer);
        SendWhisper("Here's my non-attack spells:", fromPlayer);
        ch.SendSysMessage(posOut.str().c_str());
        SendWhisper("Here's my attack spells:", fromPlayer);
        ch.SendSysMessage(negOut.str().c_str());
    }

    else if (text.size() > 13 && text.substr(0,13) == "accept quest ")
    {
        uint32 questId = 0;
        std::string questStr = text.substr(text.find(" ") + 1);
        questStr = questStr.substr(questStr.find(" ") + 1);
        std::string questStrLink = questStr;
        if (questStr.find("Hquest:"))
        {
            questStr = questStr.substr(questStr.find("|h[") + 3);
            questStr = questStr.substr(0, questStr.find("]"));
        }
        else
            questId = (uint32)atol(questStr.c_str());

        // try and get quest ID by name
        if (questId == 0)
            questId = getQuestId(questStr.c_str(), 0);
        std::ostringstream out;
        out << "Quest " << questId << " " << questStr.c_str();

        PlayerbotChatHandler ch(m_master);
        uint64 oldSel = 0;
        if (m_master->GetSelection())
            oldSel = m_master->GetSelection();
        m_master->SetSelection(m_bot->GetGUID());
        if (questId != 0 && ch.acceptQuest(questStrLink.c_str()))
        {
            out << " accepted.";
            TellMaster(out.str());
        }
        else
        {
            out << " not accepted.";
            TellMaster(out.str());
        }
        if (oldSel > 0)
            m_master->SetSelection(oldSel);
    }

    else if (text.size() > 14 && text.substr(0,14) == "abandon quest ")
    {
        uint32 questId = 0;
        std::string questStr = text.substr(text.find(" ") + 1);
        questStr = questStr.substr(questStr.find(" ") + 1);
        std::string questStrLink = questStr;
        if (questStr.find("Hquest:"))
        {
            questStr = questStr.substr(questStr.find("|h[") + 3);
            questStr = questStr.substr(0, questStr.find("]"));
        }
        else
            questId = (uint32)atol(questStr.c_str());

        // try and get quest ID by name
        if (questId == 0)
            questId = getQuestId(questStr.c_str(), 1);
        std::ostringstream out;
        out << "Quest " << questId << " " << questStr.c_str();

        PlayerbotChatHandler ch(m_master);
        uint64 oldSel = 0;
        if (m_master->GetSelection())
            oldSel = m_master->GetSelection();
        m_master->SetSelection(m_bot->GetGUID());
        if (questId != 0 && ch.abandonQuest(questStrLink.c_str()))
        {
            out << " abandoned.";
            TellMaster(out.str());
        }
               else
        {
            out << " not abandoned.";
            TellMaster(out.str());
        }
        if (oldSel > 0)
            m_master->SetSelection(oldSel);
    }

    else if (text.size() == 1 && text.substr(0,1) == "q" ||
        text == "quests")
    {
        std::ostringstream out;
        for (QuestStatusMap::iterator iter=m_bot->getQuestStatusMap().begin(); iter!=m_bot->getQuestStatusMap().end(); ++iter)
        {
            const Quest *qInfo = sObjectMgr->GetQuestTemplate(iter->first);
            if (!qInfo)
                continue;
            QuestStatusData *qData = &iter->second;

            uint32 questId = qInfo->GetQuestId();
            const std::string name = qInfo->GetTitle();
            if (name.empty())
                continue;
            //out << qData->m_status << " ";
            if (qData->m_status == QUEST_STATUS_NONE)
            {
                //out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                //out << " no status" << "\n";
                continue;
            }
            else if (qData->m_status == QUEST_STATUS_COMPLETE)
            {
                out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                out << " complete" << "\n";
            }
            else if (qData->m_status == QUEST_STATUS_UNAVAILABLE)
            {
                //out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                //out << " unavailable" << "\n";
                continue;
            }
            else if (qData->m_status == QUEST_STATUS_INCOMPLETE)
            {
                out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                out << " incomplete" << "\n";
            }
            else if (qData->m_status == QUEST_STATUS_AVAILABLE)
            {
                //out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                //out << " available" << "\n";
                continue;
            }
            else if (qData->m_status == QUEST_STATUS_FAILED)
            {
                out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                out << " failed" << "\n";
            }
            else
            {
                //out << "Quest " << " |cffffffff|Hquest:" << questId << "|h[" << name << "]|h|r" << " ";
                //out << " unknown" << "\n";
                continue;
            }
        }
        if (!out.str().empty())
            TellMaster(out.str());
    }

    else if (text == "train")
    {
         Unit *unit = m_master->GetSelectedUnit();

         if (!unit)
         {
             TellMaster("Please select the trainer which I should learn  from!");
             return;
         }
         if (!unit->isTrainer())
         {
             TellMaster("This is not a trainer!");
             return;
         }

         Creature *creature =  m_bot->GetMap()->GetCreature(m_master->GetSelection());
         if(!creature->isCanTrainingOf(m_bot, false))
         {
             TellMaster("This trainer can not train me anything at  all!");
             return;
         }

         CreatureInfo const *ci = creature->GetCreatureInfo();

         if (!ci)
         {
             TellMaster("This trainer can not train me anything at  all!");
             return;
         }

         TrainerSpellData const* trainer_spells =  creature->GetTrainerSpells();
         if(!trainer_spells)
         {
             TellMaster("No training spells can be found from this  trainer");
             return;
         }
         // reputation discount
         float fDiscountMod =  m_bot->GetReputationPriceDiscount(creature);
         bool can_learn_primary_prof =  m_bot->GetFreePrimaryProfessionPoints() > 0;

         std::ostringstream msg;
         msg << "I had learnt the following spells:\n";
         uint32 totalCost = 0;
         uint32 totalSpellLearnt = 0;
         int loc = m_master->GetSession()->GetSessionDbcLocale();
         for (TrainerSpellMap::const_iterator itr =  trainer_spells->spellList.begin(); itr !=  trainer_spells->spellList.end(); ++itr)
         {
             TrainerSpell const* tSpell = &itr->second;

             bool valid = true;
             bool primary_prof_first_rank = false;
             for (uint8 i = 0; i < MAX_SPELL_EFFECTS ; ++i)
             {
                 if (!tSpell->learnedSpell[i])
                     continue;
                  if(!m_bot->IsSpellFitByClassAndRace(tSpell->learnedSpell[i]))
                 {
                     valid = false;
                     break;
                 }
                 if  (sSpellMgr->IsPrimaryProfessionFirstRankSpell(tSpell->learnedSpell[i]))
                     primary_prof_first_rank = true;
             }
             if (!valid)
                 continue;

             TrainerSpellState state =  m_bot->GetTrainerSpellState(tSpell);
             if (state != TRAINER_SPELL_GREEN)
             {
                 continue;
             }
             //data << uint32(primary_prof_first_rank &&  can_learn_primary_prof ? 1 : 0);
             uint32 spellId = tSpell->spell;
             const SpellEntry *const pSpellInfo =  sSpellStore.LookupEntry(spellId);
             if (!pSpellInfo)
             {
                 continue;
             }
             uint32 cost = (uint32) (floor(tSpell->spellCost *  fDiscountMod));

             // check money requirement
             if(m_bot->GetMoney() < cost )
                 continue;

             m_bot->ModifyMoney( -int32(cost) );

             // learn explicitly or cast explicitly
             if(tSpell->IsCastable())
                 //FIXME: prof. spell entry in trainer list not marked  gray until list re-open.
                 m_bot->CastSpell(m_bot,tSpell->spell,true);
             else
                 m_bot->learnSpell(spellId,false);
             totalSpellLearnt++;
             totalCost = totalCost + cost;

             msg << " |cffffffff|Hspell:" << spellId <<  "|h[" << pSpellInfo->SpellName[loc] << "]|h|r" <<  ", ";
             uint32 gold = uint32(cost / 10000);
             cost -= (gold * 10000);
             uint32 silver = uint32(cost / 100);
             cost -= (silver * 100);
             if (gold > 0)
             {
                 msg << gold <<  "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
             }
             if (silver > 0)
             {
                 msg << silver <<  "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
             }
             msg << cost <<  "|TInterface\\Icons\\INV_Misc_Coin_05:16|t\n";
         }

         uint32 gold = uint32(totalCost / 10000);
         totalCost -= (gold * 10000);
         uint32 silver = uint32(totalCost / 100);
         totalCost -= (silver * 100);
         msg << "Total of " << totalSpellLearnt << "  spell(s) learnt, ";
         if (gold > 0)
         {
             msg << gold <<  "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
         }
         if (silver > 0)
         {
             msg << silver <<  "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
         }
         msg << totalCost <<  "|TInterface\\Icons\\INV_Misc_Coin_05:16|t spent.";

         TellMaster(msg.str());

         m_bot->GetPlayerbotAI()->GetClassAI()->LoadSpells();

    }
    else if (text == "train list")
    {
         Unit *unit = m_master->GetSelectedUnit();

         if (!unit)
         {
             TellMaster("Please select the trainer which I should learn  from!");
             return;
         }
         if (!unit->isTrainer())
         {
             TellMaster("This is not a trainer!");
             return;
         }

         Creature *creature =  m_bot->GetMap()->GetCreature(m_master->GetSelection());
         if(!creature->isCanTrainingOf(m_bot, false))
         {
             TellMaster("This trainer can not train me anything at  all!");
             return;
         }

         CreatureInfo const *ci = creature->GetCreatureInfo();

         if (!ci)
         {
             TellMaster("This trainer can not train me anything at  all!");
             return;
         }

         TrainerSpellData const* trainer_spells =  creature->GetTrainerSpells();
         if(!trainer_spells)
         {
             TellMaster("No training spells can be found from this  trainer");
             return;
         }
         // reputation discount
         float fDiscountMod =  m_bot->GetReputationPriceDiscount(creature);
         bool can_learn_primary_prof =  m_bot->GetFreePrimaryProfessionPoints() > 0;

         std::ostringstream msg;
         msg << "The spells I can learn and their costs are:\n";
         uint32 totalCost = 0;
         int loc = m_master->GetSession()->GetSessionDbcLocale();
         for (TrainerSpellMap::const_iterator itr =  trainer_spells->spellList.begin(); itr !=  trainer_spells->spellList.end(); ++itr)
         {
             TrainerSpell const* tSpell = &itr->second;

             bool valid = true;
             bool primary_prof_first_rank = false;
             for (uint8 i = 0; i < MAX_SPELL_EFFECTS ; ++i)
             {
                 if (!tSpell->learnedSpell[i])
                     continue;
                  if(!m_bot->IsSpellFitByClassAndRace(tSpell->learnedSpell[i]))
                 {
                     valid = false;
                     break;
                 }
                 if  (sSpellMgr->IsPrimaryProfessionFirstRankSpell(tSpell->learnedSpell[i]))
                     primary_prof_first_rank = true;
             }
             if (!valid)
                 continue;

             TrainerSpellState state =  m_bot->GetTrainerSpellState(tSpell);
             if (state != TRAINER_SPELL_GREEN)
             {
                 continue;
             }
             //data << uint32(primary_prof_first_rank &&  can_learn_primary_prof ? 1 : 0);
             uint32 spellId = tSpell->spell;
             const SpellEntry *const pSpellInfo =  sSpellStore.LookupEntry(spellId);
             if (!pSpellInfo)
             {
                 continue;
             }
             uint32 cost = (uint32) (floor(tSpell->spellCost *  fDiscountMod));
             totalCost = totalCost + cost;

             uint32 gold = uint32(cost / 10000);
             cost -= (gold * 10000);
             uint32 silver = uint32(cost / 100);
             cost -= (silver * 100);
             msg << " |cffffffff|Hspell:" << spellId <<  "|h[" << pSpellInfo->SpellName[loc] << "]|h|r" <<  ", ";
             if (gold > 0)
             {
                 msg << gold <<  "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
             }
             if (silver > 0)
             {
                 msg << silver <<  "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
             }
             msg << cost <<  "|TInterface\\Icons\\INV_Misc_Coin_05:16|t\n";
         }
         uint32 moneyDiff = m_bot->GetMoney() - totalCost;
         if (moneyDiff >= 0)
         {
             uint32 gold = uint32(moneyDiff / 10000);
             moneyDiff -= (gold * 10000);
             uint32 silver = uint32(moneyDiff / 100);
             moneyDiff -= (silver * 100);
             if (gold > 0)
             {
                 msg << gold <<  "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
             }
             if (silver > 0)
             {
                 msg << silver <<  "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
             }
             msg << moneyDiff <<  "|TInterface\\Icons\\INV_Misc_Coin_05:16|t left.";
             }
         else
         {
             moneyDiff = moneyDiff * (-1);
             uint32 gold = uint32(moneyDiff / 10000);
             moneyDiff -= (gold * 10000);
             uint32 silver = uint32(moneyDiff / 100);
             moneyDiff -= (silver * 100);
             msg << "I need ";
             if (gold > 0)
             {
                 msg << gold <<  "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
             }
             if (silver > 0)
             {
                 msg << silver <<  "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
             }
             msg << moneyDiff <<  "|TInterface\\Icons\\INV_Misc_Coin_05:16|t more to learn all the  spells!";
         }
         TellMaster(msg.str());

    }
 else if(text.size() >= 4 && text.substr(0, 4) == "sell")
    {
        Unit *unit = m_master->GetSelectedUnit();

        if (!unit)
        {
            TellMaster("Please show me who I should trade with!");
            return;
        }
        if (!unit->isVendor())
        {
            TellMaster("This person does not want to trade with me!");
            return;
        }
        if (!m_bot->IsInMap((WorldObject*) unit))
        {
            TellMaster("I'm too far away to sell items!");
            return;
        }
        uint32 TotalCost = 0;
        uint32 TotalSold = 0;
        std::ostringstream report;

        std::list<uint32> itemIds;
        std::list<Item *> itemList;
        extractItemIds(text, itemIds);
        findItemsInInv(itemIds, itemList);
        for(std::list<Item *>::iterator it = itemList.begin(); it != itemList.end(); ++it)
        {
            if ((**it).GetProto()->SellPrice > 0)
            {
                int32 cost = (**it).GetCount() * (**it).GetProto()->SellPrice;
                m_bot->ModifyMoney(cost);
                m_bot->MoveItemFromInventory((**it).GetBagSlot(), (**it).GetSlot(), true);

                TotalSold = TotalSold + 1;
                TotalCost = TotalCost + cost;

                if ((**it).GetCount() > 0) {
                    report << "Sold " << (**it).GetCount() << "x";
                    report << " |cffffffff|Hitem:" << (**it).GetProto()->ItemId << ":0:0:0:0:0:0:0" << "|h[" << (**it).GetProto()->Name1 << "]|h|r";
                    report << " for ";

                    uint32 gold = uint32(cost / 10000);
                    cost -= (gold * 10000);
                    uint32 silver = uint32(cost / 100);
                    cost -= (silver * 100);

                    if (gold > 0)
                    {
                        report << gold << "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
                    }
                    if (silver > 0)
                    {
                        report << silver << "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
                    }
                    report << cost << "|TInterface\\Icons\\INV_Misc_Coin_05:16|t\n";
                }
            }
        }
        if (TotalSold > 0) {
            report << "Sold total " << TotalSold << " item(s) for ";

            uint32 gold = uint32(TotalCost / 10000);
            TotalCost -= (gold * 10000);
            uint32 silver = uint32(TotalCost / 100);
            TotalCost -= (silver * 100);

            if (gold > 0)
            {
                report << gold << "|TInterface\\Icons\\INV_Misc_Coin_01:16|t";
            }
            if (silver > 0)
            {
                report << silver << "|TInterface\\Icons\\INV_Misc_Coin_03:16|t";
            }
            report << TotalCost << "|TInterface\\Icons\\INV_Misc_Coin_05:16|t.";
            TellMaster(report.str());
        }
    }

 else if (text.size() > 2 && text.substr(0, 2) == "g " || text.size() > 4 && text.substr(0, 4) == "get ")
 {
     uint32 guid;
     float x,y,z;
     uint32 entry;
     int mapid;
     if (extractGOinfo(text, guid, entry, mapid, x, y, z))
     {
         sLog->outDebug("find: guid : %u entry : %u x : (%f) y : (%f) z : (%f) mapid : %d",guid, entry, x, y, z, mapid);
         m_lootCurrent = MAKE_NEW_GUID(guid, entry, HIGHGUID_GAMEOBJECT);
         GameObject *go = m_bot->GetMap()->GetGameObject(m_lootCurrent);
         if (!go)
         {
             m_bot->Say("I can't find it.", LANG_UNIVERSAL);
             m_lootCurrent = 0;
             return;
         }

         if ( !go->isSpawned() ) {
             m_bot->Say("It is not there anymore.", LANG_UNIVERSAL);
             return;
         }

         m_bot->UpdateGroundPositionZ(x,y,z);
         m_bot->GetMotionMaster()->MovePoint( mapid, x, y, z );
         m_bot->SetPosition(x, y, z, m_bot->GetOrientation());
         m_bot->SendLoot( m_lootCurrent, LOOT_CORPSE );
         Loot *loot = &go->loot;
         uint32 lootNum = loot->GetMaxSlotInLootFor( m_bot );

         sLog->outDebug( "[PlayerbotAI]: GetGOType %u - %s looting: '%s' got %d items", go->GetGoType(), m_bot->GetName(), go->GetGOInfo()->name, loot->GetMaxSlotInLootFor( m_bot ));
         for ( uint32 l=0; l<lootNum; l++ )
         {
             QuestItem *qitem=0, *ffaitem=0, *conditem=0;
             LootItem *item = loot->LootItemInSlot( l, m_bot, &qitem, &ffaitem, &conditem );
             if ( !item )
                 continue;

             if ( !qitem && item->is_blocked )
             {
                 m_bot->SendLootRelease( m_lootCurrent );
                 continue;
             }

             if ( m_needItemList[item->itemid]>0 )
             {
                 ItemPosCountVec dest;
                 if ( m_bot->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, item->itemid, item->count ) == EQUIP_ERR_OK )
                 {
                     Item * newitem = m_bot->StoreNewItem( dest, item->itemid, true, item->randomPropertyId);
                     if ( qitem )
                     {
                         qitem->is_looted = true;
                         if ( item->freeforall || loot->GetPlayerQuestItems().size() == 1 )
                             m_bot->SendNotifyLootItemRemoved( l );
                         else
                             loot->NotifyQuestItemRemoved( qitem->index );
                     }
                     else
                     {
                         if ( ffaitem )
                         {
                             ffaitem->is_looted=true;
                             m_bot->SendNotifyLootItemRemoved( l );
                         }
                         else
                         {
                             if ( conditem )
                                 conditem->is_looted=true;
                             loot->NotifyItemRemoved( l );
                         }
                     }
                     if (!item->freeforall)
                         item->is_looted = true;
                     --loot->unlootedCount;
                     m_bot->SendNewItem( newitem, uint32(item->count), false, false, true );

                     m_bot->GetAchievementMgr().UpdateAchievementCriteria( ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item->itemid, item->count );
                 }
             }
             uint32 lockId = go->GetGOInfo()->GetLockId();
             LockEntry const *lockInfo = sLockStore.LookupEntry(lockId);
             if(lockInfo)
             {
                 uint32 skillId = SkillByLockType(LockType(lockInfo->Index[0]));
                 switch(skillId)
                 {
                 case SKILL_MINING:
                     if (m_bot->HasSkill(SKILL_MINING) && HasPick()) // Has skill & suitable pick
                     {
                         ItemPosCountVec dest;
                         if ( m_bot->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, item->itemid, item->count) == EQUIP_ERR_OK )
                         {
                             Item* pItem = m_bot->StoreNewItem (dest,item->itemid,true,item->randomPropertyId);
                             uint32 reqSkillValue = lockInfo->Skill[0];
                             uint32 SkillValue = m_bot->GetPureSkillValue(SKILL_MINING);
                             if (SkillValue >= reqSkillValue)
                             {
                                 m_bot->SendNewItem(pItem, uint32(item->count), false, false, true);
                                 m_bot->UpdateGatherSkill(SKILL_MINING, SkillValue, reqSkillValue);
                                 --loot->unlootedCount;
                             }
                         }
                     }
                     break;
                 case SKILL_HERBALISM:
                     if (m_bot->HasSkill(SKILL_HERBALISM)) // Has skill
                     {
                         ItemPosCountVec dest;
                         if ( m_bot->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, item->itemid, item->count) == EQUIP_ERR_OK )

                         {
                             Item* pItem = m_bot->StoreNewItem (dest,item->itemid,true,item->randomPropertyId);
                             uint32 reqSkillValue = lockInfo->Skill[0];
                             uint32 SkillValue = m_bot->GetPureSkillValue(SKILL_HERBALISM);
                             if (SkillValue >= reqSkillValue)
                             {
                                 m_bot->SendNewItem(pItem, uint32(item->count), false, false, true);
                                 m_bot->UpdateGatherSkill(SKILL_HERBALISM, SkillValue, reqSkillValue);
                                 --loot->unlootedCount;
                             }
                         }
                     }
                     break;
                 }
             }
         }
         // release loot
         m_bot->GetSession()->DoLootRelease( m_lootCurrent );

         // clear movement target, take next target on next update
         m_bot->GetMotionMaster()->Clear();
         m_bot->GetMotionMaster()->MoveIdle();
         sLog->outDebug( "[PlayerbotAI]: %s looted target 0x%08X", m_bot->GetName(), m_lootCurrent );
         SetQuestNeedItems();
     }
     else
         SendWhisper("I have no info on that object", fromPlayer);
 }

 else if (text == "survey")
 {
      float distance = 100.0f;
      uint32 count = 0;
      std::ostringstream detectout;

      QueryResult result = WorldDatabase.PQuery("SELECT guid, id, position_x, position_y, position_z, map, "
        "(POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) AS order_ "
        "FROM gameobject WHERE map='%u' AND (POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) <= '%f' ORDER BY order_",
         m_bot->GetPositionX(), m_bot->GetPositionY(), m_bot->GetPositionZ(),
         m_bot->GetMapId(), m_bot->GetPositionX(), m_bot->GetPositionY(), m_bot->GetPositionZ(), distance*distance);

      if (result)
      {
         do
         {
             Field *fields = result->Fetch();
             uint32 guid = fields[0].GetUInt32();
             uint32 entry = fields[1].GetUInt32();
             float x = fields[2].GetFloat();
             float y = fields[3].GetFloat();
             float z = fields[4].GetFloat();
             int mapid = fields[5].GetUInt16();

             GameObjectInfo const * gInfo = ObjectMgr::GetGameObjectInfo(entry);

             if(!gInfo)
                 continue;

             uint64 objGuid = MAKE_NEW_GUID(guid, entry, HIGHGUID_GAMEOBJECT);
             GameObject *go = m_bot->GetMap()->GetGameObject(objGuid);
             if ( !go || (go && !go->isSpawned()) ) continue;

             if(count < 12) // count, limits number of links
                  detectout << "|cFFFFFF00|Hfound:" << guid << ":" << entry << ":" << x << ":" << y << ":" << z  << ":" << mapid  << ":" <<  "|h[" << gInfo->name << "]|h|r";
             ++count;
         } while (result->NextRow());

      }
      SendWhisper(detectout.str().c_str(), fromPlayer);
    }

    else if (text == "pull")
    {
        m_bot->GetPlayerbotAI()->GetClassAI()->Pull();
    }
    else if(text == "help")
    {
        std::string msg;
        if (m_bot->getClass() == CLASS_ROGUE)
            msg = "The commands I respond to are \n follow, stay, (c)ast <spellname>, spells, (e)quip, (u)se, \nnpcbot (a)dd, npcbot (d)elete <class>,\n(q)uests, accept quest <hlink>, abandon quest <hlink>\npoison [main | off].\ntrain list";
        else
            msg = "The commands I respond to are \n follow, stay, (c)ast <spellname>, spells, (e)quip, (u)se, \nnpcbot (a)dd, npcbot (d)elete <class>,\n(q)uests, accept quest <hlink>, abandon quest <hlink>\ntrain list.";

        SendWhisper(msg, fromPlayer);
        m_bot->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
    }
      else
    {
        // if this looks like an item link, reward item it completed quest and talking to NPC
        std::list<uint32> itemIds;
        extractItemIds(text, itemIds);
        if (!itemIds.empty()) {
            uint32 itemId = itemIds.front();
            bool wasRewarded = false;
            uint64 questRewarderGUID = m_bot->GetSelection();
            Object* const pNpc = ObjectAccessor::GetObjectByTypeMask(*m_bot, questRewarderGUID, TYPEMASK_UNIT|TYPEMASK_GAMEOBJECT);
            if (!pNpc)
                return;

            QuestMenu& questMenu = m_bot->PlayerTalkClass->GetQuestMenu();
            for (uint32 iI = 0; !wasRewarded && iI < questMenu.MenuItemCount(); ++iI)
            {
                QuestMenuItem const& qItem = questMenu.GetItem(iI);

                uint32 questID = qItem.m_qId;
                Quest const* pQuest = sObjectMgr->GetQuestTemplate(questID);
                QuestStatus status = m_bot->GetQuestStatus(questID);

                // if quest is complete, turn it in
                if (status == QUEST_STATUS_COMPLETE &&
                    ! m_bot->GetQuestRewardStatus(questID) &&
                    pQuest->GetRewChoiceItemsCount() > 1 &&
                    m_bot->CanRewardQuest(pQuest, false))
                {
                    for (uint8 rewardIdx=0; !wasRewarded && rewardIdx < pQuest->GetRewChoiceItemsCount(); ++rewardIdx)
                    {
                        ItemPrototype const * const pRewardItem = sObjectMgr->GetItemPrototype(pQuest->RewChoiceItemId[rewardIdx]);
                        if (itemId == pRewardItem->ItemId)
                        {
                            m_bot->RewardQuest(pQuest, rewardIdx, pNpc, false);

                            std::string questTitle  = pQuest->GetTitle();
                            m_bot->GetPlayerbotAI()->QuestLocalization(questTitle, questID);
                            std::string itemName = pRewardItem->Name1;
                            m_bot->GetPlayerbotAI()->ItemLocalization(itemName, pRewardItem->ItemId);

                            std::ostringstream out;
                            out << "|cffffffff|Hitem:" << pRewardItem->ItemId << ":0:0:0:0:0:0:0" << "|h[" << itemName << "]|h|r rewarded";
                            SendWhisper(out.str(), fromPlayer);
                            wasRewarded = true;
                        }
                    }
                }
            }
        }
    }

}

void PlayerbotAI::SetLooting(bool looting)
{
    isLooting = looting;
}

void PlayerbotAI::AddLootGUID(uint64 guid) {
    m_lootCreature.push_back(guid);
}

bool PlayerbotAI::DoLoot()
{
    if(!m_lootCurrent && m_lootCreature.empty())
    {
        sLog->outDebug("[PlayerbotAI]: %s reset loot list / go back to idle", m_bot->GetName());
//        SetQuestNeedItems();
        isLooting = false;
        return false;
    }

    if(m_bot->isDead())
    {
        isLooting = false;
        return false;
    }

    if(!m_lootCurrent)
    {
        m_lootCurrent = m_lootCreature.front();

        if(!m_lootCurrent)
        {
            //sLog->outError("PlayerbotAI::DoLoot() error location #1, please report this error immediately!");
            return false;
        }

        Creature *c = m_bot->GetMap()->GetCreature(m_lootCurrent);
        GameObject *o = m_bot->GetMap()->GetGameObject( m_lootCurrent );

        if(!c && !o)
        {
            m_lootCurrent = 0;
            isLooting = false;
            return false;
        }
        if(c && c->isAlive())
        {
            m_lootCurrent = 0;
            isLooting = false;
            return false; //not dead yet
        }

        sLog->outDebug("[PlayerbotAI]: %s got loot target 0x%08X", m_bot->GetName(), m_lootCurrent);
        Position pos;

        WorldObject *object;
        if (c) object = c;
        else object = o;

        if(m_bot->IsWithinDistInMap(object, INTERACTION_DISTANCE * 3)){ //Verify if the bot it close to a loot.

            m_lootCreature.pop_front();
            object->GetPosition(&pos);
            m_bot->GetMotionMaster()->MovePoint(object->GetMapId(), (const Position &)(pos));

        } else { //Rotate the loot to very if one is not near the bot.

            m_lootobjtemp = m_lootCreature.front();
            m_lootCreature.pop_front();
            m_lootCreature.push_back(m_lootobjtemp);
            m_lootCurrent = 0;
        }
        return true;

    } else {

        Creature *c = m_bot->GetMap()->GetCreature(m_lootCurrent);
        GameObject *o = m_bot->GetMap()->GetGameObject( m_lootCurrent );
        if(!c && !o)
        {
            m_lootCurrent = 0;
            isLooting = false;
            return false;
        }
        if(c && c->isAlive())
        {
            m_lootCurrent = 0;
            isLooting = false;
            return false; //not dead yet
        }

        WorldObject *object;
        if (c) object = c;
        else object = o;

        if(m_bot->IsWithinDistInMap(object, INTERACTION_DISTANCE))
        {
            //check for needed items
            m_bot->SendLoot(m_lootCurrent, LOOT_CORPSE);

            Loot *loot;
            if (c)
                loot = &c->loot;
            else
                loot = &o->loot;

            assert(loot);

            uint32 lootNum = loot->GetMaxSlotInLootFor(m_bot);
//sLog->outError("[PlayerbotAI]: %s loot target 0x%08X got %d items", m_bot->GetName(), m_lootCurrent, loot->GetMaxSlotInLootFor(m_bot));

            for(uint32 l = 0; l < lootNum; ++l)
            {
                QuestItem *qitem = 0, *ffaitem = 0, *conditem = 0;
                LootItem *item = loot->LootItemInSlot(l, m_bot, &qitem, &ffaitem, &conditem);
                if(!item) continue;
                if(!qitem && item->is_blocked)
                {
                    m_bot->SendLootRelease(m_bot->GetLootGUID());
                    continue;
                }

                if(m_needItemList[item->itemid] > 0)
                {
//sLog->outError("[PlayerbotAI]: %s LOOT needed item 0x%04X", m_bot->GetName(), item->itemid);
                    ItemPosCountVec dest;
                    if(m_bot->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item->itemid, item->count) == EQUIP_ERR_OK)
                    {
                        Item *newitem = m_bot->StoreNewItem(dest, item->itemid,true, item->randomPropertyId);
                        if(qitem)
                        {
                            qitem->is_looted = true;
                            if(item->freeforall ||loot->GetPlayerQuestItems().size() == 1)
                                m_bot->SendNotifyLootItemRemoved(l);
                            else
                                loot->NotifyQuestItemRemoved(qitem->index);
                        }
                        else if(ffaitem)
                        {
                            ffaitem->is_looted = true;
                            m_bot->SendNotifyLootItemRemoved(l);
                        } else {
                            if(conditem) conditem->is_looted = true;
                            loot->NotifyItemRemoved(l);
                        }
                        if(!item->freeforall) item->is_looted = true;
                        --(loot->unlootedCount);
                        m_bot->SendNewItem(newitem, uint32(item->count), false,false, true);
                        m_bot->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM, item->itemid, item->count);

                        std::ostringstream out;
                        PlayerbotChatHandler ch(m_master);
                        out << m_bot->GetName() << " needs " << m_needItemList[item->itemid]-1 << " more.";
                        ch.sysmessage(out.str().c_str());
                        m_ignoreAIUpdatesUntilTime = time(0);
                    }
                }
            }
            //release loot
            if(uint64 lguid = m_bot->GetLootGUID() && m_bot->GetSession())
                m_bot->GetSession()->DoLootRelease(lguid);
            else if(!m_bot->GetSession())
                sLog->outDebug("[PlayerbotAI]: %s has no session. Cannot releaseloot!", m_bot->GetName());

            //clear movement target, take next target on next update
            m_bot->GetMotionMaster()->Clear();
            m_bot->GetMotionMaster()->MoveIdle();
            sLog->outDebug("[PlayerbotAI]: %s looted target 0x%08X", m_bot->GetName(), m_lootCurrent);
            m_lootCurrent = 0;
            m_ignoreAIUpdatesUntilTime = time(0);
            isLooting = false;
        } else { //keep moving till we get there
            Position pos;

            if (c) {
                c->GetPosition(&pos);
                m_bot->GetMotionMaster()->MovePoint(c->GetMapId(), (const Position &)(pos));
            } else {
                o->GetPosition(&pos);
                m_bot->GetMotionMaster()->MovePoint(o->GetMapId(), (const Position &)(pos));
            }

        }
    }
    return false;
} //end DoLoot

void PlayerbotAI::SetQuestNeedItems()
{
    //reset values first
    m_needItemList.clear();
    m_lootCreature.clear();
    m_lootCurrent = 0;

    //run through accepted quests, get quest infoand data
    for(QuestStatusMap::iterator iter=m_bot->getQuestStatusMap().begin(); iter!=m_bot->getQuestStatusMap().end(); ++iter)
    {
        const Quest *qInfo = sObjectMgr->GetQuestTemplate(iter->first);
        if(!qInfo) continue;
        QuestStatusData *qData = &iter->second;

        //only check quest if it is incomplete
        if(qData->m_status != QUEST_STATUS_INCOMPLETE) continue;

        //check for items we not have enough of
        for(uint8 i = 0; i < QUEST_OBJECTIVES_COUNT; i++)
        {
            if(!qInfo->ReqItemCount[i] || (qInfo->ReqItemCount[i]-qData->m_itemcount[i]) <= 0) continue;
            m_needItemList[qInfo->ReqItemId[i]] = (qInfo->ReqItemCount[i]-qData->m_itemcount[i]);
        }
    }
}//end SetQuestNeedItems

//Localization support
void PlayerbotAI::ItemLocalization(std::string &itemName, const uint32 itemID) const
{
    int loc = m_master->GetSession()->GetSessionDbLocaleIndex();
    std::wstring wnamepart;
    ItemLocale const *pItemInfo = sObjectMgr->GetItemLocale(itemID);
    if(pItemInfo)
    {
        if(pItemInfo->Name.size() > loc && !pItemInfo->Name[loc].empty())
        {
            const std::string name = pItemInfo->Name[loc];
            if(Utf8FitTo(name, wnamepart))
                itemName = name.c_str();
        }
    }
}

void PlayerbotAI::QuestLocalization(std::string &questTitle, const uint32 questID) const
{
    int loc = m_master->GetSession()->GetSessionDbLocaleIndex();
    std::wstring wnamepart;
    QuestLocale const *pQuestInfo = sObjectMgr->GetQuestLocale(questID);
    if(pQuestInfo)
    {
        if(pQuestInfo->Title.size() > loc && !pQuestInfo->Title[loc].empty())
        {
            const std::string title = pQuestInfo->Title[loc];
            if(Utf8FitTo(title, wnamepart))
                questTitle = title.c_str();
        }
    }
}

void PlayerbotAI::TurnInQuests( WorldObject *pNpc )
{

    uint64 npcGUID = pNpc->GetGUID();
    if (!m_bot->IsInMap((WorldObject*) pNpc))
        m_bot->GetPlayerbotAI()->TellMaster("hey you are turning in quests without me!");
    else
    {
        m_bot->SetSelection(npcGUID);

        // auto complete every completed quest this NPC has
        m_bot->PrepareQuestMenu(npcGUID);
        QuestMenu& questMenu = m_bot->PlayerTalkClass->GetQuestMenu();
        for (uint32 iI = 0; iI < questMenu.MenuItemCount(); ++iI)
        {
            QuestMenuItem const& qItem = questMenu.GetItem(iI);
            uint32 questID = qItem.m_qId;
            Quest const* pQuest = sObjectMgr->GetQuestTemplate(questID);

            std::ostringstream out;
            std::string questTitle  = pQuest->GetTitle();
            m_bot->GetPlayerbotAI()->QuestLocalization(questTitle, questID);

            QuestStatus status = m_bot->GetQuestStatus(questID);
            // if quest is complete, turn it in
            if (status == QUEST_STATUS_COMPLETE)
            {
                // if bot hasn't already turned quest in
                if (! m_bot->GetQuestRewardStatus(questID))
                {
                    // auto reward quest if no choice in reward
                    if (pQuest->GetRewChoiceItemsCount() == 0)
                    {
                        if (m_bot->CanRewardQuest(pQuest, false))
                        {
                            m_bot->RewardQuest(pQuest, 0, pNpc, false);
                            out << "Quest complete: |cff808080|Hquest:" << questID << ':' << pQuest->GetQuestLevel() << "|h[" << questTitle << "]|h|r";
                        }
                        else
                        {
                            out << "|cffff0000Unable to turn quest in:|r |cff808080|Hquest:" << questID << ':' << pQuest->GetQuestLevel() << "|h[" << questTitle << "]|h|r";
                        }
                    }

                    // auto reward quest if one item as reward
                    else if (pQuest->GetRewChoiceItemsCount() == 1)
                    {
                        int rewardIdx = 0;
                        ItemPrototype const *pRewardItem = sObjectMgr->GetItemPrototype(pQuest->RewChoiceItemId[rewardIdx]);
                        std::string itemName = pRewardItem->Name1;
                        m_bot->GetPlayerbotAI()->ItemLocalization(itemName, pRewardItem->ItemId);
                        if (m_bot->CanRewardQuest(pQuest, rewardIdx, false))
                        {
                            m_bot->RewardQuest(pQuest, rewardIdx, pNpc, true);

                            std::string itemName = pRewardItem->Name1;
                            m_bot->GetPlayerbotAI()->ItemLocalization(itemName, pRewardItem->ItemId);

                            out << "Quest complete: "
                                << " |cff808080|Hquest:" << questID << ':' << pQuest->GetQuestLevel()
                                << "|h[" << questTitle << "]|h|r reward: |cffffffff|Hitem:"
                                << pRewardItem->ItemId << ":0:0:0:0:0:0:0" << "|h[" << itemName << "]|h|r";
                        }
                        else
                        {
                            out << "|cffff0000Unable to turn quest in:|r "
                                << "|cff808080|Hquest:" << questID << ':'
                                << pQuest->GetQuestLevel() << "|h[" << questTitle << "]|h|r"
                                << " reward: |cffffffff|Hitem:"
                                << pRewardItem->ItemId << ":0:0:0:0:0:0:0" << "|h[" << itemName << "]|h|r";
                        }
                    }

                    // else multiple rewards - let master pick
                    else {
                        out << "What reward should I take for |cff808080|Hquest:" << questID << ':' << pQuest->GetQuestLevel()
                            << "|h[" << questTitle << "]|h|r? ";
                        for (uint8 i=0; i < pQuest->GetRewChoiceItemsCount(); ++i)
                        {
                            ItemPrototype const * const pRewardItem = sObjectMgr->GetItemPrototype(pQuest->RewChoiceItemId[i]);
                            std::string itemName = pRewardItem->Name1;
                            m_bot->GetPlayerbotAI()->ItemLocalization(itemName, pRewardItem->ItemId);
                            out << "|cffffffff|Hitem:" << pRewardItem->ItemId << ":0:0:0:0:0:0:0" << "|h[" << itemName << "]|h|r";
                        }
                    }
                }
            }

            else if (status == QUEST_STATUS_INCOMPLETE) {
                out << "|cffff0000Quest incomplete:|r "
                    << " |cff808080|Hquest:" << questID << ':' << pQuest->GetQuestLevel() << "|h[" << questTitle << "]|h|r";
            }

//            else if (status == QUEST_STATUS_AVAILABLE){
  //              out << "|cff00ff00Quest available:|r "
//                    << " |cff808080|Hquest:" << questID << ':' << pQuest->GetQuestLevel() << "|h[" << questTitle << "]|h|r";
//            }

            if (! out.str().empty())
                m_bot->GetPlayerbotAI()->TellMaster(out.str());
        }
    }
} // TurnInQuests


 void PlayerbotAI::SetCombatOrder (CombatOrderType orders)
 {
    m_combatOrder = orders;
 }
