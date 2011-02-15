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

#include "ScriptPCH.h"
#include "icecrown_citadel.h"
#include "Spell.h"

#define GOSSIP_TEXT(id) sObjectMgr->GetGossipText(id)->Options[0].Text_0
#define IS_TELEPORT_ENABLED(id) ((go->GetMap()->GetGameObject(instance->GetData64(id))->GetGoState() == GO_STATE_ACTIVE) ? true: false)
enum Spells
{
    SPELL_ON_ORGRIMS_HAMMER_DECK    = 70121, //maybe for gunship battle
    SPELL_DARKMARTYRDOM_FANATIC     = 71236,
    SPELL_DARKMARTYRDOM_ADHERENT    = 70903,
    SPELL_DARKTRANSFORMATION        = 70900,
    SPELL_NECROTICSTRIKE            = 70659,
    SPELL_SHADOWCLEAVE              = 70670,
    SPELL_VAMPIRICMIGHT             = 70674,
    SPELL_FANATIC_DETERMINATION     = 71235,
    SPELL_ADHERENT_DETERMINATION    = 71234,
    SPELL_EMPOWEREMENT              = 70901,
    SPELL_FROST_FEVER               = 67767,
    SPELL_DEATHCHILL_BLAST          = 70906,
    SPELL_DEATHCHILL_BOLT           = 70594,
    SPELL_CURSE_OF_TORPOR           = 71237,
    SPELL_SHORUD_OF_THE_OCCULT      = 70768,
    SPELL_DARK_TRANSFORMATION_T     = 70895,
    SPELL_DARK_EMPOWERMENT_T        = 70896,
    SPELL_STANDART_HORDE            = 69811,
    SPELL_COLDFLAME_JETS            = 70460,
    SPELL_SPIRIT_ALARM_1            = 70536,
    SPELL_SPIRIT_ALARM_2            = 70545,
    SPELL_SPIRIT_ALARM_3            = 70546,
    SPELL_SPIRIT_ALARM_4            = 70547,

};

