/*
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
 *
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
#include "icecrown_citadel.h"
#include "MapManager.h"
#include "Transport.h"
class npc_muradin_bronzebeard_gunship : public CreatureScript
{
    public:
        npc_muradin_bronzebeard_gunship() : CreatureScript("npc_myradin_icc") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature)
        {
            InstanceScript* pInstance = pCreature->GetInstanceScript();
            if (pInstance && pInstance->GetBossState(DATA_GUNSHIP_EVENT) != DONE)
            {
                if (!pCreature->GetTransport())
                {
            pPlayer->ADD_GOSSIP_ITEM(0, "START", 631, 1000);
                //      } if(pPlayer->GetSecurity())
                }else
                pPlayer->ADD_GOSSIP_ITEM(0, "My companions are all accounted for, Saurfang. Let's go!", 631, 1001);
                pPlayer->ADD_GOSSIP_ITEM(0, "DESPAWN", 631, 1003);
                                        
                        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                return true;
            }

            return false;
        }

        bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action)
        {
            pPlayer->PlayerTalkClass->ClearMenus();
            pPlayer->CLOSE_GOSSIP_MENU();
            if (action == 1000)
            {

            uint32 entry = 201812;
            const GameObjectInfo *goinfo = sObjectMgr->GetGameObjectInfo(entry);
               if (!goinfo)
                             // sLog.outErrorDb("Transport ID:%u, Name: %s, will not be loaded, gameobject_template missing", entry, name.c_str());
                    return false;
                
                Transport *t = new Transport(51584, goinfo->ScriptId);
                                       
               std::set<uint32> unused;
                if (!t->GenerateWaypoints(goinfo->moTransport.taxiPathId, unused))
                {
                    delete t;
                    return false;
                }

                theguid = sObjectMgr->GenerateLowGuid(HIGHGUID_MO_TRANSPORT);
                // creates the Gameobject
                if (!t->Create(theguid, entry, t->m_WayPoints.begin()->second.mapid, t->m_WayPoints.begin()->second.x, t->m_WayPoints.begin()->second.y, t->m_WayPoints.begin()->second.z, 0.0f, 0, 0))
                {
                    delete t;
                    return false;
               }

               Map* tMap = pPlayer->GetMap();
              //  t->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0x10830010);   // dunno, seen in sniffs 
                //t->SetFloatValue(GAMEOBJECT_PARENTROTATION+3, 1.0f);
                t->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_IN_USE);
                t->SetGoState(GO_STATE_READY);

                t->SetMap(tMap);
                t->AddToWorld();

                // transmit creation packet
                for (Map::PlayerList::const_iterator itr = tMap->GetPlayers().begin(); itr != tMap->GetPlayers().end(); ++itr)
                {
                    if (Player* pPlayer = itr->getSource())
                    {
                        UpdateData transData;
                        t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
                        WorldPacket packet;
                        transData.BuildPacket(&packet);
                        pPlayer->SendDirectMessage(&packet);
                     
               }
                }

                 sMapMgr->m_Transports.insert(t); // for updating
                 
               
                               t->GetTransport()->AddNPCPassenger(0, 0, t->GetTransOffsetX(), t->GetTransOffsetY(), t->GetTransOffsetZ(), t->GetTransOffsetO());
           
                               
               UpdateData transData;
                
                t->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
                WorldPacket packet;
                transData.BuildPacket(&packet);
                pPlayer->GetSession()->SendPacket(&packet);
                t->UpdateNPCPositions();
                t->Update(1);
                t->BuildStopMovePacket(tMap);
                
                
            }
             
            if (action == 1001)
            {
        if (Transport* t = pCreature->GetTransport())
                {    
                pInstance->SetData(DATA_GUNSHIP_EVENT, IN_PROGRESS);
                skybreaker->BuildStartMovePacket(pPlayer->GetMap());
                skybreaker->SetGoState(GO_STATE_ACTIVE);
                skybreaker->Update(0);
                skybreaker->SetUInt32Value(GAMEOBJECT_DYNAMIC, 0x10830010);   // dunno, seen in sniffs
                skybreaker->SetFloatValue(GAMEOBJECT_PARENTROTATION+3, 1.0f);
               
                skybreaker->AddToWorld();
                UpdateData transData;
                skybreaker->BuildCreateUpdateBlockForPlayer(&transData, pPlayer);
                WorldPacket packet;
                transData.BuildPacket(&packet);
                pPlayer->GetSession()->SendPacket(&packet);
                Map* tMap = pPlayer->GetMap();
        
                skybreaker->BuildStopMovePacket(tMap);
                                                                
                }    
            }
            else if (action == 1003)
            {
                Transport* skybreaker = pCreature->GetTransport();
                if (!skybreaker)
                    return true;

                sMapMgr->m_Transports.erase(skybreaker);
                for (Transport::PlayerSet::const_iterator itr = skybreaker->GetPassengers().begin(); itr != skybreaker->GetPassengers().end(); ++itr)
                {
                    skybreaker->RemovePassenger(*itr);
                    if (Player* plr = *itr)
                        plr->SetTransport(NULL);
                }
                         uint64 n = 0;
                         for (Transport::CreatureSet::const_iterator itr = skybreaker->m_NPCPassengerSet.begin(); itr != skybreaker->m_NPCPassengerSet.end(); ++itr)
                   
                                 if (Creature *npc = Unit::GetCreature(*pPlayer, n++))
                       npc->AddObjectToRemoveList();
               


                skybreaker->m_NPCPassengerSet.clear();

                UpdateData transData;
                skybreaker->BuildOutOfRangeUpdateBlock(&transData);
                WorldPacket out_packet;
                transData.BuildPacket(&out_packet);

                for (Map::PlayerList::const_iterator itr = skybreaker->GetMap()->GetPlayers().begin(); itr != skybreaker->GetMap()->GetPlayers().end(); ++itr)
                    if (skybreaker != itr->getSource()->GetTransport())
                        itr->getSource()->SendDirectMessage(&out_packet);

                skybreaker->Delete();
                skybreaker = NULL;
            }

            return true;
        }
        uint64 theguid;
                Transport *skybreaker;
        private:
                 InstanceScript* pInstance;
                 
};

 
void AddSC_boss_gunship_battle()
{
    //new transport_gunship();
    new npc_muradin_bronzebeard_gunship();
}