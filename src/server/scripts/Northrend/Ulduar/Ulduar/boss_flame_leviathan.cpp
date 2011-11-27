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

/*
 * Comment: there is missing code on triggers,
 *          brann bronzebeard needs correct gossip info.
 *          requires more work involving area triggers.
 *          if reached brann speaks through his radio..
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "ObjectMgr.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "ulduar.h"


enum Spells
{
    SPELL_PURSUED                  = 62374,
    SPELL_GATHERING_SPEED          = 62375,
    SPELL_BATTERING_RAM            = 62376,
    SPELL_FLAME_VENTS              = 62396,
    SPELL_MISSILE_BARRAGE          = 62400,
    SPELL_SYSTEMS_SHUTDOWN         = 62475,
    SPELL_OVERLOAD_CIRCUIT         = 62399,
    SPELL_START_THE_ENGINE         = 62472,
    SPELL_SEARING_FLAME            = 62402,
    SPELL_BLAZE                    = 62292,
    SPELL_TAR_PASSIVE              = 62288,
    SPELL_SMOKE_TRAIL              = 63575,
    SPELL_SMOKE_TRAIL_VISUAL       = 61364,
    SPELL_ELECTROSHOCK             = 62522,
    SPELL_NAPALM                   = 63666,
    SPELL_INVIS_AND_STEALTH_DETECT = 18950, // Passive
    // TOWER Additional SPELLS
    SPELL_THORIMS_HAMMER           = 62911, // Tower of Storms
    SPELL_MIMIRONS_INFERNO         = 62909, // Tower of Flames
    SPELL_HODIRS_FURY              = 62533, // Tower of Frost
    SPELL_FREYAS_WARD              = 62906, // Tower of Nature
    SPELL_FREYA_SUMMONS            = 62947, // Tower of Nature
    // TOWER ap & health spells
    SPELL_BUFF_TOWER_OF_STORMS     = 65076,
    SPELL_BUFF_TOWER_OF_FLAMES     = 65075,
    SPELL_BUFF_TOWER_OF_FR0ST      = 65077,
    SPELL_BUFF_TOWER_OF_LIFE       = 64482,
    // Additional Spells
    SPELL_FORCE_REACTION           = 57605, // temporary

    SPELL_LASH                     = 65062,
    SPELL_AUTO_REPAIR              = 62705,

    SPELL_BLUE_SKYBEAM             = 63769,
    SPELL_GREEN_SKYBEAM            = 63771,
    SPELL_RED_SKYBEAM              = 63772,
    SPELL_LIGHTNING_SKYBEAM        = 63773,

    SPELL_LIQUID_PYRITE            = 62494,
    SPELL_DUSTY_EXPLOSION          = 63360,
    SPELL_DUST_CLOUD_IMPACT        = 54740,
    SPELL_ANTI_AIR_ROCKET_DMG      = 62363,
    SPELL_GROUND_SLAM              = 62625
};

enum Creatures
{
    NPC_SEAT                       = 33114,
    NPC_DEFENSE_TURRET             = 33142,
    NPC_OVERLOAD_DEVICE            = 33143,
    NPC_MECHANOLIFT                = 33214,
    NPC_LIQUID                     = 33189,
    NPC_CONTAINER                  = 33218,
    NPC_THORIMS_HAMMER             = 33365,
    NPC_MIMIRONS_INFERNO           = 33370,
    NPC_HODIRS_FURY                = 33212,
    NPC_FREYAS_WARD                = 33367,
    NPC_THORIM_RETICLE             = 33364,
    NPC_MIMIRON_RETICLE            = 33369,
    NPC_HODIR_RETICLE              = 33108,
    NPC_FREYA_RETICLE              = 33366,
    NPC_WRITHING_LASHER            = 33387,
    NPC_WARD_OF_LIFE               = 34275,
    NPC_LOREKEEPER                 = 33686, // Hard mode starter
    NPC_BRANN_BRONZBEARD           = 33579,
    NPC_DELORAH                    = 33701,
    NPC_ULDUAR_GAUNTLET_GENERATOR  = 33571  // Trigger tied to towers
};

enum Events
{
    EVENT_PURSUE            = 1,
    EVENT_MISSILE           = 2,
    EVENT_VENT              = 3,
    EVENT_SPEED             = 4,
    EVENT_SHUTDOWN          = 5,
    EVENT_REPAIR            = 6,
    EVENT_THORIMS_HAMMER    = 7,    // Tower of Storms
    EVENT_MIMIRONS_INFERNO  = 8,    // Tower of Flames
    EVENT_HODIRS_FURY       = 9,    // Tower of Frost
    EVENT_FREYAS_WARD       = 10    // Tower of Nature
};

enum Seats
{
    SEAT_PLAYER             = 0,
    SEAT_TURRET             = 1,
    SEAT_DEVICE             = 2,
    SEAT_CANNON             = 7
};

enum Vehicles
{
    VEHICLE_SIEGE           = 33060,
    VEHICLE_CHOPPER         = 33062,
    VEHICLE_DEMOLISHER      = 33109
};

#define EMOTE_PURSUE          "Flame Leviathan pursues $N."
#define EMOTE_OVERLOAD        "Flame Leviathan's circuits overloaded."
#define EMOTE_REPAIR          "Automatic repair sequence initiated."

enum Yells
{
    SAY_AGGRO               = -1603060,
    SAY_SLAY                = -1603061,
    SAY_DEATH               = -1603062,
    SAY_TARGET_1            = -1603063,
    SAY_TARGET_2            = -1603064,
    SAY_TARGET_3            = -1603065,
    SAY_HARDMODE            = -1603066,
    SAY_TOWER_NONE          = -1603067,
    SAY_TOWER_FROST         = -1603068,
    SAY_TOWER_FLAME         = -1603069,
    SAY_TOWER_NATURE        = -1603070,
    SAY_TOWER_STORM         = -1603071,
    SAY_PLAYER_RIDING       = -1603072,
    SAY_OVERLOAD_1          = -1603073,
    SAY_OVERLOAD_2          = -1603074,
    SAY_OVERLOAD_3          = -1603075
};

enum AchievementData
{
    DATA_SHUTOUT            = 1,
    DATA_ORBIT_ACHIEVEMENTS = 2
};

enum Actions
{
    ACTION_START_ENCOUNTER           = 10,
    ACTION_OVERLOAD_CIRCUIT          = 11,
    ACTION_ACTIVATE_HARD_MODE        = 12
};

Position const Misc[]=
{
    {266.689f, -33.391f, 409.99f, 0.0f},
    {379.158f, -33.318f, 409.81f, 0.0f},
    {266.611f, -133.387f, 409.81f, 0.0f},
    {158.411f, -33.311f, 409.81f, 0.0f},
    {266.699f, 66.632f, 409.81f, 0.0f}
};

Position const Center[]=
{
    {354.8771f, -12.90240f, 409.803650f, 0.0f},
};

class boss_flame_leviathan : public CreatureScript
{
    public:
        boss_flame_leviathan() : CreatureScript("boss_flame_leviathan") { }

        struct boss_flame_leviathanAI : public BossAI
        {
            boss_flame_leviathanAI(Creature* creature) : BossAI(creature, BOSS_LEVIATHAN), vehicle(creature->GetVehicleKit())
            {
            }

            void InitializeAI()
            {
                ASSERT(vehicle);

                if (!me->isDead())
                    Reset();

                ActiveTowersCount = 4;
                Shutdown = 0;
                ActiveTowers = false;
                towerOfStorms = false;
                towerOfLife = false;
                towerOfFlames = false;
                towerOfFrost = false;
                Shutout = true;
                Unbroken = true;

                DoCast(SPELL_INVIS_AND_STEALTH_DETECT);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                me->SetReactState(REACT_PASSIVE);

                if (instance->GetData(DATA_COLOSSUS) == 2)
                {
                    DoAction(ACTION_START_ENCOUNTER);
                    if (GameObject* Gate = me->FindNearestGameObject(GO_LEVIATHAN_GATE, 50.0f))
                        Gate->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                }
            }

            Vehicle* vehicle;
            uint8 ActiveTowersCount;
            uint8 Shutdown;
            bool ActiveTowers;
            bool towerOfStorms;
            bool towerOfLife;
            bool towerOfFlames;
            bool towerOfFrost;
            bool Shutout;
            bool Unbroken;
            bool Pursued;

            void Reset()
            {
                _Reset();
                Shutdown = 0;
                Pursued = false;
                _pursueTarget = 0;
                me->SetReactState(REACT_DEFENSIVE);
                me->setActive(true);
            }

            void JustReachedHome()
            {
                _JustReachedHome();
                vehicle->InstallAllAccessories(false);
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                me->SetReactState(REACT_AGGRESSIVE);
                events.ScheduleEvent(EVENT_PURSUE, 30*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_MISSILE, urand(1500, 4*IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_VENT, 20*IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_SPEED, 15*IN_MILLISECONDS);
                ActiveTower();
                ClearSeats();

                // TODO: remove later
                me->RemoveLootMode(LOOT_MODE_DEFAULT);
            }

            // installing accessorys to creatures that have different vehicle ids in there difficulty modes seems nyi
            // so remove 2 unecessary turrets/overload devices in 10 man mode
            void ClearSeats()
            {
                if (Is25ManRaid())
                    return;

                for (uint8 i = 0; i < 2; ++i)
                    if (Unit* seat = vehicle->GetPassenger(i))
                        if (seat->ToCreature())
                            seat->ToCreature()->DespawnOrUnsummon();
            }

            void SetGUID(uint64 guid, int32 /*id*/ = 0)
            {
                if (!me->isInCombat())
                    return;

                if (Unit* passenger = ObjectAccessor::GetUnit(*me, guid))
                    for (uint8 i = RAID_MODE<uint8>(2, 0); i < 4; ++i)
                        if (Unit* seat = vehicle->GetPassenger(i))
                            if (seat->GetVehicleKit()->HasEmptySeat(SEAT_PLAYER) && !seat->GetVehicleKit()->HasEmptySeat(SEAT_TURRET))
                            {
                                passenger->EnterVehicle(seat, SEAT_PLAYER);
                                return;
                            }
            }

            void ActiveTower()
            {
                if (ActiveTowers)
                {
                    if (towerOfFrost)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_FR0ST, me);
                        events.ScheduleEvent(EVENT_HODIRS_FURY, 20*IN_MILLISECONDS);
                    }

                    if (towerOfLife)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_LIFE, me);
                        events.ScheduleEvent(EVENT_FREYAS_WARD, 30*IN_MILLISECONDS);
                    }

                    if (towerOfFlames)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_FLAMES, me);
                        events.ScheduleEvent(EVENT_MIMIRONS_INFERNO, 40*IN_MILLISECONDS);
                    }

                    if (towerOfStorms)
                    {
                        me->AddAura(SPELL_BUFF_TOWER_OF_STORMS, me);
                        events.ScheduleEvent(EVENT_THORIMS_HAMMER, 50*IN_MILLISECONDS);
                    }

                    if (!towerOfLife && !towerOfFrost && !towerOfFlames && !towerOfStorms)
                        DoScriptText(SAY_TOWER_NONE, me);
                    else
                        DoScriptText(SAY_HARDMODE, me);
                }
                else
                    DoScriptText(SAY_AGGRO, me);
            }

            void JustDied(Unit* /*victim*/)
            {
                _JustDied();
                // Set Field Flags 67108928 = 64 | 67108864 = UNIT_FLAG_UNK_6 | UNIT_FLAG_SKINNABLE
                // Set DynFlags 12
                // Set NPCFlags 0
                DoScriptText(SAY_DEATH, me);
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_PURSUED)
                {
                    DoResetThreat();
                    me->AddThreat(target, 9999999.0f);
                    me->GetMotionMaster()->MoveChase(target);

                    _pursueTarget = target->GetGUID();

                    if (Player* player = target->GetCharmerOrOwnerPlayerOrPlayerItself())
                        me->MonsterTextEmote(EMOTE_PURSUE, player->GetGUID(), true);
                }
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_START_THE_ENGINE:
                        vehicle->InstallAllAccessories(false);
                        ClearSeats();
                        break;
                    case SPELL_ELECTROSHOCK:
                        me->InterruptSpell(CURRENT_CHANNELED_SPELL);
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_SHUTOUT:
                        return Shutout ? 1 : 0;
                    case DATA_UNBROKEN:
                        return Unbroken ? 1 : 0;
                    case DATA_ORBIT_ACHIEVEMENTS:
                        if (ActiveTowers) // HardMode
                            return ActiveTowersCount;
                    default:
                        break;
                }

                return 0;
            }

            void SetData(uint32 id, uint32 data)
            {
                if (id == DATA_UNBROKEN)
                    Unbroken = data ? true : false;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                _DoAggroPulse(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if (!Pursued && me->getVictim() && !me->getVictim()->HasAura(SPELL_PURSUED) && !me->HasAura(SPELL_SYSTEMS_SHUTDOWN))
                {
                    Pursued = true;
                    events.RescheduleEvent(EVENT_PURSUE, 1000);
                }

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PURSUE:
                            DoScriptText(RAND(SAY_TARGET_1, SAY_TARGET_2, SAY_TARGET_3), me);
                            DoCast(SPELL_PURSUED);
                            Pursued = false;
                            events.ScheduleEvent(EVENT_PURSUE, 30*IN_MILLISECONDS);
                            break;
                        case EVENT_MISSILE:
                            DoCast(me, SPELL_MISSILE_BARRAGE, true);
                            events.ScheduleEvent(EVENT_MISSILE, 1500);
                            break;
                        case EVENT_VENT:
                            DoCastAOE(SPELL_FLAME_VENTS);
                            events.ScheduleEvent(EVENT_VENT, 20*IN_MILLISECONDS);
                            break;
                        case EVENT_SPEED:
                            DoCastAOE(SPELL_GATHERING_SPEED);
                            events.ScheduleEvent(EVENT_SPEED, 15*IN_MILLISECONDS);
                            break;
                        case EVENT_SHUTDOWN:
                            DoScriptText(RAND(SAY_OVERLOAD_1, SAY_OVERLOAD_2, SAY_OVERLOAD_3), me);
                            me->MonsterTextEmote(EMOTE_OVERLOAD, 0, true);
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveIdle();
                            me->CastSpell(me, SPELL_SYSTEMS_SHUTDOWN, true);
                            events.DelayEvents(20*IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_REPAIR, 4*IN_MILLISECONDS);
                            break;
                        case EVENT_REPAIR:
                            me->MonsterTextEmote(EMOTE_REPAIR, 0, true);
                            for (uint8 i = RAID_MODE<uint8>(2, 0); i < 4; ++i)
                                if (Unit* seat = vehicle->GetPassenger(i))
                                    if (seat->ToCreature())
                                        seat->ToCreature()->DespawnOrUnsummon();
                            break;
                        case EVENT_THORIMS_HAMMER:
                            for (uint8 i = 0; i < 15; ++i)
                            {
                                Position pos = Misc[0];
                                pos.m_positionX += float(irand(-100, 105));
                                pos.m_positionY += float(irand(-95, 95));
                                DoSummon(NPC_THORIM_RETICLE, pos, 30*IN_MILLISECONDS, TEMPSUMMON_TIMED_DESPAWN);
                            }
                            DoScriptText(SAY_TOWER_STORM, me);
                            break;
                        case EVENT_MIMIRONS_INFERNO:
                            me->SummonCreature(NPC_MIMIRON_RETICLE, Misc[1]);
                            DoScriptText(SAY_TOWER_FLAME, me);
                            break;
                        case EVENT_HODIRS_FURY:
                            for (uint8 i = 0; i < 2; ++i)
                                DoSummon(NPC_HODIR_RETICLE, me, 50.0f, 0);
                            DoScriptText(SAY_TOWER_FROST, me);
                            break;
                        case EVENT_FREYAS_WARD:
                            DoScriptText(SAY_TOWER_NATURE, me);
                            StartFreyaEvent();
                            break;
                    }
                }

                DoBatteringRamIfReady();
                EnterEvadeIfOutOfCombatArea(diff);
            }

            void StartFreyaEvent()
            {
                me->SummonCreature(NPC_FREYA_RETICLE, 377.02f, -119.10f, 409.81f);
                me->SummonCreature(NPC_FREYA_RETICLE, 377.02f, 54.78f, 409.81f);
                me->SummonCreature(NPC_FREYA_RETICLE, 185.62f, 54.78f, 409.81f);
                me->SummonCreature(NPC_FREYA_RETICLE, 185.62f, -119.10f, 409.81f);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_OVERLOAD_CIRCUIT)
                {
                    ++Shutdown;

                    if (Shutdown == RAID_MODE<uint8>(2, 4))
                    {
                        Shutdown = 0;
                        Shutout = false;

                        events.ScheduleEvent(EVENT_SHUTDOWN, 0);
                        me->InterruptNonMeleeSpells(true);

                        // force all players to exit
                        for (uint8 i = RAID_MODE<uint8>(2, 0); i < 4; ++i)
                            if (Unit* seat = vehicle->GetPassenger(i))
                                if (seat->GetVehicleKit())
                                    if (Unit* passenger = seat->GetVehicleKit()->GetPassenger(SEAT_PLAYER))
                                        passenger->ExitVehicle();
                    }
                }

                if (action == ACTION_START_ENCOUNTER)
                {
                    if (!me->isDead())
                    {
                        me->SetHomePosition(Center->GetPositionX(), Center->GetPositionY(), Center->GetPositionZ(), 0);
                        me->GetMotionMaster()->MoveCharge(Center->GetPositionX(), Center->GetPositionY(), Center->GetPositionZ());
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
                        return;
                    }
                }

                if (action && action <= 4) // Tower destruction
                    --ActiveTowersCount;

                switch (action)
                {
                    case ACTION_ACTIVATE_HARD_MODE:
                        ActiveTowers = true;
                        towerOfStorms = true;
                        towerOfLife = true;
                        towerOfFlames = true;
                        towerOfFrost = true;
                        break;
                    case ACTION_TOWER_OF_STORM_DESTROYED:
                        towerOfStorms = false;
                        break;
                    case ACTION_TOWER_OF_FROST_DESTROYED:
                        towerOfFrost = false;
                        break;
                    case ACTION_TOWER_OF_FLAMES_DESTROYED:
                        towerOfFlames = false;
                        break;
                    case ACTION_TOWER_OF_LIFE_DESTROYED:
                        towerOfLife = false;
                        break;
                    default:
                        break;
                }
            }

            private:

                void DoBatteringRamIfReady()
                {
                    if (me->isAttackReady())
                    {
                        Unit* target = ObjectAccessor::GetUnit(*me, _pursueTarget);
                        if (me->IsWithinCombatRange(target, 15.0f))
                        {
                            DoCast(target, SPELL_BATTERING_RAM);
                            me->resetAttackTimer();
                        }
                    }
                }

                uint64 _pursueTarget;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_flame_leviathanAI(creature);
        }
};

