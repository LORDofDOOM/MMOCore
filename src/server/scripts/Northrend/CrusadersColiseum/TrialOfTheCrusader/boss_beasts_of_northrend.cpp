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

#include <string>

#include "ScriptPCH.h"
#include "trial_of_the_crusader.h"

extern Position positionCenter;

enum Spells
{
    ///* GORMOK *///
    //gormok
    IMPALE_10N      = 66331,
    IMPALE_10H      = 67478,
    IMPALE_25N      = 67477,
    IMPALE_25H      = 67479,

    STOMP_10N       = 66330,
    STOMP_10H       = STOMP_10N,
    STOMP_25N       = 67648,
    STOMP_25H       = 67649,

    ANGER           = 66636,

    //snobold
    BATTER          = 66408,
    BOMB            = 66313,
    CRACK_HEAD      = 66407,

    ///* ZMIJE *///
    //ognisty
    BURNING_BITE_10N    = 66879,
    BURNING_BITE_10H    = 67625,
    BURNING_BITE_25N    = 67624,
    BURNING_BITE_25H    = 67624,

    BURNING_SPRAY_10N   = 66902,
    BURNING_SPRAY_10H   = 67628,
    BURNING_SPRAY_25N   = 67627,
    BURNING_SPRAY_25H   = 67629,

    FIRE_SPIT_10N       = 66796,
    FIRE_SPIT_10H       = 67633,
    FIRE_SPIT_25N       = 67632,
    FIRE_SPIT_25H       = 67634,

    MOLTEN_SPEW_10N     = 66821,
    MOLTEN_SPEW_10H     = 66821,
    MOLTEN_SPEW_25N     = 66821,
    MOLTEN_SPWE_25H     = 66821,

    //toksyczny
    ACID_SPIT_10N       = 66880,
    ACID_SPIT_10H       = 67607,
    ACID_SPIT_25N       = 67606,
    ACID_SPIT_25H       = 67608,

    ACID_SPEW_10N       = 66818,
    ACID_SPEW_10H       = 66818,
    ACID_SPEW_25N       = 66818,
    ACID_SPEW_25H       = 66818,

    PARALYTIC_BITE_10N  = 66824,
    PARALYTIC_BITE_10H  = 67613,
    PARALYTIC_BITE_25N  = 67612,
    PARALYTIC_BITE_25H  = 67614,

    PARALYTIC_SPRAY_10N = 66901,
    PARALYTIC_SPRAY_10H = 67616,
    PARALYTIC_SPRAY_25N = 67615,
    PARALYTIC_SPRAY_25H = 67617,


    //oba
    ZMIJE_ENRAGE        = 68335,
    ADD_10N             = 66883,
    ADD_10H             = 67642,
    ADD_25N             = 67641,
    ADD_25H             = 67643,
    SWEEP_10N           = 66794,
    SWEEP_10H           = 67645,
    SWEEP_25N           = 67644,
    SWEEP_25H           = 67646,

    BURROW              = 53421, //visual
    EMERGE              = 66947, //visual
    ENRAGE              = 68335,

    ///* ICEHOWL *///
    FEROCIOUS_BUTT_10N  = 66770,
    FEROCIOUS_BUTT_10H  = 67655,
    FEROCIOUS_BUTT_25N  = 67654,
    FEROCIOUS_BUTT_25H  = 67656,

    MASSIVE_CRASH_10N   = 66683,
    MASSIVE_CRASH_10H   = 67661,
    MASSIVE_CRASH_25N   = 67660,
    MASSIVE_CRASH_25H   = 67662,

    WHIRL_10N           = 67345,
    WHIRL_10H           = 67664,
    WHIRL_25N           = 67663,
    WHIRL_25H           = 67665,

    CHARGE              = 66734,
    DAZE                = 66758,
    BUFF                = 66759,
    BREATH              = 66689,
    BERSERK             = 26662
};

enum Says
{
    SAY_BEASTS_JORMUNGAR_KILLED = -1760003,
    SAY_BEASTS_WIPE         = -1760006
};

enum Events
{
    EVENT_NULL,

