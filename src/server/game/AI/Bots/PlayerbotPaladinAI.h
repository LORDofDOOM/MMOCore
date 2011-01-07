#ifndef _PLAYERBOTPALADINAI_H
#define _PLAYERBOTPALADINAI_H

#include "PlayerbotClassAI.h"
#include "SharedDefines.h"

class PlayerbotPaladinAI : PlayerbotClassAI
{
    public:
        PlayerbotPaladinAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotPaladinAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        //buff a specific player, usually a real PC who is not in group
        bool BuffPlayer(Unit *target);

        //Heals the target based off its HP
        bool HealTarget(Unit *target, uint8 hp);

        //Cures the target
        bool CureTarget(Unit *target);

        bool RezTarget (Unit *target);

        bool ChangeAura(uint32 aura);

    private:
        //heals
        uint32 FLASH_OF_LIGHT, HOLY_LIGHT, HOLY_SHOCK, REZZ, CLEANSE, LOH, SACRED_SHIELD, BEACON_OF_LIGHT, DIVINE_FAVOR, REDEMPTION;

        //Damages
        uint32 JOL, JOW, JOJ, HAMMER_OF_WRATH, EXORCISM, HOLY_WRATH, CONSECRATION, AVENGERS_SHIELD, SHIELD_OF_RIGHTEOUSNESS, HOTR, CRUSADER_STRIKE, DIVINE_STORM;

        //CC
        uint32 HAMMER_OF_JUSTICE, REPENTANCE;

        //Self buffs
        uint32 SOL, SOW, SOR, SOC, SOV, DIVINE_PLEA, HOLY_SHIELD, RIGHTEOUS_FURY, DIVINE_SHIELD, AVENGING_WRATH;

        //AURAS
        uint32 DEVOTION_AURA, RETRIBUTION_AURA, CONCENTRATION_AURA, FIRE_AURA, FROST_AURA, SHADOW_AURA, CRUSADER_AURA ;

        //Blessings
        uint32 BOW, BOM, BOS, BOK, GBOW, GBOM, GBOS, GBOK;

        //Hands
        uint32 HOF, HOR, HOS, HOP, DIVINE_SACRIFICE;

        //Taunt
        uint32 RIGHTEOUS_DEFENSE;

        uint32 FORBEARANCE;

        uint32 TALENT_HOLY, TALENT_PROT, TALENT_RETRI;

		//procs
		uint32 AOW;

};



#endif
