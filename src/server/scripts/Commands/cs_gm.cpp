/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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
Name: gm_commandscript
%Complete: 100
Comment: All gm related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Chat.h"

class gm_commandscript : public CommandScript
{
public:
    gm_commandscript() : CommandScript("gm_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand gmCommandTable[] =
        {
            { "chat",           SEC_MODERATOR,      false, &HandleGMChatCommand,              "", NULL },
            { "fly",            SEC_ADMINISTRATOR,  false, &HandleGMFlyCommand,               "", NULL },
            { "ingame",         SEC_PLAYER,         true,  &HandleGMListIngameCommand,        "", NULL },
            { "list",           SEC_ADMINISTRATOR,  true,  &HandleGMListFullCommand,          "", NULL },
            { "visible",        SEC_MODERATOR,      false, &HandleGMVisibleCommand,           "", NULL },
            { "",               SEC_MODERATOR,      false, &HandleGMCommand,                  "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "gm",             SEC_MODERATOR,      false, NULL,                     "", gmCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    // Enables or disables hiding of the staff badge
    static bool HandleGMChatCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            if (handler->GetSession()->GetPlayer()->isGMChat())
                handler->GetSession()->SendNotification(LANG_GM_CHAT_ON);
            else
                handler->GetSession()->SendNotification(LANG_GM_CHAT_OFF);
            return true;
        }

        std::string argstr = (char*)args;

        if (argstr == "on")
        {
            handler->GetSession()->GetPlayer()->SetGMChat(true);
            handler->GetSession()->SendNotification(LANG_GM_CHAT_ON);
            return true;
        }

        if (argstr == "off")
        {
            handler->GetSession()->GetPlayer()->SetGMChat(false);
            handler->GetSession()->SendNotification(LANG_GM_CHAT_OFF);
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }

    static bool HandleGMFlyCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
            return false;

        Player *target =  handler->getSelectedPlayer();
        if (!target)
            target = handler->GetSession()->GetPlayer();

        WorldPacket data(12);
        if (strncmp(args, "on", 3) == 0)
            data.SetOpcode(SMSG_MOVE_SET_CAN_FLY);
        else if (strncmp(args, "off", 4) == 0)
            data.SetOpcode(SMSG_MOVE_UNSET_CAN_FLY);
        else
        {
            handler->SendSysMessage(LANG_USE_BOL);
            return false;
        }
        data.append(target->GetPackGUID());
        data << uint32(0);                                      // unknown
        target->SendMessageToSet(&data, true);
        handler->PSendSysMessage(LANG_COMMAND_FLYMODE_STATUS, handler->GetNameLink(target).c_str(), args);
        return true;
    }

    static bool HandleGMListIngameCommand(ChatHandler* handler, const char* /*args*/)
    {
        bool first = true;
        bool footer = false;

        ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, *HashMapHolder<Player>::GetLock(), true);
        HashMapHolder<Player>::MapType &m = sObjectAccessor->GetPlayers();
        for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        {
            AccountTypes itr_sec = itr->second->GetSession()->GetSecurity();
            if ((itr->second->isGameMaster() || (itr_sec > SEC_PLAYER && itr_sec <= AccountTypes(sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_GM_LIST)))) &&
                (!handler->GetSession() || itr->second->IsVisibleGloballyFor(handler->GetSession()->GetPlayer())))
            {
                if (first)
                {
                    first = false;
                    footer = true;
                    handler->SendSysMessage(LANG_GMS_ON_SRV);
                    handler->SendSysMessage("========================");
                }
                const char* name = itr->second->GetName();
                uint8 security = itr_sec;
                uint8 max = ((16 - strlen(name)) / 2);
                uint8 max2 = max;
                if (((max)+(max2)+(strlen(name))) == 16)
                    max2 = ((max)-1);
                if (handler->GetSession())
                    handler->PSendSysMessage("|    %s GMLevel %u", name, security);
                else
                    handler->PSendSysMessage("|%*s%s%*s|   %u  |", max, " ", name, max2, " ", security);
            }
        }
        if (footer)
            handler->SendSysMessage("========================");
        if (first)
            handler->SendSysMessage(LANG_GMS_NOT_LOGGED);
        return true;
    }

    /// Display the list of GMs
    static bool HandleGMListFullCommand(ChatHandler* handler, const char* /*args*/)
    {
        ///- Get the accounts with GM Level >0
        QueryResult result = LoginDatabase.Query("SELECT a.username,aa.gmlevel FROM account a, account_access aa WHERE a.id=aa.id AND aa.gmlevel > 0");
        if (result)
        {
            handler->SendSysMessage(LANG_GMLIST);
            handler->SendSysMessage("========================");
            ///- Cycle through them. Display username and GM level
            do
            {
                Field *fields = result->Fetch();
                const char* name = fields[0].GetCString();
                uint8 security = fields[1].GetUInt8();
                uint8 max = ((16 - strlen(name)) / 2);
                uint8 max2 = max;
                if (((max)+(max2)+(strlen(name))) == 16)
                    max2 = ((max)-1);
                if (handler->GetSession())
                    handler->PSendSysMessage("|    %s GMLevel %u", name, security);
                else
                    handler->PSendSysMessage("|%*s%s%*s|   %u  |", max, " ", name, max2, " ", security);
            }
            while (result->NextRow());

            handler->SendSysMessage("========================");
        }
        else
            handler->PSendSysMessage(LANG_GMLIST_EMPTY);
        return true;
    }

    //Enable\Disable Invisible mode
    static bool HandleGMVisibleCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            handler->PSendSysMessage(LANG_YOU_ARE, handler->GetSession()->GetPlayer()->isGMVisible() ?  handler->GetTrinityString(LANG_VISIBLE) : handler->GetTrinityString(LANG_INVISIBLE));
            return true;
        }

        std::string argstr = (char*)args;

        if (argstr == "on")
        {
            handler->GetSession()->GetPlayer()->SetGMVisible(true);
            handler->GetSession()->SendNotification(LANG_INVISIBLE_VISIBLE);
            return true;
        }

        if (argstr == "off")
        {
            handler->GetSession()->SendNotification(LANG_INVISIBLE_INVISIBLE);
            handler->GetSession()->GetPlayer()->SetGMVisible(false);
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }

    //Enable\Disable GM Mode
    static bool HandleGMCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            if (handler->GetSession()->GetPlayer()->isGameMaster())
                handler->GetSession()->SendNotification(LANG_GM_ON);
            else
                handler->GetSession()->SendNotification(LANG_GM_OFF);
            return true;
        }

        std::string argstr = (char*)args;

        if (argstr == "on")
        {
            handler->GetSession()->GetPlayer()->SetGameMaster(true);
            handler->GetSession()->SendNotification(LANG_GM_ON);
            handler->GetSession()->GetPlayer()->UpdateTriggerVisibility();
#ifdef _DEBUG_VMAPS
            VMAP::IVMapManager *vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
            vMapManager->processCommand("stoplog");
#endif
            return true;
        }

        if (argstr == "off")
        {
            handler->GetSession()->GetPlayer()->SetGameMaster(false);
            handler->GetSession()->SendNotification(LANG_GM_OFF);
            handler->GetSession()->GetPlayer()->UpdateTriggerVisibility();
#ifdef _DEBUG_VMAPS
            VMAP::IVMapManager *vMapManager = VMAP::VMapFactory::createOrGetVMapManager();
            vMapManager->processCommand("startlog");
#endif
            return true;
        }

        handler->SendSysMessage(LANG_USE_BOL);
        handler->SetSentErrorMessage(true);
        return false;
    }
};

