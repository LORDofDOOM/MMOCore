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


/*
* TODO:
* - handle snobold's fire bomb, [DONE]
* - fix acidmaw's paralitic toxin, [NOT DONE]
* - fix jormungars "static" phase also burrow break and adds, [DONE/DONE/NOT DONE]
* - fix icehowl charge and also add "on stun's remove" speed buff,
* - fix jaraxxus' adds (sisters and volcans), [+/- DONE]
* - write/rewrite champions, [+/- DONE]
* - fix valkyr's timers to prevent begining event while casting spell, [DONE]
* - handle "Powering Up"'s effect (stacking on absorbing dmg), [+/- DONE]
* - [visual] handle model changes effect of Light/Dark Essence, [NOT DONE]
* - summon Anub's adds, [DONE]
* - handle Anub's leeching swarm spells effect, [NOT DONE]
* - handle Anub's spike, [NOT DONE]
* - add missing cache after defeating Anub, [DONE]
*
* - ACHIEVEMENTS. [DONE]
*/

#include "ScriptPCH.h"
#include "trial_of_the_crusader.h"
#include "Group.h"

enum Says
{
    SAY_TOC_WELCOME         = -1760000,
    SAY_BEASTS_GORMOK       = -1760001,
    SAY_BEASTS_JORMUNGARS   = -1760002,
    SAY_BEASTS_ICEHOWL      = -1760004,
    SAY_BEASTS_DEFEATED     = -1760005,

    SAY_JARAXXUS_INTRO_1    = -1760007,
    SAY_JARAXXUS_INTRO_2    = -1760008,
    SAY_JARAXXUS_INTRO_3    = -1760009,
    SAY_JARAXXUS_INTRO_4    = -1760010,
    SAY_JARAXXUS_INTRO_5    = -1760011,
    SAY_JARAXXUS_INTRO_6    = -1760012,
    SAY_JARAXXUS_INTRO_7    = -1760013,
    SAY_JARAXXUS_KILL_GNOME = -1760020,

    SAY_JARAXXUS_OUTRO_1    = -1760022,
    SAY_JARAXXUS_OUTRO_2    = -1760023,
    SAY_JARAXXUS_OUTRO_3    = -1760024,
    SAY_JARAXXUS_OUTRO_4    = -1760025,

    SAY_CHAMPIONS_INTROH_1  = -1760026,
    SAY_CHAMPIONS_INTROA_1  = -1760026,
    SAY_CHAMPIONS_INTROA_2  = -1760027,
    SAY_CHAMPIONS_INTROH_2  = -1760028,
    SAY_CHAMPIONS_INTROH_3  = -1760029,
    SAY_CHAMPIONS_INTROA_3  = -1760029,
    SAY_CHAMPIONS_INTROA_4  = -1760030,
    SAY_CHAMPIONS_INTROH_4  = -1760031,

    SAY_CHAMPIONS_OUTRO     = -1760042,

    SAY_TWINS_INTRO_1       = -1760043,
    SAY_TWINS_INTRO_2       = -1760044,

    SAY_TWINS_OUTRO_H       = -1760062,
    SAY_TWINS_OUTRO_A       = -1760063,

    SAY_ANUB_INTRO_1        = -1760064,
    SAY_ANUB_INTRO_2        = -1760065,
    SAY_ANUB_INTRO_3        = -1760066,
    SAY_ANUB_INTRO_4        = -1760067,
    SAY_ANUB_INTRO_5        = -1760068,

    SAY_ANUB_OUTRO          = -1760076
};

enum
{
    NPC_WILFRED_PORTAL      = 17965,
    SPELL_WILFRED_PORTAL    = 68424,
    NPC_TRIGGER             = 35651
};

Position positionSpawnBeasts        = { 563.449036f, 221.046951f, 394.696320f, 4.753102f };
Position positionSpawnBeastsLeft    = { 556.771118f, 221.083862f, 394.696320f, 4.753102f };
Position positionSpawnBeastsRight   = { 569.916931f, 221.011200f, 394.696320f, 4.753102f };
Position positionMoveBeasts         = { 563.783936f, 172.178909f, 394.436584f, 4.728253f };
Position positionMoveBeastsLeft     = { 554.333557f, 175.043610f, 394.803589f, 4.438710f };
Position positionMoveBeastsRight    = { 574.216431f, 174.672394f, 394.802795f, 4.670400f };
Position positionCenter             = { 563.800964f, 140.557205f, 393.957205f, 4.700161f };
Position positionTirionRelocate     = { 690.230408f, 141.717117f, 142.148895f, 6.148393f };
Position positionPortalSpawn        = { 677.135498f, 130.135345f, 142.123535f, 6.128760f };
Position positionGnomEnter          = { 564.215942f, 148.305939f, 394.039246f, 4.666639f };
Position positionJaraxxusMove       = { 563.767639f, 128.735519f, 394.039215f, 1.535515f };


Position AllyChampionsEnter[10]  =
{
    { 616.278076f, 137.326508f, 395.243225f, 3.045343f },
    { 616.613098f, 140.796371f, 395.247955f, 3.045343f },
    { 613.833984f, 144.433746f, 395.237885f, 3.049270f },
    { 612.967957f, 135.079758f, 395.241821f, 3.049270f },
    { 611.324707f, 137.977295f, 395.247955f, 3.061051f },
    { 611.448120f, 141.866608f, 395.247711f, 3.147445f },
    { 615.436157f, 139.114532f, 395.259827f, 3.119956f },
    { 617.680786f, 131.650848f, 395.220215f, 3.061051f },
    { 616.449768f, 148.894180f, 395.229095f, 3.072832f },
    { 622.792908f, 139.316803f, 395.260101f, 3.147445f }
};

