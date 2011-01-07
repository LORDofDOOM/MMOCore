#ifndef _PLAYERBOTWARRIORAI_H
#define _PLAYERBOTWARRIORAI_H

#include "PlayerbotClassAI.h"

class PlayerbotWarriorAI : PlayerbotClassAI
{
    public:
        PlayerbotWarriorAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotWarriorAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        virtual void Pull();

    private:
        //Defensive Stance
        uint32 SHIELD_WALL, REVENGE, SHIELD_BLOCK, DISARM, INTERVENE;

        //Berserker Stance
        uint32 RECKLESSNESS, WHIRLWIND, PUMMEL, INTERCEPT;

        //Battle Stance
        uint32 RETALIATION, CHARGE, OVERPOWER, SHATTERING_THROW;

        //Mixed Attacks                                              //1 3
        uint32 REND, THUNDER_CLAP, SPELL_REFLECTION, SHIELD_BASH, EXECUTE, HAMSTRING, SWEEPING_STRIKES, VICTORY_RUSH;

        //General attacks
        uint32 HEROIC_STRIKE, MORTAL_STRIKE, BLOODTHIRST, SHIELD_SLAM, SHOCKWAVE, SLAM, CLEAVE, BLADESTORM, HEROIC_THROW, CONCUSSION_BLOW, SUNDER_ARMOR, DEMORALIZING_SHOUT, INTIMIDATING_SHOUT, PIERCING_HOWL, DEVASTATE;

        //buffs
        uint32 COMMANDING_SHOUT, BATTLE_SHOUT, VIGILANCE, BERSERKER_RAGE, ENRAGED_REGENERATION, BLOODRAGE, LAST_STAND, HEROIC_FURY, DEATH_WISH;

        //Stances
        uint32 DEFENSIVE_STANCE, BATTLE_STANCE, BERSERKER_STANCE;

        //Taunts
        uint32 TAUNT, CHALLENGING_SHOUT, MOCKING_BLOW;

        //Special
        uint32 SLAMM;

        uint32 TALENT_ARMS, TALENT_FURY, TALENT_PROT;

        bool ChangeStance(uint32 stance);


};

#endif
