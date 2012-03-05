/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Spell.h"
#include "Vehicle.h"
#include "MapManager.h"
#include "ruby_sanctum.h"

/*
TODO:
- Look over how threat should be treaten (apparently there's a creature (entry 40151) that keeps 200 threat on both Halion's)
- See if corporeality code can be improved
- Script adds and surrounding trash
*/

/*
Info list
- Twilight Halion seems to be spawned while Halion is spawned.
*/

enum Texts
{
    SAY_INTRO                        = 0, // Meddlesome insects! You are too late. The Ruby Sanctum is lost!
    SAY_AGGRO                        = 1, // Your world teeters on the brink of annihilation. You will ALL bear witness to the coming of a new age of DESTRUCTION!
    SAY_METEOR_STRIKE                = 2, // The heavens burn!
    SAY_PHASE_TWO                    = 3, // You will find only suffering within the realm of twilight! Enter if you dare!
    SAY_DEATH                        = 4, // Relish this victory, mortals, for it will be your last! This world will burn with the master's return!
    SAY_KILL                         = 5, // Another "hero" falls.
    SAY_BERSERK                      = 6, // Not good enough.

    SAY_SPHERE_PULSE                 = 0, // Beware the shadow!
    SAY_PHASE_THREE                  = 1, // I am the light and the darkness! Cower, mortals, before the herald of Deathwing!

    EMOTE_TWILIGHT_OUT_TWILIGHT      = 0, // Your companion's efforts have forced Halion further out of the Twilight realm!
    EMOTE_TWILIGHT_IN_TWILIGHT       = 1, // Your efforts have forced Halion further into the Twilight realm!
    EMOTE_PHYSICAL_OUT_PHYSICAL      = 2, // Your companion's efforts have forced Halion further out of the Physical realm!
    EMOTE_PHYSICAL_IN_PHYSICAL       = 3, // Your efforts have forced Halion further into the Physical realm!
    EMOTE_REGENERATE                 = 4, // Without pressure in both realms, Halion begins to regenerate.

    EMOTE_WARN_LASER                 = 0, // The orbiting spheres pulse with dark energy!
};

enum Spells
{
    // Halion
    SPELL_FLAME_BREATH                  = 74525,
    SPELL_CLEAVE                        = 74524,
    SPELL_METEOR_STRIKE                 = 74637,
    SPELL_TAIL_LASH                     = 74531,

    SPELL_FIERY_COMBUSTION              = 74562,
    SPELL_MARK_OF_COMBUSTION            = 74567,
    SPELL_FIERY_COMBUSTION_EXPLOSION    = 74607,
    SPELL_FIERY_COMBUSTION_SUMMON       = 74610,

    // Combustion & Consumption
    SPELL_SCALE_AURA                    = 70507, // Aura created in spell_dbc.
    SPELL_COMBUSTION_DAMAGE_AURA        = 74629,
    SPELL_CONSUMPTION_DAMAGE_AURA       = 74803,

    // Twilight Halion
    SPELL_DARK_BREATH                   = 74806,

    SPELL_MARK_OF_CONSUMPTION           = 74795,
    SPELL_SOUL_CONSUMPTION              = 74792,
    SPELL_SOUL_CONSUMPTION_EXPLOSION    = 74799,
    SPELL_SOUL_CONSUMPTION_SUMMON       = 74800,

    // Living Inferno
    SPELL_BLAZING_AURA                  = 75885,

    // Halion Controller
    SPELL_COSMETIC_FIRE_PILLAR          = 76006,
    SPELL_FIERY_EXPLOSION               = 76010,
    SPELL_CLEAR_DEBUFFS                 = 75396,

    // Meteor Strike
    SPELL_METEOR_STRIKE_COUNTDOWN       = 74641,
    SPELL_METEOR_STRIKE_AOE_DAMAGE      = 74648,
    SPELL_METEOR_STRIKE_FIRE_AURA_1     = 74713,
    SPELL_METEOR_STRIKE_FIRE_AURA_2     = 74718,
    SPELL_BIRTH_NO_VISUAL               = 40031,

    // Shadow Orb
    SPELL_TWILIGHT_CUTTER               = 74768, // Unknown dummy effect (EFFECT_0)
    SPELL_TWILIGHT_CUTTER_TRIGGERED     = 74769,
    SPELL_TWILIGHT_PULSE_PERIODIC       = 78861,
    SPELL_TRACK_ROTATION                = 74758,

    // Misc
    SPELL_TWILIGHT_DIVISION             = 75063,    // Phase spell from phase 2 to phase 3
    SPELL_LEAVE_TWILIGHT_REALM          = 74812,
    SPELL_TWILIGHT_PHASING              = 74808,    // Phase spell from phase 1 to phase 2
    SPELL_SUMMON_TWILIGHT_PORTAL        = 74809,    // Summons go 202794
    SPELL_TWILIGHT_MENDING              = 75509,
    SPELL_TWILIGHT_REALM                = 74807,

    SPELL_COPY_DAMAGE                   = 74810,
};

enum Events
{
    // Halion
    EVENT_ACTIVATE_FIREWALL     = 1,
    EVENT_CLEAVE                = 2, // Used by Twilight Halion too
    EVENT_FLAME_BREATH          = 3,
    EVENT_METEOR_STRIKE         = 4,
    EVENT_FIERY_COMBUSTION      = 5,
    EVENT_TAIL_LASH             = 6,

    // Halion Controller
    EVENT_START_INTRO           = 7,
    EVENT_INTRO_PROGRESS_1      = 8,
    EVENT_INTRO_PROGRESS_2      = 9,
    EVENT_INTRO_PROGRESS_3      = 10,
    EVENT_CHECK_CORPOREALITY    = 11,
    EVENT_SHADOW_PULSARS_SHOOT  = 12,
    EVENT_BERSERK               = 13,

    // Meteor Strike
    EVENT_SPAWN_METEOR_FLAME    = 14,

    // Twilight Halion
    EVENT_DARK_BREATH           = 15,
    EVENT_SOUL_CONSUMPTION      = 16,

    // Living Ember
    EVENT_EMBER_ENRAGE          = 17,

    // Misc
    EVENT_CHECK_THREAT          = 18,
    // This is all shitty for now. Halion, Twilight Halion, and Halion Controller will check their threat list
    // every two seconds and if they find out that either one of the NPCs lost aggro on any player, that would
    // mean that the encounter has to reset. Not yet implemented, this comment block is rather some way for me
    // to brain a bit about this fubarish stuff.
};