enum TeleportSpells
{
    HAMMER        = 70781,
    ORATORY       = 70856,
    RAMPART       = 70857,
    SAURFANG      = 70858,
    UPPER_SPIRE   = 70859,
    //PLAGUEWORKS   = 9995,
    //CRIMSONHALL   = 9996,
    //FWHALLS       = 9997,
    SINDRAGOSA    = 70861,
    LICHKING      = 70860
};
enum eTeleportGossips
{
    GOSSIP_TELEPORT_LIGHTS_HAMMER           = 800000,
    GOSSIP_TELEPORT_ORATORY_OF_THE_DAMNED   = 800001,
    GOSSIP_TELEPORT_RAMPART_OF_SKULLS       = 800002,
    GOSSIP_TELEPORT_DEATHBRINGERS_RISE      = 800003,
    GOSSIP_TELEPORT_UPPER_SPIRE             = 800004,
    GOSSIP_TELEPORT_SINDRAGOSAS_LAIR        = 800005,
    GOSSIP_TELEPORT_FROZEN_THRONE           = 800006
};
enum eTrapEvents
{
    EVENT_COLDFLAME_JETS = 1,
    EVENT_REMOVE_COLDFLAME_JETS
};
enum eTrapActions
{
    ACTION_DISARM_FROST_TRAP = 2
};
//class npc_cult_fanatic_and_adherent : public CreatureScript
//{
//    public:
//        npc_cult_fanatic_and_adherent() : CreatureScript("npc_cult_fanatic_and_adherent") { }
//
//        struct npc_cult_fanatic_and_adherentAI : public ScriptedAI
//        {
//            npc_cult_fanatic_and_adherentAI(Creature* creature) : ScriptedAI(creature) { }
//
//            void Reset()
//            {
//                uiStrikeTimer = urand(8000, 11000);
//                uiVampirTimer = urand(19000, 22000);
//                uiCleaveTimer = urand(7000, 9000);
//                uiMartyrdomTimer = urand(24000, 30000);
//                uiFrostFeverTimer = urand(9000, 11000);
//                uiDeathchillTimer = urand(10000, 12000);
//                uiCurseTimer = urand(8000, 10000);
//                uiOccultTimer = urand(25000, 29000);
//
//                bEmpowerement = false;
//
//                //if(me->GetEntry() == CREATURE_REANIMATED_ADHERENT)
//                //    DoCast(me, SPELL_ADHERENT_DETERMINATION);
//                //if(me->GetEntry() == CREATURE_REANIMATED_FANATIC)
//                //    DoCast(me, SPELL_FANATIC_DETERMINATION);
//            }
//
//            void JustDied(Unit* /*who*/) { }
//
//            void SpellHit(Unit* /*caster*/, const SpellEntry * spell)
//            {
//                if(me->GetEntry() == CREATURE_ADHERENT)
//                {
//                    if (spell->Id == SPELL_EMPOWEREMENT)
//                        me->UpdateEntry(CREATURE_EMPOWERED_ADHERENT);
//                    else if (spell->Id == SPELL_DARK_EMPOWERMENT_T)
//                    {
//                        DoCast(me, SPELL_EMPOWEREMENT);
//                        bEmpowerement = true;
//                    }
//                }
//                if(me->GetEntry() == CREATURE_FANATIC)
//                {
//                    if (spell->Id == SPELL_DARKTRANSFORMATION)
//                        me->UpdateEntry(CREATURE_DEFORMED_FANATIC);
//                    else if (spell->Id == SPELL_DARK_TRANSFORMATION_T)
//                    {
//                        DoCast(me, SPELL_DARKTRANSFORMATION);
//                    }
//                }
//            }
//
//            void UpdateAI(const uint32 uiDiff)
//            {
//                if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
//                    return;
//
//                if((me->GetEntry() == CREATURE_ADHERENT) || (me->GetEntry() == CREATURE_EMPOWERED_ADHERENT) || (me->GetEntry() == CREATURE_REANIMATED_ADHERENT))
//                {
//                    if (uiMartyrdomTimer <= uiDiff && !bEmpowerement)
//                    {
//                        DoCast(me, SPELL_DARKMARTYRDOM_ADHERENT);
//                        uiMartyrdomTimer = urand(24000, 30000);
//                    } else uiMartyrdomTimer -= uiDiff;
//
//                    if (uiFrostFeverTimer <= uiDiff)
//                    {
//                        DoCast(me->getVictim(), SPELL_FROST_FEVER);
//                        uiFrostFeverTimer = urand(9000, 11000);
//                    } else uiFrostFeverTimer -= uiDiff;
//
//                    if (uiDeathchillTimer <= uiDiff)
//                    {
//                        if (me->GetEntry() == CREATURE_EMPOWERED_ADHERENT)
//                            DoCast(me->getVictim(), SPELL_DEATHCHILL_BLAST);
//                        else
//                            DoCast(me->getVictim(), SPELL_DEATHCHILL_BOLT);
//                        uiDeathchillTimer = urand(10000, 12000);
//                    } else uiDeathchillTimer -= uiDiff;
//
//                    if (uiCurseTimer <= uiDiff)
//                    {
//                        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 1))
//                            DoCast(target, SPELL_CURSE_OF_TORPOR);
//                        uiCurseTimer = urand(8000, 10000);
//                    } else uiCurseTimer -= uiDiff;
//
//                    if (uiOccultTimer <= uiDiff)
//                    {
//                        DoCast(me, SPELL_SHORUD_OF_THE_OCCULT);
//                        uiOccultTimer = urand(25000, 29000);
//                    } else uiOccultTimer -= uiDiff;
//                }
//                if((me->GetEntry() == CREATURE_FANATIC) || (me->GetEntry() == CREATURE_REANIMATED_FANATIC) || (me->GetEntry() == CREATURE_DEFORMED_FANATIC))
//                {
//                    if (uiMartyrdomTimer <= uiDiff)
//                    {
//                        DoCast(me, SPELL_DARKMARTYRDOM_FANATIC);
//                        uiMartyrdomTimer = urand(24000, 30000);
//                    } else uiMartyrdomTimer -= uiDiff;
//
//                    if (uiStrikeTimer <= uiDiff)
//                    {
//                        DoCast(me, SPELL_NECROTICSTRIKE);
//                        uiStrikeTimer = urand(8000, 11000);
//                    } else uiStrikeTimer -= uiDiff;
//
//                    if (uiCleaveTimer <= uiDiff)
//                    {
//                        DoCast(me, SPELL_SHADOWCLEAVE);
//                        uiCleaveTimer = urand(7000, 9000);
//                    } else uiCleaveTimer -= uiDiff;
//
//                    if (uiVampirTimer <= uiDiff)
//                    {
//                        DoCast(me, SPELL_VAMPIRICMIGHT);
//                        uiVampirTimer = urand(19000, 22000);
//                    } else uiVampirTimer -= uiDiff;
//                }
//
//                DoMeleeAttackIfReady();
//            }
//
//        private:
//            uint32 uiStrikeTimer;
//            uint32 uiCleaveTimer;
//            uint32 uiVampirTimer;
//            uint32 uiMartyrdomTimer;
//            uint32 uiFrostFeverTimer;
//            uint32 uiDeathchillTimer;
//            uint32 uiCurseTimer;
//            uint32 uiOccultTimer;
//
//            bool bEmpowerement;
//        };
//
//        CreatureAI* GetAI(Creature* creature) const
//        {
//            return new npc_cult_fanatic_and_adherentAI(creature);
//        }
//};
class go_icecrown_teleporter : public GameObjectScript
{
    public:
        go_icecrown_teleporter() : GameObjectScript("go_icecrown_teleporter") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            InstanceScript* instance = go->GetInstanceScript();
            if(!instance)
                return false;
            if (instance->IsEncounterInProgress())
                return false;
            if (go->GetEntry() == LICH_TELEPORT)
            {
                if (instance->GetData(DATA_PROFESSOR_PUTRICIDE_EVENT) == DONE && instance->GetData(DATA_BLOOD_QUEEN_LANA_THEL_EVENT) == DONE && instance->GetData(DATA_SINDRAGOSA_EVENT) == DONE)
                {
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_FROZEN_THRONE), GOSSIP_SENDER_MAIN, LICHKING);
                    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
                    return true;
                }
                return false;
            }
            instance->HandleGameObject(NULL, true, go);
            if (go->GetEntry() != FIRST_TELEPORT)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_LIGHTS_HAMMER), GOSSIP_SENDER_MAIN, HAMMER);
            
            if (go->GetEntry() != LORD_TELEPORT && instance->GetData(DATA_MARROWGAR_EVENT) == DONE && IS_TELEPORT_ENABLED(DATA_TELEPORT_ORATORY_OF_THE_DAMNED))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_ORATORY_OF_THE_DAMNED), GOSSIP_SENDER_MAIN, ORATORY);
            }
            if (go->GetEntry() != GUNSHIP_TELEPORT && instance->GetData(DATA_DEATHWHISPER_EVENT) == DONE && IS_TELEPORT_ENABLED(DATA_TELEPORT_RAMPART_OF_SKULLS))
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_RAMPART_OF_SKULLS), GOSSIP_SENDER_MAIN, RAMPART);
            }
            if (go->GetEntry() != SAURFANG_TELEPORT
             && instance->GetData(DATA_GUNSHIP_BATTLE_EVENT) == DONE
             && instance->GetData(DATA_DEATHWHISPER_EVENT) == DONE
             //&& IS_TELEPORT_ENABLED(DATA_TELEPORT_DEATHBRINGERS_RISE) //Disabled until Gunship Battle encounter is implemented
             )
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_DEATHBRINGERS_RISE), GOSSIP_SENDER_MAIN, SAURFANG);
            }
            if (go->GetEntry() != CITADEL_TELEPORT &&
                instance->GetData(DATA_SAURFANG_EVENT) == DONE && IS_TELEPORT_ENABLED(DATA_TELEPORT_UPPER_SPIRE))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_UPPER_SPIRE), GOSSIP_SENDER_MAIN, UPPER_SPIRE);
            if (go->GetEntry() != SINDRAGOSA_TELEPORT &&
                instance->GetData(DATA_VALITHRIA_DREAMWALKER_EVENT) == DONE && IS_TELEPORT_ENABLED(DATA_TELEPORT_SINDRAGOSAS_LAIR))
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TEXT(GOSSIP_TELEPORT_SINDRAGOSAS_LAIR), GOSSIP_SENDER_MAIN, SINDRAGOSA);

            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 uiSender, uint32 uiAction)
        {
            //player->PlayerTalkClass->ClearMenus();
            if(!player->getAttackers().empty())
                return false;

            SpellEntry const* spell = sSpellStore.LookupEntry(uiAction);
            if (!spell)
                return false;

            if (player->isInCombat())
            {
                Spell::SendCastResult(player, spell, 0, SPELL_FAILED_AFFECTING_COMBAT);
                return true;
            }

            if (uiSender == GOSSIP_SENDER_MAIN)
                player->CastSpell(player, spell, true);

            return true;
        }
};

