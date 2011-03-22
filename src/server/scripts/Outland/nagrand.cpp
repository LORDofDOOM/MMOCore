/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Nagrand
SD%Complete: 90
SDComment: Quest support: 9849, 9868, 9918, 9874, 9991, 10107, 10108, 10044, 10172, 10646, 10085, 10987. TextId's unknown for altruis_the_sufferer and greatmother_geyah (npc_text)
SDCategory: Nagrand
EndScriptData */

/* ContentData
mob_shattered_rumbler
mob_lump
mob_sunspring_villager
npc_altruis_the_sufferer
npc_greatmother_geyah
npc_lantresor_of_the_blade
npc_maghar_captive
npc_creditmarker_visit_with_ancestors
mob_sparrowhawk
EndContentData */



#include "ScriptedEscortAI.h"

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include <cstring>
#include <cstring>
#include "Pet.h"
#include "Formulas.h"

/*######
## mob_shattered_rumbler - this should be done with ACID
######*/

class mob_shattered_rumbler : public CreatureScript
{
public:
    mob_shattered_rumbler() : CreatureScript("mob_shattered_rumbler") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_shattered_rumblerAI (pCreature);
    }

    struct mob_shattered_rumblerAI : public ScriptedAI
    {
        bool Spawn;

        mob_shattered_rumblerAI(Creature *c) : ScriptedAI(c) {}

        void Reset()
        {
            Spawn = false;
        }

        void EnterCombat(Unit* /*who*/) {}

        void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
        {
            if (Spellkind->Id == 32001 && !Spawn)
            {
                float x = me->GetPositionX();
                float y = me->GetPositionY();
                float z = me->GetPositionZ();

                Hitter->SummonCreature(18181,x+(0.7f * (rand()%30)),y+(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
                Hitter->SummonCreature(18181,x+(rand()%5),y-(rand()%5),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
                Hitter->SummonCreature(18181,x-(rand()%5),y+(0.5f *(rand()%60)),z,0,TEMPSUMMON_CORPSE_TIMED_DESPAWN,60000);
                me->setDeathState(CORPSE);
                Spawn = true;
            }
            return;
        }
    };

};

/*######
## mob_lump
######*/

#define SPELL_VISUAL_SLEEP  16093
#define SPELL_SPEAR_THROW   32248

#define LUMP_SAY0 -1000190
#define LUMP_SAY1 -1000191

#define LUMP_DEFEAT -1000192

#define GOSSIP_HL "I need answers, ogre!"
#define GOSSIP_SL1 "Why are Boulderfist out this far? You know that this is Kurenai territory."
#define GOSSIP_SL2 "And you think you can just eat anything you want? You're obviously trying to eat the Broken of Telaar."
#define GOSSIP_SL3 "This means war, Lump! War I say!"

class mob_lump : public CreatureScript
{
public:
    mob_lump() : CreatureScript("mob_lump") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SL1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                pPlayer->SEND_GOSSIP_MENU(9353, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SL2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                pPlayer->SEND_GOSSIP_MENU(9354, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SL3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                pPlayer->SEND_GOSSIP_MENU(9355, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                pPlayer->SEND_GOSSIP_MENU(9356, pCreature->GetGUID());
                pPlayer->TalkedToCreature(18354, pCreature->GetGUID());
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer->GetQuestStatus(9918) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        pPlayer->SEND_GOSSIP_MENU(9352, pCreature->GetGUID());

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_lumpAI(pCreature);
    }

    struct mob_lumpAI : public ScriptedAI
    {
        mob_lumpAI(Creature *c) : ScriptedAI(c)
        {
            bReset = false;
        }

        uint32 Reset_Timer;
        uint32 Spear_Throw_Timer;
        bool bReset;

        void Reset()
        {
            Reset_Timer = 60000;
            Spear_Throw_Timer = 2000;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void AttackedBy(Unit* pAttacker)
        {
            if (me->getVictim())
                return;

            if (me->IsFriendlyTo(pAttacker))
                return;

            AttackStart(pAttacker);
        }

        void DamageTaken(Unit *done_by, uint32 & damage)
        {
            if (done_by->GetTypeId() == TYPEID_PLAYER && me->HealthBelowPctDamaged(30, damage))
            {
                if (!bReset && CAST_PLR(done_by)->GetQuestStatus(9918) == QUEST_STATUS_INCOMPLETE)
                {
                    //Take 0 damage
                    damage = 0;

                    CAST_PLR(done_by)->AttackStop();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAllAuras();
                    me->DeleteThreatList();
                    me->CombatStop(true);
                    me->setFaction(1080);               //friendly
                    me->SetStandState(UNIT_STAND_STATE_SIT);
                    DoScriptText(LUMP_DEFEAT, me);

                    bReset = true;
                }
            }
        }

        void EnterCombat(Unit * /*who*/)
        {
            if (me->HasAura(SPELL_VISUAL_SLEEP))
                me->RemoveAura(SPELL_VISUAL_SLEEP);

            if (!me->IsStandState())
                 me->SetStandState(UNIT_STAND_STATE_STAND);

            DoScriptText(RAND(LUMP_SAY0,LUMP_SAY1), me);
        }

        void UpdateAI(const uint32 diff)
        {
            //check if we waiting for a reset
            if (bReset)
            {
                if (Reset_Timer <= diff)
                {
                    EnterEvadeMode();
                    bReset = false;
                    me->setFaction(1711);               //hostile
                    return;
                }
                else Reset_Timer -= diff;
            }

            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Spear_Throw_Timer
            if (Spear_Throw_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SPEAR_THROW);
                Spear_Throw_Timer = 20000;
            } else Spear_Throw_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

/*####
# mob_sunspring_villager - should be done with ACID
####*/

class mob_sunspring_villager : public CreatureScript
{
public:
    mob_sunspring_villager() : CreatureScript("mob_sunspring_villager") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_sunspring_villagerAI (pCreature);
    }

    struct mob_sunspring_villagerAI : public ScriptedAI
    {
        mob_sunspring_villagerAI(Creature *c) : ScriptedAI(c) {}

        void Reset()
        {
            me->SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
            me->SetStandState(UNIT_STAND_STATE_DEAD);
        }

        void EnterCombat(Unit * /*who*/) {}

        void SpellHit(Unit * /*caster*/, const SpellEntry *spell)
        {
            if (spell->Id == 32146)
            {
                me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                me->RemoveCorpse();
            }
        }
    };

};

/*######
## npc_altruis_the_sufferer
######*/

#define GOSSIP_HATS1 "I see twisted steel and smell sundered earth."
#define GOSSIP_HATS2 "Well...?"
#define GOSSIP_HATS3 "[PH] Story about Illidan's Pupil"

#define GOSSIP_SATS1 "Legion?"
#define GOSSIP_SATS2 "And now?"
#define GOSSIP_SATS3 "How do you see them now?"
#define GOSSIP_SATS4 "Forge camps?"
#define GOSSIP_SATS5 "Ok."
#define GOSSIP_SATS6 "[PH] Story done"

class npc_altruis_the_sufferer : public CreatureScript
{
public:
    npc_altruis_the_sufferer() : CreatureScript("npc_altruis_the_sufferer") { }

    bool OnQuestAccept(Player* pPlayer, Creature* /*pCreature*/, Quest const * /*quest*/)
    {
        if (!pPlayer->GetQuestRewardStatus(9991))              //Survey the Land, q-id 9991
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->ActivateTaxiPathTo(532);                  //TaxiPath 532
        }
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+10:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SATS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                pPlayer->SEND_GOSSIP_MENU(9420, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+11:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SATS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                pPlayer->SEND_GOSSIP_MENU(9421, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+12:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SATS3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                pPlayer->SEND_GOSSIP_MENU(9422, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+13:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SATS4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                pPlayer->SEND_GOSSIP_MENU(9423, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+14:
                pPlayer->SEND_GOSSIP_MENU(9424, pCreature->GetGUID());
                break;

            case GOSSIP_ACTION_INFO_DEF+20:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SATS5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
                pPlayer->SEND_GOSSIP_MENU(9427, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+21:
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->AreaExploredOrEventHappens(9991);
                break;

            case GOSSIP_ACTION_INFO_DEF+30:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SATS6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
                pPlayer->SEND_GOSSIP_MENU(384, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+31:
                pPlayer->CLOSE_GOSSIP_MENU();
                pPlayer->AreaExploredOrEventHappens(10646);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        //gossip before obtaining Survey the Land
        if (pPlayer->GetQuestStatus(9991) == QUEST_STATUS_NONE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HATS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+10);

        //gossip when Survey the Land is incomplete (technically, after the flight)
        if (pPlayer->GetQuestStatus(9991) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HATS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+20);

        //wowwiki.com/Varedis
        if (pPlayer->GetQuestStatus(10646) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HATS3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+30);

        pPlayer->SEND_GOSSIP_MENU(9419, pCreature->GetGUID());

        return true;
    }

};

/*######
## npc_greatmother_geyah
######*/

#define GOSSIP_HGG1 "Hello, Greatmother. Garrosh told me that you wanted to speak with me."
#define GOSSIP_HGG2 "Garrosh is beyond redemption, Greatmother. I fear that in helping the Mag'har, I have convinced Garrosh that he is unfit to lead."

#define GOSSIP_SGG1 "You raised all of the orcs here, Greatmother?"
#define GOSSIP_SGG2 "Do you believe that?"
#define GOSSIP_SGG3 "What can be done? I have tried many different things. I have done my best to help the people of Nagrand. Each time I have approached Garrosh, he has dismissed me."
#define GOSSIP_SGG4 "Left? How can you choose to leave?"
#define GOSSIP_SGG5 "What is this duty?"
#define GOSSIP_SGG6 "Is there anything I can do for you, Greatmother?"
#define GOSSIP_SGG7 "I have done all that I could, Greatmother. I thank you for your kind words."
#define GOSSIP_SGG8 "Greatmother, you are the mother of Durotan?"
#define GOSSIP_SGG9 "Greatmother, I never had the honor. Durotan died long before my time, but his heroics are known to all on my world. The orcs of Azeroth reside in a place known as Durotar, named after your son. And ... (You take a moment to breathe and think through what you are about to tell the Greatmother.)"
#define GOSSIP_SGG10 "It is my Warchief, Greatmother. The leader of my people. From my world. He ... He is the son of Durotan. He is your grandchild."
#define GOSSIP_SGG11 "I will return to Azeroth at once, Greatmother."

//all the textId's for the below is unknown, but i do believe the gossip item texts are proper.
class npc_greatmother_geyah : public CreatureScript
{
public:
    npc_greatmother_geyah() : CreatureScript("npc_greatmother_geyah") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 5:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 6:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 7:
                pPlayer->AreaExploredOrEventHappens(10044);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;

            case GOSSIP_ACTION_INFO_DEF + 10:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 11:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG8, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 12:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG9, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 13:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG10, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 14:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SGG11, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF + 15:
                pPlayer->AreaExploredOrEventHappens(10172);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(10044) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HGG1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        }
        else if (pPlayer->GetQuestStatus(10172) == QUEST_STATUS_INCOMPLETE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HGG2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
        }
        else

            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

};

/*######
## npc_lantresor_of_the_blade
######*/

#define GOSSIP_HLB "I have killed many of your ogres, Lantresor. I have no fear."
#define GOSSIP_SLB1 "Should I know? You look like an orc to me."
#define GOSSIP_SLB2 "And the other half?"
#define GOSSIP_SLB3 "I have heard of your kind, but I never thought to see the day when I would meet a half-breed."
#define GOSSIP_SLB4 "My apologies. I did not mean to offend. I am here on behalf of my people."
#define GOSSIP_SLB5 "My people ask that you pull back your Boulderfist ogres and cease all attacks on our territories. In return, we will also pull back our forces."
#define GOSSIP_SLB6 "We will fight you until the end, then, Lantresor. We will not stand idly by as you pillage our towns and kill our people."
#define GOSSIP_SLB7 "What do I need to do?"

class npc_lantresor_of_the_blade : public CreatureScript
{
public:
    npc_lantresor_of_the_blade() : CreatureScript("npc_lantresor_of_the_blade") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                pPlayer->SEND_GOSSIP_MENU(9362, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                pPlayer->SEND_GOSSIP_MENU(9363, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                pPlayer->SEND_GOSSIP_MENU(9364, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                pPlayer->SEND_GOSSIP_MENU(9365, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                pPlayer->SEND_GOSSIP_MENU(9366, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+5:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                pPlayer->SEND_GOSSIP_MENU(9367, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+6:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SLB7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                pPlayer->SEND_GOSSIP_MENU(9368, pCreature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+7:
                pPlayer->SEND_GOSSIP_MENU(9369, pCreature->GetGUID());
                if (pPlayer->GetQuestStatus(10107) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->AreaExploredOrEventHappens(10107);
                if (pPlayer->GetQuestStatus(10108) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->AreaExploredOrEventHappens(10108);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pPlayer->GetQuestStatus(10107) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestStatus(10108) == QUEST_STATUS_INCOMPLETE)
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HLB, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        pPlayer->SEND_GOSSIP_MENU(9361, pCreature->GetGUID());

        return true;
    }

};

/*#####
## npc_maghar_captive
#####*/

enum eMagharCaptive
{
    SAY_MAG_START               = -1000482,
    SAY_MAG_NO_ESCAPE           = -1000483,
    SAY_MAG_MORE                = -1000484,
    SAY_MAG_MORE_REPLY          = -1000485,
    SAY_MAG_LIGHTNING           = -1000486,
    SAY_MAG_SHOCK               = -1000487,
    SAY_MAG_COMPLETE            = -1000488,

    SPELL_CHAIN_LIGHTNING       = 16006,
    SPELL_EARTHBIND_TOTEM       = 15786,
    SPELL_FROST_SHOCK           = 12548,
    SPELL_HEALING_WAVE          = 12491,

    QUEST_TOTEM_KARDASH_H       = 9868,

    NPC_MURK_RAIDER             = 18203,
    NPC_MURK_BRUTE              = 18211,
    NPC_MURK_SCAVENGER          = 18207,
    NPC_MURK_PUTRIFIER          = 18202
};

static float m_afAmbushA[]= {-1568.805786f, 8533.873047f, 1.958f};
static float m_afAmbushB[]= {-1491.554321f, 8506.483398f, 1.248f};

class npc_maghar_captive : public CreatureScript
{
public:
    npc_maghar_captive() : CreatureScript("npc_maghar_captive") { }

    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
    {
        if (pQuest->GetQuestId() == QUEST_TOTEM_KARDASH_H)
        {
            if (npc_maghar_captiveAI* pEscortAI = dynamic_cast<npc_maghar_captiveAI*>(pCreature->AI()))
            {
                pCreature->SetStandState(UNIT_STAND_STATE_STAND);
                pCreature->setFaction(232);

                pEscortAI->Start(true, false, pPlayer->GetGUID(), pQuest);

                DoScriptText(SAY_MAG_START, pCreature);

                pCreature->SummonCreature(NPC_MURK_RAIDER, m_afAmbushA[0]+2.5f, m_afAmbushA[1]-2.5f, m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                pCreature->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushA[0]-2.5f, m_afAmbushA[1]+2.5f, m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                pCreature->SummonCreature(NPC_MURK_BRUTE, m_afAmbushA[0], m_afAmbushA[1], m_afAmbushA[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
            }
        }
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_maghar_captiveAI(pCreature);
    }

    struct npc_maghar_captiveAI : public npc_escortAI
    {
        npc_maghar_captiveAI(Creature* pCreature) : npc_escortAI(pCreature) { Reset(); }

        uint32 m_uiChainLightningTimer;
        uint32 m_uiHealTimer;
        uint32 m_uiFrostShockTimer;

        void Reset()
        {
            m_uiChainLightningTimer = 1000;
            m_uiHealTimer = 0;
            m_uiFrostShockTimer = 6000;
        }

        void EnterCombat(Unit* /*pWho*/)
        {
            DoCast(me, SPELL_EARTHBIND_TOTEM, false);
        }

        void WaypointReached(uint32 uiPointId)
        {
            switch(uiPointId)
            {
                case 7:
                    DoScriptText(SAY_MAG_MORE, me);

                    if (Creature* pTemp = me->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushB[0], m_afAmbushB[1], m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000))
                        DoScriptText(SAY_MAG_MORE_REPLY, pTemp);

                    me->SummonCreature(NPC_MURK_PUTRIFIER, m_afAmbushB[0]-2.5f, m_afAmbushB[1]-2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);

                    me->SummonCreature(NPC_MURK_SCAVENGER, m_afAmbushB[0]+2.5f, m_afAmbushB[1]+2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    me->SummonCreature(NPC_MURK_SCAVENGER, m_afAmbushB[0]+2.5f, m_afAmbushB[1]-2.5f, m_afAmbushB[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                    break;
                case 16:
                    DoScriptText(SAY_MAG_COMPLETE, me);

                    if (Player* pPlayer = GetPlayerForEscort())
                        pPlayer->GroupEventHappens(QUEST_TOTEM_KARDASH_H, me);

                    SetRun();
                    break;
            }
        }

        void JustSummoned(Creature* pSummoned)
        {
            if (pSummoned->GetEntry() == NPC_MURK_BRUTE)
                DoScriptText(SAY_MAG_NO_ESCAPE, pSummoned);

            if (pSummoned->isTotem())
                return;

            pSummoned->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
            pSummoned->GetMotionMaster()->MovePoint(0, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
            pSummoned->AI()->AttackStart(me);

        }

        void SpellHitTarget(Unit* /*pTarget*/, const SpellEntry* pSpell)
        {
            if (pSpell->Id == SPELL_CHAIN_LIGHTNING)
            {
                if (rand()%10)
                    return;

                DoScriptText(SAY_MAG_LIGHTNING, me);
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);
            if (!me->getVictim())
                return;

            if (m_uiChainLightningTimer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_CHAIN_LIGHTNING);
                m_uiChainLightningTimer = urand(7000, 14000);
            }
            else
                m_uiChainLightningTimer -= uiDiff;

            if (HealthBelowPct(30))
            {
                if (m_uiHealTimer <= uiDiff)
                {
                    DoCast(me, SPELL_HEALING_WAVE);
                    m_uiHealTimer = 5000;
                }
                else
                    m_uiHealTimer -= uiDiff;
            }

            if (m_uiFrostShockTimer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_FROST_SHOCK);
                m_uiFrostShockTimer = urand(7500, 15000);
            }
            else
                m_uiFrostShockTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};

/*######
## npc_creditmarker_visist_with_ancestors
######*/

class npc_creditmarker_visit_with_ancestors : public CreatureScript
{
public:
    npc_creditmarker_visit_with_ancestors() : CreatureScript("npc_creditmarker_visit_with_ancestors") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_creditmarker_visit_with_ancestorsAI (pCreature);
    }

    struct npc_creditmarker_visit_with_ancestorsAI : public ScriptedAI
    {
        npc_creditmarker_visit_with_ancestorsAI(Creature* c) : ScriptedAI(c) {}

        void Reset() {}

        void EnterCombat(Unit* /*who*/) {}

        void MoveInLineOfSight(Unit *who)
        {
            if (!who)
                return;

            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                if (CAST_PLR(who)->GetQuestStatus(10085) == QUEST_STATUS_INCOMPLETE)
                {
                    uint32 creditMarkerId = me->GetEntry();
                    if ((creditMarkerId >= 18840) && (creditMarkerId <= 18843))
                    {
                        // 18840: Sunspring, 18841: Laughing, 18842: Garadar, 18843: Bleeding
                        if (!CAST_PLR(who)->GetReqKillOrCastCurrentCount(10085, creditMarkerId))
                            CAST_PLR(who)->KilledMonsterCredit(creditMarkerId, me->GetGUID());
                    }
                }
            }
        }
    };

};

/*######
## mob_sparrowhawk
######*/

#define SPELL_SPARROWHAWK_NET 39810
#define SPELL_ITEM_CAPTIVE_SPARROWHAWK 39812

class mob_sparrowhawk : public CreatureScript
{
public:
    mob_sparrowhawk() : CreatureScript("mob_sparrowhawk") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_sparrowhawkAI (pCreature);
    }

    struct mob_sparrowhawkAI : public ScriptedAI
    {

        mob_sparrowhawkAI(Creature *c) : ScriptedAI(c) {}

        uint32 Check_Timer;
        uint64 PlayerGUID;
        bool fleeing;

        void Reset()
        {
            me->RemoveAurasDueToSpell(SPELL_SPARROWHAWK_NET);
            Check_Timer = 1000;
            PlayerGUID = 0;
            fleeing = false;
        }
        void AttackStart(Unit *who)
        {
            if (PlayerGUID)
                return;

            ScriptedAI::AttackStart(who);
        }

        void EnterCombat(Unit* /*who*/) {}

        void MoveInLineOfSight(Unit *who)
        {
            if (!who || PlayerGUID)
                return;

            if (!PlayerGUID && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 30) && CAST_PLR(who)->GetQuestStatus(10987) == QUEST_STATUS_INCOMPLETE)
            {
                PlayerGUID = who->GetGUID();
                return;
            }

            ScriptedAI::MoveInLineOfSight(who);
        }

        void UpdateAI(const uint32 diff)
        {
            if (Check_Timer <= diff)
            {
                if (PlayerGUID)
                {
                    if (fleeing && me->GetMotionMaster()->GetCurrentMovementGeneratorType() != FLEEING_MOTION_TYPE)
                        fleeing = false;

                    Player* pPlayer = Unit::GetPlayer(*me, PlayerGUID);
                    if (pPlayer && me->IsWithinDistInMap(pPlayer, 30))
                    {
                        if (!fleeing)
                        {
                            me->DeleteThreatList();
                            me->GetMotionMaster()->MoveFleeing(pPlayer);
                            fleeing = true;
                        }
                    }
                    else if (fleeing)
                    {
                        me->GetMotionMaster()->MovementExpired(false);
                        PlayerGUID = 0;
                        fleeing = false;
                    }
                }
                Check_Timer = 1000;
            } else Check_Timer -= diff;

            if (PlayerGUID)
                return;

            ScriptedAI::UpdateAI(diff);
        }

        void SpellHit(Unit *caster, const SpellEntry *spell)
        {
            if (caster->GetTypeId() == TYPEID_PLAYER)
            {
                if (spell->Id == SPELL_SPARROWHAWK_NET && CAST_PLR(caster)->GetQuestStatus(10987) == QUEST_STATUS_INCOMPLETE)
                {
                    DoCast(caster, SPELL_ITEM_CAPTIVE_SPARROWHAWK, true);
                    me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                }
            }
            return;
        }
    };

};

/*####
# Quest Soporte http://www.wowhead.com/quest=9923 "HELP!"
####*/

enum eHelp
{
    QUEST_HELP                      = 9923,
    NPC_CORKI                       = 18445,
    SAY_CREDIT                      = -1850070
};

class go_help_cage : public GameObjectScript
{
public:
    go_help_cage() : GameObjectScript("go_help_cage") { }

    bool OnGossipHello(Player *pPlayer, GameObject *pGO)
    {
        if (pPlayer->GetQuestStatus(QUEST_HELP) == QUEST_STATUS_INCOMPLETE)
        {
            Creature *pCorki = pGO->FindNearestCreature(NPC_CORKI,1.0f);
            if (pCorki)
            {
                DoScriptText(SAY_CREDIT, pCorki);
                Quest const* qInfo = sObjectMgr->GetQuestTemplate(QUEST_HELP);
                if (qInfo)
                {
                    pPlayer->KilledMonsterCredit(qInfo->ReqCreatureOrGOId[0],0);
                }
            }
        }
        return true;
    }
};


/*####
# Quest Soporte http://www.wowhead.com/quest=9924 "Corki's Gone Missing Again!"
####*/

enum eCorki
{
    QUEST_CORKI_GONE_MISSING_AGAIN  = 9924,
    NPC_CORKI_RUN                   = 20812,//Guid: 79588
    SAY_RUN                         = -1850071//This is the last time I get caughht! I promise! Bye!
};

class go_corki_cage : public GameObjectScript
{
public:
    go_corki_cage() : GameObjectScript("go_corki_cage") { }

    bool OnGossipHello(Player *pPlayer, GameObject *pGO)
    {
        if (pPlayer->GetQuestStatus(QUEST_CORKI_GONE_MISSING_AGAIN) == QUEST_STATUS_INCOMPLETE)
        {
            Creature *pCorkiRun = pGO->FindNearestCreature(NPC_CORKI_RUN,1.0f);
            if (pCorkiRun)
            {
                pCorkiRun->GetMotionMaster()->MoveFleeing(pPlayer, 3500);
                DoScriptText(SAY_RUN, pCorkiRun);
                pCorkiRun->DespawnOrUnsummon();
                Quest const* qInfo = sObjectMgr->GetQuestTemplate(QUEST_CORKI_GONE_MISSING_AGAIN);
                if (qInfo)
                {
                    pPlayer->KilledMonsterCredit(qInfo->ReqCreatureOrGOId[0],0);
                }
            }
        }
        return true;
    }
};


/*####
# Beast Master
####*/

#define GOSSIP_ITEM_STABLE  "Stable"
#define GOSSIP_ITEM_NEWPET  "New Pet"
#define GOSSIP_ITEM_BOAR    "Boar"
#define GOSSIP_ITEM_SERPENT "Serpent"
#define GOSSIP_ITEM_SCRAB   "Scrab"
#define GOSSIP_ITEM_LION    "Lion"
#define GOSSIP_ITEM_WOLF    "Wolf"
#define GOSSIP_ITEM_RAVAGER "Ravenger"

#define GOSSIP_ITEM_UNTRAINEPET "Restart Pet"

void CreatePet(Player *player, Creature * m_creature, uint32 entry) {

    if(player->getClass() != CLASS_HUNTER) {
        m_creature->MonsterWhisper("You are not hunter! Your order have been rejected.", player->GetGUID());
        player->PlayerTalkClass->CloseGossip();
        return;
    }

    if(player->GetPet()) {
        m_creature->MonsterWhisper("First you must drop your pet!", player->GetGUID());
        player->PlayerTalkClass->CloseGossip();
        return;
    }

    Creature *creatureTarget = m_creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
    if(!creatureTarget) return;
    
    Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
    if(!pet) return;

    // kill original creature
    creatureTarget->setDeathState(JUST_DIED);
    creatureTarget->RemoveCorpse();
    creatureTarget->SetHealth(0);                       // just for nice GM-mode view

    pet->SetPower(POWER_HAPPINESS, 1048000);

    //pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
    //pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, uint32((Trinity::XP::xp_to_level(70))/4));

    // prepare visual effect for levelup
        pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
    pet->GetMap()->Add((Creature*)pet);
    // visual effect for levelup
    pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());

    
    /*if(!pet->InitStatsForLevel(player->getLevel()))
        error_log ("Pet Create fail: no init stats for entry %u", entry);*/

    pet->UpdateAllStats();
    
    // caster have pet now
    player->SetMinion(pet, true);

    pet->SavePetToDB(PET_SAVE_AS_CURRENT);
    pet->InitTalentForLevel();
    player->PetSpellInitialize();
    
    //end
    player->PlayerTalkClass->CloseGossip();
    m_creature->MonsterWhisper("Pet added. You might want to feed it and name it somehow.", player->GetGUID());
};


bool GossipHello_npc_beastmaster(Player *player, Creature *_Creature)
{

    if(player->getClass() != CLASS_HUNTER)
      {
      _Creature->MonsterWhisper("You are not hunter!", player->GetGUID());
        return true;
        }
      player->ADD_GOSSIP_ITEM(4, "Get new pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
      if (player->CanTameExoticPets())
      {
        player->ADD_GOSSIP_ITEM(4, "Get new exotic pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50);
      }
      player->ADD_GOSSIP_ITEM(2, "Take me to stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
      player->ADD_GOSSIP_ITEM(6, "Sell me some food", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
    player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
    return true;
};
                        //player->SEND_VENDORLIST( _Creature->GetGUID() );

bool GossipSelect_npc_beastmaster(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    switch (action)
    {
    
   case GOSSIP_ACTION_INFO_DEF + 100:
    player->ADD_GOSSIP_ITEM(4, "Get new pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
      if (player->CanTameExoticPets())
      {
        player->ADD_GOSSIP_ITEM(4, "Get new exotic pet", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 50);
      }
      player->ADD_GOSSIP_ITEM(2, "Take me to stable", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_STABLEPET);
      player->ADD_GOSSIP_ITEM(6, "Sell me some food", GOSSIP_SENDER_MAIN, GOSSIP_OPTION_VENDOR);
      player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
    break;
    
  case GOSSIP_ACTION_INFO_DEF + 30:
    player->ADD_GOSSIP_ITEM(2, "<< Back to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
    player->ADD_GOSSIP_ITEM(4, "Next page >>", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
    player->ADD_GOSSIP_ITEM(6, "Bat", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
    player->ADD_GOSSIP_ITEM(6, "Bear", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    player->ADD_GOSSIP_ITEM(6, "Boar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    player->ADD_GOSSIP_ITEM(6, "Cat", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
    player->ADD_GOSSIP_ITEM(6, "Carrion Bird", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
    player->ADD_GOSSIP_ITEM(6, "Crab", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
    player->ADD_GOSSIP_ITEM(6, "Crocolisk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
    player->ADD_GOSSIP_ITEM(6, "Dragonhawk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
    player->ADD_GOSSIP_ITEM(6, "Gorilla", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
    player->ADD_GOSSIP_ITEM(6, "Hound", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
    player->ADD_GOSSIP_ITEM(6, "Hyena", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
    player->ADD_GOSSIP_ITEM(6, "Moth", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
    player->ADD_GOSSIP_ITEM(6, "Owl", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
    player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
    break;
    
  case GOSSIP_ACTION_INFO_DEF + 31:  
    player->ADD_GOSSIP_ITEM(2, "<< Back to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
    player->ADD_GOSSIP_ITEM(4, "<< Previous page", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
    player->ADD_GOSSIP_ITEM(6, "Raptor", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
    player->ADD_GOSSIP_ITEM(6, "Ravager", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
    player->ADD_GOSSIP_ITEM(6, "Strider", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
    player->ADD_GOSSIP_ITEM(6, "Scorpid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
    player->ADD_GOSSIP_ITEM(6, "Spider", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
    player->ADD_GOSSIP_ITEM(6, "Serpent", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);  
    player->ADD_GOSSIP_ITEM(6, "Turtle", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
    player->ADD_GOSSIP_ITEM(6, "Wasp", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
    break;
    
  case GOSSIP_ACTION_INFO_DEF + 50:  
    player->ADD_GOSSIP_ITEM(2, "<< Back to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
    player->ADD_GOSSIP_ITEM(6, "Chimaera", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 51);
    player->ADD_GOSSIP_ITEM(6, "Core Hound", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 52);
    player->ADD_GOSSIP_ITEM(6, "Devilsaur", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 53);
    player->ADD_GOSSIP_ITEM(6, "Rhino", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 54);
    player->ADD_GOSSIP_ITEM(6, "Silithid", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 55);
    player->ADD_GOSSIP_ITEM(6, "Worm", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 56);  
    player->ADD_GOSSIP_ITEM(6, "Loque'nahak", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 57);
        player->ADD_GOSSIP_ITEM(6, "Skoll", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 58);
        player->ADD_GOSSIP_ITEM(6, "Gondria", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 59);
        player->SEND_GOSSIP_MENU(1,_Creature->GetGUID());
    break;
  
        case GOSSIP_OPTION_STABLEPET:
            player->GetSession()->SendStablePet(_Creature->GetGUID());
            break; 
        case GOSSIP_OPTION_VENDOR:
            player->SEND_VENDORLIST( _Creature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 51: //chimera
            CreatePet(player, _Creature, 21879);
            break;
        case GOSSIP_ACTION_INFO_DEF + 52: //core hound
            CreatePet(player, _Creature, 21108);
            break;
        case GOSSIP_ACTION_INFO_DEF + 53: //Devilsaur
            CreatePet(player, _Creature, 20931);
            break;
        case GOSSIP_ACTION_INFO_DEF + 54: //rhino
            CreatePet(player, _Creature, 30445);
            break;
        case GOSSIP_ACTION_INFO_DEF + 55: //silithid
            CreatePet(player, _Creature, 5460);
            break;
        case GOSSIP_ACTION_INFO_DEF + 56: //Worm
            CreatePet(player, _Creature, 30148);
            break;
        case GOSSIP_ACTION_INFO_DEF + 57: //Loque'nahak
            CreatePet(player, _Creature, 32517);
            break;
        case GOSSIP_ACTION_INFO_DEF + 58: //Skoll
            CreatePet(player, _Creature, 35189);
            break;
        case GOSSIP_ACTION_INFO_DEF + 59: //Gondria
            CreatePet(player, _Creature, 33776);
            break;
        case GOSSIP_ACTION_INFO_DEF + 16: //Spider
            CreatePet(player, _Creature, 2349);
            break;
        case GOSSIP_ACTION_INFO_DEF + 17: //Dragonhawk
            CreatePet(player, _Creature, 27946);
            break;
        case GOSSIP_ACTION_INFO_DEF + 18: //Bat
            CreatePet(player, _Creature, 28233);
            break;
        case GOSSIP_ACTION_INFO_DEF + 19: //Ravager
            CreatePet(player, _Creature, 17199);
            break;
        case GOSSIP_ACTION_INFO_DEF + 20: //Raptor
            CreatePet(player, _Creature, 14821);
            break;
        case GOSSIP_ACTION_INFO_DEF + 21: //Serpent
            CreatePet(player, _Creature, 28358);
            break;
        case GOSSIP_ACTION_INFO_DEF + 1: //bear
            CreatePet(player, _Creature, 29319);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2: //Boar
            CreatePet(player, _Creature, 29996);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3: //Bug
            CreatePet(player, _Creature, 28085);
            break;
        case GOSSIP_ACTION_INFO_DEF + 4: //cat
            CreatePet(player, _Creature, 28097);
            break;
        case GOSSIP_ACTION_INFO_DEF + 5: //carrion
            CreatePet(player, _Creature, 26838);
            break;
        case GOSSIP_ACTION_INFO_DEF + 6: //crab
            CreatePet(player, _Creature, 24478);
            break;   
        case GOSSIP_ACTION_INFO_DEF + 7: //crocolisk
            CreatePet(player, _Creature, 1417);
            break;  
        case GOSSIP_ACTION_INFO_DEF + 8: //gorila
            CreatePet(player, _Creature, 28213);
            break;
        case GOSSIP_ACTION_INFO_DEF + 9: //hound
            CreatePet(player, _Creature, 29452);
            break;
        case GOSSIP_ACTION_INFO_DEF + 10: //hynea
            CreatePet(player, _Creature, 13036);
            break;
        case GOSSIP_ACTION_INFO_DEF + 11: //Moth
            CreatePet(player, _Creature, 27421);
            break;
        case GOSSIP_ACTION_INFO_DEF + 12: //owl
            CreatePet(player, _Creature, 23136);
            break;
        case GOSSIP_ACTION_INFO_DEF + 13: //strider
            CreatePet(player, _Creature, 22807);
            break;
        case GOSSIP_ACTION_INFO_DEF + 14: //scorpid
            CreatePet(player, _Creature, 9698);
            break;
        case GOSSIP_ACTION_INFO_DEF + 15: //turtle
            CreatePet(player, _Creature, 25482);
            break;
    }
    
    return true;

};

void AddSC_nagrand()
{
    new mob_shattered_rumbler();
    new mob_lump();
    new mob_sunspring_villager();
    new npc_altruis_the_sufferer();
    new npc_greatmother_geyah();
    new npc_lantresor_of_the_blade();
    new npc_maghar_captive();
    new npc_creditmarker_visit_with_ancestors();
    new mob_sparrowhawk();
    new go_help_cage();
    new go_corki_cage();
}