enum Actions
{
    // Meteor Strike
    ACTION_METEOR_STRIKE_BURN   = 1,
    ACTION_METEOR_STRIKE_AOE    = 2,

    // Halion
    ACTION_BERSERK              = 3,

    // Halion Controller
    ACTION_PHASE_TWO            = 4,
    ACTION_PHASE_THREE          = 5,
    ACTION_CLEANUP              = 6,

    // Orb Rotation Focus
    ACTION_BEGIN_ROTATION       = 7,

    // Orb Carrier
    ACTION_SHOOT                = 8,
};

enum Phases
{
    PHASE_ALL           = 0,
    PHASE_ONE           = 1,
    PHASE_TWO           = 2,
    PHASE_THREE         = 3,

    PHASE_ONE_MASK      = 1 << PHASE_ONE,
    PHASE_TWO_MASK      = 1 << PHASE_TWO,
    PHASE_THREE_MASK    = 1 << PHASE_THREE,
};

enum Misc
{
    DATA_TWILIGHT_DAMAGE_TAKEN   = 1,
    DATA_MATERIAL_DAMAGE_TAKEN   = 2,
    DATA_STACKS_DISPELLED        = 3,
    DATA_FIGHT_PHASE             = 4,
};

enum OrbCarrierSeats
{
    SEAT_NORTH            = 0,
    SEAT_SOUTH            = 1,
    SEAT_EAST             = 2,
    SEAT_WEST             = 3,
};

Position const HalionSpawnPos = {3156.67f, 533.8108f, 72.98822f, 3.159046f};

struct CorporealityData
{
    uint8 materialPercentage;
    uint32 materialRealmSpell;
    uint32 twilightRealmSpell;
};

uint8 const MAX_CORPOREALITY_STATE = 11;

CorporealityData const corporealityReference[MAX_CORPOREALITY_STATE] =
{
    {  0, 74836, 74831},
    { 10, 74835, 74830},
    { 20, 74834, 74829},
    { 30, 74833, 74828},
    { 40, 74832, 74827},
    { 50, 74826, 74826},
    { 60, 74827, 74832},
    { 70, 74828, 74833},
    { 80, 74829, 74834},
    { 90, 74830, 74835},
    {100, 74831, 74836},
};

class boss_halion : public CreatureScript
{
    public:
        boss_halion() : CreatureScript("boss_halion") { }

        struct boss_halionAI : public BossAI
        {
            boss_halionAI(Creature* creature) : BossAI(creature, DATA_HALION) { }

            void Reset()
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                _Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ADD, me, 1);

                events.Reset();
                events.SetPhase(PHASE_ONE);
                // Schedule events without taking care of phases, since EventMap will not be updated under phase 2.
                events.ScheduleEvent(EVENT_ACTIVATE_FIREWALL, 10000);
                events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 10000));
                events.ScheduleEvent(EVENT_FLAME_BREATH, urand(10000, 12000));
                events.ScheduleEvent(EVENT_METEOR_STRIKE, urand(20000, 25000));
                events.ScheduleEvent(EVENT_FIERY_COMBUSTION, urand(15000, 18000));
                events.ScheduleEvent(EVENT_TAIL_LASH, 10000);

                // Due to Halion's EventMap not being updated under phase two, Berserk will be triggered by the Controller
                // so that the timer still ticks in phase two.
                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->DoAction(ACTION_BERSERK);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);

                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->Reset();

                // This block shouldn't be needed anymore after spell 74810 is working.
                if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                    if (twilightHalion->isAlive())
                        twilightHalion->Kill(twilightHalion);
            }

            void JustReachedHome()
            {
                instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);

                if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->Reset();

                // I strongly begin to believe that both Halions are spawned after Halion's intro is processed.
                if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                    twilightHalion->DespawnOrUnsummon();

                _JustReachedHome();
            }

            Position const* GetMeteorStrikePosition() const { return &_meteorStrikePos; }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                if ((me->GetHealth() - damage) > 0 && (events.GetPhaseMask() & (PHASE_ONE_MASK | PHASE_THREE_MASK)))
                    if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TWILIGHT_HALION)))
                        me->CastCustomSpell(SPELL_COPY_DAMAGE, SPELLVALUE_BASE_POINT0, damage, twilightHalion, true);

                if (me->HealthBelowPctDamaged(75, damage) && (events.GetPhaseMask() & PHASE_ONE_MASK))
                {
                    events.SetPhase(PHASE_TWO);
                    events.DelayEvents(2600); // 2.5 sec + 0.1 sec lag

                    me->CastStop();
                    me->AttackStop();

                    Talk(SAY_PHASE_TWO);
                    DoCast(me, SPELL_TWILIGHT_PHASING);
                    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                        controller->AI()->DoAction(ACTION_PHASE_TWO);
                }

                if (events.GetPhaseMask() & PHASE_THREE_MASK)
                    if (Creature* controller = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_HALION_CONTROLLER)))
                        controller->AI()->SetData(DATA_MATERIAL_DAMAGE_TAKEN, damage);
            }

            bool CanAIAttack(Unit const* victim) { return !victim->HasAura(SPELL_TWILIGHT_REALM); }

            void UpdateAI(uint32 const diff)
            {
                if ((!UpdateVictim() && (events.GetPhaseMask() & (PHASE_ONE_MASK | PHASE_THREE_MASK))) || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                // Events won't be updated under phase two.
                if (!(events.GetPhaseMask() & PHASE_TWO_MASK))
                    events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ACTIVATE_FIREWALL:
                        {
                            // Firewall is activated 10 seconds after starting encounter, DOOR_TYPE_ROOM is only instant.
                            if (GameObject* firewall = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_FLAME_RING)))
                                instance->HandleGameObject(instance->GetData64(DATA_FLAME_RING), false, firewall);
                            if (GameObject* firewall = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_TWILIGHT_FLAME_RING)))
                                instance->HandleGameObject(instance->GetData64(DATA_TWILIGHT_FLAME_RING), false, firewall);
                            break;
                        }
                        case EVENT_FLAME_BREATH:
                            DoCast(me, SPELL_FLAME_BREATH);
                            events.ScheduleEvent(EVENT_FLAME_BREATH, 25000);
                            break;
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 10000));
                            break;
                        case EVENT_TAIL_LASH:
                            DoCastAOE(SPELL_TAIL_LASH);
                            events.ScheduleEvent(EVENT_TAIL_LASH, 10000);
                            break;
                        case EVENT_METEOR_STRIKE:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -SPELL_TWILIGHT_REALM))
                            {
                                target->GetPosition(&_meteorStrikePos);
                                me->CastSpell(_meteorStrikePos.GetPositionX(), _meteorStrikePos.GetPositionY(), _meteorStrikePos.GetPositionZ(), SPELL_METEOR_STRIKE, true, NULL, NULL, me->GetGUID());
                                Talk(SAY_METEOR_STRIKE);
                            }
                            events.ScheduleEvent(EVENT_METEOR_STRIKE, 40000);
                            break;
                        }
                        case EVENT_FIERY_COMBUSTION:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_TWILIGHT_REALM);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, -SPELL_TWILIGHT_REALM);
                            if (target)
                                DoCast(target, SPELL_FIERY_COMBUSTION);
                            events.ScheduleEvent(EVENT_FIERY_COMBUSTION, 25000);
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            void SetData(uint32 index, uint32 value)
            {
                if (index == DATA_FIGHT_PHASE)
                    events.SetPhase(value);
            }

            void DoAction(int32 const action)
            {
                if (action != ACTION_BERSERK)
                    return;

                Talk(SAY_BERSERK);
                DoCast(me, SPELL_BERSERK);
            }

        private:
            Position _meteorStrikePos;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<boss_halionAI>(creature);
        }
};

