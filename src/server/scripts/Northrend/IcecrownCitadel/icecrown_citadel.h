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

#ifndef DEF_ICECROWN_CITADEL_H
#define DEF_ICECROWN_CITADEL_H

#define ICCScriptName "instance_icecrown_citadel"

enum eSharedSpells
{
    SPELL_BERSERK   = 26662,
    SPELL_BERSERK2  = 47008
}; 

enum eEnums
{
    SOUND_ENDING_7_KING                   = 17360,
    MOVIE_ID_ARTHAS_DEATH                 = 16
};

enum eAchievements
{
    //ACHIEV_BONED_10                             = 4534,
    //ACHIEV_BONED_25                             = 4610,
    //AVHIEV_FULL_HOUSE_10                        = 4535,
    //AVHIEV_FULL_HOUSE_25                        = 4611,
    ACHIEV_MESS_10                              = 4537,
    ACHIEV_MESS_25                              = 4613,
    //ACHIEVMENT_ONCE_BITTEN_TWICE_SHY_10         = 4539,
    //ACHIEVMENT_ONCE_BITTEN_TWICE_SHY_25         = 4618,
    ACHIEV_ALL_YOU_CAN_EAT_10                   = 4580,
    ACHIEV_ALL_YOU_CAN_EAT_25                   = 4620,
    ACHIEV_BEEN_WAITING_A_LONG_TIME_FOR_THIS_10 = 4601,
    ACHIEV_BEEN_WAITING_A_LONG_TIME_FOR_THIS_25 = 4621,
    ACHIEV_NECK_DEEP_IN_VILE_10                 = 4581,
    ACHIEV_NECK_DEEP_IN_VILE_25                 = 4622,
    ACHIEV_PORTAL_JOCKEY_10                     = 4579,
    ACHIEV_PORTAL_JOCKEY_25                     = 4619
};

enum eAchievementCriteria
{
    // Lord Marrowgar
    CRITERIA_BONED_10N                  = 12775,
    CRITERIA_BONED_25N                  = 12962,
    CRITERIA_BONED_10H                  = 13393,
    CRITERIA_BONED_25H                  = 13394,

    // Rotface
    CRITERIA_DANCES_WITH_OOZES_10N      = 12984,
    CRITERIA_DANCES_WITH_OOZES_25N      = 12966,
    CRITERIA_DANCES_WITH_OOZES_10H      = 12985,
    CRITERIA_DANCES_WITH_OOZES_25H      = 12983,

    // Professor Putricide
    CRITERIA_NAUSEA_10N                 = 12987,
    CRITERIA_NAUSEA_25N                 = 12968,
    CRITERIA_NAUSEA_10H                 = 12988,
    CRITERIA_NAUSEA_25H                 = 12981,

    // Blood Prince Council
    CRITERIA_ORB_WHISPERER_10N          = 13033,
    CRITERIA_ORB_WHISPERER_25N          = 12969,
    CRITERIA_ORB_WHISPERER_10H          = 13034,
    CRITERIA_ORB_WHISPERER_25H          = 13032,

    // Blood-Queen Lana'thel
    CRITERIA_KILL_LANA_THEL_10M         = 13340,
    CRITERIA_KILL_LANA_THEL_25M         = 13360,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_10N  = 12780,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_25N  = 13012,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_10V  = 13011,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_25V  = 13013,
};

enum ePutricideActions
{
    // Festergut
    ACTION_FESTERGUT_COMBAT     = -366260,
    ACTION_FESTERGUT_GAS        = -366261,
    ACTION_FESTERGUT_DEATH      = -366262,

    // Rotface
    ACTION_ROTFACE_COMBAT       = -366270,
    ACTION_ROTFACE_OOZE         = -366271,
    ACTION_ROTFACE_DEATH        = -366272,
    ACTION_CHANGE_PHASE         = -366780
};

