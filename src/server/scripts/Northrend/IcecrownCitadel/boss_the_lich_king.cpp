/*
* Copyright (C) 2009 - 2010 TrinityCore <http://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "icecrown_citadel.h"

#define GOSSIP_MENU 10600
//#define GOSSIP_MENU "Long have I waited for this day, hero. Are you and your allies prepared to bring the Lich King to justice? We charge on your command!"
#define GOSSIP_START_EVENT "We are prepared, Highlord. Let us battle for the fate of Azeroth! For the light of dawn!"

enum Yells
{
    SAY_INTRO_1_KING         = -1810001,
    SAY_INTRO_2_TIRION       = -1810002,
    SAY_INTRO_3_KING         = -1810003,
    SAY_INTRO_4_TIRION       = -1810004,
    SAY_INTRO_5_KING         = -1810005,
    SAY_AGGRO                = -1810006,
    SAY_REMORSELESS_WINTER   = -1810007,
    SAY_RANDOM_1             = -1810008,
    SAY_RANDOM_2             = -1810009,
    SAY_KILL_1               = -1810010,
    SAY_KILL_2               = -1810011,
    SAY_BERSERK              = -1810012,
    SAY_ENDING_1_KING        = -1810013,
    SAY_ENDING_2_KING        = -1810014,
    SAY_ENDING_3_KING        = -1810015,
    SAY_ENDING_4_KING        = -1810016,
    SAY_ENDING_5_TIRION      = -1810017,
    SAY_ENDING_6_KING        = -1810018,
    SAY_ENDING_8_TIRION      = -1810020,
    SAY_ENDING_9_FATHER      = -1810021,
    SAY_ENDING_10_TIRION     = -1810022,
    SAY_ENDING_11_FATHER     = -1810023,
    SAY_ENDING_12_KING       = -1810024,
    SAY_DEATH_KING           = -1810025,
    SAY_ESCAPE_FROSTMOURNE   = -1810026,
    SAY_HARVEST_SOUL         = -1810027,
    SAY_DEVOURED_FROSTMOURNE = -1810028,
    SAY_SUMMON_VALKYR        = -1810029,
    SAY_BROKEN_ARENA         = -1810030,
    SAY_10_PROZENT           = -1810031,
    SAY_EMOTE_DEFILE         = -1810032,
};

enum Spells
{
    SPELL_SUMMON_SHAMBLING_HORROR    = 70372,
    SPELL_SUMMON_DRUDGE_GHOULS       = 70358,
    SPELL_SUMMON_ICE_SPEHERE         = 69104,
    SPELL_SUMMON_RAGING_SPIRIT       = 69200,
    SPELL_SUMMON_VALKYR              = 74361,
    SPELL_SUMMON_DEFILE              = 72762,
    SPELL_SUMMON_VILE_SPIRIT         = 70498, //????????
    SPELL_SUMMON_BROKEN_FROSTMOURNE  = 72406,
    SPELL_SUMMON_SHADOW_TRAP         = 73539,
    SPELL_INFEST                     = 70541,
    SPELL_NECROTIC_PLAGUE            = 70337, //70337 - initial cast
    SPELL_NECROTIC_PLAGUE_IMMUNITY   = 72846,
    SPELL_PLAGUE_SIPHON              = 74074,
    SPELL_REMORSELES_WINTER          = 68981,
    SPELL_REMORSELES_WINTER_DAMAGE   = 68983,
    SPELL_PAIN_AND_SUFFERING         = 74115,
    //SPELL_RANDOM_TALK                = 73985,
    SPELL_WINGS_OF_THE_DAMNED        = 74352,
    SPELL_SOUL_REAPER                = 69409,
    SPELL_SOUL_REAPER_HASTE_AURA     = 69410,
    SPELL_HARVEST_SOUL_TELEPORT      = 71372, //72597
    SPELL_HARVEST_SOULS              = 68980,
    SPELL_QUAKE                      = 72262,
    SPELL_CHANNEL_KING               = 71769,
    SPELL_BROKEN_FROSTMOURNE         = 72398,
    SPELL_BOOM_VISUAL                = 72726,
    SPELL_ICEBLOCK_TRIGGER           = 71614,
    SPELL_TIRION_LIGHT               = 71797,
    SPELL_FROSTMOURNE_TRIGGER        = 72405,
    SPELL_DISENGAGE                  = 61508,
    SPELL_FURY_OF_FROSTMOURNE        = 72350,
    SPELL_REVIVE                     = 72429,
    SPELL_REVIVE_EFFECT              = 72423,
    SPELL_CLONE_PLAYER               = 57507,
    SPELL_DEFILE                     = 72743,
    SPELL_ICE_PULSE                  = 69091,
    SPELL_ICE_BURST                  = 69108,
    SPELL_LIFE_SIPHON                = 73783,
    SPELL_SOUL_SHRIEK                = 69242,
    SPELL_WHOCKVAWE                  = 72149,
    SPELL_ENRAGE                     = 72143,
    SPELL_BURST                      = 70503,
    SPELL_VILE_SPIRIT_DISTANCE_CHECK = 70502,
    SPELL_ICE_BURST_DISTANCE_CHECK   = 69109,
    SPELL_VILE_SPIRIT_ACTIVE         = 72130,
    SPELL_RAGING_VISUAL              = 69198,
    SPELL_REMOVE_WEAPON              = 72399,
    SPELL_DROP_FROSTMOURNE           = 73017,
    SPELL_SUMMON_FROSTMOURNE_TRIGGER = 72407,
    SPELL_WMO_INTACT                 = 50176,
    SPELL_WMO_DAMAGE                 = 50177,
    SPELL_PLAY_MOVIE                 = 73159,
    SPELL_SUMMON_MENETHIL            = 72420, //caster - 38584
    SPELL_MENETHIL_VISUAL            = 72372,
    SPELL_VALKYR_CARRY_CAN_CAST      = 74506,
    SPELL_VALKYR_MOVE_PLAYER         = 68985, //74445
    SPELL_VALKYR_TARGET_SEARCH       = 69030,
    SPELL_VALKYR_CHARGE              = 74399,
    SPELL_VALYR_EJECT_PASSANGER      = 68576,
    SPELL_LIGH_EFFECT                = 71773,
    SPELL_EMOTE_SHOUT                = 73213,
    SPELL_RAGING_GHOUL_VISUAL        = 69636,
    SPELL_RISEN_WITCH_DOCTOR_SPAWN   = 69639,
    SPELL_ICE_SPHERE_VISUAL          = 69090,
    SPELL_TIRION_JUMP                = 71809,
    SPELL_LICH_KING_STOP_CAST_RES    = 72431,
    SPELL_FROSTMOURNE_DESPAWN        = 72726,
    SPELL_SUMMON_FROSTMOURNE         = 74081,
    SPELL_SOUL_EFFECT                = 72305,

    //SPELL_70501 //Vile Spirit Move Target Search
};

enum eActions
{
    ACTION_PHASESWITCH_1        = 1, //phase 1 and 3
    ACTION_PHASESWITCH_2        = 2, //phase 2 and 4
    ACTION_START_EVENT          = 3
};

enum ePoints
{
    POINT_START_EVENT_1              = 3659700,
    POINT_PLATFORM_CENTRE            = 3659701,
    POINT_PLATFORM_END               = 3659702,
    POINT_VALKYR_END                 = 3659703,
    POINT_VALKYR_ZET                 = 3659704
};

struct Position StartEvent[]=
{
    {465.0731f, -2123.473f, 1040.8569f},
    {462.8351f, -2123.673f, 1040.9082f},
    {461.5851f, -2123.673f, 1041.4082f},
    {445.5851f, -2123.673f, 1056.1582f},
    {436.0851f, -2123.673f, 1064.6582f}
};

struct Position MovePos[]=
{
    {461.792f, -2125.85f, 1040.860f, 0.0f}, // move
    {503.156f, -2124.51f, 1040.860f, 0.0f}, // move center X: 505.2118 Y: -2124.353 Z: 840.9403
    {490.110f, -2124.98f, 1040.860f, 0.0f}, // move tirion frostmourne
    {467.069f, -2123.58f, 1040.857f, 0.0f}, // move tirion attack
    {498.004f, 2201.57f, 1046.093f, 0.0f},  // move valkyr
    {489.297f, -2124.84f, 1040.857f, 0.0f}, //start event tirion move 1
    {503.682f, -2126.63f, 1040.940f, 0.0f}, //лич бежит после вайпа
    {508.989f, -2124.55f, 1045.356f, 0.0f} //лич пархает над мечем
};

/*struct Locations
{
    float x,y,z;
};

static Locations TeleportPoint[]=
{
    {959.996f, 212.576f, 193.843f},
    {932.537f, 231.813f, 193.838f},
    {958.675f, 254.767f, 193.822f},
    {946.955f, 201.316f, 192.535f},
    {944.294f, 149.676f, 197.551f},
    {930.548f, 284.888f, 193.367f},
    {965.997f, 278.398f, 195.777f},
};*/

