#include "ScriptPCH.h"
#include "bot_priest_ai.h"
#include "Group.h"

class priest_bot : public CreatureScript
{
public:
    priest_bot() : CreatureScript("priest_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new priest_botAI(pCreature);
    }

	struct priest_botAI : public bot_ai
	{
		priest_botAI(Creature *c) :bot_ai(c)
		{
			Reset();
		}

		int32 Heal_Timer;
		int32 Renew_Timer;
		int32 Self_Renew_Timer;
		int32 PWS_Timer;
		int32 Others_Heal_Timer;
		int32 Oom_timer;
		int32 Fade_Timer;
		int32 Potion_Timer;
		int32 Rez_Timer;

		Unit *mobsTarget;

		void Reset()
		{
			GC_Timer = 0;
			Heal_Timer = 0;
			Renew_Timer = 0;
			Self_Renew_Timer = 0;
			PWS_Timer = 0;
			Others_Heal_Timer = 0;
			Oom_timer = 0;
			Fade_Timer = 0;
			Potion_Timer = 0;
			Rez_Timer = 0;

			if (master) {
				setStats(CLASS_PRIEST, m_creature->getRace(), master->getLevel());
			}
		}

		void EnterEvadeMode(){ Oom_timer = 0; }

		void Aggro(Unit *who){}

		void AttackStart(Unit *u)
		{
			   m_creature->GetMotionMaster()->MoveFollow(master, urand(5, 10), PET_FOLLOW_ANGLE);

		}