enum Data
{
    DATA_MARROWGAR_EVENT,
    DATA_DEATHWHISPER_EVENT,
    DATA_GUNSHIP_BATTLE_EVENT,
    DATA_SAURFANG_EVENT,
    DATA_FESTERGUT_EVENT,
    DATA_ROTFACE_EVENT,
    DATA_PROFESSOR_PUTRICIDE_EVENT,
    DATA_BLOOD_PRINCE_COUNCIL_EVENT,
    DATA_BLOOD_QUEEN_LANA_THEL_EVENT,
    DATA_VALITHRIA_DREAMWALKER_EVENT,
    DATA_SINDRAGOSA_EVENT,
    DATA_LICH_KING_EVENT,
    DATA_SPAWN
};

enum Data64
{
    // Encounter States/Boss GUIDs
    DATA_LORD_MARROWGAR             = 0,
    DATA_LADY_DEATHWHISPER          = 1,
    DATA_SAURFANG,
    DATA_GUNSHIP_BATTLE,
    DATA_FESTERGUT,
    DATA_ROTFACE,
    DATA_PROFESSOR_PUTRICIDE,
    DATA_PRINCE_VALANAR_ICC,
    DATA_PRINCE_KELESETH_ICC,
    DATA_PRINCE_TALDARAM_ICC,
    DATA_BLOOD_QUEEN_LANA_THEL,
    DATA_VALITHRIA_DREAMWALKER,
    DATA_VALITHRIA_ALTERNATIVE,
    DATA_VALITHRIA_COMBAT_TRIGGER,
    DATA_SINDRAGOSA,
    DATA_LICH_KING,

	// Additional data
    DATA_TIRION,
    DATA_ANGLE,
    DATA_ALL_YOU_CAN_EAT,
    DATA_BEEN_WAITING,
    DATA_NECK_DEEP,
    DATA_BLOOD_PRINCES_CONTROL,
    DATA_NECROTIC_STACK,
    DATA_PUTRICIDE_TABLE,
	//Achievements
    DATA_BONED_ACHIEVEMENT,
	DATA_OOZE_DANCE_ACHIEVEMENT,
	DATA_NAUSEA_ACHIEVEMENT,
	DATA_ORB_WHISPERER_ACHIEVEMENT,
    DATA_PORTAL_JOCKEY_ACHIEVEMENT
};

enum eCreatures
{
    // Lord Marrowgar
    NPC_LORD_MARROWGAR                          = 36612,
    NPC_COLDFLAME                               = 36672,
    NPC_BONE_SPIKE                              = 36619,

    // Lady Deathwhisper
    NPC_LADY_DEATHWHISPER                       = 36855,
    NPC_CULT_FANATIC                            = 37890,
    NPC_DEFORMED_FANATIC                        = 38135,
    NPC_REANIMATED_FANATIC                      = 38009,
    NPC_CULT_ADHERENT                           = 37949,
    NPC_EMPOWERED_ADHERENT                      = 38136,
    NPC_REANIMATED_ADHERENT                     = 38010,
    NPC_VENGEFUL_SHADE                          = 38222,

