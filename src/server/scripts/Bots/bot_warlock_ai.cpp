#include "ScriptPCH.h"
#include "bot_warlock_ai.h"
#include "Group.h"

class warlock_bot : public CreatureScript
{
public:
    warlock_bot() : CreatureScript("warlock_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new warlock_botAI(pCreature);
    }

	struct warlock_botAI : public bot_ai
	{
		warlock_botAI(Creature *c) :bot_ai(c)
		{
			Reset();
			pet = NULL;
		}

		bool oom_spam;

		uint8 state;
		uint8 next_state;
		uint32 next_state_timer;

		Creature *pet;
		Unit *opponent;

		void Reset()
		{
			FirstAid_cd = 0;
			GC_Timer = 0;

			conflagarate_cd = 0;
			chaos_bolt_cd = 0;

			oom_spam = false;

			uint8 state = 1;
			next_state = 0;
			next_state_timer = 0;

			opponent = NULL;

			if (master) {
				setStats(CLASS_WARLOCK, m_creature->getRace(), master->getLevel());

				if (!m_creature->HasAura(56235)) m_creature->AddAura(56235,m_creature);  // Glyph of Conflagrate
				if (!m_creature->HasAura(63302)) m_creature->AddAura(63302,m_creature);  // Glyph of Haunt
				if (!m_creature->HasAura(17834)) m_creature->AddAura(17834,m_creature);  // Improved Immolation
				if (!m_creature->HasAura(17814)) m_creature->AddAura(17814,m_creature);  // Improved Corruption

			}
		}

		void CreatePet()
		{
			pet = master->GetBotsPet(60237);

			if(pet == NULL)
				return;

			pet->UpdateCharmAI();
			pet->setFaction(m_creature->getFaction());
			pet->SetReactState(REACT_DEFENSIVE);
			pet->GetMotionMaster()->MoveFollow(m_creature, PET_FOLLOW_DIST*urand(1, 2),PET_FOLLOW_ANGLE);
			CharmInfo *charmInfonewbot = pet->InitCharmInfo();
			pet->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW);
			pet->UpdateStats(STAT_STRENGTH);
			pet->UpdateStats(STAT_AGILITY);
			pet->SetLevel(master->getLevel());

			float val2 = master->getLevel()*4.0f + pet->GetStat(STAT_STRENGTH)*2.0f;

