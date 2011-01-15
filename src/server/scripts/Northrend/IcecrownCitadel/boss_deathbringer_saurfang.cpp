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

#include "ScriptPCH.h"
#include "icecrown_citadel.h"

enum Yells
{
    SAY_AGGRO                       = -1631045,
    SAY_MARK_OF_THE_FALLEN_CHAMPION = -1631046,
    SAY_BLOOD_BEASTS                = -1631047,
    SAY_KILL_1                      = -1631048,
    SAY_KILL_2                      = -1631049,
    SAY_FRENZY                      = -1631050,
    SAY_BERSERK                     = -1631051,
    SAY_DEATH                       = -1631052,

    SAY_INTRO_ALLIANCE_1            = -1631029,
    SAY_INTRO_ALLIANCE_2            = -1631030,
    SAY_INTRO_ALLIANCE_3            = -1631031,
    SAY_INTRO_ALLIANCE_4            = -1631032,
    SAY_INTRO_ALLIANCE_5            = -1631033,
    SAY_INTRO_ALLIANCE_6            = -1631034,
    SAY_INTRO_ALLIANCE_7            = -1631035,

    SAY_INTRO_HORDE_1               = -1631036,
    SAY_INTRO_HORDE_2               = -1631037,
    SAY_INTRO_HORDE_3               = -1631038,
    SAY_INTRO_HORDE_4               = -1631039,
    SAY_INTRO_HORDE_5               = -1631040,
    SAY_INTRO_HORDE_6               = -1631041,
    SAY_INTRO_HORDE_7               = -1631042,
    SAY_INTRO_HORDE_8               = -1631043,
    SAY_INTRO_HORDE_9               = -1631044,

    SAY_OUTRO_ALLIANCE_1            = -1631053,
    SAY_OUTRO_ALLIANCE_2            = -1631054,
    SAY_OUTRO_ALLIANCE_3            = -1631055,
    SAY_OUTRO_ALLIANCE_4            = -1631056,
    SAY_OUTRO_ALLIANCE_5            = -1631057,
    SAY_OUTRO_ALLIANCE_6            = -1631058,
    SAY_OUTRO_ALLIANCE_7            = -1631059,
    SAY_OUTRO_ALLIANCE_8            = -1631060,
    SAY_OUTRO_ALLIANCE_9            = -1631061,
    SAY_OUTRO_ALLIANCE_10           = -1631062,
    SAY_OUTRO_ALLIANCE_11           = -1631063,
    SAY_OUTRO_ALLIANCE_12           = -1631064,
    SAY_OUTRO_ALLIANCE_13           = -1631065,
    SAY_OUTRO_ALLIANCE_14           = -1631066,
    SAY_OUTRO_ALLIANCE_15           = -1631067,
    SAY_OUTRO_ALLIANCE_16           = -1631068,
    SAY_OUTRO_ALLIANCE_17           = -1631069,
    SAY_OUTRO_ALLIANCE_18           = -1631070,
    SAY_OUTRO_ALLIANCE_19           = -1631071,
    SAY_OUTRO_ALLIANCE_20           = -1631072,
    SAY_OUTRO_ALLIANCE_21           = -1631073,

    SAY_OUTRO_HORDE_1               = -1631074,
    SAY_OUTRO_HORDE_2               = -1631075,
    SAY_OUTRO_HORDE_3               = -1631076,
    SAY_OUTRO_HORDE_4               = -1631077
};

enum Spells
{
    SPELL_BLOOD_LINK            = 72178,
    SPELL_BLOOD_POWER           = 72371,
    SPELL_FRENZY                = 72737,
    SPELL_TASTE_OF_BLOOD        = 69634,
    SPELL_FALLEN_CHAMPION       = 72293,
    SPELL_FALLEN_CHAMPION_AURA  = 72256,
    SPELL_BOILING_BLOOD         = 72385,
    SPELL_BLOOD_NOVA            = 72378,
    SPELL_RUNE_OF_BLOOD_AURA    = 72408,
    SPELL_RUNE_OF_BLOOD         = 72410,
    SPELL_BLOOD_LINK_BEAST      = 72176,
    SPELL_RESISTAN_SKIN         = 72723,
    SPELL_SCENT_OF_BLOOD        = 72769,
    SPELL_AGONY                 = 70572, //saurfang event
    SPELL_ZERO_POWER            = 72242
};

static const uint32 summonBloodBeast[5] = {72172, 72173, 72356, 72357, 72358};

class boss_saurfang : public CreatureScript
{
    public:
        boss_saurfang() : CreatureScript("boss_saurfang") { }

        struct boss_saurfangAI : public BossAI
        {
            boss_saurfangAI(Creature* creature) : BossAI(creature, DATA_SAURFANG_EVENT), vehicle(creature->GetVehicleKit())
            {
                instance = me->GetInstanceScript();
                assert(vehicle);
                bFrenzy = false;
                me->CastSpell(me, SPELL_ZERO_POWER, true);
                me->SetPower(me->getPowerType(), 0);
            }

