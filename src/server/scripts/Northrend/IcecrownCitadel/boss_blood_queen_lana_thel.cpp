/* Copyright (C) 2009 - 2010 TrinityCore <http://www.trinitycore.org/>
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
    SAY_AGGRO            = -1666054,
    SAY_VAMP_BITE_1      = -1666055,
    SAY_VAMP_BITE_2      = -1666056,
    SAY_SWARM_SHADOWS    = -1666057,
    SAY_PACT_DARKFALLEN  = -1666058,
    SAY_MIND_CONTROLL    = -1666059,
    SAY_AIR_PHASE        = -1666060,
    SAY_BERSERK          = -1666061,
    SAY_DEATH            = -1666062
};

enum Spells
{
    SPELL_PRESENCE_OF_DARKFALLEN    = 71952,
    SPELL_SHROUD_OF_SORROW          = 72981,
    SPELL_DELIRIOUS_SLASH           = 71623,
    SPELL_BLOOD_MIRROR_1            = 70821,
    SPELL_BLOOD_MIRROR_2            = 71510,
    SPELL_VAMPIRIC_BITE             = 71726,
    SPELL_PACT_OF_THE_DARKFALLEN_1  = 71340,
    SPELL_PACT_OF_THE_DARKFALLEN_2  = 71341,
    SPELL_SWARMING_SHADOWS          = 71264,
    SPELL_TWILIGHT_BLOODBOLT        = 71446,
    SPELL_BLOODBOLT_WHIRL           = 71899,
    SPELL_BLOODBOLT_SPLASH          = 71447,
    SPELL_INCITE_TERROR             = 73070,
    SPELL_SWARMING_SHADOWS_VISUAL   = 71267,
    SPELL_SWARMING_SHADOW           = 71267,
    SPELL_ESSENCE_OF_BLOOD_QWEEN    = 70867,
    SPELL_FRENZIED_BLOODTHIRST      = 70877,
    SPELL_UNCONTROLLABLE_FRENZY     = 70923
};

const Position SpawnLoc[]=
{
    {4595.640f, 2769.195f, 400.137f, 0.0f}, //phase 2 pos
    {4595.904f, 2769.315f, 421.838f, 0.0f} //fly pos
};

class boss_blood_queen_lanathel : public CreatureScript
{
    public:
        boss_blood_queen_lanathel() : CreatureScript("boss_blood_queen_lanathel") { }

        struct boss_blood_queen_lanathelAI : public BossAI
        {
            boss_blood_queen_lanathelAI(Creature* pCreature) : BossAI(pCreature, DATA_BLOOD_QUEEN_LANATHEL)
            {
                pInstance = me->GetInstanceScript();
            }

            void Reset()
            {
                m_uiPhase = 1;
                m_uiPhaseTimer = 90000;
                m_uiTwilightBloodboltTimer = 10000;
                m_uiVampBiteTimer  = 15000;
                m_uiSwarmingShadowsTimer = 30000;
                m_uiPactofDarkfallenTimer  = 5000;
                m_uiSetHoverTimer = 90000;
                m_uiBloodboldSplashTimer = 900000;
                m_uiLandingTimer = 900000;
                m_uiFlyingFalseTimer = 900000;
                m_uiBerserkTimer = 330000;

                memset(&Darkfallen, 0, sizeof(Darkfallen));

                if (pInstance && me->isAlive())
                    pInstance->SetData(DATA_BLOOD_QUEEN_LANATHEL_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);
                DoCast(SPELL_SHROUD_OF_SORROW);

                if (pInstance)
                    pInstance->SetData(DATA_BLOOD_QUEEN_LANATHEL_EVENT, IN_PROGRESS);
            }

            void KilledUnit(Unit* pVictim)
            {
                pVictim->RemoveAllAuras();
            }

            void JustDied(Unit* /*Killer*/)
            {
                DoScriptText(SAY_DEATH, me);

                if (pInstance)
                    pInstance->SetData(DATA_BLOOD_QUEEN_LANATHEL_EVENT, DONE);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if (!me->HasAura(SPELL_SHROUD_OF_SORROW))
                    DoCast(me, SPELL_SHROUD_OF_SORROW);

                if (m_uiBerserkTimer < uiDiff)
                {
                    DoScriptText(SAY_BERSERK, me);
                    DoCast(me, SPELL_BERSERK);
                    m_uiBerserkTimer = 330000;
                } else m_uiBerserkTimer -= uiDiff;

                if (m_uiPhase == 1)
                {
                    if (m_uiPactofDarkfallenTimer <= uiDiff)
                    {
                        DoScriptText(SAY_PACT_DARKFALLEN, me);
                        m_uiPactofDarkfallenExplosionCount = 0;
                        m_uiPactofDarkfallenExplodeTimer = 2000;
                        m_uiPactofDarkfallenTimer = 25000;
                    } else m_uiPactofDarkfallenTimer -= uiDiff;

                    if (m_uiSwarmingShadowsTimer < uiDiff)
                    {
                        if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                        DoCast(pTarget, SPELL_SWARMING_SHADOWS);
                        m_uiSwarmingShadowsTimer = 30000;
                    } else m_uiSwarmingShadowsTimer -= uiDiff;

                    if (m_uiTwilightBloodboltTimer < uiDiff)
                    {
                        if(Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                        DoCast(pTarget, SPELL_TWILIGHT_BLOODBOLT);
                        m_uiTwilightBloodboltTimer = 9000;
                    } else m_uiTwilightBloodboltTimer -= uiDiff;

                    if (m_uiVampBiteTimer < uiDiff)
                    {
                        DoScriptText(RAND(SAY_VAMP_BITE_1,SAY_VAMP_BITE_2), me);
                        DoCast(me->getVictim(), SPELL_VAMPIRIC_BITE);
                        m_uiVampBiteTimer = 40000;
                    } else m_uiVampBiteTimer -= uiDiff;

                    if (m_uiPhaseTimer < uiDiff)
                    {
                        DoScriptText(SAY_AIR_PHASE, me);
                        DoCast(me, SPELL_INCITE_TERROR);
                        me->GetMotionMaster()->MovePoint(0, SpawnLoc[0]);
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        m_uiPhase = 2;
                        m_uiPhaseTimer = 90000;
                        m_uiSetHoverTimer = 5000;
                        m_uiBloodboldSplashTimer = 10000;
                        m_uiLandingTimer = 26000;
                        m_uiFlyingFalseTimer = 30000;
                    } else m_uiPhaseTimer -= uiDiff;
                }

                if (m_uiPhase == 2)
                {
                    if (m_uiSetHoverTimer < uiDiff)
                    {
                        me->GetMotionMaster()->MovePoint(0, SpawnLoc[1]);
                        me->SetUnitMovementFlags(MOVEMENTFLAG_JUMPING);
                        me->SetFlying(true);
                        m_uiSetHoverTimer = 90000;
                    } else m_uiSetHoverTimer -= uiDiff;

                    if (m_uiBloodboldSplashTimer < uiDiff)
                    {
                        std::list<Unit*> targets;
                        SelectTargetList(targets, 5, SELECT_TARGET_RANDOM, 80, true);
                        for (std::list<Unit*>::const_iterator i = targets.begin(); i != targets.end(); ++i)
                            DoCast(*i, SPELL_TWILIGHT_BLOODBOLT);
                        m_uiBloodboldSplashTimer = 2000;
                    } else m_uiBloodboldSplashTimer -= uiDiff;

                    if (m_uiLandingTimer < uiDiff)
                    {
                        me->GetMotionMaster()->MovePoint(0, SpawnLoc[0]);
                        me->SetUnitMovementFlags(MOVEMENTFLAG_JUMPING);
                        m_uiLandingTimer = 900000;
                        m_uiBloodboldSplashTimer = 900000;
                    } else m_uiLandingTimer -= uiDiff;

                    if (m_uiFlyingFalseTimer < uiDiff)
                    {
                        me->SetFlying(false);
                        me->RemoveAllAuras();
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                        m_uiPhase = 1;
                        m_uiFlyingFalseTimer = 900000;
                    } else m_uiFlyingFalseTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* pInstance;

            uint32 m_uiTwilightBloodboltTimer;
            uint32 m_uiPhaseTimer;
            uint32 m_uiVampBiteTimer;
            uint32 m_uiSwarmingShadowsTimer;
            uint32 m_uiPactofDarkfallenTimer;
            uint32 m_uiPactofDarkfallenExplodeTimer;
            uint32 m_uiPactofDarkfallenExplosionCount;
            uint32 m_uiPhase;
            uint32 m_uiBerserkTimer;
            uint32 m_uiSetHoverTimer;
            uint32 m_uiLandingTimer;
            uint32 m_uiFlyingFalseTimer;
            uint32 m_uiBloodboldSplashTimer;
            uint32 m_uiResetTimer;
            Unit* Darkfallen[5];
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_blood_queen_lanathelAI(pCreature);
        }
};

class npc_swarming_shadows : public CreatureScript
{
    public:
        npc_swarming_shadows() : CreatureScript("npc_swarming_shadows") { }

        struct npc_swarming_shadowsAI : public Scripted_NoMovementAI
        {
            npc_swarming_shadowsAI(Creature *pCreature) : Scripted_NoMovementAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            void Reset()
            {
                m_uiSwarmingShadowTimer = 1200;
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!me->HasAura(SPELL_SWARMING_SHADOWS_VISUAL))
                    DoCast(me, SPELL_SWARMING_SHADOWS_VISUAL);

                if (m_uiSwarmingShadowTimer < uiDiff)
                {
                    DoCast(me, SPELL_SWARMING_SHADOW);
                    m_uiSwarmingShadowTimer = 2000;
                } else m_uiSwarmingShadowTimer -= uiDiff;
            }
        private:
            InstanceScript* pInstance;

            uint32 m_uiSwarmingShadowTimer;
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_swarming_shadowsAI(pCreature);
        }
};

void AddSC_boss_blood_queen_lana_thel()
{
    new boss_blood_queen_lanathel();
    new npc_swarming_shadows();
}