class anticheat_commandscript : public CommandScript
{
public:
    anticheat_commandscript() : CommandScript("anticheat_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand anticheatCommandTable[] =
        {
            { "global",         SEC_GAMEMASTER,     true,  &HandleAntiCheatGlobalCommand,         "", NULL },
            { "player",         SEC_GAMEMASTER,     true,  &HandleAntiCheatPlayerCommand,         "", NULL },
            { "delete",         SEC_ADMINISTRATOR,  true,  &HandleAntiCheatDeleteCommand,         "", NULL },
            { "handle",         SEC_ADMINISTRATOR,  true,  &HandleAntiCheatHandleCommand,         "", NULL },
            { "jail",           SEC_GAMEMASTER,     true,  &HandleAnticheatJailCommand,         "", NULL },
            { "warn",           SEC_GAMEMASTER,     true,  &HandleAnticheatWarnCommand,         "", NULL },
            { NULL,             0,                     false, NULL,                                           "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "anticheat",      SEC_GAMEMASTER,     true, NULL,                     "",  anticheatCommandTable},
            { NULL,             0,                  false, NULL,                               "", NULL }
        };

        return commandTable;
    }

    static bool HandleAnticheatWarnCommand(ChatHandler* handler, const char* args)
    {
        Player* pTarget = NULL;

        std::string strCommand;

        char* command = strtok((char*)args, " ");

        if (command)
        {
            strCommand = command;
            normalizePlayerName(strCommand);

            pTarget = sObjectMgr->GetPlayer(strCommand.c_str()); //get player by name
        }else
            pTarget = handler->getSelectedPlayer();

        if (!pTarget)
            return false;

        WorldPacket data;

        // need copy to prevent corruption by strtok call in LineFromMessage original string
        char* buf = strdup("The anticheat system has reported several times that you may be cheating. You will be monitored to confirm if this is accurate.");
        char* pos = buf;

        while (char* line = handler->LineFromMessage(pos))
        {
            handler->FillSystemMessageData(&data, line);
            pTarget->GetSession()->SendPacket(&data);
        }

        free(buf);
        return true;
    }
    static bool HandleAnticheatJailCommand(ChatHandler* handler, const char* args)
    {
        Player* pTarget = NULL;

        std::string strCommand;

        char* command = strtok((char*)args, " ");

        if (command)
        {
            strCommand = command;
            normalizePlayerName(strCommand);

            pTarget = sObjectMgr->GetPlayer(strCommand.c_str()); //get player by name
        }else
            pTarget = handler->getSelectedPlayer();

        if (!pTarget)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (pTarget == handler->GetSession()->GetPlayer())
            return false;

        // teleport both to jail.
        pTarget->TeleportTo(1,16226.5f,16403.6f,-64.5f,3.2f);
        handler->GetSession()->GetPlayer()->TeleportTo(1,16226.5f,16403.6f,-64.5f,3.2f);

        WorldLocation loc;

        // the player should be already there, but no :(
        // pTarget->GetPosition(&loc);

        loc.m_mapId = 1;
        loc.m_positionX = 16226.5f;
        loc.m_positionY = 16403.6f;
        loc.m_positionZ = -64.5f;
        loc.m_orientation = 3.2f;

        pTarget->SetHomebind(loc,876);
        return true;
    }

    static bool HandleAntiCheatDeleteCommand(ChatHandler* handler, const char* args)
    {
        std::string strCommand;

        char* command = strtok((char*)args, " "); //get entered name

        if (!command)
            return true;

        strCommand = command;

        if (strCommand.compare("deleteall") == 0)
        {
            uint8 uiStmt[3] = {CHAR_ANTICHEAT_CLEAN_CHEAT_FIRST_REPORT,CHAR_ANTICHEAT_CLEAN_CHEATERS_TEMP, CHAR_ANTICHEAT_CLEAN_CHEATERS };
            for (uint8 uiI = 0; uiI < 3; uiI++)
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(uiStmt[uiI]);
                CharacterDatabase.Execute(stmt);
            }

        } else
        {
            normalizePlayerName(strCommand);
            Player* pPlayer = sObjectMgr->GetPlayer(strCommand.c_str()); //get player by name

            if (!pPlayer)
                handler->PSendSysMessage("Player doesn't exist");
            else
            {
                uint8 uiStmt[3] = {CHAR_ANTICHEAT_DEL_CHEATERS, CHAR_ANTICHEAT_DEL_CHEATERS_TEMP,CHAR_ANTICHEAT_DEL_CHEAT_FIRST_REPORT};

                for (uint8 uiI = 0; uiI < 3; uiI++)
                {
                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(uiStmt[uiI]);
                    stmt->setUInt64(0, pPlayer->GetGUIDLow());
                    CharacterDatabase.Execute(stmt);
                }
            }
        }

        return true;
    }

