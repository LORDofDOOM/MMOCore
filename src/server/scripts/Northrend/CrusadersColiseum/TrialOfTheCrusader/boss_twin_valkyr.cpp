/*
 * Copyright (C) 2008 - 2010 Trinity <http://www.trinitycore.org/>
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
#include "trial_of_the_crusader.h"

enum Spells
{
    DARK_ESSENCE        = 65684,
    LIGHT_ESSENCE       = 65686,
    EMPROVED_DARKNESS   = 67215,
    EMPROVED_LIGHT      = 67218,
    UNLEASHED_DARK      = 67172,
    UNLEASHED_LIGHT     = 65795,
    POWERING_UP         = 67604,


    //DARK
    DARK_VORTEX_10N     = 66058,
    DARK_VORTEX_10H     = 67183,
    DARK_VORTEX_25N     = 67182,
    DARK_VORTEX_25H     = 67184,

    DARK_SHIELD_10N     = 65874,
    DARK_SHIELD_10H     = 67257,
    DARK_SHIELD_25N     = 67256,
    DARK_SHIELD_25H     = 67258,

    SURGE_OF_DARKNESS_10N   = 65768,
    SURGE_OF_DARKNESS_10H   = 67263,
    SURGE_OF_DARKNESS_25N   = 67262,
    SURGE_OF_DARKNESS_25H   = 67264,

    TOUCH_OF_DARKNESS_10    = 67282, //only heroic
    TOUCH_OF_DARKNESS_25    = 67283,


    //LIGHT
    LIGHT_VORTEX_10N        = 66046,
    LIGHT_VORTEX_10H        = 67207,
    LIGHT_VORTEX_25N        = 67206,
    LIGHT_VORTEX_25H        = 67208,

    LIGHT_SHIELD_10N        = 65858,
    LIGHT_SHIELD_10H        = 67260,
    LIGHT_SHIELD_25N        = 67259,
    LIGHT_SHIELD_25H        = 67261,

    SURGE_OF_LIGHT_10N      = 65766,
    SURGE_OF_LIGHT_10H      = 67271,
    SURGE_OF_LIGHT_25N      = 67270,
    SURGE_OF_LIGHT_25H      = 67272,

    TOUCH_OF_LIGHT_10       = 67297, //only heroic
    TOUCH_OF_LIGHT_25       = 67298,


    //SHARED
    TWINS_PACT_10N      = 65875,
    TWINS_PACT_10H      = 67304,
    TWINS_PACT_25N      = 67303,
    TWINS_PACT_25H      = 67305,

    POWER_OF_TWINS_10N  = 65879,
    POWER_OF_TWINS_10H  = 67245,
    POWER_OF_TWINS_25N  = 67244,
    POWER_OF_TWINS_25H  = 67246,

    TWIN_SPIKE_10N      = 66069,
    TWIN_SPIKE_10H      = 67310,
    TWIN_SPIKE_25N      = 67309,
    TWIN_SPIKE_25H      = 67311,

    BERSERK             = 64238
};


enum Says
{
    SAY_AGGRO_L         = -1760045,
    SAY_AGGRO_D         = -1760046,

    SAY_PACT_L          = -1760047,
    SAY_PACT_D          = -1760048,

    SAY_DVORTEX_L       = -1760049,
    SAY_DVORTEX_D       = -1760050,

    SAY_LVORTEX_L       = -1760051,
    SAY_LVORTEX_D       = -1760052,

    SAY_KILLING_L1      = -1760053,
    SAY_KILLING_D1      = -1760054,

    SAY_KILLING_L2      = -1760055,
    SAY_KILLING_D2      = -1760056,

    SAY_BERSERK_L       = -1760057,
    SAY_BERSERK_D       = -1760058,

    SAY_DEATH_L         = -1760059,
    SAY_DEATH_D         = -1760060
};


enum Events
{
    EVENT_NONE,

    EVENT_PACT,
    EVENT_VORTEX,
    EVENT_SOURGE,
    EVENT_TOUCH,
    EVENT_SPIKE
};


#define Pact_Timer              urand(49000, 51000)
#define Vortex_Timer            urand(20000, 25000)
#define Sourge_Timer            urand(29000, 31000)
#define Touch_Timer             urand(44000, 46000)
#define Spike_Timer             urand(10000, 14000)


Position portalSpawnsDark[2]        =
{
    { 548.262085f, 155.034042f, 394.441650f, 1.630077f },
    { 579.656372f, 123.650337f, 394.441559f, 2.348724f }
};

Position portalSpawnsLight[2]      =
{
    { 580.662415f, 156.519287f, 394.442169f, 3.928951f },
    { 546.694153f, 122.910606f, 394.441620f, 0.775576f }
};

Position energiesSpawns[20]         =
{
    //lewy gorny
    { 516.120850f, 153.876724f, 395.257050f, 5.947426f },
    { 520.381531f, 163.129730f, 395.257721f, 5.857108f },
    { 524.978210f, 170.881180f, 395.257721f, 5.735374f },
    { 533.405457f, 179.017960f, 395.255463f, 5.275928f },
    { 545.795288f, 179.278381f, 395.257690f, 4.910729f },
    //prawy gorny
    { 585.779724f, 178.406448f, 395.257477f, 4.368811f },
    { 593.253479f, 174.178940f, 394.803131f, 4.129275f },
    { 599.540405f, 167.275803f, 395.258484f, 3.713021f },
    { 604.066467f, 159.609192f, 395.258484f, 3.571659f },
    { 607.621155f, 148.601044f, 395.258545f, 3.461709f },
    //lewy dolny
    { 607.049744f, 126.789886f, 395.260040f, 2.730471f },
    { 600.815613f, 113.664787f, 395.258667f, 2.596953f },
    { 593.267151f, 105.430504f, 395.258667f, 2.290648f },
    { 583.886169f, 99.776947f, 395.258667f, 2.188545f },
    { 572.608643f, 94.870926f, 395.260468f, 1.776990f },
    //prawy dolny
    { 554.215271f, 95.081627f, 395.261047f, 1.215430f },
    { 545.551514f, 99.042221f, 395.259644f, 1.077986f },
    { 537.194580f, 103.717400f, 395.259674f, 0.952322f },
    { 528.627991f, 110.522202f, 395.259827f, 0.681359f },
    { 522.244507f, 122.121460f, 395.259827f, 0.414324f }
};


struct boss_valkyr_twinAI : public ScriptedAI
{
    boss_valkyr_twinAI(bool light, Creature* creature) : ScriptedAI(creature), summons(me), isLight(light)
    {
        instance = creature->GetInstanceScript();

        if (creature->GetMapId() == 649) // Trial of the Crusader
        {
            if (!isLight)
            {
                summons.Summon(DoSummon(34567, portalSpawnsDark[0])); //dark portals
                summons.Summon(DoSummon(34567, portalSpawnsDark[1]));
            }
            else
            {
                summons.Summon(DoSummon(34568, portalSpawnsLight[0])); //light posrtals
                summons.Summon(DoSummon(34568, portalSpawnsLight[1]));
            }
        }
    }

    InstanceScript* instance;
    EventMap events;
    SummonList summons;
    bool isLight;
    bool isPact;
    uint32 PactTimer;

    uint32 fight_Timer;

    uint32 shield_amount;
    uint32 Energies_Timer;

    void Reset()
    {
        events.Reset();
        summons.DespawnEntry(34630);
        summons.DespawnEntry(34628);

        isPact = false;
        PactTimer = 15000;

        fight_Timer = 0;

        shield_amount = 0;
        Energies_Timer = urand(30000, 35000);

        isLight ? SetEquipmentSlots(false, 49303, 47146, 47267) : SetEquipmentSlots(false, 45990, 47470, 47267);
    }

    void JustReachedHome()
    {
        summons.DespawnAll();
        if (GetTwin() && GetTwin()->isAlive())
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->summons.DespawnAll();

        instance->SetData(RESET_TO_FIGHT, DATA_TWIN_VALKYR);
    }

    void EnterCombat(Unit* who)
    {
        if (instance->GetBossState(DATA_TWIN_VALKYR) != IN_PROGRESS)
        {
            instance->SetBossState(DATA_TWIN_VALKYR, IN_PROGRESS);
            DoScriptText(isLight ? SAY_AGGRO_L : SAY_AGGRO_D, me);
            if (GetTwin() && GetTwin()->isAlive())
                DoScriptText(isLight ? SAY_AGGRO_D : SAY_AGGRO_L, GetTwin());
        }

        if (Creature* twin = GetTwin())
            if (!twin->getVictim())
                twin->AI()->AttackStart(who);


        events.ScheduleEvent(EVENT_PACT, Pact_Timer);
        events.ScheduleEvent(EVENT_VORTEX, Vortex_Timer);
        events.ScheduleEvent(EVENT_SOURGE, Sourge_Timer);
        events.ScheduleEvent(EVENT_SPIKE, Spike_Timer);

        if (getDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || getDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
            events.ScheduleEvent(EVENT_TOUCH, Touch_Timer);
    }

    void KilledUnit(Unit* who)
    {
        switch (urand(0, 1))
        {
        case 0:
            DoScriptText(isLight ? SAY_KILLING_L1 : SAY_KILLING_D1, me);
            if (GetTwin() && GetTwin()->isAlive())
                DoScriptText(isLight ? SAY_KILLING_D1 : SAY_KILLING_L1, GetTwin());
            break;

        case 1:
            DoScriptText(isLight ? SAY_KILLING_L2 : SAY_KILLING_D2, me);
            if (GetTwin() && GetTwin()->isAlive())
                DoScriptText(isLight ? SAY_KILLING_D2 : SAY_KILLING_L2, GetTwin());
            break;
        }

        if (who->GetTypeId() == TYPEID_PLAYER)
            instance->SetData(UPDATE_DEATHS_COUNT, 0);
    }

    void JustDied(Unit* who)
    {
        DoScriptText(isLight ? SAY_DEATH_L : SAY_DEATH_D, me);

        if (GetTwin() && GetTwin()->isAlive())
        {
            who->Kill(GetTwin());
        }

        instance->SetBossState(DATA_TWIN_VALKYR, DONE);
        summons.DespawnAll();

        if (fight_Timer <= 180000) //3 min
            instance->DoCompleteAchievement(RAID_MODE(3799, 3815, 3799, 3815)); //Salt and Pepper
    }

    void SummonedCreatureDespawn(Creature* despawn)
    {
        summons.Despawn(despawn);
    }
    void JustSummoned(Creature* summon)
    {
        summons.Summon(summon);
    }

    void DamageTaken(Unit* who, uint32& damage)
    {
        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            if (who->HasAura(DARK_ESSENCE))
                isLight ? damage *= (uint32)1.5 : damage /= (uint32)1.5;
            if (who->HasAura(LIGHT_ESSENCE))
                isLight ? damage *= (uint32)1.5 : damage /= (uint32)1.5;
        }

        if (me->HasAura(isLight ? RAID_MODE(LIGHT_SHIELD_10N, LIGHT_SHIELD_25N, LIGHT_SHIELD_10H, LIGHT_SHIELD_25H)
            : RAID_MODE(DARK_SHIELD_10N, DARK_SHIELD_25N, DARK_SHIELD_10H, DARK_SHIELD_25H)))
        {
            shield_amount += damage;
            damage = 0;

            if (shield_amount >= (uint32)RAID_MODE(175000, 700000, 300000, 1200000))
            {
                me->RemoveAurasDueToSpell(isLight ? RAID_MODE(LIGHT_SHIELD_10N, LIGHT_SHIELD_25N, LIGHT_SHIELD_10H, LIGHT_SHIELD_25H)
                : RAID_MODE(DARK_SHIELD_10N, DARK_SHIELD_25N, DARK_SHIELD_10H, DARK_SHIELD_25H));

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
            }
        }

        if (GetTwin() && GetTwin()->isAlive())
            GetTwin()->SetHealth(GetTwin()->GetHealth()-damage);
    }

    void SpellHitTarget(Unit* who, const SpellEntry* spell)
    {
        if (spell->Id==RAID_MODE(DARK_VORTEX_10N, DARK_VORTEX_25N, DARK_VORTEX_10H, DARK_VORTEX_25H))
        {
            if (who->HasAura(POWERING_UP) && who->HasAura(DARK_ESSENCE))
                who->CastSpell(who, POWERING_UP, true);
        }

        if (spell->Id==RAID_MODE(LIGHT_VORTEX_10N, LIGHT_VORTEX_25N, LIGHT_VORTEX_10H, LIGHT_VORTEX_25H))
        {
            if (who->HasAura(POWERING_UP) && who->HasAura(LIGHT_ESSENCE))
                who->CastSpell(who, POWERING_UP, true);
        }
    }

    void SpellHit(Unit* who, const SpellEntry* spell)
    {
        if (spell->Id == RAID_MODE(TWINS_PACT_10N, TWINS_PACT_25N, TWINS_PACT_10H, TWINS_PACT_25H))
            if (GetTwin() && GetTwin()->isAlive())
                GetTwin()->SetHealth((GetTwin()->GetMaxHealth()*RAID_MODE(20, 20, 50, 50)/100)+GetTwin()->GetHealth());

    }


    void UpdateAI(const uint32 diff)
    {
        if (me->isInCombat())
            fight_Timer += diff;

        if (!UpdateVictim())
            return;

        if (isPact && !me->IsNonMeleeSpellCasted(true))
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);

        if (!isPact && !me->IsNonMeleeSpellCasted(true) && (!GetTwin() || !GetTwin()->IsNonMeleeSpellCasted(true)))
            events.Update(diff);

        if (Energies_Timer < diff)
        {
            DoSpawnEnergies();
            Energies_Timer = urand(15000, 20000);
        } else Energies_Timer -= diff;


        while (uint32 id = events.GetEvent())
        {
            switch (id)
            {
            case EVENT_PACT:
                DoPact();
                events.RepeatEvent(Pact_Timer);
                break;

            case EVENT_SOURGE:
                DoSourge();
                events.RepeatEvent(Sourge_Timer);
                break;

            case EVENT_TOUCH:
                DoTouch();
                events.RepeatEvent(Touch_Timer);
                break;

            case EVENT_VORTEX:
                DoVortex();
                events.RepeatEvent(Vortex_Timer);
                break;

            case EVENT_SPIKE:
                DoCast(me->getVictim(), RAID_MODE(TWIN_SPIKE_10N, TWIN_SPIKE_25N, TWIN_SPIKE_10H, TWIN_SPIKE_25H));
                events.RepeatEvent(Spike_Timer);
                break;

            default:
                events.PopEvent();
            }
        }

        if (isPact)
        {
            if (PactTimer < diff)
            {
                isPact = false;
                if (GetTwin() && GetTwin()->isAlive())
                    CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->isPact = false;

                if (shield_amount)
                    shield_amount = 0;

                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);

                PactTimer = 15000;
            } else PactTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }

    void DoPact()
    {
        me->MonsterTextEmote(isLight ? "Floja Lightbane begins to cast Twin's Pact!" : "Eydis Darkbane begins to cast Twin's Pact!", 0, true);
        isPact = true;
        me->CastStop();

        if (isLight)
        {
            DoScriptText(SAY_PACT_L, me);
            if (GetTwin() && GetTwin()->isAlive())
                DoScriptText(SAY_PACT_D, GetTwin());

            DoCast(me, RAID_MODE(LIGHT_SHIELD_10N, LIGHT_SHIELD_25N, LIGHT_SHIELD_10H, LIGHT_SHIELD_25H));
        }
        else
        {
            DoScriptText(SAY_PACT_D, me);
            if (GetTwin() && GetTwin()->isAlive())
                DoScriptText(SAY_PACT_L, GetTwin());

            DoCast(me, RAID_MODE(DARK_SHIELD_10N, DARK_SHIELD_25N, DARK_SHIELD_10H, DARK_SHIELD_25H));
        }

        DoCast(me, RAID_MODE(TWINS_PACT_10N, TWINS_PACT_25N, TWINS_PACT_10H, TWINS_PACT_25H));
        if (GetTwin() && GetTwin()->isAlive())
            DoCast(GetTwin(), RAID_MODE(POWER_OF_TWINS_10N, POWER_OF_TWINS_25N, POWER_OF_TWINS_10H, POWER_OF_TWINS_25H));


        if (GetTwin() && GetTwin()->isAlive())
        {
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->events.RescheduleEvent(EVENT_PACT, Pact_Timer);
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->isPact = true;
        }
    }



    void DoSourge()
    {
        if (isLight)
            DoCast(me->getVictim(), RAID_MODE(SURGE_OF_LIGHT_10N, SURGE_OF_LIGHT_25N, SURGE_OF_LIGHT_10H, SURGE_OF_LIGHT_25H));
        else
            DoCast(me->getVictim(), RAID_MODE(SURGE_OF_DARKNESS_10N, SURGE_OF_DARKNESS_25N, SURGE_OF_DARKNESS_10H, SURGE_OF_DARKNESS_25H));

        if (GetTwin() && GetTwin()->isAlive())
        {
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->events.RescheduleEvent(EVENT_SOURGE, Sourge_Timer);
        }
    }

    void DoTouch()
    {
        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
        if (target)
        {
            if (isLight)
                me->CastCustomSpell(RAID_MODE(TOUCH_OF_LIGHT_10, TOUCH_OF_LIGHT_25, TOUCH_OF_LIGHT_10, TOUCH_OF_LIGHT_25), SPELLVALUE_MAX_TARGETS, 1, target);
            else
                me->CastCustomSpell(RAID_MODE(TOUCH_OF_DARKNESS_10, TOUCH_OF_DARKNESS_25, TOUCH_OF_DARKNESS_10, TOUCH_OF_DARKNESS_25), SPELLVALUE_MAX_TARGETS, 1, target);
        }
        if (GetTwin() && GetTwin()->isAlive())
        {
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->events.RescheduleEvent(EVENT_TOUCH, Touch_Timer);
        }
    }



    void DoVortex()
    {
        Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
        if (target)
        {
            if (isLight)
            {
                DoScriptText(SAY_LVORTEX_L, me);
                if (GetTwin() && GetTwin()->isAlive())
                    DoScriptText(SAY_LVORTEX_D, GetTwin());

                DoCast(target, RAID_MODE(LIGHT_VORTEX_10N, LIGHT_VORTEX_25N, LIGHT_VORTEX_10H, LIGHT_VORTEX_25H));
            }
            else
            {
                DoScriptText(SAY_DVORTEX_D, me);
                if (GetTwin() && GetTwin()->isAlive())
                    DoScriptText(SAY_DVORTEX_L, GetTwin());

                DoCast(target, RAID_MODE(DARK_VORTEX_10N, DARK_VORTEX_25N, DARK_VORTEX_10H, DARK_VORTEX_25H));
            }
        }

        if (GetTwin() && GetTwin()->isAlive())
        {
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->events.RescheduleEvent(EVENT_VORTEX, Vortex_Timer);
        }
    }

    void DoSpawnEnergies()
    {
        bool low = RAID_MODE(true, false, true, false);
        if (GetTwin() && GetTwin()->isAlive())
        {
            CAST_AI(boss_valkyr_twinAI, GetTwin()->AI())->Energies_Timer = urand(15000, 20000);
        }

        uint8 lightcount=0;
        uint8 darkcount=0;
        bool firstLight = (urand(0,1) == 1);
        for (int i = 0; i < RAID_MODE(10, 20, 10, 20); ++i)
        {
            uint8 randMove = urand(1,3);
            uint8 randcolor = urand(0,1);
            if (i==0 || i%2==0)
            {
                if (Creature* energies = DoSummon(firstLight ? randcolor?(lightcount< (low ? 5:10)?34630:34628):(darkcount< (low ? 5:10)?34628:34630) : randcolor?(darkcount< (low ? 5:10)?34628:34630):(lightcount< (low ? 5:10)?34630:34628), energiesSpawns[low ? i*2 : i], 45000, TEMPSUMMON_TIMED_DESPAWN))
                {
                    energies->SetSpeed(MOVE_RUN, 0.575f, true);
                    if (energies->GetEntry() == 34630)
                        ++lightcount;
                    if (energies->GetEntry() == 34628)
                        ++darkcount;
                }
            }
            else
            {
                if (Creature* energies = DoSummon(firstLight ? randcolor?(darkcount< (low ? 5:10)?34628:34630):(lightcount< (low ? 5:10)?34630:34628) : randcolor?(lightcount< (low ? 5:10)?34630:34628):(darkcount< (low ? 5:10)?34628:34630), energiesSpawns[low ? i*2 : i], 45000, TEMPSUMMON_TIMED_DESPAWN))
                {
                    energies->SetSpeed(MOVE_RUN, 0.575f, true);
                    if (energies->GetEntry() == 34630)
                        ++lightcount;
                    if (energies->GetEntry() == 34628)
                        ++darkcount;
                }
            }
        }

        if (getDifficulty() == 2 || getDifficulty() == 3) //hero only
        {
            for (int i=0; i<6; ++i)
            {
                uint8 p = urand(0, 19);
                uint8 randcolor = urand(0,1);
                if (i==0 || i%2==0)
                {
                    if (Creature* energies = DoSummon(firstLight ? randcolor?(lightcount< (low ? 8:13)?34630:34628):(darkcount< (low ? 8:13)?34628:34630) : randcolor?(darkcount< (low ? 8:13)?34628:34630):(lightcount< (low ? 8:13)?34630:34628), energiesSpawns[low ? p*2 : p], 45000, TEMPSUMMON_TIMED_DESPAWN))
                    {
                        energies->SetSpeed(MOVE_RUN, 0.575f, true);
                        if (energies->GetEntry() == 34630)
                            ++lightcount;
                        if (energies->GetEntry() == 34628)
                            ++darkcount;
                    }
                }
                else
                {
                    if (Creature* energies = DoSummon(firstLight ? randcolor?(darkcount< (low ? 8:13)?34628:34630):(lightcount< (low ? 8:13)?34630:34628) : randcolor?(lightcount< (low ? 8:13)?34630:34628):(darkcount< (low ? 8:13)?34628:34630), energiesSpawns[low ? p*2 : p], 45000, TEMPSUMMON_TIMED_DESPAWN))
                    {
                        energies->SetSpeed(MOVE_RUN, 0.575f, true);
                        if (energies->GetEntry() == 34630)
                            ++lightcount;
                        if (energies->GetEntry() == 34628)
                            ++darkcount;
                    }
                }
            }
        }
    }


private:
    Creature* GetTwin()
    {
        if (isLight)
            return ObjectAccessor::GetCreatureOrPetOrVehicle((*me), instance->GetData64(BOSS_DARK_TWIN));
        else
            return ObjectAccessor::GetCreatureOrPetOrVehicle((*me), instance->GetData64(BOSS_LIGHT_TWIN));
    }
};

class boss_light_twin : public CreatureScript
{
public:
    boss_light_twin() : CreatureScript("boss_light_twin") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_valkyr_twinAI(true, creature);
    }

};

class boss_dark_twin : public CreatureScript
{
public:
    boss_dark_twin() : CreatureScript("boss_dark_twin") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_valkyr_twinAI(false, creature);
    }

};



struct mob_concentrated_energieAI : public ScriptedAI
{
    mob_concentrated_energieAI(bool light, Creature* creature) : ScriptedAI(creature), isLight(light)
    {
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        creature->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pos = (*creature);
        actualPosition = (*creature);
        inOnePlace_Timer = 0;
        done = false;
    }

    bool isLight;
    uint32 inOnePlace_Timer;
    Position pos;
    Position actualPosition;
    bool done;

    void KilledUnit(Unit* who)
    {
        if (who->GetTypeId() == TYPEID_PLAYER && me->GetInstanceScript())
            me->GetInstanceScript()->SetData(UPDATE_DEATHS_COUNT, 0);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!me->GetInstanceScript() || !me->GetMap()->HavePlayers())
            return;

        if (me->GetPositionX() == actualPosition.GetPositionX()
            && me->GetPositionY() == actualPosition.GetPositionY())
            inOnePlace_Timer += diff;
        else
        {
            inOnePlace_Timer = 0;
            actualPosition = (*me);
        }

        if (inOnePlace_Timer >= 1000)
        {
            inOnePlace_Timer = 0;
            int8 curpos = -1;
            for (int i=0; i<20; ++i)
            {
                if (energiesSpawns[i].GetPositionX() == pos.GetPositionX()
                    && energiesSpawns[i].GetPositionY() == pos.GetPositionY())
                {
                    curpos = i;
                    break;
                }
            }

            if (curpos == -1)
            {
                uint32 random = urand(0,19);
                pos = energiesSpawns[random];
                me->GetMotionMaster()->MovePoint(0, pos);
            }
            else
            {
                int8 goToPosition = curpos;

                uint8 randMove = urand(1,3);
                uint8 goRight = urand(0,1);
                uint8 minus = urand(0,1);

                goToPosition = goRight ? curpos+5 : curpos-5;
                goToPosition = minus ? goToPosition -= randMove : goToPosition += randMove;

                if (goToPosition < 0)
                    goToPosition += 20;
                if (goToPosition > 20)
                    goToPosition -= 20;

                //pos = energiesSpawns[curpos >= 10 ? (curpos-5-randMove<0)?(curpos-5)+randMove:(curpos-5)-randMove : (curpos+5+randMove>=20)?(curpos+5)+randMove:(curpos-5)-randMove];
                pos = energiesSpawns[goToPosition];
                me->GetMotionMaster()->MovePoint(0, pos);
            }
        }

        if (done)
            return;

        if (const Map* map = me->GetMap())
        {
            Map::PlayerList const& list = map->GetPlayers();
            for (Map::PlayerList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
            {
                if (Player* plr = itr->getSource())
                {
                    if (plr->isGameMaster())
                        continue;
                    if (!plr->HasAura(LIGHT_ESSENCE) && !plr->HasAura(DARK_ESSENCE))
                        continue;
                    if (me->GetDistance2d(plr) > 0.5f)
                        continue;


                    if (plr->HasAura(isLight ? LIGHT_ESSENCE : DARK_ESSENCE))
                    {
                        //don't turning done 'true' will make energies to cast buff 2-3 times on target, witch (afaik) is blizzlike
                        plr->AddAura(POWERING_UP, plr);
                        me->ForcedDespawn(500);
                        return;
                    }
                    else if (plr->HasAura(isLight ? DARK_ESSENCE : LIGHT_ESSENCE))
                    {
                        DoCast(plr, isLight ? UNLEASHED_LIGHT : UNLEASHED_DARK);
                        me->ForcedDespawn(500);
                        done = true;
                        return;
                    }
                }
            }
        }
    }
};


class mob_concetrated_light : public CreatureScript
{
public:
    mob_concetrated_light() : CreatureScript("mob_concetrated_light") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_concentrated_energieAI(true, creature);
    }

};


class mob_concetrated_dark : public CreatureScript
{
public:
    mob_concetrated_dark() : CreatureScript("mob_concetrated_dark") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_concentrated_energieAI(false, creature);
    }

};



class dark_essence : public CreatureScript
{
public:
    dark_essence() : CreatureScript("dark_essence") { }

    bool OnGossipHello(Player* player, Creature* /*creature*/)
    {
        if (player->HasAura(LIGHT_ESSENCE))
            player->RemoveAurasDueToSpell(LIGHT_ESSENCE);

        player->CastSpell(player, DARK_ESSENCE, false);

        return true;
    }

};

class light_essence : public CreatureScript
{
public:
    light_essence() : CreatureScript("light_essence") { }

    bool OnGossipHello(Player* player, Creature* /*creature*/)
    {
        if (player->HasAura(DARK_ESSENCE))
            player->RemoveAurasDueToSpell(DARK_ESSENCE);

        player->CastSpell(player, LIGHT_ESSENCE, false);

        return true;
    }

};

void AddSC_boss_twins_valkyr()
{
    new boss_light_twin();
    new boss_dark_twin();
    new mob_concetrated_light();
    new mob_concetrated_dark();
    new dark_essence();
    new light_essence();
}