class boss_the_lich_king : public CreatureScript
{
    public:
        boss_the_lich_king() : CreatureScript("boss_the_lich_king") { }

        struct boss_the_lich_kingAI : public BossAI
        {
            boss_the_lich_kingAI(Creature* creature) : BossAI(creature, DATA_LICH_KING), summons(me)
            {
                instance = me->GetInstanceScript();
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                uiPhase = 1;
                uiRandomSpeechTimer = 33000;
                uiBerserkTimer = 900000;
                uiSummonShamblingHorrorTimer = 20000;
                uiSummonDrudgeGhoulsTimer = 30000;
                uiInfestTimer = 30000;
                uiNecroticPlagueTimer = 30000;
                uiIcePulsSummonTimer = 10000;
                uiSummonSpiritTimer = 25000;
                uiSummonValkyrTimer = 5000;
                uiSoulReaperTimer = 30000;
                uiDefileTimer = 25000;
                uiInfestTimer = 40000;
                uiSummonVileSpiritTimer = 30000;
                uiHarvestSoulTimer = 70000;
                uiSummonShadowTrap = 20000;
                uiEndingTimer = 1000;
                uiStage = 1;
                uiTirionGUID = 0;

                if(!instance)
                    return;

                instance->SetData(DATA_LICH_KING_EVENT, NOT_STARTED);
                instance->SetData(DATA_BEEN_WAITING, NOT_STARTED);
                instance->SetData(DATA_NECK_DEEP, DONE);

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();

                if(SpellEntry* spellRevive = GET_SPELL(SPELL_SUMMON_DEFILE))
                    spellRevive->DurationIndex = 3;

                if(SpellEntry* lock = GET_SPELL(SPELL_ICEBLOCK_TRIGGER))
                    lock->Targets = 6; //target chain damage

                if(SpellEntry* reaper = GET_SPELL(SPELL_SOUL_REAPER_HASTE_AURA))
                    reaper->Targets = 1;

                if(SpellEntry* plague = GET_SPELL(SPELL_PLAGUE_SIPHON)) //hack
                    plague->Targets = 18;

                if(SpellEntry* raging = GET_SPELL(SPELL_SUMMON_RAGING_SPIRIT))
                {
                    raging->DurationIndex = 28;
                    raging->Effect[0] = 6;
                }
            }

