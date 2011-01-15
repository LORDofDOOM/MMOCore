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
    FREEZING_SLASH          = 66012,

    SWARM_10N               = 66118,
    SWARM_10H               = 68646,
    SWARM_25N               = 67630,
    SWARM_25H               = 68647,

    COLD_10N                = 66013,
    COLD_10H                = 68509,
    COLD_25N                = 67700,
    COLD_25H                = 68510,

    PERMAFROST_N            = 66193,
    PERMAFROST_H            = 67856,

    PURSUE                  = 67574,
    SPIKE                   = 65922,


    SUBMERGE_ANUB           = 65981,
    SUBMERGE_BURROWER       = 68394,
    BERSERK                 = 26662,


    //spike
    SPELL_SUMMON_SPIKE      = 66169,
    SPELL_SPIKE_SPEED1      = 65920,
    SPELL_SPIKE_TRAIL       = 65921,
    SPELL_SPIKE_SPEED2      = 65922,
    SPELL_SPIKE_SPEED3      = 65923,
    SPELL_SPIKE_FAIL        = 66181,
    SPELL_SPIKE_TELE        = 66170,

    SPELL_MARK              = 67574,
    SPELL_SPIKE_CALL        = 66169,

    //frost sphere
    SPELL_FROST_SPHERE      = 67539,
    SPELL_PERMAFROST        = 66193,
    SPELL_PERMAFROST_VISUAL = 65882,

    //adds
    NPC_FROST_SPHERE     = 34606,
    NPC_BURROW           = 34862,
    NPC_SPIKE            = 34660
};

enum Says
{
    SAY_GREET               = -1760069,
    SAY_AGGRO               = -1760070,
    SAY_SUBMERGE            = -1760071,
    SAY_SWARM               = -1760072,
    SAY_KILLING_1           = -1760073,
    SAY_KILLING_2           = -1760074,
    SAY_DEATH               = -1760075
};

enum Events
{
    EVENT_NULL,
    EVENT_COLD,
    EVENT_FREEZING_SLASH,
    EVENT_SPIKE
};

typedef std::list<HostileReference*> ThreatList;

const Position spawnPositions[6]         =
{
    { 735.114990f, 191.412994f, 142.117996f, 4.971080f },
    { 696.405273f, 186.296097f, 142.117996f, 5.434452f },
    { 686.743835f, 98.430656f, 142.120285f, 0.884638f },
    { 711.423523f, 78.581139f, 142.120285f, 0.708715f },
    { 756.423462f, 90.765480f, 142.124878f, 1.305618f },
    { 784.310913f, 171.513092f, 142.120331f, 4.325483f }
};

const Position SphereSpawn[6] =
{
    { 786.6439f, 108.2498f, 155.6701f },
    { 806.8429f, 150.5902f, 155.6701f },
    { 759.1386f, 163.9654f, 155.6701f },
    { 744.3701f, 119.5211f, 155.6701f },
    { 710.0211f, 120.8152f, 155.6701f },
    { 706.6383f, 161.5266f, 155.6701f },
};

#define SAY_KILLING                 RAND(SAY_KILLING_1, SAY_KILLING_2)
#define SPELL_PERMAFROST            RAID_MODE(PERMAFROST_N, PERMAFROST_N, PERMAFROST_H, PERMAFROST_H)

#define Cold_Timer                  18000
#define FreezingSlash_Timer         20000
#define Spike_Timer                 20000

