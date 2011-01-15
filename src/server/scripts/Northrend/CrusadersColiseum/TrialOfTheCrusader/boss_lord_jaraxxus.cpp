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
    BUFF                = 67108,
    FELL_LIGHTING       = 67888,
    BERSERK             = 64238,
    TOUCH               = 66209,

    FEL_FIREBALL_10N    = 66532,
    FEL_FIREBALL_10H    = 66964,
    FEL_FIREBALL_25N    = 66963,
    FEL_FIREBALL_25H    = 66965,

    FEL_LIGHTING_10N    = 66528,
    FEL_LIGHTING_10H    = 67030,
    FEL_LIGHTING_25N    = 67029,
    FEL_LIGHTING_25H    = 67031,

    INICIRATE_10N       = 66237,
    INICIRATE_10H       = 67050,
    INICIRATE_25N       = 67049,
    INICIRATE_25H       = 67051,

    ERRUPTION_10N       = 66258,
    ERRUPTION_10H       = 67902,
    ERRUPTION_25N       = 67901,
    ERRUPTION_25H       = 67903,

    LEGION_FLAME_10N    = 66197,
    LEGION_FLAME_10H    = 68124,
    LEGION_FLAME_25N    = 68123,
    LEGION_FLAME_25H    = 68125,

    PORTAL_10N          = 66269,
    PORTAL_10H          = 67899,
    PORTAL_25N          = 67898,
    PORTAL_25H          = 67900
};

enum Says
{
    SAY_AGGRO           = -1760014,
    SAY_INICIRATE       = -1760015,
    SAY_INICIRATE_EMOTE = -1760016,
    SAY_SISTER          = -1760017,
    SAY_INFERNUS        = -1760018,
    SAY_KILLING1        = -1760019,
    SAY_KILLING2        = -1760020,
    SAY_DEATH           = -1760021
};


enum Events
{
    EVENT_NULL,

    EVENT_INICIRATE_FLESH,
    EVENT_BUFF,
    EVENT_FEL_FIREBALL,
    EVENT_FEL_LIGHTING,
    EVENT_TOUCH_OF_JARAXXUS,
    EVENT_LEGION_FLAME,
    EVENT_SUMMON_P,
    EVENT_SUMMON_V
};

#define SAY_KILLING         RAND(SAY_KILLING1, SAY_KILLING2)

#define Inicirate_Timer         urand(25000, 35000)
#define Buff_Timer              urand(40000, 45000)
#define Summon_Timer            urand(30000, 45000)
#define LegionFlame_Timer       urand(20000, 30000)
#define FelFireball_Timer       10000
#define FelLighting_Timer       22000
#define Touch_Timer             urand(27000, 30000)