    static bool HandleAntiCheatPlayerCommand(ChatHandler* handler, const char* args)
    {
        std::string strCommand;

        char* command = strtok((char*)args, " ");

        uint32 uiGUID = 0;
        Player* pPlayer = NULL;

        if (command)
        {
            strCommand = command;

            normalizePlayerName(strCommand);

            pPlayer = sObjectMgr->GetPlayer(strCommand.c_str()); //get player by name

            if (pPlayer)
                uiGUID = pPlayer->GetGUIDLow();
        }else
        {
            pPlayer = handler->getSelectedPlayer();
            if (pPlayer)
                uiGUID = pPlayer->GetGUIDLow();
        }

        if (uiGUID == 0)
        {
            handler->PSendSysMessage("There is no player.");
            return true;
        }

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_ANTICHEAT_GET_CHEATERS_BY_GUID);
        stmt->setUInt32(0,uiGUID);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            do
            {
                Field* fields=result->Fetch();
                uint32 warnings = fields[0].GetUInt32();
                handler->PSendSysMessage("Amount: %u", warnings);
            }
            while (result->NextRow());
        } else
            handler->PSendSysMessage("Player's amount log is empty!.");

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_ANTICHEAT_GET_CHEATERS_AVERAGE_BY_GUID);
        stmt->setUInt32(0,uiGUID);
        result = CharacterDatabase.Query(stmt);

        if (result)
        {
            do
            {
                Field* fields=result->Fetch();
                uint32 average = fields[0].GetUInt32();
                uint32 warnings = fields[1].GetUInt32();

                handler->PSendSysMessage("Average: %u Warnings: %u", average, warnings);
            }
            while (result->NextRow());
        } else
            handler->PSendSysMessage("Player's average log is empty!.");


        stmt = CharacterDatabase.GetPreparedStatement(CHAR_ANTICHEAT_GET_REPORTS_TYPE_BY_GUID);
        stmt->setUInt32(0,uiGUID);
        result = CharacterDatabase.Query(stmt);

        uint32 uiReportsAmount[5] = {0,0,0,0,0};

        if (result)
        {
            do
            {
                Field* fields=result->Fetch();
                std::string report = fields[0].GetString();

                if (report.compare("Speed-Hack") == 0)
                    uiReportsAmount[0]++;
                else if (report.compare("Fly-Hack") == 0)
                    uiReportsAmount[1]++;
                else if (report.compare("WalkOnWater-Hack") == 0)
                    uiReportsAmount[2]++;
                else if (report.compare("Teleport-Hack") == 0)
                    uiReportsAmount[3]++;
                else if (report.compare("Jump-Hack") == 0)
                    uiReportsAmount[4]++;
            }
            while (result->NextRow());
        }

        handler->PSendSysMessage("Speed-Hack reports: %u || Fly-Hack reports: %u ||  WalkOnWater-Hack reports: %u || Teleport-Hack reports: %u || Jump-Hack reports: %u", uiReportsAmount[0],uiReportsAmount[1],uiReportsAmount[2],uiReportsAmount[3],uiReportsAmount[4]);

        return true;
    }

    static bool HandleAntiCheatHandleCommand(ChatHandler* handler, const char* args)
    {
        std::string strCommand;

        char* command = strtok((char*)args, " ");

        if (!command)
            return true;

        if (!handler->GetSession()->GetPlayer())
            return true;

        strCommand = command;

        if (strCommand.compare("on") == 0)
        {
            sWorld->setBoolConfig(CONFIG_ANTICHEAT_ENABLE,true);
            handler->SendSysMessage("The AntiCheat-System is now: Enabled!");
        }
        else if (strCommand.compare("off") == 0)
        {
            sWorld->setBoolConfig(CONFIG_ANTICHEAT_ENABLE,false);
            handler->SendSysMessage("The AntiCheat-System is now: Disabled!");
        }

        return true;
    }

    static bool HandleAntiCheatGlobalCommand(ChatHandler* handler, const char* args)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_ANTICHEAT_GET_CHEATERS);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        handler->PSendSysMessage("Cheaters by Amount: -------------");
        if (result)
        {
            do
            {
                Field* fields=result->Fetch();
                std::string name = fields[0].GetCString();
                uint32 warnings = fields[1].GetUInt32();

                handler->PSendSysMessage("Name: %s Warnings: %u", name.c_str(), warnings);
            }
            while (result->NextRow());
        } else
            handler->PSendSysMessage("Cheaters amount log empty!.");

        handler->PSendSysMessage("Cheaters by Average: -------------");

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_ANTICHEAT_GET_CHEATERS_AVERAGE);
        result = CharacterDatabase.Query(stmt);

        if (result)
        {
            do
            {
                Field* fields=result->Fetch();
                std::string name = fields[0].GetCString();
                uint32 average = fields[1].GetUInt32();
                uint32 warnings = fields[2].GetUInt32();

                handler->PSendSysMessage("Name: %s  Average: %u Warnings: %u", name.c_str(), average, warnings);
            }
            while (result->NextRow());
        } else
            handler->PSendSysMessage("Cheaters average log empty!.");

        return true;
    }
};

void AddSC_gm_commandscript()
{
    new gm_commandscript();
    new anticheat_commandscript();
}