Position HordeChampionsEnter[10] =
{
    { 511.680023f, 141.800217f, 395.239777f, 6.056552f },
    { 511.073547f, 138.317688f, 395.251678f, 6.115457f },
    { 513.268250f, 144.702560f, 395.236938f, 6.146870f },
    { 512.882202f, 134.244919f, 395.234497f, 6.256824f },
    { 516.797974f, 135.115753f, 395.255585f, 0.012908f },
    { 517.336060f, 140.881577f, 395.254211f, 0.041182f },
    { 515.247925f, 138.992340f, 395.258514f, 0.017611f },
    { 507.589478f, 139.128632f, 395.259857f, 0.041170f },
    { 512.324036f, 130.595093f, 395.238464f, 0.204556f },
    { 517.693970f, 145.195480f, 395.259247f, 0.072586f }
};


class toc_tirion_fordring : public CreatureScript
{
public:
    toc_tirion_fordring() : CreatureScript("toc_tirion_fordring") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new toc_tirion_fordringAI(pCreature);
    }

    struct toc_tirion_fordringAI : public ScriptedAI
    {
        toc_tirion_fordringAI(Creature* c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
            greet = false;
        }

        InstanceScript* pInstance;

        bool started;
        bool gormokDefeted;
        bool jormungarsDefeted;
        bool openGate;
        bool greet;

        uint32 boss_nr;
        uint32 text_nr;
        uint32 timer;
        uint32 gateTimer;
        uint32 heroBeastsTimer;
        uint64 jaraxxusPortal;
        uint64 jaraxxusTrigger;

        void Reset()
        {
            started = false;
            gormokDefeted = false;
            jormungarsDefeted = false;
            boss_nr = 0;
            text_nr = 0;
            timer = 5000;
            openGate = false;
            gateTimer = 5000;
            heroBeastsTimer = 180000;
            jaraxxusPortal = 0;
            jaraxxusTrigger = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!pInstance)
                return;

            if (!started && boss_nr == 1)
                heroBeastsTimer = 180000;

            if (openGate)
            {
                if (gateTimer < diff)
                {
                    pInstance->DoUseDoorOrButton(pInstance->GetData64(MAIN_GATE));
                    openGate = false;
                    gateTimer = 5000;
                } else gateTimer -= diff;
            }

            if (started && (getDifficulty() == 2 || getDifficulty() == 3) && boss_nr == 1)
            {
                if (!gormokDefeted)
                {
                    if (heroBeastsTimer < diff)
                    {
                        gormokDefeted = true;
                        heroBeastsTimer = 180000;
                    } else heroBeastsTimer -= diff;
                }
                else if (!jormungarsDefeted)
                {
                    if (heroBeastsTimer < diff)
                    {
                        jormungarsDefeted = true;
                        heroBeastsTimer = 180000;
                    } else heroBeastsTimer -= diff;
                }
            }

            UpdateBosses();

            if (started)
            {
                if (boss_nr == 0)
                    boss_nr = 1;
                if (text_nr == 0)
                    text_nr = 1;

                switch (boss_nr)
                {
                case 1:
                    if (text_nr == 1)
                    {
                        if (!greet && !pInstance->GetData(DATA_DEATHS_COUNT))
                        {
                            if (timer < diff)
                            {
                                DoScriptText(SAY_TOC_WELCOME, me);
                                greet = true;
                                timer = 30000;
                                ++text_nr;
                            } else timer -= diff;
                        }
                        else
                            ++text_nr;
                    }

                    //beasts introes
                    if (text_nr == 2)
                    {
                        if (timer < diff)
                        {
                            ++text_nr;
                            timer = 8000;

                            DoScriptText(SAY_BEASTS_GORMOK, me);
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(MAIN_GATE));
                            openGate = true;
                            if (Creature* gormok = me->SummonCreature(NPC_GORMOK, positionSpawnBeasts))
                            {
                                gormok->GetMotionMaster()->MovePoint(0, positionMoveBeasts);
                                gormok->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                gormok->SetReactState(REACT_PASSIVE);
                            }
                        } else timer -= diff;
                    }

                    if (text_nr == 3)
                    {
                        if (timer < diff)
                        {
                            ++text_nr;
                            timer = 5000;
                            if (Creature* gormok = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_GORMOK)))
                            {
                                gormok->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                gormok->SetReactState(REACT_AGGRESSIVE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                    gormok->AI()->AttackStart(target);
                            }
                        } else timer -= diff;
                    }

                    if (gormokDefeted && text_nr == 4)
                    {
                        if (timer < diff)
                        {
                            ++text_nr;
                            timer = 8000;

                            DoScriptText(SAY_BEASTS_JORMUNGARS, me);
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(MAIN_GATE));
                            openGate = true;
                            if (Creature* dreadscale = me->SummonCreature(NPC_DREADSCALE, positionSpawnBeasts))
                            {
                                dreadscale->GetMotionMaster()->MovePoint(0, positionMoveBeasts);
                                dreadscale->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                dreadscale->SetReactState(REACT_PASSIVE);
                            }
                            if (Creature* acidmaw = me->SummonCreature(NPC_ACIDMAW, positionSpawnBeastsLeft))
                            {
                                acidmaw->GetMotionMaster()->MovePoint(0, positionMoveBeastsLeft);
                                acidmaw->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                acidmaw->SetReactState(REACT_PASSIVE);
                            }
                        } else timer -= diff;
                    }

                    if (text_nr == 5)
                    {
                        if (timer < diff)
                        {
                            ++text_nr;
                            timer = 5000;
                            if (Creature* acidmaw = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_ACIDMAW)))
                            {
                                acidmaw->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                acidmaw->SetReactState(REACT_AGGRESSIVE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                    acidmaw->AI()->AttackStart(target);
                            }
                            if (Creature* dreadscale = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_DREADSCALE)))
                            {
                                dreadscale->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                dreadscale->SetReactState(REACT_AGGRESSIVE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                    dreadscale->AI()->AttackStart(target);
                            }
                        } else timer -= diff;
                    }

                    if (jormungarsDefeted && text_nr == 6)
                    {
                        if (timer < diff)
                        {
                            ++text_nr;
                            timer = 8000;

                            DoScriptText(SAY_BEASTS_ICEHOWL, me);
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(MAIN_GATE));
                            openGate = true;
                            if (Creature* icehowl = me->SummonCreature(NPC_ICEHOWL, positionSpawnBeasts))
                            {
                                icehowl->GetMotionMaster()->MovePoint(0, positionMoveBeasts);
                                icehowl->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                icehowl->SetReactState(REACT_PASSIVE);
                            }

                            break;

                        } else timer -= diff;
                    }

                    if (text_nr == 7)
                    {
                        if (timer < diff)
                        {
                            ++text_nr;
                            timer = 5000;
                            if (Creature* icehowl = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_ICEHOWL)))
                            {
                                icehowl->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                icehowl->SetReactState(REACT_AGGRESSIVE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                    icehowl->AI()->AttackStart(target);
                            }
                        }else timer -= diff;
                    }

                    break;

                case 2:
                    //beasts outro
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            if (Creature* gadajacy = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GADAJACY)))
                            {
                                if (!gadajacy->isAlive())
                                    gadajacy->Respawn(true);

                                gadajacy->SetVisible(true);
                            }
                            DoScriptText(SAY_BEASTS_DEFEATED, me);

                            ++boss_nr;//switch to next boss' intro and wait for player (started == true)
                            text_nr=0;
                            timer = 5000;
                            started = false;
                            break;
                        }
                    } else timer -= diff;
                    break;

                case 3:
                    //Jaraxxus intro
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            DoScriptText(SAY_JARAXXUS_INTRO_1, me);

                            for (int i=BOSS_GORMOK; i<=BOSS_ICEHOWL; ++i)
                            {
                                if (Creature* boss = ObjectAccessor::GetCreature((*me), pInstance->GetData64(i)))
                                    boss->DespawnOrUnsummon();
                            }

                            if (GameObject* go = me->GetMap()->GetGameObject(pInstance->GetData64(CHAMPIONS_CACHE)))
                                go->Delete();

                            ++text_nr;
                            timer = 10000;
                            break;

                        case 2:
                            if (Creature* gnomLock = DoSummon(35458, positionSpawnBeasts))
                            {
                                gnomLock->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                                gnomLock->GetMotionMaster()->MovePoint(0, positionGnomEnter);
                                pInstance->DoUseDoorOrButton(pInstance->GetData64(MAIN_GATE));
                                openGate = true;
                            }
                            ++text_nr;
                            timer = 30000;
                            break;

                        case 3:
                            if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                            {
                                DoScriptText(SAY_JARAXXUS_INTRO_2, gnomLock);
                                gnomLock->HandleEmoteCommand(EMOTE_ONESHOT_BOW);
                            }
                            ++text_nr;
                            timer = 12000;
                            break;

                        case 4:
                            if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                            {
                                DoScriptText(SAY_JARAXXUS_INTRO_3, gnomLock);
                                gnomLock->HandleEmoteCommand(EMOTE_ONESHOT_SPELLCAST_OMNI);
                                if (Unit* pTrigger =  me->SummonCreature(NPC_TRIGGER, positionCenter, TEMPSUMMON_MANUAL_DESPAWN))
                                {
                                    jaraxxusTrigger = pTrigger->GetGUID();
                                    pTrigger->SetFloatValue(OBJECT_FIELD_SCALE_X, 2.0f);
                                    pTrigger->SetDisplayId(22862);
                                    pTrigger->CastSpell(pTrigger, SPELL_WILFRED_PORTAL, false);
                                }
                            }

                            ++text_nr;
                            timer = 6000;
                            break;

                        case 5:
                            if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                            {
                                gnomLock->HandleEmoteCommand(EMOTE_ONESHOT_SPELLCAST_OMNI);
                                if (Creature* pPortal = me->SummonCreature(NPC_WILFRED_PORTAL, positionCenter, TEMPSUMMON_MANUAL_DESPAWN))
                                {
                                    pPortal->SetReactState(REACT_PASSIVE);
                                    pPortal->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.5f);
                                    pPortal->CastSpell(pPortal,SPELL_WILFRED_PORTAL,false);
                                    jaraxxusPortal = pPortal->GetGUID();
                                }
                            }
                            ++text_nr;
                            timer = 2000;

                        case 6:
                            if (Creature* jaraxxus = me->SummonCreature(NPC_LORD_JARAXXUS, positionCenter))
                            {
                                jaraxxus->SetReactState(REACT_PASSIVE);
                                jaraxxus->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                jaraxxus->AttackStop();
                                jaraxxus->GetMotionMaster()->MovePoint(0, positionJaraxxusMove);
                            }
                            ++text_nr;
                            timer = 2000;
                            break;

                        case 7:
                            if (Creature* pTemp = Unit::GetCreature(*me, pInstance->GetData64(BOSS_JARAXXUS)))
                            {
                                if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                                {
                                    pTemp->SetUInt64Value(UNIT_FIELD_TARGET, gnomLock->GetGUID());
                                    pTemp->SetFacingToObject(gnomLock);
                                }
                            }
                            if (Creature* pTrigger = Unit::GetCreature(*me, jaraxxusTrigger))
                                pTrigger->DespawnOrUnsummon();
                            if (Creature* pPortal = Unit::GetCreature(*me, jaraxxusPortal))
                                pPortal->DespawnOrUnsummon();
                            jaraxxusPortal = 0;
                            jaraxxusTrigger = 0;
                            ++text_nr;
                            timer = 1000;
                            break;

                        case 8:
                            if (Creature* gnomLock = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), pInstance->GetData64(GNOM_WARLOCK)))
                                DoScriptText(SAY_JARAXXUS_INTRO_4, gnomLock);

                            ++text_nr;
                            timer = 5000;
                            break;

                        case 9:
                            if (Creature* jaraxxus = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), pInstance->GetData64(BOSS_JARAXXUS)))
                                DoScriptText(SAY_JARAXXUS_INTRO_5, jaraxxus);

                            ++text_nr;
                            timer = 6000;
                            break;

                        case 10:
                            if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                                DoScriptText(SAY_JARAXXUS_INTRO_6, gnomLock);

                            ++text_nr;
                            timer = 750;
                            break;

                        case 11:
                            if (Creature* jaraxxus = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_JARAXXUS)))
                                if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                                {
                                    jaraxxus->CastSpell(gnomLock, 67888, false);
                                    DoScriptText(SAY_JARAXXUS_KILL_GNOME, jaraxxus);
                                }

                            ++text_nr;
                            timer = 2500;
                            break;

                        case 12:
                            DoScriptText(SAY_JARAXXUS_INTRO_7, me);
                            if (Creature* jaraxxus = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_JARAXXUS)))
                                jaraxxus->SetFacingToObject(me);

                            ++text_nr;
                            timer = 5500;
                            break;

                        case 13:
                            if (Creature* jaraxxus = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_JARAXXUS)))
                            {
                                jaraxxus->SetReactState(REACT_AGGRESSIVE);
                                jaraxxus->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                {
                                    jaraxxus->AI()->EnterCombat(target);
                                    jaraxxus->AI()->AttackStart(target);
                                }
                            }
                            ++text_nr;
                            timer = 5000;
                            break;
                        }
                    } else timer -= diff;
                    break;

                case 4:
                    //Jaraxxus outro
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            DoScriptText(SAY_JARAXXUS_OUTRO_1, me);

                            ++text_nr;
                            timer = 14000;
                            break;

                        case 2:
                            if (Creature* garosh = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GAROSH_HELLSCREAM)))
                            {
                                DoScriptText(SAY_JARAXXUS_OUTRO_2, garosh);
                                ++text_nr;
                                timer = 7000;
                            }
                            break;

                        case 3:
                            if (Creature* vrynn = ObjectAccessor::GetCreature((*me), pInstance->GetData64(WARIAN_VRYNN)))
                            {
                                DoScriptText(SAY_JARAXXUS_OUTRO_3, vrynn);
                                ++text_nr;
                                timer = 7000;
                            }
                            break;

                        case 4:
                            DoScriptText(SAY_JARAXXUS_OUTRO_4, me);

                            if (Creature* gadajacy = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GADAJACY)))
                            {
                                if (!gadajacy->isAlive())
                                    gadajacy->Respawn(true);

                                gadajacy->SetVisible(true);
                            }

                            ++boss_nr;
                            text_nr=0;
                            timer = 5000;
                            started = false;
                            break;
                        }
                    } else timer -= diff;
                    break;

                case 5:
                    //champios intro
                    if (uint32 team = pInstance->GetData(DATA_TEAM))
                    {
                        switch (team)
                        {
                        case ALLIANCE:
                            if (timer < diff)
                            {
                                switch (text_nr)
                                {
                                case 1:
                                    DoScriptText(SAY_CHAMPIONS_INTROA_1, me);

                                    for (int i=BOSS_JARAXXUS; i<=BOSS_JARAXXUS; ++i)
                                    {
                                        if (Creature* boss = ObjectAccessor::GetCreature((*me), pInstance->GetData64(i)))
                                            boss->DespawnOrUnsummon();
                                    }

                                    if (Creature* gnomLock = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GNOM_WARLOCK)))
                                        gnomLock->SetVisible(false);

                                    ++text_nr;
                                    timer = 10500;
                                    break;

                                case 2:
                                    if (Creature* garosh = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GAROSH_HELLSCREAM)))
                                    {
                                        DoScriptText(SAY_CHAMPIONS_INTROA_2, garosh);
                                        ++text_nr;
                                        timer = 12000;
                                    }
                                    break;

                                case 3:
                                    DoScriptText(SAY_CHAMPIONS_INTROA_3, me);
                                    SummonChampions();
                                    ++text_nr;
                                    timer = 4000;
                                    break;

                                case 4:
                                    if (Creature* garosh = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GAROSH_HELLSCREAM)))
                                    {
                                        DoScriptText(SAY_CHAMPIONS_INTROA_4, garosh);
                                        ++text_nr;
                                        timer = 6000;
                                    }
                                    break;
                                }
                            } else timer -= diff;
                            break;

                        case HORDE:
                            //ladujemy dialogi dla hordy
                            if (timer < diff)
                            {
                                switch (text_nr)
                                {
                                case 1:
                                    DoScriptText(SAY_CHAMPIONS_INTROH_1, me);

                                    for (int i=BOSS_JARAXXUS; i<=BOSS_JARAXXUS; ++i)
                                    {
                                        if (Creature* boss = ObjectAccessor::GetCreature((*me), pInstance->GetData64(i)))
                                            boss->DespawnOrUnsummon();
                                    }

                                    ++text_nr;
                                    timer = 10500;
                                    break;

                                case 2:
                                    if (Creature* vrynn = ObjectAccessor::GetCreature((*me), pInstance->GetData64(WARIAN_VRYNN)))
                                    {
                                        DoScriptText(SAY_CHAMPIONS_INTROH_2, vrynn);
                                        ++text_nr;
                                        timer = 11000;
                                    }
                                    break;

                                case 3:
                                    DoScriptText(SAY_CHAMPIONS_INTROH_3, me);
                                    SummonChampions();
                                    ++text_nr;
                                    timer = 4000;
                                    break;

                                case 4:
                                    if (Creature* vrynn = ObjectAccessor::GetCreature((*me), pInstance->GetData64(WARIAN_VRYNN)))
                                    {
                                        DoScriptText(SAY_CHAMPIONS_INTROH_4, vrynn);
                                        ++text_nr;
                                        timer = 6000;
                                    }
                                    break;
                                }
                            } else timer -= diff;
                            break;
                        }
                    }
                    break;

                case 6:
                    //champions outro
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            DoScriptText(SAY_CHAMPIONS_OUTRO, me);

                             if (Creature* gadajacy = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GADAJACY)))
                            {
                                if (!gadajacy->isAlive())
                                    gadajacy->Respawn(true);

                                gadajacy->SetVisible(true);
                            }

                            ++boss_nr;
                            text_nr=0;
                            timer = 5000;
                            started = false;
                            break;
                        }
                    } else timer -= diff;
                    break;

                case 7:
                    //twins intro
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            DoScriptText(SAY_TWINS_INTRO_1, me);
                            pInstance->DoUseDoorOrButton(pInstance->GetData64(MAIN_GATE));
                            openGate = true;

                            if (GameObject* chest = me->GetMap()->GetGameObject(pInstance->GetData64(CHAMPIONS_CACHE)))
                                chest->Delete();

                            for (int i=BOSS_CHAMPION_DK; i<=BOSS_CHAMPION_WAREK; ++i)
                            {
                                if (Creature* boss = ObjectAccessor::GetCreature((*me), pInstance->GetData64(i)))
                                    boss->DespawnOrUnsummon();
                            }

                            if (Creature* lightTwin = me->SummonCreature(NPC_LIGHT_TWIN, positionSpawnBeastsLeft))
                            {
                                lightTwin->GetMotionMaster()->MovePoint(0, positionMoveBeastsLeft);
                                lightTwin->SetReactState(REACT_PASSIVE);
                                lightTwin->SetUnitMovementFlags(MOVEMENTFLAG_JUMPING);
                                lightTwin->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            }
                            if (Creature* darkTwin = me->SummonCreature(NPC_DARK_TWIN, positionSpawnBeastsRight))
                            {
                                darkTwin->GetMotionMaster()->MovePoint(0, positionMoveBeastsRight);
                                darkTwin->SetReactState(REACT_PASSIVE);
                                darkTwin->SetUnitMovementFlags(MOVEMENTFLAG_JUMPING);
                                darkTwin->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            }

                            ++text_nr;
                            timer = 13000;
                            break;

                        case 2:
                            DoScriptText(SAY_TWINS_INTRO_2, me);
                            ++text_nr;
                            timer = 2500;
                            break;

                        case 3:
                            if (Creature* lightTwin = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_LIGHT_TWIN)))
                            {
                                lightTwin->SetReactState(REACT_AGGRESSIVE);
                                lightTwin->RemoveUnitMovementFlag(MOVEMENTFLAG_JUMPING);
                                lightTwin->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                    lightTwin->AI()->AttackStart(target);
                            }
                            if (Creature* darkTwin = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_DARK_TWIN)))
                            {
                                darkTwin->SetReactState(REACT_AGGRESSIVE);
                                darkTwin->RemoveUnitMovementFlag(MOVEMENTFLAG_JUMPING);
                                darkTwin->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                if (Unit* target = GetRandomPlayerFromMap())
                                    darkTwin->AI()->AttackStart(target);
                            }
                            ++text_nr;
                            timer = 5000;
                        }
                    } else timer -= diff;
                    break;

                case 8:
                    //twins outro
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            switch(pInstance->GetData(DATA_TEAM))
                            {
                            case HORDE:
                                if (Creature* garrosh = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GAROSH_HELLSCREAM)))
                                    DoScriptText(SAY_TWINS_OUTRO_H, garrosh);
                                break;
                            case ALLIANCE:
                                if (Creature* varian = ObjectAccessor::GetCreature((*me), pInstance->GetData64(WARIAN_VRYNN)))
                                    DoScriptText(SAY_TWINS_OUTRO_A, varian);
                                break;
                            }

                            if (Creature* gadajacy = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GADAJACY)))
                            {
                                if (!gadajacy->isAlive())
                                    gadajacy->Respawn(true);

                                gadajacy->SetVisible(true);
                            }

                            ++boss_nr;
                            text_nr=0;
                            timer = 5000;
                            started = false;
                            break;
                        }
                    } else timer -= diff;
                    break;

                case 9:
                    //anub intro (Lich King)
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            DoScriptText(SAY_ANUB_INTRO_1, me);

                            for (int i=BOSS_LIGHT_TWIN; i<=BOSS_DARK_TWIN; ++i)
                            {
                                if (Creature* boss = ObjectAccessor::GetCreature((*me), pInstance->GetData64(i)))
                                    boss->DespawnOrUnsummon();
                            }

                            ++text_nr;
                            timer = 20000;
                            break;

                        case 2:
                            if (Creature* lich = me->SummonCreature(NPC_LICH_KING, positionMoveBeasts))
                            {
                                lich->SetReactState(REACT_PASSIVE);
                                lich->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                lich->SetVisible(false);

                                DoScriptText(SAY_ANUB_INTRO_2, lich);
                            }
                            ++text_nr;
                            timer = 2600;
                            break;

                        case 3:
                            if (Creature* portal = me->SummonCreature(22517, positionMoveBeasts, TEMPSUMMON_TIMED_DESPAWN, 30000))
                            {
                                portal->CastSpell(portal, 51807, false);
                                portal->SetDisplayId(17612);
                            }
                            ++text_nr;
                            timer = 1500;
                            break;

                        case 4:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                                lich->SetVisible(true);
                            ++text_nr;
                            timer = 750;
                            break;

                        case 5:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                            {
                                lich->RemoveUnitMovementFlag(lich->GetUnitMovementFlags());
                                lich->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                                lich->SendMovementFlagUpdate();
                                lich->GetMotionMaster()->MovePoint(0, positionCenter);
                            }
                            ++text_nr;
                            timer = 1500;
                            break;

                        case 6:
                            DoScriptText(SAY_ANUB_INTRO_3, me);
                            ++text_nr;
                            timer = 7500;
                            break;

                        case 7:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                            {
                                lich->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
                                DoScriptText(SAY_ANUB_INTRO_4, lich);
                            }
                            ++text_nr;
                            timer = 3000;
                            break;

                        case 8:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                            ++text_nr;
                            timer = 9000;
                            break;

                        case 9:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                                lich->HandleEmoteCommand(EMOTE_ONESHOT_EXCLAMATION);
                            ++text_nr;
                            timer = 4000;
                            break;

                        case 10:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                                lich->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_TALK);
                            ++text_nr;
                            timer = 1500;
                            break;

                        case 11:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                                lich->HandleEmoteCommand(EMOTE_ONESHOT_KNEEL);
                            ++text_nr;
                            timer = 1200;
                            break;

                        case 12:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                            {
                                if (Unit* target = GetRandomPlayerFromMap())
                                    target->CastSpell(target, 68193, false); //destroy floor knockback
                                lich->CastSpell(lich, 68198, false); //frost nova (visual boom!)
                                lich->SetVisible(false);
                            }
                            if (GameObject* floor = me->GetMap()->GetGameObject(pInstance->GetData64(FLOOR)))
                            {
                                floor->SetUInt32Value(GAMEOBJECT_DISPLAYID, 9060);
                                floor->DestroyForNearbyPlayers();
                                floor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_NODESPAWN);
                                floor->SetUInt32Value(GAMEOBJECT_BYTES_1, 8449);
                                floor->Update(0);
                            }

                            ++text_nr;
                            timer = 2000;
                            break;

                        case 13:
                            if (Creature* lich = ObjectAccessor::GetCreature((*me), pInstance->GetData64(LICH_KING)))
                            {
                                DoScriptText(SAY_ANUB_INTRO_5, lich);
                                lich->DespawnOrUnsummon();
                            }

                            ++text_nr;
                            timer = 5000;
                            break;
                        }
                    } else timer -= diff;
                    break;


                case 10:
                    if (timer < diff)
                    {
                        switch (text_nr)
                        {
                        case 1:
                            DoScriptText(SAY_ANUB_OUTRO, me);
                            ++text_nr;
                            timer = 10000;
                            break;

                        case 2:
                            ++text_nr;
                            if (pInstance->GetData(DATA_ATTEMPTS) > 1 && (getDifficulty() == 2 || getDifficulty() == 3))
                                me->MonsterYell("Let me hand you the chests as a reward, and let its contents will serve you faithfully in the campaign against Arthas in the heart of the Icecrown Citadel!", 0, 0);
                            break;
                        }
                    } else timer -= diff;
                    break;
                }//end switch (boss_nr)
            }//end if (started)
            else
                timer = 5000;


            if (!started)
            {
                if (Creature* gadajacy = ObjectAccessor::GetCreature((*me), pInstance->GetData64(GADAJACY)))
                {
                    if (!gadajacy->isAlive())
                        gadajacy->Respawn(true);

                    gadajacy->SetVisible(true);
                }
            }

        }//end UpdateAI


        void UpdateBosses()
        {
            if (!started)
                return;

            if (boss_nr == 0)
                boss_nr = 1;

            if (pInstance->GetBossState(DATA_TWIN_VALKYR) == FAIL)
            {
                boss_nr = 7;
                text_nr = 0;
                timer = 3000;
                started = false;
                return;
            }

            if (pInstance->GetBossState(DATA_FACTION_CHAMPIONS) == FAIL)
            {
                boss_nr = 5;
                text_nr = 0;
                timer = 3000;
                started = false;
                return;
            }

            if (pInstance->GetBossState(DATA_LORD_JARAXXUS) == FAIL)
            {
                boss_nr = 3;
                text_nr = 0;
                timer = 3000;
                started = false;
                return;
            }

            if (pInstance->GetBossState(DATA_BEASTS) == FAIL)
            {
                boss_nr = 1;
                text_nr = 0;
                timer = 3000;

                gormokDefeted = false;
                jormungarsDefeted = false;

                started = false;
                return;
            }

            if (pInstance->GetBossState(DATA_BEASTS) == DONE && boss_nr < 2)
            {
                gormokDefeted = true;
                jormungarsDefeted = true;

                boss_nr=2;
                text_nr=0;
            }

            if (!gormokDefeted)
            {
                Creature* gormok = ObjectAccessor::GetCreatureOrPetOrVehicle((*me), pInstance->GetData64(BOSS_GORMOK));
                if (gormok && !gormok->isAlive())
                {
                    gormokDefeted = true;
                    heroBeastsTimer = 180000;
                }
            }
            if (!jormungarsDefeted)
            {
                Creature* dreadscale = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_DREADSCALE));
                Creature* acidmaw = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_ACIDMAW));
                if ((dreadscale && !dreadscale->isAlive()) && (acidmaw && !acidmaw->isAlive()))
                    jormungarsDefeted = true;
            }
            if (boss_nr == 1 && jormungarsDefeted)
            {
                Creature* icehowl = ObjectAccessor::GetCreature((*me), pInstance->GetData64(BOSS_ICEHOWL));
                if (icehowl && !icehowl->isAlive())
                {
                    if (boss_nr < 2)
                    {
                        boss_nr=2;
                        text_nr = 0;
                    }
                }
            }

            if (pInstance->GetBossState(DATA_LORD_JARAXXUS) == DONE && boss_nr < 4)
            {
                boss_nr = 4;
                text_nr = 0;
                timer = 5000;
            }

            if (pInstance->GetBossState(DATA_FACTION_CHAMPIONS) == DONE && boss_nr < 6)
            {
                boss_nr = 6;
                text_nr = 0;
                timer = 3000;
            }

            if (pInstance->GetBossState(DATA_TWIN_VALKYR) == DONE  && boss_nr < 8)
            {
                boss_nr = 8;
                text_nr = 0;
                timer = 3000;
            }

            if (pInstance->GetBossState(DATA_ANUBARAK) == DONE  && boss_nr < 10)
            {
                boss_nr = 10;
                text_nr = 0;
                timer = 3000;

                me->Relocate(positionTirionRelocate);
                me->SummonGameObject(191164, 677.135498f, 130.135345f, 142.123535f, 6.128760f, 0, 0, 0, 0, 0);
            }
        }




        void SummonChampions()
        {
            uint32 team = pInstance->GetData(DATA_TEAM);

            uint8 count;

            uint8 healers = 0;
            uint8 meleedps = 0;
            uint8 rangedps = 0;


            if (getDifficulty() == RAID_DIFFICULTY_10MAN_NORMAL || getDifficulty() == RAID_DIFFICULTY_10MAN_HEROIC)
                count = 6;
            else
                count = 10;


            std::vector<uint32> ids;
            for (int i=0; i<count; ++i)
            {
                uint32 id = 0;

                if (i == 0 || i == 1 || id == 6)
                {
                    do
                    {
                        uint32 random = urand(0, 3);
                        if (random == 0)
                            id = (team == ALLIANCE ? NPC_DRZEWO_HORDE : NPC_DRZEWO_ALLY);
                        else if (random == 1)
                            id = (team == ALLIANCE ? NPC_HOLYPALA_HORDE : NPC_HOLYPALA_ALLY);
                        else if (random == 2)
                            id = (team == ALLIANCE ? NPC_RESTOSH_HORDE : NPC_RESTOSH_ALLY);
                        else
                            id = (team == ALLIANCE ? NPC_DISCI_HORDE : NPC_DISCI_ALLY);

                    }while (HasData(ids, id));

                    ids.push_back(id);
                }



                if (i == 2 || i == 3 || i == 7)
                {
                    do
                    {
                        uint32 random = urand(0, 4);
                        if (random == 0)
                            id = (team == ALLIANCE ? NPC_DK_HORDE : NPC_DK_ALLY);
                        else if (random == 1)
                            id = (team == ALLIANCE ? NPC_RETRIPALA_HORDE : NPC_RETRIPALA_ALLY);
                        else if (random == 2)
                            id = (team == ALLIANCE ? NPC_ROGAL_HORDE : NPC_ROGAL_ALLY);
                        else if (random == 3)
                            id = (team == ALLIANCE ? NPC_ENH_HORDE : NPC_ENH_ALLY);
                        else
                            id = (team == ALLIANCE ? NPC_WAREK_HORDE : NPC_WAREK_ALLY);

                    }while (HasData(ids, id));

                    ids.push_back(id);
                }



                if (i == 4 || i == 5 || i == 8)
                {
                    do
                    {
                        uint32 random = urand(0, 4);
                        if (random == 0)
                            id = (team == ALLIANCE ? NPC_SOWA_HORDE : NPC_SOWA_ALLY);
                        else if (random == 1)
                            id = (team == ALLIANCE ? NPC_HUNTER_HORDE : NPC_HUNTER_ALLY);
                        else if (random == 2)
                            id = (team == ALLIANCE ? NPC_MAGE_HORDE : NPC_MAGE_ALLY);
                        else if (random == 3)
                            id = (team == ALLIANCE ? NPC_SHADOW_HORDE : NPC_SHADOW_ALLY);
                        else
                            id = (team == ALLIANCE ? NPC_LOCK_HORDE : NPC_LOCK_ALLY);

                    }while (HasData(ids, id));

                    ids.push_back(id);
                }


                if (i == 9)
                {
                    uint32 randomchampion = urand(0,2);
                    if (randomchampion == 0)
                    {
                        do
                        {
                            uint32 random = urand(0, 3);
                            if (random == 0)
                                id = (team == ALLIANCE ? NPC_DRZEWO_HORDE : NPC_DRZEWO_ALLY);
                            else if (random == 1)
                                id = (team == ALLIANCE ? NPC_HOLYPALA_HORDE : NPC_HOLYPALA_ALLY);
                            else if (random == 2)
                                id = (team == ALLIANCE ? NPC_RESTOSH_HORDE : NPC_RESTOSH_ALLY);
                            else
                                id = (team == ALLIANCE ? NPC_DISCI_HORDE : NPC_DISCI_ALLY);

                        }while (HasData(ids, id));

                        ids.push_back(id);
                    }
                    else if (randomchampion == 1)
                    {
                        do
                        {
                            uint32 random = urand(0, 4);
                            if (random == 0)
                                id = (team == ALLIANCE ? NPC_DK_HORDE : NPC_DK_ALLY);
                            else if (random == 1)
                                id = (team == ALLIANCE ? NPC_RETRIPALA_HORDE : NPC_RETRIPALA_ALLY);
                            else if (random == 2)
                                id = (team == ALLIANCE ? NPC_ROGAL_HORDE : NPC_ROGAL_ALLY);
                            else if (random == 3)
                                id = (team == ALLIANCE ? NPC_ENH_HORDE : NPC_ENH_ALLY);
                            else
                                id = (team == ALLIANCE ? NPC_WAREK_HORDE : NPC_WAREK_ALLY);

                        }while (HasData(ids, id));

                        ids.push_back(id);
                    }
                    else
                    {
                        do
                        {
                            uint32 random = urand(0, 4);
                            if (random == 0)
                                id = (team == ALLIANCE ? NPC_SOWA_HORDE : NPC_SOWA_ALLY);
                            else if (random == 1)
                                id = (team == ALLIANCE ? NPC_HUNTER_HORDE : NPC_HUNTER_ALLY);
                            else if (random == 2)
                                id = (team == ALLIANCE ? NPC_MAGE_HORDE : NPC_MAGE_ALLY);
                            else if (random == 3)
                                id = (team == ALLIANCE ? NPC_SHADOW_HORDE : NPC_SHADOW_ALLY);
                            else
                                id = (team == ALLIANCE ? NPC_LOCK_HORDE : NPC_LOCK_ALLY);

                        }while (HasData(ids, id));

                        ids.push_back(id);
                    }
                }//end if (id==9)
            }//end for (i=0; i<count; ++i)


            for (int i=0; i<(int)ids.size(); ++i)
            {
                if (team == ALLIANCE)
                    me->SummonCreature(ids[i], HordeChampionsEnter[i]);
                else if (team == HORDE)
                    me->SummonCreature(ids[i], AllyChampionsEnter[i]);
            }
        }

        bool HasData(const std::vector<uint32> vec, uint32 data)
        {
            for (int i=0; i<(int)vec.size(); ++i)
            {
                if (vec[i] == data)
                    return true;
            }

            return false;
        }

        Unit* GetRandomPlayerFromMap()
        {
            if (!me->GetMap()->HavePlayers())
                return NULL;

            if (me->GetMap()->GetPlayersCountExceptGMs() == 0)
                return NULL;

            std::vector<uint64> players;
            Map::PlayerList::const_iterator itr = me->GetMap()->GetPlayers().begin();
            while (itr != me->GetMap()->GetPlayers().end())
            {
                if (itr->getSource() && itr->getSource()->isAlive())
                    players.push_back(itr->getSource()->GetGUID());

                ++itr;
            }

            if (players.empty())
                return NULL;

            return ObjectAccessor::GetUnit((*me), players[rand()%players.size()]);
        }
    };

};