class npc_flame_leviathan_defense_cannon : public CreatureScript
{
    public:
        npc_flame_leviathan_defense_cannon() : CreatureScript("npc_flame_leviathan_defense_cannon") { }

        struct npc_flame_leviathan_defense_cannonAI : public ScriptedAI
        {
            npc_flame_leviathan_defense_cannonAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                _napalmTimer = 15000;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_napalmTimer <= diff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        if (CanNapalmAttack(target))
                            DoCast(target, SPELL_NAPALM, true);

                    _napalmTimer = 10000;
                }
                else
                    _napalmTimer -= diff;
            }

            bool CanNapalmAttack(Unit const* who) const
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                    return false;
                return true;
            }

        private:
            uint32 _napalmTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_flame_leviathan_defense_cannonAI(creature);
        }
};

class npc_flame_leviathan_seat : public CreatureScript
{
    public:
        npc_flame_leviathan_seat() : CreatureScript("npc_flame_leviathan_seat") { }

        struct npc_flame_leviathan_seatAI : public ScriptedAI
        {
            npc_flame_leviathan_seatAI(Creature* creature) : ScriptedAI(creature), _vehicle(creature->GetVehicleKit())
            {
                ASSERT(_vehicle);
                me->SetReactState(REACT_PASSIVE);
                me->SetDisplayId(me->GetCreatureInfo()->Modelid1);
                me->setActive(true);
            }