class spell_icc_spirit_alarm : public SpellScriptLoader
{
    public:
        spell_icc_spirit_alarm() : SpellScriptLoader("spell_icc_spirit_alarm") { }

        class spell_icc_spirit_alarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_icc_spirit_alarm_SpellScript);

            void AwakenDeathboundWard(SpellEffIndex effIndex)
            {
                if (!GetTargetUnit() || GetTargetUnit()->GetTypeId() != TYPEID_PLAYER)
                    return;
                if (!GetCaster())
                    return;
                InstanceScript *instance = GetCaster()->GetInstanceScript();
                GameObject *spiritAlarm = NULL;
                Creature *pWard = NULL;
                switch (GetSpellInfo()->Id)
                {
                    case SPELL_SPIRIT_ALARM_1:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM1));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD1));
                        //Preload Spirit Alarm traps near Lord Marrowgar
                        spiritAlarm->GetMap()->LoadGrid(-273.845f, 2220.22f);
                        spiritAlarm->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM3))->SetPhaseMask(1, true);
                        break;
                    case SPELL_SPIRIT_ALARM_2:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM2));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD2));
                        //Preload Spirit Alarm traps near Lord Marrowgar
                        spiritAlarm->GetMap()->LoadGrid(-273.845f, 2220.22f);
                        spiritAlarm->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM4))->SetPhaseMask(1, true);
                        break;
                    case SPELL_SPIRIT_ALARM_3:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM3));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD3));
                        break;
                    case SPELL_SPIRIT_ALARM_4:
                        spiritAlarm = GetCaster()->GetMap()->GetGameObject(instance->GetData64(DATA_SPIRIT_ALARM4));
                        pWard = spiritAlarm->GetMap()->GetCreature(instance->GetData64(DATA_DEATHBOUND_WARD4));
                        break;
                }
                spiritAlarm->setActive(false);
                if (pWard && pWard->isAlive())
                {
                    pWard->SetReactState(REACT_AGGRESSIVE);
                    pWard->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    pWard->AI()->AttackStart(GetTargetUnit());
                }
            }
            void Register()
            {
                OnEffect += SpellEffectFn(spell_icc_spirit_alarm_SpellScript::AwakenDeathboundWard, EFFECT_2, SPELL_EFFECT_SEND_EVENT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_icc_spirit_alarm_SpellScript();
        }
};

