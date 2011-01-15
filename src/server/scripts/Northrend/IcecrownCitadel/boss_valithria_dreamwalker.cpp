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

#include "ScriptPCH.h"
#include "icecrown_citadel.h"

enum Yells
{
    SAY_AGGRO       = -1666063,
    SAY_BELOW_25    = -1666066,
    SAY_ABOVE_75    = -1666065,
    SAY_DEATH       = -1666067,
    SAY_PDEATH      = -1666068,
    SAY_END         = -1666070,
    SAY_BERSERK     = -1666069,
    SAY_OPEN_PORTAL = -1666064
};

enum Spells
{
    SPELL_CORRUPTION    = 70904,
    SPELL_DREAM_SLIP    = 71196,
    SPELL_RAGE          = 71189,
    SPELL_COLUMN        = 70704,
    SPELL_DREAM_STATE   = 70766,
    SPELL_VIGOR         = 70873,
    SPELL_CLOUD_VISUAL  = 70876,
    SPELL_PORTAL_N_PRE  = 71301,
    SPELL_PORTAL_N_NPC  = 71305,
    SPELL_PORTAL_H_PRE  = 71977,
    SPELL_PORTAL_H_NPC  = 71987,
};

const Position Pos[] =
{
    {4239.579102f, 2566.753418f, 364.868439f, 0.0f}, //normal 0,1
    {4240.688477f, 2405.794678f, 364.868591f, 0.0f}, // normal
    {4165.112305f, 2405.872559f, 364.872925f, 0.0f}, //2,3
    {4166.216797f, 2564.197266f, 364.873047f, 0.0f}
};

class boss_valithria : public CreatureScript
{
    public:
        boss_valithria() : CreatureScript("boss_valithria") { }

        struct boss_valithriaAI : public BossAI
        {
            boss_valithriaAI(Creature* pCreature) : BossAI(pCreature, DATA_VALITHRIA_DREAMWALKER)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                m_uiStage = 1;

                DoCast(me, SPELL_CORRUPTION);
                me->SetHealth(uint32(me->GetMaxHealth() / 2));

                m_uiEndTimer = 1000;

                bIntro = false;
                bEnd = false;
                bAboveHP = false;
                bBelowHP = false;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (pInstance)
                    pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, NOT_STARTED);
            }

            void MoveInLineOfSight(Unit *who)
            {
                if (pInstance && pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) == NOT_STARTED)
                    pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, IN_PROGRESS);

                if (!bIntro && who->IsWithinDistInMap(me, 40.0f,true))
                {
                    DoScriptText(SAY_AGGRO, me);
                    bIntro = true;

                    ScriptedAI::MoveInLineOfSight(who);
                }
                if (pInstance && pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) == IN_PROGRESS)
                    if(!who->IsWithinDistInMap(me, 60.0f,true))
                        pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, FAIL);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                DoScriptText(SAY_DEATH, me);

                if (pInstance)
                    pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, FAIL);
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                if (!pInstance || pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) == DONE)
                    return;

                if (!bAboveHP && HealthAbovePct(74))
                {
                    DoScriptText(SAY_ABOVE_75, me);
                    bAboveHP = true;
                }
                if (!bBelowHP && HealthBelowPct(26))
                {
                    DoScriptText(SAY_BELOW_25, me);
                    bBelowHP = true;
                }
                if (!bEnd && HealthAbovePct(99))
                {
                    DoScriptText(SAY_END, me);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetReactState(REACT_PASSIVE);
                    me->RemoveAurasDueToSpell(SPELL_CORRUPTION);
                    bEnd = true;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!bEnd || !pInstance || pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) == DONE)
                    return;

                if (m_uiEndTimer <= diff)
                {
                    switch(m_uiStage)
                    {
                        case 1:
                            DoScriptText(SAY_BERSERK , me);
                            DoCast(me, SPELL_RAGE);
                            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            ++m_uiStage;
                            m_uiEndTimer = 6000;
                            break;
                        case 2:
                            DoCast(SPELL_DREAM_SLIP);
                            ++m_uiStage;
                            m_uiEndTimer = 1000;
                            break;
                        case 3:
                            me->DespawnOrUnsummon();
                            pInstance->SetData(DATA_VALITHRIA_DREAMWALKER_EVENT, DONE);
                            m_uiEndTimer = 1000;
                            bEnd = false;
                            ++m_uiStage;
                            break;
                    }
                } else m_uiEndTimer -= diff;
            }

        private:
            InstanceScript* pInstance;

            uint8 m_uiStage;
            uint32 m_uiEndTimer;
            bool bIntro;
            bool bEnd;
            bool bAboveHP;
            bool bBelowHP;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_valithriaAI(pCreature);
        }
};

class npc_valithria_alternative : public CreatureScript
{
    public:
        npc_valithria_alternative() : CreatureScript("npc_valithria_alternative") { }

        struct npc_valithria_alternativeAI : public ScriptedAI
        {
            npc_valithria_alternativeAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                me->SetPhaseMask(16, true);
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY);
                me->SendMovementFlagUpdate();
                me->SetFlying(true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                m_uiSummonPortalTimer = 5000;
            }

            void UpdateAI(const uint32 diff)
            {
                if (pInstance && pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) != IN_PROGRESS)
                    return;

                if (m_uiSummonPortalTimer <= diff)
                {
                    float x, y, z;
                    me->GetPosition(x,y,z);
                    for(uint8 i = 0; i <= 8; ++i) //need correct count
                        me->SummonCreature(CREATURE_CLOUD, x + (urand(2, 6) * 10), y + (urand(1, 4) * 10), z + urand(2,8), 0, TEMPSUMMON_TIMED_DESPAWN, 15000);
                    m_uiSummonPortalTimer = 5000;
                } else m_uiSummonPortalTimer -= diff;
            }
        private:
            InstanceScript* pInstance;

            uint32 m_uiSummonPortalTimer;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_valithria_alternativeAI(pCreature);
        }
};