class boss_anubarak_trial : public CreatureScript
{
public:
    boss_anubarak_trial() : CreatureScript("boss_anubarak_trial") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_anubarakAI(creature);
    }

    struct boss_anubarakAI : public BossAI
    {
        boss_anubarakAI(Creature* creature) : BossAI(creature, DATA_ANUBARAK)
        {
            greet = false;
            if (me->GetMap()->GetId() == 649)
            {
                //Spawn Frost Spheres
                for (int i=0; i < 6; i++)
                    if (Unit *pSummoned = me->SummonCreature(34606, SphereSpawn[i])) //frost sphere
                        m_aSphereGUID[i] = pSummoned->GetGUID();

            }
        };

        uint32 phase;

        uint32 SwarmScarab_Timer;
        uint32 Burrower_Timer;

        uint32 Burrow_Timer;
        bool burrowed;

        uint32 tick_Timer;

        std::vector<uint64> ColdTargets;
        bool greet;

        uint64 m_aSphereGUID[6];
        uint32 summonFrostSphereTimer;

        void Reset()
        {
            _Reset();

            phase = 1;
            SwarmScarab_Timer = 5000;
            Burrower_Timer = 20000;
            Burrow_Timer = 90000;
            burrowed = false;
            tick_Timer = 1000;

            me->RemoveAllAuras();
            me->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);

            instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(PERMAFROST_N, PERMAFROST_N, PERMAFROST_H, PERMAFROST_H));
            ColdTargets.resize(5);

            summonFrostSphereTimer = urand(20,30)*IN_MILLISECONDS;
        }

        void JustReachedHome()
        {
            instance->SetData(RESET_TO_FIGHT, DATA_ANUBARAK);
        }

        void EnterCombat(Unit* who)
        {
            _EnterCombat();

            DoScriptText(SAY_AGGRO, me);

            events.ScheduleEvent(EVENT_COLD, 5000);
            events.ScheduleEvent(EVENT_FREEZING_SLASH, FreezingSlash_Timer);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER)
                DoScriptText(SAY_KILLING, me);


            if (who->GetTypeId() == TYPEID_PLAYER)
                instance->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void JustDied(Unit* who)
        {
            _JustDied();

            DoScriptText(SAY_DEATH, me);
            instance->DoRemoveAurasDueToSpellOnPlayers(RAID_MODE(PERMAFROST_N, PERMAFROST_N, PERMAFROST_H, PERMAFROST_H));
        }

        void JustSummoned(Creature* pSummoned)
        {
            Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true);
            switch(pSummoned->GetEntry())
            {
                case 34862: //burrower
                    pSummoned->CastSpell(pSummoned,66969,false); //churning ground
                    break;
                case 34660: //spike
                    pSummoned->CombatStart(pTarget);
                    break;
            }
            summons.Summon(pSummoned);
        }




        void UpdateAI(const uint32 diff)
        {
            if (!greet)
            {
                Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
                if (!PlayerList.isEmpty())
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        if (Player* pPlayer = i->getSource())
                            if (!pPlayer->isGameMaster() && me->GetDistance(pPlayer) <= 75.0f)
                            {
                                DoScriptText(SAY_GREET, me);
                                greet = true;
                                break;
                            }
            }

            if (!UpdateVictim())
                return;

            if (phase != 3)
            {
                if (SwarmScarab_Timer < diff)
                {
                    //spell 66340
                    if (Creature* scarab = DoSummon(NPC_SCARAB, spawnPositions[urand(0,5)]))
                    {
                        if (phase != 2)
                            scarab->GetMotionMaster()->MoveRandom(30.0f);
                        else
                        {
                            scarab->setFaction(me->getFaction());
                            scarab->AI()->AttackStart(SelectUnit(SELECT_TARGET_RANDOM, 0));
                        }
                    }
                    SwarmScarab_Timer = 10000;
                } else SwarmScarab_Timer -= diff;
            }

            if (phase == 1 || (RAID_MODE(0,0,1,1) && phase ==3))
            {
                if (Burrower_Timer < diff)
                {
                    for (int i = 0; i < RAID_MODE(1,2,2,4); ++i)
                    {
                        if (Creature* burrower = DoSummon(NPC_BURROWER, spawnPositions[urand(0,5)]))
                            burrower->AI()->AttackStart(SelectUnit(SELECT_TARGET_RANDOM, 0));
                    }

                    Burrower_Timer = urand(55000, 65000);
                } else Burrower_Timer -= diff;
            }

            /*if (phase == 3)          -> already handle in core
            {
                if (tick_Timer < diff)
                {
                    if (me->GetMap()->HavePlayers())
                    {
                        uint32 healammount=0;
                        Map::PlayerList const& list = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            Player* plr = itr->getSource();
                            if (!plr || !plr->isAlive() || !plr->HasAura(RAID_MODE(SWARM_10N, SWARM_25N, SWARM_10H, SWARM_25H)) || plr->isGameMaster())
                                continue;

                            uint8 percent = RAID_MODE(10, 10, 20, 30);
                            uint32 dmg = plr->GetHealth()*percent/100;

                            if (dmg < 250)
                                dmg = 250;

                            me->DealDamage(plr, dmg);
                            healammount+=dmg;
                        }

                        if (healammount)
                            me->DealHeal(me, healammount, GetSpellStore()->LookupEntry(RAID_MODE(SWARM_10N, SWARM_25N, SWARM_10H, SWARM_25H)));

                    }
                    tick_Timer = 1000;
                } else tick_Timer -= diff;
            }*/

            events.Update(diff);
            UpdateBurrowers();

            if (!burrowed && phase != 3)
            {
                if (Burrow_Timer < diff)
                {
                    DoScriptText(SAY_SUBMERGE, me);
                    DoCast(me, SUBMERGE_ANUB);

                    burrowed = true;
                    SwitchPhase(2);

                    Burrow_Timer = 60000;
                } else Burrow_Timer -= diff;
            }
            else if (phase != 3)
            {
                if (Burrow_Timer < diff)
                {
                    me->RemoveAurasDueToSpell(SUBMERGE_ANUB);
                    DoCast(me, 65982); //emerge

                    burrowed = false;
                    SwitchPhase(1);

                    Burrow_Timer = 90000;
                } else Burrow_Timer -= diff;
            }

            if (HealthBelowPct(31) && phase != 3)
                SwitchPhase(3);

            while (uint32 id = events.GetEvent())
            {
                switch (id)
                {
                case EVENT_COLD:
                    DoColdEvent();
                    me->CastCustomSpell(RAID_MODE(COLD_10N, COLD_25N, COLD_10H, COLD_25H), SPELLVALUE_MAX_TARGETS, RAID_MODE(2, 5, 2, 5));
                    events.RepeatEvent(Cold_Timer);
                    break;

                case EVENT_FREEZING_SLASH:
                    DoCast(me->getVictim(), FREEZING_SLASH);
                    events.RepeatEvent(FreezingSlash_Timer);
                    break;

                case EVENT_SPIKE:
                    DoCast(SPELL_SPIKE_CALL);
                    events.RepeatEvent(90000); //shouldn't be more than one per phase
                    break;

                default:
                    events.PopEvent();
                }
            }

            if (!IsHeroic())
            {
                if (summonFrostSphereTimer <= diff)
                {
                    uint8 startAt = urand(0,5);
                    uint8 i = startAt;
                    do
                    {
                        if (Unit *pSphere = Unit::GetCreature(*me,m_aSphereGUID[i]))
                        {
                            if (!pSphere->HasAura(SPELL_FROST_SPHERE))
                            {
                                if (Creature *pSummon = me->SummonCreature(NPC_FROST_SPHERE,SphereSpawn[i]))
                                    m_aSphereGUID[i] = pSummon->GetGUID();
                                break;
                            }
                        }
                        i = (i+1)%6;
                    } while (i != startAt);
                    summonFrostSphereTimer = urand(20,30)*IN_MILLISECONDS;
                } else summonFrostSphereTimer -= diff;
            }


            if (!burrowed)
                DoMeleeAttackIfReady();
        }




        void SwitchPhase(int i)
        {
            events.Reset();

            switch (i)
            {
            case 1:
                phase = 1;
                events.ScheduleEvent(EVENT_COLD, Cold_Timer);
                events.ScheduleEvent(EVENT_FREEZING_SLASH, FreezingSlash_Timer);
                me->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                DoCast(me, 66170); //spike tele
                summons.DespawnEntry(34660); //spike
                break;

            case 2:
                phase = 2;
                events.ScheduleEvent(EVENT_SPIKE, Spike_Timer);
                me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                for (SummonList::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
                {
                    if (Creature* scarab = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), (*itr)))
                    {
                        if (scarab->isAlive() && scarab->GetEntry() == NPC_SCARAB)
                        {
                            scarab->setFaction(me->getFaction());
                            scarab->AI()->AttackStart(SelectUnit(SELECT_TARGET_RANDOM, 0));
                        }
                    }
                }
                break;

            case 3:
                phase = 3;
                DoCast(me, RAID_MODE(SWARM_10N, SWARM_25N, SWARM_10H, SWARM_25H));
                events.ScheduleEvent(EVENT_COLD, Cold_Timer);
                events.ScheduleEvent(EVENT_FREEZING_SLASH, FreezingSlash_Timer);
                break;
            }
        }

        void DoColdEvent()
        {
            return;

            std::vector<uint64> newTargets;

            ThreatList const& list = me->getThreatManager().getThreatList();
            uint32 size = list.size();

            ThreatList::const_iterator itr = list.begin();
            while (itr != list.end())
            {
                Unit* unit = ObjectAccessor::GetUnit((*me), (*itr)->getUnitGuid());
                if (!unit || !unit->isAlive())
                {
                    --size;
                }
                else
                {
                    uint64 guid = unit->GetGUID();

                    bool ok = true;

                    //if in boss' threat list is only 5 targtes of less, add them anyway
                    if (size > 5 && !ColdTargets.empty())
                    {
                        //if more, add only if prevoiusly wasn't targeted
                        for (int i = 0; i < (int)ColdTargets.size(); ++i)
                        {
                            if (ColdTargets[i] == guid)
                                ok = false;
                        }
                    }

                    if (ok)
                        newTargets.push_back(guid);

                }

                ++itr;
            }

            if (!newTargets.empty())
            {
                //if we have less than 5 targets (if threat list have more than 5 positions but less than 10)
                if (newTargets.size() < 5 && size > 5)
                {
                    do
                    {
                        uint64 guid = SelectUnit(SELECT_TARGET_RANDOM, 0)->GetGUID();
                        bool ok = true;

                        for (int i = 0; i < (int)newTargets.size(); ++i)
                        {
                            if (newTargets[i] == guid)
                                ok = false;
                        }

                        if (ok)
                            newTargets.push_back(guid);
                    }
                    while (newTargets.size() < 5);
                }

                ColdTargets.clear(); //na wszelki wypadek :)
                ColdTargets = newTargets;
            }

            for (int i = 0; i < (int)ColdTargets.size(); ++i)
            {
                Unit* unit = ObjectAccessor::GetUnit((*me), ColdTargets[i]);
                DoCast(unit, RAID_MODE(COLD_10N, COLD_25N, COLD_10H, COLD_25H));
            }
        }

        void UpdateBurrowers()
        {
            if (summons.empty() || summons.size() == 1)
                return;

            for (SummonList::const_iterator itr = summons.begin(); itr != summons.end(); ++itr)
            {
                uint32 count = 0;
                Creature* burrower = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), (*itr));
                if (!burrower || !burrower->isAlive())
                    continue;
                if (burrower->GetEntry() != NPC_BURROWER)
                    continue;

                for (SummonList::const_iterator itr2 = summons.begin(); itr2 != summons.end(); ++itr2)
                {
                    Creature* burrower2 = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), (*itr));
                    if (!burrower2 || !burrower2->isAlive())
                        continue;
                    if (burrower2->GetEntry() != NPC_BURROWER)
                        continue;
                    if (burrower2->GetGUID() == burrower->GetGUID())
                        continue;

                    if (burrower->GetDistance2d(burrower2) <= 12.0f)
                        ++count;
                }

                if (count)
                {
                    if (burrower->HasAura(66129))
                    {
                        while (burrower->GetAura(66129)->GetStackAmount() < count)
                            burrower->CastSpell(burrower, 66129, false);
                    }
                    else
                    {
                        if (Aura* aura = burrower->AddAura(66129, burrower))
                            aura->SetStackAmount(count);
                    }
                }
            }
        }
    };

};