            void EnterCombat(Unit* /*pWho*/)
            {
                DoScriptText(SAY_AGGRO, me);
                DoCast(me, SPELL_NECROTIC_PLAGUE_IMMUNITY);
                if(instance)
                    uiTirionGUID = instance->GetData64(DATA_TIRION);

                if(instance)
                    instance->SetData(DATA_LICH_KING_EVENT, IN_PROGRESS);
            }

            void JustDied(Unit* /*pKiller*/)
            {
                if(!instance)
                    return;

                DoScriptText(SAY_DEATH_KING, me);
                DoCast(SPELL_PLAY_MOVIE);
                instance->SetData(DATA_LICH_KING_EVENT, DONE);
                if(instance->GetData(DATA_BEEN_WAITING) == DONE)
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_BEEN_WAITING_A_LONG_TIME_FOR_THIS_10,ACHIEV_BEEN_WAITING_A_LONG_TIME_FOR_THIS_25));
                if(instance->GetData(DATA_NECK_DEEP) == FAIL)
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_NECK_DEEP_IN_VILE_10,ACHIEV_NECK_DEEP_IN_VILE_25));

                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFEST);

                summons.DespawnAll();
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if(type != POINT_MOTION_TYPE)
                    return;

                switch(id)
                {
                    case POINT_PLATFORM_CENTRE:
                        DoScriptText(SAY_REMORSELESS_WINTER, me);
                        DoCast(me, SPELL_REMORSELES_WINTER);
                        DoCast(me, SPELL_PAIN_AND_SUFFERING);
                        DoCast(me, SPELL_WMO_INTACT);
                        me->GetMotionMaster()->MovementExpired();
                        break;
                }
            }

            void JustReachedHome()
            {
                if(!instance)
                    return;

                instance->SetData(DATA_LICH_KING_EVENT, FAIL);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFEST);

                if(uiTirionGUID)
                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                        tirion->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                summons.DespawnAll();
            }

            void KilledUnit(Unit* victim)
            {
                if (uiPhase != 6 && victim->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
            }

            void JustSummoned(Creature* summoned)
            {
                summons.Summon(summoned);
                switch(summoned->GetEntry())
                {
                    case CREATURE_ICE_SPHERE:
                        summoned->CastSpell(summoned, SPELL_ICE_BURST_DISTANCE_CHECK, true);
                        summoned->CastSpell(summoned, SPELL_ICE_PULSE, true);
                        summoned->CastSpell(summoned, SPELL_ICE_SPHERE_VISUAL, true);
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                        {
                            summoned->AI()->AttackStart(target);
                            summoned->GetMotionMaster()->MoveChase(target);
                        }
                        break;
                    case CREATURE_DEFILE:
                        summoned->CastSpell(summoned, SPELL_DEFILE, true);
                        summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case CREATURE_RAGING_SPIRIT:
                        summoned->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                        summoned->getVictim()->CastSpell(summoned, SPELL_RAGING_VISUAL, true);
                        summoned->CastSpell(summoned, SPELL_NECROTIC_PLAGUE_IMMUNITY, true);
                        break;
                    case CREATURE_VILE_SPIRIT:
                        summoned->CastSpell(summoned, SPELL_VILE_SPIRIT_DISTANCE_CHECK, true);
                        break;
                    case CREATURE_TRIGGER:
                        summoned->AI()->AttackStart(me);
                        summoned->SetVisible(false);
                        summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case CREATURE_FROSTMOURNE_TRIGGER:
                        summoned->CastSpell(summoned, SPELL_BROKEN_FROSTMOURNE, false);
                        summoned->CastSpell(summoned, SPELL_FROSTMOURNE_TRIGGER, false);
                        summoned->SetVisible(false);
                        break;
                    case CREAUTRE_MENETHIL:
                        DoScriptText(SAY_ENDING_9_FATHER, summoned);
                        summoned->CastSpell(summoned, SPELL_REVIVE, true);
                        summoned->CastSpell(summoned, SPELL_MENETHIL_VISUAL, true);
                        break;
                    case CREATURE_VALKYR:
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            summoned->AI()->AttackStart(target);
                        summoned->CastSpell(summoned, SPELL_WINGS_OF_THE_DAMNED, true);
                        summoned->CastSpell(summoned, SPELL_VALKYR_CARRY_CAN_CAST, true);
                        break;
                    case CREATURE_DRUDGE_GHOUL:
                        summoned->CastSpell(summoned, SPELL_RAGING_GHOUL_VISUAL, true);
                        break;
                    case CREATURE_SHAMBLING_HORROR:
                        summoned->CastSpell(summoned, SPELL_RISEN_WITCH_DOCTOR_SPAWN, true);
                        break;
                }
            }

            void DoAction(const int32 action)
            {
                switch(action)
                {
                    case ACTION_PHASESWITCH_1:
                        me->SetReactState(REACT_PASSIVE);
                        me->AttackStop();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        SetCombatMovement(false);
                        me->SetInCombatWithZone();
                        me->GetMotionMaster()->MovePoint(POINT_PLATFORM_CENTRE, MovePos[1]);
                        break;
                    case ACTION_PHASESWITCH_2:
                        uiPhase = uiPhase == 2 ? 3 : 5;
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveAurasDueToSpell(SPELL_PAIN_AND_SUFFERING);
                        me->SetInCombatWithZone();
                        SetCombatMovement(true);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        break;
                }
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                switch(uiPhase)
                {
                    case 1:
                        if(!HealthAbovePct(71))
                        {
                            uiPhase = 2;
                            DoAction(ACTION_PHASESWITCH_1);
                            break;
                        }
                    case 3:
                        if(!HealthAbovePct(41))
                        {
                            uiPhase = 4;
                            DoAction(ACTION_PHASESWITCH_1);
                            break;
                        }
                    case 5:
                        if(!HealthAbovePct(11))
                        {
                            me->SummonCreature(CREATURE_TRIGGER, MovePos[6], TEMPSUMMON_CORPSE_DESPAWN, 900000);
                            summons.DespawnAll();
                            uiPhase = 6; //ending
                        }
                        break;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;

                if(uiPhase != 6)
                {
                    if (uiRandomSpeechTimer < uiDiff)
                    {
                        DoScriptText(RAND(SAY_RANDOM_1,SAY_RANDOM_2), me);
                        uiRandomSpeechTimer = 33000;
                    } else uiRandomSpeechTimer -= uiDiff;

                    if (uiBerserkTimer < uiDiff)
                    {
                        DoScriptText(SAY_BERSERK, me);
                        DoCast(me, SPELL_BERSERK);
                        uiBerserkTimer = 900000;
                    } else uiBerserkTimer -= uiDiff;
                }

                if(uiPhase == 1)
                {
                    if (IsHeroic())
                    {
                        if (uiSummonShadowTrap < uiDiff)
                        {
                            if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                                DoCast(target, SPELL_SUMMON_SHADOW_TRAP, true);
                            uiSummonShadowTrap = 30000;
                        } else uiSummonShadowTrap -= uiDiff;
                    }

                    if (uiInfestTimer < uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_INFEST);
                        uiInfestTimer = 30000;
                    } else uiInfestTimer -= uiDiff;

                    if (uiSummonDrudgeGhoulsTimer < uiDiff)
                    {
                        DoCast(SPELL_SUMMON_DRUDGE_GHOULS);
                        uiSummonDrudgeGhoulsTimer = 20000;
                    } else uiSummonDrudgeGhoulsTimer -= uiDiff;

                    if (uiSummonShamblingHorrorTimer < uiDiff)
                    {
                        DoCast(SPELL_SUMMON_SHAMBLING_HORROR);
                        uiSummonShamblingHorrorTimer = 30000;
                    } else uiSummonShamblingHorrorTimer -= uiDiff;

                    if (uiNecroticPlagueTimer < uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_NECROTIC_PLAGUE);
                    } else uiNecroticPlagueTimer -= uiDiff;
                }

                if(uiPhase == 2) //transition phase 1
                {
                    if (uiSummonSpiritTimer < uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_SUMMON_RAGING_SPIRIT);
                        uiSummonSpiritTimer = 20000;
                    } else uiSummonSpiritTimer -= uiDiff;

                    if (uiIcePulsSummonTimer < uiDiff)
                    {
                        DoCast(SPELL_SUMMON_ICE_SPEHERE);
                        uiIcePulsSummonTimer = 15000;
                    } else uiIcePulsSummonTimer -= uiDiff;
                }

                if(uiPhase == 3)
                {
                    if (uiDefileTimer < uiDiff)
                    {
                        DoScriptText(SAY_EMOTE_DEFILE, me);
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            DoCast(target, SPELL_SUMMON_DEFILE);
                        uiDefileTimer = 20000;
                    } else uiDefileTimer -= uiDiff;

                    if (uiSummonValkyrTimer < uiDiff)
                    {
                        DoScriptText(SAY_SUMMON_VALKYR, me);
                        DoCast(SPELL_SUMMON_VALKYR);
                        uiSummonValkyrTimer = 35000;
                        uiDefileTimer = 5000; //cast defile after cast valkyr summon
                    } else uiSummonValkyrTimer -= uiDiff;

                    if (uiSoulReaperTimer < uiDiff)
                    {
                        DoCastVictim(SPELL_SOUL_REAPER);
                        DoCast(SPELL_SOUL_REAPER_HASTE_AURA);
                        uiSoulReaperTimer = 30000;
                    } else uiSoulReaperTimer -= uiDiff;

                    if (uiInfestTimer < uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_INFEST);
                        uiInfestTimer = 30000;
                    } else uiInfestTimer -= uiDiff;
                }

                if(uiPhase == 4) //transition phase 2
                {
                    if (uiSummonSpiritTimer < uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_SUMMON_RAGING_SPIRIT);
                        uiSummonSpiritTimer = 25000;
                    } else uiSummonSpiritTimer -= uiDiff;

                    if (uiIcePulsSummonTimer < uiDiff)
                    {
                        DoCast(SPELL_SUMMON_ICE_SPEHERE);
                        uiIcePulsSummonTimer = 15000;
                    } else uiIcePulsSummonTimer -= uiDiff;
                }

                if(uiPhase == 5)
                {
                    if (uiSummonVileSpiritTimer < uiDiff)
                    {
                        DoCast(SPELL_SUMMON_VILE_SPIRIT);
                        uiSummonVileSpiritTimer = 30000;
                    } else uiSummonVileSpiritTimer -= uiDiff;

                    if (uiHarvestSoulTimer < uiDiff)
                    {
                        DoScriptText(SAY_HARVEST_SOUL, me);
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_HARVEST_SOULS);
                        uiHarvestSoulTimer = 70000;
                    } else uiHarvestSoulTimer -= uiDiff;

                    if (uiSoulReaperTimer < uiDiff)
                    {
                        DoCastVictim(SPELL_SOUL_REAPER);
                        DoCast(SPELL_SOUL_REAPER_HASTE_AURA);
                        uiSoulReaperTimer = 30000;
                    } else uiSoulReaperTimer -= uiDiff;

                    if (uiDefileTimer < uiDiff)
                    {
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            DoCast(target, SPELL_SUMMON_DEFILE);
                        uiDefileTimer = 20000;
                    } else uiDefileTimer -= uiDiff;
                }

                if(uiPhase == 6)
                {
                    if (uiEndingTimer <= uiDiff)
                    {
                        switch(uiStage)
                        {
                            case 1:
                                me->GetMotionMaster()->MoveIdle();
                                me->SetReactState(REACT_PASSIVE);
                                me->AttackStop();
                                me->CastStop();
                                me->SetInCombatWithZone();
                                DoScriptText(SAY_10_PROZENT, me);
                                DoCast(SPELL_FURY_OF_FROSTMOURNE);
                                uiEndingTimer = 15000;
                                break;
                            case 2:
                                DoScriptText(SAY_ENDING_1_KING, me);
                                uiEndingTimer = 24000;
                                break;
                            case 3:
                                DoScriptText(SAY_ENDING_2_KING, me);
                                uiEndingTimer = 25000;
                                break;
                            case 4:
                                me->GetMotionMaster()->MovePoint(0, MovePos[1]);
                                uiEndingTimer = 4000;
                                break;
                            case 5:
                                DoScriptText(SAY_ENDING_3_KING, me);
                                DoCast(me, SPELL_CHANNEL_KING);
                                me->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                                uiEndingTimer = 28000;
                                break;
                            case 6:
                                DoScriptText(SAY_ENDING_4_KING, me);
                                uiEndingTimer = 8000;
                                break;
                            case 7:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        DoScriptText(SAY_ENDING_5_TIRION, tirion);
                                uiEndingTimer = 11000;
                                break;
                            }
                            case 8:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        tirion->CastSpell(tirion, SPELL_TIRION_LIGHT, true);
                                uiEndingTimer = 7000;
                                break;
                            }
                            case 9:
                            {
                                if(uiTirionGUID)
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        tirion->GetMotionMaster()->MovePoint(0, MovePos[2]);
                                uiEndingTimer = 1000;
                                break;
                            }
                            case 10:
                            {
                                if(uiTirionGUID)
                                {
                                    if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                    {
                                        tirion->GetMotionMaster()->MoveJump(517.482910f, -2124.905762f, 1040.861328f, 10.0f, 15.0f);
                                        tirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, 375);
                                    }
                                }
                                uiEndingTimer = 1000;
                                break;
                            }
                            case 11:
                                me->RemoveAura(SPELL_CHANNEL_KING);
                                me->CastSpell(me, SPELL_BOOM_VISUAL, false);
                                uiEndingTimer = 3000;
                                break;
                            case 12:
                                DoCast(me, SPELL_DROP_FROSTMOURNE);
                                uiEndingTimer = 1000;
                                break;
                            case 13:
                                DoScriptText(SAY_ENDING_6_KING, me);
                                uiEndingTimer = 3000;
                                break;
                            case 14:
                                DoCast(me, SPELL_SUMMON_FROSTMOURNE_TRIGGER);
                                DoCast(me, SPELL_REMOVE_WEAPON);
                                uiEndingTimer = 2000;
                                break;
                            case 15:
                                me->RemoveAllAuras();
                                DoPlaySoundToSet(me, SOUND_ENDING_7_KING);
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 473);
                                uiEndingTimer = 5000;
                                break;
                            case 16:
                                DoScriptText(SAY_ENDING_8_TIRION, me);
                                uiEndingTimer = 6000;
                                break;
                            case 17:
                                DoCast(SPELL_SUMMON_MENETHIL);
                                uiEndingTimer = 3000;
                                break;
                            case 18:
                                DoScriptText(SAY_ENDING_11_FATHER, me);
                                uiEndingTimer = 6000;
                                break;
                            case 19:
                                DoScriptText(SAY_ENDING_10_TIRION, me);
                                uiEndingTimer = 5000;
                                break;
                            case 20:
                                {
                                    DoScriptText(SAY_ENDING_12_KING, me);
                                    if(uiTirionGUID)
                                    {
                                        if(Creature* tirion = Unit::GetCreature(*me, uiTirionGUID))
                                        {
                                            tirion->SetReactState(REACT_AGGRESSIVE);
                                            tirion->AI()->AttackStart(me);
                                        }
                                    }
                                    if(Creature* father = me->FindNearestCreature(CREAUTRE_MENETHIL, 25.0f, true))
                                        father->AI()->AttackStart(me);
                                    uiEndingTimer = 5000;
                                    break;
                                }
                        }
                        ++uiStage;
                    } else uiEndingTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
            }
        private:
            InstanceScript* instance;

            uint8 uiStage;
            uint8 uiPhase;

            uint32 uiEndingTimer;
            uint32 uiSummonShamblingHorrorTimer;
            uint32 uiSummonDrudgeGhoulsTimer;
            uint32 uiSummonShadowTrap;
            uint32 uiInfestTimer;
            uint32 uiNecroticPlagueTimer;
            uint32 uiBerserkTimer;
            uint32 uiSummonValkyrTimer;
            uint32 uiSoulReaperTimer;
            uint32 uiDefileTimer;
            uint32 uiHarvestSoulTimer;
            uint32 uiSummonVileSpiritTimer;
            uint32 uiIcePulsSummonTimer;
            uint32 uiSummonSpiritTimer;
            uint32 uiRandomSpeechTimer;
            uint64 uiTirionGUID;

            SummonList summons;

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_the_lich_kingAI(creature);
        }
};