typedef boss_halion::boss_halionAI HalionAI;

class boss_twilight_halion : public CreatureScript
{
    public:
        boss_twilight_halion() : CreatureScript("boss_twilight_halion") { }

        struct boss_twilight_halionAI : public ScriptedAI
        {
            boss_twilight_halionAI(Creature* creature) : ScriptedAI(creature),
                _instance(creature->GetInstanceScript())
            {
                me->SetPhaseMask(0x20, true); // Should not be visible with phasemask 0x21, so only 0x20
                me->SetHealth(uint32(0.75f * me->GetMaxHealth()));
            }

            void EnterCombat(Unit* /*who*/)
            {
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_ADD, me, 2);
                events.Reset();
                events.SetPhase(PHASE_TWO);
                //! All of Twilight Halion's abilities are not phase dependant as he is never on Phase One.
                //! However, phasemasks are "needed" so that we know on which phase we are when Halion takes
                //! damage, causing corporeality not to tick in phase two.
                events.ScheduleEvent(EVENT_DARK_BREATH, urand(10000, 15000));
                events.ScheduleEvent(EVENT_SOUL_CONSUMPTION, 20000);
                events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 10000));
                events.ScheduleEvent(EVENT_TAIL_LASH, 10000);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);

                // Victims should not be in the Twilight Realm
                me->CastSpell(victim, SPELL_LEAVE_TWILIGHT_REALM, true);
            }

            void JustDied(Unit* killer)
            {
                if (Creature* halion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION)))
                {
                    // Ensure looting
                    if (me->IsDamageEnoughForLootingAndReward())
                        halion->LowerPlayerDamageReq(halion->GetMaxHealth());

                    if (halion->isAlive())
                        killer->Kill(halion);
                }

                if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->CastSpell(controller, SPELL_CLEAR_DEBUFFS);
                _instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);
            }

            void JustReachedHome()
            {
                // If the Twilight Halion enters evade mode on phase 2, the players in the Physical realm
                // should enter the Twilight Realm to end the fight (i.e. wipe).
                // As a consequence, the Twilight Halion entering evade mode does not end the encounter.
                if (events.GetPhaseMask() & PHASE_TWO_MASK)
                    return;

                _instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);
                ScriptedAI::JustReachedHome();
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage)
            {
                if (me->GetHealth() - damage > 0)
                    if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_TWILIGHT_HALION)))
                        me->CastCustomSpell(SPELL_COPY_DAMAGE, SPELLVALUE_BASE_POINT0, damage, twilightHalion, true);

                if (me->HealthBelowPctDamaged(50, damage) && (events.GetPhaseMask() & PHASE_TWO_MASK))
                {
                    events.SetPhase(PHASE_THREE);
                    events.DelayEvents(2600); // 2.5 sec + 0.1sec lag

                    me->CastStop();
                    DoCast(me, SPELL_TWILIGHT_DIVISION);
                    Talk(SAY_PHASE_THREE);
                    //! Stop here, else damage that triggered the phase change will be taken
                    //! into consideration in the next lines.
                    return;
                }

                if (events.GetPhaseMask() & PHASE_THREE_MASK)
                    if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                        controller->AI()->SetData(DATA_TWILIGHT_DAMAGE_TAKEN, damage);
            }

            void SpellHitTarget(Unit* /*who*/, SpellInfo const* spell)
            {
                if (spell->Id != SPELL_TWILIGHT_DIVISION)
                    return;

                DoCast(me, corporealityReference[5].twilightRealmSpell);
                if (Creature* halion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION)))
                {
                    halion->CastSpell(halion, corporealityReference[5].materialRealmSpell, false);
                    halion->RemoveAurasDueToSpell(SPELL_TWILIGHT_PHASING);
                    halion->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    halion->AI()->SetData(DATA_FIGHT_PHASE, PHASE_THREE);
                }

                if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->DoAction(ACTION_PHASE_THREE);
            }

            bool CanAIAttack(Unit const* victim)
            {
                return victim->HasAura(SPELL_TWILIGHT_REALM);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_BERSERK)
                    DoCast(me, SPELL_BERSERK);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DARK_BREATH:
                        {
                            DoCast(me, SPELL_DARK_BREATH);
                            events.ScheduleEvent(EVENT_DARK_BREATH, urand(10000, 15000));
                            break;
                        }
                        case EVENT_SOUL_CONSUMPTION:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, SPELL_TWILIGHT_REALM);
                            if (!target)
                                target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true, SPELL_TWILIGHT_REALM);
                            if (target)
                                DoCast(target, SPELL_SOUL_CONSUMPTION);
                            events.ScheduleEvent(EVENT_SOUL_CONSUMPTION, 20000);
                            break;
                        }
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            events.ScheduleEvent(EVENT_CLEAVE, urand(8000, 10000));
                            break;
                        case EVENT_TAIL_LASH:
                            DoCastAOE(SPELL_TAIL_LASH);
                            events.ScheduleEvent(EVENT_TAIL_LASH, 10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<boss_twilight_halionAI>(creature);
        }
};

typedef boss_twilight_halion::boss_twilight_halionAI twilightHalionAI;

class npc_halion_controller : public CreatureScript
{
    public:
        npc_halion_controller() : CreatureScript("npc_halion_controller") { }

