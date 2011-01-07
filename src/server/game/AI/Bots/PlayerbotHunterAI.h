#ifndef _PLAYERHUNTERAI_H
#define _PLAYERHUNTERAI_H

#include "PlayerbotClassAI.h"

//class Player;

class PlayerbotHunterAI : PlayerbotClassAI
{
    public:
        PlayerbotHunterAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotHunterAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        bool HasPet(Player* bot);

        virtual void Pull();

        //buff a specific player, usually a real PC who is not in group
        //void BuffPlayer(Player *target);

    private:
        //Hunter
        bool m_petSummonFailed;
        bool m_petFeedPetFailed;

        // PET CONTROL
        uint32 PET_SUMMON, PET_DISMISS, PET_REVIVE, PET_MEND, PET_FEED, KILL_COMMAND, INTIMIDATION, BESTIAL_WRATH;

        // PET SPELL
        uint32 GROWL, COWER, BAD_ATTITUDE, SONIC_BLAST, NETHER_SHOCK, DEMORALIZING_SCREECH;

        // RANGED ATTACK
        uint32 AUTO_SHOT, ARCANE_SHOT, EXPLOSIVE_SHOT, STEADY_SHOT, AIMED_SHOT, SCATTER_SHOT, KILL_SHOT, CHIMERA_SHOT, CONCUSSIVE_SHOT, DISTRACTING_SHOT, SILENCING_SHOT;

        // STINGS
        uint32 SERPENT_STING, SCORPID_STING, WYVERN_STING, VIPER_STING;

        // DEBUFF
        uint32 HUNTERS_MARK, SCARE_BEAST;

        //AOE
        uint32 VOLLEY, MULTI_SHOT;

        //MELEE
        uint32 RAPTOR_STRIKE, WING_CLIP, MONGOOSE_BITE, COUNTERATTACK;

        //TRAP
        uint32 FREEZING_TRAP, IMMOLATION_TRAP, FROST_TRAP, EXPLOSIVE_TRAP, SNAKE_TRAP, ARCANE_TRAP, FREEZING_ARROW, BLACK_ARROW;

        //BUFF
        uint32 TRUESHOT_AURA, DETERRENCE, FEIGN_DEATH, DISENGAGE, RAPID_FIRE, READINESS, MISDIRECTION;

        //ASPECT
        uint32 ASPECT_OF_THE_HAWK, ASPECT_OF_THE_MONKEY, ASPECT_OF_THE_VIPER;

        uint32 TALENT_MM, TALENT_BM, TALENT_SURVIVAL;

};

#endif


