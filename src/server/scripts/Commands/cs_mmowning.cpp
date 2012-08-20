/*
 * Copyright (C) 2012 MMOwning
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
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "Language.h"
#include "Log.h"
#include "SpellAuras.h"
#include "World.h"

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"
#include "SocialMgr.h"
#include "../TriniChat/IRCClient.h"
#include "../AuctionHouse/AuctionHouseBot/AuctionHouseBot.h"

class mmowning_commandscript : public CommandScript
{
public:
    mmowning_commandscript() : CommandScript("mmowning_commandscript") { }

    ChatCommand* GetCommands() const
    {

        static ChatCommand commandTable[] =
        {
			{ "ahbotoptions",   SEC_GAMEMASTER,     true,  &HandleAHBotOptionsCommand,        "", NULL },
			//{ "goname",         SEC_MODERATOR,      false, OldHandler<&ChatHandler::HandleAppearCommand>,              "", NULL },				
			//{ "namego",         SEC_MODERATOR,      false, OldHandler<&ChatHandler::HandleSummonCommand>,              "", NULL },				
			//Allows your players to gamble for fun and prizes
			{ "gamble",         SEC_PLAYER,         false, &HandleGambleCommand,              "", NULL },	
			{ "roulette",       SEC_PLAYER,         false, &HandleRouletteCommand,            "", NULL },
			//Mall Teleporter
			{ "mall",			SEC_PLAYER,			false, &HandleMallCommand,				"", NULL }, 		
			//Dalaran Teleporter
			{ "dala",			SEC_PLAYER,			false, &HandleDalaCommand,				"", NULL }, 				
			//Buffer
			{ "buffs",			SEC_PLAYER,			false, &HandleBuffsCommand,				"", NULL }, 
			//GuildHouse Tele
			{ "gh",             SEC_PLAYER,      	false, &HandleGHCommand,				"", NULL },		
			//{ "tcrecon",        SEC_MODERATOR,      false, &HandleIRCRelogCommand,            "", NULL },	
        };

        return commandTable;
    }


//Allows your players to gamble for fun and prizes
static bool HandleGambleCommand(ChatHandler* handler, const char* args)
{
    Player *chr = handler->GetSession()->GetPlayer();

    char* px = strtok((char*)args, " ");

    if (!px)
        return false;

    uint32 money = (uint32)atoi(px);

    if (chr->GetMoney() < money)
    {
        handler->PSendSysMessage("Du kannst kein Gold setzen welches du nicht hast!");
        return true;
    }

    else
    {
        if (money>0)
        {
             //if (rand()%100 < 50)
			 if (rand()%100 < 40)
             {
				  chr->ModifyMoney(money);	
                  //chr->ModifyMoney(money*2);
                  handler->PSendSysMessage("Du hast gewonnen und einen Einsatz verdoppelt");
             }
             else
             {
                  chr->ModifyMoney(-int(money));
                  handler->PSendSysMessage("Du hast verloren");
             }  
        }
    }

    return true;
}

static bool HandleRouletteCommand(ChatHandler* handler, const char* args)
{
    Player *chr = handler->GetSession()->GetPlayer();

    char* px = strtok((char*)args, " ");

    if (!px)
        return false;

    uint32 money = (uint32)atoi(px);

    if (chr->GetMoney() < money)
    {
        handler->PSendSysMessage("Du kannst kein Gold setzen welches du nicht hast!");
        return true;
    }

    else
    {
        if (money>0)
        {
             //if (rand()%36 < 1)
			 if (rand()%42 < 1)
             {
                  chr->ModifyMoney(money*36);
                  handler->PSendSysMessage("Du hast das 36x deines Einsatzes gewonnen, GZ!");
             }
             else
             {
                  chr->ModifyMoney(-int(money));
                  handler->PSendSysMessage("Du hast verloren");
             }  
        }
    }

     return true;
 }

//Mall Teleporter
static bool HandleMallCommand(ChatHandler* handler, const char* /*args*/)
{
        //MALL command
        
        Player *chr = handler->GetSession()->GetPlayer();

        if (chr->isInCombat())
        {
        handler->PSendSysMessage(LANG_YOU_IN_COMBAT);
        //SetSentErrorMessage(true);
        return false;
        }
        if (chr->isInFlight())
        {
        handler->PSendSysMessage(LANG_YOU_IN_FLIGHT);
        //SetSentErrorMessage(true);
        return false;
        }

        chr->ResurrectPlayer(0.5, false);

        switch(chr->GetTeam())
   {
        case ALLIANCE:
                chr->TeleportTo(0, -8865.454102f, 591.230469f, 92.323326f, 4.69254f);    // Insert Ally mall Cords here
                break;

        case HORDE:
                        chr->TeleportTo(1, 1554.14f, -4424.46f, 9.86212f, 5.96413f);    // Insert Horde mall Cords here
                break;
   }
        return true;
}

