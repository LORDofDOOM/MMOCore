/*
 * Copyright (C) 2010 Myth <http://code.google.com/p/mythcore/>
 *
 * Copyright (C) 2008-2010 Trinity <http://www.trinitycore.org/>
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

/*
SQL:
UPDATE `creature_template` SET `ScriptName` = 'npc_NerubarBroodkeeper' WHERE `entry` = 36725;
UPDATE `creature_template` SET `ScriptName` = 'npc_TheDamned' WHERE `entry` = 37011;
UPDATE `creature_template` SET `ScriptName` = 'npc_ServantoftheThrone' WHERE `entry` = 36724;
UPDATE `creature_template` SET `ScriptName` = 'npc_AncientSkeletalSoldier' WHERE `entry` = 37012;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathboundWard' WHERE `entry` = 37007;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerAttedant' WHERE `entry` = 36811;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerDisciple' WHERE `entry` = 36807;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerHighPriest' WHERE `entry` = 36829;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerServant' WHERE `entry` = 36805;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerZealot' WHERE `entry` = 36808;
*/

#include "ScriptPCH.h"
#include "icecrown_citadel.h"

/***************************************SPELLS*************************************/
//Nerub'ar Broodkeeper
#define SPELL_CRYPTSCARABS                70965
#define SPELL_DARKMENDING                 71020
#define SPELL_WEBWRAP                     70980

//The Damned
#define SPELL_BONEFLURRY                  70960
#define SPELL_SHATTEREDBONES              70961

//Servant of the Throne
#define SPELL_GLACIALBLAST                71029

//Ancient Skeletal Soldier
#define SPELL_SHIELDBASH                  70964

//Deathbound Ward
#define SPELL_DISRUPTINGSHOUT             71022
#define SPELL_SABERLASH                   71021

//Deathspeaker Attendant
#define SPELL_SHADOWBOLT                  69387
#define SPELL_SHADOWNOVA                  RAID_MODE(69355,71106,69355,71106)

//Deathspeaker Disciple
#define SPELL_DARKBLESSING                69391
#define SPELL_SHADOWBOLT2                 69387
#define SPELL_SHADOWMEND                  RAID_MODE(69389,71107,69389,71107)

//Deathspeaker High Priest
#define SPELL_AURAOFDARKNESS              69491
#define SPELL_DARKRECKONING               69483
#define SPELL_DARKRECKONING_EFFECT        69482

//Deathspeaker Servant
#define SPELL_CHAOSBOLT                   RAID_MODE(69576,71108,69576,71108)
#define SPELL_CONSUMINGSHADOWS            69405
#define SPELL_CURSEOFAGONY                RAID_MODE(69404,71112,69404,71112)

//Deathspeaker Zealot
#define SPELL_SHADOWCLEAVE                69492

/****************************************EVENTS************************************/
//Nerub'ar Broodkeeper
#define EVENT_CRYPTSCARABS          1
#define EVENT_DARKMENDING           2
#define EVENT_WEBWRAP               3

//The Damned
#define EVENT_BONEFLURRY            4
#define EVENT_SHATTEREDBONES        5

//Servant of the Throne
#define EVENT_GLACIALBLAST          6

//Ancient Skeletal Soldier
#define EVENT_SHIELDBASH            7

//Deathbound Ward
#define EVENT_DISRUPTINGSHOUT       8
#define EVENT_SABERLASH             9

//Deathspeaker Attendant
#define EVENT_SHADOWBOLT            10
#define EVENT_SHADOWNOVA            11

//Deathspeaker Disciple
#define EVENT_SHADOWBOLT2           12
#define EVENT_DARKBLESSING          13
#define EVENT_SHADOWMEND            14

//Deathspeaker High Priest
#define EVENT_DARKRECKONING         15
#define EVENT_DARKRECKONING_EFFECT  16

//Deathspeaker Servant
#define EVENT_CHAOSBOLT             17
#define EVENT_CONSUMINGSHADOWS      18
#define EVENT_CURSEOFAGONY          19

//Deathspeaker Zealot
#define EVENT_SHADOWCLEAVE          20

