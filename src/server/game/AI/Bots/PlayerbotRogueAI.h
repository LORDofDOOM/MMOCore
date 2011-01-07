#ifndef _PLAYERBOTROGUEAI_H
#define _PLAYERBOTROGUEAI_H

#include "PlayerbotClassAI.h"
#include "TargetedMovementGenerator.h"

class PlayerbotRogueAI : PlayerbotClassAI
{
    public:
        PlayerbotRogueAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
        virtual ~PlayerbotRogueAI();

        virtual void LoadSpells();

        //all combat actions go here
        void DoNextCombatManeuver(Unit *);

        //all non combat actions go here, ex buffs, heals, REBIRTHes
        void DoNonCombatActions();

        //buff a specific player, usually a real PC who is not in group
        bool ChangeWeaponEnchants();

    private:
        //Damage spells
        uint32 BACKSTAB, SINISTER_STRIKE, MUTILATE, HEMORRHAGE, GHOSTLY_STRIKE, RIPOSTE, SHIV, FAN_OF_KNIVES;
        //Finishing Moves
        uint32 EVISCERATE, RUPTURE, KIDNEY_SHOT, ENVENOM, SLICE_AND_DICE, EXPOSE_ARMOR, DEADLY_THROW;
        //Buffs
        uint32 STEALTH, VANISH, EVASION, CLOAK_OF_SHADOWS, SPRINT, COLD_BLOOD, HUNGER_FOR_BLOOD, BLADE_FLURRY, ADRENALINE_RUSH, KILLING_SPREE, SHADOW_DANCE;
        //Openers
        uint32 CHEAP_SHOT, GARROTE, AMBUSH;
        //Others
        uint32 GOUGE, BLIND, DISMANTLE, SAP, KICK, PREPARATION, PREMEDITATION, SHADOWSTEP, FEINT, TRICKS_OF_THE_TRADE;

        uint32 TALENT_ASSASSINATION, TALENT_COMBAT, TALENT_SUBTELTY;

        uint32 THROW;

};
#endif
