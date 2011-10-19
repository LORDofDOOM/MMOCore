/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
 *
 * Created by LordPsyan for Flaminglegion.net
 *
 */

#include "ScriptPCH.h"
#include "Config.h"

#define GOSSIP_SENDER_MAIN      1000
#define SPELL_RESURRECTION_SICKNESS_15007  15007

class levelnpc : public CreatureScript
{
public:
    levelnpc() : CreatureScript("levelnpc") {}


bool OnGossipHello(Player* pPlayer, Creature* pCreature)
{
	if(sWorld->getBoolConfig(CONFIG_LEVELNPC_ONLYGMS)) // If LevelNPC.OnlyGMs is enabled in trinitycore.conf
		if (pPlayer->GetSession()->GetSecurity() == SEC_PLAYER)
		{
			pCreature->MonsterWhisper("Sorry ich kann nur EliteAccounts Level hinzufügen.", pPlayer->GetGUID());
			return true;
		}

	bool EnableLevel80 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL80);
	bool EnableLevel100 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL100);
	bool EnableLevel150 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL150);
	bool EnableLevel200 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL200);
	bool EnableLevel255 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL255);
	bool UseTokens 		= sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL255);	
    if (pPlayer->GetTeam() == ALLIANCE)
	{
		pPlayer->ADD_GOSSIP_ITEM( 7, "Instant Levels ->"        , GOSSIP_SENDER_MAIN, 1000);
		pPlayer->ADD_GOSSIP_ITEM( 10, "Todenachwirkungen entfernen" , GOSSIP_SENDER_MAIN, 5000);
	}
	else
	{
		pPlayer->ADD_GOSSIP_ITEM( 7, "Instant Levels ->"        , GOSSIP_SENDER_MAIN, 1000);
		pPlayer->ADD_GOSSIP_ITEM( 10, "Todenachwirkungen entfernen" , GOSSIP_SENDER_MAIN, 5000);
	}
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

return true;
}

void SendDefaultMenu(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

// Not allow in combat
if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("Du befindest dich im Kampf!", LANG_UNIVERSAL, NULL);
	return;
}

	bool EnableLevel80 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL80);
	bool EnableLevel100 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL100);
	bool EnableLevel150 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL150);
	bool EnableLevel200 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL200);
	bool EnableLevel255 = sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL255);
	bool UseTokens 		= sWorld->getBoolConfig(CONFIG_LEVELNPC_ENABLELEVEL255);	
	