/*****************************MARROWGAR TRASH****************************/
class npc_NerubarBroodkeeper : public CreatureScript
{
public:
    npc_NerubarBroodkeeper() : CreatureScript("npc_NerubarBroodkeeper") { }
    struct npc_NerubarBroodkeeperAI: public ScriptedAI
    {
        npc_NerubarBroodkeeperAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_CRYPTSCARABS, 10000);
            events.ScheduleEvent(EVENT_DARKMENDING, 17800);
            events.ScheduleEvent(EVENT_WEBWRAP, 12000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_CRYPTSCARABS:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_CRYPTSCARABS);
                        events.RescheduleEvent(EVENT_CRYPTSCARABS, 12000);
                        break;
                    case EVENT_DARKMENDING:
                        DoCast(me, SPELL_DARKMENDING);
                        events.RescheduleEvent(EVENT_DARKMENDING, 17800);
                        break;
                    case EVENT_WEBWRAP:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_WEBWRAP);

                        events.RescheduleEvent(EVENT_WEBWRAP, 15000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_NerubarBroodkeeperAI(pCreature);
    }
};

class npc_ServantoftheThrone : public CreatureScript
{
public:
    npc_ServantoftheThrone() : CreatureScript("npc_ServantoftheThrone") { }
    struct npc_ServantoftheThroneAI: public ScriptedAI
    {
        npc_ServantoftheThroneAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_GLACIALBLAST, 13000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_GLACIALBLAST:
                        DoCastAOE(SPELL_GLACIALBLAST);
                        events.RescheduleEvent(EVENT_GLACIALBLAST, 8000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_ServantoftheThroneAI(pCreature);
    }
};

class npc_AncientSkeletalSoldier : public CreatureScript
{
public:
    npc_AncientSkeletalSoldier() : CreatureScript("npc_AncientSkeletalSoldier") { }
    struct npc_AncientSkeletalSoldierAI: public ScriptedAI
    {
        npc_AncientSkeletalSoldierAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_SHIELDBASH, 8000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SHIELDBASH:
                        DoCast(me->getVictim(), SPELL_SHIELDBASH);
                        events.RescheduleEvent(EVENT_SHIELDBASH, 8000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_AncientSkeletalSoldierAI(pCreature);
    }
};

class npc_DeathboundWard : public CreatureScript
{
public:
    npc_DeathboundWard() : CreatureScript("npc_DeathboundWard") { }
    struct npc_DeathboundWardAI: public ScriptedAI
    {
        npc_DeathboundWardAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_DISRUPTINGSHOUT, 8000);
            events.ScheduleEvent(EVENT_SABERLASH, 8000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_DISRUPTINGSHOUT:
                        DoCast(me, SPELL_DISRUPTINGSHOUT);
                        events.RescheduleEvent(EVENT_DISRUPTINGSHOUT, 8000);
                        break;
                    case EVENT_SABERLASH:
                        DoCast(me->getVictim(), SPELL_SABERLASH, true);
                        events.RescheduleEvent(EVENT_SABERLASH, 8000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_DeathboundWardAI(pCreature);
    }
};

class npc_DeathspeakerAttedant : public CreatureScript
{
public:
    npc_DeathspeakerAttedant() : CreatureScript("npc_DeathspeakerAttedant") { }
    struct npc_DeathspeakerAttedantAI: public ScriptedAI
    {
        npc_DeathspeakerAttedantAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_SHADOWBOLT, 8000);
            events.ScheduleEvent(EVENT_SHADOWNOVA, 23000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SHADOWBOLT:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_SHADOWBOLT);

                        events.RescheduleEvent(EVENT_SHADOWBOLT, 8000);
                        break;
                    case EVENT_SHADOWNOVA:
                        DoCastAOE(SPELL_SHADOWNOVA);
                        events.RescheduleEvent(EVENT_SHADOWNOVA, 17000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_DeathspeakerAttedantAI(pCreature);
    }
};

class npc_DeathspeakerDisciple : public CreatureScript
{
public:
    npc_DeathspeakerDisciple() : CreatureScript("npc_DeathspeakerDisciple") { }
    struct npc_DeathspeakerDiscipleAI: public ScriptedAI
    {
        npc_DeathspeakerDiscipleAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_SHADOWBOLT2, 8000);
            events.ScheduleEvent(EVENT_DARKBLESSING, urand(15000, 25000));
            events.ScheduleEvent(EVENT_SHADOWMEND, urand(10000, 20000));
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_DARKBLESSING:
                        if(Unit* pTarget = DoSelectLowestHpFriendly(30.0f,1))
                            DoCast(pTarget, SPELL_DARKBLESSING);

