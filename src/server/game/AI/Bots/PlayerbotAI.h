#ifndef _PLAYERBOTAI_H
#define _PLAYERBOTAI_H

#include "Common.h"


class WorldPacket;
class Player;
class Unit;
class Object;
class Item;
class PlayerbotClassAI;

enum ScenarioType
{
    SCENARIO_PVEEASY,
    SCENARIO_PVEHARD,
    SCENARIO_DUEL,
    SCENARIO_PVPEASY,
    SCENARIO_PVPHARD
};

//masters orders that should be obeyed by the AI during the updteAI routine
//the master will auto set the target of the bot
enum CombatOrderType
{
    ORDERS_NONE,
    ORDERS_KILL,
    ORDERS_CC,
    ORDERS_HEAL,
    ORDERS_TANK,
    ORDERS_PROTECT,
    ORDERS_REGEN
};

typedef std::set<Unit *> AttackerInfoList;

class PlayerbotAI
{
public:
    //******* Stuff the outside world calls ****************************
    PlayerbotAI(Player *const master, Player *const bot);
    virtual ~PlayerbotAI();

    //This is called from Unit.cpp and is called every second (I think)
    void UpdateAI(const uint32 p_time);

    //This is called from ChatHandler.cpp when there is an incoming message to the bot
    //from a whisper or from the party channel
    void HandleCommand(const std::string &text, Player &fromPlayer);

    //This is called by WorldSession.pm
    //It provides a view of packets normally sent to the client.
    //Since there is no client at the other end, the packets are dropped of course.
    //For a list of opcodes that can be caught see Opcodes.cpp (SMSG_* opcodes only)
    void HandleBotOutgoingPacket(const WorldPacket &packet);

    //This is called whenever the master sends a packet to the server.
    //These packets can be viewed, but not edited.
    //It allows bot creators to craft AI in response to a master's actions.
    //For a list of opcodes that can be caught see Opcodes.cpp (CMSG_* opcodes only)
    //Notice: that this is static which means it is called once for all bots of the master.
    static void HandleMasterIncomingPacket(const WorldPacket &packet, WorldSession &masterSession);
    static void HandleMasterOutgoingPacket(const WorldPacket &packet, WorldSession &masterSession);

    //Returns what kind of situation we are in so the AI can react accordingly
    ScenarioType GetScenarioType(){ return m_ScenarioType; }

    PlayerbotClassAI *GetClassAI(){ return m_classAI; }

    //protected:

    //******* Utilities ***************************************************

    //finds spell ID for matching substring args
    //in priority of full text match, spells not taking reagents, and highest rank
    uint32 getSpellId(const char *args, bool master=false) const;
    //Main PlayerBot spell finding function Returns ONLY exact matches including Upper/Lower case differentiation.
    uint32 getSpellIdExact(const char *args, bool includePassive=false, bool master=false);

    // finds quest ID for matching substring args
    uint32 getQuestId(const char* args, bool remove) const;

    //extracts item ids from links
    void extractItemIds(const std::string &text, std::list<uint32> &itemIds) const;

    //extracts currency from a string as #g#s#c and returns the total in copper
    uint32 extractMoney(const std::string &text) const;

    // extracts gameobject info from link
    bool extractGOinfo(const std::string& text, uint32 &guid,  uint32 &entry, int &mapid, float &x, float &y, float &z) const;

    // finds items in equipment and adds Item* to foundItemList
    // also removes found item IDs from itemIdSearchList when found
    void findItemsInEquip(std::list<uint32>& itemIdSearchList, std::list<Item*>& foundItemList) const;

    //finds items in bots inventory and adds them to foundItemList, removes found items from itemIdSearchList
    void findItemsInInv(std::list<uint32>& itemIdSearchList, std::list<Item*>& foundItemList) const;

    //currently bots only obey commands from the master
    bool canObeyCommandFrom(const Player &player) const;

