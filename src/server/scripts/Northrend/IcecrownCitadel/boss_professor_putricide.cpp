/*
* Copyright (C) 2009 - 2010 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "icecrown_citadel.h"

enum eScriptTexts
{
    // Festergut
    SAY_FESTERGUT_GASEOUS_BLIGHT    = 0,
    SAY_FESTERGUT_DEATH             = 1,

    // Rotface
    SAY_ROTFACE_OOZE_FLOOD          = 2,
    SAY_ROTFACE_DEATH               = 3,

    // Professor Putricide
    SAY_AGGRO                       = 4,
    EMOTE_UNSTABLE_EXPERIMENT       = 5,
    SAY_PHASE_TRANSITION_HEROIC     = 6,
    SAY_TRANSFORM_1                 = 7,
    SAY_TRANSFORM_2                 = 8,    // always used for phase2 change, DO NOT GROUP WITH SAY_TRANSFORM_1
    EMOTE_MALLEABLE_GOO             = 9,
    EMOTE_CHOKING_GAS_BOMB          = 10,
    SAY_KILL                        = 11,
    SAY_BERSERK                     = 12,
    SAY_DEATH                       = 13
}; 
//enum eBrothersYells
//{
//    SAY_FESTERGUT_GASEOUS_BLIGHT = -1631080,
//    SAY_FESTERGUT_DEATH          = -1631090,
//
//    SAY_ROTFACE_DEATH            = -1666025
//};
enum ePoints
{
    POINT_FESTERGUT = 366260,
    POINT_ROTFACE   = 366270,
    POINT_TABLE     = 366780,
};
enum eBrothersSpells
{
    SPELL_RELEASE_GAS_VISUAL    = 69125,
    SPELL_MALLABLE_GOO_H        = 70852,

    SPELL_OOZE_FLOOD_H          = 69783,
    SPELL_VILE_GAS_H            = 72272
};

//enum Yells
//{
//    SAY_AGGRO         = -1631104,
//    SAY_AIRLOCK       = -1666027,
//    SAY_TRANSFORM_1   = -1631107,
//    SAY_TRANSFORM_2   = -1631108,
//    SAY_BERSERK       = -1631113,
//    SAY_DEATH         = -1631114,
//    EMOTE_GOO         = -1631109,
//    EMOTE_GAS_BOMB    = -1631110
//};

enum Spells
{
    SPELL_UNSTABLE_EXPERIMENT   = 70351,
    SPELL_TEAR_GAS              = 71617,
    SPELL_CREATE_CONCOTION      = 71621,
    SPELL_GUZZLE_POTIONS        = 71893,
    SPELL_MALLEABLE_GOO         = 70852,
    SPELL_MUTATED_PLAGUE        = 72451,
    SPELL_OOZE_ERUPTION         = 70492,
    SPELL_VOLATILE_OOZE         = 70447,
    SPELL_CHOKING_GAS           = 71278,
    SPELL_SLIME_PUDDLE          = 70341,
    SPELL_SLIME_PUDDLE_AURA     = 70346,
    SPELL_CHOKING_GAS_EXPLOSION = 71279,
    SPELL_CHOKING_GAS_BOMB      = 71273,
    SPELL_CHOKING_GAS_BOMB_1    = 71275,
    SPELL_CHOKING_GAS_BOMB_2    = 71276,
    SPELL_CHOKING_GAS_AURA      = 71259,
    SPELL_SUMMON_OOZE           = 71413,
    SPELL_GASEOUS_BLOAT         = 70672,
    SPELL_MUTATED               = 70405,
    SPELL_STRENGTH              = 71603,
    SPELL_PUDDLE_TRIGGER        = 71425,
    SPELL_HITTIN_PROC           = 71971,
    SPELL_OOZE_SEARCH_EFFECT    = 70457,
    SPELL_OOZE_FLOOD            = 69783,
    SPELL_GAS_FLOOD             = 71379,
    SPELL_TRANSFORMATION        = 70311,
    SPELL_TRANSFORMATION_DAMAGE = 70405,
    SPELL_TRANSFORMATION_NAME   = 72401,
    SPELL_GASEOUS_PROTECTION    = 70812,
    SPELL_VOLATILE_PROTECTION   = 70530
};

enum ePhases
{
    PHASE_FESTERGUT     = 1,
    PHASE_ROTFACE       = 2,
    PHASE_COMBAT        = 3,

    PHASE_MASK_NOT_SELF = (1 << PHASE_FESTERGUT) | (1 << PHASE_ROTFACE)
};

static const Position festergutWatchPos = {4324.820f, 3166.03f, 389.3831f, 3.316126f}; //emote 432 (release gas)
static const Position rotfaceWatchPos   = {4390.371f, 3164.50f, 389.3890f, 5.497787f}; //emote 432 (release ooze)
static const Position tablePos          = {4356.190f, 3262.90f, 389.4820f, 1.483530f}; 

const Position SpawnLoc[]=
{
    {4486.825f, 3211.986f, 404.385f, 0.0f},
    {4486.825f, 3111.452f, 389.385f, 0.0f},
    {4486.825f, 3213.452f, 404.385f, 0.0f},
    {4486.825f, 3213.452f, 389.385f, 0.0f}
};

class boss_professor_putricide : public CreatureScript
{
    public:
        boss_professor_putricide() : CreatureScript("boss_professor_putricide") { }

        struct boss_professor_putricideAI : public BossAI
        {
            boss_professor_putricideAI(Creature* pCreature) : BossAI(pCreature, DATA_PROFESSOR_PUTRICIDE)
            {
                pInstance = me->GetInstanceScript();
                phase = ePhases(0);
                PhaseSwitch1 = false;
                PhaseSwitch2 = false;
                fDie = false; //festergut die
                rDie = false; // rotface die
            }

            void Reset()
            {
                m_uiPhase = 1;
                m_uiUnstableExperimentTimer = 35000;
                m_uiPuddleTimer = 30000;
                m_uiMalleableTimer = 15000;
                m_uiPhaseSwitchTimer = 1000;
                m_uiBerserkTimer = 600000;
                m_uiBombTimer = 25000;
                m_uiMutatedPlague = 10000;

                experement = 1;
                phaseswitch = 0;
                stage = 1;

                fBaseSpeed = me->GetSpeedRate(MOVE_RUN);
                me->SetReactState(REACT_DEFENSIVE);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

                if (pInstance && me->isAlive())
                    pInstance->SetData(DATA_PROFESSOR_PUTRICIDE_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                if(!pInstance)
                    return;

                pInstance->SetData(DATA_PROFESSOR_PUTRICIDE_EVENT, IN_PROGRESS);
                SetSonPhase(PHASE_COMBAT);
                Talk(SAY_AGGRO);
                DoZoneInCombat(me);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                Talk(SAY_DEATH);

                if (pInstance)
                    pInstance->SetData(DATA_PROFESSOR_PUTRICIDE_EVENT, DONE);

                summons.DespawnAll();
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                {
                    Talk(SAY_KILL);
                }
            }

            void JustReachedHome()
            {
                if(pInstance)
                    pInstance->SetData(DATA_PROFESSOR_PUTRICIDE_EVENT, FAIL);

                summons.DespawnAll();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
                switch (summon->GetEntry())
                {
                    case CREATURE_GAS_CLOUD:
                        summon->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                        summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_GASEOUS_PROTECTION))
                        {
                            summon->AI()->AttackStart(target);
                            summon->CastCustomSpell(SPELL_GASEOUS_BLOAT, SPELLVALUE_MAX_TARGETS, 1, target, false);
                            summon->ClearUnitState(UNIT_STAT_CASTING);
                            summon->GetMotionMaster()->MoveIdle();
                        }
                        return;
                    case CREATURE_VOLATILE_OOZE:
                        summon->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                        summon->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_VOLATILE_PROTECTION))
                        {
                            summon->AI()->AttackStart(target);
                            summon->CastCustomSpell(SPELL_VOLATILE_OOZE, SPELLVALUE_MAX_TARGETS, 1, target, false);
                            summon->ClearUnitState(UNIT_STAT_CASTING);
                            summon->GetMotionMaster()->MoveIdle();
                        }
                        return;
                    default:
                        break;
                }
                if (me->isInCombat())
                    DoZoneInCombat(summon);
            }

            void SetSonPhase(ePhases newPhase)
            {
                phase = newPhase;
                events.SetPhase(newPhase);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_FESTERGUT_COMBAT:
                        SetSonPhase(PHASE_FESTERGUT);
                        me->SetSpeed(MOVE_RUN, fBaseSpeed*2.0f, true);
                        me->GetMotionMaster()->MovePoint(POINT_FESTERGUT, festergutWatchPos);
                        me->SetReactState(REACT_PASSIVE);
                        if (IsHeroic())
                        {
                            m_uiMalleableGooTimer = urand(16000, 20000);
                        }
                        break;
                    case ACTION_ROTFACE_COMBAT:
                        SetSonPhase(PHASE_ROTFACE);
                        me->SetSpeed(MOVE_RUN, fBaseSpeed*2.0f, true);
                        me->GetMotionMaster()->MovePoint(POINT_ROTFACE, rotfaceWatchPos);
                        me->SetReactState(REACT_PASSIVE);
                        if (IsHeroic())
                        {
                            m_uiVileGasTimer = urand(16000, 20000);
                        }
                        break;
                    case ACTION_FESTERGUT_GAS:
                        Talk(SAY_FESTERGUT_GASEOUS_BLIGHT);
                        if (Creature* bfestergut = Unit::GetCreature(*me, instance->GetData64(DATA_FESTERGUT)))
                        {
                            bfestergut->CastSpell(bfestergut, SPELL_RELEASE_GAS_VISUAL, false, NULL, NULL, me->GetGUID());
                        }
                        break;
                    //case ACTION_ROTFACE_GAS:
                    case ACTION_FESTERGUT_DEATH:
                        m_uiSayDieTimer = 4000;
                        fDie = true;
                        break;
                    case ACTION_ROTFACE_DEATH:
                        m_uiSayDieTimer = 4000;
                        rDie = true;
                        break;
                    default:
                        break;
                    }
                }

            void SpawnAdds()
            {
                switch(experement)
                {
                case 1:
                    {
                        Creature* green_stalker = me->SummonCreature(CREATURE_PUDDLE_STALKER, SpawnLoc[0], TEMPSUMMON_TIMED_DESPAWN, 4000);
                        if(green_stalker)
                        {
                            green_stalker->CastSpell(green_stalker, SPELL_OOZE_FLOOD, true);
                            DoSummon(CREATURE_VOLATILE_OOZE, SpawnLoc[1]);
                        }
                    }
                    break;
                case 2:
                    {
                        Creature* orange_stalker = me->SummonCreature(CREATURE_PUDDLE_STALKER, SpawnLoc[2], TEMPSUMMON_TIMED_DESPAWN, 4000);
                        if(orange_stalker)
                        {
                            orange_stalker->CastSpell(orange_stalker, SPELL_GAS_FLOOD, true);
                            DoSummon(CREATURE_GAS_CLOUD, SpawnLoc[3]);
                        }
                    }
                }
            }

            void TwoAdds()
            {
                Creature* green_stalker = me->SummonCreature(CREATURE_PUDDLE_STALKER, SpawnLoc[0], TEMPSUMMON_TIMED_DESPAWN, 4000);//green
                if(green_stalker)
                {
                    green_stalker->CastSpell(green_stalker, SPELL_OOZE_FLOOD, true);
                    DoSummon(CREATURE_VOLATILE_OOZE, SpawnLoc[2]);
                }
                Creature* orange_stalker = me->SummonCreature(CREATURE_PUDDLE_STALKER, SpawnLoc[1], TEMPSUMMON_TIMED_DESPAWN, 4000);//orange
                if(orange_stalker)
                {
                    orange_stalker->CastSpell(orange_stalker, SPELL_GAS_FLOOD, true);
                    DoSummon(CREATURE_GAS_CLOUD, SpawnLoc[3]);
                }
            }

            void PhaseSwitch()
            {
                switch(phaseswitch)
                    {
                    case 0:
                        if (!IsHeroic())
                        {
                            Creature* gas_stalker = me->FindNearestCreature(CREATURE_TEAR_GAS_STALKER, 150.0f, true);
                            if(gas_stalker)
                            {
                                me->CastSpell(gas_stalker, SPELL_TEAR_GAS, true);
                            }
                        }
                        if (IsHeroic())
                        {
                            Talk(SAY_PHASE_TRANSITION_HEROIC);
                            TwoAdds();
                        }
                        ++phaseswitch;
                        m_uiPhaseSwitchTimer = 4000;
                        break;
                    case 1:
                        me->GetMotionMaster()->MovePoint(0, tablePos);
                        ++phaseswitch;
                        m_uiPhaseSwitchTimer = 9000;
                        break;
                    case 2:
                        if(m_uiPhase == 1)
                            DoCast(SPELL_CREATE_CONCOTION);
                        if(m_uiPhase == 2)
                            DoCast(SPELL_GUZZLE_POTIONS);
                        ++phaseswitch;
                        m_uiPhaseSwitchTimer = 3000;
                        break;
                    case 3:
                        if(m_uiPhase == 1)
                            m_uiPhase = 2;
                        if(m_uiPhase == 2)
                            m_uiPhase = 3;
                        pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TEAR_GAS);
                        ++phaseswitch;
                        break;
                    }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(phase == PHASE_FESTERGUT)
                {
                    if (m_uiMalleableGooTimer < uiDiff)
                    {
                        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                            DoCast(pTarget, SPELL_MALLABLE_GOO_H);
                        m_uiMalleableGooTimer = urand(16000, 20000);
                    } else m_uiMalleableGooTimer -= uiDiff;

                    if (m_uiSayDieTimer < uiDiff)
                    {
                        if(fDie)
                        {
                            Talk(SAY_FESTERGUT_DEATH);
                            EnterEvadeMode();
                            fDie = false;
                        }
                        m_uiSayDieTimer = 4000;
                    } else m_uiSayDieTimer -= uiDiff;
                }

                if(phase == PHASE_ROTFACE)
                {
                    if (m_uiVileGasTimer < uiDiff)
                    {
                        if (Creature* rotface = Unit::GetCreature(*me, instance->GetData64(DATA_ROTFACE)))
                            if (rotface->isAlive())
                            {
                                std::list<Unit*> targetList;
                                uint32 minTargets = RAID_MODE<uint32>(3, 8, 3, 8);
                                rotface->AI()->SelectTargetList(targetList, minTargets, SELECT_TARGET_RANDOM, -5.0f, true);
                                Unit* target;
                                if (targetList.size() >= minTargets)
                                {
                                    std::list<Unit*>::iterator itr = targetList.begin();
                                    advance(itr, urand(0, targetList.size()-1));;
                                    target = *itr;
                                }
                                else
                                    target = rotface->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true);
                                if (target)
                                    DoCast(target, SPELL_VILE_GAS_H, true); // triggered, to skip LoS check
                            }
                        m_uiVileGasTimer = urand (16000, 20000);
                    } else m_uiVileGasTimer -= uiDiff;

                    if (m_uiSayDieTimer < uiDiff)
                    {
                        if(rDie)
                        {
                            Talk(SAY_ROTFACE_DEATH);
                            EnterEvadeMode();
                            rDie = false;
                        }
                        m_uiSayDieTimer = 4000;
                    } else m_uiSayDieTimer -= uiDiff;
                }
                if (phase == PHASE_COMBAT)
                {
                    if (m_uiBerserkTimer < uiDiff)
                    {
                        DoCast(me, SPELL_BERSERK);
                        Talk(SAY_BERSERK);
                        m_uiBerserkTimer = 600000;
                    } else m_uiBerserkTimer -= uiDiff;

                    if((me->GetHealth()*100) / me->GetMaxHealth() < 81 && PhaseSwitch1 == false)
                    {
                        PhaseSwitch();
                        PhaseSwitch1 = true;
                    }

                    if((me->GetHealth()*100) / me->GetMaxHealth() < 36 && PhaseSwitch2 == false)
                    {
                        PhaseSwitch();
                        PhaseSwitch2 = true;
                    }

                    if (m_uiPuddleTimer < uiDiff)
                    {
                        for (uint8 i = 1; i <= 2; i++)
                        {
                            if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, -8.0f, true))
                                DoCast(pTarget, SPELL_SLIME_PUDDLE, true);
                        }
                        m_uiPuddleTimer = 30000;
                    } else m_uiPuddleTimer -= uiDiff;

                    if (m_uiPhase == 1)
                    {
                        if (m_uiUnstableExperimentTimer < uiDiff)
                        {
                            SpawnAdds();
                            DoCast(SPELL_UNSTABLE_EXPERIMENT);
                            Talk(EMOTE_UNSTABLE_EXPERIMENT);
                            m_uiUnstableExperimentTimer = 40000;
                        } else m_uiUnstableExperimentTimer -= uiDiff;
                    }

                    if (m_uiPhase == 2)
                    {
                        if (m_uiUnstableExperimentTimer < uiDiff)
                        {
                            DoCast(me, SPELL_UNSTABLE_EXPERIMENT);
                            Talk(EMOTE_UNSTABLE_EXPERIMENT);
                            TwoAdds();
                            m_uiUnstableExperimentTimer = 40000;
                        } else m_uiUnstableExperimentTimer -= uiDiff;

                        if (m_uiMalleableTimer < uiDiff)
                        {
                            for (uint8 i = 0; i <= RAID_MODE(1,2,1,2); i++)
                            {
                                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, -8.0f, true))
                                    DoCast(pTarget, SPELL_MALLEABLE_GOO);
                            }
                            m_uiMalleableTimer = 16000;
                        } else m_uiMalleableTimer -= uiDiff;

                        if (m_uiBombTimer < uiDiff)
                        {
                            DoCast(SPELL_CHOKING_GAS_BOMB);
                            DoCast(SPELL_CHOKING_GAS_BOMB_1);
                            DoCast(SPELL_CHOKING_GAS_BOMB_2);
                            m_uiBombTimer = 30000;
                        } else m_uiBombTimer -= uiDiff;
                    }
                    if (m_uiPhase == 3)
                    {
                        if(!me->HasAura(SPELL_STRENGTH))
                        {
                            DoCast(me, SPELL_STRENGTH);
                        }

                        if (m_uiMalleableTimer < uiDiff)
                        {
                            for (uint8 i = 0; i <= RAID_MODE(1,2,1,2); i++)
                            {
                                if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, -8.0f, true))
                                    DoCast(pTarget, SPELL_MALLEABLE_GOO);
                            }
                            m_uiMalleableTimer = 16000;
                        } else m_uiMalleableTimer -= uiDiff;

                        if (m_uiBombTimer < uiDiff)
                        {
                            DoCast(SPELL_CHOKING_GAS_BOMB);
                            DoCast(SPELL_CHOKING_GAS_BOMB_1);
                            DoCast(SPELL_CHOKING_GAS_BOMB_2);
                            m_uiBombTimer = 30000;
                        } else m_uiBombTimer -= uiDiff;

                        if (m_uiMutatedPlague < uiDiff)
                        {
                            DoCastVictim(SPELL_MUTATED_PLAGUE);
                            m_uiMutatedPlague = 10000;
                        } else m_uiMutatedPlague -= uiDiff;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* pInstance;

            uint32 m_uiPhase;
            uint32 m_uiUnstableExperimentTimer;
            uint32 m_uiResetTimer;
            uint32 m_uiPuddleTimer;
            uint32 m_uiMalleableTimer;
            uint32 m_uiPhaseSwitchTimer;
            uint32 m_uiBerserkTimer;
            uint32 m_uiBombTimer;
            uint32 m_uiMutatedPlague;
            //festergut and rotface
            uint32 m_uiMalleableGooTimer;
            uint32 m_uiVileGasTimer;
            uint32 m_uiSayDieTimer;
            //SummonList summons;
            uint8 experement;
            uint8 phaseswitch;
            uint8 stage;

            bool PhaseSwitch1;
            bool PhaseSwitch2;
            bool fDie; //festergut die
            bool rDie; // rotface die

            float fBaseSpeed;

            ePhases phase;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_professor_putricideAI(pCreature);
        }
};

class npc_volatile_ooze : public CreatureScript
{
    public:
        npc_volatile_ooze() : CreatureScript("npc_volatile_ooze") { }

        struct npc_volatile_oozeAI : public ScriptedAI
        {
            npc_volatile_oozeAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                me->SetSpeed(MOVE_RUN, 0.5);
                me->SetSpeed(MOVE_WALK, 0.5);

                m_uiSearchTargetTimer = 1000;
                target = false;

                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
            }

            void SpellHitTarget(Unit* /*pTarget*/, const SpellEntry* spell)
            {
                SpellEntry const* explode = sSpellStore.LookupEntry(SPELL_OOZE_ERUPTION);
                explode = sSpellMgr->GetSpellForDifficultyFromSpell(explode, me);
                if (explode->Id == spell->Id)
                {
                    m_uiSearchTargetTimer = 5000;
                    DoStartNoMovement(me);
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (!target && m_uiSearchTargetTimer < uiDiff)
                {
                    me->CastCustomSpell(SPELL_VOLATILE_OOZE, SPELLVALUE_MAX_TARGETS, 1, me->getVictim(), false);
                    me->CastCustomSpell(SPELL_OOZE_SEARCH_EFFECT, SPELLVALUE_MAX_TARGETS, 1, me->getVictim(), false);
                    target = true;
                    m_uiSearchTargetTimer = 1000;
                } else m_uiSearchTargetTimer -= uiDiff;

                if (me->getVictim() && me->getVictim()->IsWithinDistInMap(me, 1))
                {
                    DoCast(me, SPELL_OOZE_ERUPTION);
                    me->getVictim()->RemoveAurasDueToSpell(SPELL_VOLATILE_OOZE);
                    me->getVictim()->RemoveAurasDueToSpell(SPELL_OOZE_SEARCH_EFFECT);
                    target = false;
                }

                DoMeleeAttackIfReady();
            }
        private:
             InstanceScript* pInstance;

             uint32 m_uiSearchTargetTimer;
             bool target;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_volatile_oozeAI(pCreature);
        }
};

