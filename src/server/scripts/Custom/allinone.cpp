/* SPGM Memeber of Trinitycore Server: http://immortalones.game-server.cc Repo: http://code.google.com/p/spgm-trinity/
*  This is a script I combined from Buff Npc and professions, I have redesigned it and added to it. To come: Enchanting, Guildhouse, TeleNpc2, Bots so this will be a all in one NPC with options in the config file and Database.
*/

/* ScriptData
SDName: pryds_allinone
SD%Complete: 100
SDComment: Buff Master Profession SkillUp Trainers
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"
#include "../../shared/Configuration/Config.h"
#ifndef _TRINITY_CORE_CONFIG
# define _TRINITY_CORE_CONFIG  "worldserver.conf"
#endif _TRINITY_CORE_CONFIG


class pryds_allinone : public CreatureScript
{
    public:

        pryds_allinone()
            : CreatureScript("pryds_allinone")
        {
        }

        struct pryds_allinoneAI : public ScriptedAI
        {
	    //*** HANDLED FUNCTION ***
        //This is the constructor, called only once when the Creature is first created
            pryds_allinoneAI(Creature *c) : ScriptedAI(c) {}   
        }; 



bool OnGossipHello(Player  *player, Creature* pCreature)
{
	//Config sConfig;
	if (!ConfigMgr::Load(_TRINITY_CORE_CONFIG))
		sLog->outInfo(LOG_FILTER_GENERAL, "TScript: Unable to open configuration file");

	if (ConfigMgr::GetBoolDefault("pryds_allinone.OnlyGMs", false)) // If pryds_allinone.OnlyGMs is enabled in trinitycore.conf
		if (player->GetSession()->GetSecurity() == SEC_PLAYER)
		{
			pCreature->MonsterWhisper("Sorry, I am set only for Game Masters.", player->GetGUID());
			return true;
		}

	bool EnableBuffs = ConfigMgr::GetBoolDefault("pryds_allinone.EnableBuffs", true);
	bool EnableProfessionsSkillUp = ConfigMgr::GetBoolDefault("pryds_allinone.EnableProfessionsSkillUp", true);
	bool EnableTrainers = ConfigMgr::GetBoolDefault("pryds_allinone.EnableTrainers", true);
	bool EnableBots = ConfigMgr::GetBoolDefault("pryds_allinone.EnableBots", true);
	bool EnablePlayerTools = ConfigMgr::GetBoolDefault("pryds_allinone.EnablePlayerTools", true);
	bool EnableRemoveResSickness = ConfigMgr::GetBoolDefault("pryds_allinone.EnableRemoveResSickness", true);
	bool EnableMorph = ConfigMgr::GetBoolDefault("pryds_allinone.EnableMorph", true);

	// Main Menu for Alliance
    if (player->GetTeam() == ALLIANCE)
	{
	// Buffs
	if (EnableBuffs)
		player->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		player->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Bots
	if (EnableBots)
		player->ADD_GOSSIP_ITEM(3, "Bots ->"  				  , GOSSIP_SENDER_MAIN, 600);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		player->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
		// Check config if "Morph" is enabled or not
	if (EnableMorph)
		player->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}
	else // Main Menu for Horde
	{
	// Buffs
	if (EnableBuffs)
		player->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		player->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		player->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
	// Check config if "Morph" is enabled or not
	if (EnableMorph)
		player->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}

	// Check config file if "Remove res sickness" option is enabled or not
	if (EnableRemoveResSickness)
		player->ADD_GOSSIP_ITEM(9, "Remove Resurrect Sickness" , GOSSIP_SENDER_MAIN, 7000);

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

return true;
}

void SendDefaultMenu_pryds_allinone(Player  *player, Creature* pCreature, uint32 uiAction)
{

// Not allow in combat
if (player->isInCombat())
{
    player->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
	return;
}

//Config sConfig;
if (!ConfigMgr::Load(_TRINITY_CORE_CONFIG))
		sLog->outInfo(LOG_FILTER_GENERAL, "TScript: Unable to open configuration file");
	bool EnableBuffs = ConfigMgr::GetBoolDefault("pryds_allinone.EnableBuffs", true);
	bool EnableProfessionsSkillUp = ConfigMgr::GetBoolDefault("pryds_allinone.EnableProfessionsSkillUp", true);
	bool EnableSmallBuff = ConfigMgr::GetBoolDefault("pryds_allinone.EnableSmallBuff", true);
	bool EnableGreatBuff = ConfigMgr::GetBoolDefault("pryds_allinone.EnableGreatBuff", true);
	bool EnableGMBuff = ConfigMgr::GetBoolDefault("pryds_allinone.EnableGMBuff", true);
	bool EnableTrainers = ConfigMgr::GetBoolDefault("pryds_allinone.EnableTrainers", true);
	bool EnableClassTrainers = ConfigMgr::GetBoolDefault("pryds_allinone.EnableClassTrainers", true);
	bool EnableProfessionsTrainers = ConfigMgr::GetBoolDefault("pryds_allinone.EnableProfessionsTrainers", true);
	bool EnablePlayerTools = ConfigMgr::GetBoolDefault("pryds_allinone.EnablePlayerTools", true);
	bool EnableRemoveResSickness = ConfigMgr::GetBoolDefault("pryds_allinone.EnableRemoveResSickness", true);
	bool EnableGivemeGold = ConfigMgr::GetBoolDefault("pryds_allinone.EnableGivemeGold", true);
	bool EnableProfessions = ConfigMgr::GetBoolDefault("pryds_allinone.EnableProfessions", true);
	bool EnableSecondarySkills = ConfigMgr::GetBoolDefault("pryds_allinone.EnableSecondarySkills", true);
	bool EnableMorph = ConfigMgr::GetBoolDefault("pryds_allinone.EnableMorph", true);

//Mony Check
if (player->GetMoney() < (ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)))
{
    player->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("You don't have enough money.", player->GetGUID());
	return;
}

switch(uiAction)
{

case 100: // Buffs
	if (EnableSmallBuff)
		player->ADD_GOSSIP_ITEM(2, "Small Buff ->"   			               , GOSSIP_SENDER_MAIN, 1000);
	if (EnableGreatBuff)
		player->ADD_GOSSIP_ITEM(2, "Great Buff ->"                           , GOSSIP_SENDER_MAIN, 2000);
	if (EnableGMBuff)
		player->ADD_GOSSIP_ITEM(2, "GM Buff ->"    			               , GOSSIP_SENDER_MAIN, 3100);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 200: //Professions SkillUp
	// Check config if "EnableProfessions" is enabled or not
	if (EnableProfessions)
		player->ADD_GOSSIP_ITEM(3, "Professions ->"       				   , GOSSIP_SENDER_MAIN, 3000);
	// Check config if "EnableSecondarySkills" is enabled or not
	if (EnableSecondarySkills)
		player->ADD_GOSSIP_ITEM(3, "Secondary Skills ->"  				   , GOSSIP_SENDER_MAIN, 6000);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 300: //Trainers
	// Check config if "Class Trainers" is enabled or not
	if (EnableClassTrainers)
		player->ADD_GOSSIP_ITEM(3, "Class Trainers ->"       			       , GOSSIP_SENDER_MAIN, 400);
	// Check config if "Profession Trainers" is enabled or not
	if (EnableProfessionsTrainers)
		player->ADD_GOSSIP_ITEM(3, "Professions Trainers ->" 				   , GOSSIP_SENDER_MAIN, 500);
	// Professions Skillup
	if (EnableProfessionsSkillUp)
		player->ADD_GOSSIP_ITEM(3, "Professions SkillUp ->"  				   , GOSSIP_SENDER_MAIN, 200);
		player->ADD_GOSSIP_ITEM(3, "Mount Trainer"       					   , GOSSIP_SENDER_MAIN, 634);
		player->ADD_GOSSIP_ITEM(3, "Stable Master"  				           , GOSSIP_SENDER_MAIN, 612);
		player->ADD_GOSSIP_ITEM(3, "Weapon Master"  				           , GOSSIP_SENDER_MAIN, 613);
		player->ADD_GOSSIP_ITEM(3, "Max Weapon Skill"    				       , GOSSIP_SENDER_MAIN, 4026);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 700: //Morph
		player->ADD_GOSSIP_ITEM(4, "Famous ->"   		    			       , GOSSIP_SENDER_MAIN, 710);
		player->ADD_GOSSIP_ITEM(4, "Funny ->" 							   , GOSSIP_SENDER_MAIN, 750);
		player->ADD_GOSSIP_ITEM(4, "Make me Bigger ->" 					   , GOSSIP_SENDER_MAIN, 771);
		player->ADD_GOSSIP_ITEM(4, "Make me Smaller ->" 					   , GOSSIP_SENDER_MAIN, 772);
		player->ADD_GOSSIP_ITEM(4, "DeMorph ->" 							   , GOSSIP_SENDER_MAIN, 799);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 710: //Morph Famous
		player->ADD_GOSSIP_ITEM(4, "Algalon ->"  		    			       , GOSSIP_SENDER_MAIN, 711);
		player->ADD_GOSSIP_ITEM(4, "Doomwalker ->" 						   , GOSSIP_SENDER_MAIN, 712);
		player->ADD_GOSSIP_ITEM(4, "Doomlord kazzak ->" 					   , GOSSIP_SENDER_MAIN, 713);
		player->ADD_GOSSIP_ITEM(4, "Onyxia ->" 							   , GOSSIP_SENDER_MAIN, 714);
		player->ADD_GOSSIP_ITEM(4, "Thaddius ->" 							   , GOSSIP_SENDER_MAIN, 715);
		player->ADD_GOSSIP_ITEM(4, "Gluth ->" 							   , GOSSIP_SENDER_MAIN, 716);
		player->ADD_GOSSIP_ITEM(4, "Maexxna ->" 							   , GOSSIP_SENDER_MAIN, 717);
		player->ADD_GOSSIP_ITEM(4, "Sapphiron->" 							   , GOSSIP_SENDER_MAIN, 718);
		player->ADD_GOSSIP_ITEM(4, "Noth The Plaguebringer->"				   , GOSSIP_SENDER_MAIN, 719);
		player->ADD_GOSSIP_ITEM(4, "Kel'Thuzad ->" 						   , GOSSIP_SENDER_MAIN, 720);
		player->ADD_GOSSIP_ITEM(4, "Kael'thas Sunstrider ->" 				   , GOSSIP_SENDER_MAIN, 721);
		player->ADD_GOSSIP_ITEM(4, "C'thun ->" 							   , GOSSIP_SENDER_MAIN, 722);
		player->ADD_GOSSIP_ITEM(4, "Thrall ->" 							   , GOSSIP_SENDER_MAIN, 723);
		player->ADD_GOSSIP_ITEM(4, "DeMorph ->" 							   , GOSSIP_SENDER_MAIN, 799);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 700);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 750: //Morph Funny
		player->ADD_GOSSIP_ITEM(4, "Gerky ->"   	 		   			       , GOSSIP_SENDER_MAIN, 751);
		player->ADD_GOSSIP_ITEM(4, "Fire elemental ->" 					   , GOSSIP_SENDER_MAIN, 752);
		player->ADD_GOSSIP_ITEM(4, "Pandaren Monk ->" 					   , GOSSIP_SENDER_MAIN, 753);
		player->ADD_GOSSIP_ITEM(4, "Lil' K.T ->" 					 		   , GOSSIP_SENDER_MAIN, 754);
		player->ADD_GOSSIP_ITEM(4, "The Weapon you're Wearing ->"	 	       , GOSSIP_SENDER_MAIN, 755);
		player->ADD_GOSSIP_ITEM(4, "Red Bubbles ->" 					       , GOSSIP_SENDER_MAIN, 756);
		player->ADD_GOSSIP_ITEM(4, "A Shark ->"    		   			       , GOSSIP_SENDER_MAIN, 757);
		player->ADD_GOSSIP_ITEM(4, "A Fel cannon ->"   	   			       , GOSSIP_SENDER_MAIN, 758);
		player->ADD_GOSSIP_ITEM(4, "Human Female  ->" 	   			       , GOSSIP_SENDER_MAIN, 759);
		player->ADD_GOSSIP_ITEM(4, "A Fat Human ->" 		   			       , GOSSIP_SENDER_MAIN, 760);
		player->ADD_GOSSIP_ITEM(4, "A Rabbit ->" 	 		   			       , GOSSIP_SENDER_MAIN, 761);
		player->ADD_GOSSIP_ITEM(4, "A Cool looking Bug ->"   			       , GOSSIP_SENDER_MAIN, 762);
		player->ADD_GOSSIP_ITEM(4, "Eye Of C'thun ->"   	   			       , GOSSIP_SENDER_MAIN, 763);
		player->ADD_GOSSIP_ITEM(4, "DeMorph ->" 							   , GOSSIP_SENDER_MAIN, 799);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 700);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                	           , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 1000: //Small  Buff
		player->ADD_GOSSIP_ITEM(2, "Buff me Mark of the Wild"                , GOSSIP_SENDER_MAIN, 1001);
		player->ADD_GOSSIP_ITEM(2, "Buff me Thorns"                          , GOSSIP_SENDER_MAIN, 1005);
		player->ADD_GOSSIP_ITEM(2, "Buff me Amplify Magic"                   , GOSSIP_SENDER_MAIN, 1010);
		player->ADD_GOSSIP_ITEM(2, "Buff me Arcane Intellect"                , GOSSIP_SENDER_MAIN, 1015);
		player->ADD_GOSSIP_ITEM(2, "Buff me Dalaran Intellect"               , GOSSIP_SENDER_MAIN, 1020);
		player->ADD_GOSSIP_ITEM(2, "Buff me Dampen Magic"                    , GOSSIP_SENDER_MAIN, 1025);
		player->ADD_GOSSIP_ITEM(2, "Buff me Blessing of Kings"               , GOSSIP_SENDER_MAIN, 1030);
		player->ADD_GOSSIP_ITEM(2, "Buff me Blessing of Might"               , GOSSIP_SENDER_MAIN, 1035);
		player->ADD_GOSSIP_ITEM(2, "Buff me Blessing of Wisdom"              , GOSSIP_SENDER_MAIN, 1040);
		player->ADD_GOSSIP_ITEM(2, "Buff me Divine Spirit"                   , GOSSIP_SENDER_MAIN, 1045);
		player->ADD_GOSSIP_ITEM(2, "Buff me Power Word: Fortitude"           , GOSSIP_SENDER_MAIN, 1050);
		player->ADD_GOSSIP_ITEM(2, "Buff me Shadow Protection"               , GOSSIP_SENDER_MAIN, 1055);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 100);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000: //Great Buff
		player->ADD_GOSSIP_ITEM(2, "Buff me Gift of the Wild"                , GOSSIP_SENDER_MAIN, 2001);
		player->ADD_GOSSIP_ITEM(2, "Buff me Arcane Brilliance"               , GOSSIP_SENDER_MAIN, 2005);
		player->ADD_GOSSIP_ITEM(2, "Buff me Dalaran Brilliance"              , GOSSIP_SENDER_MAIN, 2010);
		player->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Kings"       , GOSSIP_SENDER_MAIN, 2015);
		player->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Might"       , GOSSIP_SENDER_MAIN, 2020);
		player->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Sanctuary"   , GOSSIP_SENDER_MAIN, 2025);
		player->ADD_GOSSIP_ITEM(2, "Buff me Greater Blessing of Wisdom"      , GOSSIP_SENDER_MAIN, 2030);
		player->ADD_GOSSIP_ITEM(2, "Buff me Prayer of Fortitude"             , GOSSIP_SENDER_MAIN, 2035);
		player->ADD_GOSSIP_ITEM(2, "Buff me Prayer of Shadow Protection"     , GOSSIP_SENDER_MAIN, 2040);
		player->ADD_GOSSIP_ITEM(2, "Buff me Prayer of Spirit"                , GOSSIP_SENDER_MAIN, 2045);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 100);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3100: //GM  Buff
		player->ADD_GOSSIP_ITEM(2, "Buff me Agamaggan's Agility"             , GOSSIP_SENDER_MAIN, 3001);
		player->ADD_GOSSIP_ITEM(2, "Buff me Agamaggan's Strength"            , GOSSIP_SENDER_MAIN, 3005);
		player->ADD_GOSSIP_ITEM(2, "Buff me Armor Magic"                     , GOSSIP_SENDER_MAIN, 3010);
		player->ADD_GOSSIP_ITEM(2, "Buff me Armor Penetration"               , GOSSIP_SENDER_MAIN, 3015);
		player->ADD_GOSSIP_ITEM(2, "Buff me Increased Stamina"               , GOSSIP_SENDER_MAIN, 3020);
		player->ADD_GOSSIP_ITEM(2, "Buff me Razorhide"                       , GOSSIP_SENDER_MAIN, 3025);
		player->ADD_GOSSIP_ITEM(2, "Buff me Rising Spirit"                   , GOSSIP_SENDER_MAIN, 3030);
		player->ADD_GOSSIP_ITEM(2, "Buff me Spirit of the Wind"              , GOSSIP_SENDER_MAIN, 3035);
		player->ADD_GOSSIP_ITEM(2, "Buff me Wisdom of Agamaggan"             , GOSSIP_SENDER_MAIN, 3040);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 100);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 4000: //Player Tools
	// Check config if "Give me Gold" is enabled or not
	if (EnableGivemeGold)
		player->ADD_GOSSIP_ITEM(8, "Give me 5000 Gold"                       , GOSSIP_SENDER_MAIN, 4001);
		player->ADD_GOSSIP_ITEM(8, "Give me Soul Shard"                     , GOSSIP_SENDER_MAIN, 4005);
		player->ADD_GOSSIP_ITEM(8, "Heal me Please"                          , GOSSIP_SENDER_MAIN, 4010);
		player->ADD_GOSSIP_ITEM(8, "Heal me and party members Please"        , GOSSIP_SENDER_MAIN, 4015);
		player->ADD_GOSSIP_ITEM(8, "Conjure Level 80 Refreshment"            , GOSSIP_SENDER_MAIN, 4020);
		player->ADD_GOSSIP_ITEM(8, "Conjure Mana Gem Level 77"               , GOSSIP_SENDER_MAIN, 4025);
		player->ADD_GOSSIP_ITEM(8, "Drunk"   						           , GOSSIP_SENDER_MAIN, 4027);
		player->ADD_GOSSIP_ITEM(8, "Sober"   						           , GOSSIP_SENDER_MAIN, 4028);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 400: //Class Trainers
		player->ADD_GOSSIP_ITEM(3, "Death Knight"               			   , GOSSIP_SENDER_MAIN, 601);
		player->ADD_GOSSIP_ITEM(3, "Druid"                        		   , GOSSIP_SENDER_MAIN, 602);
		player->ADD_GOSSIP_ITEM(3, "Hunter"                 				   , GOSSIP_SENDER_MAIN, 603);
		player->ADD_GOSSIP_ITEM(3, "Mage"                 				   , GOSSIP_SENDER_MAIN, 604);
		player->ADD_GOSSIP_ITEM(3, "Paladin"              				   , GOSSIP_SENDER_MAIN, 605);
		player->ADD_GOSSIP_ITEM(3, "Priest"    	         			       , GOSSIP_SENDER_MAIN, 606);
		player->ADD_GOSSIP_ITEM(3, "Rogue"               	 				   , GOSSIP_SENDER_MAIN, 607);
		player->ADD_GOSSIP_ITEM(3, "Shaman"            					   , GOSSIP_SENDER_MAIN, 608);
		player->ADD_GOSSIP_ITEM(3, "Warlock"         						   , GOSSIP_SENDER_MAIN, 609);
		player->ADD_GOSSIP_ITEM(3, "Warrior"          				       , GOSSIP_SENDER_MAIN, 610);
		player->ADD_GOSSIP_ITEM(3, "Hunter Pet"    					       , GOSSIP_SENDER_MAIN, 611);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 300);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 500: //Profession Trainers
		player->ADD_GOSSIP_ITEM(3, "Alchemy"      				           , GOSSIP_SENDER_MAIN, 620);
		player->ADD_GOSSIP_ITEM(3, "Blacksmithing"                           , GOSSIP_SENDER_MAIN, 621);
		player->ADD_GOSSIP_ITEM(3, "Enchanter"         			           , GOSSIP_SENDER_MAIN, 622);
		player->ADD_GOSSIP_ITEM(3, "Engineering"           				   , GOSSIP_SENDER_MAIN, 623);
		player->ADD_GOSSIP_ITEM(3, "Herbalism"             				   , GOSSIP_SENDER_MAIN, 624);
		player->ADD_GOSSIP_ITEM(3, "Inscription"             			       , GOSSIP_SENDER_MAIN, 625);
		player->ADD_GOSSIP_ITEM(3, "Jewelcrafting"            			   , GOSSIP_SENDER_MAIN, 626);
		player->ADD_GOSSIP_ITEM(3, "Leatherworking"            			   , GOSSIP_SENDER_MAIN, 627);
		player->ADD_GOSSIP_ITEM(3, "Mining"             					   , GOSSIP_SENDER_MAIN, 628);
		player->ADD_GOSSIP_ITEM(3, "Skinning"                 			   , GOSSIP_SENDER_MAIN, 629);
		player->ADD_GOSSIP_ITEM(3, "Tailoring"           					   , GOSSIP_SENDER_MAIN, 630);
		player->ADD_GOSSIP_ITEM(3, "Cooking"               				   , GOSSIP_SENDER_MAIN, 631);
		player->ADD_GOSSIP_ITEM(3, "First Aid"             				   , GOSSIP_SENDER_MAIN, 632);
		player->ADD_GOSSIP_ITEM(3, "Fishing"               				   , GOSSIP_SENDER_MAIN, 633);
		player->ADD_GOSSIP_ITEM(7, "<- Back"       		                   , GOSSIP_SENDER_MAIN, 300);
		player->ADD_GOSSIP_ITEM(7, "<- Main Menu"                            , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000: //Profession
            player->ADD_GOSSIP_ITEM(3, "Alchemy."              , GOSSIP_SENDER_MAIN, 1101);
            player->ADD_GOSSIP_ITEM(3, "Blacksmithing."        , GOSSIP_SENDER_MAIN, 1102);
            player->ADD_GOSSIP_ITEM(3, "Enchanting."           , GOSSIP_SENDER_MAIN, 1103);
            player->ADD_GOSSIP_ITEM(3, "Engineering."          , GOSSIP_SENDER_MAIN, 1104);
            player->ADD_GOSSIP_ITEM(3, "Herbalism."            , GOSSIP_SENDER_MAIN, 1105);
            player->ADD_GOSSIP_ITEM(3, "Inscription."          , GOSSIP_SENDER_MAIN, 1106);
            player->ADD_GOSSIP_ITEM(3, "Jewelcrafting."        , GOSSIP_SENDER_MAIN, 1107);
            player->ADD_GOSSIP_ITEM(3, "Leatherworking."       , GOSSIP_SENDER_MAIN, 1108);
            player->ADD_GOSSIP_ITEM(3, "Mining."               , GOSSIP_SENDER_MAIN, 1109);
            player->ADD_GOSSIP_ITEM(3, "Skinning."             , GOSSIP_SENDER_MAIN, 1110);
            player->ADD_GOSSIP_ITEM(3, "Tailoring."            , GOSSIP_SENDER_MAIN, 1111);
			player->ADD_GOSSIP_ITEM(7, "<- Back"               , GOSSIP_SENDER_MAIN, 200);
            player->ADD_GOSSIP_ITEM(7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 6000: //Secondary Skills
            player->ADD_GOSSIP_ITEM(3, "Cooking."              , GOSSIP_SENDER_MAIN, 2101);
            player->ADD_GOSSIP_ITEM(3, "First Aid."            , GOSSIP_SENDER_MAIN, 2102);
            player->ADD_GOSSIP_ITEM(3, "Fishing."              , GOSSIP_SENDER_MAIN, 2103);
          //player->ADD_GOSSIP_ITEM(3, "Riding."               , GOSSIP_SENDER_MAIN, 2104);
			player->ADD_GOSSIP_ITEM(7, "<- Back"               , GOSSIP_SENDER_MAIN, 200);
            player->ADD_GOSSIP_ITEM(7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 5005: //Back To Main Menu
	// Main Menu for Alliance
    if (player->GetTeam() == ALLIANCE)
	{
	// Buffs
	if (EnableBuffs)
		player->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		player->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		player->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
			// Check config if "Morph" is enabled or not
	if (EnableMorph)
		player->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}
	else // Main Menu for Horde
	{
	// Buffs
	if (EnableBuffs)
		player->ADD_GOSSIP_ITEM(2, " Buffs ->"  		       	  , GOSSIP_SENDER_MAIN, 100);
	// EnableTrainers
	if (EnableTrainers)
		player->ADD_GOSSIP_ITEM(3, "Trainers ->"  			  , GOSSIP_SENDER_MAIN, 300);
	// Check config if "Player Tools" is enabled or not
	if (EnablePlayerTools)
		player->ADD_GOSSIP_ITEM(8, "Player Tools ->"            , GOSSIP_SENDER_MAIN, 4000);
			// Check config if "Morph" is enabled or not
	if (EnableMorph)
		player->ADD_GOSSIP_ITEM(4, "Morph ->"       			  , GOSSIP_SENDER_MAIN, 700);

	}

	// Check config file if "Remove res sickness" option is enabled or not
	if (EnableRemoveResSickness)
		player->ADD_GOSSIP_ITEM(9, "Remove Resurrect Sickness" , GOSSIP_SENDER_MAIN, 7000);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

//////////////////////////////////////////////////Small Buff///////////////////////////////////////////////////////////////

case 1001: // Buff me Mark of the Wild
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48469,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1005: // Buff me Thorns
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,53307,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1010: // Buff me Amplify Magic
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,43017,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1015: // Buff me Arcane Intellect
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,42995,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1020: // Buff me Dalaran Intellect
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,61024,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1025: // Buff me Dampen Magic
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,43015,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1030: // Buff me Blessing of Kings
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,20217,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1035: // Buff me Blessing of Might
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48932,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1040: // Buff me Blessing of Wisdom
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48936,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1045: // Buff me Divine Spirit
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48073,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1050: // Buff me Power Word: Fortitude
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48161,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 1055: // Buff me Shadow Protection
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48169,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

//////////////////////////////////////////////////Great Buff///////////////////////////////////////////////////////////////

case 2001: // Buff me Gift of the Wild
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,48470,true);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2005: // Buff me Arcane Brilliance
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,43002,true);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2010: // Buff me Dalaran Brilliance
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,61316,true);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2015: // Buff me Greater Blessing of Kings
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,25898,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2020: // Buff me Greater Blessing of Might
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48934,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2025: // Buff me Greater Blessing of Sanctuary
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,25899,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2030: // Buff me Greater Blessing of Wisdom
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,48938,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2035: // Buff me Prayer of Fortitude
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,48162,true);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2040: // Buff me Prayer of Shadow Protection
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,48170,true);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 2045: // Buff me Prayer of Spirit
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,48074,true);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

//////////////////////////////////////////////////GM Buff///////////////////////////////////////////////////////////////

case 3001: // Buff me Agamaggan's Agility
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,17013,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3005: // Buff me Agamaggan's Strength
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,16612,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3010: // Buff me Armor Magic
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,58453,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3015: // Buff me Armor Penetration
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,34106,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3020: // Buff me Increased Stamina
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,25661,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3025: // Buff me Razorhide
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,16610,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3030: // Buff me Rising Spirit
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,10767,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3035: // Buff me Spirit of the Wind
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,16618,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 3040: // Buff me Wisdom of Agamaggan
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,7764,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

//////////////////////////////////////////////////Trainers ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////Class ///////////////////////////////////////////////////////////////
case 601: // Death Knight
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28472,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 602: // Druid
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(12042,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 603: // Hunter
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(303911,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 604: // Mage
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28958,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 605: // Paladin
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(352811,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 606: // Priest
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(166581,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 607: // Rogue
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(6707,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 608: // Shaman
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(172041,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 609: // Warlock
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(23534,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 610: // Warrior
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(45931,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

//////////////////////////////////////////////////Profession and Extras ///////////////////////////////////////////////////////////////

case 620: // Alchemy
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(33630,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
	player->SummonCreature(28703,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 621: // Blacksmithing
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28694,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 622: // Enchanting
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28693,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
	player->SummonCreature(33633,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 623: // Engineering
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28697,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 624: // Herbalism
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28704,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 625: // Inscription
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28702,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 626: // Jewelcrafting
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28701,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 627: // Leatherworking
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28700,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 628: // Mining
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28698,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 629: // Skinning
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28696,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 630: // Tailoring
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28699,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 631: // Cooking
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28705,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 632: // First Aid
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28706,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);;
break;

case 633: // Fishing
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28742,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 634: // Mount
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(31238,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 613: // Weapon Master
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(12704,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 611: // Hunters Pet
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(100901,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

case 612: // Stable Master
    player->CLOSE_GOSSIP_MENU();
	player->SummonCreature(28690,player->GetPositionX() ,player->GetPositionY(), player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,120000);
break;

//////////////////////////////////////////////////Morph ///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////Famous ///////////////////////////////////////////////////////////////

case 711: // Algalon
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(28641);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
break;

case 712: // Doomwalker
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(16630);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
break;

case 713: // Doomlord kazzak
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(17887);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.10f);
break;

case 714: // Onyxia
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(8570);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
break;

case 715: // Thaddius
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(16137);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
break;

case 716: // Gluth
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(16064);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
break;

case 717: // Maexxna
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15928);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.15f);
break;

case 718: // Sapphiron
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(16033);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
break;

case 719: // Noth The Plaguebringer
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(16590);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
break;

case 720: // Kel'Thuzad
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15945);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
break;

case 721: // Kael'thas Sunstrider
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(20023);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
break;

case 722: // C'thun
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15786);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
break;

case 723: // Thrall
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(4527);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
break;

//////////////////////////////////////////////////Funny ///////////////////////////////////////////////////////////////

case 751: // Gerky
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(29348);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
break;

case 752: // Water elemental
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(12129);
		player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.3f);
break;

case 753: // Pandaren Monk
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(30414);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
break;

case 754: // Lil' K.T
	player->CLOSE_GOSSIP_MENU();
	player->DeMorph();
    player->SetDisplayId(30507);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
break;

case 755: // The Weapon you're Wearing
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15880);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
break;

case 756: // Red Bubbles
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(20262);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
break;

case 757: // A Shark
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15555);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
break;

case 758: // A Fel cannon
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(18505);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
break;

case 759: // Human Female
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(100);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
break;

case 760: // A Fat Human
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(103);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
break;

case 761: // A Rabbit
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(328);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 3.0f);
break;

case 762: // A Cool looking Bug
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15695);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
break;

case 763: // Eye Of C'thun
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15556);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
break;

case 771: // Make me Bigger
    player->CLOSE_GOSSIP_MENU();
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
break;

case 772: // Make me Smaller
    player->CLOSE_GOSSIP_MENU();
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
break;

case 799: // DeMorph
    player->CLOSE_GOSSIP_MENU();
	player->DeMorph();
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
break;

//////////////////////////////////////////////////Player Tools///////////////////////////////////////////////////////////////

case 4001://Give me Gold
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,46642,false); // 5000 gold
break;

case 4005://Give me Soul Shard
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,24827,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 4010: // Heal me please
    player->CLOSE_GOSSIP_MENU();
    pCreature->CastSpell(player,38588,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 4015: // Heal me and party members Please
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,53251,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 4020: // Conjure Refreshment
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,42956,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
break;

case 4025: // Conjure Mana Gem
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,42985,false);
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));

case 4026: // Update Skill to Max for Level
    player->CLOSE_GOSSIP_MENU();
    player->UpdateSkillsToMaxSkillsForLevel();
    player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneBuffGoldCost",0)));
    pCreature->MonsterSay("Your Skills have now been Maxed for your Level.", LANG_UNIVERSAL, NULL);
    
break;

case 4027: // Drunk
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,46876,false);
    pCreature->MonsterSay("Hahahaha! You are Drunk Now!", LANG_UNIVERSAL, NULL);
    
break;

case 4028: // Sober
    player->CLOSE_GOSSIP_MENU();
    player->SetDrunkValue(0, 9);
    pCreature->MonsterSay("Hahahaha! I knew you could not handle you drink!", LANG_UNIVERSAL, NULL);
    
break;

case 7000://Remove Res Sickness
	if (!player->HasAura(15007,0))
	{
		pCreature->MonsterWhisper("You don't have resurrection sickness.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	pCreature->CastSpell(player,38588,false); // Healing effect
	player->RemoveAurasDueToSpell(15007,0);
	player->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Professions///////////////////////////////////////////////////////////////

case 1101: // Alchemy
	if (!player->UpdateSkill(171,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(171,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1102: // Blacksmithing
	if (!player->UpdateSkill(164,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(164,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1103: // Enchanting
	if (!player->UpdateSkill(333,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(333,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1104: // Engineering
	if (!player->UpdateSkill(202,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(202,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1105: // Herbalism
	if (!player->UpdateSkill(182,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(182,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1106: // Inscription
	if (!player->UpdateSkill(773,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(773,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1107: // Jewelcrafting
	if (!player->UpdateSkill(755,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(755,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1108: // Leatherworking
	if (!player->UpdateSkill(165,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(165,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1109: // Mining
	if (!player->UpdateSkill(186,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(186,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1110: // Skinning
	if (!player->UpdateSkill(393,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(393,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 1111: // Tailoring
	if (!player->UpdateSkill(197,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(197,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Secondary Skills///////////////////////////////////////////////////////////////

case 2101: // Cooking
	if (!player->UpdateSkill(185,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(185,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 2102: // First Aid
	if (!player->UpdateSkill(129,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(129,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

case 2103: // Fishing
	if (!player->UpdateSkill(356,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
		OnGossipHello(player, pCreature);
		return;
	}

	player->UpdateSkill(356,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
	player->CLOSE_GOSSIP_MENU();
break;

//case 2104: // Riding
//	if (!player->UpdateSkill(,0))
//    {
//		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", player->GetGUID());
//		OnGossipHello_ProfessionNPC(player, pCreature);
//		return;
//	}
//
//	player->UpdateSkill(,(ConfigMgr::GetFloatDefault("pryds_allinoneSkillPoints",0)));
//	player->ModifyMoney(-(ConfigMgr::GetFloatDefault("pryds_allinoneSkillGoldCost",0)));
//	player->CLOSE_GOSSIP_MENU();
//break;
 player->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function



bool OnGossipSelect(Player  *player, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_pryds_allinone(player, pCreature, uiAction);

return true;
}

};

void AddSC_pryds_allinone()
{
    new pryds_allinone();
}