    CREATURE_OOZE_COVERED_TENTACLE_STALKER = 38308, 
    CREATURE_SLIMY_TENTACLE_STALKER  = 38309,
    CREATURE_GUNSHIP                 = 30343,
    CREATURE_KOR_KRON_GENERAL        = 37189,
    CREATURE_ALLIANCE_COMMANDER      = 37190,
    CREATURE_TORTUNOK                = 37992,
    CREATURE_ALANA_MOONSTRIKE        = 37999,
    CREATURE_GERARDO_THE_SUAVE       = 37993,
    CREATURE_TALAN_MOONSTRIKE        = 37998,
    CREATURE_UVLUS_BANEFIRE          = 38284,
    CREATURE_MALFUS_GRIMFROST        = 38283,
    CREATURE_IKFIRUS_THE_VILE        = 37991,
    CREATURE_YILI                    = 37997,
    CREATURE_VOL_GUK                 = 38841,
    CREATURE_JEDEBIA                 = 38840,
    CREATURE_HARAGG_THE_UNSEEN       = 38181,
    CREATURE_SE_MURADIN_BRONZEBEARD  = 37200,
    CREATURE_SE_KING_VARIAN_WRYNN    = 37879,
    CREATURE_SE_HIGH_OVERLORD_SAURFANG    = 37187,
    CREATURE_SE_KOR_KRON_REAVER      = 37920,
    CREATURE_SE_SKYBREAKER_MARINE    = 37830,
    CREATURE_NIBY_THE_ALMIGHTY       = 38182,
    CREATURE_GARROSH_HELLSCREAM      = 39372,
    CREATURE_KING_VARIAN_WRYNN       = 39371,
    CREATURE_SAURFANG                = 37813,
    CREATURE_FESTERGUT              = 36626,
    CREATURE_PUDDLE_STALKER          = 37013,
    CREATURE_ORANGE_GAS_STALKER      = 36659,
    CREATURE_VILE_GAS_STALKER        = 38548,
    CREATURE_ROTFACE                 = 36627,
    CREATURE_OOZE_SPRAY_STALKER      = 37986,
    CREATURE_GREEN_GAS_STALKER       = 37824,
    CREATURE_LITTLE_OOZE             = 36897,
    CREATURE_BIG_OOZE                = 36899,
    CREATURE_STINKY                  = 37025,
    CREATURE_PRECIOUS                = 37217,
    CREATURE_PROFESSOR_PUTRICIDE     = 36678,
    CREATURE_VOLATILE_OOZE           = 37697,
    CREATURE_GAS_CLOUD               = 37562,
    CREATURE_GROWING_OZZE_STALKER    = 38234,
    CREATURE_TEAR_GAS_STALKER        = 38317,
    CREATURE_CHOKING_GAS_BOMB        = 38159,
    NPC_PROFESSOR_PUTRICIDE                     = 36678,
    NPC_ABOMINATION_WING_MAD_SCIENTIST_STALKER  = 37824,
    NPC_GROWING_OOZE_PUDDLE                     = 37690,
    NPC_GAS_CLOUD                               = 37562,
    NPC_VOLATILE_OOZE                           = 37697,
    NPC_CHOKING_GAS_BOMB                        = 38159,
    NPC_TEAR_GAS_TARGET_STALKER                 = 38317,
    CREATURE_PRINCE_VALANAR_ICC      = 37970,
    CREATURE_PRINCE_KELESETH_ICC     = 37972,
    CREATURE_PRINCE_TALDARAM_ICC     = 37973,
    CREATURE_BLOOD_ORB_CONTROLLER    = 38008,
    CREATURE_FLOATING_TRIGGER        = 30298,
    CREATURE_DARK_NUCLEUS            = 38369,
    CREATURE_BALL_OF_FLAME           = 38332,
    CREATURE_BALL_OF_INFERNO_FLAME   = 38451,
    CREATURE_KINETIC_BOMB_TARGET     = 38458,
    CREATURE_KINETIC_BOMB            = 38454,
    CREATURE_SHOCK_VORTEX            = 38422,
    CREATURE_BLOOD_QUEEN_LANA_THEL   = 37955,
    CREATURE_SWARMING_SHADOWS        = 38163,
    CREATURE_VALITHRIA_DREAMWALKER   = 36789,
    CREATURE_VALITHRIA_ALTERNATIVE   = 37950,
    CREATURE_PORTAL_NORMAL_MODE_PRE  = 38186,
    CREATURE_PORTAL_NORMAL_MODE_NPC  = 37945,
    CREATURE_PORTAL_HEROIC_MODE_PRE  = 38429,
    CREATURE_PORTAL_HEROIC_MODE_NPC  = 38430,
    CREATURE_ROT_WORM                = 37907,
    CREATURE_PORTAL                  = 37945,
    CREATURE_DREAM_CLOUD             = 37985,
    CREATURE_NIGHTMARE_CLOUD         = 38421,
    CREATURE_MANA_VOID               = 38068,
    CREATURE_ABOMINATION             = 37886,
    CREATURE_SKELETON                = 36791,
    CREATURE_ARCHMAGE                = 37868,
    CREATURE_SUPPRESSOR              = 37863,
    CREATURE_ZOMBIE                  = 37934,
    CREATURE_COLUMN_OF_FROST         = 37918,
    CREATURE_COMBAT_TRIGGER          = 38752,
    CREATURE_SINDRAGOSA              = 36853,
    CREATURE_ICE_TOMB                = 36980,
    CREATURE_RIMEFANG                = 37533,
    CREATURE_SPINESTALKER            = 37534,
    CREATURE_LICH_KING               = 36597,
    CREATURE_TIRION_ICC              = 38995,
    CREAUTRE_MENETHIL                = 38579,
    CREATURE_FROSTMOURNE_TRIGGER     = 38584,
    CREATURE_TERENAS_F               = 36823, //for frostmourne
    CREATURE_SPIRIT_WARDEN_F         = 36824,
    CREATURE_ICE_SPHERE              = 36633,
    CREATURE_DRUDGE_GHOUL            = 37695,
    CREATURE_SHAMBLING_HORROR        = 37698,
    CREATURE_VALKYR                  = 36609,
    CREATURE_DEFILE                  = 38757,
    CREATURE_RAGING_SPIRIT           = 36701,
    CREATURE_TRIGGER                 = 38667,
    CREATURE_VILE_SPIRIT             = 37799
};

