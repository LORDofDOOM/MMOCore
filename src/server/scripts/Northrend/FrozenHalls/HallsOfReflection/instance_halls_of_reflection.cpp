#include "ScriptPCH.h"
#include "halls_of_reflection.h"
#include "World.h"

class instance_halls_of_reflection : public InstanceMapScript
{
public:
    instance_halls_of_reflection() : InstanceMapScript("instance_halls_of_reflection", 668) { }

    struct instance_halls_of_reflection_InstanceMapScript : public InstanceScript
    {
        instance_halls_of_reflection_InstanceMapScript(Map* map) : InstanceScript(map) 
        {
            Difficulty = map->GetDifficulty();
            Initialize();
        }

        uint32 m_auiEncounter[MAX_ENCOUNTERS+1];
        uint32 m_auiLider;
        std::string strSaveData;

        uint8 Difficulty;
        uint8 m_uiSummons;

        uint64 m_uiFalricGUID;
        uint64 m_uiMarwynGUID;  
        uint64 m_uiLichKingGUID;
        uint64 m_uiLiderGUID;

        uint64 m_uiMainGateGUID;
        uint64 m_uiExitGateGUID;
        uint64 m_uiDoor2GUID;
        uint64 m_uiDoor3GUID;

        uint64 m_uiFrostGeneralGUID;
        uint64 m_uiCaptainsChestHordeGUID;
        uint64 m_uiCaptainsChestAllianceGUID;
        uint64 m_uiFrostmourneGUID;
        uint64 m_uiFrostmourneAltarGUID;
        uint64 m_uiPortalGUID;
        uint64 m_uiIceWall1GUID;
        uint64 m_uiIceWall2GUID;
        uint64 m_uiIceWall3GUID;
        uint64 m_uiIceWall4GUID;
        uint64 m_uiGoCaveGUID;

        void Initialize()
        {
            for (uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
                m_auiEncounter[i] = NOT_STARTED;
            m_uiMainGateGUID = 0;
            m_uiFrostmourneGUID = 0;
            m_uiFalricGUID = 0;
            m_uiLiderGUID = 0;
            m_uiLichKingGUID = 0;
            m_uiExitGateGUID = 0;
            m_uiSummons = 0;
            m_uiIceWall1GUID = 0;
            m_uiIceWall2GUID = 0;
            m_uiIceWall3GUID = 0;
            m_uiIceWall4GUID = 0;
            m_uiGoCaveGUID = 0;
        }

        void OpenDoor(uint64 guid)
        {
            if(!guid) return;
            GameObject* go = instance->GetGameObject(guid);
            if(go) go->SetGoState(GO_STATE_ACTIVE);
        }

        void CloseDoor(uint64 guid)
        {
            if(!guid) return;
            GameObject* go = instance->GetGameObject(guid);
            if(go) go->SetGoState(GO_STATE_READY);
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch(creature->GetEntry())
            {
                case NPC_FALRIC: 
                       m_uiFalricGUID = creature->GetGUID(); 
                       break;
                case NPC_MARWYN: 
                       m_uiMarwynGUID = creature->GetGUID();  
                       break;
                case BOSS_LICH_KING: 
                       m_uiLichKingGUID = creature->GetGUID();
                       break;
                case NPC_FROST_GENERAL:
                       m_uiFrostGeneralGUID = creature->GetGUID();
                       break;
            }
        }

        void OnPlayerEnter(Player *pPlayer)
        {

        enum PhaseControl
        {
            HORDE_CONTROL_PHASE_SHIFT_1    = 55773,
            HORDE_CONTROL_PHASE_SHIFT_2    = 60028,
            ALLIANCE_CONTROL_PHASE_SHIFT_1 = 55774,
            ALLIANCE_CONTROL_PHASE_SHIFT_2 = 60027,
        };
        
         if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP)) return;

            switch (pPlayer->GetTeam())
            {
                case ALLIANCE:
                      if (pPlayer && pPlayer->IsInWorld() && pPlayer->HasAura(HORDE_CONTROL_PHASE_SHIFT_1))
                          pPlayer->RemoveAurasDueToSpell(HORDE_CONTROL_PHASE_SHIFT_1);
                      pPlayer->CastSpell(pPlayer, HORDE_CONTROL_PHASE_SHIFT_2, false);
                      break;
                case HORDE:
                      if (pPlayer && pPlayer->IsInWorld() && pPlayer->HasAura(ALLIANCE_CONTROL_PHASE_SHIFT_1)) 
                          pPlayer->RemoveAurasDueToSpell(ALLIANCE_CONTROL_PHASE_SHIFT_1);
                      pPlayer->CastSpell(pPlayer, ALLIANCE_CONTROL_PHASE_SHIFT_2, false);
                      break;
            };

        };

