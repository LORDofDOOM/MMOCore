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
 */

/* ScriptData
SDName: professionnpc
SD%Complete: 100
SDComment: Profession Master
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "../../shared/Configuration/Config.h"
#ifndef _TRINITY_CORE_CONFIG
# define _TRINITY_CORE_CONFIG  "worldserver.conf"
#endif _TRINITY_CORE_CONFIG

bool GossipHello_ProfessionNPC(Player* pPlayer, Creature* pCreature)
{
	Config TScriptConfig;
	if(!TScriptConfig.SetSource(_TRINITY_CORE_CONFIG,true))
		sLog.outError("TScript: Unable to open configuration file");

	if(TScriptConfig.GetBoolDefault("ProfessionNPC.OnlyGMs", false)) // If ProfessionNPC.OnlyGMs is enabled in trinitycore.conf
		if (pPlayer->GetSession()->GetSecurity() == SEC_PLAYER)
		{
			pCreature->MonsterWhisper("Sorry, I can only trade to game masters.", pPlayer->GetGUID());
			return true;
		}

	bool EnableProfessions = TScriptConfig.GetBoolDefault("ProfessionNPC.EnableProfessions", true);
	bool EnableSecondarySkills = TScriptConfig.GetBoolDefault("ProfessionNPC.EnableSecondarySkills", true);

	// Main Menu for Alliance
    if (pPlayer->GetTeam() == ALLIANCE)
	{
	// Check config if "Professions" is enabled or not
	if(EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Secondary Skills" is enabled or not
	if(EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}
	else // Main Menu for Horde
	{
	// Check config if "Professions" is enabled or not
	if(EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Secondary Skills" is enabled or not
	if(EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

return true;
}

void SendDefaultMenu_ProfessionNPC(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

// Not allow in combat
if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
	return;
}

Config TScriptConfig;
if(!TScriptConfig.SetSource(_TRINITY_CORE_CONFIG,true))
		sLog.outError("TScript: Unable to open configuration file");
	bool EnableProfessions = TScriptConfig.GetBoolDefault("ProfessionNPC.EnableProfessions", true);
	bool EnableSecondarySkills = TScriptConfig.GetBoolDefault("ProfessionNPC.EnableSecondarySkills", true);

//Mony Check
if (pPlayer->GetMoney() < (TScriptConfig.GetFloatDefault("SkillGoldCost",0)))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("You don't have enough money.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

case 1000: //Profession
            pPlayer->ADD_GOSSIP_ITEM( 5, "Alchemy."              , GOSSIP_SENDER_MAIN, 1001);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Blacksmithing."        , GOSSIP_SENDER_MAIN, 1002);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Enchanting."           , GOSSIP_SENDER_MAIN, 1003);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Engineering."          , GOSSIP_SENDER_MAIN, 1004);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Herbalism."            , GOSSIP_SENDER_MAIN, 1005);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Inscription."          , GOSSIP_SENDER_MAIN, 1006);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Jewelcrafting."        , GOSSIP_SENDER_MAIN, 1007);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Leatherworking."       , GOSSIP_SENDER_MAIN, 1008);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Mining."               , GOSSIP_SENDER_MAIN, 1009);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Skinning."             , GOSSIP_SENDER_MAIN, 1010);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Tailoring."            , GOSSIP_SENDER_MAIN, 1011);
            pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 3000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000: //Secondary Skills
            pPlayer->ADD_GOSSIP_ITEM( 5, "Cooking."              , GOSSIP_SENDER_MAIN, 2001);
            pPlayer->ADD_GOSSIP_ITEM( 5, "First Aid."            , GOSSIP_SENDER_MAIN, 2002);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Fishing."              , GOSSIP_SENDER_MAIN, 2003);
          //pPlayer->ADD_GOSSIP_ITEM( 5, "Riding."               , GOSSIP_SENDER_MAIN, 2004);
            pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 3000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000: //Back To Main Menu
	// Main Menu for Alliance
    if (pPlayer->GetTeam() == ALLIANCE)
	{
	// Check config if "Professions" is enabled or not
	if(EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Secondary Skills" is enabled or not
	if(EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}
	else // Main Menu for Horde
	{
	// Check config if "Professions" is enabled or not
	if(EnableProfessions)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	// Check config if "Secondary Skills" is enabled or not
	if(EnableSecondarySkills)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

//////////////////////////////////////////////////Professions///////////////////////////////////////////////////////////////

case 1001: // Alchemy
	if(!pPlayer->UpdateSkill(171,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(171,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1002: // Blacksmithing
	if(!pPlayer->UpdateSkill(164,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(164,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1003: // Enchanting
	if(!pPlayer->UpdateSkill(333,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(333,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1004: // Engineering
	if(!pPlayer->UpdateSkill(202,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(202,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1005: // Herbalism
	if(!pPlayer->UpdateSkill(182,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(182,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1006: // Inscription
	if(!pPlayer->UpdateSkill(773,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(773,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1007: // Jewelcrafting
	if(!pPlayer->UpdateSkill(755,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(755,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1008: // Leatherworking
	if(!pPlayer->UpdateSkill(165,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(165,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1009: // Mining
	if(!pPlayer->UpdateSkill(186,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(186,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1010: // Skinning
	if(!pPlayer->UpdateSkill(393,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(393,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1011: // Tailoring
	if(!pPlayer->UpdateSkill(197,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(197,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Secondary Skills///////////////////////////////////////////////////////////////

case 2001: // Cooking
	if(!pPlayer->UpdateSkill(185,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(185,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2002: // First Aid
	if(!pPlayer->UpdateSkill(129,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(129,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2003: // Fishing
	if(!pPlayer->UpdateSkill(356,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_ProfessionNPC(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(356,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//case 2004: // Riding
//	if(!pPlayer->UpdateSkill(,0))
//    {
//		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
//		GossipHello_ProfessionNPC(pPlayer, pCreature);
//		return;
//	}
//
//	pPlayer->UpdateSkill(,(TScriptConfig.GetFloatDefault("SkillPoints",0)));
//	pPlayer->ModifyMoney(-(TScriptConfig.GetFloatDefault("SkillGoldCost",0)));
//	pPlayer->CLOSE_GOSSIP_MENU();
//break;

 pPlayer->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function



bool GossipSelect_ProfessionNPC(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_ProfessionNPC(pPlayer, pCreature, uiAction);

return true;
}

void AddSC_professionnpc()
{
    Script *newscript;

	newscript = new Script;
    newscript->Name = "professionnpc";
    newscript->pGossipHello = &GossipHello_ProfessionNPC;
    newscript->pGossipSelect = &GossipSelect_ProfessionNPC;
    newscript->RegisterSelf();
}