#include "ScriptPCH.h"

enum LevelRewards
{
    EMBLEM_OF_CONQUEST  = 45624
};

class level_rewards : public PlayerScript
{
public:
    level_rewards() : PlayerScript("level_rewards") {}

    void OnLevelChanged(Player* player, uint8 oldLevel)
    {
        if (player->isGameMaster())
            return;

        uint32 money = 0;

        switch (oldLevel)
        {
            case 9:
                money = 15000;
                break;
            case 19:
                money = 25000;
                break;
            case 29:
                money = 40000;
                break;
            case 39:
                money = 50000;
                break;
            case 49:
                money = 60000;
                break;
            case 59:
                money = 100000;
                break;
            case 69:
                money = 150000;
                break;
            case 79:
                money = 200000;
                break;
            default:
                return;
        }

        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);

        MailDraft draft("Herzlichen Glückwunsch", "Herzlichen Glückwünsch du bist auf MMOwning vorangekommen und verdienst eine Belohnung. :)");

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        if (money > 0)
            draft.AddMoney(money);
		/*
        if (oldLevel == 79)
        {
            Item* item = Item::CreateItem(EMBLEM_OF_CONQUEST, 300, 0);
            item->SaveToDB(trans);
            draft.AddItem(item);
        }
		*/
        draft.SendMailTo(trans, MailReceiver(player), sender);
        CharacterDatabase.CommitTransaction(trans);
    }
};

void AddSC_level_rewards()
{
    new level_rewards();
}