class npc_gas_cloud_icc : public CreatureScript
{
    public:
        npc_gas_cloud_icc() : CreatureScript("npc_gas_cloud_icc") { }

        struct npc_gas_cloud_iccAI : public ScriptedAI
        {
            npc_gas_cloud_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                me->SetSpeed(MOVE_RUN, 0.5);
                me->SetSpeed(MOVE_WALK, 0.5);
                me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);

                m_uiBloatTimer = 6000;
                m_uiSearchTargetTimer = 1000;

                target = false;
            }

            void EnterCombat(Unit* /*who*/) { }

            void UpdateAI(const uint32 diff)
            {
                if (m_uiBloatTimer < diff)
                {
                    DoCast(me->getVictim(), SPELL_GASEOUS_BLOAT);
                    m_uiBloatTimer = 5000;
                } else m_uiBloatTimer -= diff;

                if (!target && m_uiSearchTargetTimer < diff)
                {
                    if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
                    {
                        DoCast(pTarget, SPELL_GASEOUS_BLOAT);
                        me->AddThreat(pTarget, 10000000.0f);
                        me->GetMotionMaster()->MoveChase(pTarget);
                        target = true;
                    }
                    m_uiSearchTargetTimer = 1000;
                } else m_uiSearchTargetTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* pInstance;

            uint32 m_uiBloatTimer;
            uint32 m_uiSearchTargetTimer;
            bool target;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_gas_cloud_iccAI(pCreature);
        }
};