class toc_barrett_ramsey : public CreatureScript
{
public:
    toc_barrett_ramsey() : CreatureScript("toc_barrett_ramsey") { }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
    {
        InstanceScript* pInstance = pCreature->GetInstanceScript();
        if (!pInstance)
        {
            pCreature->MonsterSay("Brak instancji", LANG_UNIVERSAL, 0);
            return false;
        }

        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            if (Creature* tirion = ObjectAccessor::GetCreatureOrPetOrVehicle((*pCreature), pInstance->GetData64(TIRION_FORDING)))
                CAST_AI(toc_tirion_fordring::toc_tirion_fordringAI, tirion->AI())->started = true;

            for (int i = DATA_BEASTS; i<DATA_ANUBARAK; ++i)
            {
                if (pInstance->GetBossState(i) == FAIL)
                {
                    pInstance->SetBossState(i, NOT_STARTED);
                    break;
                }
            }

            pCreature->SetVisible(false);
            break;

        case GOSSIP_ACTION_INFO_DEF+2:
            pCreature->MonsterSay("Хорошо, я дождусь вашего лидера.", LANG_UNIVERSAL, pPlayer->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+3:
            pCreature->MonsterSay("Надеюсь, статистика верна. Хорошего дня.", LANG_UNIVERSAL, pPlayer->GetGUID());
            break;

        default:
            pCreature->MonsterSay("Wybrano niezdefiniowana opcje", LANG_UNIVERSAL, 0);
        }

        pPlayer->CLOSE_GOSSIP_MENU();

        return true;
    }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if ((!pPlayer->GetGroup() || !pPlayer->GetGroup()->IsLeader(pPlayer->GetGUID())) && !pPlayer->isGameMaster())
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Извини, я не яляюсь лидером рейда.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(13691, pCreature->GetGUID());
            return true;
        }

        InstanceScript* instance = pCreature->GetInstanceScript();
        if (!instance)
            return false;

        if (instance->GetBossState(DATA_TWIN_VALKYR) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Мы одержали победу над Валь'кирами-близнецами!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }
        else if (instance->GetBossState(DATA_FACTION_CHAMPIONS) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Мы одержали победу над чемпионами фракций!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }
        else if (instance->GetBossState(DATA_LORD_JARAXXUS) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Мы одержали победу над лордом Джараксусом!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }
        else if (instance->GetBossState(DATA_BEASTS) == DONE)
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Мы одержали победу над Нордскольскими чудовищами!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }
        else
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Мы готовы к началу сотязания!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        }


        if (pPlayer->isGameMaster())
        {
            std::stringstream str;
            str << "Deaths Count: ";
            str << instance->GetData(DATA_DEATHS_COUNT);
            str << ", Attempts remaning: ";
            str << instance->GetData(DATA_ATTEMPTS);
            str << ", Played as: ";
            str << (instance->GetData(DATA_TEAM) == ALLIANCE ? "alliance" : instance->GetData(DATA_TEAM) == HORDE ? "horde" : "<error: team not defined>");
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, str.str().c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
        }

        pPlayer->SEND_GOSSIP_MENU(13691, pCreature->GetGUID());
        return true;
    }
};

void AddSC_trial_of_the_crusader()
{
    new toc_tirion_fordring();
    new toc_barrett_ramsey();
}