            void SetImmunitys(Unit* target, bool apply)
            {
                // TODO: limit target selections instead
                target->ApplySpellImmune(0, IMMUNITY_ID, SPELL_PURSUED, apply);
                target->ApplySpellImmune(0, IMMUNITY_ID, SPELL_MISSILE_BARRAGE, apply);
                target->ApplySpellImmune(0, IMMUNITY_ID, SPELL_BATTERING_RAM, apply);
                target->ApplySpellImmune(0, IMMUNITY_ID, SPELL_LASH, apply);
                target->ApplySpellImmune(0, IMMUNITY_ID, 62912, apply); // Thorims Hammer
                target->ApplySpellImmune(0, IMMUNITY_ID, 62910, apply); // Mimirons Inferno
                target->ApplySpellImmune(0, IMMUNITY_ID, 62297, apply); // Hodirs Fury
            }

            void PassengerBoarded(Unit* who, int8 seatId, bool apply)
            {
                //if (!me->GetVehicle())
                //    return;

                if (seatId == SEAT_PLAYER)
                {
                    SetImmunitys(who, apply);

                    if (apply)
                    {
                        if (Unit* turret = _vehicle->GetPassenger(SEAT_TURRET))
                            turret->SetUInt32Value(UNIT_FIELD_FLAGS, 0);

                        me->AddAura(SPELL_FORCE_REACTION, who);
                        DoScriptText(SAY_PLAYER_RIDING, me);
                    }
                    else // exit player
                    {
                        who->RemoveAurasDueToSpell(SPELL_FORCE_REACTION);
                        who->CastSpell(who, SPELL_SMOKE_TRAIL, true);
                        who->GetMotionMaster()->MoveJump(me->GetPositionX() + 20.0f, me->GetPositionY(), me->GetPositionZ() + 30.0f, 5.0f, 5.0f);
                    }
                }
            }

