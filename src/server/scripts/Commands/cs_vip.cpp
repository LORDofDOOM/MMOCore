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
Name: vip_commandscript
%Complete: 100
Comment: All vip related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "Chat.h"

class vip_commandscript : public CommandScript
{
public:
    vip_commandscript() : CommandScript("vip_commandscript") { }

    ChatCommand* GetCommands() const
    {
        ChatCommand static vipCommandTable[] =
        {
            { "add",            SEC_ADMINISTRATOR,  false, &HandleVIPAddCommand,              "", NULL },
            { "",               SEC_PLAYER,         true,  &HandleVIPCommand,                 "", NULL },
            { NULL,             0,                  false, NULL,                              "", NULL }
        };
        ChatCommand static commandTable[] =
        {
            { "vip",            SEC_PLAYER,         false, NULL,                    "", vipCommandTable },
            { NULL,             0,                  false, NULL,                               "", NULL }
        };
        return commandTable;
    }

    bool static HandleVIPAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        ///- Get the command line arguments
        char* account = strtok ((char*)args, " ");
        char* duration = strtok (NULL," ");

        if (!account || !duration || !atoi(duration))
            return false;

        std::string accountName = account;
        uint32 durationSecs = atoi(duration) * 86400;
        std::string gm = handler->GetSession()->GetPlayerName();

        if (!AccountMgr::normalizeString(accountName))
        {
            handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 targetAccountId = AccountMgr::GetId(accountName);
        if (!targetAccountId)
        {
            handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        /// can set vip only for target with less security
        /// This is also reject self apply in fact
        if (handler->HasLowerSecurityAccount(NULL, targetAccountId, true))
            return false;

        QueryResult resultLogin = LoginDatabase.PQuery("SELECT active FROM account_premium WHERE id = %u AND active = 1", targetAccountId);
        if (resultLogin)
        {
            LoginDatabase.PExecute("UPDATE account_premium "
                "SET gm = '%s', unsetdate = unsetdate + %u "
                "WHERE id = %u", gm.c_str(), durationSecs, targetAccountId);

            handler->PSendSysMessage("Elite geupdated.");

            return true;
        }

        LoginDatabase.PExecute("DELETE FROM account_premium WHERE id = %u", targetAccountId);

        LoginDatabase.PExecute("INSERT INTO account_premium "
            "(id, gm, setdate, unsetdate, active) "
            "VALUES ('%u', '%s', UNIX_TIMESTAMP(), UNIX_TIMESTAMP() + %u, 1)",
            targetAccountId, gm.c_str(), durationSecs);

        handler->PSendSysMessage("Der Account ist jetzt als Elite Account geflaggt.");

        return true;
    }

    static bool HandleVIPCommand(ChatHandler* handler, char const* args)
    {
        uint32 accountId;

        AccountTypes level = handler->GetSession()->GetSecurity();
        if (uint32(level) > 0)
        {
            if (!*args)
                return false;

            std::string accountName = strtok((char*)args, " ");

            if (!AccountMgr::normalizeString(accountName))
            {
                handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }

            accountId = AccountMgr::GetId(accountName);
            if (!accountId)
            {
                handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, accountName.c_str());
                handler->SetSentErrorMessage(true);
                return false;
            }
        }
        else
            accountId = handler->GetSession()->GetAccountId();

        uint32 days = AccountMgr::VipDaysLeft(accountId);

        if (days > 0)
            handler->PSendSysMessage("Du hast keinen Elite Account.", days);
        else
            handler->PSendSysMessage("Dein Elite Account endet in %u Tagen.");

        return true;
    }
};

void AddSC_vip_commandscript()
{
    new vip_commandscript();
}
