/*
* Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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
SDName: BloodyWars Lottery
SDAuthor: PrinceCreed
SD%Complete: 100%
SDComment: //
SDCategory: Custom
EndScriptData */

#include "ScriptPCH.h"

/*######
## npc_lotto
######*/

#define GOSSIP_BUY_TICKET "Kauf dir ein Lotterie Los"
#define TICKET_COST 500000
#define EVENT_BLOODYLOTTO 132

class npc_lotto : public CreatureScript
{
public:
    npc_lotto() : CreatureScript("npc_lotto") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pPlayer && !pPlayer->isGameMaster())
        {
            QueryResult result = CharacterDatabase.PQuery("SELECT id FROM lotto_tickets WHERE guid=%u", pPlayer->GetGUIDLow());
            if (result)
            {
                pPlayer->SEND_GOSSIP_MENU(100001, pCreature->GetGUID());
                return false;
            }

            pPlayer->PrepareGossipMenu(pCreature);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_BUY_TICKET, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            pPlayer->SEND_GOSSIP_MENU(100000, pCreature->GetGUID());
        }
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        
        if (!pPlayer->HasEnoughMoney(TICKET_COST))
            return false;
        
        switch(uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF:
                pPlayer->ModifyMoney(-TICKET_COST);
                QueryResult result = CharacterDatabase.Query("SELECT MAX(id) FROM lotto_tickets");
                uint32 id = result->Fetch()->GetUInt32();
                CharacterDatabase.PExecute("INSERT INTO lotto_tickets (id,name,guid) VALUES (%u,'%s',%u);", id+1, pPlayer->GetName(), pPlayer->GetGUIDLow());
                char msg[500];
                sprintf(msg, "Viel Glück, $N. Dein Los hat die Nummer %i", id+1);
                pCreature->MonsterWhisper(msg, pPlayer->GetGUID());
                break;
        }
        pPlayer->PlayerTalkClass->CloseGossip();
        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lottoAI (pCreature);
    }

    struct npc_lottoAI : public ScriptedAI
    {
        npc_lottoAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            SayTimer = 1800*IN_MILLISECONDS;
        }
        
        uint32 SayTimer;

        void UpdateAI(const uint32 diff)
        {
            if (IsEventActive(EVENT_BLOODYLOTTO))
            {
                if (me->IsVisible())
                {
                    me->SetVisible(false);
                    QueryResult result = CharacterDatabase.Query("SELECT MAX(id) FROM lotto_tickets");
                    uint32 maxTickets = result->Fetch()->GetUInt32();
                    if (!maxTickets)
                        return;

                    result = CharacterDatabase.Query("SELECT name, guid FROM `lotto_tickets` ORDER BY RAND() LIMIT 3;");
                    uint32 position = 0;

                    do
                    {
                        ++position;

                        Field *fields = result->Fetch();

                        const char* name = fields[0].GetCString();
                        uint32 guid = fields[1].GetUInt32();
                        uint32 reward = TICKET_COST / (1 << position) * maxTickets;

                        CharacterDatabase.PExecute("INSERT INTO `lotto_extractions` (winner,guid,position,reward) VALUES ('%s',%u,%u,%u);",name,guid,position,reward);

                        // Send reward by mail
                        Player *pPlayer = sObjectMgr->GetPlayerByLowGUID(guid);
                        SQLTransaction trans = CharacterDatabase.BeginTransaction();
                        MailDraft("Herzlichen Glückwunsch", "Du hast in unserer Lotterie gewonnen!")
                            .AddMoney(reward)
                            .SendMailTo(trans, MailReceiver(pPlayer, GUID_LOPART(guid)), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM));
                        CharacterDatabase.CommitTransaction(trans);

                        // Event Message
                        char msg[500];
                        switch (position)
                        {
                            case 1:
                                sWorld->SendWorldText(LANG_EVENTMESSAGE, "| Die Lotteriegewinner wurden gezogen |");
                                sprintf(msg, "| 1 - %s gewinnt %i gold!",name,reward/10000);
                                break;
                            case 2:
                                sprintf(msg, "| 2 - %s gewinnt %i gold!",name,reward/10000);
                                break;
                            case 3:
                                sprintf(msg, "| 3 - %s gewinnt %i gold!",name,reward/10000);
                                break;
                        }
                        sWorld->SendWorldText(LANG_EVENTMESSAGE, msg);
                    }
                    while (result->NextRow());

                    // Delete tickets after extraction
                    CharacterDatabase.PExecute("DELETE FROM lotto_tickets;");
                }
            }
            else
            {
                if (!me->IsVisible())
                    me->SetVisible(true);
                    
                if (SayTimer <= diff)
                {
                    me->MonsterSay("Kauft Lottoscheine! Werdet mit nur 50 Gold Einsatz reich!", 0, NULL);
                    SayTimer = 1800*IN_MILLISECONDS;
                }
                else SayTimer -= diff;
            }
        }
    };

};


void AddSC_npc_lottery()
{
    new npc_lotto;
}