class npc_tirion_icc : public CreatureScript
{
    public:
        npc_tirion_icc() : CreatureScript("npc_tirion_icc") { }

        struct npc_tirion_iccAI : public ScriptedAI
        {
            npc_tirion_iccAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset()
            {
                uiIntroTimer = 1000;
                uiStage = 1;
                uiLichKingGUID = 0;
                bIntro = false;

                me->SetReactState(REACT_PASSIVE);
                me->SetSpeed(MOVE_RUN, 1.8f);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void DoAction(const int32 action)
            {
                if(action == ACTION_START_EVENT)
                {
                    bIntro = true;
                    if(instance)
                        uiLichKingGUID = instance->GetData64(DATA_LICH_KING);
                }
            }

            void SpellHit(Unit* /*caster*/, const SpellEntry * spell)
            {
                if(spell->Id == SPELL_LIGH_EFFECT)
                    me->RemoveAurasDueToSpell(SPELL_ICEBLOCK_TRIGGER);
            }

            void UpdateAI(const uint32 diff)
            {
                if(!bIntro || !uiLichKingGUID)
                    return;

                if(uiIntroTimer <= diff)
                {
                    switch(uiStage)
                    {
                        case 1:
                            {
                                if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                {
                                    lich->SetStandState(UNIT_STAND_STATE_STAND);
                                    lich->GetMotionMaster()->MovePoint(POINT_START_EVENT_1, MovePos[0]);
                                }
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2H);
                                uiIntroTimer = 3000;
                                break;
                            }
                        case 2:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_TALK);
                            DoScriptText(SAY_INTRO_1_KING, me);
                            uiIntroTimer = 14000;
                            break;
                        }
                        case 3:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            DoScriptText(SAY_INTRO_2_TIRION, me);
                            uiIntroTimer = 9000;
                            break;
                        }
                        case 4:
                        {
                            DoScriptText(SAY_INTRO_3_KING, me);
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_LAUGH);
                            uiIntroTimer = 3000;
                            break;
                        }
                        case 5:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NOSHEATHE);
                            uiIntroTimer = 2000;
                            break;
                        }
                        case 6:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            uiIntroTimer = 18000;
                            break;
                        }
                        case 7:
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_POINT_NOSHEATHE);
                            DoScriptText(SAY_INTRO_4_TIRION, me);
                            uiIntroTimer = 1000;
                            break;
                        case 8:
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
                            me->GetMotionMaster()->MovePoint(0, MovePos[3]);
                            uiIntroTimer = 2000;
                            break;
                        case 9:
                        {
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                                lich->CastSpell(me, SPELL_ICEBLOCK_TRIGGER, true);
                            uiIntroTimer = 2000;
                            break;
                        }
                        case 10:
                        {
                            DoScriptText(SAY_INTRO_5_KING, me);
                            if(Creature* lich = Unit::GetCreature(*me, uiLichKingGUID))
                            {
                                lich->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                lich->SetReactState(REACT_AGGRESSIVE);
                                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                    lich->AI()->AttackStart(target);
                            }
                            uiIntroTimer = 12000;
                            break;
                        }
                    }
                    ++uiStage;
                } else uiIntroTimer -= diff;
            }
            private:
                InstanceScript* instance;

                uint64 uiLichKingGUID;
                uint32 uiIntroTimer;
                uint8 uiStage;
                bool bIntro;
        };

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START_EVENT, GOSSIP_SENDER_MAIN, 999999);
            player->SEND_GOSSIP_MENU(GOSSIP_MENU, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
        {
            if (uiAction == 999999)
            {
                CAST_AI(npc_tirion_icc::npc_tirion_iccAI, creature->AI())->DoAction(ACTION_START_EVENT);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                player->CLOSE_GOSSIP_MENU();
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tirion_iccAI(creature);
        }
};

class npc_valkyr_icc : public CreatureScript
{
    public:
        npc_valkyr_icc() : CreatureScript("npc_valkyr_icc") { }

        struct npc_valkyr_iccAI : public ScriptedAI
        {
            npc_valkyr_iccAI(Creature* creature) : ScriptedAI(creature), vehicle(creature->GetVehicleKit())
            {
                ASSERT(vehicle);
            }

            void Reset()
            {
                me->SetFlying(true);
                bCanCast = false;
                m_uiLifeSiphonTimer = 3000;

                if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                    me->GetMotionMaster()->MovementExpired();
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                if(!HealthAbovePct(50) && IsHeroic() && !bCanCast)
                {
                    vehicle->RemoveAllPassengers();
                    me->GetMotionMaster()->MovePoint(POINT_VALKYR_ZET, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 10);
                }
            }

            void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply)
            {
                if(apply)
                    me->GetMotionMaster()->MovePoint(POINT_PLATFORM_END, MovePos[4]);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if(type != POINT_MOTION_TYPE)
                    return;

                if(bCanCast)
                    me->GetMotionMaster()->Clear();

                switch(id)
                {
                    case POINT_PLATFORM_END:
                        vehicle->RemoveAllPassengers();
                        float x,y,z;
                        me->GetNearPoint2D(x, y, 50, me->GetAngle(me));
                        me->GetMotionMaster()->MovePoint(POINT_VALKYR_END,x,y,z+15);
                        break;
                    case POINT_VALKYR_END:
                        me->DespawnOrUnsummon();
                        break;
                    case POINT_VALKYR_ZET:
                        bCanCast = true;
                        break;
                }
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim() || !bCanCast)
                    return;

                if (m_uiLifeSiphonTimer < uiDiff)
                {
                    DoCastVictim(SPELL_LIFE_SIPHON);
                    m_uiLifeSiphonTimer = 3000;
                } else m_uiLifeSiphonTimer -= uiDiff;
            }
        private:
            uint32 m_uiLifeSiphonTimer;
            bool bCanCast;

            Vehicle* vehicle;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_valkyr_iccAI(creature);
        }
};

