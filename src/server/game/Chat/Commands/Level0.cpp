/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "DatabaseEnv.h"
#include "World.h"
#include "Player.h"
#include "Opcodes.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "AccountMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "Util.h"

bool ChatHandler::HandleHelpCommand(const char* args)
{
    char* cmd = strtok((char*)args, " ");
    if (!cmd)
    {
        ShowHelpForCommand(getCommandTable(), "help");
        ShowHelpForCommand(getCommandTable(), "");
    }
    else
    {
        if (!ShowHelpForCommand(getCommandTable(), cmd))
            SendSysMessage(LANG_NO_HELP_CMD);
    }

    return true;
}

bool ChatHandler::HandleCommandsCommand(const char* /*args*/)
{
    ShowHelpForCommand(getCommandTable(), "");
    return true;
}

bool ChatHandler::HandleStartCommand(const char* /*args*/)
{
    Player *chr = m_session->GetPlayer();

    if (chr->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    if (chr->isInCombat())
    {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
    }

    if ((chr->isDead()) || (chr->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST)))
    {
    // if player is dead and stuck, send ghost to graveyard
    chr->RepopAtGraveyard();
    return true;
    }

    // cast spell Stuck
    chr->CastSpell(chr, 7355, false);
    return true;
}

//Allows your players to gamble for fun and prizes
bool ChatHandler::HandleGambleCommand(const char* args)
{
    Player *chr = m_session->GetPlayer();

    char* px = strtok((char*)args, " ");

    if (!px)
        return false;

    uint32 money = (uint32)atoi(px);

    if (chr->GetMoney() < money)
    {
        SendSysMessage("Du kannst kein Gold setzen welches du nicht hast!");
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
                  SendSysMessage("Du hast gewonnen und einen Einsatz verdoppelt");
             }
             else
             {
                  chr->ModifyMoney(-int(money));
                  SendSysMessage("Du hast verloren");
             }  
        }
    }

    return true;
}

bool ChatHandler::HandleRouletteCommand(const char* args)
{
    Player *chr = m_session->GetPlayer();

    char* px = strtok((char*)args, " ");

    if (!px)
        return false;

    uint32 money = (uint32)atoi(px);

    if (chr->GetMoney() < money)
    {
        SendSysMessage("Du kannst kein Gold setzen welches du nicht hast!");
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
                  SendSysMessage("Du hast das 36x deines Einsatzes gewonnen, GZ!");
             }
             else
             {
                  chr->ModifyMoney(-int(money));
                  SendSysMessage("Du hast verloren");
             }  
        }
    }

     return true;
 }

//Mall Teleporter
bool ChatHandler::HandleMallCommand(const char* /*args*/)
{
        //MALL command
        
        Player *chr = m_session->GetPlayer();

        if (chr->isInCombat())
        {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
        }
        if (chr->isInFlight())
        {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
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

//Buffer
bool ChatHandler::HandleBuffsCommand(const char* /*args*/)              
{
                                Player *chr = m_session->GetPlayer();
                                
        if (chr->isInCombat())
        {
        SendSysMessage("Du kannst dich nicht während eines Kampfes buffen");
        SetSentErrorMessage(true);
        return false;
        }
        if (chr->isInFlight())
        {
        SendSysMessage("Du kannste dich nicht während des Fliegens buffen");
        SetSentErrorMessage(true);
        return false;
        }
                
        if (chr->GetMoney() >= 2000000)
                {
                                chr->Unmount();
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
                                SendSysMessage("Du bist jetzt gebufft!");
                                return false;
                }
        else
                {
                SendSysMessage("Du hast nicht genug Gold!");
        }
                return false;
}	

//GuildHouse Tele
bool ChatHandler::HandleGHCommand(const char* args)
{
        Player *chr = m_session->GetPlayer();

        if(chr->isInFlight())
        {
                //pokud hrac leti
                SendSysMessage(LANG_YOU_IN_FLIGHT);
                SetSentErrorMessage(true);
                return false;
        }

        if(chr->isInCombat())
        {
                //pokud je hrac v combatu
                SendSysMessage(LANG_YOU_IN_COMBAT);
                SetSentErrorMessage(true);
                return false;
        }

        if (chr->GetGuildId() == 0)
        {
                //pokud hrac nema guildu
                return false;
        }

        QueryResult result;
            result = CharacterDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", chr->GetGuildId());
        if(result == NULL)
        {
                //pokud guilda nema guildhouse zapsany v tabulce guildhouses
                SendSysMessage("GH Port");
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
 
bool ChatHandler::HandleServerInfoCommand(const char* /*args*/)
{
    uint32 PlayersNum = sWorld->GetPlayerCount();
    uint32 MaxPlayersNum = sWorld->GetMaxPlayerCount();
    uint32 activeClientsNum = sWorld->GetActiveSessionCount();
    uint32 queuedClientsNum = sWorld->GetQueuedSessionCount();
    uint32 maxActiveClientsNum = sWorld->GetMaxActiveSessionCount();
    uint32 maxQueuedClientsNum = sWorld->GetMaxQueuedSessionCount();
    std::string uptime = secsToTimeString(sWorld->GetUptime());
    uint32 updateTime = sWorld->GetUpdateTime();

    SendSysMessage(_FULLVERSION);
    PSendSysMessage(LANG_CONNECTED_PLAYERS, PlayersNum, MaxPlayersNum);
    PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);
    PSendSysMessage(LANG_UPTIME, uptime.c_str());
    PSendSysMessage("Update time diff: %u.", updateTime);

    return true;
}

bool ChatHandler::HandleDismountCommand(const char* /*args*/)
{
    //If player is not mounted, so go out :)
    if (!m_session->GetPlayer()->IsMounted())
    {
        SendSysMessage(LANG_CHAR_NON_MOUNTED);
        SetSentErrorMessage(true);
        return false;
    }

    if (m_session->GetPlayer()->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    m_session->GetPlayer()->Unmount();
    m_session->GetPlayer()->RemoveAurasByType(SPELL_AURA_MOUNTED);
    return true;
}

bool ChatHandler::HandleSaveCommand(const char* /*args*/)
{
    Player* player = m_session->GetPlayer();

    // save GM account without delay and output message
    if (!AccountMgr::IsPlayerAccount(m_session->GetSecurity()))
    {
        if (Player *target = getSelectedPlayer())
            target->SaveToDB();
        else
            player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save if the player has last been saved over 20 seconds ago
    uint32 save_interval = sWorld->getIntConfig(CONFIG_INTERVAL_SAVE);
    if (save_interval == 0 || (save_interval > 20*IN_MILLISECONDS && player->GetSaveTimer() <= save_interval - 20*IN_MILLISECONDS))
        player->SaveToDB();

    return true;
}

/// Display the 'Message of the day' for the realm
bool ChatHandler::HandleServerMotdCommand(const char* /*args*/)
{
    PSendSysMessage(LANG_MOTD_CURRENT, sWorld->GetMotd());
    return true;
}

