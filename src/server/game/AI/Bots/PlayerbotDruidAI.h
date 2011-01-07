#ifndef _PLAYERBOTDRUIDAI_H
#define _PLAYERBOTDRUIDAI_H

#include "PlayerbotClassAI.h"

class PlayerbotDruidAI : PlayerbotClassAI
{
    public:
        PlayerbotDruidAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotDruidAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        //buff a specific player, usually a real PC who is not in group
        bool BuffPlayer(Unit *target);

        //Heals the target based off its HP
        bool HealTarget(Unit *target, uint8 hp);

        bool HealGroup(Unit *target, uint8 hp, uint8 &countNeedHeal);

        //Cures the target
        bool CureTarget(Unit *target);

        bool RezTarget(Unit *target);

        // find any specific mount spells, ie druids=cat, shaman=ghost wolf etc
        /* virtual bool FindMount();
        virtual bool Unmount();
        virtual bool IsMounted(); */

        //Change Form
        bool ChangeForm(uint32 form);

    private:

        // BALANCE Attacks
        uint32 MOONFIRE, WRATH, STARFALL, STARFIRE, TYPHOON, HURRICANE, FORCE_OF_NATURE, INSECT_SWARM, CYCLONE, ROOTS, NATURES_GRASP, HIBERNATE, FAERIE_FIRE;

        // RESTORATION Spells
        uint32 LIFEBLOOM, REJUVENATION, REGROWTH, NOURISH, SWIFTMEND, HEALING_TOUCH, NATURES_SWIFTNESS, INNERVATE, WILD_GROWTH, TRANQUILITY, REBIRTH, REVIVE, CURE_POISON, BARKSKIN;

        // BEAR SPELLS
        uint32 MAUL, BASH, LACERATE, MANGLE_BEAR, SWIPE_BEAR, DEMORALIZING_ROAR, GROWL, CHALLENGING_ROAR , ENRAGE, FERAL_CHARGE_BEAR, FRENZIED_REGENERATION;

        // CAT SPELLS
        uint32 CLAW, RAKE, SHRED, MANGLE_CAT, RIP, FEROCIOUS_BITE, SAVAGE_ROAR, MAIM, FERAL_CHARGE_CAT, COWER, TIGERS_FURY;

        // FERAL General
        uint32 BERSERK, FAERIE_FIRE_FERAL;

        // BUFFS
        uint32 MARK_OF_THE_WILD, GIFT_OF_THE_WILD, THORNS, SURVIVAL_INSTINCTS;

        // FORMS
        uint32 CAT_FORM, BEAR_FORM, MOONKIN_FORM, TREE_OF_LIFE_FORM, TRAVEL_FORM, FLIGHT_FORM, AQUATIC_FORM;

        // Key TALENT SPELLS
        uint32 TALENT_BALANCE, TALENT_RESTO, TALENT_FERAL;

};


#endif