            void Reset()
            {
                uiBoilingBloodTimer = 25000;
                uiBloodNovaTimer = 30000;
                uiRuneOfBloodTimer = 20000;
                uiSummonBloodBeastTimer = 40000;
                uiBerserkTimer = 480000;

                uiMessCount = 0;

                me->GetVehicleKit();

                if (instance)
                    instance->SetData(DATA_SAURFANG_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                if(!instance)
                    return;

                DoScriptText(SAY_AGGRO, me);

                DoCast(me, SPELL_BLOOD_LINK, true);
                DoCast(me, SPELL_BLOOD_POWER, true);
                DoCast(me, SPELL_FALLEN_CHAMPION_AURA, true);
                DoCast(me, SPELL_RUNE_OF_BLOOD_AURA, true);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FALLEN_CHAMPION);
                instance->SetData(DATA_SAURFANG_EVENT, IN_PROGRESS);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!instance)
                    return;

                DoScriptText(SAY_DEATH, me);

                instance->SetData(DATA_SAURFANG_EVENT, DONE);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FALLEN_CHAMPION);
                me->RemoveAurasDueToSpell(SPELL_FALLEN_CHAMPION_AURA);
                me->RemoveAurasDueToSpell(SPELL_RUNE_OF_BLOOD_AURA);
                me->SetPower(me->getPowerType(), 0);

                if(uiMessCount < RAID_MODE(3,5,3,5))
                    instance->DoCompleteAchievement(RAID_MODE(ACHIEV_MESS_10, ACHIEV_MESS_25));

            }

            void JustReachedHome()
            {
                if (!instance)
                    return;

                instance->SetData(DATA_SAURFANG_EVENT, FAIL);
                instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FALLEN_CHAMPION);
                me->RemoveAurasDueToSpell(SPELL_BLOOD_LINK);
                me->RemoveAurasDueToSpell(SPELL_BLOOD_POWER);
                me->RemoveAurasDueToSpell(SPELL_FALLEN_CHAMPION_AURA);
                me->RemoveAurasDueToSpell(SPELL_RUNE_OF_BLOOD_AURA);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_KILL_1,SAY_KILL_2), me);
            }

            void JustSummoned(Creature* summoned)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 1, 100.0f, true))
                    summoned->AI()->AttackStart(target);

                summoned->CastSpell(summoned, SPELL_BLOOD_LINK_BEAST, true);
                summoned->CastSpell(summoned, SPELL_RESISTAN_SKIN, true);

                if (IsHeroic())
                    summoned->CastSpell(summoned, SPELL_SCENT_OF_BLOOD, true);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if(!UpdateVictim())
                    return;

                if (uiBoilingBloodTimer < uiDiff)
                {
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                        DoCast(target, SPELL_BOILING_BLOOD);
                    uiBoilingBloodTimer = 25000;
                } else uiBoilingBloodTimer -= uiDiff;

                if (uiBloodNovaTimer < uiDiff)
                {
                    for (uint8 i = 1; i <= RAID_MODE(1,3,1,3); ++i)
                        if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_BLOOD_NOVA);
                    uiBloodNovaTimer = 30000;
                } else uiBloodNovaTimer -= uiDiff;

                if (uiSummonBloodBeastTimer <= uiDiff)
                {
                    for(uint8 i = 0; i < RAID_MODE(2,5,2,5); ++i)
                        DoCast(me, summonBloodBeast[i]);
                    DoScriptText(SAY_BLOOD_BEASTS, me);
                    uiSummonBloodBeastTimer = 40000;
                } else uiSummonBloodBeastTimer -= uiDiff;

                if (me->GetPower(POWER_ENERGY) > 99)
                {
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true, -SPELL_FALLEN_CHAMPION))
                    {
                        DoCast(target, SPELL_FALLEN_CHAMPION);
                        DoScriptText(SAY_MARK_OF_THE_FALLEN_CHAMPION, me);
                        me->SetPower(me->getPowerType(), 0);
                        ++uiMessCount;
                    }
                }

                if (uiRuneOfBloodTimer < uiDiff)
                {
                    DoCastVictim( SPELL_RUNE_OF_BLOOD);
                    uiRuneOfBloodTimer = 20000;
                } else uiRuneOfBloodTimer -= uiDiff;

                if(me->GetHealth()*100 / me->GetMaxHealth() < 31)
                {
                    if (!bFrenzy)
                    {
                        DoCast(me, SPELL_FRENZY);
                        DoScriptText(SAY_FRENZY, me);
                        bFrenzy = true;
                    }
                }

                if (uiBerserkTimer < uiDiff)
                {
                    DoCast(SPELL_BERSERK);
                    DoScriptText(SAY_BERSERK, me);
                    uiBerserkTimer = 480000;
                } else uiBerserkTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;

            Vehicle* vehicle;

            uint32 uiBoilingBloodTimer;
            uint32 uiBloodNovaTimer;
            uint32 uiRuneOfBloodTimer;
            uint32 uiSummonBloodBeastTimer;
            uint32 uiBerserkTimer;

            uint8 uiMessCount;

            bool bFrenzy;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_saurfangAI(creature);
        }
};

void AddSC_boss_saurfang()
{
    new boss_saurfang();
}