        struct npc_halion_controllerAI : public ScriptedAI
        {
            npc_halion_controllerAI(Creature* creature) : ScriptedAI(creature),
                _instance(creature->GetInstanceScript()), _summons(me)
            {
                me->SetPhaseMask(me->GetPhaseMask() | 0x20, true);
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                _summons.DespawnAll();
                _events.Reset();
                DoCast(me, SPELL_CLEAR_DEBUFFS);
            }

            void JustSummoned(Creature* who)
            {
                _summons.Summon(who);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_INTRO_HALION:
                    {
                        _events.Reset();
                        _events.ScheduleEvent(EVENT_START_INTRO, 2000);
                        _events.ScheduleEvent(EVENT_INTRO_PROGRESS_1, 6000);
                        _events.ScheduleEvent(EVENT_INTRO_PROGRESS_2, 10000);
                        _events.ScheduleEvent(EVENT_INTRO_PROGRESS_3, 14000);
                        break;
                    }
                    case ACTION_PHASE_TWO:
                    {
                        _events.ScheduleEvent(EVENT_SHADOW_PULSARS_SHOOT, 10000); // Fix the timer
                        me->GetMap()->SummonCreature(NPC_TWILIGHT_HALION, HalionSpawnPos);
                        if (Creature* rotationFocus = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORB_ROTATION_FOCUS)))
                            rotationFocus->AI()->DoAction(ACTION_BEGIN_ROTATION);
                        break;
                    }
                    case ACTION_PHASE_THREE:
                    {
                        _events.ScheduleEvent(EVENT_CHECK_CORPOREALITY, 20000);
                        TwilightDamageTaken = 0;
                        MaterialDamageTaken = 0;
                        materialCorporealityValue = 50;
                        _instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TOGGLE, 1);
                        _instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_MATERIAL, 50);
                        _instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TWILIGHT, 50);
                        break;
                    }
                    case ACTION_BERSERK:
                    {
                        _events.ScheduleEvent(EVENT_BERSERK, 8 * MINUTE * IN_MILLISECONDS);
                        //! Here because this is called when Halion is engaged.
                        me->SetInCombatWithZone();
                        break;
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_START_INTRO:
                            DoCast(me, SPELL_COSMETIC_FIRE_PILLAR, true);
                            break;
                        case EVENT_INTRO_PROGRESS_1:
                            for (uint8 i = DATA_BURNING_TREE_3; i <= DATA_BURNING_TREE_4; ++i)
                                if (GameObject* tree = ObjectAccessor::GetGameObject(*me, _instance->GetData64(i)))
                                    _instance->HandleGameObject(_instance->GetData64(i), true, tree);
                            break;
                        case EVENT_INTRO_PROGRESS_2:
                            for (uint8 i = DATA_BURNING_TREE_1; i <= DATA_BURNING_TREE_2; ++i)
                                if (GameObject* tree = ObjectAccessor::GetGameObject(*me, _instance->GetData64(i)))
                                    _instance->HandleGameObject(_instance->GetData64(i), true, tree);
                            break;
                        case EVENT_INTRO_PROGRESS_3:
                            DoCast(me, SPELL_FIERY_EXPLOSION);
                            if (Creature* halion = me->GetMap()->SummonCreature(NPC_HALION, HalionSpawnPos))
                                halion->AI()->Talk(SAY_INTRO);
                            break;
                        case EVENT_SHADOW_PULSARS_SHOOT:
                        {
                            if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_TWILIGHT_HALION)))
                                twilightHalion->AI()->Talk(SAY_SPHERE_PULSE);

                            if (Creature* orbCarrier = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_ORB_CARRIER)))
                                orbCarrier->AI()->DoAction(ACTION_SHOOT);

                            _events.ScheduleEvent(EVENT_SHADOW_PULSARS_SHOOT, 29000);   // 9 sec channel duration, every 20th second
                            break;
                        }
                        case EVENT_CHECK_CORPOREALITY:
                        {
                            bool canUpdate = false;
                            if (MaterialDamageTaken != 0 && TwilightDamageTaken != 0)
                            {
                                // Guessed scaling
                                if (MaterialDamageTaken >= 1.02f * TwilightDamageTaken)
                                {
                                    TwilightDamageTaken = 0;
                                    MaterialDamageTaken = 0;
                                    canUpdate = (materialCorporealityValue != 0);
                                    if (canUpdate)
                                        materialCorporealityValue -= 10;
                                }
                                else if (TwilightDamageTaken >= 1.02f * MaterialDamageTaken)
                                {
                                    TwilightDamageTaken = 0;
                                    MaterialDamageTaken = 0;
                                    canUpdate = (materialCorporealityValue != 100);
                                    if (canUpdate)
                                        materialCorporealityValue += 10;
                                }
                            }
                            else
                            {
                                if (Creature* halion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION)))
                                {
                                    if (Creature* twilightHalion = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_TWILIGHT_HALION)))
                                    {
                                        twilightHalion->CastSpell(halion, SPELL_TWILIGHT_MENDING);

                                        Map::PlayerList const &PlList = me->GetMap()->GetPlayers();
                                        for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                                            if (Player* player = i->getSource())
                                                Talk(EMOTE_REGENERATE, player->GetGUID());
                                    }
                                }
                                _events.ScheduleEvent(EVENT_CHECK_CORPOREALITY, 15000);
                                break;
                            }

                            if (canUpdate)
                            {
                                for (uint8 i = 0; i < MAX_CORPOREALITY_STATE; i++)
                                {
                                    if (corporealityReference[i].materialPercentage == materialCorporealityValue)
                                    {
                                        for (uint8 j = DATA_HALION; j <= DATA_TWILIGHT_HALION; j++)
                                        {
                                            if (Creature* halion = ObjectAccessor::GetCreature(*me, _instance->GetData64(j)))
                                            {
                                                RemoveCorporeality(halion);
                                                halion->CastSpell(halion, (j == DATA_HALION ? corporealityReference[i].materialRealmSpell : corporealityReference[i].twilightRealmSpell), true);
                                            }
                                        }
                                        break; // No need to iterate more
                                    }
                                }

                                _instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_MATERIAL, materialCorporealityValue);
                                _instance->DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TWILIGHT, (100 - materialCorporealityValue));

                                Map::PlayerList const &playersList = me->GetMap()->GetPlayers();
                                for (Map::PlayerList::const_iterator i = playersList.begin(); i != playersList.end(); ++i)
                                {
                                    if (Player* player = i->getSource())
                                    {
                                        if (materialCorporealityValue > 50) // Range is 0 ... 100
                                        {
                                            if (player->HasAura(SPELL_TWILIGHT_REALM))
                                                Talk(EMOTE_TWILIGHT_OUT_TWILIGHT, player->GetGUID());
                                            else
                                                Talk(EMOTE_PHYSICAL_IN_PHYSICAL, player->GetGUID());
                                        }
                                        else
                                        {
                                            if (player->HasAura(SPELL_TWILIGHT_REALM))
                                                Talk(EMOTE_TWILIGHT_IN_TWILIGHT, player->GetGUID());
                                            else
                                                Talk(EMOTE_PHYSICAL_OUT_PHYSICAL, player->GetGUID());
                                        }
                                    }
                                }
                            }
                            _events.ScheduleEvent(EVENT_CHECK_CORPOREALITY, 15000);
                            break;
                        }
                        case EVENT_BERSERK:
                        {
                            for (uint8 i = DATA_HALION; i <= DATA_TWILIGHT_HALION; i++)
                                if (Creature* halion = ObjectAccessor::GetCreature(*me, _instance->GetData64(i)))
                                    halion->AI()->DoAction(ACTION_BERSERK);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void SetData(uint32 id, uint32 value)
            {
                switch (id)
                {
                    case DATA_MATERIAL_DAMAGE_TAKEN:
                        MaterialDamageTaken += value;
                        break;
                    case DATA_TWILIGHT_DAMAGE_TAKEN:
                        TwilightDamageTaken += value;
                        break;
                }
            }

        private:

            void RemoveCorporeality(Creature* who)
            {
                for (uint8 i = 0; i < MAX_CORPOREALITY_STATE; i++)
                {
                    if (who->HasAura(corporealityReference[i].materialRealmSpell))
                    {
                        who->RemoveAurasDueToSpell(corporealityReference[i].materialRealmSpell);
                        break;
                    }
                    if (who->HasAura(corporealityReference[i].twilightRealmSpell))
                    {
                        who->RemoveAurasDueToSpell(corporealityReference[i].twilightRealmSpell);
                        break;
                    }
                }
            }

            EventMap _events;
            InstanceScript* _instance;
            SummonList _summons;
            uint32 TwilightDamageTaken;
            uint32 MaterialDamageTaken;
            uint8 materialCorporealityValue;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_halion_controllerAI>(creature);
        }
};

