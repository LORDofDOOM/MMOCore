#ifndef _PLAYERBOTSHAMANAI_H
#define _PLAYERBOTSHAMANAI_H

#include "PlayerbotClassAI.h"

class PlayerbotShamanAI : PlayerbotClassAI
{
    public:
        PlayerbotShamanAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotShamanAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, REBIRTHes
        void DoNonCombatActions();

        //buff a specific player, usually a real PC who is not in group
        bool BuffPlayer(Unit *target);

        //Heals the target based off its HP
        bool HealTarget(Unit *target, uint8 hp);

        bool HealGroup (Unit *target, uint8 hp, uint8 &countNeedHeal);

        //Cures the target
        bool CureTarget(Unit *target);

        bool RezTarget (Unit *target);

        bool ChangeTotems(uint32 mode);

        bool ChangeWeaponEnchants();

        /*//find any specific mount spells, ie druids=cat, shaman=ghost wolf etc
        virtual bool FindMount();
        virtual bool Unmount();
        virtual bool IsMounted(); */

    private:

        //totems
        uint32 HEALING_STREAM_TOTEM, MANA_SPRING_TOTEM, MANA_TIDE_TOTEM, CLEANSING_TOTEM, FIRE_RESISTANCE_TOTEM; //water
        uint32 WINDFURY_TOTEM, WRATH_OF_AIR_TOTEM, GROUNDING_TOTEM, NATURE_RESISTANCE_TOTEM;  //air
        uint32 STRENGTH_OF_EARTH_TOTEM, EARTHBIND_TOTEM, STONESKIN_TOTEM, STONECLAW_TOTEM, TREMOR_TOTEM, EARTH_ELEMENTAL_TOTEM ; //earth
        uint32 FLAMETONGUE_TOTEM, TOTEM_OF_WRATH, SEARING_TOTEM, MAGMA_TOTEM, FIRE_ELEMENTAL_TOTEM, FROST_RESISTANCE_TOTEM; //fire
        uint32 TOTEMIC_RECALL, CALL_OF_THE_ELEMENTS, CALL_OF_THE_ANCESTORS, CALL_OF_THE_SPIRITS;

        //restoration
        uint32 HEAL, LESSER_HEAL, CHAIN_HEAL, RIPTIDE, ANCESTRAL_SPIRIT, CLEANSE_SPIRIT;

        //offensive spells
        uint32 LIGHTNING_BOLT, CHAIN_LIGHTNING, FIRE_NOVA, THUNDERSTORM, LAVA_BURST, EARTH_SHOCK, WIND_SHEAR, FLAME_SHOCK, FROST_SHOCK, PURGE, HEX ;

        //buffs
        uint32 LIGHTNING_SHIELD, WATER_SHIELD, EARTH_SHIELD, HEROISM, ELEMENTAL_MASTERY, NATURES_SWIFTNESS,
            WINDFURY_WEAPON, FLAMETONGUE_WEAPON, FROSTBRAND_WEAPON, ROCKBITER_WEAPON, EARTHLIVING_WEAPON,
            WATER_BREATHING, WATER_WALKING ;

        //mellee
        uint32 LAVA_LASH, STORMSTRIKE, SHAMANISTIC_RAGE, FERAL_SPIRIT;

        uint32 GHOST_WOLF;

        //special
        uint32 EXHAUSTION, SATED, MAELSTROM_WEAPON;

        uint32 TALENT_ELEMENTAL, TALENT_ENHANCEMENT, TALENT_RESTO;

};


#endif
