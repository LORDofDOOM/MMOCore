#include "ScriptPCH.h"

///////////////////////////////////////////////////////////////////////
///////////////////////      CONFIG      /////////////////////////////
/////////////////////////////////////////////////////////////////////

float Version = 2.50f; //Don't touch this.
bool PvPSystemEnabled = true; //Set to false if you want to disable the PvP System
bool OnlyInBattlegrounds = false; //Set to true if you want to enable this only in battlegrounds.
bool LooseTokenOnPvPDeath = false; //Set to true if you want the victim to loose tokens when the victim dies.
int32 AmountOfItemsYouWantTheVictimToLoose = 1; //Amount of items you want the victim to loose when victim dies.
bool AddTokenOnPvPKill = true; //Set to false if you don't want the killer to be rewarded.
int32 ItemReward = 29434; //The ItemID of the reward.
int32 AmountOfRewardsOnKillStreak[5] = { 1, 3, 5, 7, 10 }; //With how many items you want to reward the killer when he slays the victim.
int32 HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim = 1; //Name speaks for It self.
const int32 KillerStreak1 = 10; 
const int32 KillerStreak2 = 20; 
const int32 KillerStreak3 = 30; 
const int32 KillerStreak4 = 40; 
const int32 KillerStreak5 = 50; 
int32 KillStreaks[5] = { KillerStreak1, KillerStreak2, KillerStreak3, KillerStreak4, KillerStreak5 };//On how many kills the killstreaks should announce & Reward.

///////////////////////////////////////////////////////////////////
///////////////////////      END      ////////////////////////////
/////////////////////////////////////////////////////////////////

struct SystemInfo
{
    uint32              KillStreak;
    uint64              LastGUIDKill;
    uint8               KillCount;
};

static std::map<uint32, SystemInfo> KillingStreak;

class System_OnPvPKill : public PlayerScript
{
    public:
        System_OnPvPKill() : PlayerScript("System_OnPvPKill") {}

        
void OnPVPKill(Player *pKiller, Player *pVictim)
{
    if(!PvPSystemEnabled)
    {
             return;
    }
    else if(PvPSystemEnabled)
    {


        uint64 kGUID; 
        uint64 vGUID;
        char msg[500];
        kGUID = pKiller->GetGUID();
        vGUID = pVictim->GetGUID();                 
        if(kGUID == vGUID)
        {
          return;
        }
                
      if(KillingStreak[kGUID].LastGUIDKill == vGUID)






      {
          KillingStreak[kGUID].KillCount++;
          KillingStreak[vGUID].KillCount = 1;
          pKiller->AddItem(ItemReward, 1);
          if(LooseTokenOnPvPDeath == true)


              pVictim->DestroyItemCount(ItemReward, AmountOfItemsYouWantTheVictimToLoose, true, false);
      }

      if(KillingStreak[kGUID].LastGUIDKill != vGUID)
      {
          KillingStreak[kGUID].KillCount = 1;
          KillingStreak[vGUID].KillCount = 1;
      }

      if(KillingStreak[kGUID].KillCount == HowManyTimesYouWantTheKillerToGetAwardedForKillingTheSameVictim)
      {
          return;
      }

      if(OnlyInBattlegrounds)

      {
          if(!pKiller->GetMap()->IsBattleground())
              return;
      }

      KillingStreak[kGUID].KillStreak++;
      KillingStreak[vGUID].KillStreak = 0;
      KillingStreak[kGUID].LastGUIDKill = vGUID;
      KillingStreak[vGUID].LastGUIDKill = 0;
      if(AddTokenOnPvPKill == true)
          pKiller->AddItem(ItemReward, 1);
      if(LooseTokenOnPvPDeath == true)

          pVictim->DestroyItemCount(ItemReward, AmountOfItemsYouWantTheVictimToLoose, true, false);
                
        switch(KillingStreak[kGUID].KillStreak)
        {
        case KillerStreak1:
        sprintf(msg, "[PvP System]: %s killed %s and is on a %u kill streak. ", pKiller->GetName(), pVictim->GetName(), KillStreaks[1]);
        sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[1]);
        break;

        case KillerStreak2:
        sprintf(msg, "[PvP System]: %s killed %s and is on a %u kill streak. ", pKiller->GetName(), pVictim->GetName(), KillStreaks[2]);
        sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[2]);
        break;

        case KillerStreak3:
        sprintf(msg, "[PvP System]: %s killed %s and is on a %u kill streak. ", pKiller->GetName(), pVictim->GetName(), KillStreaks[3]);
        sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[3]);
        break;

        case KillerStreak4:
        sprintf(msg, "[PvP System]: %s killed %s and is on a %u kill streak. ", pKiller->GetName(), pVictim->GetName(), KillStreaks[4]);
        sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[4]);
        break;

        case KillerStreak5:
        sprintf(msg, "[PvP System]: %s killed %s and is on a %u kill streak. ", pKiller->GetName(), pVictim->GetName(), KillStreaks[5]);
        sWorld->SendWorldText(LANG_SYSTEMMESSAGE, msg);
        pKiller->AddItem(ItemReward, AmountOfRewardsOnKillStreak[5]);
        KillingStreak[kGUID].KillStreak = 0;
        break;
     }
    }
}
};

void AddSC_PVP_System()
{
    new System_OnPvPKill;
}