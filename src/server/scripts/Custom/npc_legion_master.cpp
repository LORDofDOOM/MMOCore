#include "ScriptPCH.h"

enum Phases
{
    PHASE_ONE           = 1,
    PHASE_TWO           = 2,
    PHASE_THREE         = 3,
    PHASE_FOUR          = 4,
    PHASE_FIVE          = 5,

    PHASE_ONE_MASK      = 1 << PHASE_ONE,
    PHASE_TWO_MASK      = 1 << PHASE_TWO,
    PHASE_THREE_MASK    = 1 << PHASE_THREE,
    PHASE_FOUR_MASK     = 1 << PHASE_FOUR
};

enum Events
{
    EVENT_BERSERK               = 1,
    EVENT_DEATH_AND_DECAY       = 2,
    EVENT_SUMMON_SNOBOLD_VASSAL = 3,
    EVENT_DOMINATE_MIND         = 4,

    // Phase two only
    EVENT_P2_CHILLING_WAVE      = 5,

    // Phase three only
    EVENT_P3_TWILIGHT_BLOODBOLT = 6,

    // Phase four only
    EVENT_P4_BLIZZARD           = 7,
    EVENT_P4_MANA_DETONATION    = 8,

    // Phase five only
    EVENT_P5_FURY               = 9,
    EVENT_P5_STUNNING_FORCE     = 10,
    EVENT_P5_FLASH_HEAL         = 11
};

enum Spells
{
    SPELL_BERSERK               = 47008,
    SPELL_RISING_ANGER          = 66636,
    SPELL_DEATH_AND_DECAY       = 71001,
    SPELL_DOMINATE_MIND         = 71289,
    SPELL_INCITE_TERROR         = 73070,
    SPELL_CHILLING_WAVE         = 68778,
    SPELL_TWILIGHT_BLOODBOLT    = 71446,
    SPELL_BLIZZARD              = 71118,
    SPELL_STUNNING_FORCE        = 52402,
    SPELL_MANA_DETONATION       = 27819,
    SPELL_FLASH_HEAL            = 71783,
    SPELL_CLEAR_DEBUFFS         = 34098,
    SPELL_REPELLING_WAVE        = 74509,
    SPELL_FURY                  = 66721,
    SPELL_FRENZY                = 47774
};

enum Summons
{
    NPC_SNOBOLD_VASSAL          = 34800
};

enum Models
{
    MODEL_BOY                   = 338,
    MODEL_MONSTER               = 20736
};

#define LM_YELL_AGGRO       "Sendet sie zur Hölle!"
#define LM_YELL_KILL        "Stirb!!"
#define LM_YELL_DIE         "Oh nein... ich... sterbe..."
#define LM_YELL_BERSERK     "Spürt meine Wut!"
#define LM_YELL_ENFURECIDO  "Genug ist genug!!"

class npc_legion_master : public CreatureScript
{
    public:
        npc_legion_master() : CreatureScript("npc_legion_master") {}

        struct npc_legion_masterAI : public WorldBossAI
        {
            npc_legion_masterAI(Creature* creature) : WorldBossAI(creature) {}

            void Reset()
            {
                _Reset();

                me->RemoveAurasDueToSpell(SPELL_BERSERK);
                me->RemoveAurasDueToSpell(SPELL_FURY);

                me->SetDisplayId(MODEL_BOY);
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);

                me->SetReactState(REACT_DEFENSIVE);

                events.SetPhase(PHASE_ONE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                me->SetDisplayId(MODEL_MONSTER);
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.2f);

                me->MonsterYell(LM_YELL_AGGRO, LANG_UNIVERSAL, 0);

                events.Reset();
                events.SetPhase(PHASE_ONE);
                // Phase independent events
                events.ScheduleEvent(EVENT_BERSERK, 600000);
                events.ScheduleEvent(EVENT_DEATH_AND_DECAY, 10000);
                events.ScheduleEvent(EVENT_SUMMON_SNOBOLD_VASSAL, 20000);
                events.ScheduleEvent(EVENT_DOMINATE_MIND, 30000);
            }

            void EnterEvadeMode()
            {
                ScriptedAI::EnterEvadeMode();
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    me->MonsterYell(LM_YELL_KILL, LANG_UNIVERSAL, 0);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->MonsterYell(LM_YELL_DIE, LANG_UNIVERSAL, 0);
                _JustDied();
            }

            void CastInciteTerror()
            {
                DoCast(me, SPELL_INCITE_TERROR);
            }