    ///* GORMOK *///
    EVENT_IMPALE,
    EVENT_STOMP,
    EVENT_SNOBOLD,

    ///* ZMIJE *///
    //ognisty
    EVENT_BURN,
    EVENT_MOLTEN,

    //toksyczny
    EVENT_ACID,
    EVENT_PARALYTIC,

    //oba
    EVENT_ADD,
    EVENT_KNOCKBACK,
    EVENT_BURRED,

    ///* ICEHOWL *///
    EVENT_FEROCIOUS_BUTT,
    EVENT_ARCTIC_BREATH,
    EVENT_WHIRL,
    EVENT_MASSIVE_CRASH
};


#define Impale_Timer        urand(7500, 9000)
#define Stomp_Timer         urand(15000, 20000)

class boss_gormok_the_impaler : public CreatureScript
{
public:
    boss_gormok_the_impaler() : CreatureScript("boss_gormok_the_impaler") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_gormok_the_impalerAI(creature);
    }

    struct boss_gormok_the_impalerAI : public ScriptedAI
    {
        boss_gormok_the_impalerAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            Instance = creature->GetInstanceScript();
        }

        InstanceScript* Instance;
        EventMap events;
        SummonList summons;

        bool snobold80;
        bool snobold60;
        bool snobold40;
        bool snobold20;

        void Reset()
        {
            events.Reset();
            summons.DespawnAll();
            snobold80 = false;
            snobold60 = false;
            snobold40 = false;
            snobold20 = false;
        }

        void JustReachedHome()
        {
            if (Instance->GetBossState(DATA_BEASTS) != FAIL)
            {
                Instance->SetData(RESET_TO_FIGHT, DATA_BEASTS);
                if (Unit* fordring = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), Instance->GetData64(TIRION_FORDING)))
                    DoScriptText(SAY_BEASTS_WIPE, fordring);
            }

            me->DespawnOrUnsummon();
        }


        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_IMPALE, Impale_Timer);
            events.ScheduleEvent(EVENT_STOMP, Stomp_Timer);

            Instance->SetBossState(DATA_BEASTS, IN_PROGRESS);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                Instance->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (HealthBelowPct(81) && !snobold80)
            {
                DoSnobold();
                snobold80 = true;
            }
            if (HealthBelowPct(61) && !snobold60)
            {
                DoSnobold();
                snobold60 = true;
            }
            if (HealthBelowPct(41) && !snobold40)
            {
                DoSnobold();
                snobold40 = true;
            }
            if (HealthBelowPct(21) && !snobold20)
            {
                DoSnobold();
                snobold20 = true;
            }


            while (uint32 id = events.GetEvent())
            {
                switch (id)
                {
                case EVENT_IMPALE:
                    DoCast(me->getVictim(), RAID_MODE(IMPALE_10N, IMPALE_25N, IMPALE_10H, IMPALE_25H));
                    events.RepeatEvent(Impale_Timer);
                    break;

                case EVENT_STOMP:
                    DoCast(me->getVictim(), RAID_MODE(STOMP_10N, STOMP_25N, STOMP_10H, STOMP_25H));
                    events.RepeatEvent(Stomp_Timer);
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }



        void DoSnobold()
        {
            DoCast(me, ANGER);

            uint8 plr_count = 0;
            std::vector<uint64> targets;
            for (std::list<HostileReference*>::const_iterator itr = me->getThreatManager().getThreatList().begin();
                itr != me->getThreatManager().getThreatList().end(); ++itr)
            {
                Unit* u = ObjectAccessor::GetUnit((*me), (*itr)->getUnitGuid());
                if (!u || !u->isAlive() || u->GetTypeId() != TYPEID_PLAYER || u->HasAura(66406)) //already snobolled!
                    continue;

                if (u->ToPlayer()->isGameMaster())
                    continue;
                ++plr_count;
                targets.push_back(u->GetGUID());
            }

            if (!plr_count || targets.empty())
            {
                Creature* snobold = DoSummon(NPC_SNOBOLD, (*me));
                return;
            }

            Unit* target = ObjectAccessor::GetUnit((*me), targets[rand()%targets.size()]);

            if (target)
            {
                Creature* snobold = DoSummon(NPC_SNOBOLD, (*target));
                me->AddAura(66406, target); //snobolled!
                snobold->AI()->AttackStart(target);
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }
        void SummonedCreatureDespawn(Creature* despawn)
        {
            summons.Despawn(despawn);
        }
    };

};