typedef npc_halion_controller::npc_halion_controllerAI controllerAI;

class npc_meteor_strike_initial : public CreatureScript
{
    public:
        npc_meteor_strike_initial() : CreatureScript("npc_meteor_strike_initial") { }

        struct npc_meteor_strike_initialAI : public Scripted_NoMovementAI
        {
            npc_meteor_strike_initialAI(Creature* creature) : Scripted_NoMovementAI(creature),
                _instance(creature->GetInstanceScript())
            { }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_METEOR_STRIKE_AOE:
                        DoCast(me, SPELL_METEOR_STRIKE_AOE_DAMAGE, true);
                        DoCast(me, SPELL_METEOR_STRIKE_FIRE_AURA_1, true);
                        for (std::list<Creature*>::iterator itr = _meteorList.begin(); itr != _meteorList.end(); ++itr)
                            (*itr)->AI()->DoAction(ACTION_METEOR_STRIKE_BURN);
                        break;
                }
            }

            void IsSummonedBy(Unit* summoner)
            {
                Creature* owner = summoner->ToCreature();
                if (!owner)
                    return;

                // Let Halion Controller count as summoner
                if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->JustSummoned(me);

                DoCast(me, SPELL_METEOR_STRIKE_COUNTDOWN);
                DoCast(me, SPELL_BIRTH_NO_VISUAL); // Unknown purpose

                if (HalionAI* halionAI = CAST_AI(HalionAI, owner->AI()))
                {
                    Position const* ownerPos = halionAI->GetMeteorStrikePosition();
                    Position newPos;
                    float angle[4];
                    angle[0] = me->GetAngle(ownerPos);
                    angle[1] = me->GetAngle(ownerPos) - static_cast<float>(M_PI/2);
                    angle[2] = me->GetAngle(ownerPos) - static_cast<float>(-M_PI/2);
                    angle[3] = me->GetAngle(ownerPos) - static_cast<float>(M_PI);

                    _meteorList.clear();
                    for (uint8 i = 0; i < 4; i++)
                    {
                        MapManager::NormalizeOrientation(angle[i]);
                        me->SetOrientation(angle[i]);
                        me->GetNearPosition(newPos, 10.0f, 0.0f); // Exact distance
                        if (Creature* meteor = me->SummonCreature(NPC_METEOR_STRIKE_NORTH + i, newPos, TEMPSUMMON_TIMED_DESPAWN, 30000))
                            _meteorList.push_back(meteor);
                    }
                }
            }

            void UpdateAI(uint32 const /*diff*/) { }
            void EnterEvadeMode() { }

        private:
            InstanceScript* _instance;
            std::list<Creature*> _meteorList;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_meteor_strike_initialAI>(creature);
        }
};

class npc_meteor_strike : public CreatureScript
{
    public:
        npc_meteor_strike() : CreatureScript("npc_meteor_strike") { }

        struct npc_meteor_strikeAI : public Scripted_NoMovementAI
        {
            npc_meteor_strikeAI(Creature* creature) : Scripted_NoMovementAI(creature),
                _instance(creature->GetInstanceScript())
            {
                _range = 5.0f;
                _spawnCount = 0;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_METEOR_STRIKE_BURN)
                {
                    DoCast(me, SPELL_METEOR_STRIKE_FIRE_AURA_2, true);
                    me->setActive(true);
                    _events.ScheduleEvent(EVENT_SPAWN_METEOR_FLAME, 500);
                }
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                // Let Halion Controller count as summoner.
                if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->JustSummoned(me);
            }

            void UpdateAI(uint32 const diff)
            {
                if (_spawnCount > 5)
                    return;

                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_SPAWN_METEOR_FLAME)
                {
                    Position pos;
                    me->GetNearPosition(pos, _range, 0.0f);

                    if (Creature* flame = me->SummonCreature(NPC_METEOR_STRIKE_FLAME, pos, TEMPSUMMON_TIMED_DESPAWN, 25000))
                    {
                        if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                            controller->AI()->JustSummoned(flame);

                        flame->CastSpell(flame, SPELL_METEOR_STRIKE_FIRE_AURA_2, true);
                        ++_spawnCount;
                    }
                    _range += 5.0f;
                    _events.ScheduleEvent(EVENT_SPAWN_METEOR_FLAME, 800);
                }
            }

        private:
            InstanceScript* _instance;
            EventMap _events;
            float _range;
            uint8 _spawnCount;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_meteor_strikeAI>(creature);
        }
};

