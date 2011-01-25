/* 
 * Copyright (C) 2008 - 2010 Trinity <http://www.trinitycore.org/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * Script Author: LordVanMartin
 */
 
#include "ScriptPCH.h"

#define SPELL_BATTLE_SHOUT                                    31403
#define SPELL_CHARGE                                          60067
#define SPELL_FRIGHTENING_SHOUT                               19134
#define SPELL_WHIRLWIND_1                                     38619
#define SPELL_WHIRLWIND_2                                     38618

//not used
//Yell
#define SAY_AGGRO                                          -1576024
#define SAY_KILL                                           -1576025
#define SAY_DEATH                                          -1576026

class boss_commander_kolurg : public CreatureScript
{
public:
    boss_commander_kolurg() : CreatureScript("boss_commander_kolurg") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_commander_kolurgAI (pCreature);
    }

    struct boss_commander_kolurgAI : public ScriptedAI
    {
        boss_commander_kolurgAI(Creature *c) : ScriptedAI(c) {}

        bool bChargePause;

        uint32 uiChargePauseTimer;
        uint32 uiBattleShoutTimer;
        uint32 uiFearTimer;
        uint32 uiWhirlwindTimer;

        void Reset() 
        {
            bChargePause=false;
            uiChargePauseTimer=0;
            uiBattleShoutTimer=0;
            uiFearTimer=20000;
            uiWhirlwindTimer=15000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (!bChargePause)
                {
                    if (Unit* pTemp=SelectTarget(SELECT_TARGET_FARTHEST,0,25,true))
                        if (me->GetExactDist(pTemp->GetPositionX(),pTemp->GetPositionY(),pTemp->GetPositionZ())>8)
                        {
                            DoCast(pTemp,SPELL_CHARGE);
                            uiChargePauseTimer=5000;
                            bChargePause=true;
                        }
                }

            if (bChargePause)
                if (uiChargePauseTimer<=diff)
                    bChargePause=false;
                else
                    uiChargePauseTimer-=diff;

            if (uiBattleShoutTimer<=diff)
            {
                DoCast(me,SPELL_BATTLE_SHOUT);
                uiBattleShoutTimer=120000;
            } else
                uiBattleShoutTimer-=diff;
        
            if (uiWhirlwindTimer<=diff)
            {
                DoCast(me,SPELL_WHIRLWIND_2);
                uiWhirlwindTimer=15000;
                uiChargePauseTimer=3500;
                bChargePause=true;
            } else uiWhirlwindTimer-=diff;

            if (uiFearTimer<=diff)
            {
                DoCast(me,SPELL_FRIGHTENING_SHOUT);
                uiFearTimer=20000;
            } else uiFearTimer-=diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);
        }

        void KilledUnit(Unit* /*pVictim*/)
        {
            DoScriptText(SAY_KILL, me);
        }
    };

};

void AddSC_boss_commander_kolurg()
{
    new boss_commander_kolurg();
}
