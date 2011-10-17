#include "ScriptPCH.h"
#include "Config.h"

class npc_vip : public CreatureScript
{
public:
    npc_vip() : CreatureScript("npc_vip") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (!player->GetSession()->IsPremium() && !player->isGameMaster())
        {
            player->CLOSE_GOSSIP_MENU();
            creature->MonsterWhisper("Es tut mir leid $N, Du bist kein MMO Elite Mitglied.", player->GetGUID(), false);
            return true;
        }

        if (creature->isVendor())
        {
            player->CLOSE_GOSSIP_MENU();
            player->GetSession()->SendListInventory(creature->GetGUID());
            return true;
        }

        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

		player->ADD_GOSSIP_ITEM(7, "[Morphs - Famous] ->"               , GOSSIP_SENDER_MAIN, 1204);	
		player->ADD_GOSSIP_ITEM(7, "[Morphs - Funny] ->"               , GOSSIP_SENDER_MAIN, 1205);		
        player->ADD_GOSSIP_ITEM(7, "[Buffs] ->"                , GOSSIP_SENDER_MAIN, 1206);
        player->ADD_GOSSIP_ITEM(5, "DeMorph"                , GOSSIP_SENDER_MAIN, 1207);		
		
        player->ADD_GOSSIP_ITEM(9, "Heilen"                     , GOSSIP_SENDER_MAIN, 1202);
        //player->ADD_GOSSIP_ITEM(5, "Talente resetten"          , GOSSIP_SENDER_MAIN, 4035);
		player->ADD_GOSSIP_ITEM(8, "Manastrudel"            , GOSSIP_SENDER_MAIN, 4020);		
		player->ADD_GOSSIP_ITEM(8, "Seelensplitter"                     , GOSSIP_SENDER_MAIN, 4024);		
		player->ADD_GOSSIP_ITEM(8, "Level 77 Mana Stein"               , GOSSIP_SENDER_MAIN, 4025);
		player->ADD_GOSSIP_ITEM(3, "Max Waffen Skill"    				       , GOSSIP_SENDER_MAIN, 4026);		
		player->ADD_GOSSIP_ITEM(5, "Betrinken"   						           , GOSSIP_SENDER_MAIN, 4027);
		player->ADD_GOSSIP_ITEM(5, "Ausnüchtern"   						           , GOSSIP_SENDER_MAIN, 4028);
		player->ADD_GOSSIP_ITEM(9, "Todesnachwirkungen entfernen" , GOSSIP_SENDER_MAIN, 7000);		
		
