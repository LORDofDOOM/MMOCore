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
 */

#include "ScriptPCH.h"
#include "eye_of_eternity.h"
#include "WorldPacket.h"

enum Talks
{
    SAY_OUTRO_1                     = -1616030,
    SAY_OUTRO_2                     = -1616031,
    SAY_OUTRO_3                     = -1616032,
    SAY_OUTRO_4                     = -1616033,
    SAY_OUTRO_5                     = -1616034,
};

class npc_alexsrtaza : public CreatureScript
{
public:
    npc_alexsrtaza() : CreatureScript("npc_alexsrtaza") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_alexstrazaAI(pCreature);
    }

    struct npc_alexstrazaAI : public ScriptedAI
    {
        InstanceScript* m_pInstance;

        npc_alexstrazaAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_pInstance = (InstanceScript*)pCreature->GetInstanceScript();
        }
    };
};

void AddSC_npc_alexstraza()
{
    new npc_alexsrtaza();
}
