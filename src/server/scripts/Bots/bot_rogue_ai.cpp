#include "ScriptPCH.h"
#include "bot_rogue_ai.h"
#include "Group.h"

class rogue_bot : public CreatureScript
{
public:
    rogue_bot() : CreatureScript("rogue_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new rogue_botAI(pCreature);
    }

struct rogue_botAI : public bot_ai
{
    rogue_botAI(Creature *c) : bot_ai(c)
    {
        Reset();
    }

    int32 GC_Timer;                // global cooldown
    int32 BS_Timer;
    int32 SinisterStrike_Timer;
    int32 Eviscerate_Timer;
    int32 SliceDice_Timer;
    int32 Rupture_Timer;
    int32 Kick_Timer;
    int32 Poison_Timer;
    int32 Potion_Timer;
    int32 Shadowstep_Timer;
    int32 Mutilate_Timer;

    uint8 energy;
    uint8 comboPoints;

    Unit *opponent;

    void Reset()
    {
        GC_Timer = 0;
        BS_Timer = 50;
        Mutilate_Timer = 0;
        SinisterStrike_Timer = 30;
        Eviscerate_Timer = 90;
        SliceDice_Timer = 75;
        Rupture_Timer = 80;
        Kick_Timer = 20;
        Poison_Timer = 15;
        Potion_Timer=0;
        Shadowstep_Timer = 0;
        comboPoints = 0;
        energy = 100;

        opponent = NULL;

        m_creature->setPowerType(POWER_ENERGY);
        m_creature->SetMaxPower(POWER_ENERGY, 100);
        m_creature->SetPower(POWER_ENERGY, 100);

        if (master) {
            //        if (!m_creature->HasAura(61331)) m_creature->AddAura(61331,m_creature);  // Aggression
            //         if (!m_creature->HasAura(14137)) m_creature->AddAura(14137,m_creature);  // Lethality
            //          if (!m_creature->HasAura(14166)) m_creature->AddAura(14166,m_creature);  // Improved Slice and Dice
            //           if (!m_creature->HasAura(14164)) m_creature->AddAura(14164,m_creature);  // Improved Eviserate

            setStats(CLASS_ROGUE, m_creature->getRace(), master->getLevel());

            if (m_creature->getLevel()>70 && !m_creature->HasAura(37169)) m_creature->AddAura(37169,m_creature);  // Death Mantle item set
        }

    }

    void EnterEvadeMode(){}

    void Aggro(Unit *who){}

    void MoveInLineOfSight(Unit *target){}

    bool isTimerReady(int32 timer)
    {
        if(timer <= 0 && GC_Timer <= 0) return true;
        else                            return false;
    } //end isTimerReady

    void doCast(Unit *victim, uint32 spellId, bool triggered = false)
    {
        if(spellId == 0) return;
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        GC_Timer = 10;
        DoCast(victim, spellId, triggered);
    } //end doCast

    void decrementTimers()
    {
        if(GC_Timer > 0)             --GC_Timer;
        if(BS_Timer > 0)             --BS_Timer;
        if(Kick_Timer > 0)           --Kick_Timer;
        if(SliceDice_Timer > 0)      --SliceDice_Timer;
        if(SinisterStrike_Timer > 0) --SinisterStrike_Timer;
        if(Eviscerate_Timer > 0)     --Eviscerate_Timer;
        if(Rupture_Timer > 0)        --Rupture_Timer;
        if(Poison_Timer > 0)         --Poison_Timer;
        if(Potion_Timer > 0)         --Potion_Timer;
        if(Shadowstep_Timer > 0)     --Shadowstep_Timer;
        if (Mutilate_Timer > 0)            Mutilate_Timer--;
    } //end decrementTImers