class npc_dreamportal_icc : public CreatureScript //портал в комнате
{
    public:
        npc_dreamportal_icc() : CreatureScript("npc_dreamportal_icc") { }

        struct npc_dreamportal_iccAI : public ScriptedAI
        {
            npc_dreamportal_iccAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);

                m_uiChangeTimer = 15000;
            }

            void UpdateAI(const uint32 diff)
            {
                if(me->GetEntry() == CREATURE_PORTAL_NORMAL_MODE_PRE)
                {
                    if (m_uiChangeTimer <= diff)
                    {
                        me->UpdateEntry(CREATURE_PORTAL_NORMAL_MODE_NPC);
                    } else m_uiChangeTimer -= diff;
                }

                if(me->GetEntry() == CREATURE_PORTAL_HEROIC_MODE_PRE)
                {
                    if (m_uiChangeTimer <= diff)
                    {
                        me->UpdateEntry(CREATURE_PORTAL_HEROIC_MODE_NPC);
                    } else m_uiChangeTimer -= diff;
                }
            }

        private:
            uint32 m_uiChangeTimer;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_dreamportal_iccAI(pCreature);
        }
};

class npc_dreamcloud_icc : public CreatureScript //облако в комнате
{
    public:
        npc_dreamcloud_icc() : CreatureScript("npc_dreamcloud_icc") { }

        struct npc_dreamcloud_iccAI : public ScriptedAI
        {
            npc_dreamcloud_iccAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void Reset()
            {
                me->SetPhaseMask(16, true);
                DoCast(SPELL_CLOUD_VISUAL);
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY);
                me->SendMovementFlagUpdate();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void MoveInLineOfSight(Unit *who)
            {
                if (me->IsWithinDistInMap(who, 5.0f, true))
                    DoCast(SPELL_VIGOR);

                ScriptedAI::MoveInLineOfSight(who);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_dreamcloud_iccAI(pCreature);
        }
};

class npc_icc_combat_stalker : public CreatureScript
{
    public:
        npc_icc_combat_stalker() : CreatureScript("npc_icc_combat_stalker") { }

        struct npc_icc_combat_stalkerAI : public ScriptedAI
        {
            npc_icc_combat_stalkerAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetInCombatWithZone();
                DoStartNoMovement(me->getVictim());
                m_uiSummonTimer = 20000;
                m_uiPortalTimer = 50000;
                m_uiColumnTimer = 13000;

                if (pInstance && pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) != IN_PROGRESS)
                    summons.DespawnAll();
            }

            void JustSummoned(Creature* pSummoned)
            {
                if (Creature* valithria = Unit::GetCreature(*me, pInstance->GetData64(DATA_VALITHRIA_DREAMWALKER)))
                    pSummoned->AI()->AttackStart(valithria);

                summons.Summon(pSummoned);
            }

            void KilledUnit(Unit* /*victim*/)
            {
                if (Creature* valithria = Unit::GetCreature(*me, pInstance->GetData64(DATA_VALITHRIA_DREAMWALKER)))
                    DoScriptText(SAY_PDEATH, valithria);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!pInstance || pInstance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) != IN_PROGRESS)
                    return;

                if (m_uiSummonTimer <= diff)
                {
                    for (uint8 coords = 0; coords <= RAID_MODE(1,3,1,3); ++coords)
                    {
                        if(urand(0,1)) DoSummon(CREATURE_ZOMBIE, Pos[coords]);
                        if(urand(0,1)) DoSummon(CREATURE_SKELETON, Pos[coords]);
                        if(urand(0,1)) DoSummon(CREATURE_ARCHMAGE, Pos[coords]);
                        if(urand(0,1)) DoSummon(CREATURE_SUPPRESSER, Pos[coords]);
                        if(urand(0,1)) DoSummon(CREATURE_ABOMINATION, Pos[coords]);
                    }
                    m_uiSummonTimer = 20000;
                } else m_uiSummonTimer -= diff;

                if (m_uiColumnTimer <= diff)
                {
                    if(Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                        DoCast(pTarget, SPELL_COLUMN);
                    m_uiColumnTimer = 13000;
                } else m_uiColumnTimer -= diff;

                if (m_uiPortalTimer <= diff)
                {
                    if (Creature* valithria = Unit::GetCreature(*me, pInstance->GetData64(DATA_VALITHRIA_DREAMWALKER)))
                        DoScriptText(SAY_OPEN_PORTAL, valithria);

                    for(uint8 p = 0; p < 6; ++p)
                    {
                        DoCast(RAID_MODE(SPELL_PORTAL_N_PRE,SPELL_PORTAL_N_PRE,SPELL_PORTAL_H_PRE,SPELL_PORTAL_H_PRE));
                    }
                    m_uiPortalTimer = 50000;
                } else m_uiPortalTimer -= diff;
            }
            private:
                InstanceScript* pInstance;

                uint32 m_uiSummonTimer;
                uint32 m_uiPortalTimer;
                uint32 m_uiColumnTimer;
                SummonList summons;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_icc_combat_stalkerAI(pCreature);
        }
};

void AddSC_boss_valithria()
{
    new boss_valithria();
    new npc_dreamportal_icc();
    new npc_dreamcloud_icc();
    new npc_icc_combat_stalker();
    new npc_valithria_alternative();
}