class npc_vile_spirit_icc : public CreatureScript
{
    public:
        npc_vile_spirit_icc() : CreatureScript("npc_vile_spirit_icc") { }

        struct npc_vile_spirit_iccAI : public ScriptedAI
        {
            npc_vile_spirit_iccAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                uiMoveTimer = 15000;
                SetCombatMovement(false);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (uiMoveTimer < uiDiff)
                {
                    DoCast(me, SPELL_VILE_SPIRIT_ACTIVE);
                    SetCombatMovement(true);
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true))
                        me->AddThreat(target, 100000.0f);
                    uiMoveTimer = 1500000;
                } else uiMoveTimer -= uiDiff;
            }
        private:
            uint32 uiMoveTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_vile_spirit_iccAI(creature);
        }
};

class spell_lich_king_necrotic_plague : public SpellScriptLoader
{
    public:
        spell_lich_king_necrotic_plague() : SpellScriptLoader("spell_lich_king_necrotic_plague") { } //70338

        class spell_lich_king_necrotic_plague_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_necrotic_plague_AuraScript)

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                {
                    SetStackAmount(instance->GetData(DATA_NECROTIC_STACK));
                    instance->SetData(DATA_NECROTIC_STACK, GetStackAmount() + 1);
                    if(GetStackAmount() >= 30)
                        instance->SetData(DATA_BEEN_WAITING, DONE);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_necrotic_plague_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_necrotic_plague_AuraScript();
        }
};