                        events.RescheduleEvent(EVENT_DARKBLESSING, 20000);
                        break;
                    case EVENT_SHADOWBOLT2:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_SHADOWBOLT2);

                        events.RescheduleEvent(EVENT_SHADOWBOLT2, 8000);
                        break;
                    case EVENT_SHADOWMEND:
                        if(Unit* pTarget = DoSelectLowestHpFriendly(40.0f,1))
                            DoCast(pTarget, SPELL_SHADOWMEND);

                        events.RescheduleEvent(EVENT_SHADOWMEND, 25000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_DeathspeakerDiscipleAI(pCreature);
    }
};

class npc_DeathspeakerHighPriest : public CreatureScript
{
public:
    npc_DeathspeakerHighPriest() : CreatureScript("npc_DeathspeakerHighPriest") { }
    struct npc_DeathspeakerHighPriestAI: public ScriptedAI
    {
        npc_DeathspeakerHighPriestAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        uint64 targetGuid;

        void Reset()
        {
            targetGuid = 0;
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_DARKRECKONING, 10000);
            events.ScheduleEvent(EVENT_DARKRECKONING_EFFECT, 19000);
            DoCast(me, SPELL_AURAOFDARKNESS);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_DARKRECKONING:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        {
                            targetGuid = pTarget->GetGUID();
                            DoCast(pTarget, SPELL_DARKRECKONING);
                        }

                        events.RescheduleEvent(EVENT_DARKRECKONING, 20000);
                        break;
                    case EVENT_DARKRECKONING_EFFECT:
                        if (targetGuid)
                            if (Unit *target = me->GetUnit((*me), targetGuid))
                                if (me->IsValidAttackTarget(target))
                                    DoCast(target, SPELL_DARKRECKONING_EFFECT);

                        events.RescheduleEvent(EVENT_DARKRECKONING_EFFECT, 20000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_DeathspeakerHighPriestAI(pCreature);
    }
};

class npc_DeathspeakerServant : public CreatureScript
{
public:
    npc_DeathspeakerServant() : CreatureScript("npc_DeathspeakerServant") { }
    struct npc_DeathspeakerServantAI: public ScriptedAI
    {
        npc_DeathspeakerServantAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_CHAOSBOLT, 15000);
            events.ScheduleEvent(EVENT_CONSUMINGSHADOWS, 13000);
            events.ScheduleEvent(EVENT_CURSEOFAGONY, 10000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_CHAOSBOLT:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                           DoCast(pTarget, SPELL_CHAOSBOLT);

                        events.RescheduleEvent(EVENT_CHAOSBOLT, 15000);
                        break;
                    case EVENT_CONSUMINGSHADOWS:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_CONSUMINGSHADOWS);

                        events.RescheduleEvent(EVENT_CONSUMINGSHADOWS, 13000);
                        break;
                    case EVENT_CURSEOFAGONY:
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(pTarget, SPELL_CURSEOFAGONY);

                        events.RescheduleEvent(EVENT_CURSEOFAGONY, 17000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_DeathspeakerServantAI(pCreature);
    }
};

class npc_DeathspeakerZealot : public CreatureScript
{
public:
    npc_DeathspeakerZealot() : CreatureScript("npc_DeathspeakerZealot") { }
    struct npc_DeathspeakerZealotAI: public ScriptedAI
    {
        npc_DeathspeakerZealotAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_SHADOWCLEAVE, 8000);
        }

        void UpdateAI(const uint32 diff)
        {
             //Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while(uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    case EVENT_SHADOWCLEAVE:
                        DoCast(me->getVictim(), SPELL_SHADOWCLEAVE);
                        events.RescheduleEvent(EVENT_SHADOWCLEAVE, 8000);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_DeathspeakerZealotAI (pCreature);
    }
};

void AddSC_icc_lower_spire_trash()
{
    new npc_NerubarBroodkeeper();
    new npc_ServantoftheThrone();
    new npc_AncientSkeletalSoldier();
    new npc_DeathboundWard();
    new npc_DeathspeakerAttedant();
    new npc_DeathspeakerDisciple();
    new npc_DeathspeakerHighPriest();
    new npc_DeathspeakerServant();
    new npc_DeathspeakerZealot();
}