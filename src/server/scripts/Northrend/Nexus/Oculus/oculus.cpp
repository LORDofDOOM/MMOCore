/*
 * Copyright (C) 2008 - 2010 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
 * Author: Bondiano
 */
 
#include "ScriptPCH.h"
#include "oculus.h"

#define GOSSIP_ITEM_DRAKES         "So where do we go from here?"
#define GOSSIP_ITEM_BELGARISTRASZ1 "I want to fly on the wings of the Red Flight"
#define GOSSIP_ITEM_BELGARISTRASZ2 "What abilities do Ruby Drakes have?"
#define GOSSIP_ITEM_VERDISA1       "I want to fly on the wings of the Green Flight"
#define GOSSIP_ITEM_VERDISA2       "What abilities do Emerald Drakes have?"
#define GOSSIP_ITEM_ETERNOS1       "I want to fly on the wings of the Bronze Flight"
#define GOSSIP_ITEM_ETERNOS2       "What abilities do Amber Drakes have?"

#define GO_ORB_OF_NEXUS     188715
#define GO_NEXUS_PORTAL     189985

#define HAS_ESSENCE(a) ((a)->HasItemCount(ITEM_EMERALD_ESSENCE,1) || (a)->HasItemCount(ITEM_AMBER_ESSENCE,1) || (a)->HasItemCount(ITEM_RUBY_ESSENCE,1))

enum Drakes
{
    GOSSIP_TEXTID_DRAKES                          = 13267,
    GOSSIP_TEXTID_BELGARISTRASZ1                  = 12916,
    GOSSIP_TEXTID_BELGARISTRASZ2                  = 13466,
    GOSSIP_TEXTID_BELGARISTRASZ3                  = 13254,
    GOSSIP_TEXTID_VERDISA1                        = 1,
    GOSSIP_TEXTID_VERDISA2                        = 1,
    GOSSIP_TEXTID_VERDISA3                        = 1,
    GOSSIP_TEXTID_ETERNOS1                        = 1,
    GOSSIP_TEXTID_ETERNOS2                        = 1,
    GOSSIP_TEXTID_ETERNOS3                        = 13256,

    ITEM_EMERALD_ESSENCE                          = 37815,
    ITEM_AMBER_ESSENCE                            = 37859,
    ITEM_RUBY_ESSENCE                             = 37860,

	//spells
	SPELL_PARACHUTE								  = 61243    
};

class mob_centrifige_construct : public CreatureScript
{
public:
    mob_centrifige_construct() : CreatureScript("mob_centrifige_construct") { }

    struct mob_CentrifigeConstructAI : public ScriptedAI
    {
        mob_CentrifigeConstructAI(Creature *c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
        }

        InstanceScript* pInstance;


	    void DismountPlayers()
	    {
		    std::list<HostileReference*>& m_threatlist = me->getThreatManager().getThreatList();
		    std::list<HostileReference*>::const_iterator i = m_threatlist.begin();
		    for (i = m_threatlist.begin(); i!= m_threatlist.end(); ++i)
		    {
			    Unit* pUnit = Unit::GetUnit((*me), (*i)->getUnitGuid());
			    if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER) )
			    {
				    Vehicle* v = pUnit->GetVehicle();
				    if(v)
				    {
					    pUnit->ExitVehicle();
					    v->Dismiss();
					    DoCast(pUnit,SPELL_PARACHUTE);					
				    }
			    }
		    }
	    }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;
		    DismountPlayers();
            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* killer)
        {
            if (pInstance)
                pInstance->SetData(DATA_CENTRIFUGE_CONSTRUCT_EVENT, pInstance->GetData(DATA_CENTRIFUGE_CONSTRUCT_EVENT)+1);
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_CentrifigeConstructAI (pCreature);
    };
};

class npc_oculus_drake : public CreatureScript
{
    public:
    npc_oculus_drake() : CreatureScript("npc_oculus_drake") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if (pCreature->GetInstanceScript()->GetData(DATA_DRAKOS_EVENT) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRAKES, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DRAKES, pCreature->GetGUID());
        }

        return true;
    };

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch(pCreature->GetEntry())
        {
        case NPC_VERDISA: //Verdisa
            switch(uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(pPlayer))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA1, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_VERDISA2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA2, pCreature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_EMERALD_ESSENCE, 1);
                if (msg == EQUIP_ERR_OK)
                    pPlayer->StoreNewItem(dest, ITEM_EMERALD_ESSENCE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_VERDISA3, pCreature->GetGUID());
                break;
            }
            break;
        case NPC_BELGARISTRASZ: //Belgaristrasz
            switch(uiAction)
            class oculus_teleporter : public GameObjectScript
};
public:
    oculus_teleporter() : GameObjectScript("oculus_teleporter") { }

    bool OnGossipHello(Player* pPlayer, GameObject *pGo)
    {
        if(pGo->GetEntry() == GO_ORB_OF_NEXUS)
            pPlayer->TeleportTo(571, 3872.43f, 6984.34f, 108.055f, 6.20f);
        if(pGo->GetEntry() == GO_NEXUS_PORTAL)
            pPlayer->TeleportTo(578, 988.303f, 1056.88f, 359.967f, 3.42f);

        return true;
    }	
};	
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(pPlayer))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ1, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BELGARISTRASZ2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ2, pCreature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_RUBY_ESSENCE, 1);
                if (msg == EQUIP_ERR_OK)
                    pPlayer->StoreNewItem(dest, ITEM_RUBY_ESSENCE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_BELGARISTRASZ3, pCreature->GetGUID());
                break;
            }
            break;
        case NPC_ETERNOS: //Eternos
            switch(uiAction)
            {
            case GOSSIP_ACTION_INFO_DEF + 1:
                if (!HAS_ESSENCE(pPlayer))
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS1, pCreature->GetGUID());
                }
                else
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ETERNOS2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS2, pCreature->GetGUID());
                }
                break;
            case GOSSIP_ACTION_INFO_DEF + 2:
            {
                ItemPosCountVec dest;
                uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_AMBER_ESSENCE, 1);
                if (msg == EQUIP_ERR_OK)
                    pPlayer->StoreNewItem(dest, ITEM_AMBER_ESSENCE, true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 3:
                pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ETERNOS3, pCreature->GetGUID());
                break;
            }
            break;
        }

        return true;
    };
};

void AddSC_oculus()
{
    new npc_oculus_drake();
    new oculus_teleporter();
	new mob_centrifige_construct();
}