        private:
            Vehicle* _vehicle;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_flame_leviathan_seatAI(creature);
        }
};

class npc_flame_leviathan_defense_turret : public CreatureScript
{
    public:
        npc_flame_leviathan_defense_turret() : CreatureScript("npc_flame_leviathan_defense_turret") { }

        struct npc_flame_leviathan_defense_turretAI : public TurretAI
        {
            npc_flame_leviathan_defense_turretAI(Creature* creature) : TurretAI(creature)
            {
                me->setActive(true);
            }

            void DamageTaken(Unit* who, uint32 &damage)
            {
                if (!CanTakeDamageFrom(who))
                    damage = 0;

                if (damage >= me->GetHealth())
                    if (Vehicle* seat = me->GetVehicle())
                        if (Unit* device = seat->GetPassenger(SEAT_DEVICE))
                            device->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            bool CanTakeDamageFrom(Unit const* who) const
            {
                if (who->GetTypeId() != TYPEID_PLAYER || !who->GetVehicle() || who->GetVehicleBase()->GetEntry() != NPC_SEAT)
                    return false;

                return true;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_flame_leviathan_defense_turretAI(creature);
        }
};

class npc_flame_leviathan_overload_device : public CreatureScript
{
    public:
        npc_flame_leviathan_overload_device() : CreatureScript("npc_flame_leviathan_overload_device") { }

        struct npc_flame_leviathan_overload_deviceAI : public PassiveAI
        {
            npc_flame_leviathan_overload_deviceAI(Creature* creature) : PassiveAI(creature)
            {
                me->SetDisplayId(me->GetCreatureInfo()->Modelid3);
                _instance = creature->GetInstanceScript();
                me->setActive(true);
            }

            void DoAction(int32 const param)
            {
                if (param == EVENT_SPELLCLICK)
                {
                    if (me->GetVehicle() && _instance)
                    {
                        if (Creature* leviathan = ObjectAccessor::GetCreature(*me, _instance->GetData64(BOSS_LEVIATHAN)))
                            leviathan->AI()->DoAction(ACTION_OVERLOAD_CIRCUIT);

                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    }
                }
            }

        private:
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_flame_leviathan_overload_deviceAI(creature);
        }
};

class npc_mechanolift : public CreatureScript
{
    public:
        npc_mechanolift() : CreatureScript("npc_mechanolift") { }

        struct npc_mechanoliftAI : public PassiveAI
        {
            npc_mechanoliftAI(Creature* creature) : PassiveAI(creature)
            {
            }

            void Reset()
            {
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                me->SetVisible(true);
            }

            void DamageTaken(Unit* /*who*/, uint32 &damage)
            {
                if (damage >= me->GetHealth())
                {
                    if (Creature* liquid = DoSummon(NPC_LIQUID, me, 0, 190*IN_MILLISECONDS, TEMPSUMMON_TIMED_DESPAWN))
                    {
                        float x, y, z;
                        me->GetPosition(x, y, z);
                        z = me->GetMap()->GetHeight(x, y, MAX_HEIGHT);

                        liquid->SetFlying(true);
                        liquid->GetMotionMaster()->MovePoint(0, x, y, z);
                    }

                    me->SetVisible(false);
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mechanoliftAI(creature);
        }
};

class npc_liquid_pyrite : public CreatureScript
{
    public:
        npc_liquid_pyrite() : CreatureScript("npc_liquid_pyrite") { }

        struct npc_liquid_pyriteAI : public PassiveAI
        {
            npc_liquid_pyriteAI(Creature* creature) : PassiveAI(creature) { }

            void Reset()
            {
                DoCast(me, SPELL_LIQUID_PYRITE, true);
                me->SetDisplayId(28476);
                _despawnTimer = 5*IN_MILLISECONDS;
            }

            void MovementInform(uint32 type, uint32 /*id*/)
            {
                if (type == POINT_MOTION_TYPE)
                {
                    DoCast(me, SPELL_DUSTY_EXPLOSION, true);
                    DoCast(me, SPELL_DUST_CLOUD_IMPACT, true);
                    me->SetDisplayId(28783);
                }
            }

            void DamageTaken(Unit* /*who*/, uint32& damage)
            {
                damage = 0;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_despawnTimer <= diff)
                {
                    if (me->GetVehicle())
                        me->DisappearAndDie();
                    _despawnTimer = 5*IN_MILLISECONDS;
                }
                else
                    _despawnTimer -= diff;
            }

        private:
            uint32 _despawnTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_liquid_pyriteAI(creature);
        }
};

class npc_pool_of_tar : public CreatureScript
{
    public:
        npc_pool_of_tar() : CreatureScript("npc_pool_of_tar") { }

        struct npc_pool_of_tarAI : public PassiveAI
        {
            npc_pool_of_tarAI(Creature* creature) : PassiveAI(creature)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
                DoCast(me, SPELL_TAR_PASSIVE, true);
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
                me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            }

            void DamageTaken(Unit* /*who*/, uint32& damage)
            {
                damage = 0;
            }

            void SpellHit(Unit* /*caster*/, SpellInfo const* spell)
            {
                if ((spell->Id == 65044 || spell->Id == 65045) && !me->HasAura(SPELL_BLAZE))
                    DoCast(me, SPELL_BLAZE, true);
            }

            void UpdateAI(uint32 const /*diff*/) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_pool_of_tarAI(creature);
        }
};

class npc_colossus : public CreatureScript
{
    public:
        npc_colossus() : CreatureScript("npc_colossus")
        {
        }

        struct npc_colossusAI : public ScriptedAI
        {
            npc_colossusAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                _groundSlamTimer = urand(8, 10) *IN_MILLISECONDS;
            }