enum eGameobjects
{
    LORD_ICE_WALL_1                = 201910,
    LORD_ICE_WALL_2                = 201911,
    LORD_ENTRANCE                  = 201857,
    ORATORY_ENTRANCE               = 201563,
    LADY_ELEVATOR                  = 202220,
    ORGRIM_S_HAMMER_HORDE_ICC      = 201812,
    ORGRIM_S_HAMMER_ALLIANCE_ICC   = 201581,
    THE_SKYBREAKER_HORDE_ICC       = 201811,
    THE_SKYBREAKER_ALLIANCE_ICC    = 201580,
    SAURFANG_DOOR                  = 201825,
    BLOODWING_DOOR                 = 201920,
    FROSTWING_DOOR                 = 201919,
    CRIMSONHALL_DOOR               = 201376,
    CRIMSONHALL_DOOR_1             = 201377,
    CRIMSONHALL_DOOR_2             = 201378,
    BLOODQUEEN_ELEVATOR            = 201755,
    DRAGON_DOOR_1                  = 201375,
    DRAGON_DOOR_2                  = 201374,
    DRAGON_DOOR_3                  = 201379,
    DREAMWALKER_DOOR_1             = 201380,
    DREAMWALKER_DOOR_2             = 201381,
    DREAMWALKER_DOOR_3             = 201382,
    DREAMWALKER_DOOR_4             = 201383,
    VALITHRIA_ELEVATOR             = 202234,
    SINDRAGOSSA_ELEVATOR           = 196840,
    SINDRAGOSSA_DOOR_1             = 201373,
    SINDRAGOSSA_DOOR_2             = 201369,
    PROF_COLLISION_DOOR            = 201612,
    GREEN_PIPE                     = 201618,
    ORANGE_PIPE                    = 201617,
    OOZE_VALVE                     = 201615,
    GAS_VALVE                      = 201616,
    PROF_DOOR_GREEN                = 201614,
    PROF_DOOR_ORANGE               = 201613,
    ROTFACE_DOOR                   = 201370,
    FESTERGUT_DOOR                 = 201371,
    PROFESSOR_DOOR                 = 201372,
    DRINK_ME                       = 201584,
    SAURFANG_CACHE_10_N            = 202238,
    SAURFANG_CACHE_25_N            = 202239,
    SAURFANG_CACHE_10_H            = 202240,
    SAURFANG_CACHE_25_H            = 202241,
    DREAMWALKER_CACHE_10_N         = 201959,
    DREAMWALKER_CACHE_25_N         = 202338,
    DREAMWALKER_CACHE_10_H         = 202339,
    DREAMWALKER_CACHE_25_H         = 202340,
    ARTHAS_PLATFORM                = 202161,
    FIRST_TELEPORT                 = 202242,
    LORD_TELEPORT                  = 202243,
    GUNSHIP_TELEPORT               = 202244,
    SAURFANG_TELEPORT              = 202245,
    CITADEL_TELEPORT               = 202235,
    SINDRAGOSSA_TELEPORT           = 202246,
    LICH_TELEPORT                  = 202223
};