class mob_snobold : public CreatureScript
{
public:
    mob_snobold() : CreatureScript("mob_snobold") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_snoboldAI(creature);
    }

    struct mob_snoboldAI : public ScriptedAI
    {
        mob_snoboldAI(Creature* creature) : ScriptedAI(creature)
        {
            creature->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            targetDead = false;
        }

        uint64 targetguid;
        std::list<uint64> targets;

        uint32 Batter_Cooldown;
        uint32 Bomb_Timer;
        uint32 HeadCrack_Cooldown;
        bool targetDead;

        void Reset()
        {
            targetguid = 0;
            targets.clear();

            Batter_Cooldown = 15000;
            Bomb_Timer = urand(7500, 10000);
            HeadCrack_Cooldown = 30000;
            targetDead = false;
        }

        void JustDied(Unit* who)
        {
            if (targetguid)
                if (Unit* target = ObjectAccessor::GetUnit((*me), targetguid))
                    if (target->HasAura(66406))
                        target->RemoveAurasDueToSpell(66406);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && me->GetInstanceScript())
                me->GetInstanceScript()->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void AttackStart(Unit* who)
        {
            if (!targetguid && !targetDead)
            {
                targetguid = who->GetGUID();
            }

            ScriptedAI::AttackStart(who);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!targetguid || !ObjectAccessor::GetUnit((*me), targetguid))
            {
                Creature* gormok = ObjectAccessor::GetCreature((*me), me->GetInstanceScript()->GetData64(BOSS_GORMOK));
                if (gormok && gormok->isAlive())
                {
                    me->GetMotionMaster()->MoveJump(gormok->GetPositionX(), gormok->GetPositionY(), gormok->GetPositionZ(), 15.0f, 15.0f);
                    SetCombatMovement(false);
                    if (!targetDead)
                        targetDead = true;
                }
                else
                {
                    SetCombatMovement(true);
                    targetDead = false;
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true);
                    if (target)
                        targetguid = target->GetGUID();
                    else
                        targetguid = 0;
                }
            }

            if (targetguid && me->getVictim()->GetGUID() != targetguid)
            {
                if (Unit* target = ObjectAccessor::GetUnit((*me), targetguid))
                    me->AI()->AttackStart(target);
                else
                    targetguid = 0;
            }

            targets.clear();

            if (const Map* map = me->GetMap())
            {
                Map::PlayerList const& list = map->GetPlayers();
                if (!list.isEmpty())
                    for (Map::PlayerList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
                    {
                        Player* actual = itr->getSource();

                        if (actual)
                        {
                            if (actual->isGameMaster())
                                continue;

                            if (me->GetDistance2d(actual) < 7.5f)
                                targets.push_back(actual->GetGUID());
                        }
                    }
            }

            if (Bomb_Timer < diff)
            {
                if (Unit* target = GetRandomUnitFromList())
                {
                    DoCast(target, BOMB);
                    Bomb_Timer = urand(7500, 10000);
                }
            } else Bomb_Timer -= diff;

            if (Batter_Cooldown != 0)
            {
                if (diff > Batter_Cooldown)
                    Batter_Cooldown = 0;
                else
                    Batter_Cooldown -= diff;
            }
            if (HeadCrack_Cooldown != 0)
            {
                if (diff > HeadCrack_Cooldown)
                    HeadCrack_Cooldown = 0;
                else
                    HeadCrack_Cooldown -= diff;
            }

            if (!targets.empty())
                for (std::list<uint64>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                {
                    Unit* target = Unit::GetUnit((*me), (*itr));
                    if (target)
                    {
                        if (target->IsNonMeleeSpellCasted(true))
                        {
                            if (Batter_Cooldown == 0)
                            {
                                DoCast(target, BATTER);
                                Batter_Cooldown = 15000;
                                continue;
                            }
                            if (HeadCrack_Cooldown == 0)
                            {
                                DoCast(target, CRACK_HEAD);
                                HeadCrack_Cooldown = 30000;
                                break;
                            }
                        }
                    }
                }


            DoMeleeAttackIfReady();
        }


        Unit* GetRandomUnitFromList()
        {
            if (targets.empty())
                return NULL;

            std::list<uint64>::iterator itr = targets.begin();
            advance (itr , (rand() % (targets.size())));
            return ObjectAccessor::GetUnit((*me),(*itr));
        }
    };

};