class spell_lich_king_infection : public SpellScriptLoader
{
    public:
        spell_lich_king_infection() : SpellScriptLoader("spell_lich_king_infection") { } //70541


        class spell_lich_king_infection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_infection_AuraScript)

            void OnPeriodic(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if(GetTarget()->GetHealthPct() < 90 || !GetCaster())
                    return;

                GetTarget()->RemoveAurasDueToSpell(SPELL_INFEST);
            }

            void Register()
            {
               OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_infection_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_infection_AuraScript();
        }
};

class spell_lich_king_valkyr_summon : public SpellScriptLoader
{
    public:
        spell_lich_king_valkyr_summon() : SpellScriptLoader("spell_lich_king_valkyr_summon") { } //74361


        class spell_lich_king_valkyr_summon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_valkyr_summon_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    uint32 spawnMod = caster->GetMap()->GetSpawnMode();
                    SetDuration(spawnMod == 1 || spawnMod == 3 ? 3000 : 1000);
                }
            }

            void HandleTriggerSpell(AuraEffect const* aurEff)
            {
                PreventDefaultAction();
                if (Unit* caster = GetCaster())
                {
                    uint8 triggerSpellId = GetSpellProto()->EffectTriggerSpell[aurEff->GetEffIndex()];
                    float x, y, z;
                    caster->GetPosition(x, y, z);
                    caster->CastSpell(x, y, z + 6, triggerSpellId, true, NULL, NULL, GetCasterGUID(), caster);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_lich_king_valkyr_summon_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectApply += AuraEffectApplyFn(spell_lich_king_valkyr_summon_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_valkyr_summon_AuraScript();
        }
};

class spell_lich_king_winter : public SpellScriptLoader
{
    public:
        spell_lich_king_winter() : SpellScriptLoader("spell_lich_king_winter") { } //68981


        class spell_lich_king_winter_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_winter_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_QUAKE, true);
                    DoScriptText(SAY_BROKEN_ARENA, caster);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_winter_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_winter_AuraScript();
        }
};