        //player->ADD_GOSSIP_ITEM(7, "[Morphs] ->"               , GOSSIP_SENDER_MAIN, 1203);


        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        switch(action)
        {
            case 1202: // Heal
                if (player->HasAura(45523))
                {
                    player->CLOSE_GOSSIP_MENU();
                    creature->MonsterWhisper("Es tut mir leid $N, diese Aktion steht zur Zeit nicht zur Verfügung.", player->GetGUID(), false);
                }
                else
                {
                    player->CLOSE_GOSSIP_MENU();
                    player->CastSpell(player, 39321, true);
                    player->CastSpell(player, 45523, true);
                }
                break;
            case 4035: // Reset talents
                player->CLOSE_GOSSIP_MENU();
                player->SendTalentWipeConfirm(creature->GetGUID());
                break;
			case 4020: // Manastudel
				player->CLOSE_GOSSIP_MENU();
				player->CastSpell(player,42956,false);
			break;			
			case 4024://Seelenstein
				player->CLOSE_GOSSIP_MENU();
				player->CastSpell(player,24827,false);
			break;			
			case 4025: // Manastein
				player->CLOSE_GOSSIP_MENU();
				player->CastSpell(player,42985,false);
			break;		
			case 4026: // Update Skill to Max for Level
				player->CLOSE_GOSSIP_MENU();
				player->UpdateSkillsToMaxSkillsForLevel();
				creature->MonsterSay("Deine Waffenskills wurden maximiert.", LANG_UNIVERSAL, NULL);
			break;			
			case 4027: // Betrinken
				player->CLOSE_GOSSIP_MENU();
				player->CastSpell(player,46876,false);
				creature->MonsterSay("Prost!", LANG_UNIVERSAL, NULL);
			break;
			case 4028: // Ausnüchtern
				player->CLOSE_GOSSIP_MENU();
				player->SetDrunkValue(0, 9);
				creature->MonsterSay("Hahahaha! Du scheinst deinen Drink nicht vertragen zu habem!", LANG_UNIVERSAL, NULL);
			break;
			case 7000://Remove Res Sickness
				if (!player->HasAura(15007,0))
				{
					creature->MonsterWhisper("Du hast keine Todesnachwirkungen.", player->GetGUID());
					OnGossipHello(player, creature);
				}
				creature->CastSpell(player,38588,false); // Healing effect
				player->RemoveAurasDueToSpell(15007,0);
				player->CLOSE_GOSSIP_MENU();
			break;			
            case 1203: // Morphs
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;
            case 1204: // Morphs Famous
                player->ADD_GOSSIP_ITEM(5, "Algalon", GOSSIP_SENDER_MAIN, 711);
                player->ADD_GOSSIP_ITEM(5, "Doomwalker", GOSSIP_SENDER_MAIN, 712);
				player->ADD_GOSSIP_ITEM(5, "Doomlord kazzak", GOSSIP_SENDER_MAIN, 713);
                player->ADD_GOSSIP_ITEM(5, "Onyxia", GOSSIP_SENDER_MAIN, 714);
                player->ADD_GOSSIP_ITEM(5, "Thaddius", GOSSIP_SENDER_MAIN, 715);
                player->ADD_GOSSIP_ITEM(5, "Gluth", GOSSIP_SENDER_MAIN, 716);
                player->ADD_GOSSIP_ITEM(5, "Maexxna", GOSSIP_SENDER_MAIN, 717);
                player->ADD_GOSSIP_ITEM(5, "Sapphiron", GOSSIP_SENDER_MAIN, 718);
                player->ADD_GOSSIP_ITEM(5, "Noth The Plaguebringer", GOSSIP_SENDER_MAIN, 719);
                player->ADD_GOSSIP_ITEM(5, "Kel'Thuzad", GOSSIP_SENDER_MAIN, 720);
                player->ADD_GOSSIP_ITEM(5, "Kael'thas Sunstrider", GOSSIP_SENDER_MAIN, 721);
                player->ADD_GOSSIP_ITEM(5, "C'thun", GOSSIP_SENDER_MAIN, 722);
                player->ADD_GOSSIP_ITEM(5, "Thrall", GOSSIP_SENDER_MAIN, 723);
                player->ADD_GOSSIP_ITEM(5, "Sylvanas Windrunner", GOSSIP_SENDER_MAIN, 724);			
                player->ADD_GOSSIP_ITEM(5, "Alexstrasza (Blutelf)", GOSSIP_SENDER_MAIN, 725);		
                player->ADD_GOSSIP_ITEM(5, "Varian Wrynn", GOSSIP_SENDER_MAIN, 726);		
                player->ADD_GOSSIP_ITEM(5, "Varimathras", GOSSIP_SENDER_MAIN, 727);		
                player->ADD_GOSSIP_ITEM(5, "Alexstrasza (Drache)", GOSSIP_SENDER_MAIN, 728);		
                player->ADD_GOSSIP_ITEM(5, "Mal'ganis", GOSSIP_SENDER_MAIN, 729);		
                player->ADD_GOSSIP_ITEM(5, "Illidan", GOSSIP_SENDER_MAIN, 500);
                player->ADD_GOSSIP_ITEM(5, "Akama", GOSSIP_SENDER_MAIN, 502);
                player->ADD_GOSSIP_ITEM(5, "Todesritter", GOSSIP_SENDER_MAIN, 503);
                player->ADD_GOSSIP_ITEM(5, "Taure", GOSSIP_SENDER_MAIN, 504);
                player->ADD_GOSSIP_ITEM(5, "Untoter", GOSSIP_SENDER_MAIN, 505);
                player->ADD_GOSSIP_ITEM(5, "Mensch", GOSSIP_SENDER_MAIN, 506);
                player->ADD_GOSSIP_ITEM(5, "Lich König", GOSSIP_SENDER_MAIN, 507);
                player->ADD_GOSSIP_ITEM(5, "Tirion", GOSSIP_SENDER_MAIN, 508);
                player->ADD_GOSSIP_ITEM(5, "Uther Lichtbringer", GOSSIP_SENDER_MAIN, 509);
                player->ADD_GOSSIP_ITEM(5, "Arthas", GOSSIP_SENDER_MAIN, 510);
                player->ADD_GOSSIP_ITEM(5, "DEMORPH", GOSSIP_SENDER_MAIN, 511);				
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;	
            case 1205: // Morphs Funny
                player->ADD_GOSSIP_ITEM(5, "Gerky", GOSSIP_SENDER_MAIN, 751);
                player->ADD_GOSSIP_ITEM(5, "Feuer Elementar", GOSSIP_SENDER_MAIN, 752);
                player->ADD_GOSSIP_ITEM(5, "Pandaren Mönch", GOSSIP_SENDER_MAIN, 753);
                player->ADD_GOSSIP_ITEM(5, "Kleiner Kel'Thuzad", GOSSIP_SENDER_MAIN, 754);
                //player->ADD_GOSSIP_ITEM(5, "Deine aktuelle Waffe", GOSSIP_SENDER_MAIN, 755);
                //player->ADD_GOSSIP_ITEM(5, "Rote Blase", GOSSIP_SENDER_MAIN, 756);
                player->ADD_GOSSIP_ITEM(5, "Hai", GOSSIP_SENDER_MAIN, 757);
                //player->ADD_GOSSIP_ITEM(5, "Kanone", GOSSIP_SENDER_MAIN, 758);
                player->ADD_GOSSIP_ITEM(5, "Frau (Mensch)", GOSSIP_SENDER_MAIN, 759);
                player->ADD_GOSSIP_ITEM(5, "Fetter Mann", GOSSIP_SENDER_MAIN, 760);
                player->ADD_GOSSIP_ITEM(5, "Hase", GOSSIP_SENDER_MAIN, 761);
                player->ADD_GOSSIP_ITEM(5, "Käfer", GOSSIP_SENDER_MAIN, 762);	
                player->ADD_GOSSIP_ITEM(5, "Auge von C'thun", GOSSIP_SENDER_MAIN, 763);				
                player->ADD_GOSSIP_ITEM(5, "Tanzender Murloc", GOSSIP_SENDER_MAIN, 764);						
                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
                break;	
				
				
            case 500:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(21135);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);				
                break;
            case 502:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(20681);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 503:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(16508);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 504:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(17332);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 505:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(2789);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 506:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(16280);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.8f);						
                break;
            case 507:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(24191);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 508:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(22209);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 509:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(23889);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 510:
                player->CLOSE_GOSSIP_MENU();
                player->SetDisplayId(24949);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);						
                break;
            case 511:
                player->CLOSE_GOSSIP_MENU();
                player->DeMorph();
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);							
                break;
            case 1206: // Buffs
                player->ADD_GOSSIP_ITEM(5, "Machtwort: Seelenstärke, Rang 8", GOSSIP_SENDER_MAIN, 4000);
                player->ADD_GOSSIP_ITEM(5, "Großer Segen der Könige", GOSSIP_SENDER_MAIN, 4001);
                player->ADD_GOSSIP_ITEM(5, "Großer Segen der Macht", GOSSIP_SENDER_MAIN,4002);
                player->ADD_GOSSIP_ITEM(5, "Großer Segen der Weisheit", GOSSIP_SENDER_MAIN, 4003);
                player->ADD_GOSSIP_ITEM(5, "Mal der Wildnis, Rang 9", GOSSIP_SENDER_MAIN, 4004);
                player->ADD_GOSSIP_ITEM(5, "Arkane Intelligenz, Rang 7", GOSSIP_SENDER_MAIN, 4005);
                player->ADD_GOSSIP_ITEM(5, "Dornen, Rang 8", GOSSIP_SENDER_MAIN, 4007);
                player->ADD_GOSSIP_ITEM(5, "Göttlicher Wille, Rang 8", GOSSIP_SENDER_MAIN, 4008);
                player->ADD_GOSSIP_ITEM(5, "Schattenschutz, Rang 5", GOSSIP_SENDER_MAIN, 4009);

                player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());

                break;
            case 1207: // DeMorph
                player->CLOSE_GOSSIP_MENU();
                player->DeMorph();
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);				
                break;				
            case 4000:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48161, true);
                break;
            case 4001:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 25898, true);
                break;
            case 4002:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48934, true);
                break;
            case 4003:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48938, true);
                break;
            case 4004:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48469, true);
                break;
            case 4005:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 42995, true);
                break;
            case 4007:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 53307, true);
                break;
            case 4008:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48073, true);
                break;
            case 4009:
                player->CLOSE_GOSSIP_MENU();
                player->CastSpell(player, 48169, true);
                break;
				
				


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

			case 724: // Sylvanas Windrunner
				player->CLOSE_GOSSIP_MENU();
				player->SetDisplayId(28213);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
			break;

			case 725: // Alexstrasza, BElf
				player->CLOSE_GOSSIP_MENU();
				player->SetDisplayId(28227);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.8f);
			break;

			case 726: // Varian Wrynn
				player->CLOSE_GOSSIP_MENU();
				player->SetDisplayId(28127);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
			break;

			case 727: // Varimathras
				player->CLOSE_GOSSIP_MENU();
				player->SetDisplayId(28220);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.7f);
			break;

			case 728: // Alexstrasza, dragon
				player->CLOSE_GOSSIP_MENU();
				player->SetDisplayId(27569);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
			break;

			case 729: // Mal'ganis
				player->CLOSE_GOSSIP_MENU();
				player->SetDisplayId(26582);
				player->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5f);
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
				
case 764: // Murloc
    player->CLOSE_GOSSIP_MENU();
	player->SetDisplayId(15984);
	player->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
break;				
				
				
        }

        return true;
    }
};

void AddSC_npc_vip()
{
    new npc_vip;
}