//Dalaran Teleporter
static bool HandleDalaCommand(ChatHandler* handler, const char* /*args*/)
{
        //MALL command
        
        Player *chr = handler->GetSession()->GetPlayer();

        if (chr->isInCombat())
        {
        handler->PSendSysMessage(LANG_YOU_IN_COMBAT);
        //SetSentErrorMessage(true);
        return false;
        }
        if (chr->isInFlight())
        {
        handler->PSendSysMessage(LANG_YOU_IN_FLIGHT);
        //SetSentErrorMessage(true);
        return false;
        }

        chr->ResurrectPlayer(0.5, false);

        chr->TeleportTo(571, 5813.93f, 449.06f, 658.75f, 1.276612f);    // Insert Dala Coords

        return true;
}

//Buffer
static bool HandleBuffsCommand(ChatHandler* handler, const char* /*args*/)              
{
                                Player *chr = handler->GetSession()->GetPlayer();
                                
        if (chr->isInCombat())
        {
        handler->PSendSysMessage("Du kannst dich nicht waehrend eines Kampfes buffen");
        //SetSentErrorMessage(true);
        return false;
        }
        if (chr->isInFlight())
        {
        handler->PSendSysMessage("Du kannste dich nicht waehrend des Fliegens buffen");
        //SetSentErrorMessage(true);
        return false;
        }
                
        if (chr->GetMoney() >= 2000000)
                {
								chr->Dismount();
                                chr->RemoveAurasByType(SPELL_AURA_MOUNTED);
                                chr-> AddAura(48161, chr);              // Power Word: Fortitude        
                                chr-> AddAura(48073, chr);              // Divine Spirit
                                chr-> AddAura(20217, chr);              // Blessing of Kings
                                chr-> AddAura(48469, chr);              // Mark of the wild
                                chr-> AddAura(16609, chr);              // Spirit of Zandalar
                                chr-> AddAura(15366, chr);              // Songflower Serenade
                                chr-> AddAura(22888, chr);              // Rallying Cry of the Dragonslayer
                                chr-> AddAura(57399, chr);              // Well Fed
                                chr-> AddAura(17013, chr);              // Agamaggan's Agility
                                chr-> AddAura(16612, chr);              // Agamaggan's Strength
                                chr->ModifyMoney(-2000000);
                                handler->PSendSysMessage("Du bist jetzt gebufft!");
                                return false;
                }
        else
                {
                handler->PSendSysMessage("Du hast nicht genug Gold!");
        }
                return false;
}	

//GuildHouse Tele
static bool HandleGHCommand(ChatHandler* handler, const char* args)
{
        Player *chr = handler->GetSession()->GetPlayer();

        if(chr->isInFlight())
        {
                //pokud hrac leti
                handler->PSendSysMessage(LANG_YOU_IN_FLIGHT);
                //SetSentErrorMessage(true);
                return false;
        }

        if(chr->isInCombat())
        {
                //pokud je hrac v combatu
                handler->PSendSysMessage(LANG_YOU_IN_COMBAT);
                //SetSentErrorMessage(true);
                return false;
        }

        if (chr->GetGuildId() == 0)
        {
                //pokud hrac nema guildu
                return false;
        }

        QueryResult result;
        result = CharacterDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", chr->GetGuildId());
        if(!result)
        {
                //pokud guilda nema guildhouse zapsany v tabulce guildhouses
                handler->PSendSysMessage("GH Port");
                return false;
        }


        float x, y, z;
        uint32 map;

        Field *fields = result->Fetch();
        x = fields[0].GetFloat();
        y = fields[1].GetFloat();
        z = fields[2].GetFloat();
        map = fields[3].GetUInt32();
        

        chr->SaveRecallPosition();
        chr->TeleportTo(map, x, y, z, 0);
        chr->SaveToDB();
        return true;
}

static bool HandleIRCpmCommand(ChatHandler* handler, const char* args)
{
   std::string Msg = args;
   if (Msg.find(" ") == std::string::npos)
       return false;
   std::string To = Msg.substr(0, Msg.find(" "));
   Msg = Msg.substr(Msg.find(" ") + 1);
   std::size_t pos;
   while((pos = To.find("||")) != std::string::npos)
   {
       std::size_t find1 = To.find("||", pos);
       To.replace(pos, find1 - pos + 2, "|");
   }
   sIRC.SendIRC("PRIVMSG "+To+" : <WoW>["+handler->GetSession()->GetPlayerName()+"] : " + Msg);
   sIRC.Send_WoW_Player(handler->GetSession()->GetPlayer(), "|cffCC4ACCTo ["+To+"]: "+Msg);
   return true;
}

