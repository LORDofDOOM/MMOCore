#include "ScriptPCH.h"
#include "bot_paladin_ai.h"
#include "Group.h"

class paladin_bot : public CreatureScript
{
public:
    paladin_bot() : CreatureScript("paladin_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new paladin_botAI(pCreature);
    }

	struct paladin_botAI : public bot_ai
	{
		paladin_botAI(Creature *c) :bot_ai(c)
		{
			Reset();
		}

		bool oom_spam;

		Unit *opponent;

		int32 LOH_Timer;
		int32 HOJ_Timer;

		void Reset()
		{
			oom_spam = false;

			opponent = NULL;

			GC_Timer = 0;
			LOH_Timer = 0;
			HOJ_Timer = 0;

			if (master) {
				setStats(CLASS_PALADIN, m_creature->getRace(), master->getLevel());
		   }
		}

		bool CureTarget(Unit *target)
		{
			if (!isTimerReady(GC_Timer)) return false;
			if (HasAuraName(target, "Withering Heat"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of withering heat");
				doCast(target, CLEANSE);
			}
			if (HasAuraName(target, "Ancient Dread"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of Ancient Dread");
				doCast(target, CLEANSE);
			}
					if (HasAuraName(target, "Ancient Dread"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of Ancient Dread");
				doCast(target, CLEANSE);
			}
			if (HasAuraName(target, "Arcane Buffet"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of Arcane Buffet");
				doCast(target, CLEANSE);
			}
			if (HasAuraName(target, "Shadow Buffet"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of Shadow Buffet");
				doCast(target, CLEANSE);
			}
			if (HasAuraName(target, "Flame Buffet"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of Flame Buffet");
				doCast(target, CLEANSE);
			}
			if (HasAuraName(target, "Frost Buffet"))
			{
	//sLog->outError ("PaladingBotAI.CureTarget: curing %s of Frost Buffet");
				doCast(target, CLEANSE);
			}
			return true;
		}

		bool HealTarget(Unit *target, uint8 hp)
		{
			if (!isTimerReady(GC_Timer)) return false;
			if (m_creature->IsNonMeleeSpellCasted(true)) return false;
			if(!target || target->isDead()) return false;
			if(hp < 25 && isTimerReady(LOH_Timer))
			{
				// 33% to cast loh, else just do a fast heal
				uint64 m_rand = urand(1, 3);
				switch(m_rand)
				{
					 case 1: {
						std::string loh = "Lay of Hands on ";
						loh += target->GetName();
						loh += ".";

						m_creature->MonsterSay(loh.c_str(), LANG_UNIVERSAL, NULL);

						doCast(target, LAY_OF_HANDS);
						LOH_Timer = 1600;
						return true;
					}
					case 2:
					case 3: {
						doCast(target, FLASH_OF_LIGHT);
						GiveManaBack(15);
						return true;
					}
				}

			}
			if(hp < 60) { doCast(target, FLASH_OF_LIGHT); GiveManaBack(15); return true; }
			if(hp < 80) { doCast(target, HOLY_LIGHT); GiveManaBack(20); return true; }

			return true;
		} //end HealTarget


		void UpdateAI(const uint32 diff)
		{
			if(IAmDead()) return;

			ReduceCD(diff);

		   // opponent = master->getVictim() ? master->getVictim() : SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
			opponent = m_creature->getVictim();
			if(!opponent )
			{
				ResetOrGetNextTarget();
				DoNonCombatActions();
				return;
			}

			m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
			m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
			if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.5 && isTimerReady(Potion_cd))
			{
				doCast(m_creature, HEALINGPOTION);
				Potion_cd = Potion_cd;
			}

			oom_spam = false;

			//buff and heal master's group
			if(master->GetGroup())
			{
				BuffAndHealGroup(master);
				CureGroup(master);
			}

			// Heal myself
			HealTarget (m_creature, m_creature->GetHealth()*100 / m_creature->GetMaxHealth());



			DoNormalAttack(diff);
			Counter(diff);

			ScriptedAI::UpdateAI(diff);
		}

		void Aggro(Unit *who){
		}

		void Armour(const uint32 diff)
		{

		} //end Armour

		void DoNonCombatActions()
		{
			if (m_creature->HasAura(DEVOTION_AURA)) Aura = DEVOTIONAURA;
			else if (m_creature->HasAura(FIRE_RESISTANCE_AURA)) Aura = FIRERESISTANCEAURA;
			else Aura = NOAURA;

			//buff myself
			if(!master->HasAura(DEVOTION_AURA) && isTimerReady(GC_Timer)) {
				doCast(m_creature, DEVOTION_AURA, true);
			}
			else
			// I already have devotion aura and its not mine, cast different aura
			if (master->HasAura(DEVOTION_AURA) &&
				!master->HasAura(DEVOTION_AURA, m_creature->GetGUID()) &&
				Aura == NOAURA &&
				isTimerReady(GC_Timer))
				doCast(m_creature, FIRE_RESISTANCE_AURA, true);

			if(!m_creature->HasAura(SEAL_OF_LIGHT) && isTimerReady(GC_Timer))
				doCast(m_creature, SEAL_OF_LIGHT, true);

			//buff and heal master's group
			if(master->GetGroup())
			{
				RezGroup(REDEMPTION, master);
				BuffAndHealGroup(master);
				CureGroup(master);
			}

		}

		void BuffTarget(Unit *target)
		{
			if(!target) return;
			switch(target->getClass())
			{
			case CLASS_MAGE:
			case CLASS_PRIEST:
			case CLASS_WARLOCK:
				if (!HasAuraName(target, GetSpellName(BLESSING_OF_WISDOM))) doCast(target, BLESSING_OF_WISDOM, true);
				break;
			case CLASS_PALADIN:
				if (!HasAuraName(target, GetSpellName(BLESSING_OF_SANCTUARY))) doCast(target, BLESSING_OF_SANCTUARY, true);
				break;
			default:
				if (!HasAuraName(target, GetSpellName(BLESSING_OF_KINGS))) doCast(target, BLESSING_OF_KINGS, true);
				break;
			}

		}


		void JustDied(Unit *Killer)
		{
			master->SetBotCommandState(COMMAND_FOLLOW);
		}

		void AttackStart(Unit *u)
		{
			Aggro(u);
			ScriptedAI::AttackStart(u);
		}

		void KilledUnit(Unit *)
		{
			((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
		}

		void Counter(const uint32 diff)
		{
			if(opponent == NULL) return;
			if(opponent->isDead()) return;
			if(opponent->IsNonMeleeSpellCasted(true) && isTimerReady(HOJ_Timer))
			{
				doCast(opponent, HAMMER_OF_JUSTICE);
				HOJ_Timer = 600;
			}
		}

		void DoNormalAttack(const uint32 diff)
		{
			if(!opponent->HasAura(JUDGEMENT_OF_LIGHT) && isTimerReady(GC_Timer))
				doCast(opponent, JUDGEMENT_OF_LIGHT, true);

		}

		void ReduceCD(const uint32 diff)
		{
			if(GC_Timer > 0)        --GC_Timer;
			if(LOH_Timer > 0)       --LOH_Timer;
			if(HOJ_Timer > 0)       --HOJ_Timer;
		}
		void ReceiveBowEmote(Player *player)
		{
			BuffTarget(player);
		}
	};
};

void AddSC_paladin_bot()
{
    new paladin_bot();
}