    //get current casting spell (will return NULL if no spell!)
    Spell *GetCurrentSpell() const;

    bool HasAura(uint32 spellId, const Unit *player) const;
    bool HasAura(const char *spellName, const Unit *player) const;
    bool HasAura(const char *spellName) const;
    void HandleTeleportAck();

    bool HasPick();

    uint8 GetHealthPercent(const Unit &target) const;
    uint8 GetHealthPercent() const;
    uint8 GetBaseManaPercent(const Unit &target) const;
    uint8 GetBaseManaPercent() const;
    uint8 GetManaPercent(const Unit &target) const;
    uint8 GetManaPercent() const;
    uint8 GetRageAmount(const Unit &target) const;
    uint8 GetRageAmount() const;
    uint8 GetEnergyAmount(const Unit &target) const;
    uint8 GetEnergyAmount() const;
    uint8 GetRunicPower(const Unit &target) const;
    uint8 GetRunicPower() const;

    Item *FindFood() const;
    Item *FindDrink() const;
    Item *FindPotion() const;
    Item *FindBandage() const;
    Item *FindPoisonForward() const;    // finds poison starting from the front
    Item *FindPoisonBackward() const;   // finds poison starting from the back

    void UseMount() const;

    //******* Actions ****************************************
    //Your handlers can call these actions to make the bot do things.
    void TellMaster(const std::string &text);
    void SendWhisper(const std::string &text, Player &player);
    bool CastSpell(const char *args);
    //Player bots main spell cast function, if checkFirst > performs canCast() first, if castExistingAura > performs the cast even if the aura exists on target,
    //if skipFriendlyCheck > do not perform spell positive/negative and target friendly/hostile checks (Useful for dual purpose spells like holy shock)
    virtual bool CastSpell(uint32 spellId, Unit *target=NULL, bool checkFirst=true, bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false, bool triggered=false);
    virtual bool CastSpell(const SpellEntry * pSpellInfo, Unit *target=NULL, bool checkFirst=true, bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false, bool triggered=false);
    //Simple Checks to determine if the bot can cast the spell or not...
    //Mana/Stance/EquipmentRequirement/Distance/TargetInFront/OverwriteOrStackExistingAura/FriendlyFire checks
    virtual bool CanCast(uint32 spellId, Unit *target=NULL,bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false);
    virtual bool CanCast(const SpellEntry * pSpellInfo, Unit *target=NULL,bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false);
    //Gets the current form/stance of player
    uint8 GetForm(Unit *pPlayer=NULL);
    void UseItem(Item &item);
    void PoisonWeapon(Item &item, uint32 spellid=0, uint32 target=0, EquipmentSlots weapon=EQUIPMENT_SLOT_MAINHAND);
    void EquipItem(Item &item);
    void Stay();
    bool Follow(Player &player);
    void SendNotEquipList(Player &player);
    void Feast();
    void SetLooting(bool looting);
    void InterruptCurrentCastingSpell();
    void GetCombatOrders();
    void DoNextCombatManeuver();
    void KilledMonster(uint32 entry, uint64 guid);
    void ItemLocalization(std::string &itemName, const uint32 itemID) const;
    void QuestLocalization(std::string &questTitle, const uint32 questID) const;

    uint32 GetAttackerCount(){ return m_attackerInfo.size(); }
	void SetIgnoreUpdateTime(float t){m_ignoreAIUpdatesUntilTime=getMSTime() + (t * 1000); };

    Player *GetPlayerBot(){ return m_bot; }
    void SetInFront(const Unit *obj);

    bool CanBotsFly();  // take the flight path?
    uint32 GetStartMapID() { return m_startMapID; };
    uint32 GetStartZoneID() { return m_startZoneID; };
    uint32 GetStartAreaID() { return m_startAreaID; };
    uint32 GetStartPhase() { return m_startPhase; };
    uint32 GetStartDifficulty() { return m_startDifficulty; };
    uint32 GetStartInstanceID() { return m_startInstanceID; };
    float GetStartX() { return m_startX; };
    float GetStartY() { return m_startY; };
    float GetStartZ() { return m_startZ; };
    float GetStartO() { return m_startO; };