class npc_combustion_consumption : public CreatureScript
{
    public:
        npc_combustion_consumption() : CreatureScript("npc_combustion_consumption") { }

        struct npc_combustion_consumptionAI : public Scripted_NoMovementAI
        {
            npc_combustion_consumptionAI(Creature* creature) : Scripted_NoMovementAI(creature),
                   _summonerGuid(0), _instance(creature->GetInstanceScript())
            {
                switch (me->GetEntry())
                {
                    case NPC_COMBUSTION:
                        _explosionSpell = SPELL_FIERY_COMBUSTION_EXPLOSION;
                        _damageSpell = SPELL_COMBUSTION_DAMAGE_AURA;
                        break;
                    case NPC_CONSUMPTION:
                        _explosionSpell = SPELL_SOUL_CONSUMPTION_EXPLOSION;
                        _damageSpell = SPELL_CONSUMPTION_DAMAGE_AURA;
                        me->SetPhaseMask(0x20, true);
                        break;
                    default: // Should never happen
                        _explosionSpell = 0;
                        _damageSpell = 0;
                        break;
                }
                if (IsHeroic())
                    me->SetPhaseMask(0x01 | 0x20, true);
            }

            void IsSummonedBy(Unit* summoner)
            {
                // Let Halion Controller count as summoner
                if (Creature* controller = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HALION_CONTROLLER)))
                    controller->AI()->JustSummoned(me);

                _summonerGuid = summoner->GetGUID();
            }

            void SetData(uint32 type, uint32 value)
            {
                Unit* summoner = ObjectAccessor::GetUnit(*me, _summonerGuid);

                if (type != DATA_STACKS_DISPELLED || !_damageSpell || !_explosionSpell || !summoner)
                    return;

                me->CastCustomSpell(SPELL_SCALE_AURA, SPELLVALUE_AURA_STACK, value, me);
                DoCast(me, _damageSpell);

                int32 damage = 1200 + (value * 1290); // Needs moar research.
                // Target is TARGET_UNIT_AREA_ALLY_SRC (TARGET_SRC_CASTER)
                summoner->CastCustomSpell(_explosionSpell, SPELLVALUE_BASE_POINT0, damage, summoner);
            }

            void UpdateAI(uint32 const /*diff*/) { }

        private:
            InstanceScript* _instance;
            uint32 _explosionSpell;
            uint32 _damageSpell;
            uint64 _summonerGuid;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_combustion_consumptionAI>(creature);
        }
};

class npc_orb_carrier : public CreatureScript
{
    public:
        npc_orb_carrier() : CreatureScript("npc_orb_carrier") { }

        struct npc_orb_carrierAI : public ScriptedAI
        {
            npc_orb_carrierAI(Creature* creature) : ScriptedAI(creature)
            {
                ASSERT(creature->GetVehicleKit());
            }

            void UpdateAI(uint32 const diff)
            {
                //! According to sniffs this spell is cast every 1 or 2 seconds.
                //! However, refreshing it looks bad, so just cast the spell if
                //! we are not channeling it. Targeting will be handled by conditions.
                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    DoCast(me, SPELL_TRACK_ROTATION, false);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_SHOOT)
                {
                    Vehicle* vehicle = me->GetVehicleKit();
                    Unit* southOrb = vehicle->GetPassenger(SEAT_SOUTH);
                    Unit* northOrb = vehicle->GetPassenger(SEAT_NORTH);
                    if (southOrb && northOrb)
                    {
                        if (northOrb->GetTypeId() != TYPEID_UNIT || southOrb->GetTypeId() != TYPEID_UNIT)
                            return;

                        northOrb->ToCreature()->AI()->Talk(EMOTE_WARN_LASER);
                        northOrb->CastSpell(northOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                        southOrb->CastSpell(southOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                        northOrb->CastSpell(southOrb, SPELL_TWILIGHT_CUTTER, false);
                    }

                    if (!IsHeroic())
                        return;

                    Unit* eastOrb = vehicle->GetPassenger(SEAT_EAST);
                    Unit* westOrb = vehicle->GetPassenger(SEAT_WEST);
                    if (eastOrb && westOrb)
                    {
                        if (eastOrb->GetTypeId() != TYPEID_UNIT || westOrb->GetTypeId() != TYPEID_UNIT)
                            return;

                        eastOrb->CastSpell(eastOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                        westOrb->CastSpell(westOrb, SPELL_TWILIGHT_PULSE_PERIODIC, true);
                        eastOrb->CastSpell(westOrb, SPELL_TWILIGHT_CUTTER, false);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_orb_carrierAI>(creature);
        }
};

class npc_combat_stalker : public CreatureScript
{
    public:
        npc_combat_stalker() : CreatureScript("npc_combat_stalker") { }

        struct npc_combat_stalkerAI : public Scripted_NoMovementAI
        {
            npc_combat_stalkerAI(Creature* creature) : Scripted_NoMovementAI(creature),
                   _instance(creature->GetInstanceScript())
            {
            }

            void Reset()
            {
                ScriptedAI::Reset();
                me->SetReactState(REACT_PASSIVE);
            }

            bool CanAIAttack(const Unit * target) const
            {
                if (target->GetTypeId() == TYPEID_UNIT)
                {
                    switch (target->ToCreature()->GetEntry())
                    {
                        case NPC_HALION:
                        case NPC_TWILIGHT_HALION:
                            return true;
                        default:
                            break;
                    }
                }

                return false;
            }

            void UpdateAI(uint32 const /*diff*/) { }

        private:
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_combat_stalkerAI>(creature);
        }
};

class npc_living_inferno : public CreatureScript
{
    public:
        npc_living_inferno() : CreatureScript("npc_living_inferno") { }

        struct npc_living_infernoAI : public ScriptedAI
        {
            npc_living_infernoAI(Creature* creature) : ScriptedAI(creature) { }

            void JustSummoned(Creature* /*summoner*/)
            {
                me->SetInCombatWithZone();
                DoCast(me, SPELL_BLAZING_AURA);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_living_infernoAI>(creature);
        }
};

//! Need sniff data
class npc_living_ember : public CreatureScript
{
    public:
        npc_living_ember() : CreatureScript("npc_living_ember") { }

        struct npc_living_emberAI : public ScriptedAI
        {
            npc_living_emberAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                _hasEnraged = false;
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_EMBER_ENRAGE, 20000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (!_hasEnraged && _events.ExecuteEvent() == EVENT_EMBER_ENRAGE)
                {
                    _hasEnraged = true;
                    DoCast(me, SPELL_BERSERK);
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
            bool _hasEnraged;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetRubySanctumAI<npc_living_emberAI>(creature);
        }
};

class spell_halion_meteor_strike_marker : public SpellScriptLoader
{
    public:
        spell_halion_meteor_strike_marker() : SpellScriptLoader("spell_halion_meteor_strike_marker") { }

        class spell_halion_meteor_strike_marker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_meteor_strike_marker_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                    if (Creature* creCaster = GetCaster()->ToCreature())
                        creCaster->AI()->DoAction(ACTION_METEOR_STRIKE_AOE);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_halion_meteor_strike_marker_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_meteor_strike_marker_AuraScript();
        }
};

class spell_halion_fiery_combustion : public SpellScriptLoader
{
    public:
        spell_halion_fiery_combustion() : SpellScriptLoader("spell_halion_fiery_combustion") { }

        class spell_halion_fiery_combustion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_fiery_combustion_AuraScript);

            bool Validate(SpellEntry const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MARK_OF_COMBUSTION))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->HasAura(SPELL_MARK_OF_COMBUSTION))
                    GetTarget()->RemoveAurasDueToSpell(SPELL_MARK_OF_COMBUSTION, 0, 0, AURA_REMOVE_BY_EXPIRE);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_COMBUSTION, true);
            }