#define Burred_Timer            40000
#define Knockback_Timer         urand(10000, 15000)
#define Add_Timer               urand(10000, 15000)
#define Acid_Timer              urand(6000, 8000)
#define Paralytic_Timer         isMobile ? 15000 : 6000
#define Burn_Timer              urand(6000, 8000)
#define Molten_Timer            isMobile ? 15000 : urand(6000, 8000)

class boss_acidmaw : public CreatureScript
{
public:
    boss_acidmaw() : CreatureScript("boss_acidmaw") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_acidmawAI(creature);
    }

    struct boss_acidmawAI : public ScriptedAI
    {
        boss_acidmawAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            Instance = creature->GetInstanceScript();
            me->SetVisible(false);
        }

        InstanceScript* Instance;
        bool isMobile;
        bool isBurred;
        bool isBurred_End;

        uint32 isBurred_Timer;

        EventMap events;
        SummonList summons;

        void Reset()
        {
            isMobile = false;
            isBurred = false;
            isBurred_End = false;
            isBurred_Timer = 5000;

            events.Reset();
            summons.DespawnAll();
        }

        void JustReachedHome()
        {
            if (Instance->GetBossState(DATA_BEASTS) != FAIL)
            {
                Instance->SetData(RESET_TO_FIGHT, DATA_BEASTS);
                if (Unit* fordring = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), Instance->GetData64(TIRION_FORDING)))
                    DoScriptText(SAY_BEASTS_WIPE, fordring);
            }

            me->DespawnOrUnsummon();
        }


        void EnterCombat(Unit* who)
        {
            me->SetVisible(true);
            DoCast(me, EMERGE);

            events.ScheduleEvent(EVENT_BURRED, Burred_Timer);
            events.ScheduleEvent(EVENT_ACID, Acid_Timer);
            events.ScheduleEvent(EVENT_PARALYTIC, Paralytic_Timer);
            events.ScheduleEvent(EVENT_KNOCKBACK, Knockback_Timer);

            if (!isMobile)
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                Instance->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void JustDied(Unit* who)
        {
            Instance->SetData(TIME_ACIDMAW_DEAD, 0);
            summons.DespawnAll();

            if (Creature* jormungar = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), Instance->GetData64(BOSS_DREADSCALE)))
            {
                if (jormungar->isAlive())
                {
                    jormungar->CastSpell(jormungar, ENRAGE, false);
                    DoScriptText(SAY_BEASTS_JORMUNGAR_KILLED, jormungar);
                }
            }
        }


        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!isBurred)
                events.Update(diff);

            while (uint32 id = events.ExecuteEvent())
            {
                switch (id)
                {
                case EVENT_BURRED:
                    ChangeState();
                    break;

                case EVENT_ACID:
                    if (isMobile)
                        DoCast(me->getVictim(), RAID_MODE(ACID_SPEW_10N, ACID_SPEW_25N, ACID_SPEW_10H, ACID_SPEW_25H));
                    else
                        DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), RAID_MODE(ACID_SPIT_10N, ACID_SPIT_25N, ACID_SPIT_10H, ACID_SPIT_25H));

                    events.ScheduleEvent(EVENT_ACID, urand(6000, 8000));
                    break;

                case EVENT_PARALYTIC:
                    if (isMobile)
                        DoCast(me->getVictim(), RAID_MODE(PARALYTIC_BITE_10N, PARALYTIC_BITE_25N, PARALYTIC_BITE_10H, PARALYTIC_BITE_25H));
                    else
                        DoCast(me->getVictim(), RAID_MODE(PARALYTIC_SPRAY_10N, PARALYTIC_SPRAY_25N, PARALYTIC_SPRAY_10H, PARALYTIC_SPRAY_25H));

                    events.ScheduleEvent(EVENT_PARALYTIC, Paralytic_Timer);
                    break;

                case EVENT_KNOCKBACK:
                    DoCast(RAID_MODE(SWEEP_10N, SWEEP_25N, SWEEP_10H, SWEEP_25H));
                    events.ScheduleEvent(EVENT_KNOCKBACK, Knockback_Timer);
                    break;

                case EVENT_ADD:
                    DoSummon(35176, (*me), 60000, TEMPSUMMON_TIMED_DESPAWN);
                    events.ScheduleEvent(EVENT_ADD, Add_Timer);
                    break;
                }
            }

            if (isBurred)
            {
                if (isBurred_Timer < diff)
                {
                    me->RemoveAllAuras();
                    me->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                    DoResetThreat();

                    DoCast(me, EMERGE);
                    isBurred = false;
                    isBurred_End = false;

                    if (isMobile)
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    }
                    else
                    {
                        me->StopMoving();
                        me->GetMotionMaster()->MoveIdle();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    }
                    isBurred_Timer = 4000;
                } else isBurred_Timer -= diff;
            }

            if (!isBurred)
                DoMeleeAttackIfReady();
        }


        void ChangeState()
        {
            isBurred = true;
            DoCast(me, BURROW);

            me->RemoveAllAuras();
            me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            DoResetThreat();

            events.Reset();

            if (isMobile)
            {
                isMobile = false;
                me->SetDisplayId(29815); //acidmaw's stacionary model
                events.ScheduleEvent(EVENT_BURRED, Burred_Timer);
                events.ScheduleEvent(EVENT_ACID, urand(6000, 8000));
                events.ScheduleEvent(EVENT_PARALYTIC, Paralytic_Timer);
                events.ScheduleEvent(EVENT_KNOCKBACK, Knockback_Timer);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }
            else
            {
                isMobile = true;
                me->SetDisplayId(29816); //acidmaw's mobile model
                events.ScheduleEvent(EVENT_BURRED, Burred_Timer);
                events.ScheduleEvent(EVENT_ACID, urand(6000, 8000));
                events.ScheduleEvent(EVENT_PARALYTIC, Paralytic_Timer);
                events.ScheduleEvent(EVENT_ADD, Add_Timer);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }
        }
    };

};

