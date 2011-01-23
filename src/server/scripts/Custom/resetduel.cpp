#include "ScriptPCH.h"

class Reset_OnDuelEnd : public PlayerScript
{
    public:
        Reset_OnDuelEnd() : PlayerScript("Reset_OnDuelEnd") {}

    void OnDuelEnd(Player *winner, Player *looser, DuelCompleteType type)
    {
        // reset cooldowns in zone 0, 41 and 616
                if (winner->GetZoneId() == 0 || winner->GetZoneId() == 41 || winner->GetZoneId() == 616)
                {
                        winner->RemoveArenaSpellCooldowns();
                        looser->RemoveArenaSpellCooldowns();
                        winner->SetHealth(winner->GetMaxHealth());
                        winner->SetPower(POWER_MANA, winner->GetMaxPower(POWER_MANA));
                        looser->SetHealth(looser->GetMaxHealth());
                        looser->SetPower(POWER_MANA, looser->GetMaxPower(POWER_MANA));
                }
    }
};

void AddSC_Reset()
{
    new Reset_OnDuelEnd;
}