static bool HandleAHBotOptionsCommand(ChatHandler* handler, const char *args)
{
    uint32 ahMapID = 0;
    char * opt = strtok((char*)args, " ");
    char * ahMapIdStr = strtok(NULL, " ");
    if (ahMapIdStr)
    {
        ahMapID = (uint32) strtoul(ahMapIdStr, NULL, 0);
        switch (ahMapID)
        {
        case 2:
        case 6:
        case 7:
            break;
        default:
            opt = NULL;
            break;
        }
    }
    if (!opt)
    {
        handler->PSendSysMessage("Syntax is: ahbotoptions $option $ahMapID (2, 6 or 7) $parameter");
        handler->PSendSysMessage("Try ahbotoptions help to see a list of options.");
        return false;
    }
    int l = strlen(opt);

    if (strncmp(opt,"help",l) == 0)
    {
        handler->PSendSysMessage("AHBot commands:");
        handler->PSendSysMessage("ahexpire");
        handler->PSendSysMessage("minitems");
        handler->PSendSysMessage("maxitems");
        //handler->PSendSysMessage("");
        //handler->PSendSysMessage("");
        handler->PSendSysMessage("percentages");
        handler->PSendSysMessage("minprice");
        handler->PSendSysMessage("maxprice");
        handler->PSendSysMessage("minbidprice");
        handler->PSendSysMessage("maxbidprice");
        handler->PSendSysMessage("maxstack");
        handler->PSendSysMessage("buyerprice");
        handler->PSendSysMessage("bidinterval");
        handler->PSendSysMessage("bidsperinterval");
        return true;
    }
    else if (strncmp(opt,"ahexpire",l) == 0)
    {
        if (!ahMapIdStr)
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions ahexpire $ahMapID (2, 6 or 7)");
            return false;
        }
        auctionbot.Commands(0, ahMapID, NULL, NULL);
    }
    else if (strncmp(opt,"minitems",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions minitems $ahMapID (2, 6 or 7) $minItems");
            return false;
        }
        auctionbot.Commands(1, ahMapID, NULL, param1);
    }
    else if (strncmp(opt,"maxitems",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxitems $ahMapID (2, 6 or 7) $maxItems");
            return false;
        }
        auctionbot.Commands(2, ahMapID, NULL, param1);
    }
    else if (strncmp(opt,"mintime",l) == 0)
    {
        handler->PSendSysMessage("ahbotoptions mintime has been deprecated");
        return false;
        /*
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions mintime $ahMapID (2, 6 or 7) $mintime");
            return false;
        }
        auctionbot.Commands(3, ahMapID, NULL, param1);
        */
    }
    else if (strncmp(opt,"maxtime",l) == 0)
    {
        handler->PSendSysMessage("ahbotoptions maxtime has been deprecated");
        return false;
        /*
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxtime $ahMapID (2, 6 or 7) $maxtime");
            return false;
        }
        auctionbot.Commands(4, ahMapID, NULL, param1);
        */
    }
    else if (strncmp(opt,"percentages",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        char * param3 = strtok(NULL, " ");
        char * param4 = strtok(NULL, " ");
        char * param5 = strtok(NULL, " ");
        char * param6 = strtok(NULL, " ");
        char * param7 = strtok(NULL, " ");
        char * param8 = strtok(NULL, " ");
        char * param9 = strtok(NULL, " ");
        char * param10 = strtok(NULL, " ");
        char * param11 = strtok(NULL, " ");
        char * param12 = strtok(NULL, " ");
        char * param13 = strtok(NULL, " ");
        char * param14 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param14))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions percentages $ahMapID (2, 6 or 7) $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14");
            handler->PSendSysMessage("1 GreyTradeGoods 2 WhiteTradeGoods 3 GreenTradeGoods 4 BlueTradeGoods 5 PurpleTradeGoods");
            handler->PSendSysMessage("6 OrangeTradeGoods 7 YellowTradeGoods 8 GreyItems 9 WhiteItems 10 GreenItems 11 BlueItems");
            handler->PSendSysMessage("12 PurpleItems 13 OrangeItems 14 YellowItems");
            handler->PSendSysMessage("The total must add up to 100%");
            return false;
        }
        uint32 greytg = (uint32) strtoul(param1, NULL, 0);
        uint32 whitetg = (uint32) strtoul(param2, NULL, 0);
        uint32 greentg = (uint32) strtoul(param3, NULL, 0);
        uint32 bluetg = (uint32) strtoul(param3, NULL, 0);
        uint32 purpletg = (uint32) strtoul(param5, NULL, 0);
        uint32 orangetg = (uint32) strtoul(param6, NULL, 0);
        uint32 yellowtg = (uint32) strtoul(param7, NULL, 0);
        uint32 greyi = (uint32) strtoul(param8, NULL, 0);
        uint32 whitei = (uint32) strtoul(param9, NULL, 0);
        uint32 greeni = (uint32) strtoul(param10, NULL, 0);
        uint32 bluei = (uint32) strtoul(param11, NULL, 0);
        uint32 purplei = (uint32) strtoul(param12, NULL, 0);
        uint32 orangei = (uint32) strtoul(param13, NULL, 0);
        uint32 yellowi = (uint32) strtoul(param14, NULL, 0);
        uint32 totalPercent = greytg + whitetg + greentg + bluetg + purpletg + orangetg + yellowtg + greyi + whitei + greeni + bluei + purplei + orangei + yellowi;
        if ((totalPercent == 0) || (totalPercent != 100))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions percentages $ahMapID (2, 6 or 7) $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14");
            handler->PSendSysMessage("1 GreyTradeGoods 2 WhiteTradeGoods 3 GreenTradeGoods 4 BlueTradeGoods 5 PurpleTradeGoods");
            handler->PSendSysMessage("6 OrangeTradeGoods 7 YellowTradeGoods 8 GreyItems 9 WhiteItems 10 GreenItems 11 BlueItems");
            handler->PSendSysMessage("12 PurpleItems 13 OrangeItems 14 YellowItems");
            handler->PSendSysMessage("The total must add up to 100%");
            return false;
        }
        char param[100];
        param[0] = '\0';
        strcat(param, param1);
        strcat(param, " ");
        strcat(param, param2);
        strcat(param, " ");
        strcat(param, param3);
        strcat(param, " ");
        strcat(param, param4);
        strcat(param, " ");
        strcat(param, param5);
        strcat(param, " ");
        strcat(param, param6);
        strcat(param, " ");
        strcat(param, param7);
        strcat(param, " ");
        strcat(param, param8);
        strcat(param, " ");
        strcat(param, param9);
        strcat(param, " ");
        strcat(param, param10);
        strcat(param, " ");
        strcat(param, param11);
        strcat(param, " ");
        strcat(param, param12);
        strcat(param, " ");
        strcat(param, param13);
        strcat(param, " ");
        strcat(param, param14);
        auctionbot.Commands(5, ahMapID, NULL, param);
    }
    else if (strncmp(opt,"minprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions minprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions minprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"maxprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"minbidprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions minbidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        uint32 minBidPrice = (uint32) strtoul(param2, NULL, 0);
        if ((minBidPrice < 1) || (minBidPrice > 100))
        {
            handler->PSendSysMessage("The min bid price multiplier must be between 1 and 100");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions minbidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"maxbidprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxbidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        uint32 maxBidPrice = (uint32) strtoul(param2, NULL, 0);
        if ((maxBidPrice < 1) || (maxBidPrice > 100))
        {
            handler->PSendSysMessage("The max bid price multiplier must be between 1 and 100");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions max bidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"maxstack",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxstack $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $value");
            return false;
        }
        uint32 maxStack = (uint32) strtoul(param2, NULL, 0);
        if (maxStack < 0)
        {
            handler->PSendSysMessage("maxstack can't be a negative number.");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions maxstack $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $value");
            return false;
        }
    }
    else if (strncmp(opt,"buyerprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions buyerprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue or purple) $price");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions buyerprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue or purple) $price");
            return false;
        }
    }
    else if (strncmp(opt,"bidinterval",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions bidinterval $ahMapID (2, 6 or 7) $interval(in minutes)");
            return false;
        }
        auctionbot.Commands(12, ahMapID, NULL, param1);
    }
    else if (strncmp(opt,"bidsperinterval",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            handler->PSendSysMessage("Syntax is: ahbotoptions bidsperinterval $ahMapID (2, 6 or 7) $bids");
            return false;
        }
        auctionbot.Commands(13, ahMapID, NULL, param1);
    }
    else
    {
        handler->PSendSysMessage("Syntax is: ahbotoptions $option $ahMapID (2, 6 or 7) $parameter");
        handler->PSendSysMessage("Try ahbotoptions help to see a list of options.");
        return false;
    }
    return true;
}
};

void AddSC_mmowning_commandscript()
{
    new mmowning_commandscript();
}