class boss_lord_jaraxxus : public CreatureScript
{
public:
    boss_lord_jaraxxus() : CreatureScript("boss_lord_jaraxxus") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_lord_jaraxxusAI(creature);
    }

    struct boss_lord_jaraxxusAI : public BossAI
    {
        boss_lord_jaraxxusAI(Creature* creature) : BossAI(creature, DATA_LORD_JARAXXUS)
        {
        }

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit* who)
        {
            if (who->GetTypeId() != TYPEID_PLAYER)
            {
                _EnterCombat();
                return;
            }

            events.ScheduleEvent(EVENT_INICIRATE_FLESH, Inicirate_Timer);
            events.ScheduleEvent(EVENT_BUFF, Buff_Timer);
            events.ScheduleEvent(EVENT_FEL_FIREBALL, FelFireball_Timer);
            events.ScheduleEvent(EVENT_FEL_LIGHTING, FelLighting_Timer);
            events.ScheduleEvent(EVENT_LEGION_FLAME, LegionFlame_Timer);
            events.ScheduleEvent(EVENT_SUMMON_V, Summon_Timer-urand(15000, 20000));
            events.ScheduleEvent(EVENT_SUMMON_P, Summon_Timer-urand(15000, 20000));
            if (getDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || getDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
                events.ScheduleEvent(EVENT_TOUCH_OF_JARAXXUS, Touch_Timer);

            if (Aura* aura = me->AddAura(BUFF, me))
                aura->ModStackAmount(RAID_MODE(5, 10, 5, 10));

            DoScriptText(SAY_AGGRO, me);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
            {
                DoScriptText(SAY_KILLING, me);
                instance->SetData(UPDATE_DEATHS_COUNT, 0);
            }
        }

        void JustDied(Unit* who)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);
        }

        void JustReachedHome()
        {
            instance->SetData(RESET_TO_FIGHT, DATA_LORD_JARAXXUS);
        }


        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!me->IsNonMeleeSpellCasted(true))
                me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);

            if (!me->IsNonMeleeSpellCasted(true))
                events.Update(diff);

            while (uint32 id = events.GetEvent())
            {
                switch (id)
                {
                case EVENT_BUFF:
                    if (Aura* aura = me->GetAura(BUFF))
                        aura->ModStackAmount(aura->GetStackAmount()+RAID_MODE(5, 10, 5, 10));
                    else if (Aura* aura = me->AddAura(BUFF, me))
                        aura->ModStackAmount(RAID_MODE(5, 10, 5, 10));

                    events.RepeatEvent(Buff_Timer);
                    break;

                case EVENT_FEL_FIREBALL:
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, false);
                    DoCast(me->getVictim(), RAID_MODE(FEL_FIREBALL_10N, FEL_FIREBALL_25N, FEL_FIREBALL_10H, FEL_FIREBALL_25H));
                    events.RepeatEvent(FelFireball_Timer);
                    break;

                case EVENT_FEL_LIGHTING:
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), RAID_MODE(FEL_LIGHTING_10N, FEL_LIGHTING_25N, FEL_LIGHTING_10H, FEL_LIGHTING_25H));
                    events.RepeatEvent(FelLighting_Timer);
                    break;

                case EVENT_INICIRATE_FLESH:
                    DoScriptText(SAY_INICIRATE, me);
                    if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                    {
                        DoCast(target, RAID_MODE(INICIRATE_10N, INICIRATE_25N, INICIRATE_10H, INICIRATE_25H));
                        DoScriptText(SAY_INICIRATE_EMOTE, me, target);
                    }
                    events.RepeatEvent(Inicirate_Timer);
                    break;

                case EVENT_LEGION_FLAME:
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), RAID_MODE(LEGION_FLAME_10N, LEGION_FLAME_25N, LEGION_FLAME_10H, LEGION_FLAME_25H));
                    events.RepeatEvent(LegionFlame_Timer);
                    break;

                case EVENT_SUMMON_V:
                    DoErruptionEvent();
                    events.RescheduleEvent(EVENT_SUMMON_P, Summon_Timer-10000);
                    events.RepeatEvent(Summon_Timer);
                    break;

                case EVENT_SUMMON_P:
                    DoPortalEvent();
                    events.RescheduleEvent(EVENT_SUMMON_V, Summon_Timer-10000);
                    events.RepeatEvent(Summon_Timer);
                    break;

                case EVENT_TOUCH_OF_JARAXXUS:
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), TOUCH);
                    events.RepeatEvent(Touch_Timer);
                    break;

                default:
                    events.PopEvent();
                }
            }


            DoMeleeAttackIfReady();
        }


        void DoErruptionEvent()
        {
            DoScriptText(SAY_INFERNUS, me);
            float x = 0, y = 0, z = 0;
            SelectUnit(SELECT_TARGET_RANDOM, 0)->GetClosePoint(x, y, z, 1, 20);
            if (x && y && z)
                me->CastSpell(x, y, z, RAID_MODE(ERRUPTION_10N, ERRUPTION_25N, ERRUPTION_10H, ERRUPTION_25H), false);//summon vulcan
        }

        void DoPortalEvent()
        {
            DoScriptText(SAY_SISTER, me);
            float x = 0, y = 0, z = 0;

            SelectUnit(SELECT_TARGET_RANDOM, 0)->GetClosePoint(x, y, z, 1, 20);
            if (x && y && z)
                me->CastSpell(x, y, z, RAID_MODE(PORTAL_10N, PORTAL_25N, PORTAL_10H, PORTAL_25H), false);//summon portal
        }
    };

};



class mob_jaraxxus_portal : public CreatureScript
{
public:
    mob_jaraxxus_portal() : CreatureScript("mob_jaraxxus_portal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_jaraxxus_portalAI(creature);
    }

    struct mob_jaraxxus_portalAI : public ScriptedAI
    {
        mob_jaraxxus_portalAI(Creature* creature) : ScriptedAI(creature)
        {
            summon = false;
            buff = false;
            Summoning_Timer = 8500;
            creature->SetReactState(REACT_PASSIVE);

            if (!getDifficulty() || getDifficulty() == 1)
                me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        }

        bool summon;
        bool buff;
        uint32 Summoning_Timer;

        void UpdateAI(const uint32 diff)
        {
            if (!me->GetInstanceScript())
                return;

            if (!ObjectAccessor::GetCreatureOrPetOrVehicle((*me), me->GetInstanceScript()->GetData64(BOSS_JARAXXUS)))
                return;

            if (!buff)
            {
                DoCast(me, 67103);
                buff = true;
            }

            if (summon && (!getDifficulty() || getDifficulty() == 1)) //normal 10man and normal 25man
            {
                me->ForcedDespawn(1500);
                return;
            }

            if (Summoning_Timer < diff)
            {
                summon = true;
                Summoning_Timer = 8500;
                if (getDifficulty() == 2 || getDifficulty() == 3)
                    DoCast(me, 67103);
            } else Summoning_Timer -= diff;
        }
    };

};