class mob_frost_sphere : public CreatureScript
{
public:
    mob_frost_sphere() : CreatureScript("mob_frost_sphere") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_frost_sphereAI(pCreature);
    };

    struct mob_frost_sphereAI : public ScriptedAI
    {
        mob_frost_sphereAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
        }

        bool   m_bFall;
        uint32 m_uiPermafrostTimer;

        void Reset()
        {
            m_bFall = false;
            m_uiPermafrostTimer = 0;
            me->SetReactState(REACT_PASSIVE);
            me->SetFlying(true);
            me->SetDisplayId(25144);
            me->SetSpeed(MOVE_RUN, 0.5, false);
            me->GetMotionMaster()->MoveRandom(20.0f);
            DoCast(67539); //frost sphere
        }

        void DamageTaken(Unit* pWho, uint32& uiDamage)
        {
            if (me->GetHealth() < uiDamage)
            {
                uiDamage = 0;
                if (!m_bFall)
                {
                    m_bFall = true;
                    me->SetFlying(false);
                    me->GetMotionMaster()->MoveIdle();
                    me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
                    //At hit the ground
                    me->GetMotionMaster()->MoveFall(142.2f, 0);
                    //me->FallGround(); //need correct vmap use (i believe it isn't working properly right now)
                }
            }
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if(uiType != POINT_MOTION_TYPE) return;


            switch (uiId)
            {
                case 0:
                    m_uiPermafrostTimer = IN_MILLISECONDS;
                    break;
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (m_uiPermafrostTimer)
            {
                if (m_uiPermafrostTimer <= uiDiff)
                {
                    m_uiPermafrostTimer = 0;
                    me->RemoveAurasDueToSpell(67539); //frost sphere
                    me->SetDisplayId(11686);
                    me->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
                    DoCast(me, SPELL_PERMAFROST_VISUAL);
                    DoCast(me, SPELL_PERMAFROST);
                } else m_uiPermafrostTimer -= uiDiff;
            }
        }
    };

};