            void JustDied(Unit* /*who*/)
            {
                if (me->GetHomePosition().IsInDist(Center, 50.f))
                    _instance->SetData(DATA_COLOSSUS, _instance->GetData(DATA_COLOSSUS) + 1);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_groundSlamTimer <= diff)
                {
                    DoCastVictim(SPELL_GROUND_SLAM);
                    _groundSlamTimer = urand(20, 25) *IN_MILLISECONDS;
                }
                else
                    _groundSlamTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            uint32 _groundSlamTimer;
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_colossusAI(creature);
        }
};

class npc_thorims_hammer : public CreatureScript
{
    public:
        npc_thorims_hammer() : CreatureScript("npc_thorims_hammer") { }

        struct npc_thorims_hammerAI : public Scripted_NoMovementAI
        {
            npc_thorims_hammerAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetDisplayId(me->GetCreatureInfo()->Modelid2);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                _timer = urand(2, 10) *IN_MILLISECONDS;
                _action = 1;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_timer <= diff)
                {
                    switch (_action)
                    {
                        case 1:
                            me->AddAura(SPELL_LIGHTNING_SKYBEAM, me);
                            ++_action;
                            _timer = 4*IN_MILLISECONDS;
                            break;
                        case 2:
                            if (Creature* trigger = DoSummonFlyer(NPC_THORIMS_HAMMER, me, 50.0f, 0, 10*IN_MILLISECONDS, TEMPSUMMON_TIMED_DESPAWN))
                            {
                                trigger->SetDisplayId(trigger->GetCreatureInfo()->Modelid2);
                                trigger->CastSpell(me, SPELL_THORIMS_HAMMER, true);
                            }
                            ++_action;
                            _timer = 4*IN_MILLISECONDS;
                            break;
                        case 3:
                            me->RemoveAllAuras();
                            _timer = 30*IN_MILLISECONDS;
                            break;
                    }
                }
                else
                    _timer -= diff;
            }

        private:
            uint32 _action;
            uint32 _timer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_thorims_hammerAI(creature);
        }
};

class npc_mimirons_inferno : public CreatureScript
{
    public:
        npc_mimirons_inferno() : CreatureScript("npc_mimirons_inferno") { }

        struct npc_mimirons_infernoAI : public ScriptedAI
        {
            npc_mimirons_infernoAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetDisplayId(me->GetCreatureInfo()->Modelid2);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_RED_SKYBEAM, me);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                _count = 2;
                _infernoTimer = 2000;
                _pointReached = true;
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || id != _count)
                    return;

                    if (++_count > 4)
                        _count = 1;

                    _pointReached = true;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_pointReached)
                {
                    _pointReached = false;
                    me->GetMotionMaster()->MovePoint(_count, Misc[_count]);
                }

                if (_infernoTimer <= diff)
                {
                    if (Creature* trigger = DoSummonFlyer(NPC_MIMIRONS_INFERNO, me, 50.0f, 0, 40*IN_MILLISECONDS, TEMPSUMMON_TIMED_DESPAWN))
                    {
                        trigger->SetDisplayId(trigger->GetCreatureInfo()->Modelid2);
                        trigger->CastSpell(me, SPELL_MIMIRONS_INFERNO, true);
                        _infernoTimer = 2*IN_MILLISECONDS;
                    }
                }
                else
                    _infernoTimer -= diff;
            }

        private:
            uint32 _infernoTimer;
            uint8 _count;
            bool _pointReached;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mimirons_infernoAI(creature);
        }
};

class npc_hodirs_fury : public CreatureScript
{
    public:
        npc_hodirs_fury() : CreatureScript("npc_hodirs_fury") { }

        struct npc_hodirs_furyAI : public ScriptedAI
        {
            npc_hodirs_furyAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
                me->SetDisplayId(me->GetCreatureInfo()->Modelid2);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->AddAura(SPELL_BLUE_SKYBEAM, me);
            }

            void Reset()
            {
                _targetGUID = 0;
                _action = 3;
                _timer = 3*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_timer <= diff)
                {
                    switch (_action)
                    {
                        case 1:
                        {
                            _timer = 1*IN_MILLISECONDS;

                            Unit* target = ObjectAccessor::GetUnit(*me, _targetGUID);
                            if (target && me->IsInRange(target, 0.0f, 5.0f, false))
                            {
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MoveIdle();
                                _timer = 3*IN_MILLISECONDS;
                                _targetGUID = 0;
                                ++_action;
                            }
                            else if (!target)
                                _action = 3;

                            break;
                        }
                        case 2:
                            if (Creature* trigger = DoSummonFlyer(NPC_HODIRS_FURY, me, 50.0f, 0, 10*IN_MILLISECONDS, TEMPSUMMON_TIMED_DESPAWN))
                            {
                                trigger->SetDisplayId(trigger->GetCreatureInfo()->Modelid2);
                                trigger->CastSpell(me, SPELL_HODIRS_FURY, true);
                                ++_action;
                            }
                            _timer = 7*IN_MILLISECONDS;
                            break;
                        case 3:
                            DoAttackerAreaInCombat(me, 200.0f);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true))
                            {
                                _targetGUID = target->GetGUID();
                                me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                                _action = 1;
                            }
                            _timer = 5*IN_MILLISECONDS;
                    }
                }
                else
                    _timer -= diff;
            }

        private:
            uint64 _targetGUID;
            uint32 _timer;
            uint8 _action;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_hodirs_furyAI(creature);
        }
};

class npc_freyas_ward : public CreatureScript
{
    public:
        npc_freyas_ward() : CreatureScript("npc_freyas_ward") { }

        struct npc_freyas_wardAI : public Scripted_NoMovementAI
        {
            npc_freyas_wardAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                me->setActive(true);
                me->SetDisplayId(me->GetCreatureInfo()->Modelid2);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->AddAura(SPELL_GREEN_SKYBEAM, me);
                me->SetReactState(REACT_PASSIVE);
            }

            void Reset()
            {
                _summonTimer = 10*IN_MILLISECONDS;
            }

            void UpdateAI(uint32 const diff)
            {
                if (_summonTimer <= diff)
                {
                    if (Creature* trigger = DoSummonFlyer(NPC_FREYAS_WARD, me, 50.0f, 0, 10*IN_MILLISECONDS, TEMPSUMMON_TIMED_DESPAWN))
                    {
                        trigger->SetDisplayId(trigger->GetCreatureInfo()->Modelid2);
                        trigger->CastSpell(me, SPELL_FREYAS_WARD, true);
                        _summonTimer = 30*IN_MILLISECONDS;
                    }
                }
                else
                    _summonTimer -= diff;
            }

        private:
            uint32 _summonTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_freyas_wardAI(creature);
        }
};

class npc_freyas_ward_summon : public CreatureScript
{
    public:
        npc_freyas_ward_summon() : CreatureScript("npc_freyas_ward_summon") { }

