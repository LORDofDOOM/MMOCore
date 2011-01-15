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

#ifndef TRIAL_OF_THE_CRUSADER_H
#define TRIAL_OF_THE_CRUSADER_H

enum
{
    DATA_BEASTS                     = 0,
    DATA_LORD_JARAXXUS              = 1,
    DATA_FACTION_CHAMPIONS          = 2,
    DATA_TWIN_VALKYR                = 3,
    DATA_ANUBARAK                   = 4,

    MAX_ENCOURTMENT,
    RESET_TO_FIGHT,

    BOSS_GORMOK,
    BOSS_DREADSCALE,
    BOSS_ACIDMAW,
    BOSS_ICEHOWL,
    BOSS_JARAXXUS,

    BOSS_LIGHT_TWIN,
    BOSS_DARK_TWIN,
    BOSS_ANUBARAK,

    BOSS_CHAMPION_DK,
    BOSS_CHAMPION_SOWA,
    BOSS_CHAMPION_DRZEWO,
    BOSS_CHAMPION_HUNTER,
    BOSS_CHAMPION_MAGE,
    BOSS_CHAMPION_HOLYPALA,
    BOSS_CHAMPION_RETRIPALA,
    BOSS_CHAMPION_DISCI,
    BOSS_CHAMPION_SHADOW,
    BOSS_CHAMPION_ROGAL,
    BOSS_CHAMPION_ENH,
    BOSS_CHAMPION_RESTOSH,
    BOSS_CHAMPION_LOCK,
    BOSS_CHAMPION_WAREK,

    TIRION_FORDING,
    GAROSH_HELLSCREAM,
    WARIAN_VRYNN,
    LICH_KING,
    GNOM_WARLOCK,
    GADAJACY,

    DATA_TEAM,


    FLOOR,
    MAIN_GATE,
    CHAMPIONS_CACHE,

    ///*=============== DATAS FOR ACHIEVEMENTS
    TIME_ACIDMAW_DEAD,
    TIME_DREADSCALE_DEAD,
    DATA_ATTEMPTS,
    DATA_DEATHS_COUNT,
    UPDATE_DEATHS_COUNT,
    SCARAB_DEATH,

    ///*=============== ENTRY

    NPC_GORMOK                      = 34796,
    NPC_ACIDMAW                     = 35144,
    NPC_DREADSCALE                  = 34799,
    NPC_ICEHOWL                     = 34797,

    NPC_LORD_JARAXXUS               = 34780,

    NPC_DK_HORDE                    = 34458,
    NPC_SOWA_HORDE                  = 34451,
    NPC_DRZEWO_HORDE                = 34459,
    NPC_HUNTER_HORDE                = 34448,
    NPC_MAGE_HORDE                  = 34449,
    NPC_HOLYPALA_HORDE              = 34445,
    NPC_RETRIPALA_HORDE             = 34456,
    NPC_DISCI_HORDE                 = 34447,
    NPC_SHADOW_HORDE                = 34441,
    NPC_ROGAL_HORDE                 = 34454,
    NPC_ENH_HORDE                   = 34455,
    NPC_RESTOSH_HORDE               = 34444,
    NPC_LOCK_HORDE                  = 34450,
    NPC_WAREK_HORDE                 = 34453,

    NPC_DK_ALLY                     = 34461,
    NPC_SOWA_ALLY                   = 34460,
    NPC_DRZEWO_ALLY                 = 34469,
    NPC_HUNTER_ALLY                 = 34467,
    NPC_MAGE_ALLY                   = 34468,
    NPC_HOLYPALA_ALLY               = 34465,
    NPC_RETRIPALA_ALLY              = 34471,
    NPC_DISCI_ALLY                  = 34466,
    NPC_SHADOW_ALLY                 = 34473,
    NPC_ROGAL_ALLY                  = 34472,
    NPC_ENH_ALLY                    = 34463,
    NPC_RESTOSH_ALLY                = 34470,
    NPC_LOCK_ALLY                   = 34474,
    NPC_WAREK_ALLY                  = 34475,

    NPC_DARK_TWIN                   = 34496,
    NPC_LIGHT_TWIN                  = 34497,

    NPC_ANUBARAK                    = 34564,
    NPC_SCARAB                      = 34605,
    NPC_BURROWER                    = 34607,

    NPC_TIRION_FORDING              = 34996,
    NPC_WARIAN_VRYNN                = 34990,
    NPC_GAROSH_HELLSCREAM           = 34995,
    NPC_GNOM_WARLOCK                = 35458,
    NPC_LICH_KING                   = 35877,

    NPC_GADAJACY                    = 34816,
    NPC_SNOBOLD                     = 34800,
    NPC_MISSTRESS_OF_PAIN           = 34826,



    GO_MAIN_GATE                    = 195647,
    GO_FLOOR                        = 195527,
    GO_DOOR_N                       = 195650,
    GO_DOOR_S                       = 195649,
    GO_DOOR_E                       = 195648,
    GO_CACHE                        = 195631
};


#endif // #ifndef TRIAL_OF_THE_CRUSADER_H