class npc_bomb_icc : public CreatureScript
{
    public:
        npc_bomb_icc() : CreatureScript("npc_bomb_icc") { }

        struct npc_bomb_iccAI : public ScriptedAI
        {
            npc_bomb_iccAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                m_uiBoomTimer = 20000;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetInCombatWithZone();
                DoStartNoMovement(me->getVictim());
            }

            void EnterCombat(Unit* /*who*/) { }

            void UpdateAI(const uint32 diff)
            {
                if (m_uiBombTimer < diff)
                {
                    DoCast(SPELL_CHOKING_GAS_AURA);
                    m_uiBombTimer = 1500;
                } else m_uiBombTimer -= diff;

                if (m_uiBoomTimer < diff)
                {
                    DoCast(SPELL_CHOKING_GAS_EXPLOSION);
                    me->DespawnOrUnsummon();
                    m_uiBoomTimer = 20000;
                } else m_uiBoomTimer -= diff;
            }

        private:
            InstanceScript* pInstance;

            uint32 m_uiBombTimer;
            uint32 m_uiBoomTimer;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_bomb_iccAI(pCreature);
        }
};

class spell_putricide_mutation_init : public SpellScriptLoader
{
    public:
        spell_putricide_mutation_init() : SpellScriptLoader("spell_putricide_mutation_init") { }


