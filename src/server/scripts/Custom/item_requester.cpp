/* Copyright (C) 2010 SpectralCreations
 * Author: Spectre
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

#include "ScriptPCH.h"

// All Text will be stored here for easy configuration

// ScriptName much match scriptName in creature_template
#define REQUESTER_SCRIPT_NAME "custom_gossip_script_itemrequester"

// Menus
#define REQUESTER_MENU_REQUEST_ITEM "Request an item"
#define REQUESTER_MENU_REQUEST_CONFIRMATION "Enter Item Id Or Name"
#define REQUESTER_MENU_EXIT	"[Exit]"

// Errors
#define REQUESTER_ERROR_INVALID_ITEM "Invalid Item entered"
#define REQUESTER_ERROR_QUALITY_TOO_HIGH "Requested Item Quality is too high."
#define REQUESTER_ERROR_REQUIREMENTS_NOT_MET "Item cannot be requested, you do not meet the requirements."
#define REQUESTER_ERROR_ITEM_CREATION_FAILED "Item creation failed"

// Mail
#define REQUESTER_DEFAULT_MAIL_SUBJECT "ItemRequester: Your item has arrived"
#define REQUESTER_DEFAULT_MAIL_BODY ""
#define REQUESTER_MAIL_SUCCESS "Item has been mailed"

// Level Requirements
enum REQUESTER_QUALITY_LEVEL_REQUIRED
{
	REQUESTER_QUALITY_LEVEL_PNU		= 55,	// ITEM_QUALITY_POOR / ITEM_QUALITY_NORMAL / ITEM_QUALITY_UNCOMMON
	REQUESTER_QUALITY_LEVEL_RARE	= 60,	// ITEM_QUALITY_RARE
	REQUESTER_QUALITY_LEVEL_EPIC	= 65	// ITEM_QUALITY_EPIC
};

enum ITEM_REQUESTER_MENUS
{
	ITEM_REQUESTER_MENU_SELECT_ITEM	= 1,
	ITEM_REQUESTER_MENU_EXIT,
};

class ItemRequester : public CreatureScript
{
	public:
		ItemRequester() : CreatureScript("ItemRequester") {}
		
		bool OnGossipHello(Player *pPlayer, Creature *pCreature) 
		{ 
			pPlayer->PlayerTalkClass->GetGossipMenu().AddMenuItem(8, REQUESTER_MENU_REQUEST_ITEM, ITEM_REQUESTER_MENU_SELECT_ITEM, GOSSIP_ACTION_INFO_DEF + 1, REQUESTER_MENU_REQUEST_CONFIRMATION, 0, true);
			pPlayer->PlayerTalkClass->GetGossipMenu().AddMenuItem(8, REQUESTER_MENU_EXIT, ITEM_REQUESTER_MENU_EXIT, GOSSIP_ACTION_INFO_DEF + 1, "", false);
			pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
			return true; 
		}

        bool OnGossipSelect(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action) 
		{
			pPlayer->PlayerTalkClass->ClearMenus();
			switch(sender)
			{
				case ITEM_REQUESTER_MENU_EXIT:
				{
					pPlayer->CLOSE_GOSSIP_MENU();
				}break;
			}

			return true;
		}

		bool OnGossipSelectCode(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action, const char *pCode)
		{
			switch(sender)
			{
				case ITEM_REQUESTER_MENU_SELECT_ITEM:
				{
					pPlayer->CLOSE_GOSSIP_MENU();
					uint32 itemId = atol(pCode);
					if(itemId == 0)
					{
						std::string escapedCode = pCode;
						WorldDatabase.escape_string(escapedCode);
						if(QueryResult queryResult = WorldDatabase.PQuery("SELECT `entry` FROM `item_template` WHERE `name` = '%s'", escapedCode.c_str()))
							itemId = queryResult->Fetch()[0].GetUInt32();
					}

					const ItemPrototype *itemProto = sObjectMgr->GetItemPrototype(itemId);
					if(itemProto == NULL)
					{
						ChatHandler(pPlayer).PSendSysMessage(REQUESTER_ERROR_INVALID_ITEM);
						return true;
					}

					if(CheckQualityRequirements(pPlayer, itemProto->Quality) == false)
					{
						ChatHandler(pPlayer).PSendSysMessage(REQUESTER_ERROR_REQUIREMENTS_NOT_MET);
						return true;
					}
					if(CreateItemAndMailToPlayer(pPlayer, itemId) == true)
						ChatHandler(pPlayer).PSendSysMessage(REQUESTER_MAIL_SUCCESS);
					else
						ChatHandler(pPlayer).PSendSysMessage(REQUESTER_ERROR_ITEM_CREATION_FAILED );
				}break;
			}
			return true;
		}

		bool CheckQualityRequirements(Player *pPlayer, uint32 qualityType)
		{
			switch(qualityType)
			{
				case ITEM_QUALITY_POOR:
				case ITEM_QUALITY_NORMAL:
				case ITEM_QUALITY_UNCOMMON:
				{
					if(pPlayer->getLevel() >= REQUESTER_QUALITY_LEVEL_PNU)
						return true;

				}break;

				case ITEM_QUALITY_RARE:
				{
					if(pPlayer->getLevel() >= REQUESTER_QUALITY_LEVEL_RARE)
						return true;
				}break;

				case ITEM_QUALITY_EPIC:
				{
					if(pPlayer->getLevel() >= REQUESTER_QUALITY_LEVEL_EPIC)
						return true;
				}break;

				default:
				{
					ChatHandler(pPlayer).PSendSysMessage(REQUESTER_ERROR_QUALITY_TOO_HIGH);
				}break;
			}
			return false;
		}

		bool CreateItemAndMailToPlayer(Player *pPlayer, uint32 itemId)
		{
			Item *pItem = new Item();
			if(pItem->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), itemId, pPlayer) == false)
				return false;

			MailSender toSend(MAIL_NORMAL, pPlayer->GetGUIDLow(), MAIL_STATIONERY_GM);
			SQLTransaction trans = CharacterDatabase.BeginTransaction();
			pItem->SaveToDB(trans);

			MailDraft mailDraft(REQUESTER_DEFAULT_MAIL_SUBJECT, REQUESTER_DEFAULT_MAIL_BODY);
			mailDraft.AddItem(pItem);
			mailDraft.SendMailTo(trans, MailReceiver(pPlayer), toSend);
			CharacterDatabase.CommitTransaction(trans);
			return true;
		}
};

void AddSC_ItemRequester()
{
	new ItemRequester();
}