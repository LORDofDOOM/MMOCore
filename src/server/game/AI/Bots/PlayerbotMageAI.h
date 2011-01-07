#ifndef _PLAYERBOTMAGEAI_H
#define _PLAYERBOTMAGEAI_H

#include "PlayerbotClassAI.h"

enum
{
    SPELL_SCORCH,
    SPELL_POM,
    SPELL_ARCANE_POWER,
    SPELL_FIREBALL,
    SPELL_MISSILES,
    SPELL_FROSTBOLT
};

//class Player;

class PlayerbotMageAI : PlayerbotClassAI
{
    public:
        PlayerbotMageAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotMageAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        //buff a specific player, usually a real PC who is not in group
        bool BuffPlayer(Unit *target);

        //Cures the target
        bool CureTarget(Unit *target);

        typedef std::set<Unit *> AttackerSet;
    private:
        //arcane
        uint32 ARCANE_MISSILES, ARCANE_EXPLOSION, ARCANE_BLAST, ARCANE_BARRAGE;

        //fire
        uint32 FIREBALL, FROSTFIRE_BOLT, FIRE_BLAST, FLAMESTRIKE, BLAST_WAVE, SCORCH, PYROBLAST, LIVING_BOMB;

        //cold
        uint32 FROSTBOLT, FROST_NOVA, ICE_LANCE, BLIZZARD, CONE_OF_COLD, WATER_ELEMENTAL;

        // buffs
        uint32 FROST_ARMOR, ICE_ARMOR, MAGE_ARMOR, MOLTEN_ARMOR, FIRE_WARD, FROST_WARD, MANA_SHIELD, ICE_BARRIER, POM, FOCUS_MAGIC, ARCANE_POWER, COMBUSTION, ICY_VEINS,
            ARCANE_INTELLECT, ARCANE_BRILLIANCE, DALARAN_INTELLECT, DALARAN_BRILLIANCE, DAMPEN_MAGIC, AMPLIFY_MAGIC;

        //CC
        uint32 POLYMORPH, DRAGONS_BREATH, DEEP_FREEZE;

        //other
        uint32 CONJURE_REFRESHMENT, CONJURE_WATER, CONJURE_FOOD, CONJURE_MANA_GEM, MIRROR_IMAGE, BLINK, ICE_BLOCK, INVISIBILITY, EVOCATION, REMOVE_CURSE, COUNTER_SPELL, SLOW, SHOOT;

        //special
        uint32 P_BRAIN_FREEZE, P_FIRESTARTER, P_HOT_STREAK, P_ARCANE_BLAST, P_MISSILE_BARRAGE, P_FINGERS_OF_FROST, IMP_SCORCH;


        uint32 TALENT_ARCANE, TALENT_FIRE, TALENT_FROST;

};



#endif