        class spell_putricide_mutation_init_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_putricide_mutation_init_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                uint32 spellId = 70311;
                if (GetTarget()->GetMap()->GetSpawnMode() & 1)
                    spellId = 71503;

                GetTarget()->CastSpell(GetTarget(), spellId, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_putricide_mutation_init_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };


        AuraScript* GetAuraScript() const
        {
            return new spell_putricide_mutation_init_AuraScript();
        }
};

class spell_putricide_mutated_transformation_dismiss : public SpellScriptLoader
{
    public:
        spell_putricide_mutated_transformation_dismiss() : SpellScriptLoader("spell_putricide_mutated_transformation_dismiss") { }


        class spell_putricide_mutated_transformation_dismiss_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_putricide_mutated_transformation_dismiss_AuraScript);


            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Vehicle* veh = GetTarget()->GetVehicleKit())
                    veh->RemoveAllPassengers();
            }


            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_putricide_mutated_transformation_dismiss_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };


        AuraScript* GetAuraScript() const
        {
            return new spell_putricide_mutated_transformation_dismiss_AuraScript();
        }
};

class spell_putricide_mutated_transformation : public SpellScriptLoader
{
    public:
        spell_putricide_mutated_transformation() : SpellScriptLoader("spell_putricide_mutated_transformation") { }