class npc_frost_freeze_trap : public CreatureScript
{
    public:
        npc_frost_freeze_trap() : CreatureScript("npc_frost_freeze_trap") { }

        struct npc_frost_freeze_trapAI : public Scripted_NoMovementAI
        {
            npc_frost_freeze_trapAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_DISARM_FROST_TRAP:
                        events.CancelEvent(EVENT_COLDFLAME_JETS);
                        break;
                    default:
                        Scripted_NoMovementAI::DoAction(action);
                        break;
                }
            }

            void Reset()
            {
                //Let's give players an opportunity to pass trap within 1.5 seconds every 5 seconds
                events.Reset();
                events.ScheduleEvent(EVENT_COLDFLAME_JETS, 1000);
                events.ScheduleEvent(EVENT_REMOVE_COLDFLAME_JETS, 6000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_COLDFLAME_JETS:
                        {
                            DoCast(me, SPELL_COLDFLAME_JETS);
                            events.ScheduleEvent(EVENT_COLDFLAME_JETS, 5000);
                            break;
                        }
                        case EVENT_REMOVE_COLDFLAME_JETS:
                        {
                            me->RemoveAurasDueToSpell(SPELL_COLDFLAME_JETS);
                            events.ScheduleEvent(EVENT_REMOVE_COLDFLAME_JETS, 5000);
                            break;
                        }

                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frost_freeze_trapAI(creature);
        }
};

class spell_coldflame_trap : public SpellScriptLoader
{
    public:
        spell_coldflame_trap() : SpellScriptLoader("spell_coldflame_trap") { }

        class spell_coldflame_trap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_coldflame_trap_SpellScript);

            void FilterTargets(std::list<Unit*>& unitList)
            {
                std::list<Unit*> nonTrapped;
                if (Unit *pCaster = GetCaster())
                {
                    float x, y, z, angle, ux, uy;
                    pCaster->GetPosition(x, y, z, angle);
                    float opX = x + 25.0f * cos(angle);
                    float opY = y + 25.0f * sin(angle);
                    for (std::list<Unit*>::iterator it = unitList.begin(); it != unitList.end(); ++it)
                    {
                        Unit *curUnit = *it;
                        float unitRadius = curUnit->GetObjectSize();
                        ux = curUnit->GetPositionX();
                        uy = curUnit->GetPositionY();
                        if (   ux - unitRadius > std::max(x, opX)
                            || ux + unitRadius < std::min(x, opX)
                            || uy - unitRadius > std::max(y, opY)
                            || uy + unitRadius < std::min(y, opY))
                        {
                            nonTrapped.push_back(curUnit);
                            continue;
                        }

                        float angle = pCaster->GetAngle(curUnit) - pCaster->GetAngle(opX, opY);
                        bool inBetween = abs(sin(angle)) * curUnit->GetExactDist2d(pCaster) < unitRadius;
                        if (!inBetween)
                            nonTrapped.push_back(curUnit);
                    }
                }
                for (std::list<Unit*>::iterator it = nonTrapped.begin(); it != nonTrapped.end(); ++it)
                    unitList.erase(std::find(unitList.begin(), unitList.end(), *it));
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_coldflame_trap_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_AREA_PATH);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_coldflame_trap_SpellScript();
        }
};

void AddSC_icecrown_citadel()
{
    //new npc_cult_fanatic_and_adherent();
    new go_icecrown_teleporter();
    new npc_frost_freeze_trap();
    new spell_icc_spirit_alarm();
    new spell_coldflame_trap();
}
