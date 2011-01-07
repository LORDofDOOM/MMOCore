#ifndef _PlayerbotWarlockAI_H
#define _PlayerbotWarlockAI_H

#include "PlayerbotClassAI.h"

//class Player;

class PlayerbotWarlockAI : PlayerbotClassAI
{
    public:
        PlayerbotWarlockAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotWarlockAI();

        virtual void LoadSpells();

    //all combat actions go here
    void DoNextCombatManeuver(Unit *);

    //all non combat actions go here, ex buffs, heals, rezzes
    void DoNonCombatActions();

    //buff a specific player, usually a real PC who is not in group
    bool BuffPlayer(Unit *target);

    uint32 CheckCurse(Unit *target);

  private:
    //CURSES
    uint32 CURSE_OF_ELEMENTS, CURSE_OF_WEAKNESS, CURSE_OF_AGONY, CURSE_OF_RECKLESSNESS, CURSE_OF_TONGUES, CURSE_OF_DOOM, CURSE_OF_EXHAUSTION;

    //AFFLICTION
    uint32 CORRUPTION, DRAIN_SOUL, DRAIN_LIFE, DRAIN_MANA, SIPHON_LIFE, UNSTABLE_AFFLICTION, HAUNT, SEED_OF_CORRUPTION, DEATH_COIL;

    //DESTRUCTION
    uint32 SHADOW_BOLT, IMMOLATE, INCINERATE, SEARING_PAIN, CONFLAGRATE, SOUL_FIRE, SHADOWBURN, SHADOWFURY, CHAOS_BOLT, SHADOWFLAME, RAIN_OF_FIRE, HELLFIRE;

    //DEMONOLOGY
    uint32 DEMON_ARMOR, FEL_ARMOR, SOULSHATTER, HEALTH_FUNNEL, DARK_PACT, SOUL_LINK, DEMONIC_EMPOWERMENT, METAMORPHOSIS, SUMMON_IMP, SUMMON_VOIDWALKER, SUMMON_SUCCUBUS, SUMMON_FELHUNTER, SUMMON_FELGUARD;

    //CC
    uint32 FEAR, HOWL_OF_TERROR, BANISH, ENSLAVE_DEMON;

    //Buff
    uint32 UNENDING_BREATH, DETECT_INVISIBILITY, SHADOW_WARD;

    //Other
    uint32 LIFE_TAP, CREATE_SOULSTONE;

    //Special
    uint32 SOUL_SHARD, P_BACKLASH, P_NIGHTFALL, SHOOT;

    uint32 TALENT_DEMONOLOGY, TALENT_AFFLICTION, TALENT_DESTRUCTION;
};

#endif