class mob_jaraxxus_vulcan : public CreatureScript
{
public:
    mob_jaraxxus_vulcan() : CreatureScript("mob_jaraxxus_vulcan") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_jaraxxus_vulcanAI(creature);
    }

    struct mob_jaraxxus_vulcanAI : public ScriptedAI
    {
        mob_jaraxxus_vulcanAI(Creature* creature) : ScriptedAI(creature)
        {
            summon = false;
            Summoning_Timer = 2000;
            count = 0;
            creature->SetReactState(REACT_PASSIVE);

            if (!getDifficulty() || getDifficulty() == 1)
                me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        }

        bool summon;
        uint32 Summoning_Timer;
        uint8 count;

        void UpdateAI(const uint32 diff)
        {
            if (!me->GetInstanceScript())
                return;

            if (!ObjectAccessor::GetCreatureOrPetOrVehicle((*me), me->GetInstanceScript()->GetData64(BOSS_JARAXXUS)))
                return;

            if (summon && (!getDifficulty() || getDifficulty() == 1)) //normal 10man and normal 25man)
            {
                me->ForcedDespawn(1500);
                return;
            }

            if (Summoning_Timer < diff)
            {
                ++count;
                float x,y,z = 0;
                me->GetClosePoint(x, y, z, 15.0f); //summon infernal in range of 15.0f from vuclano
                ObjectAccessor::GetUnit((*me), me->GetInstanceScript()->GetData64(BOSS_JARAXXUS))->CastSpell(x, y, z, 66255, false);
                Summoning_Timer = 5000;
            } else Summoning_Timer -= diff;

            if (count >= 3) //if we have summoned 3 infernal, make longest intervale
            {
                Summoning_Timer = 15000;
                count = 0;
            }
        }
    };

};



class mob_felflame_infernal : public CreatureScript
{
public:
    mob_felflame_infernal() : CreatureScript("mob_felflame_infernal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_felflame_infernalAI(creature);
    }

    struct mob_felflame_infernalAI : public ScriptedAI
    {
        mob_felflame_infernalAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 channel_Timer;
        uint32 charge_Timer;

        void Reset()
        {
            channel_Timer = 10000;
            charge_Timer = 50000;
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && me->GetInstanceScript())
                me->GetInstanceScript()->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (channel_Timer < diff)
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), 67047);
                channel_Timer = 25000;
            } else channel_Timer -= diff;

            if (charge_Timer < diff)
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), 66494);
                charge_Timer = 15000;
            } else charge_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};



class mob_misstress_of_pain : public CreatureScript
{
public:
    mob_misstress_of_pain() : CreatureScript("mob_misstress_of_pain") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_misstress_of_painAI(creature);
    }

    struct mob_misstress_of_painAI : public ScriptedAI
    {
        mob_misstress_of_painAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 leep_Timer;
        uint32 whirl_Timer;
        uint32 kiss_Timer;

        void Reset()
        {
            leep_Timer = 15000;
            whirl_Timer = 10000;
            kiss_Timer = urand(10000, 20000);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && me->GetInstanceScript())
                me->GetInstanceScript()->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (leep_Timer < diff)
            {
                DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), 66283);
                leep_Timer = 25000;
            } else leep_Timer -= diff;

            if (whirl_Timer < diff)
            {
                DoCast(me->getVictim(), RAID_MODE(66378, 67098, 67097, 67099));
                whirl_Timer = 20000;
            } else whirl_Timer -= diff;

            if (getDifficulty() == 2 || getDifficulty() == 3)
            {
                if (kiss_Timer < diff)
                {
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), RAID_MODE(0, 0, 67077, 67078));
                    kiss_Timer = urand(10000, 20000);
                } else kiss_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};



class mob_legion_flame : public CreatureScript
{
public:
    mob_legion_flame() : CreatureScript("mob_legion_flame") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_legion_flameAI(creature);
    }

    struct mob_legion_flameAI : public ScriptedAI
    {
        mob_legion_flameAI(Creature* creature) : ScriptedAI(creature)
        {
            tick_Timer = 1000;
        }

        uint32 tick_Timer;

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
            me->SetInCombatWithZone();
            DoCast(66201); //legion flame effect
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && me->GetInstanceScript())
                me->GetInstanceScript()->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
        }
    };

};


void AddSC_boss_lord_jaraxxus()
{
    new boss_lord_jaraxxus();
    new mob_jaraxxus_portal();
    new mob_jaraxxus_vulcan();
    new mob_felflame_infernal();
    new mob_misstress_of_pain();
    new mob_legion_flame();
}