        struct npc_freyas_ward_summonAI : public ScriptedAI
        {
            npc_freyas_ward_summonAI(Creature* creature) : ScriptedAI(creature)
            {
                me->setActive(true);
            }

            void Reset()
            {
                _lashTimer = urand(2, 8) *IN_MILLISECONDS;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_lashTimer <= diff)
                {
                    DoCast(SPELL_LASH);
                    _lashTimer = urand(8, 12) *IN_MILLISECONDS;
                }
                else
                    _lashTimer -= diff;

                // no melee
            }

        private:
            uint32 _lashTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_freyas_ward_summonAI(creature);
        }
};

class npc_leviathan_player_vehicle : public CreatureScript
{
    public:
        npc_leviathan_player_vehicle() : CreatureScript("npc_leviathan_player_vehicle")
        {
        }

        struct npc_leviathan_player_vehicleAI : public NullCreatureAI
        {
            npc_leviathan_player_vehicleAI(Creature* creature) : NullCreatureAI(creature)
            {
                _instance = creature->GetInstanceScript();

                if (VehicleSeatEntry* vehSeat = const_cast<VehicleSeatEntry*>(sVehicleSeatStore.LookupEntry(3013)))
                    vehSeat->m_flags &= ~VEHICLE_SEAT_FLAG_UNK11;
            }

            void PassengerBoarded(Unit* unit, int8 seat, bool apply)
            {
                if (!unit->ToPlayer() || seat != 0)
                    return;

                if (Creature* leviathan = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(BOSS_LEVIATHAN) : 0))
                {
                    if (leviathan->isInCombat())
                    {
                        me->SetInCombatWith(leviathan);
                        me->AddThreat(leviathan, 1.0f);
                        leviathan->SetInCombatWith(me);
                        leviathan->AddThreat(me, 1.0f);

                        if (apply)
                            me->SetHealth(uint32(me->GetHealth() / 2));
                    }
                }
            }

        private:
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_leviathan_player_vehicleAI(creature);
        }
};

// npc lore keeper
#define GOSSIP_ITEM_1  "Activate secondary defensive systems"
#define GOSSIP_ITEM_2  "Confirmed"

class npc_lorekeeper : public CreatureScript
{
    public:
        npc_lorekeeper() : CreatureScript("npc_lorekeeper") { }

        struct npc_lorekeeperAI : public ScriptedAI
        {
            npc_lorekeeperAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            /*
            void DoAction(int32 const action)
            {
                // Start encounter
                if (action == 0)
                {
                    for (int32 i = 0; i < RAID_MODE(2, 5); ++i)
                        DoSummon(VEHICLE_SIEGE, PosSiege[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                    for (int32 i = 0; i < RAID_MODE(2, 5); ++i)
                        DoSummon(VEHICLE_CHOPPER, PosChopper[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                    for (int32 i = 0; i < RAID_MODE(2, 5); ++i)
                        DoSummon(VEHICLE_DEMOLISHER, PosDemolisher[i], 3000, TEMPSUMMON_CORPSE_TIMED_DESPAWN);
                    return;
                }
            }*/
        };

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            InstanceScript* instance = creature->GetInstanceScript();
            if (!instance)
                return true;

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    if (player)
                    {
                        player->PrepareGossipMenu(creature);
                        instance->instance->LoadGrid(364, -16); // make sure leviathan is loaded

                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    if (player)
                        player->CLOSE_GOSSIP_MENU();

                    if (Creature* leviathan = instance->instance->GetCreature(instance->GetData64(BOSS_LEVIATHAN)))
                    {
                        leviathan->AI()->DoAction(ACTION_ACTIVATE_HARD_MODE);
                        creature->SetVisible(false);

                        if (Creature* Delorah = creature->FindNearestCreature(NPC_DELORAH, 200.0f, true))
                        {
                            if (Creature* Brann = creature->FindNearestCreature(NPC_BRANN_BRONZBEARD, 200.0f, true))
                            {
                                Delorah->GetMotionMaster()->MovePoint(0, Brann->GetPositionX() - 4, Brann->GetPositionY(), Brann->GetPositionZ());
                                // TODO DoScriptText(xxxx, Delorah, Brann); when reached brann
                            }
                        }
                    }
                    break;
            }

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            InstanceScript* instance = creature->GetInstanceScript();
            if (instance && instance->GetData(BOSS_LEVIATHAN) != DONE && player)
            {
                player->PrepareGossipMenu(creature);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_lorekeeperAI(creature);
        }
};

////npc_brann_bronzebeard this requires more work involving area triggers. if reached this guy speaks through his radio..
//#define GOSSIP_ITEM_1  "xxxxx"
//#define GOSSIP_ITEM_2  "xxxxx"
//
/*
class npc_brann_bronzebeard : public CreatureScript
{
public:
    npc_brann_bronzebeard() : CreatureScript("npc_brann_bronzebeard") { }

    //bool OnGossipSelect(Player* pPlayer, Creature* creature, uint32 uiSender, uint32 uiAction)
    //{
    //    pPlayer->PlayerTalkClass->ClearMenus();
    //    switch(uiAction)
    //    {
    //        case GOSSIP_ACTION_INFO_DEF+1:
    //            if (pPlayer)
    //            {
    //                pPlayer->PrepareGossipMenu(creature);
    //
    //                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
    //                pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(creature), creature->GetGUID());
    //            }
    //            break;
    //        case GOSSIP_ACTION_INFO_DEF+2:
    //            if (pPlayer)
    //                pPlayer->CLOSE_GOSSIP_MENU();
    //            if (Creature* Lorekeeper = creature->FindNearestCreature(NPC_LOREKEEPER, 1000, true)) //lore keeper of lorgannon
    //                Lorekeeper->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    //            break;
    //    }
    //    return true;
    //}
    //bool OnGossipHello(Player* pPlayer, Creature* creature)
    //{
    //    InstanceScript* instance = creature->GetInstanceScript();
    //    if (instance && instance->GetData(TYPE_LEVIATHAN) !=DONE)
    //    {
    //        pPlayer->PrepareGossipMenu(creature);
    //
    //        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    //        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(creature), creature->GetGUID());
    //    }
    //    return true;
    //}
    //
}
*/

class at_RX_214_repair_o_matic_station : public AreaTriggerScript
{
    public:
        at_RX_214_repair_o_matic_station() : AreaTriggerScript("at_RX_214_repair_o_matic_station") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*at*/)
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (Creature* vehicle = player->GetVehicleCreatureBase())
            {
                if (!vehicle->HasAura(SPELL_AUTO_REPAIR))
                {
                    player->MonsterTextEmote(EMOTE_REPAIR, player->GetGUID(), true);
                    player->CastSpell(vehicle, SPELL_AUTO_REPAIR, true);
                    vehicle->SetFullHealth();
                    if (Creature* leviathan = ObjectAccessor::GetCreature(*player, instance ? instance->GetData64(BOSS_LEVIATHAN) : 0))
                        leviathan->AI()->SetData(DATA_UNBROKEN, 0); // set bool to false thats checked in leviathan getdata
                }
            }
            return true;
        }
};