            void DamageTaken(Unit* /*done_by*/, uint32& /*damage*/)
            {
                if (events.GetPhaseMask() & PHASE_ONE_MASK && !HealthAbovePct(76))
                {
                    CastInciteTerror();
                    events.SetPhase(PHASE_TWO);
                    events.ScheduleEvent(EVENT_P2_CHILLING_WAVE, 5000, 0, PHASE_TWO);
                    return;
                }

                if (events.GetPhaseMask() & PHASE_TWO_MASK && !HealthAbovePct(51))
                {
                    CastInciteTerror();
                    events.SetPhase(PHASE_THREE);
                    events.ScheduleEvent(EVENT_P3_TWILIGHT_BLOODBOLT, 5000, 0, PHASE_THREE);
                    return;
                }

                if (events.GetPhaseMask() & PHASE_THREE_MASK && !HealthAbovePct(26))
                {
                    CastInciteTerror();
                    events.SetPhase(PHASE_FOUR);
                    events.ScheduleEvent(EVENT_P4_BLIZZARD, 5000, 0, PHASE_FOUR);
                    events.ScheduleEvent(EVENT_P4_MANA_DETONATION, 6000, 0, PHASE_FOUR);
                    return;
                }

                if (events.GetPhaseMask() & PHASE_FOUR_MASK && !HealthAbovePct(11))
                {
                    me->MonsterYell(LM_YELL_ENFURECIDO, LANG_UNIVERSAL, 0);
                    DoCast(me, SPELL_CLEAR_DEBUFFS);
                    DoCast(me, SPELL_REPELLING_WAVE);
                    events.SetPhase(PHASE_FIVE);
                    events.RescheduleEvent(EVENT_SUMMON_SNOBOLD_VASSAL, 5000);
                    events.ScheduleEvent(EVENT_P5_FURY, 6000, 0, PHASE_FIVE);
                    events.ScheduleEvent(EVENT_P5_STUNNING_FORCE, 10000, 0, PHASE_FIVE);
                    events.ScheduleEvent(EVENT_P5_FLASH_HEAL, 5000, 0, PHASE_FIVE);
                    return;
                }
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);

                summon->CastSpell(summon, SPELL_FRENZY, true);
                summon->CastSpell(summon, SPELL_FURY, true);

                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                {
                    if (summon->GetEntry() == NPC_SNOBOLD_VASSAL)
                    {
                        summon->GetMotionMaster()->MoveJump(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 10.0f, 20.0f);
                        DoCast(me, SPELL_RISING_ANGER);
                    }
                    summon->AI()->AttackStart(target);
                }
            }

            void ExecuteEvent(uint32 const eventId)
            {
                switch (eventId)
                {
                    case EVENT_BERSERK:
                        events.Reset();
                        me->MonsterYell(LM_YELL_BERSERK, LANG_UNIVERSAL, 0);
                        DoCast(me, SPELL_BERSERK);
                        break;
                    case EVENT_DEATH_AND_DECAY:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_DEATH_AND_DECAY);
                        events.ScheduleEvent(EVENT_DEATH_AND_DECAY, urand(10000, 12000));
                        break;
                    case EVENT_SUMMON_SNOBOLD_VASSAL:
                        me->SummonCreature(NPC_SNOBOLD_VASSAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON_SNOBOLD_VASSAL, 20000);
                        break;
                    case EVENT_DOMINATE_MIND:
                    {
                        for (uint8 i = 0; i < 3; i++)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, -SPELL_DOMINATE_MIND))
                                DoCast(target, SPELL_DOMINATE_MIND);
                        events.ScheduleEvent(EVENT_DOMINATE_MIND, urand(40000, 45000));
                        break;
                    }
                    case EVENT_P2_CHILLING_WAVE:
                        DoCast(me, SPELL_CHILLING_WAVE);
                        events.ScheduleEvent(EVENT_P2_CHILLING_WAVE, urand(20000, 30000), 0, PHASE_TWO);
                        break;
                    case EVENT_P3_TWILIGHT_BLOODBOLT:
                    {
                        std::list<Unit*> targets;
                        SelectTargetList(targets, 5, SELECT_TARGET_RANDOM, 100, true);
                        for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            DoCast(*itr, SPELL_TWILIGHT_BLOODBOLT);
                        events.ScheduleEvent(EVENT_P3_TWILIGHT_BLOODBOLT, urand(15000, 25000), 0, PHASE_THREE);
                        break;
                    }
                    case EVENT_P4_BLIZZARD:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_BLIZZARD);
                        events.ScheduleEvent(EVENT_P4_BLIZZARD, urand(20000, 25000), 0, PHASE_FOUR);
                        break;
                    case EVENT_P4_MANA_DETONATION:
                    {
                        // Select a random target in the threat list that has mana
                        std::vector<Unit*> unitList;
                        std::list<HostileReference*>& threatList = me->getThreatManager().getThreatList();
                        for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                        {
                            if (Unit* target = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                                if (target->GetCreateMana() > 0) // target has mana
                                    unitList.push_back(target);
                        }

                        if (!unitList.empty())
                        {
                            std::vector<Unit*>::const_iterator itr = unitList.begin();
                            advance(itr, rand() % unitList.size());
                            DoCast(*itr, SPELL_MANA_DETONATION);
                        }

                        events.ScheduleEvent(EVENT_P4_MANA_DETONATION, urand(20000, 40000), 0, PHASE_FOUR);
                        break;
                    }
                    case EVENT_P5_FURY:
                        if (urand(0, 4) == 0) // 20% chance to increase damage
                            me->CastSpell(me, SPELL_FURY, true);
                        events.ScheduleEvent(EVENT_P5_FURY, urand(4000, 6000), 0, PHASE_FIVE);
                        break;
                    case EVENT_P5_STUNNING_FORCE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            me->CastSpell(target, SPELL_STUNNING_FORCE, true);
                        events.ScheduleEvent(EVENT_P5_STUNNING_FORCE, urand(8000, 12000), 0, PHASE_FIVE);
                        break;
                    case EVENT_P5_FLASH_HEAL:
                        if (urand(0, 1) == 0) // 50% chance to heal
                            me->CastSpell(me, SPELL_FLASH_HEAL, true);
                        events.ScheduleEvent(EVENT_P5_FLASH_HEAL, 5000, 0, PHASE_FIVE);
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                    ExecuteEvent(eventId);

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_legion_masterAI(creature);
        }
};

void AddSC_npc_legion_master()
{
    new npc_legion_master();
}