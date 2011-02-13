/*
 * Copyright (C) 2008 - 2010 Trinity <http://www.trinitycore.org/>
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
#include "trial_of_the_crusader.h"
#include "Group.h"

extern Position positionCenter;

const DoorData doorData[] =
{
    {GO_DOOR_E,     DATA_BEASTS,              DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GO_DOOR_E,     DATA_LORD_JARAXXUS,       DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GO_DOOR_E,     DATA_FACTION_CHAMPIONS,   DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GO_DOOR_E,     DATA_TWIN_VALKYR,         DOOR_TYPE_ROOM,     BOUNDARY_W},
    {0,             0,                        DOOR_TYPE_ROOM,     0} // EOF
};


class instance_trial_of_the_crusader : public InstanceMapScript
{
public:
    instance_trial_of_the_crusader() : InstanceMapScript("instance_trial_of_the_crusader", 649) { }

    InstanceScript* GetInstanceScript(InstanceMap* pMap) const
    {
        return new instance_trial_of_the_crusader_InstanceScript(pMap);
    }

    struct instance_trial_of_the_crusader_InstanceScript : public InstanceScript
    {
        instance_trial_of_the_crusader_InstanceScript(Map* pMap) : InstanceScript(pMap)
    {
        SetBossNumber(MAX_ENCOURTMENT);
        LoadDoorData(doorData);
    }

    uint64 uiGormokGuid;
    uint64 uiDreadscaleGuid;
    uint64 uiAcidmawGuid;
    uint64 uiIcehowlGuid;
    uint64 uiJaraxussGuid;

    uint64 uiDK_Horde;
    uint64 uiSowa_Horde;
    uint64 uiDrzewo_Horde;
    uint64 uiHunter_Horde;
    uint64 uiMage_Horde;
    uint64 uiHolyPala_Horde;
    uint64 uiRetriPala_Horde;
    uint64 uiDisci_Horde;
    uint64 uiShadow_Horde;
    uint64 uiRogal_Horde;
    uint64 uiEnh_Horde;
    uint64 uiRestoSh_Horde;
    uint64 uiLock_Horde;
    uint64 uiWarek_Horde;

    uint64 uiDK_Ally;
    uint64 uiSowa_Ally;
    uint64 uiDrzewo_Ally;
    uint64 uiHunter_Ally;
    uint64 uiMage_Ally;
    uint64 uiHolyPala_Ally;
    uint64 uiRetriPala_Ally;
    uint64 uiDisci_Ally;
    uint64 uiShadow_Ally;
    uint64 uiRogal_Ally;
    uint64 uiEnh_Ally;
    uint64 uiRestoSh_Ally;
    uint64 uiLock_Ally;
    uint64 uiWarek_Ally;

    uint64 uiDarkTwinGuid;
    uint64 uiLightTwinGuid;
    uint64 uiAnubarakGuid;


    uint64 uiTirionGuid;
    uint64 uiVrynnGuid;
    uint64 uiGaroshGuid;
    uint64 uiGnomLockGuid;
    uint64 uiLichKingGuid;

    uint64 uiGadajacyGuid;

    uint64 uiMainGateGuid;
    uint64 uiFloorGuid;
    uint64 uiCacheGuid;

    uint32 m_team;

    uint64 uiSnoboldGuid_1;
    uint64 uiSnoboldGuid_2;
    uint64 uiSnoboldGuid_3;
    uint64 uiSnoboldGuid_4;

    time_t acidmawDead;
    time_t dreadscaleDead;


    uint32 attempts;
    uint32 deathsCount;

    std::vector<time_t> scarabsDeaths;
    std::vector<uint64> misstressOfPain;

    bool loading;

    void Initialize()
    {
        uiGormokGuid = 0;
        uiDreadscaleGuid = 0;
        uiAcidmawGuid = 0;
        uiIcehowlGuid = 0;
        uiJaraxussGuid = 0;

        uiDK_Horde = 0;
        uiSowa_Horde = 0;
        uiDrzewo_Horde = 0;
        uiHunter_Horde = 0;
        uiMage_Horde = 0;
        uiHolyPala_Horde = 0;
        uiRetriPala_Horde = 0;
        uiDisci_Horde = 0;
        uiShadow_Horde = 0;
        uiRogal_Horde = 0;
        uiEnh_Horde = 0;
        uiRestoSh_Horde = 0;
        uiLock_Horde = 0;
        uiWarek_Horde = 0;

        uiDK_Ally = 0;
        uiSowa_Ally = 0;
        uiDrzewo_Ally = 0;
        uiHunter_Ally = 0;
        uiMage_Ally = 0;
        uiHolyPala_Ally = 0;
        uiRetriPala_Ally = 0;
        uiDisci_Ally = 0;
        uiShadow_Ally = 0;
        uiRogal_Ally = 0;
        uiEnh_Ally = 0;
        uiRestoSh_Ally = 0;
        uiLock_Ally = 0;
        uiWarek_Ally = 0;

        uiDarkTwinGuid = 0;
        uiLightTwinGuid = 0;
        uiAnubarakGuid = 0;


        uiTirionGuid = 0;
        uiVrynnGuid = 0;
        uiGaroshGuid = 0;
        uiGnomLockGuid = 0;
        uiLichKingGuid = 0;

        uiGadajacyGuid = 0;

        uiMainGateGuid = 0;
        uiFloorGuid = 0;
        uiCacheGuid = 0;

        m_team = 0;

        uiSnoboldGuid_1 = 0;
        uiSnoboldGuid_2 = 0;
        uiSnoboldGuid_3 = 0;
        uiSnoboldGuid_4 = 0;

        acidmawDead = 0;
        dreadscaleDead = 0;

        attempts = 50;
        deathsCount = 0;

        scarabsDeaths.clear();
        misstressOfPain.clear();

        loading = false;
    }

    void OnCreatureCreate(Creature* creature)
    {
        switch (creature->GetEntry())
        {
        case NPC_GORMOK:                        uiGormokGuid        = creature->GetGUID(); return;
        case NPC_DREADSCALE:                    uiDreadscaleGuid    = creature->GetGUID(); return;
        case NPC_ACIDMAW:                       uiAcidmawGuid       = creature->GetGUID(); return;
        case NPC_ICEHOWL:                       uiIcehowlGuid       = creature->GetGUID(); return;
        case NPC_LORD_JARAXXUS:                 uiJaraxussGuid      = creature->GetGUID(); return;

        case NPC_DK_HORDE:                      uiDK_Horde          = creature->GetGUID(); return;
        case NPC_SOWA_HORDE:                    uiSowa_Horde        = creature->GetGUID(); return;
        case NPC_DRZEWO_HORDE:                  uiDrzewo_Horde      = creature->GetGUID(); return;
        case NPC_HUNTER_HORDE:                  uiHunter_Horde      = creature->GetGUID(); return;
        case NPC_MAGE_HORDE:                    uiMage_Horde        = creature->GetGUID(); return;
        case NPC_HOLYPALA_HORDE:                uiHolyPala_Horde    = creature->GetGUID(); return;
        case NPC_RETRIPALA_HORDE:               uiRetriPala_Horde   = creature->GetGUID(); return;
        case NPC_DISCI_HORDE:                   uiDisci_Horde       = creature->GetGUID(); return;
        case NPC_SHADOW_HORDE:                  uiShadow_Horde      = creature->GetGUID(); return;
        case NPC_ROGAL_HORDE:                   uiRogal_Horde       = creature->GetGUID(); return;
        case NPC_ENH_HORDE:                     uiEnh_Horde         = creature->GetGUID(); return;
        case NPC_RESTOSH_HORDE:                 uiRestoSh_Horde     = creature->GetGUID(); return;
        case NPC_LOCK_HORDE:                    uiLock_Horde        = creature->GetGUID(); return;
        case NPC_WAREK_HORDE:                   uiWarek_Horde       = creature->GetGUID(); return;

        case NPC_DK_ALLY:                       uiDK_Ally           = creature->GetGUID(); return;
        case NPC_SOWA_ALLY:                     uiSowa_Ally         = creature->GetGUID(); return;
        case NPC_DRZEWO_ALLY:                   uiDrzewo_Ally       = creature->GetGUID(); return;
        case NPC_HUNTER_ALLY:                   uiHunter_Ally       = creature->GetGUID(); return;
        case NPC_MAGE_ALLY:                     uiMage_Ally         = creature->GetGUID(); return;
        case NPC_HOLYPALA_ALLY:                 uiHolyPala_Ally     = creature->GetGUID(); return;
        case NPC_RETRIPALA_ALLY:                uiRetriPala_Ally    = creature->GetGUID(); return;
        case NPC_DISCI_ALLY:                    uiDisci_Ally        = creature->GetGUID(); return;
        case NPC_SHADOW_ALLY:                   uiShadow_Ally       = creature->GetGUID(); return;
        case NPC_ROGAL_ALLY:                    uiRogal_Ally        = creature->GetGUID(); return;
        case NPC_ENH_ALLY:                      uiEnh_Ally          = creature->GetGUID(); return;
        case NPC_RESTOSH_ALLY:                  uiRestoSh_Ally      = creature->GetGUID(); return;
        case NPC_LOCK_ALLY:                     uiLock_Ally         = creature->GetGUID(); return;
        case NPC_WAREK_ALLY:                    uiWarek_Ally        = creature->GetGUID(); return;

        case NPC_DARK_TWIN:                     uiDarkTwinGuid      = creature->GetGUID(); return;
        case NPC_LIGHT_TWIN:                    uiLightTwinGuid     = creature->GetGUID(); return;
        case NPC_ANUBARAK:                      uiAnubarakGuid      = creature->GetGUID(); return;
        case NPC_TIRION_FORDING:                uiTirionGuid        = creature->GetGUID(); return;
        case NPC_WARIAN_VRYNN:                  uiVrynnGuid         = creature->GetGUID(); return;
        case NPC_GAROSH_HELLSCREAM:             uiGaroshGuid        = creature->GetGUID(); return;
        case NPC_GNOM_WARLOCK:                  uiGnomLockGuid      = creature->GetGUID(); return;
        case NPC_LICH_KING:                     uiLichKingGuid      = creature->GetGUID(); return;

        case NPC_GADAJACY:                      uiGadajacyGuid      = creature->GetGUID(); return;
        case NPC_SNOBOLD:
            if (!uiSnoboldGuid_1)
                uiSnoboldGuid_1 = creature->GetGUID();
            else if (!uiSnoboldGuid_2)
                uiSnoboldGuid_2 = creature->GetGUID();
            else if (!uiSnoboldGuid_3)
                uiSnoboldGuid_3 = creature->GetGUID();
            else if (!uiSnoboldGuid_4)
                uiSnoboldGuid_4 = creature->GetGUID();

            return;

        case NPC_MISSTRESS_OF_PAIN:
            misstressOfPain.push_back(creature->GetGUID());
            return;
        }
    }


    void OnGameObjectCreate(GameObject* go)
    {
        switch (go->GetEntry())
        {
        case GO_MAIN_GATE: uiMainGateGuid = go->GetGUID(); break;
        case GO_FLOOR: uiFloorGuid = go->GetGUID(); break;
        case GO_CACHE: uiCacheGuid = go->GetGUID(); break;
        }

        AddDoor(go, NULL);
    }

    uint64 GetData64(uint32 data)
    {
        switch (data)
        {
        case BOSS_GORMOK:                       return uiGormokGuid;
        case BOSS_DREADSCALE:                   return uiDreadscaleGuid;
        case BOSS_ACIDMAW:                      return uiAcidmawGuid;
        case BOSS_ICEHOWL:                      return uiIcehowlGuid;
        case BOSS_JARAXXUS:                     return uiJaraxussGuid;

        case BOSS_CHAMPION_DK:                  return m_team == ALLIANCE?uiDK_Horde:uiDK_Ally; //dla graczy z przymierza mobki sa z hordy
        case BOSS_CHAMPION_SOWA:                return m_team == ALLIANCE?uiSowa_Horde:uiSowa_Ally;
        case BOSS_CHAMPION_DRZEWO:              return m_team == ALLIANCE?uiDrzewo_Horde:uiDrzewo_Ally;
        case BOSS_CHAMPION_HUNTER:              return m_team == ALLIANCE?uiHunter_Horde:uiHunter_Ally;
        case BOSS_CHAMPION_MAGE:                return m_team == ALLIANCE?uiMage_Horde:uiMage_Ally;
        case BOSS_CHAMPION_HOLYPALA:            return m_team == ALLIANCE?uiHolyPala_Horde:uiHolyPala_Ally;
        case BOSS_CHAMPION_RETRIPALA:           return m_team == ALLIANCE?uiRetriPala_Horde:uiRetriPala_Ally;
        case BOSS_CHAMPION_DISCI:               return m_team == ALLIANCE?uiDisci_Horde:uiDisci_Ally;
        case BOSS_CHAMPION_SHADOW:              return m_team == ALLIANCE?uiShadow_Horde:uiShadow_Ally;
        case BOSS_CHAMPION_ROGAL:               return m_team == ALLIANCE?uiRogal_Horde:uiRogal_Ally;
        case BOSS_CHAMPION_ENH:                 return m_team == ALLIANCE?uiEnh_Horde:uiEnh_Ally;
        case BOSS_CHAMPION_RESTOSH:             return m_team == ALLIANCE?uiRestoSh_Horde:uiRestoSh_Ally;
        case BOSS_CHAMPION_LOCK:                return m_team == ALLIANCE?uiLock_Horde:uiLock_Ally;
        case BOSS_CHAMPION_WAREK:               return m_team == ALLIANCE?uiWarek_Horde:uiWarek_Ally;

        case BOSS_DARK_TWIN:                    return uiDarkTwinGuid;
        case BOSS_LIGHT_TWIN:                   return uiLightTwinGuid;
        case BOSS_ANUBARAK:                     return uiAnubarakGuid;
        case TIRION_FORDING:                    return uiTirionGuid;
        case WARIAN_VRYNN:                      return uiVrynnGuid;
        case GAROSH_HELLSCREAM:                 return uiGaroshGuid;
        case GNOM_WARLOCK:                      return uiGnomLockGuid;
        case LICH_KING:                         return uiLichKingGuid;
        case MAIN_GATE:                         return uiMainGateGuid;
        case FLOOR:                             return uiFloorGuid;
        case CHAMPIONS_CACHE:                   return uiCacheGuid;

        case GADAJACY:                          return uiGadajacyGuid;
        }

        return 0;
    }


    void OnPlayerEnter(Player* player)
    {
        if (!m_team)
        {
            if (player->GetGroup())
                m_team = sObjectMgr->GetPlayerTeamByGUID(player->GetGroup()->GetLeaderGUID());
            else
                m_team = player->GetTeam();
        }
    }



    uint32 GetData(uint32 data)
    {
        if (data == DATA_TEAM)
            return m_team;

        if (data == DATA_ATTEMPTS)
            return attempts;

        if (data == DATA_DEATHS_COUNT)
            return deathsCount;

        return 0;
    }

    void SetData(uint32 idx, uint32 data)
    {
        if (idx == SCARAB_DEATH)
        {
            scarabsDeaths.push_back(time(NULL));
            if (scarabsDeaths.size() >= 40)
            {
                for (int i=0; i<(int)scarabsDeaths.size(); ++i)
                {
                    time_t scarab1 = scarabsDeaths[i];
                    uint32 count = 0;

                    for (int j=0; j<(int)scarabsDeaths.size(); ++j)
                    {
                        if (i == j)
                            continue;

                        time_t scarab2 = scarabsDeaths[j];

                        if ((scarab2 > scarab1 ? scarab2 - scarab1 : scarab1 - scarab2) <= 30)
                            ++count;

                        if (count >= 40)
                        {
                            if (instance->GetSpawnMode() == 0 || instance->GetSpawnMode() == 2)
                                DoCompleteAchievement(3800);
                            if (instance->GetSpawnMode() == 1 || instance->GetSpawnMode() == 3)
                                DoCompleteAchievement(3816);

                            break;
                        }
                    }

                    if (count >= 40)
                        break;
                }
            }

            return;
        }

        if (idx == UPDATE_DEATHS_COUNT)
        {
            ++deathsCount;
            return;
        }

        if (idx == TIME_ACIDMAW_DEAD || idx == TIME_DREADSCALE_DEAD)
        {
            if (idx == TIME_ACIDMAW_DEAD)
                acidmawDead = time(NULL);
            if (idx == TIME_DREADSCALE_DEAD)
                dreadscaleDead = time(NULL);

            if (acidmawDead && dreadscaleDead)
            {
                time_t interval = acidmawDead >= dreadscaleDead ? acidmawDead - dreadscaleDead : dreadscaleDead - acidmawDead;
                if (interval <= 10)
                {
                    //Not One, But Two Jormungars
                    uint32 achi = 0;
                    if (instance->GetSpawnMode() == 0 || instance->GetSpawnMode() == 2) //10 man normal/hero
                        achi = 3936;
                    else if (instance->GetSpawnMode() == 1 || instance->GetSpawnMode() == 3) //25 man normal/hero
                        achi = 3937;

                    if (achi)
                        DoCompleteAchievement(achi);
                }
            }
        }

        if (idx != RESET_TO_FIGHT)
            return;
        else
        {
            //prevent for '--attempts' for multiple function calls (ie in case of wipe during champions fight, when function is called 6-10 times)
            if (GetBossState(data) != FAIL)
            {
                if (attempts)
                    --attempts;
            }

            if (data == DATA_ANUBARAK)
            {
                scarabsDeaths.clear();
                return;
            }
        }

        for (int i=BOSS_GORMOK; i<=BOSS_DARK_TWIN; ++i)
        {
            if (Creature* boss = instance->GetCreature(GetData64(i)))
                boss->DespawnOrUnsummon();
        }

        if (Creature* gadajacy = instance->GetCreature(GetData64(GADAJACY)))
        {
            if (!gadajacy->isAlive())
                gadajacy->Respawn(true);

            gadajacy->SetVisible(true);
        }

        if (data == DATA_TWIN_VALKYR)
        {
            SetBossState(DATA_ANUBARAK, NOT_STARTED);
            SetBossState(DATA_TWIN_VALKYR, FAIL);
        }

        if (data == DATA_FACTION_CHAMPIONS)
        {
            SetBossState(DATA_ANUBARAK, NOT_STARTED);
            SetBossState(DATA_TWIN_VALKYR, NOT_STARTED);
            SetBossState(DATA_FACTION_CHAMPIONS, FAIL);
        }

        if (data == DATA_LORD_JARAXXUS)
        {
            SetBossState(DATA_ANUBARAK, NOT_STARTED);
            SetBossState(DATA_TWIN_VALKYR, NOT_STARTED);
            SetBossState(DATA_FACTION_CHAMPIONS, NOT_STARTED);
            SetBossState(DATA_LORD_JARAXXUS, FAIL);
        }

        if (data == DATA_BEASTS)
        {
            uiSnoboldGuid_1 = 0;
            uiSnoboldGuid_2 = 0;
            uiSnoboldGuid_3 = 0;
            uiSnoboldGuid_4 = 0;

            acidmawDead = 0;
            dreadscaleDead = 0;

            SetBossState(DATA_ANUBARAK, NOT_STARTED);
            SetBossState(DATA_TWIN_VALKYR, NOT_STARTED);
            SetBossState(DATA_FACTION_CHAMPIONS, NOT_STARTED);
            SetBossState(DATA_LORD_JARAXXUS, NOT_STARTED);
            SetBossState(DATA_BEASTS, FAIL);
        }
    }

    bool SetBossState(uint32 id, EncounterState state)
    {
        if (loading)
            return InstanceScript::SetBossState(id, state);

        if (id == DATA_FACTION_CHAMPIONS && state == DONE)
            instance->GetCreature(uiTirionGuid)->SummonGameObject(GO_CACHE,563.800964f, 140.557205f, 393.957205f, 4.700161f, 0, 0, 0, 0, 0);

        if (id == DATA_LORD_JARAXXUS && (state == DONE || state == FAIL) && !misstressOfPain.empty())
        {
            uint8 count = 0;
            for (std::vector<uint64>::const_iterator itr = misstressOfPain.begin(); itr != misstressOfPain.end(); ++itr)
            {
                Creature* c = instance->GetCreature(*itr);
                if (!c)
                    continue;

                if (c->isAlive())
                    ++count;
            }

            if (state == DONE)
                DoCompleteAchievement(instance->GetSpawnMode() == 0 || instance->GetSpawnMode() == 2 ? 3996 : 3997);

            misstressOfPain.clear();
        }

        if (id == DATA_TWIN_VALKYR && (state == DONE || state == FAIL))
        {
            DoRemoveAurasDueToSpellOnPlayers(65686);
            DoRemoveAurasDueToSpellOnPlayers(65684);
            DoRemoveAurasDueToSpellOnPlayers(67604);
        }

        if (id == DATA_BEASTS && state == DONE)
        {
            uint8 count = 0;
            if (uiSnoboldGuid_1)
            {
                if (Creature* c = instance->GetCreature(uiSnoboldGuid_1))
                    if (c->isAlive())
                        ++count;
            }
            if (uiSnoboldGuid_2)
            {
                if (Creature* c = instance->GetCreature(uiSnoboldGuid_2))
                    if (c->isAlive())
                        ++count;
            }
            if (uiSnoboldGuid_3)
            {
                if (Creature* c = instance->GetCreature(uiSnoboldGuid_3))
                    if (c->isAlive())
                        ++count;
            }
            if (uiSnoboldGuid_4)
            {
                if (Creature* c = instance->GetCreature(uiSnoboldGuid_4))
                    if (c->isAlive())
                        ++count;
            }

            if (count >= 2)
            {
                //Upper Back Pain
                uint32 achi = 0;
                if (instance->GetSpawnMode() == 0 || instance->GetSpawnMode() == 2) //10 man normal/hero
                    achi = 3797;
                else if (instance->GetSpawnMode() == 1 || instance->GetSpawnMode() == 3) //25 man normal/hero
                    achi = 3813;

                if (achi)
                    DoCompleteAchievement(achi);
            }
        }

        if (id == DATA_ANUBARAK && state == DONE)
        {
            if (instance->GetSpawnMode() == 2) //10 man
            {
                if (attempts >= 25)
                    DoCompleteAchievement(3808); //A Tribute to Skill
                if (attempts >= 45)
                    DoCompleteAchievement(3809); //A Tribute to Mad Skill
                if (attempts >= 50)
                    DoCompleteAchievement(3810); //A Tribute to Insanity
            }

            if (instance->GetSpawnMode() == 3) //25 man
            {
                if (attempts >= 25)
                    DoCompleteAchievement(3817); //A Tribute to Skill
                if (attempts >= 45)
                    DoCompleteAchievement(3818); //A Tribute to Mad Skill
                if (attempts >= 50)
                {
                    DoCompleteAchievement(3819); //A Tribute to Insanity

                    if (deathsCount == 0)
                    {
                        //A Tribute to Immortality
                        AchievementEntry const* horde = GetAchievementStore()->LookupEntry(4079);
                        AchievementEntry const* ally = GetAchievementStore()->LookupEntry(4156);
                        Map::PlayerList const &PlayerList = instance->GetPlayers();

                        if (!horde || !ally)
                        {
                            if (!horde) sLog->outError("TSCR: DoCompleteAchievement called for not existing achievement %u", horde);
                            if (!ally) sLog->outError("TSCR: DoCompleteAchievement called for not existing achievement %u", ally);
                            return InstanceScript::SetBossState(id, state);
                        }

                        if (!PlayerList.isEmpty())
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                if (Player *pPlayer = i->getSource())
                                    if (pPlayer->GetTeam() == HORDE)
                                        pPlayer->CompletedAchievement(horde);
                                    else if (pPlayer->GetTeam() == ALLIANCE)
                                        pPlayer->CompletedAchievement(ally);
                    }

                    //A Tribute to Dedicated Insanity
                    bool ok = true;

                    Map::PlayerList const& list = instance->GetPlayers();
                    if (!list.isEmpty())
                    {
                        for (Map::PlayerList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            Player* plr = itr->getSource();
                            if (!plr)
                                continue;

                            for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
                            {
                                if (Item *pItem = plr->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                                {
                                    if (ItemPrototype const* proto = sObjectMgr->GetItemPrototype(pItem->GetEntry()))
                                    {
                                        uint32 lvl = proto->ItemLevel;
                                        uint32 id = proto->ItemId;
                                        //ids are 272 cloacks obtained from 25-man hero Tribute Chest
                                        if (lvl>258 && id!=47545 && id!=47547 && id!=47546 && id!=47549 && id!=47554 && id!=47552 && id!=47548 && id!=47550 && id!=47553 && id!=47551)
                                        {
                                            ok = false;
                                            break;
                                        }
                                    }
                                }
                            }//end for (eq slots)
                        }//end for (players in map)
                    }//end if (not empty)
                    else
                        ok = false;

                    if (ok)
                        DoCompleteAchievement(4080);

                }//end if (50 attempts)
            }//end if (25man)


            if (instance->GetSpawnMode() == 2) //10man hero
            {
                if (attempts >= 25 && attempts < 45)
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195665, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
                else if (attempts >= 45 && attempts < 50)
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195666, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
                else if (attempts >= 50)
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195667, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
                else
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195668, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
            }

            if (instance->GetSpawnMode() == 3) //25man hero
            {
                if (attempts >= 25 && attempts < 45)
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195669, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
                else if (attempts >= 45 && attempts < 50)
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195670, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
                else if (attempts >= 50)
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195671, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
                else
                    instance->GetCreature(uiTirionGuid)->SummonGameObject(195672, 686.117249f, 149.060455f, 142.147736f, 6.164101f, 0, 0, 0, 0, 0);
            }

        }//end if (DONE ANUBARAK)

        return InstanceScript::SetBossState(id, state);
    }

    bool IsEncounterInProgress()
    {
        uint32 data = DATA_BEASTS;
        return (GetBossState(data++) == IN_PROGRESS || GetBossState(data++) == IN_PROGRESS || GetBossState(data++) == IN_PROGRESS
            || GetBossState(data++) == IN_PROGRESS || GetBossState(data) == IN_PROGRESS /* 5th fight - anub*/);
    }


    std::string GetSaveData()
    {
        std::ostringstream saveStream;
        saveStream << GetBossSaveData() << " ";
        saveStream << deathsCount << "";
        saveStream << attempts;
        return saveStream.str();
    }

    void Load(const char * data)
    {
        loading = true;
        std::istringstream loadStream(LoadBossState(data));
        uint32 deaths, tryes;
        loadStream >> deaths;
        loadStream >> tryes;

        deathsCount = deaths;
        attempts = tryes;

        loading = false;
    }
};


};

void AddSC_instance_trial_of_the_crusader()
{
    new instance_trial_of_the_crusader();
}
