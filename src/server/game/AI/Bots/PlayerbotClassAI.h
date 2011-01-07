#ifndef _PLAYERBOTCLASSAI_H
#define _PLAYERBOTCLASSAI_H

#include "Common.h"
#include "World.h"
#include "SpellMgr.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "Unit.h"
#include "SharedDefines.h"
#include "PlayerbotAI.h"
#include "SpellAuras.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"


class Player;
class PlayerbotAI;
class Aura;

        enum BotRole
        {
            BOT_ROLE_NONE,
            BOT_ROLE_TANK,
            BOT_ROLE_OFFTANK,
            BOT_ROLE_DPS_RANGED,
            BOT_ROLE_DPS_MELEE,
            BOT_ROLE_SUPPORT,
            BOT_ROLE_HEALER
        };

class PlayerbotClassAI
{
    public:
        PlayerbotClassAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotClassAI();

        //all combat actions go here
        virtual void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        virtual void DoNonCombatActions();

        //buff a specific player, usually a real PC who is not in group
        virtual bool BuffPlayer(Unit *target);

        //Heals the target based off its HP
        virtual bool HealTarget (Unit *target, uint8 hp);

        //Heals the group based off its HP
        virtual bool HealGroup (Unit *target, uint8 hp, uint8 &countNeedHeal);

        //Cure the target
        virtual bool CureTarget (Unit *target);

        //Resurrect the target (OBSOLETE - Check individual ClassAIs instead)
        virtual bool RezTarget(Unit *target);

        //find any specific mount spells, ie druids = cat, shaman = ghost wolf etc (OBSOLETE)
        virtual bool FindMount();

        virtual bool Unmount();

        virtual bool IsMounted();

        virtual void LoadSpells();

        virtual void Pull();

        //Utilities
        Player *GetMaster (){ return m_master; }
        Player *GetPlayerBot(){ return m_bot; }
        PlayerbotAI *GetAI (){ return m_ai; }

        bool isPulling() { return m_pulling; }
        bool TakePosition(Unit *followTarget, BotRole bRole=BOT_ROLE_NONE, float bDist=0, float bMinDist=0, float bMaxDist=0, float bAngle=0, Unit *faceTarget=NULL);
        //Gets the threat done by bot / threat max (percent) to the target.
        uint8 GetThreatPercent(Unit *pTarget, Unit *pFrom = NULL);
        //Gets if the unit is under attack by # of attackers
        bool isUnderAttack(Unit *pAttacked=NULL,const uint8 &minNumberOfAttackers=1);
        //Gets the first found attacker of Unit
        Unit *GetAttackerOf(Unit *pAttacked=NULL);
        //Gets the nearest attacker of Unit if not nearestToAttacked > finds the one nearest to bot
        Unit *GetNearestAttackerOf(Unit *pAttacked=NULL, bool nearestToAttacked=false);
        //Calculates Average Raid Health condition as Percentage, ref value is the Count of units need healing..
        uint8 GetHealthPercentRaid(Player *gPlayer, uint8 &countNeedHealing);

        // Called when the main tank is set from raid ui
        void SetMainTank (Unit *tank);

        //Finds the possible MainTank in Raid including Hunter/Warlock pets.. Makes the assumption based on - max maxHealth..
        Unit *FindMainTankInRaid(Player *gPlayer);

        //Finds the possible MainAssist in Raid.  Defaults to Main Tank if it cannot find one.
        Unit *FindMainAssistInRaid(Player *gPlayer);

        Player *FindMage(Player *gPlayer);
        //Finds the lowest hp creature around that is friendly with the caster.
        Unit *DoSelectLowestHpFriendly(float range, uint32 MinHPDiff);


        protected:
        bool CastSpell(uint32 spellId, Unit *target=NULL, bool checkFirst=true, bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false, bool triggered=false);
        bool CastSpell(const SpellEntry * pSpellInfo, Unit *target=NULL, bool checkFirst=true, bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false, bool triggered=false);
        bool CanCast(uint32 spellId, Unit *target=NULL,bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false);
        bool CanCast(const SpellEntry * pSpellInfo, Unit *target=NULL,bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false);

        //Debug method to list the auras currently active.
        //Use to find what spells were casted
        bool listAuras(Unit *unit);

        //More generalized method than HasAura().  It looks for
        //any rank of the spell and it doesn't care which
        //spell effect you want.  If it has the spell aura than
        //it returns true
        bool HasAuraName(Unit *unit, std::string spell, uint64 casterGuid=0);
        bool HasAuraName(Unit *unit, uint32 spellId, uint64 casterGuid=0);

        //The following functions return true only a match is found and the bot successfully casted a spell to resolve the problem
        //If the result is false, either a match is not found, or the ClassAI could not cast or refused to cast a spell for some reason..

        //Combination of all Healer roles, scans the party and decides if group healing > individual healing > Rez > curing > buffing is needed
        //and directs any matches found to individual ClassAIs
        //Main Raid scan function for Healer/Support types..
        bool DoSupportRaid(Player *gPlayer, float radius=30, bool dResurrect=true, bool dGroupHeal=true, bool dHeal=true, bool dCure=true, bool dBuff=true);
        //Find matching debuffs on target to provided Spell, and call castSpell() with provided parameters
        bool castDispel (uint32 dispelSpell, Unit *dTarget, bool checkFirst=true, bool castExistingAura=false, bool skipFriendlyCheck=false, bool skipEquipStanceCheck=false);
        //Cast matching debuffs on self with probided SpellId list.. Mainly Used for Racial spells.. List is used to prevent extra loops for each spell..
        bool castSelfCCBreakers (uint32 castList[]);



        typedef std::set<Unit *> AttackerSet;

        uint8 rezSpamTimer;
        uint32 foodDrinkSpamTimer;
        static const uint32 foodDrinkSpamCount = 100;

        BotRole m_role;
        bool m_pulling;
        uint32 threatThreshold, offensiveSpellThreshold;

        // RACIAL SPELLS
        uint32 R_ARCANE_TORRENT, R_BERSERKING, R_BLOOD_FURY, R_CANNIBALIZE, R_ESCAPE_ARTIST, R_EVERY_MAN_FOR_HIMSELF, R_GIFT_OF_NAARU, R_SHADOWMELD, R_STONEFORM, R_WAR_STOMP, R_WILL_OF_FORSAKEN;
        // first aid
        uint32 RECENTLY_BANDAGED;
        uint32 SHOOT;

    private:
        Player *m_master;
        Player *m_bot;
        PlayerbotAI *m_ai;
        Unit *mainTank;



};

#endif