//Mony Check
if(sWorld->getBoolConfig(CONFIG_LEVELNPC_USETOKENS))
{
if (!pPlayer->HasItemCount((sWorld->getIntConfig(CONFIG_LEVELNPC_ITEMENTRYNUM)), 0))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("Du hast keine Tokens.", pPlayer->GetGUID());
	return;
}
}
else if(pPlayer->GetMoney() < (sWorld->getIntConfig(CONFIG_LEVELNPC_SKILLGOLDCOST)))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("Du hast nicht genug Gold.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

//////////////////////////////////////////////////Leveling///////////////////////////////////////////////////////////////
case 1000: //Leveling
	if(EnableLevel80 && pPlayer->getLevel() < 80 && UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(1 Donation Chip) Instant 80 ->"         , GOSSIP_SENDER_MAIN, 1001);
	if(EnableLevel80 && pPlayer->getLevel() < 80 && !UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(100 Gold) Instant 80 ->"         , GOSSIP_SENDER_MAIN, 1001);	
	if(EnableLevel100 && pPlayer->getLevel() < 100 && pPlayer->getLevel() >= 80 && UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(2 Donation Chips) Instant 100 ->"        , GOSSIP_SENDER_MAIN, 1002);
	if(EnableLevel100 && pPlayer->getLevel() < 100 && pPlayer->getLevel() >= 80 && !UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(2 Donation Chips) Instant 100 ->"        , GOSSIP_SENDER_MAIN, 1002);
	if(EnableLevel150 && pPlayer->getLevel() < 150 && pPlayer->getLevel() >= 100 && UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(5 Donation Chips) Instant 150 ->"        , GOSSIP_SENDER_MAIN, 1003);
	if(EnableLevel150 && pPlayer->getLevel() < 150 && pPlayer->getLevel() >= 100 && !UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(5 Donation Chips) Instant 150 ->"        , GOSSIP_SENDER_MAIN, 1003);
	if(EnableLevel200 && pPlayer->getLevel() < 200 && pPlayer->getLevel() >= 150 && UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(10 Donation Chips) Instant 200 ->"       , GOSSIP_SENDER_MAIN, 1004);
	if(EnableLevel200 && pPlayer->getLevel() < 200 && pPlayer->getLevel() >= 150 && !UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(10 Donation Chips) Instant 200 ->"       , GOSSIP_SENDER_MAIN, 1004);
	if(EnableLevel255 && pPlayer->getLevel() < 255 && pPlayer->getLevel() >= 200 && UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(20 Donation Chips) Instant 255 ->"       , GOSSIP_SENDER_MAIN, 1005);
	if(EnableLevel255 && pPlayer->getLevel() < 255 && pPlayer->getLevel() >= 200 && !UseTokens)
		pPlayer->ADD_GOSSIP_ITEM( 7, "(20 Donation Chips) Instant 255 ->"       , GOSSIP_SENDER_MAIN, 1005);		
		pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                       , GOSSIP_SENDER_MAIN, 3000);
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;
case 3000: //Leveling
	if(EnableLevel80)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Instant Levels ->"        , GOSSIP_SENDER_MAIN, 1000);
    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;
case 1001: // Leveling
	pPlayer->GiveLevel(80);
	if(sWorld->getBoolConfig(CONFIG_LEVELNPC_USETOKENS))
	pPlayer->DestroyItemCount(99998, 1, true);
	else
	pPlayer->ModifyMoney(-(sWorld->getIntConfig(CONFIG_LEVELNPC_SKILLGOLDCOST)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;
case 1002: // Leveling
	pPlayer->GiveLevel(100);
	if(sWorld->getBoolConfig(CONFIG_LEVELNPC_USETOKENS))
	pPlayer->DestroyItemCount(99998, 2, true);
	else
	pPlayer->ModifyMoney(-(sWorld->getIntConfig(CONFIG_LEVELNPC_SKILLGOLDCOST)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;
case 1003: // Leveling
	pPlayer->GiveLevel(150);
	if(sWorld->getBoolConfig(CONFIG_LEVELNPC_USETOKENS))
	pPlayer->DestroyItemCount(99998, 5, true);
	else
	pPlayer->ModifyMoney(-(sWorld->getIntConfig(CONFIG_LEVELNPC_SKILLGOLDCOST)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;
case 1004: // Leveling
	pPlayer->GiveLevel(200);
	if(sWorld->getBoolConfig(CONFIG_LEVELNPC_USETOKENS))
	pPlayer->DestroyItemCount(99998, 10, true);
	else
	pPlayer->ModifyMoney(-(sWorld->getIntConfig(CONFIG_LEVELNPC_SKILLGOLDCOST)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;
case 1005: // Leveling
	pPlayer->GiveLevel(255);
	if(sWorld->getBoolConfig(CONFIG_LEVELNPC_USETOKENS))
	pPlayer->DestroyItemCount(99998, 20, true);
	else
	pPlayer->ModifyMoney(-(sWorld->getIntConfig(CONFIG_LEVELNPC_SKILLGOLDCOST)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;
case 5000://Remove Res Sickness
	if(!pPlayer->HasAura(SPELL_RESURRECTION_SICKNESS_15007,0))
	{
		pCreature->MonsterWhisper("Du hast keine Todesnachwirkungen", pPlayer->GetGUID());
		OnGossipHello(pPlayer, pCreature);
		return;
	}

	pCreature->CastSpell(pPlayer,38588,false); // Healing effect
	pPlayer->RemoveAurasDueToSpell(SPELL_RESURRECTION_SICKNESS_15007,0);
	pPlayer->CLOSE_GOSSIP_MENU();
break;

 pPlayer->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function

bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	pPlayer->PlayerTalkClass->ClearMenus();
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu(pPlayer, pCreature, uiAction);

return true;
}
};

void AddSC_levelnpc()
{

new levelnpc();

}