        class spell_putricide_mutated_transformation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_putricide_mutated_transformation_SpellScript);


            void HandleSummon(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                uint32 entry = GetSpellInfo()->EffectMiscValue[effIndex];
                SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(GetSpellInfo()->EffectMiscValueB[effIndex]);
                Unit* caster = GetOriginalCaster();
                if (!caster)
                    return;


                int32 duration = GetSpellDuration(GetSpellInfo());


                Position pos;
                caster->GetPosition(&pos);
                TempSummon* summon = caster->GetMap()->SummonCreature(entry, pos, properties, duration, caster);
                if (!summon || !summon->IsVehicle())
                    return;


                caster->CastSpell(summon, SPELL_TRANSFORMATION_NAME, true);
                summon->CastSpell(summon, SPELL_TRANSFORMATION_DAMAGE, true);
                caster->EnterVehicle(summon->GetVehicleKit(), 0);


                summon->SetUInt32Value(UNIT_CREATED_BY_SPELL, GetSpellInfo()->Id);
                summon->SetCreatorGUID(caster->GetGUID());
            }


            void Register()
            {
                OnEffect += SpellEffectFn(spell_putricide_mutated_transformation_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };


        SpellScript* GetSpellScript() const
        {
            return new spell_putricide_mutated_transformation_SpellScript();
        }
};

