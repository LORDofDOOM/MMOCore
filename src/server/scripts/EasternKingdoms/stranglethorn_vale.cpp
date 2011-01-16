/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Stranglethorn_Vale
SD%Complete: 100
SDComment: Quest support: 592
SDCategory: Stranglethorn Vale
EndScriptData */

/* ContentData
mob_yenniku
EndContentData */

#include "ScriptPCH.h"
#include "GameEventMgr.h"

/*######
## mob_yenniku
######*/

class mob_yenniku : public CreatureScript
{
public:
    mob_yenniku() : CreatureScript("mob_yenniku") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_yennikuAI (pCreature);
    }

    struct mob_yennikuAI : public ScriptedAI
    {
        mob_yennikuAI(Creature *c) : ScriptedAI(c)
        {
            bReset = false;
        }

        uint32 Reset_Timer;
        bool bReset;

        void Reset()
        {
            Reset_Timer = 0;
            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
        }

        void SpellHit(Unit *caster, const SpellEntry *spell)
        {
            if (caster->GetTypeId() == TYPEID_PLAYER)
            {
                                                                //Yenniku's Release
                if (!bReset && CAST_PLR(caster)->GetQuestStatus(592) == QUEST_STATUS_INCOMPLETE && spell->Id == 3607)
                {
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);
                    me->CombatStop();                   //stop combat
                    me->DeleteThreatList();             //unsure of this
                    me->setFaction(83);                 //horde generic

                    bReset = true;
                    Reset_Timer = 60000;
                }
            }
            return;
        }

        void EnterCombat(Unit * /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (bReset)
            {
                if (Reset_Timer <= diff)
                {
                    EnterEvadeMode();
                    bReset = false;
                    me->setFaction(28);                     //troll, bloodscalp
                    return;
                }
                else Reset_Timer -= diff;

                if (me->isInCombat() && me->getVictim())
                {
                    if (me->getVictim()->GetTypeId() == TYPEID_PLAYER)
                    {
                        Unit *victim = me->getVictim();
                        if (CAST_PLR(victim)->GetTeam() == HORDE)
                        {
                            me->CombatStop();
                            me->DeleteThreatList();
                        }
                    }
                }
             }

            //Return since we have no target
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

};

/*######
## npc_riggle_bassbait Evento Stranglethorn Fishing Extravaganza
######*/

enum Riggle
{
    SAY_START               = -1000356,
    SAY_WINNER              = -1000357,
    SAY_END                 = -1000358,
    QUEST_MASTER_ANGLER     = 8193
};

class npc_riggle_bassbait : public CreatureScript
{
public:
    npc_riggle_bassbait() : CreatureScript("npc_riggle_bassbait") {}

    struct npc_riggle_bassbaitAI : public ScriptedAI
    {
        npc_riggle_bassbaitAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            bEventoEmpieza = bEventoTerminado = bEventoGanadorBusqueda = false;
            Reset();        
        }

        bool bEventoEmpieza;
        bool bEventoTerminado;
        bool bEventoGanadorBusqueda;

        void Reset() {}

        void Aggro(Unit *who) {}

        void UpdateAI(const uint32 diff)
        {
            if (!bEventoEmpieza && time(NULL) % 60 == 0 && IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
            {
                sLog->outDebug("Extravaganza: npc_riggle_bassbait Anuncia Comienzo HOLIDAY_FISHING_EXTRAVAGANZA");
                DoScriptText(SAY_START, me);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                bEventoEmpieza = true;
            }

            if (bEventoEmpieza && !bEventoTerminado && time(NULL) % 60 == 0 && !IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
            {
                sLog->outDebug("Extravaganza: npc_riggle_bassbait Anuncia Termino HOLIDAY_FISHING_EXTRAVAGANZA");
                DoScriptText(SAY_END, me);
                bEventoTerminado = true;
            }
            return;
            DoMeleeAttackIfReady();
        }
    };

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
        {
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
            pPlayer->SEND_GOSSIP_MENU(7614, pCreature->GetGUID());//Gossip DB
            return true;
        }

        pPlayer->SEND_GOSSIP_MENU(7714, pCreature->GetGUID());//Gossip DB
        return true;
    }

    bool ChooseReward(Player* pPlayer, Creature* pCreature, const Quest *pQuest, uint32 item)
    {
        if (pQuest->GetQuestId() == QUEST_MASTER_ANGLER && ((npc_riggle_bassbaitAI*)(pCreature->AI()))->bEventoGanadorBusqueda == false)
        {
            DoScriptText(SAY_WINNER, pCreature,pPlayer);
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            ((npc_riggle_bassbaitAI*)(pCreature->AI()))->bEventoGanadorBusqueda = true;
            Unit* creature2 = Unit::GetUnit((*pCreature),MAKE_NEW_GUID(54687,15078,HIGHGUID_UNIT));
            if (creature2)
            {
                creature2->SetFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER);
            }
            else
            {
                sLog->outDebug("Extravaganza:No se puede cambiar la flag de Jank Creature2");
            }
            return true;
       }
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_riggle_bassbaitAI (pCreature);
    }
};

void AddSC_stranglethorn_vale()
{
    new mob_yenniku();
    new npc_riggle_bassbait();
}