			val2=100.0;
			uint32 attPowerMultiplier=1;
			pet->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, uint32(val2));
			pet->UpdateAttackPowerAndDamage();
			pet->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, uint32(val2 * attPowerMultiplier));
			pet->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, uint32(val2 * attPowerMultiplier)*2+master->getLevel());
			pet->UpdateDamagePhysical(BASE_ATTACK);

		}

		void UpdateAI(const uint32 diff)
		{

			ReduceCD();

			if(IAmDead()) return;

			if(pet && pet != NULL && pet->isDead())
			{
				master->SetBotsPetDied();
				pet = NULL;
			}

			//if we think we have a pet, but master doesn't, it means we teleported
			if(pet && master->m_botHasPet == false)
			{
				master->SetBotsPetDied();
				pet = NULL;
			}

			m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
			m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

			if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.3 && isTimerReady(Potion_cd))
			{
				doCast(m_creature, HEALINGPOTION);
				Potion_cd = Potion_cd;
			}
			if(m_creature->GetPower(POWER_MANA) < m_creature->GetMaxPower(POWER_MANA)*0.2)
			{
				if(isTimerReady(Potion_cd))
				{
					doCast(m_creature, MANAPOTION);
					//MonsterSay("MANA POTION", LANG_UNIVERSAL, NULL);
					Potion_cd = Potion_cd;
				} else {
					if(oom_spam == false)
					{
						//MonsterSay("OOM", LANG_UNIVERSAL, NULL);
						oom_spam = true;
					}
					ScriptedAI::UpdateAI(diff);
					//return; //can't do anything without mana
			   }
			}
			oom_spam = false;

			ScriptedAI::UpdateAI(diff);

			opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
			if(!opponent && !m_creature->getVictim())
			{
				ResetOrGetNextTarget();

				//to reduce the number of crashes, remove pet whenever we are not in combat
				if(pet != NULL && pet->isAlive())
				{
					master->SetBotsPetDied();
					pet = NULL;
				}

				return;
			}

			if(pet == NULL)
				CreatePet();

			if (pet && pet->isAlive() &&
				!pet->isInCombat() &&
				m_creature->getVictim()) {
				pet->Attack (m_creature->getVictim(), true);
				pet->GetMotionMaster()->MoveChase(m_creature->getVictim(), 1, 0);
			}

			if(m_creature->HasUnitState(UNIT_STAT_CASTING))
				return;

			DoNormalAttack(diff);
		}

		void Aggro(Unit *who){}

		void JustDied(Unit *Killer)
		{
			master->SetBotCommandState(COMMAND_FOLLOW);
			state = 1;
			next_state = 0;
			next_state_timer = 0;
			if(pet && pet != NULL)
			{
				master->SetBotsPetDied();
				pet = NULL;
			}
		}

		void AttackStart(Unit *u)
		{
			Aggro(u);
			ScriptedAI::AttackStartCaster(u, 25);
		}

		void KilledUnit(Unit *)
		{
			((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
			if(pet && pet->isAlive()) pet->GetMotionMaster()->MoveFollow(m_creature, PET_FOLLOW_DIST*urand(1, 2), PET_FOLLOW_ANGLE);
		}

		void DoNormalAttack(const uint32 diff)
		{
			AttackerSet m_attackers = master->getAttackers();
			if(opponent == NULL) return;
			if(opponent->isDead()) return;

			//double check that pet didn't just die
			if(pet && pet != NULL && pet->isDead())
			{
				master->SetBotsPetDied();
				pet = NULL;
			}

			//send in the pet
			if(pet && pet != NULL && pet->isAlive() && !pet->isInCombat()) pet->AI()->AttackStart(opponent);

			if(!isTimerReady(GC_Timer)) return;

			if(opponent->HasUnitMovementFlag(UNIT_FLAG_FLEEING))
			{
				//MonsterSay("Mob is fleeing!", LANG_UNIVERSAL, NULL);
				return;
			}


			//if(RAIN_OF_FIRE && m_attackers.size() > 1)
			//{
				//doCast(opponent, RAIN_OF_FIRE);
				//return;
			//}
			if(CURSE_OF_THE_ELEMENTS && !HasAuraName(opponent, CURSE_OF_THE_ELEMENTS))
			{
				doCast(opponent, CURSE_OF_THE_ELEMENTS);
				GiveManaBack();
				//return;
			}

			if(CORRUPTION && !HasAuraName (opponent, CORRUPTION, m_creature->GetGUID()))
			{
				doCast(opponent, CORRUPTION);
				GiveManaBack();
				//return;
			}

			if(HAUNT && !HasAuraName (opponent, HAUNT, m_creature->GetGUID()))
			{
				doCast(opponent, HAUNT);
				GiveManaBack();
				return;
			}

			if(UNSTABLE_AFFLICTION && !HasAuraName (opponent, UNSTABLE_AFFLICTION, m_creature->GetGUID()))
			{
				doCast(opponent, UNSTABLE_AFFLICTION);
				GiveManaBack();
				return;
			}

			if(!HasAuraName(opponent, IMMOLATE, m_creature->GetGUID()))
			{
				doCast(opponent, IMMOLATE);
				GiveManaBack();
				return;
			} else if(CONFLAGRATE && isTimerReady(conflagarate_cd))
			{
				doCast(opponent, CONFLAGRATE);
				conflagarate_cd = CONFLAGRATE_CD;
				GiveManaBack();
				return;
			}

			if(CHAOS_BOLT && isTimerReady(chaos_bolt_cd))
			{
				doCast(opponent, CHAOS_BOLT);
				GiveManaBack();
				chaos_bolt_cd = CHAOS_BOLT_CD;
				return;
			} else
				doCast(opponent, SHADOW_BOLT);
			GiveManaBack();
		} //DoNormalAttack

		void ReduceCD()
		{
			if(Potion_cd > 0)       --Potion_cd;
			if(FirstAid_cd > 0)     --FirstAid_cd;
			if(GC_Timer > 0)        --GC_Timer;

			if(conflagarate_cd > 0) --conflagarate_cd;
			if(chaos_bolt_cd > 0)   --chaos_bolt_cd;
		}
	};
};

void AddSC_warlock_bot()
{
    new warlock_bot();
}