class mob_nerubian_burrower : public CreatureScript
{
public:
    mob_nerubian_burrower() : CreatureScript("mob_nerubian_burrower") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_nerubian_burrowerAI(creature);
    }

    struct mob_nerubian_burrowerAI : public ScriptedAI
    {
        mob_nerubian_burrowerAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        bool tryToBurrow;
        uint32 exposeTimer;
        uint32 shadowStrikeTimer;
        uint32 heal_Timer;

        void Reset()
        {
            tryToBurrow = false;
            exposeTimer = me->GetAttackTime(BASE_ATTACK);
            shadowStrikeTimer = 30000;
            heal_Timer = 1000;
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if ((me->GetHealth()-damage)*100/me->GetMaxHealth() <= 10)
                tryToBurrow = true;
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

            if (tryToBurrow && !me->HasAura(RAID_MODE(PERMAFROST_N, PERMAFROST_N, PERMAFROST_H, PERMAFROST_H)))
            {
                DoCast(me, SUBMERGE_BURROWER);
                me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                tryToBurrow = false;
            }

            if (me->HasAura(SUBMERGE_BURROWER))
            {
                if (heal_Timer < diff)
                {
                    me->SetHealth(me->GetHealth()+me->GetMaxHealth()*10/100);
                    if (me->GetHealth() == me->GetMaxHealth())
                    {
                        me->RemoveAurasDueToSpell(SUBMERGE_BURROWER);
                        DoCast(me, 66947, false); //emerge
                        me->RemoveFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                    }
                    heal_Timer = 1000;
                } else heal_Timer -= diff;
            }

            if (me->HasAura(SUBMERGE_BURROWER))
                return;

            if (me->IsWithinMeleeRange(me->getVictim()) && roll_chance_i(66))
            {
                if (exposeTimer < diff)
                {
                    DoCast(me->getVictim(), 67847);
                    exposeTimer = me->GetAttackTime(BASE_ATTACK);
                } else exposeTimer -= diff;
            }

            if (getDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC || getDifficulty() == RAID_DIFFICULTY_25MAN_HEROIC)
            {
                if (shadowStrikeTimer < diff)
                {
                    DoCast(SelectUnit(SELECT_TARGET_RANDOM, 0), 66134);
                    shadowStrikeTimer = 30000;
                } else shadowStrikeTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

};



class mob_swarm_scarab : public CreatureScript
{
public:
    mob_swarm_scarab() : CreatureScript("mob_swarm_scarab") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_swarm_scarabAI(creature);
    }

    struct mob_swarm_scarabAI : public ScriptedAI
    {
        mob_swarm_scarabAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 poisonTimer;
        bool roll;

        void Reset()
        {
            roll = false;
            poisonTimer = me->GetAttackTime(BASE_ATTACK);
        }

        void KilledUnit(Unit* who)
        {
            if (who->GetTypeId() == TYPEID_PLAYER && me->GetInstanceScript())
                me->GetInstanceScript()->SetData(UPDATE_DEATHS_COUNT, 0);
        }

        void JustDied(Unit* who)
        {
            if (me->GetInstanceScript())
                me->GetInstanceScript()->SetData(SCARAB_DEATH, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (poisonTimer < diff)
            {
                //50% chance
                if (urand(0,1))
                    DoCast(me->getVictim(), RAID_MODE(65775, 67861, 67862, 67863));
                poisonTimer = me->GetAttackTime(BASE_ATTACK);
            } else poisonTimer -= diff;


            if (!roll && HealthBelowPct(30))
            {
                if (roll_chance_i(33))
                    DoCast(me, 66092);
                roll = true;
            }

            DoMeleeAttackIfReady();
        }
    };

};



class mob_anubarak_spike : public CreatureScript
{
public:
    mob_anubarak_spike() : CreatureScript("mob_anubarak_spike") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_anubarak_spikeAI(pCreature);
    };

    struct mob_anubarak_spikeAI : public ScriptedAI
    {
        mob_anubarak_spikeAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }

        InstanceScript* m_pInstance;
        uint32 m_uiIncreaseSpeedTimer;
        uint8  m_uiSpeed;
        uint64 m_uiTargetGUID;

        void Reset()
        {
            // For an unknown reason this npc isn't recognize the Aura of Permafrost with this flags =/
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
            m_uiTargetGUID = 0;
        }

        void EnterCombat(Unit *pWho)
        {
            m_uiTargetGUID = pWho->GetGUID();
            DoCast(pWho,67574); //mark
            me->SetSpeed(MOVE_RUN, 0.5f);
            m_uiSpeed = 0;
            m_uiIncreaseSpeedTimer = 1*IN_MILLISECONDS;
            me->TauntApply(pWho);
        }

        void DamageTaken(Unit* pWho, uint32& uiDamage)
        {
            uiDamage = 0;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            Unit* pTarget = Unit::GetPlayer(*me, m_uiTargetGUID);
            if (!pTarget || !pTarget->isAlive() || !pTarget->HasAura(67574)) //mark
            {
                if (Creature* pAnubarak = Unit::GetCreature((*me),m_pInstance->GetData64(NPC_ANUBARAK)))
                    pAnubarak->CastSpell(pAnubarak,66170,false); //spike tele
                me->DisappearAndDie();
                return;
            }

            if (m_uiIncreaseSpeedTimer)
                if (m_uiIncreaseSpeedTimer <= uiDiff)
                {
                    switch (m_uiSpeed)
                    {
                        case 0:
                            DoCast(me,65920); //spike speed 1
                            DoCast(me,65921); //spike trail
                            m_uiSpeed = 1;
                            m_uiIncreaseSpeedTimer = 7*IN_MILLISECONDS;
                            break;
                        case 1:
                            DoCast(me,65922); //spike speed 2
                            m_uiSpeed = 2;
                            m_uiIncreaseSpeedTimer = 7*IN_MILLISECONDS;
                            break;
                        case 2:
                            DoCast(me,65923); //spike speed 3
                            m_uiIncreaseSpeedTimer = 0;
                            break;
                    }
                } else m_uiIncreaseSpeedTimer -= uiDiff;
        }
    };

};



void AddSC_boss_anubarak_trial()
{
    new boss_anubarak_trial();
    new mob_frost_sphere();
    new mob_nerubian_burrower();
    new mob_swarm_scarab();
    new mob_anubarak_spike();
}