class boss_dreadscale : public CreatureScript
{
public:
    boss_dreadscale() : CreatureScript("boss_dreadscale") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_dreadscaleAI(creature);
    }

    struct boss_dreadscaleAI : public ScriptedAI
    {
        boss_dreadscaleAI(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            Instance = creature->GetInstanceScript();
        }

        InstanceScript* Instance;
        bool isMobile;
        bool isBurred;
        bool isBurred_End;

        uint32 isBurred_Timer;

        EventMap events;
        SummonList summons;

        void Reset()
        {
            isMobile = true;
            isBurred = false;
            isBurred_End = false;
            isBurred_Timer = 5000;

            events.Reset();
            summons.DespawnAll();
        }

        void JustReachedHome()
        {
            if (Instance->GetBossState(DATA_BEASTS) != FAIL)
            {
                Instance->SetData(RESET_TO_FIGHT, DATA_BEASTS);
                if (Unit* fordring = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), Instance->GetData64(TIRION_FORDING)))
                    DoScriptText(SAY_BEASTS_WIPE, fordring);
            }
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_BURRED, Burred_Timer);
            events.ScheduleEvent(EVENT_BURN, Burn_Timer);
            events.ScheduleEvent(EVENT_MOLTEN, Molten_Timer);
            events.ScheduleEvent(EVENT_ADD, Add_Timer);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                Instance->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void JustDied(Unit* who)
        {
            Instance->SetData(TIME_DREADSCALE_DEAD, 0);
            summons.DespawnAll();

            if (Creature* jormungar = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), Instance->GetData64(BOSS_ACIDMAW)))
            {
                if (jormungar->isAlive())
                {
                    jormungar->CastSpell(jormungar, ENRAGE, false);
                    DoScriptText(SAY_BEASTS_JORMUNGAR_KILLED, jormungar);
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!isBurred)
                events.Update(diff);

            while (uint32 id = events.GetEvent())
            {
                switch (id)
                {
                case EVENT_BURRED:
                    ChangeState();
                    break;

                case EVENT_BURN:
                    if (isMobile)
                        DoCast(me->getVictim(), RAID_MODE(BURNING_BITE_10H, BURNING_BITE_25N, BURNING_BITE_10H, BURNING_BITE_25H));
                    else
                        DoCast(me->getVictim(), RAID_MODE(BURNING_SPRAY_10H, BURNING_SPRAY_25N, BURNING_SPRAY_10H, BURNING_SPRAY_25H));

                    events.RepeatEvent(Burn_Timer);
                    break;

                case EVENT_MOLTEN:
                    if (isMobile)
                        DoCast(me->getVictim(), RAID_MODE(MOLTEN_SPEW_10N, ACID_SPEW_25N, ACID_SPEW_10H, ACID_SPEW_25H));
                    else
                        DoCast(me->getVictim(), RAID_MODE(FIRE_SPIT_10N, FIRE_SPIT_25N, FIRE_SPIT_10H, FIRE_SPIT_25H));

                    events.RepeatEvent(Molten_Timer);
                    break;

                case EVENT_KNOCKBACK:
                    DoCast(RAID_MODE(SWEEP_10N, SWEEP_25N, SWEEP_10H, SWEEP_25H));
                    events.RepeatEvent(Knockback_Timer);
                    break;

                case EVENT_ADD:
                    DoSummon(35176, (*me), 60000, TEMPSUMMON_TIMED_DESPAWN);
                    events.RepeatEvent(Add_Timer);
                    break;
                }
            }

            if (isBurred)
            {
                if (isBurred_Timer < diff)
                {
                    me->RemoveAllAuras();
                    me->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                    DoResetThreat();

                    DoCast(me, EMERGE);
                    isBurred = false;
                    isBurred_End = false;

                    if (isMobile)
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    }
                    else
                    {
                        me->StopMoving();
                        me->GetMotionMaster()->MoveIdle();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    }
                    isBurred_Timer = 4000;
                } else isBurred_Timer -= diff;
            }

            if (!isBurred)
                DoMeleeAttackIfReady();
        }


        void ChangeState()
        {
            isBurred = true;
            DoCast(me, BURROW);

            me->RemoveAllAuras();
            me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            DoResetThreat();

            events.Reset();

            if (isMobile)
            {
                isMobile = false;
                me->SetDisplayId(26935); //dreadscale's stacionary model
                events.ScheduleEvent(EVENT_BURRED, Burred_Timer);
                events.ScheduleEvent(EVENT_BURN, Burn_Timer);
                events.ScheduleEvent(EVENT_MOLTEN, Molten_Timer);
                events.ScheduleEvent(EVENT_KNOCKBACK, Knockback_Timer);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

            }
            else
            {
                isMobile = true;
                me->SetDisplayId(24564); //dreadscale's mobile model
                events.ScheduleEvent(EVENT_BURRED, Burred_Timer);
                events.ScheduleEvent(EVENT_BURN, Burn_Timer);
                events.ScheduleEvent(EVENT_MOLTEN, Molten_Timer);
                events.ScheduleEvent(EVENT_ADD, Add_Timer);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }
        }
    };

};