    void SetStartMapID(uint32 mapID) { m_startMapID = mapID; };
    void SetStartZoneID(uint32 zoneID) { m_startZoneID = zoneID; };
    void SetStartAreaID(uint32 areaID) { m_startAreaID = areaID; };
    void SetStartPhase(uint32 phase) { m_startPhase = phase; };
    void SetStartDifficulty(uint32 difficulty) { m_startDifficulty = difficulty; };
    void SetStartInstanceID(uint32 instanceID) { m_startInstanceID = instanceID; };
    void SetStartX(float x) { m_startX = x; };
    void SetStartY(float y) { m_startY = y; };
    void SetStartZ(float z) { m_startZ = z; };
    void SetStartO(float o) { m_startO = o; };

    void AddLootGUID(uint64 guid);
    void SetCombatOrder (CombatOrderType orders);

private:

    //****** Closed Actions ********************************
    //These actions may only be called at special times.
    //Trade methods are only applicable when the trade window is open
    //and are only called from within HandleCommand.
    // submits packet to trade an item (trade window must already be open)

    // default slot is -1 which means trade slots 0 to 5. if slot is set
    // to TRADE_SLOT_NONTRADED (which is slot 6) item will be shown in the
    // 'Will not be traded' slot.
    bool TradeItem(const Item& item, int8 slot=-1);

    bool TradeCopper(uint32 copper);

    //it is safe to keep these back reference pointers because m_bot
    //owns the "this" object and m_master owns m_bot. The owner always cleans up.
    Player *const m_master;
    Player *const m_bot;
    PlayerbotClassAI *m_classAI;

    //ignores AI updates until time specified
    //no need to waste CPU cycles during casting etc
    uint32 m_ignoreAIUpdatesUntilTime;

    CombatOrderType m_combatOrder;

    ScenarioType m_ScenarioType;
    typedef std::set<Unit *> AttackerSet;

    time_t m_TimeDoneEating;
    time_t m_TimeDoneDrinking;
    time_t m_TimeRessurect;
    uint32 m_CurrentlyCastingSpellId;
    bool m_IsFollowingMaster;

    //if master commands bot to do something, store here until updateAI
    //can do it
    uint32 m_spellIdCommand;
    uint64 m_targetGuidCommand;

    //finds who to attack next
    Unit *getNextTarget(Unit *victim);

    /* -- Loot routines by runsttren */
    bool DoLoot();
    void SetQuestNeedItems();

    void TurnInQuests( WorldObject *questgiver );

    typedef std::map<uint32, uint32> BotNeedItem;
    typedef std::list<uint64> BotLootCreature;
    typedef std::map<uint32, std::string> BotQuestsSeen;

    //list of items needed to fullfill quests
    BotNeedItem m_needItemList;

    //list of items needed to fullfill quests
    BotNeedItem m_needEmblemList;

    //list of quests recently seen that we can accept
    BotQuestsSeen m_questsSeen;

    //list of creatures we recently attacked and want to loot
    BotLootCreature m_lootCreature; //list of creatures
    uint64 m_lootCurrent; //current remains of interest
    uint64 m_lootobjtemp;
    bool isLooting;
    AttackerInfoList m_attackerInfo;

    float m_followDistanceMin, m_followDistanceMax;
    int m_playerBotsFly;

    uint32 m_startMapID;
    uint32 m_startZoneID;
    uint32 m_startAreaID;
    uint32 m_startPhase;
    uint32 m_startDifficulty;
    uint32 m_startInstanceID;
    float m_startX;
    float m_startY;
    float m_startZ;
    float m_startO;

    uint32 m_FeastSpamTimer;
};

#endif