class achievement_three_car_garage_demolisher : public AchievementCriteriaScript
{
    public:
        achievement_three_car_garage_demolisher() : AchievementCriteriaScript("achievement_three_car_garage_demolisher") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Creature* vehicle = source->GetVehicleCreatureBase())
            {
                if (vehicle->GetEntry() == VEHICLE_DEMOLISHER)
                    return true;
            }

            return false;
        }
};

class achievement_three_car_garage_chopper : public AchievementCriteriaScript
{
    public:
        achievement_three_car_garage_chopper() : AchievementCriteriaScript("achievement_three_car_garage_chopper") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Creature* vehicle = source->GetVehicleCreatureBase())
            {
                if (vehicle->GetEntry() == VEHICLE_CHOPPER)
                    return true;
            }

            return false;
        }
};

class achievement_three_car_garage_siege : public AchievementCriteriaScript
{
    public:
        achievement_three_car_garage_siege() : AchievementCriteriaScript("achievement_three_car_garage_siege") { }

        bool OnCheck(Player* source, Unit* /*target*/)
        {
            if (Creature* vehicle = source->GetVehicleCreatureBase())
            {
                if (vehicle->GetEntry() == VEHICLE_SIEGE)
                    return true;
            }

            return false;
        }
};

class achievement_shutout : public AchievementCriteriaScript
{
    public:
        achievement_shutout() : AchievementCriteriaScript("achievement_shutout") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target)
                if (Creature* leviathan = target->ToCreature())
                    if (leviathan->AI()->GetData(DATA_SHUTOUT))
                        return true;

            return false;
        }
};

class achievement_unbroken : public AchievementCriteriaScript
{
    public:
        achievement_unbroken() : AchievementCriteriaScript("achievement_unbroken") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (target)
                if (Creature* leviathan = target->ToCreature())
                    if (leviathan->AI()->GetData(DATA_UNBROKEN))
                        return true;

            return false;
        }
};

class achievement_orbital_bombardment : public AchievementCriteriaScript
{
    public:
        achievement_orbital_bombardment() : AchievementCriteriaScript("achievement_orbital_bombardment") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Leviathan = target->ToCreature())
                if (Leviathan->AI()->GetData(DATA_ORBIT_ACHIEVEMENTS) >= 1)
                    return true;

            return false;
        }
};

class achievement_orbital_devastation : public AchievementCriteriaScript
{
    public:
        achievement_orbital_devastation() : AchievementCriteriaScript("achievement_orbital_devastation") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Leviathan = target->ToCreature())
                if (Leviathan->AI()->GetData(DATA_ORBIT_ACHIEVEMENTS) >= 2)
                    return true;

            return false;
        }
};

class achievement_nuked_from_orbit : public AchievementCriteriaScript
{
    public:
        achievement_nuked_from_orbit() : AchievementCriteriaScript("achievement_nuked_from_orbit") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Leviathan = target->ToCreature())
                if (Leviathan->AI()->GetData(DATA_ORBIT_ACHIEVEMENTS) >= 3)
                    return true;

            return false;
        }
};

class achievement_orbit_uary : public AchievementCriteriaScript
{
    public:
        achievement_orbit_uary() : AchievementCriteriaScript("achievement_orbit_uary") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* Leviathan = target->ToCreature())
                if (Leviathan->AI()->GetData(DATA_ORBIT_ACHIEVEMENTS) == 4)
                    return true;

            return false;
        }
};

// strange workaround until traj target selection works
class spell_anti_air_rocket : public SpellScriptLoader
{
    public:
        spell_anti_air_rocket() : SpellScriptLoader("spell_anti_air_rocket") { }

        class spell_anti_air_rocket_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_anti_air_rocket_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ANTI_AIR_ROCKET_DMG))
                    return false;
                return true;
            }

            void HandleTriggerMissile(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (const WorldLocation* pos = GetTargetDest())
                {
                    if (Creature* temp = GetCaster()->SummonCreature(22515, *pos, TEMPSUMMON_TIMED_DESPAWN, 500))
                    {
                        temp->SetReactState(REACT_PASSIVE);
                        temp->SetFlying(true);
                        temp->SetVisible(false);
                        std::list<Creature*> list;
                        GetCreatureListWithEntryInGrid(list, GetCaster(), NPC_MECHANOLIFT, 100.0f);

                        while (!list.empty())
                        {
                            std::list<Creature*>::iterator itr = list.begin();
                            std::advance(itr, urand(0, list.size()-1));

                            if ((*itr)->IsInBetween(GetCaster(), temp, 10.0f))
                            {
                                GetCaster()->CastSpell((*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), SPELL_ANTI_AIR_ROCKET_DMG, true);
                                return;
                            }
                            list.erase(itr);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_anti_air_rocket_SpellScript::HandleTriggerMissile, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_anti_air_rocket_SpellScript();
        }
};

class spell_pursued : public SpellScriptLoader
{
    public:
        spell_pursued() : SpellScriptLoader("spell_pursued") { }

        class spell_pursued_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pursued_SpellScript);

            bool Load()
            {
                _target = NULL;
                return GetCaster()->GetTypeId() == TYPEID_UNIT;
            }

            void SelectTarget(std::list<Unit*>& unitList)
            {
                if (unitList.empty())
                    return;

                std::list<Unit*> tempList;

                // try to find demolisher or siege engine first (not the current target)
                for (std::list<Unit*>::iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                {
                    _target = *itr;

                    if (!_target->ToCreature() || _target->HasAura(SPELL_PURSUED))
                        continue;

                    if (_target->ToCreature()->GetEntry() == VEHICLE_SIEGE || _target->ToCreature()->GetEntry() == VEHICLE_DEMOLISHER)
                        tempList.push_back(_target);
                }

                if (tempList.empty())
                {
                    // no demolisher or siege engine, find a chopper (not the current target)
                    for (std::list<Unit*>::iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                    {
                        _target = *itr;

                        if (!_target->ToCreature() || _target->HasAura(SPELL_PURSUED))
                            continue;

                        if (_target->ToCreature()->GetEntry() == VEHICLE_CHOPPER)
                            tempList.push_back(_target);
                    }
                }

                if (!tempList.empty())
                {
                    // found one or more vehicles, select a random one
                    _target = SelectRandomContainerElement(tempList);
                    unitList.clear();
                    unitList.push_back(_target);
                }
                else
                {
                    // found no vehicles, select a random player or pet
                    _target = SelectRandomContainerElement(unitList);
                    unitList.clear();
                    unitList.push_back(_target);
                }
            }

            void SetTarget(std::list<Unit*>& unitList)
            {
                unitList.clear();

                if (_target)
                    unitList.push_back(_target);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_pursued_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnUnitTargetSelect += SpellUnitTargetFn(spell_pursued_SpellScript::SetTarget, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }

            Unit* _target;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pursued_SpellScript();
        }
};

class spell_flame_leviathan_flames : public SpellScriptLoader
{
    public:
        spell_flame_leviathan_flames() : SpellScriptLoader("spell_flame_leviathan_flames") { }

        class spell_flame_leviathan_flames_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_flame_leviathan_flames_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit()->HasAura(62297))
                    GetHitUnit()->RemoveAura(62297);
            }

            void SetDamage()
            {
                PreventHitDamage();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_flame_leviathan_flames_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnHit += SpellHitFn(spell_flame_leviathan_flames_SpellScript::SetDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_flame_leviathan_flames_SpellScript();
        }
};

class spell_load_into_catapult : public SpellScriptLoader
{
    enum Spells
    {
        SPELL_PASSENGER_LOADED = 62340,
    };

    public:
        spell_load_into_catapult() : SpellScriptLoader("spell_load_into_catapult") { }

        class spell_load_into_catapult_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_load_into_catapult_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* owner = GetOwner()->ToUnit();
                if (!owner)
                    return;

                owner->CastSpell(owner, SPELL_PASSENGER_LOADED, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* owner = GetOwner()->ToUnit();
                if (!owner)
                    return;

                owner->RemoveAurasDueToSpell(SPELL_PASSENGER_LOADED);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_load_into_catapult_AuraScript::OnApply, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_load_into_catapult_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_load_into_catapult_AuraScript();
        }
};

class spell_systems_shutdown : public SpellScriptLoader
{
    public:
        spell_systems_shutdown() : SpellScriptLoader("spell_systems_shutdown") { }

        class spell_systems_shutdown_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_systems_shutdown_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Creature* owner = GetOwner()->ToCreature();
                if (!owner)
                    return;

                //! This could probably in the SPELL_EFFECT_SEND_EVENT handler too:
                owner->AddUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                owner->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                owner->RemoveAurasDueToSpell(SPELL_GATHERING_SPEED);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Creature* owner = GetOwner()->ToCreature();
                if (!owner)
                    return;

                owner->ClearUnitState(UNIT_STAT_STUNNED | UNIT_STAT_ROOT);
                owner->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_systems_shutdown_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_systems_shutdown_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_systems_shutdown_AuraScript();
        }
};

class spell_throw_passenger : public SpellScriptLoader
{
    public:
        spell_throw_passenger() : SpellScriptLoader("spell_throw_passenger") { }

        class spell_throw_passenger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_throw_passenger_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();

                if (caster && caster->GetVehicleKit())
                    if (Unit* passenger = caster->GetVehicleKit()->GetPassenger(3))
                    {
                        passenger->ExitVehicle();
                        if (Creature* leviathan = caster->FindNearestCreature(33113, 120.0f))
                            leviathan->AI()->SetGUID(passenger->GetGUID());
                    }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_throw_passenger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_throw_passenger_SpellScript();
        }
};