        void OnGameObjectCreate(GameObject* go)
        {
            switch(go->GetEntry())
            {
                case GO_IMPENETRABLE_DOOR: m_uiMainGateGUID = go->GetGUID(); break;
                case GO_FROSTMOURNE: m_uiFrostmourneGUID = go->GetGUID(); break;
                case GO_ICECROWN_DOOR:     m_uiExitGateGUID = go->GetGUID(); break;
                case GO_ICECROWN_DOOR_2:   m_uiDoor2GUID = go->GetGUID(); break;
                case GO_ICECROWN_DOOR_3:   m_uiDoor3GUID = go->GetGUID(); break;
                case GO_PORTAL:            m_uiPortalGUID = go->GetGUID(); break;
                case GO_CAPTAIN_CHEST_1:
                                      if (Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                      m_uiCaptainsChestHordeGUID = go->GetGUID(); 
                                      break;
                case GO_CAPTAIN_CHEST_3:
                                      if (Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                      m_uiCaptainsChestHordeGUID = go->GetGUID(); 
                                      break;
                case GO_CAPTAIN_CHEST_2:
                                      if (Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                                      m_uiCaptainsChestAllianceGUID = go->GetGUID(); 
                                      break;
                case GO_CAPTAIN_CHEST_4:
                                      if (Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                                      m_uiCaptainsChestAllianceGUID = go->GetGUID(); 
                                      break;
                case GO_ICE_WALL_1:
                    m_uiIceWall1GUID = go->GetGUID();
                    go->SetGoState(GO_STATE_ACTIVE);
                    break;
                case GO_ICE_WALL_2:
                    m_uiIceWall2GUID = go->GetGUID();
                    go->SetGoState(GO_STATE_ACTIVE);
                    break;
                case GO_ICE_WALL_3:
                    m_uiIceWall3GUID = go->GetGUID();
                    go->SetGoState(GO_STATE_ACTIVE);
                    break;
                case GO_ICE_WALL_4:
                    m_uiIceWall4GUID = go->GetGUID();
                    go->SetGoState(GO_STATE_ACTIVE);
                    break;
                case GO_CAVE:
                    m_uiGoCaveGUID = go->GetGUID();
                    go->SetGoState(GO_STATE_ACTIVE);
                    break;
            }
        }

        void SetData(uint32 uiType, uint32 uiData)
        {
            switch(uiType)
            {
                case TYPE_PHASE:                m_auiEncounter[uiType] = uiData; break;
                case TYPE_EVENT:                m_auiEncounter[uiType] = uiData;
                                                uiData = NOT_STARTED;
                    break;
                case TYPE_FALRIC:               m_auiEncounter[uiType] = uiData;
                    if(uiData == SPECIAL)
                                                    CloseDoor(m_uiExitGateGUID);
                    break;
                case TYPE_MARWYN:               m_auiEncounter[uiType] = uiData;
                    if(uiData == DONE)
                    {
                                                   OpenDoor(m_uiMainGateGUID);
                                                   OpenDoor(m_uiExitGateGUID);
                    }
                    break;
                case TYPE_FROST_GENERAL:        m_auiEncounter[uiType] = uiData; 
                                                if(uiData == DONE)
                                                   OpenDoor(m_uiDoor2GUID);
                    break;
                case TYPE_LICH_KING:            m_auiEncounter[uiType] = uiData;
                                                if(uiData == IN_PROGRESS)
                                                   OpenDoor(m_uiDoor3GUID);
                                                if(uiData == DONE)
                                                {
                                                if (m_auiLider == 1)
                                                {
                                                if (GameObject* pChest = instance->GetGameObject(m_uiCaptainsChestAllianceGUID))
                                                    if (pChest && !pChest->isSpawned()) {
                                                        pChest->SetRespawnTime(DAY);
                                                    };
                                                } else
                                                if (GameObject* pChest = instance->GetGameObject(m_uiCaptainsChestHordeGUID))
                                                    if (pChest && !pChest->isSpawned()) {
                                                        pChest->SetRespawnTime(DAY);
                                                    };
                                                if (GameObject* pPortal = instance->GetGameObject(m_uiPortalGUID))
                                                    if (pPortal && !pPortal->isSpawned()) {
                                                        pPortal->SetRespawnTime(DAY);
                                                    };
                                                }
                    break;
                case TYPE_ICE_WALL_01:          m_auiEncounter[uiType] = uiData; break;
                case TYPE_ICE_WALL_02:          m_auiEncounter[uiType] = uiData; break;
                case TYPE_ICE_WALL_03:          m_auiEncounter[uiType] = uiData; break;
                case TYPE_ICE_WALL_04:          m_auiEncounter[uiType] = uiData; break;
                case TYPE_HALLS:                m_auiEncounter[uiType] = uiData; break;
                case DATA_LIDER:                m_auiLider = uiData;
                                                uiData = NOT_STARTED;
                    break;
                case DATA_SUMMONS:              if (uiData == 3) m_uiSummons = 0;
                                                else if (uiData == 1) ++m_uiSummons;
                                                else if (uiData == 0) --m_uiSummons;
                                                uiData = NOT_STARTED;
                    break;
            }

            if (uiData == DONE)
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;

                for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
                    saveStream << m_auiEncounter[i] << " ";

                strSaveData = saveStream.str();

                SaveToDB();
                OUT_SAVE_INST_DATA_COMPLETE;
            }
        }

        const char* Save()
        {
            return strSaveData.c_str();
        }

        uint32 GetData(uint32 uiType)
        {
            switch(uiType)
            {
                case TYPE_PHASE:                return m_auiEncounter[uiType];
                case TYPE_EVENT:                return m_auiEncounter[uiType];
                case TYPE_FALRIC:               return m_auiEncounter[uiType];
                case TYPE_MARWYN:               return m_auiEncounter[uiType];
                case TYPE_LICH_KING:            return m_auiEncounter[uiType];
                case TYPE_FROST_GENERAL:        return m_auiEncounter[uiType];
                case TYPE_ICE_WALL_01:          return m_auiEncounter[uiType];
                case TYPE_ICE_WALL_02:          return m_auiEncounter[uiType];
                case TYPE_ICE_WALL_03:          return m_auiEncounter[uiType];
                case TYPE_ICE_WALL_04:          return m_auiEncounter[uiType];
                case TYPE_HALLS:                return m_auiEncounter[uiType];
                case DATA_LIDER:                return m_auiLider;
                case DATA_SUMMONS:              return m_uiSummons;
                default:                        return 0;
            }
            return 0;
        }

        void SetData64(uint32 uiData, uint64 uiGuid)
        {
            switch(uiData)
            {
                case DATA_ESCAPE_LIDER:
                       m_uiLiderGUID = uiGuid;
                       break;
            }
        }

        uint64 GetData64(uint32 uiData)
        {
            switch(uiData)
            {
                case GO_IMPENETRABLE_DOOR: return m_uiMainGateGUID;
                case GO_FROSTMOURNE:       return m_uiFrostmourneGUID;
                case NPC_FALRIC:           return m_uiFalricGUID;
                case NPC_MARWYN:           return m_uiMarwynGUID;
                case BOSS_LICH_KING:       return m_uiLichKingGUID;
                case DATA_ESCAPE_LIDER:    return m_uiLiderGUID;
                case NPC_FROST_GENERAL:    return m_uiFrostGeneralGUID;
                case GO_ICECROWN_DOOR:     return m_uiExitGateGUID;
                case GO_ICECROWN_DOOR_2:   return m_uiDoor2GUID;
                case GO_ICECROWN_DOOR_3:   return m_uiDoor3GUID;
                case GO_ICE_WALL_1:        return m_uiIceWall1GUID;
                case GO_ICE_WALL_2:        return m_uiIceWall2GUID;
                case GO_ICE_WALL_3:        return m_uiIceWall3GUID;
                case GO_ICE_WALL_4:        return m_uiIceWall4GUID;
                case GO_CAVE:              return m_uiGoCaveGUID;
            }
            return 0;
        }

        void Load(const char* chrIn)
        {
            if (!chrIn)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(chrIn);

            std::istringstream loadStream(chrIn);

            for(uint8 i = 0; i < MAX_ENCOUNTERS; ++i)
            {
                loadStream >> m_auiEncounter[i];

                if (m_auiEncounter[i] == IN_PROGRESS)
                    m_auiEncounter[i] = NOT_STARTED;
            }

            OUT_LOAD_INST_DATA_COMPLETE;
        }

    };

    InstanceScript* GetInstanceScript (InstanceMap* map) const
    {
        return new instance_halls_of_reflection_InstanceMapScript(map);
    }
};


void AddSC_instance_halls_of_reflection()
{
    new instance_halls_of_reflection();
}
