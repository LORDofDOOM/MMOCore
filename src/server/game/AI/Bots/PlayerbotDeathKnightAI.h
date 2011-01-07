#ifndef _PLAYERDEATHKNIGHTAI_H
#define _PLAYERDEATHKNIGHTAI_H

#include "PlayerbotClassAI.h"

//class Player;

class PlayerbotDeathKnightAI : PlayerbotClassAI
{
    public:
        PlayerbotDeathKnightAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotDeathKnightAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        void Pull();

    private:
        // Unholy
        uint32 PLAGUE_STRIKE, DEATH_STRIKE, SCOURGE_STRIKE;

        // Frost
        uint32 ICY_TOUCH, OBLITERATE;

        // Blood
        uint32 BLOOD_STRIKE, HEART_STRIKE, RUNE_TAP, DARK_COMMAND;

        // AOE
        uint32 HOWLING_BLAST, BLOOD_BOIL, PESTILENCE, CORPSE_EXPLOSION, DEATH_AND_DECAY;

        // Rune attacks
        uint32 FROST_STRIKE, DEATH_COIL, RUNE_STRIKE;

        // CC Interrupt
        uint32 DEATH_GRIP, CHAINS_OF_ICE, MIND_FREEZE, HUNGERING_COLD, STRANGULATE;

        // Debuffs
        uint32 FROST_FEVER, BLOOD_PLAGUE, CRYPT_FEVER, EBON_PLAGUE, MARK_OF_BLOOD;

        // Buffs
        uint32 HORN_OF_WINTER, BONE_SHIELD, VAMPIRIC_BLOOD, HYSTERIA, UNBREAKABLE_ARMOR, ANTI_MAGIC_SHELL, ANTI_MAGIC_ZONE, ICEBOUND_FORTITUDE, EMPOWER_WEAPON, LICHBORNE;

        // Summons
        uint32 RAISE_DEAD, ARMY_OF_THE_DEAD, SUMMON_GARGOYLE, GHOUL_FRENZY, DEATH_PACT, DANCING_WEAPON;

        // Presences
        uint32 BLOOD_PRESENCE, FROST_PRESENCE, UNHOLY_PRESENCE;

        // Talent
        uint32 TALENT_BLOOD, TALENT_FROST, TALENT_UNHOLY;

};

#endif