            void AddMarkStack(AuraEffect const* /*aurEff*/)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_COMBUSTION, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_halion_fiery_combustion_AuraScript::AddMarkStack, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                AfterEffectApply += AuraEffectApplyFn(spell_halion_fiery_combustion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_halion_fiery_combustion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_fiery_combustion_AuraScript();
        }
};

class spell_halion_soul_consumption : public SpellScriptLoader
{
    public:
        spell_halion_soul_consumption() : SpellScriptLoader("spell_halion_soul_consumption") { }

        class spell_halion_soul_consumption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_soul_consumption_AuraScript);

            bool Validate(SpellEntry const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MARK_OF_CONSUMPTION))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->HasAura(SPELL_MARK_OF_CONSUMPTION))
                    GetTarget()->RemoveAurasDueToSpell(SPELL_MARK_OF_CONSUMPTION, 0, 0, AURA_REMOVE_BY_EXPIRE);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_CONSUMPTION, true);
            }

            void AddMarkStack(AuraEffect const* /*aurEff*/)
            {
                GetTarget()->CastSpell(GetTarget(), SPELL_MARK_OF_CONSUMPTION, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_halion_soul_consumption_AuraScript::AddMarkStack, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                AfterEffectApply += AuraEffectApplyFn(spell_halion_soul_consumption_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_halion_soul_consumption_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_soul_consumption_AuraScript();
        }
};

class spell_halion_mark_of_combustion : public SpellScriptLoader
{
    public:
        spell_halion_mark_of_combustion() : SpellScriptLoader("spell_halion_mark_of_combustion") { }

        class spell_halion_mark_of_combustion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_mark_of_combustion_AuraScript);

            bool Validate(SpellEntry const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FIERY_COMBUSTION_SUMMON))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_FIERY_COMBUSTION_EXPLOSION))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL) // Purged
                    target->RemoveAurasDueToSpell(SPELL_FIERY_COMBUSTION, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);

                //! Don't process if the aura is not considered expired.
                //! The hook will still be called upon dispelling Soul Consumption because
                //! it causes Mark of Combustion to be considered removed by expire.
                //! It will also be called upon purging the mark, but purging it will
                //! trigger Fiery Conbustion's dispel.
                //! Note: This is really fubarish, we need something simpler.
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                InstanceScript* instance = target->GetInstanceScript();
                if (!instance)
                    return;

                uint8 stacks = aurEff->GetBase()->GetStackAmount();

                // Keep track of stacks when dispelling, there's only one effect in the spell.
                CustomSpellValues values;
                values.AddSpellMod(SPELLVALUE_BASE_POINT1, stacks);

                target->CastCustomSpell(SPELL_FIERY_COMBUSTION_SUMMON, values, target, true, NULL, NULL, GetCasterGUID());
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_halion_mark_of_combustion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_mark_of_combustion_AuraScript();
        }
};

class spell_halion_mark_of_consumption : public SpellScriptLoader
{
    public:
        spell_halion_mark_of_consumption() : SpellScriptLoader("spell_halion_mark_of_consumption") { }

        class spell_halion_mark_of_consumption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_mark_of_consumption_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SOUL_CONSUMPTION_SUMMON))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_SOUL_CONSUMPTION_EXPLOSION))
                    return false;
                return true;
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_ENEMY_SPELL) // Purged
                    target->RemoveAurasDueToSpell(SPELL_SOUL_CONSUMPTION, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);

                //! Don't process if the aura is not considered expired.
                //! The hook will still be called upon dispelling Soul Consumption because
                //! it causes Mark of Consumption to be considered removed by expire.
                //! It will also be called upon purging the mark, but purging it will
                //! trigger Soul Consumption's dispel.
                //! Note: This is really fubarish, we need something simpler.
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                    return;

                InstanceScript* instance = target->GetInstanceScript();
                if (!instance)
                    return;

                uint8 stacks = aurEff->GetBase()->GetStackAmount();

                // Keep track of stacks when dispelling, there's only one effect in the spell.
                CustomSpellValues values;
                values.AddSpellMod(SPELLVALUE_BASE_POINT1, stacks);

                target->CastCustomSpell(SPELL_SOUL_CONSUMPTION_SUMMON, values, target, true, NULL, NULL, GetCasterGUID());
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_halion_mark_of_consumption_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_mark_of_consumption_AuraScript();
        }
};

class spell_halion_combustion_consumption_summon : public SpellScriptLoader
{
    public:
        spell_halion_combustion_consumption_summon() : SpellScriptLoader("spell_halion_combustion_consumption_summon") { }

        class spell_halion_combustion_consumption_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_combustion_consumption_summon_SpellScript);

            void HandleSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* caster = GetCaster();
                uint32 entry = uint32(GetSpellInfo()->Effects[effIndex].MiscValue);
                SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(uint32(GetSpellInfo()->Effects[effIndex].MiscValueB));
                uint32 duration = uint32(GetSpellInfo()->GetDuration());

                InstanceScript* instance = caster->GetInstanceScript();
                if (!instance)
                    return;

                Position pos;
                caster->GetPosition(&pos);
                if (Creature* summon = caster->GetMap()->SummonCreature(entry, pos, properties, duration, caster, GetSpellInfo()->Id))
                    if (summon->IsAIEnabled)
                        summon->AI()->SetData(DATA_STACKS_DISPELLED, GetSpellValue()->EffectBasePoints[EFFECT_1]);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_halion_combustion_consumption_summon_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halion_combustion_consumption_summon_SpellScript();
        }
};