class spell_freyas_ward_summon : public SpellScriptLoader
{
    public:
        spell_freyas_ward_summon() : SpellScriptLoader("spell_freyas_ward_summon") { }

        class spell_freyas_ward_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_freyas_ward_summon_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (Unit* caster = GetCaster())
                    if (InstanceScript* instance = caster->GetInstanceScript())
                        if (Creature* leviathan = ObjectAccessor::GetCreature(*caster, instance->GetData64(BOSS_LEVIATHAN)))
                            for (uint8 i = 0; i < urand(3, 5); ++i)
                                leviathan->SummonCreature(NPC_WRITHING_LASHER, GetTargetDest()->GetPositionX(), GetTargetDest()->GetPositionY(),
                                GetTargetDest()->GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 3000);
            }

            void HandleSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (Unit* caster = GetCaster())
                    if (InstanceScript* instance = caster->GetInstanceScript())
                        if (Creature* leviathan = ObjectAccessor::GetCreature(*caster, instance->GetData64(BOSS_LEVIATHAN)))
                            leviathan->SummonCreature(NPC_WARD_OF_LIFE, GetTargetDest()->GetPositionX(), GetTargetDest()->GetPositionY(),
                            GetTargetDest()->GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 3000);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_freyas_ward_summon_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
                OnEffectHit += SpellEffectFn(spell_freyas_ward_summon_SpellScript::HandleSummon, EFFECT_2, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_freyas_ward_summon_SpellScript();
        }
};

class FlameVentsTargetSelector
{
    public:
        bool operator() (Unit* unit)
        {
            if (unit->GetTypeId() != TYPEID_PLAYER)
            {
                if (unit->ToCreature()->GetEntry() == VEHICLE_SIEGE ||
                    unit->ToCreature()->GetEntry() == VEHICLE_CHOPPER ||
                    unit->ToCreature()->GetEntry() == VEHICLE_DEMOLISHER)
                    return false;

                if (!unit->ToCreature()->isPet())
                    return true;
            }

            // TODO: more check?
            return unit->GetVehicle();
        }
};

class spell_flame_leviathan_flame_vents : public SpellScriptLoader
{
    public:
        spell_flame_leviathan_flame_vents() : SpellScriptLoader("spell_flame_leviathan_flame_vents") { }

        class spell_flame_leviathan_flame_vents_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_flame_leviathan_flame_vents_SpellScript);

            void FilterTargets(std::list<Unit*>& unitList)
            {
                unitList.remove_if(FlameVentsTargetSelector());
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_flame_leviathan_flame_vents_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_flame_leviathan_flame_vents_SpellScript();
        }
};

class spell_shield_generator : public SpellScriptLoader
{
    public:
        spell_shield_generator() : SpellScriptLoader("spell_shield_generator") { }

        class spell_shield_generator_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_shield_generator_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* siege = caster->GetVehicleBase())
                        amount = int32(siege->CountPctFromMaxHealth(15));
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_shield_generator_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_shield_generator_AuraScript();
        }
};

void AddSC_boss_flame_leviathan()
{
    new boss_flame_leviathan();
    new npc_flame_leviathan_seat();
    new npc_flame_leviathan_defense_turret();
    new npc_flame_leviathan_defense_cannon();
    new npc_flame_leviathan_overload_device();
    new npc_mechanolift();
    new npc_liquid_pyrite();
    new npc_pool_of_tar();
    new npc_colossus();
    new npc_thorims_hammer();
    new npc_mimirons_inferno();
    new npc_hodirs_fury();
    new npc_freyas_ward();
    new npc_freyas_ward_summon();
    new npc_leviathan_player_vehicle();
    new npc_lorekeeper();

    // new npc_brann_bronzebeard();
    new at_RX_214_repair_o_matic_station();

    new achievement_three_car_garage_demolisher();
    new achievement_three_car_garage_chopper();
    new achievement_three_car_garage_siege();
    new achievement_shutout();
    new achievement_unbroken();
    new achievement_orbital_bombardment();
    new achievement_orbital_devastation();
    new achievement_nuked_from_orbit();
    new achievement_orbit_uary();

    new spell_anti_air_rocket();
    new spell_pursued();
    new spell_flame_leviathan_flames();
    new spell_load_into_catapult();
    new spell_systems_shutdown();
    new spell_throw_passenger();
    new spell_freyas_ward_summon();
    new spell_flame_leviathan_flame_vents();
    new spell_shield_generator();
}
