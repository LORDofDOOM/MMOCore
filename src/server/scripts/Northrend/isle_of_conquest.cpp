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
#include "BattlegroundIC.h"

// TO-DO: This should be done with SmartAI, but yet it does not correctly support vehicles's AIs.
//        Even adding ReactState Passive we still have issues using SmartAI.

class npc_four_car_garage : public CreatureScript
{
    public:
        npc_four_car_garage() : CreatureScript("npc_four_car_garage") {}

        struct npc_four_car_garageAI : public NullCreatureAI
        {
            npc_four_car_garageAI(Creature* pCreature) : NullCreatureAI(pCreature) { }

            void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
            {
                if (apply)
                {
                    uint32 spellId = 0;

                    switch(me->GetEntry())
                    {
                        case NPC_DEMOLISHER:
                            spellId = SPELL_DRIVING_CREDIT_DEMOLISHER;
                            break;
                        case NPC_GLAIVE_THROWER_A:
                        case NPC_GLAIVE_THROWER_H:
                            spellId = SPELL_DRIVING_CREDIT_GLAIVE;
                            break;
                        case NPC_SIEGE_ENGINE_H:
                        case NPC_SIEGE_ENGINE_A:
                            spellId = SPELL_DRIVING_CREDIT_SIEGE;
                            break;
                        case NPC_CATAPULT:
                            spellId = SPELL_DRIVING_CREDIT_CATAPULT;
                            break;
                        default:
                            return;
                    }

                    me->CastSpell(who,spellId,true);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_four_car_garageAI(creature);
        }
};

/*#####
## Bosses Isle Of Conquest (A/H)
## NPC_HIGH_COMMANDER_HALFORD_WYRMBANE = 34924, // Alliance Boss
## NPC_OVERLORD_AGMAR = 34922, // Horde Boss
## Comment: AI Basico segun WOWHEAD, para una principal solucion del exploit de LOS del BG.
##          Futuro Sniffed de blizzard por (Edward). para sonidos textos y reales spells.
#####*/

enum SpellsBosses
{
    SPELL_DAGGER_THROW_1    = 67280,
    SPELL_DAGGER_THROW_2    = 67881,
    SPELL_CRUSHING_LEAP     = 68506,
    SPELL_MORTAL_STRIKE     = 39171,
};

class bosses_isle_of_conquest : public CreatureScript
{
public:
    bosses_isle_of_conquest() : CreatureScript("bosses_isle_of_conquest") { }

    struct bosses_isle_of_conquestAI : public ScriptedAI
    {
        bosses_isle_of_conquestAI(Creature *c) : ScriptedAI(c) { }

        uint32 uiMortalStrikeTimer;
        uint32 uiDaggerThrowTimer;
        uint32 uiCrushingLeapTimer;
        uint32 uiResetTimer;

        void Reset()
        {
            uiMortalStrikeTimer = 8*IN_MILLISECONDS;
            uiDaggerThrowTimer  = 2*IN_MILLISECONDS;
            uiCrushingLeapTimer = 6*IN_MILLISECONDS;
            uiResetTimer        = 5*IN_MILLISECONDS;
        }
        
        void EnterCombat(Unit * who)
        {
            if(!me->IsWithinLOSInMap(who))// Siguie LOS
            {
                EnterEvadeMode();
            }
        }

        void JustRespawned()
        {
            Reset();
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (uiMortalStrikeTimer < diff)
            {
                DoCast(me->getVictim(), SPELL_MORTAL_STRIKE);
                uiMortalStrikeTimer = urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS);
            }
            else
                uiMortalStrikeTimer -= diff;

            if (uiDaggerThrowTimer < diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, RAID_MODE(SPELL_DAGGER_THROW_1, SPELL_DAGGER_THROW_2));
                uiDaggerThrowTimer = urand(7*IN_MILLISECONDS,12*IN_MILLISECONDS);
            }
            else
                uiDaggerThrowTimer -= diff;

            if (uiCrushingLeapTimer < diff)
            {
                if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    DoCast(pTarget, SPELL_CRUSHING_LEAP);
                uiCrushingLeapTimer = urand(12*IN_MILLISECONDS,16*IN_MILLISECONDS);
            }
            else
                uiCrushingLeapTimer -= diff;
            
            // Chequea si la creatura no esta al exterior del edificio (Antibugers >3)
            if (uiResetTimer < diff)
            {
                float x,y;
                me->GetPosition(x,y);

                if (me->GetEntry()== NPC_OVERLORD_AGMAR && (x > 1348 || x < 1283 || y < -800 || y > -730))
                {
                    me->Kill(me->getVictim());
                    me->MonsterSay("Antibugers:$N, No querias bugear rqlo :)", LANG_UNIVERSAL, me->GetGUID());
                    EnterEvadeMode();
                }

                if (me->GetEntry()== NPC_HIGH_COMMANDER_HALFORD_WYRMBANE && (x > 288 || x < 216 || y < -863 || y > -800))
                {
                    EnterEvadeMode();
                    me->MonsterSay("Antibugers:$N, No querias bugear rqlo :)", LANG_UNIVERSAL, me->GetGUID());
                    me->Kill(me->getVictim());
                }
                uiResetTimer = 200;
            }
            else   
                uiResetTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new bosses_isle_of_conquestAI(creature);
    }
};

void AddSC_isle_of_conquest()
{
    new npc_four_car_garage();
    new bosses_isle_of_conquest();
}