enum eGameobjects_TrinityCore
{
    // Lord Marrogar
    GO_DOODAD_ICECROWN_ICEWALL02            = 201910,
    GO_ICEWALL                              = 201911,
    GO_LORD_MARROWGAR_S_ENTRANCE            = 201857,

    // Lady Deathwhisper
    GO_ORATORY_OF_THE_DAMNED_ENTRANCE       = 201563,
    GO_LADY_DEATHWHISPER_ELEVATOR           = 202220,

    // Deathbringer Saurfang
    GO_SAURFANG_S_DOOR                      = 201825,
    GO_DEATHBRINGER_S_CACHE_10N             = 202239,
    GO_DEATHBRINGER_S_CACHE_25N             = 202240,
    GO_DEATHBRINGER_S_CACHE_10H             = 202238,
    GO_DEATHBRINGER_S_CACHE_25H             = 202241,
    GO_SCOURGE_TRANSPORTER_SAURFANG         = 202244,

    // Professor Putricide
    GO_ORANGE_PLAGUE_MONSTER_ENTRANCE       = 201371,
    GO_GREEN_PLAGUE_MONSTER_ENTRANCE        = 201370,
    GO_SCIENTIST_AIRLOCK_DOOR_COLLISION     = 201612,
    GO_SCIENTIST_AIRLOCK_DOOR_ORANGE        = 201613,
    GO_SCIENTIST_AIRLOCK_DOOR_GREEN         = 201614,
    GO_DOODAD_ICECROWN_ORANGETUBES02        = 201617,
    GO_DOODAD_ICECROWN_GREENTUBES02         = 201618,
    GO_SCIENTIST_ENTRANCE                   = 201372,
    GO_DRINK_ME                             = 201584,

    // Blood Prince Council
    GO_CRIMSON_HALL_DOOR                    = 201376,
    GO_BLOOD_ELF_COUNCIL_DOOR               = 201378,
    GO_BLOOD_ELF_COUNCIL_DOOR_RIGHT         = 201377,

    // Blood-Queen Lana'thel
    GO_DOODAD_ICECROWN_BLOODPRINCE_DOOR_01  = 201746,
    GO_DOODAD_ICECROWN_GRATE_01             = 201755,

    // Valithria Dreamwalker
    GO_GREEN_DRAGON_BOSS_ENTRANCE           = 201375,
    GO_GREEN_DRAGON_BOSS_EXIT               = 201374,

    // Sindragosa
    GO_SINDRAGOSA_ENTRANCE_DOOR             = 201373,
    GO_SINDRAGOSA_SHORTCUT_ENTRANCE_DOOR    = 201369,
    GO_SINDRAGOSA_SHORTCUT_EXIT_DOOR        = 201379
};
void DespawnAllCreaturesAround(Creature *ref, uint32 entry);
void UnsummonSpecificCreaturesNearby(Creature *ref, uint32 entry, float radius);
#endif