class spell_lich_king_quake : public SpellScriptLoader
{
    public:
        spell_lich_king_quake() : SpellScriptLoader("spell_lich_king_quake") { } //72262


        class spell_lich_king_quake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lich_king_quake_AuraScript)

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if(!GetCaster() || !GetOwner())
                    return;

                GetCaster()->CastSpell(GetCaster(), SPELL_WMO_DAMAGE, true);
                if (InstanceScript* instance = GetTarget()->GetInstanceScript())
                    if (Creature* lichKing = Unit::GetCreature(*GetTarget(), instance->GetData64(DATA_LICH_KING)))
                        lichKing->AI()->DoAction(ACTION_PHASESWITCH_2);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_lich_king_quake_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lich_king_quake_AuraScript();
        }
};

class spell_vile_spirit_distance_check : public SpellScriptLoader
{
    public:
        spell_vile_spirit_distance_check() : SpellScriptLoader("spell_vile_spirit_distance_check") { }


        class spell_vile_spirit_distance_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_vile_spirit_distance_check_SpellScript);


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if(Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_BURST, true);

                    if (InstanceScript* instance = caster->GetInstanceScript())
                        instance->SetData(DATA_NECK_DEEP, FAIL);
                }
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_vile_spirit_distance_check_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_vile_spirit_distance_check_SpellScript();
        }
};