    void KilledUnit(Unit *)
    {
        ((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
    }

    void AttackStart(Unit *u)
    {
        Aggro(u);
        ScriptedAI::AttackStart(u);
    }

    void UpdateAI(const uint32 diff)
    {
        decrementTimers();

        if(IAmDead()) return;

        opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
        if(!opponent && !m_creature->getVictim())
        {
            comboPoints = 0;
            energy = 100;
            ResetOrGetNextTarget();
            return;
        }

        m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

        energy += 5;
        if(comboPoints > 5) comboPoints = 5;

        //interrupt any spells
        if(opponent->IsNonMeleeSpellCasted(true) && energy >= 25)
        {
            if(isTimerReady(Kick_Timer))
            {
                doCast(opponent, KICK);
                Kick_Timer = 100;
                energy -= 25;
            }
            else if(comboPoints > 0)
            {
                switch(comboPoints)
                {
                    case 1: doCast(opponent, KIDNEY_SHOT_1); --comboPoints; break;
                    case 2: doCast(opponent, KIDNEY_SHOT_2); comboPoints-=2; break;
                    default: doCast(opponent, KIDNEY_SHOT_3); comboPoints-=3; break;
                }
                energy -= 25;
            }
        }

        if(!opponent->isInFrontInMap(m_creature, 5) && isTimerReady(BS_Timer))
        {
            doCast(opponent, BACKSTAB);
            BS_Timer = 50;
            energy -= 60;
        }
        else if(isTimerReady(Shadowstep_Timer) && energy > 10 && m_creature->GetDistance(opponent) < 25)
        {
            //doCast(opponent, SHADOWSTEP);
            Shadowstep_Timer = 300;
            energy -= 10;

            //NPCs can't really shadowstep so fake it
            float x = opponent->GetPositionX();
            float y = opponent->GetPositionY();
            float z = opponent->GetPositionZ();
            float o = opponent->GetOrientation();

            m_creature->Relocate(x-4, y-4, z, o);
            m_creature->SendMonsterMoveWithSpeed(x-4, y-4, 1, 0);
            doCast(opponent, BACKSTAB);
            return;
        }

        if(isTimerReady(SliceDice_Timer) && comboPoints > 0 && !m_creature->HasAura(SLICE_DICE))
        {
            doCast(opponent, SLICE_DICE);

            // since npcs don't use combo points, they can only case
            // first level of spell.  So only remove 1 combo point
            --comboPoints;

            SliceDice_Timer = 75;
            energy -= 25;
        }

        if (isTimerReady(Mutilate_Timer) &&
            energy>60) {
            // TODO: calculate correct dmg for mutilate (dont forget poison bonus)
            // for now use same formula as evicerate
            uint32 base_attPower = m_creature->GetUInt32Value(UNIT_FIELD_ATTACK_POWER);
            //float minDmg = m_creature->GetFloatValue(UNIT_FIELD_MINDAMAGE);
            float minDmg = m_creature->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
            int damage = irand(int32(base_attPower*5*0.03f),int32(base_attPower*5*0.07f))+minDmg+m_creature->getLevel();

            // compensate for lack of attack power
            damage = damage*(rand()%4+1);

            m_creature->CastCustomSpell(opponent, MUTILATE, &damage, NULL, NULL, false, NULL, NULL);

            //doCast (m_creature, MUTILATE);
            Mutilate_Timer = 75;
            ++comboPoints;
            energy -= 60;
        }

        if (isTimerReady(Eviscerate_Timer) && comboPoints>0) {
            uint32 base_attPower = m_creature->GetUInt32Value(UNIT_FIELD_ATTACK_POWER);
            //float minDmg = m_creature->GetFloatValue(UNIT_FIELD_MINDAMAGE);
            float minDmg = m_creature->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
            int damage = irand(int32(base_attPower*5*0.03f),int32(base_attPower*5*0.07f))+minDmg+m_creature->getLevel();
//sLog->outError ("EVISCERTE: base_attPower = %u", base_attPower);
//sLog->outError("\tminDmg = %f", minDmg);
//sLog->outError("\tmaxDmg = %f", m_creature->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE));
//sLog->outError ("\tMINDAMAGE after setting it = %f", m_creature->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE));

// compensate for lack of attack power
//sLog->outError ("\tdamage before = %u", damage);
            damage = damage*(rand()%4+1);
//sLog->outError ("\tdamage after = %u", damage);
            // Eviscerate and Envenom Bonus Damage (Deathmantle item set effect)
            if(m_creature->HasAura(37169))
                damage += comboPoints*40;

            m_creature->CastCustomSpell(opponent, EVISCERATE, &damage, NULL, NULL, false, NULL, NULL);
            //doCast(opponent, EVISCERATE);
            comboPoints=0;

            energy -= 30;
            Eviscerate_Timer = 90;
         //   return;
        }

        if(isTimerReady(SinisterStrike_Timer) && comboPoints < 5)
        {
            doCast(opponent, SINISTER_STRIKE);
            //m_creature->Say("sinister strike", LANG_UNIVERSAL, NULL);
            ++comboPoints;
            SinisterStrike_Timer = 20;
            energy -= 40;
        }

        if(isTimerReady(Rupture_Timer))
        {
            doCast(opponent, RUPTURE);
            comboPoints = 0;
            Rupture_Timer = 80;
            energy -= 40;
        }

        /*if(!opponent->HasAuraType(SPELL_AURA_MOD_DISARM))
            doCast(opponent, DISMANTLE);*/

        /*
            since npc can't really use poison, we'll pretend that we were able to poison his blades.
        */
        if(isTimerReady(Poison_Timer))
        {
            //Deadly Poison has 40% chance of proccing
            switch(rand()%5)
            {
                case 0:
                case 1:
                case 2:
                    break;
                case 4:
                case 5:
                    if(DEADLY_POISON) DoCast(opponent, DEADLY_POISON, true); break;
            }

            //Wound Poison has 50% chance of procing
            switch(rand()%2)
            {
                case 0:
                     break;
                case 1:
                    if(WOUND_POISON) DoCast(opponent, WOUND_POISON, true); break;
                    break;
            }

            Poison_Timer = 15;
        }

        //if low on health, drink a potion
        if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.6 && isTimerReady(Potion_Timer))
        {
            doCast(m_creature, HEALINGPOTION);
            Potion_Timer = 1500;
        }

        m_creature->SetPower(POWER_ENERGY, energy);

        ScriptedAI::UpdateAI(diff);
    } //end UpdateAI


}; //end rogue_bot
};

void AddSC_rogue_bot()
{
    new rogue_bot();
}