class spell_halion_leave_twilight_realm : public SpellScriptLoader
{
    public:
        spell_halion_leave_twilight_realm() : SpellScriptLoader("spell_halion_leave_twilight_realm") { }

        class spell_halion_leave_twilight_realm_AuraScript: public AuraScript
        {
            PrepareAuraScript(spell_halion_leave_twilight_realm_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*handle*/)
            {
                GetTarget()->RemoveAurasDueToSpell(SPELL_TWILIGHT_REALM);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_halion_leave_twilight_realm_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_halion_leave_twilight_realm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_leave_twilight_realm_SpellScript);

            void HandleBeforeHit()
            {
                if (Player* player = GetHitPlayer())
                    player->RemoveAurasDueToSpell(SPELL_SOUL_CONSUMPTION, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                if (!unitList.empty())
                    unitList.remove_if(Trinity::UnitAuraCheck(false, SPELL_TWILIGHT_REALM));
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_halion_leave_twilight_realm_SpellScript::HandleBeforeHit);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_halion_leave_twilight_realm_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_ANY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halion_leave_twilight_realm_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_leave_twilight_realm_AuraScript();
        }
};

class spell_halion_enter_twilight_realm : public SpellScriptLoader
{
    public:
        spell_halion_enter_twilight_realm() : SpellScriptLoader("spell_halion_enter_twilight_realm") { }

        class spell_halion_enter_twilight_realm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_enter_twilight_realm_SpellScript);

            void HandleBeforeHit()
            {
                if (Player* player = GetHitPlayer())
                    player->RemoveAurasDueToSpell(SPELL_FIERY_COMBUSTION, 0, 0, AURA_REMOVE_BY_ENEMY_SPELL);
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_halion_enter_twilight_realm_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halion_enter_twilight_realm_SpellScript();
        }
};

class spell_halion_twilight_phasing : public SpellScriptLoader
{
    public:
        spell_halion_twilight_phasing() : SpellScriptLoader("spell_halion_twilight_phasing") { }

        class spell_halion_twilight_phasing_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_twilight_phasing_SpellScript);

            void Phase()
            {
                Unit* caster = GetCaster();
                caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                caster->CastSpell(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), SPELL_SUMMON_TWILIGHT_PORTAL, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_halion_twilight_phasing_SpellScript::Phase);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halion_twilight_phasing_SpellScript();
        }
};

class spell_halion_clear_debuffs : public SpellScriptLoader
{
    public:
        spell_halion_clear_debuffs() : SpellScriptLoader("spell_halion_clear_debuffs") { }

        class spell_halion_clear_debuffs_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_clear_debuffs_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CLEAR_DEBUFFS))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TWILIGHT_REALM))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                if (GetHitUnit()->HasAura(GetSpellInfo()->Effects[effIndex].BasePoints))
                    GetHitUnit()->RemoveAurasDueToSpell(GetSpellInfo()->Effects[effIndex].BasePoints);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_halion_clear_debuffs_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halion_clear_debuffs_SpellScript();
        }
};

class TwilightCutterSelector
{
    public:
        TwilightCutterSelector(Unit* caster, Unit* cutterCaster) : _caster(caster), _cutterCaster(cutterCaster) {}

        bool operator()(Unit* unit)
        {
            return !unit->IsInBetween(_caster, _cutterCaster, 4.0f);
        }

    private:
        Unit* _caster;
        Unit* _cutterCaster;
};

class spell_halion_twilight_cutter : public SpellScriptLoader
{
    public:
        spell_halion_twilight_cutter() : SpellScriptLoader("spell_halion_twilight_cutter") { }

        class spell_halion_twilight_cutter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_halion_twilight_cutter_SpellScript);

            void RemoveNotBetween(std::list<Unit*>& unitList)
            {
                if (unitList.empty())
                    return;

                Unit* caster = GetCaster();
                if (Aura* cutter = caster->GetAura(SPELL_TWILIGHT_CUTTER))
                {
                    if (Unit* cutterCaster = cutter->GetCaster())
                    {
                        unitList.remove_if(TwilightCutterSelector(caster, cutterCaster));
                        return;
                    }
                }

                // In case cutter caster werent found for some reason
                unitList.clear();
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_halion_twilight_cutter_SpellScript::RemoveNotBetween, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_halion_twilight_cutter_SpellScript();
        }
};

void AddSC_boss_halion()
{
    new boss_halion();
    new boss_twilight_halion();
    new npc_halion_controller();
    new npc_meteor_strike_initial();
    new npc_meteor_strike();
    new npc_combustion_consumption();
    new npc_orb_carrier();
    new npc_living_inferno();
    new npc_living_ember();
    new spell_halion_meteor_strike_marker();
    new spell_halion_combustion_consumption_summon();
    new spell_halion_mark_of_combustion();
    new spell_halion_mark_of_consumption();
    new spell_halion_fiery_combustion();
    new spell_halion_soul_consumption();
    new spell_halion_leave_twilight_realm();
    new spell_halion_enter_twilight_realm();
    new spell_halion_twilight_phasing();
    new spell_halion_twilight_cutter();
    new spell_halion_clear_debuffs();
}

/*
ServerToClient: SMSG_SPELLNONMELEEDAMAGELOG (0x0250) Length: 46 Time: 06/30/2010 22:55:05.000 Number: 354005
Target GUID: Full: 0xF130009BB700E547 Type: Unit Entry: 39863 Low: 58695 <-- Halion
Caster GUID: Full: 0xF130009CCE00E548 Type: Unit Entry: 40142 Low: 58696 <-- Twilight Halion
Spell ID: 74810 <-- Not in DBCs.
Damage: 388
Overkill: 0
SchoolMask: 8
Absorb: 0
Resist: 0
Show spellname in log: False

Also
ServerToClient: SMSG_SPELLNONMELEEDAMAGELOG (0x0250) Length: 46 Time: 06/30/2010 22:39:48.000 Number: 287998
Target GUID: Full: 0xF130009CCE00DB07 Type: Unit Entry: 40142 Low: 56071 <-- Twilight Halion
Caster GUID: Full: 0xF130009BB700DB06 Type: Unit Entry: 39863 Low: 56070 <-- Halion
Spell ID: 74810
Damage: 1232
Overkill: 0

WTF is this shit ? Schoolmask changes randomly on this spell. 141 hits on three attempts to kill Halion.
*/