class spell_ice_burst_distance_check : public SpellScriptLoader
{
    public:
        spell_ice_burst_distance_check() : SpellScriptLoader("spell_ice_burst_distance_check") { }


        class spell_ice_burst_distance_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ice_burst_distance_check_SpellScript);


            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if(Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_ICE_BURST, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_ice_burst_distance_check_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ice_burst_distance_check_SpellScript();
        }
};

class spell_lich_king_play_movie : public SpellScriptLoader
{
    public:
        spell_lich_king_play_movie() : SpellScriptLoader("spell_lich_king_play_movie") { }


        class spell_lich_king_play_movie_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lich_king_play_movie_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive()))
                    return;

                if (GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
                    GetHitUnit()->ToPlayer()->SendMovieStart(MOVIE_ID_ARTHAS_DEATH);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_lich_king_play_movie_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lich_king_play_movie_SpellScript();
        }
};

class spell_valkyr_carry_can_cast : public SpellScriptLoader
{
    public:
        spell_valkyr_carry_can_cast() : SpellScriptLoader("spell_valkyr_carry_can_cast") { }


        class spell_valkyr_carry_can_cast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_valkyr_carry_can_cast_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (!(GetHitUnit() && GetHitUnit()->isAlive() && GetCaster()))
                    return;

                if (GetHitUnit()->GetTypeId() == TYPEID_PLAYER)
                    GetHitUnit()->CastSpell(GetCaster(), SPELL_VALKYR_MOVE_PLAYER, true);
            }

            void Register()
            {
                OnEffect += SpellEffectFn(spell_valkyr_carry_can_cast_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_valkyr_carry_can_cast_SpellScript();
        }
};

void AddSC_boss_lichking()
{
    new boss_the_lich_king();
    new npc_tirion_icc();
    new npc_valkyr_icc();
    new npc_vile_spirit_icc();
    new spell_lich_king_necrotic_plague();
    new spell_lich_king_infection();
    new spell_lich_king_valkyr_summon();
    new spell_lich_king_winter();
    new spell_vile_spirit_distance_check();
    new spell_ice_burst_distance_check();
    new spell_lich_king_quake();
    new spell_lich_king_play_movie();
    new spell_valkyr_carry_can_cast();
}