class spell_putricide_ooze_summon : public SpellScriptLoader
{
    public:
        spell_putricide_ooze_summon() : SpellScriptLoader("spell_putricide_ooze_summon") { }


        class spell_putricide_ooze_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_putricide_ooze_summon_AuraScript);

            void HandleTriggerSpell(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if (Unit* caster = GetCaster())
                {
                    uint32 triggerSpellId = GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                    float x, y, z;
                    GetTarget()->GetPosition(x, y, z);
                    z = GetTarget()->GetMap()->GetHeight(x, y, z, true, 25.0f);
                    x += 10.0f * cosf(caster->GetOrientation());
                    y += 10.0f * sinf(caster->GetOrientation());
                    caster->CastSpell(x, y, z, triggerSpellId, true, NULL, NULL, GetCasterGUID(), caster);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_putricide_ooze_summon_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };


        AuraScript* GetAuraScript() const
        {
            return new spell_putricide_ooze_summon_AuraScript();
        }
};
// Stinky and Precious spell, it's here because its used for both (Festergut and Rotface "pets")
class spell_stinky_precious_decimate : public SpellScriptLoader
{
    public:
        spell_stinky_precious_decimate() : SpellScriptLoader("spell_stinky_precious_decimate") { }

        class spell_stinky_precious_decimate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_stinky_precious_decimate_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit()->GetHealthPct() > float(GetEffectValue()))
                {
                    uint32 newHealth = GetHitUnit()->GetMaxHealth() * uint32(GetEffectValue()) / 100;
                    if (GetHitUnit()->GetMaxHealth() >= newHealth)
                        GetHitUnit()->SetHealth(newHealth);
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_stinky_precious_decimate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_stinky_precious_decimate_SpellScript();
        }
}; 
void AddSC_boss_professor_putricide()
{
    new boss_professor_putricide();
    new npc_volatile_ooze();
    new npc_gas_cloud_icc();
    new npc_bomb_icc();
    new spell_putricide_mutation_init();
    new spell_putricide_mutated_transformation_dismiss();
    new spell_putricide_mutated_transformation();
    new spell_putricide_ooze_summon();
    new spell_stinky_precious_decimate(); 
}