#define FerociousButt_Timer         urand(5000, 7000)
#define ArcticBreath_Timer          urand(10000, 15000)
#define Whirl_Timer                 13000
#define MassiveCrash_Timer          25000


// Icehowl
class boss_icehowl : public CreatureScript
{
public:
    boss_icehowl() : CreatureScript("boss_icehowl") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_icehowlAI(creature);
    }

    struct boss_icehowlAI : BossAI
    {
        boss_icehowlAI(Creature* creature) : BossAI(creature, DATA_BEASTS)  { }


        uint32 Berserk_Timer;
        uint32 Crashing_Timer;

        bool massiveCrashStart;
        uint32 massiveCrashStep;
        uint32 massiveCrashStep_Timer;
        uint64 massiveCrashTarget_Guid;

        float targetsX;
        float targetsY;
        float targetsZ;

        bool chargeHit;
        bool chargeStart;
        bool chargeDone;

        void Reset()
        {
            Berserk_Timer = 900000;
            Crashing_Timer = 7000;

            massiveCrashStart = false;
            massiveCrashStep = 0;
            massiveCrashStep_Timer = 1000;
            massiveCrashTarget_Guid = 0;

            targetsX = 0;
            targetsY = 0;
            targetsZ = 0;

            chargeHit = false;
            chargeStart = false;
            chargeDone = false;

            _Reset();
        }

        void JustReachedHome()
        {
            instance->SetData(RESET_TO_FIGHT, DATA_BEASTS);
            if (Unit* fordring = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), instance->GetData64(TIRION_FORDING)))
                DoScriptText(SAY_BEASTS_WIPE, fordring);
        }

        void EnterCombat(Unit* who)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_FEROCIOUS_BUTT, FerociousButt_Timer);
            events.ScheduleEvent(EVENT_ARCTIC_BREATH, ArcticBreath_Timer);
            events.ScheduleEvent(EVENT_WHIRL, Whirl_Timer);
            events.ScheduleEvent(EVENT_MASSIVE_CRASH, MassiveCrash_Timer);
        }

        void JustDied(Unit* who)
        {
            _JustDied();
        }

        void SpellHitTarget(Unit* target, const SpellEntry* spell)
        {
            if (spell->Id == CHARGE)
                if (target->GetTypeId() == TYPEID_PLAYER)
                    if (!chargeHit)
                        chargeHit = true;
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                instance->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(type != POINT_MOTION_TYPE) return;
            if (chargeStart && !chargeDone) //charge end
            {
                DoCastAOE(CHARGE);
                chargeDone = true;
            }
        }


        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!massiveCrashStart)
            {
                if (!me->IsNonMeleeSpellCasted(true))
                    events.Update(diff);
            }
            else
            {
                if (massiveCrashStep_Timer < diff)
                {
                    switch (massiveCrashStep)
                    {
                    case 0:
                    case 1:
                        me->GetMotionMaster()->MovePoint(1 ,positionCenter.GetPositionX(), positionCenter.GetPositionY(), positionCenter.GetPositionZ());
                        massiveCrashStep_Timer = 2000;
                        ++massiveCrashStep;
                        break;

                    case 2:
                        DoCastAOE(RAID_MODE(MASSIVE_CRASH_10N, MASSIVE_CRASH_25N, MASSIVE_CRASH_10H, MASSIVE_CRASH_25H));
                        massiveCrashStep_Timer = 3000;
                        ++massiveCrashStep;
                        break;

                    case 3:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                        {
                            //select target
                            massiveCrashTarget_Guid = target->GetGUID();
                            me->SetUInt64Value(UNIT_FIELD_TARGET, massiveCrashTarget_Guid);

                            //prepere boss
                            SetCombatMovement(false);
                            me->GetMotionMaster()->MoveIdle();
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->SetFacingToObject(target);

                            //make announce for players
                            std::string announce("Icehowl glares at ");
                            announce.append(target->GetName());
                            announce.append(" and lets out a bellowing roar!");
                            me->MonsterTextEmote(announce.c_str(), 0, true);
                        }
                        massiveCrashStep_Timer = 1000;
                        ++massiveCrashStep;
                        break;

                    case 4:
                        me->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                        massiveCrashStep_Timer = 1500;
                        ++massiveCrashStep;
                        me->MonsterYell("Roar", 0, 0);
                        break;

                    case 5:
                        if (massiveCrashTarget_Guid)
                        {
                            if (Unit* target = ObjectAccessor::GetUnit((*me), massiveCrashTarget_Guid))
                            {
                                if (target->isAlive())
                                {
                                    targetsX = target->GetPositionX();
                                    targetsY = target->GetPositionY();
                                    targetsZ = target->GetPositionZ();
                                    
                                    me->GetMotionMaster()->MoveCharge(2*me->GetPositionX()-targetsX, 2*me->GetPositionY()-targetsY,
                                        395.630f);
                                }
                            }
                        }
                        massiveCrashStep_Timer = 4000;
                        ++massiveCrashStep;
                        break;

                    case 6:
                        chargeStart = true;
                        if (massiveCrashTarget_Guid)
                        {
                            if (targetsX && targetsY && targetsZ)
                                me->GetMotionMaster()->MoveCharge(targetsX, targetsY, targetsZ+2, 42, 1);
                            me->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                        }
                        ++massiveCrashStep;
                        break;

                    case 7:
                        if (!chargeDone)
                            break;

                        chargeStart = false;
                        massiveCrashStart = false;
                        massiveCrashStep = 0;
                        massiveCrashStep_Timer = 1000;
                        massiveCrashTarget_Guid = 0;

                        me->Relocate(me->GetPositionX(), me->GetPositionY(), 395.630f);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->GetMotionMaster()->MoveChase(me->getVictim());

                        me->MonsterYell("Charge end", 0, 0);
                        if (!chargeHit)
                        {
                            DoCast(me, DAZE);
                            me->MonsterTextEmote("Icehowl crashes into the Colliseum wall and is stunned!", 0, true);
                        }
                        else
                        {
                            DoCast(me, BUFF);
                            me->MonsterTextEmote("Icehowl covers boiling rage, and he tramples all in its path!", 0, true);
                        }

                        targetsX = 0;
                        targetsY = 0;
                        targetsZ = 0;

                        chargeHit = false;
                        chargeDone = false;
                        break;
                    }
                } else massiveCrashStep_Timer -= diff;
            }

            while (uint32 id = events.GetEvent())
            {
                switch (id)
                {
                case EVENT_FEROCIOUS_BUTT:
                    DoCast(me->getVictim(), RAID_MODE(FEROCIOUS_BUTT_10N, FEROCIOUS_BUTT_25N, FEROCIOUS_BUTT_10H, FEROCIOUS_BUTT_25H));
                    events.RepeatEvent(FerociousButt_Timer);
                    break;

                case EVENT_ARCTIC_BREATH:
                    DoCast(me->getVictim(), BREATH);
                    events.RepeatEvent(ArcticBreath_Timer);
                    break;

                case EVENT_WHIRL:
                    DoCast(me->getVictim(), RAID_MODE(WHIRL_10N, WHIRL_25N, WHIRL_10H, WHIRL_25H));
                    events.RepeatEvent(Whirl_Timer);
                    break;

                case EVENT_MASSIVE_CRASH:
                    massiveCrashStart = true;
                    massiveCrashStep = 1;
                    events.RepeatEvent(MassiveCrash_Timer);
                    break;
                }
            }

            if (Berserk_Timer < diff)
            {
                DoCast(me, BERSERK);
                Berserk_Timer = 900000;
            } else Berserk_Timer -= diff;


            if (!massiveCrashStart)
                DoMeleeAttackIfReady();
        }
    };

};

class mob_slime_pool : public CreatureScript
{
public:
     mob_slime_pool() : CreatureScript("mob_slime_pool") { }

     CreatureAI* GetAI(Creature* pCreature) const
     {
         return new mob_slime_poolAI(pCreature);
    }

    struct mob_slime_poolAI : public ScriptedAI
    {
         mob_slime_poolAI(Creature *pCreature) : ScriptedAI(pCreature)
         {
         }

         bool casted;

         void Reset()
         {
             casted = false;
             me->SetReactState(REACT_PASSIVE);
             me->ForcedDespawn(60000);
         }

         void UpdateAI(const uint32 uiDiff)
         {
            if (!casted)
             {
                 casted = true;
                 DoCast(me, 66882); //slime pool effect
             }
         }
    };
};

void AddSC_boss_beasts_of_northrend()
{
    new boss_gormok_the_impaler();
    new mob_snobold();
    new boss_acidmaw();
    new boss_dreadscale();
    new boss_icehowl();
    new mob_slime_pool();
}