		void KilledUnit(Unit *)
		{
			((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
		}

		void MoveInLineOfSight(Unit *target)
		{
			if (master==NULL || master==0) return;

			if(target->GetGUID() == master->GetGUID()) return;

			if(!target->IsFriendlyTo(master)) return;

			if(target->isAlive() && ((target->GetHealth()*100) < 100))
				m_creature->CastSpell(target, SPELL_HEAL, false);
		}

		bool isTimerReady(int32 timer)
		{
			if(timer <= 0 && GC_Timer <= 0) return true;
			else                            return false;
		} //end isTimerReady

		void decrementTimers()
		{
			if(GC_Timer > 0)          --GC_Timer;
			if(Heal_Timer > 0)        --Heal_Timer;
			if(Others_Heal_Timer > 0) --Others_Heal_Timer;
			if(Fade_Timer > 0)        --Fade_Timer;
			if(Self_Renew_Timer > 0)  --Self_Renew_Timer;
			if(Renew_Timer > 0)       --Renew_Timer;
			if(PWS_Timer > 0)         --PWS_Timer;
			if(Potion_Timer > 0)      --Potion_Timer;
		} //end decrementTImers

		void UpdateAI(const uint32 diff)
		{
			decrementTimers();

			if(IAmDead()) return;

			if(!m_creature->isInCombat())
			{
				DoNonCombatActions();
			}

			//buff and heal master's group
			BuffAndHealGroup(master);


			//check group members
			Group::MemberSlotList const &a =((Player*)master)->GetGroup()->GetMemberSlots();
			for(Group::member_citerator itr = a.begin(); itr != a.end(); itr++)
			{
				Player *tPlayer = ((Player *)master)->GetObjPlayer(itr->guid);
				if(tPlayer == NULL) continue;
			}

			//if low on mana, drink a potion
			if(m_creature->GetPower(POWER_MANA) < 400 && isTimerReady(Potion_Timer))
			{
				 doCast(m_creature, MANAPOTION, true);
				 Potion_Timer = 1500;
			}
			//if after drinking a potion still low on mana
			//let everyone know that you are oom.
			if(m_creature->GetPower(POWER_MANA)/m_creature->GetMaxPower(POWER_MANA) < 10)
			{
				if(Oom_timer == 0)
				{
					//MonsterSay("OOM", LANG_UNIVERSAL, NULL);
					Oom_timer = 1;
				}
			}

			// Heal myself
			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 90)
			{
				if(Fade_Timer <= 0 && m_creature->isInCombat() &&
					isTimerReady(Fade_Timer))
				{
					doCast(m_creature, SPELL_FADE);
					Fade_Timer = 30;
					return;
				}


				HealTarget (m_creature, m_creature->GetHealth()*100 / m_creature->GetMaxHealth());

			}

			//now try to heal bots and pets.  DoSelectLowestHpFriendly will get
			//everyone in group including bots and pets.  Unfortunately it can
			//not be triggered for % of lost HP, so we just set it to -1000.
			//This means low level players wont be healed because they wont have
			//enough HP.
			if(isTimerReady(Others_Heal_Timer))
			{
				Unit *target;
				if(target = DoSelectLowestHpFriendly(40, 1000))
				{
					doCast(target, SPELL_HEAL, false);
					GiveManaBack();
					Others_Heal_Timer = 50;
				}
				else if(target = DoSelectLowestHpFriendly(40, 500))
				{
					if(!target->HasAura(SPELL_RENEW, 0))
					{
						doCast(target, SPELL_RENEW, false);
						GiveManaBack();
						Others_Heal_Timer = 100;
					}
				}
			} //end if isTimerReady(Others_Heal_Timer)
		} //end UpdateAI

		bool HealTarget(Unit *target, uint8 hp)
		{
		   if (!isTimerReady(GC_Timer)) return false;
			if (m_creature->IsNonMeleeSpellCasted(true)) return false;
			if(!target || target->isDead()) return false;
		   // if(hp < 60) { doCast(target, FLASH_OF_LIGHT); GiveManaBack(15); return true; }
		   // if(hp < 80) { doCast(target, HOLY_LIGHT); GiveManaBack(20); return true; }

		   if((hp < 50) &&
				isTimerReady(PWS_Timer) &&
				!target->isDead())
			{
				doCast(target, SPELL_PW_SHIELD);
				PWS_Timer = 120;
				//free (buff);
			}

			if((hp < 90) &&
				hp >75 &&
				isTimerReady(Renew_Timer))
			{
				doCast(target, SPELL_RENEW, true);
				GiveManaBack();
				Renew_Timer = 90;
				Heal_Timer = Heal_Timer + 5; //wait 5 seconds before casting a real heal
				return true;
			}

			if((hp < 75) &&
				isTimerReady(Heal_Timer) &&
				!target->isDead())
			{
				doCast(target, SPELL_HEAL);
				GiveManaBack();
				Heal_Timer = 10;
				return true;
			}

			return true;
		}

		void BuffTarget(Unit *target)
		{
			if (!HasAuraName(target, GetSpellName(SPELL_FORTITUDE))) {
				doCast(target, SPELL_FORTITUDE, false);
				GiveManaBack();
			}
			if (!HasAuraName(target, GetSpellName(SPELL_PRAYER_OF_SHADOW))) {
				doCast(target, SPELL_PRAYER_OF_SHADOW, false);
				GiveManaBack();
			}
		}

		void DoNonCombatActions()
		{
			   //if eating or drinking don't do anything
				if(m_creature->HasAura(10256) || m_creature->HasAura(1137)) return;

				Feast();

				//buff master
				if(!HasAuraName(master, SPELL_PRAYER_OF_SHADOW, 0) && isTimerReady(GC_Timer)) doCast(master, SPELL_PRAYER_OF_SHADOW, true);
				if(!HasAuraName(master, FEAR_WARD, 0) && isTimerReady(GC_Timer)) doCast(master, FEAR_WARD, true);
				if(!HasAuraName(master, DIVINE_SPIRIT, 0) && isTimerReady(GC_Timer)) {
					doCast(master, DIVINE_SPIRIT, true);
					GiveManaBack();
				}

				//buff myself
				if(!m_creature->HasAura(SPELL_INNER_FIRE, 0) && isTimerReady(GC_Timer)) doCast(m_creature, SPELL_INNER_FIRE, true);
				if(!m_creature->HasAura(SPELL_FORTITUDE, 0) && isTimerReady(GC_Timer)) doCast(m_creature, SPELL_FORTITUDE, true);
				//if(m_creature->getRace() == RACE_UNDEAD_PLAYER && !m_creature->HasAura(SPELL_TOUCH_OF_WEAKNESS, 0) && isTimerReady(GC_Timer)) doCast(m_creature, SPELL_TOUCH_OF_WEAKNESS);

	//buff and heal master's group
			if(master->GetGroup())
			{
				RezGroup(SPELL_RESURRECTION, master);
				//BuffAndHealGroup(master);
			   // CureGroup(master);
			}


		}

		void ReceiveBowEmote(Player *player)
		{
			((priest_botAI*)m_creature->AI())->doCast(player, SPELL_FORTITUDE, true);
			((priest_botAI*)m_creature->AI())->doCast(player, SPELL_PRAYER_OF_SHADOW, false);
		}

	}; //end priest_bot
};


void AddSC_priest_bot()
{
    new priest_bot();
}
