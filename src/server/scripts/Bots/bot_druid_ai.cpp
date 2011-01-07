#include "ScriptPCH.h"
#include "bot_druid_ai.h"
#include "Group.h"

class druid_bot : public CreatureScript
{
public:
    druid_bot() : CreatureScript("druid_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new bot_druid_ai(pCreature);
    }

	struct bot_druid_ai : public bot_ai
	{
		bot_druid_ai(Creature *c) : bot_ai(c)
		{
			Reset();
		}

		int32 GC_Timer; //global cooldown
		int32 Heal_Timer;
		int32 Regrowth_Timer;
		int32 Self_Regrowth_Timer;
		int32 Self_Rejuvenation_Timer;
		int32 Self_Heal_Timer;
		int32 Rejuvenation_Timer;
		int32 Others_Heal_Timer;
		int32 Oom_timer;
		int32 Fade_Timer;
		int32 Potion_Timer;
		int32 Warstomp_Timer;
		int32 Rez_Timer;

		int32 Noggenfogger_Timer;

		//Bear Timers
		int32 Demoralizing_Roar_Timer;
		int32 Swipe_Timer;

		//Cat Timers
		int32 Claw_Timer;
		int32 Rake_Timer;
		int32 Shred_Timer;
		int32 Rip_Timer;
		int32 Mangle_Cat_Timer;

		//Balance Timers
		int32 Moonfire_Timer;
		int32 Starfire_Timer;
		int32 Wrath_Timer;
		int32 Fairie_Fire_Timer;

		Unit *mobsTarget;
		Unit *opponent;

		void Reset()
		{
			GC_Timer = 0;
			Potion_Timer = 0;
			Heal_Timer = 0;
			Regrowth_Timer = 0;
			Self_Regrowth_Timer = 0;
			Self_Rejuvenation_Timer = 0;
			Self_Heal_Timer = 0;
			Rejuvenation_Timer = 0;
			Rez_Timer = 0;

			Others_Heal_Timer = 0;
			Oom_timer = 0;

			Warstomp_Timer = 0;
			Demoralizing_Roar_Timer = 10;
			Swipe_Timer = 20;
			Noggenfogger_Timer = 0;

			Claw_Timer = 0;
			Rake_Timer = 0;
			Shred_Timer = 0;
			Rip_Timer = 0;
			Mangle_Cat_Timer = 0;

			Moonfire_Timer = 30;
			Starfire_Timer = 90;
			Wrath_Timer = 150;
			Fairie_Fire_Timer = 10;

			opponent = NULL;

			if (master) {
				setStats(CLASS_DRUID, m_creature->getRace(), master->getLevel());
		   }
		}

		void Aggro(Unit *who){}

		void EnterEvadeMode(){}

		void KilledUnit(Unit *){ master->SetBotCommandState(COMMAND_FOLLOW); }

		//try to do a warstomp every time I get out of animal form
		void warstomp(const uint32 diff)
		{
			if(master->GetBotRace() != RACE_TAUREN) return;

			if(Warstomp_Timer <= 0)
			{
				if(opponent != NULL)
				{
					Warstomp_Timer = 1200; //2 minutes
					doCast(opponent, SPELL_WARSTOMP, true);
				}
			}
			else if(Warstomp_Timer >= 0)
				--Warstomp_Timer;
		} //end warstomp

		void removeFeralForm(Player *m=0)
		{
			if(!m) m=master; //if m is set than use it, else use master

			if(m_creature->HasAura(SPELL_BEAR_FORM, 0))
			{
				m_creature->RemoveAurasDueToSpell(SPELL_BEAR_FORM);
				m_creature->RemoveAurasDueToSpell(SPELL_BEAR_FORM_MOD);
				m->SetBotMustWaitForSpell1(3000);
			}

			if(m_creature->HasAura(SPELL_CAT_FORM, 0))
			{
				m_creature->RemoveAurasDueToSpell(SPELL_CAT_FORM);
				m_creature->RemoveAurasDueToSpell(SPELL_CAT_FORM_MOD);
				m->SetBotMustWaitForSpell1(3000);
			}

			m_creature->setPowerType(POWER_MANA);

		} //end removeFeralForm

		bool isTimerReady(int32 timer)
		{
			if(timer <= 0 && GC_Timer <= 0) return true;
			else                            return false;
		}

		void decrementTimers()
		{
			if(GC_Timer >= 0)                --GC_Timer;
			if(Demoralizing_Roar_Timer >= 0) --Demoralizing_Roar_Timer;
			if(Swipe_Timer >= 0)             --Swipe_Timer;
			if(Claw_Timer >= 0)              --Claw_Timer;
			if(Rake_Timer >= 0)              --Rake_Timer;
			if(Shred_Timer >= 0)             --Shred_Timer;
			if(Rip_Timer >= 0)               --Rip_Timer;
			if(Mangle_Cat_Timer >= 0)        --Mangle_Cat_Timer;
			if(Moonfire_Timer >= 0)          --Moonfire_Timer;
			if(Starfire_Timer >= 0)          --Starfire_Timer;
			if(Wrath_Timer >= 0)             --Wrath_Timer;
			if(Rip_Timer >= 0)               --Fairie_Fire_Timer;
			if(Potion_Timer >= 0)            --Potion_Timer;
			if(Rejuvenation_Timer >= 0)      --Rejuvenation_Timer;
			if(Regrowth_Timer >= 0)          --Regrowth_Timer;
			if(Heal_Timer > 0)               --Heal_Timer;
			if(Self_Regrowth_Timer >= 0)     --Self_Regrowth_Timer;
			if(Self_Rejuvenation_Timer >= 0) --Self_Rejuvenation_Timer;
			if(Self_Heal_Timer >= 0)         --Self_Heal_Timer;
			if(Others_Heal_Timer >= 0)       --Others_Heal_Timer;
			if(Rez_Timer >= 0)               --Rez_Timer;
			if(Noggenfogger_Timer >= 0)      --Noggenfogger_Timer;

		}

		void doCast(Unit *victim, uint32 spellId, bool triggered = false)
		{
			if(spellId == 0) return;

			GC_Timer = 20;
			m_creature->SetStandState(UNIT_STAND_STATE_STAND);
			DoCast(victim, spellId, triggered);
		} //end doCast

		void doBearActions(const uint32 diff)
		{
			m_creature->setPowerType(POWER_RAGE);

			if(isTimerReady(Demoralizing_Roar_Timer) && opponent != NULL)
			{
				Demoralizing_Roar_Timer = 150;
				doCast(opponent, SPELL_DEMORALIZING_ROAR, true);
			}

			if(isTimerReady(Swipe_Timer) && opponent != NULL)
			{
				Swipe_Timer = 50;
				doCast(opponent, SPELL_SWIPE, true);
			}
		}//end doBearActions



		void doCatActions(/*Player *master, Creature *m_creature,*/ const uint32 diff)
		{

			m_creature->SetPower(POWER_ENERGY, 100);

			if(isTimerReady(Claw_Timer) && opponent != NULL)
			{
				Claw_Timer = 70;
				doCast(opponent, SPELL_CLAW, true);
			}

			if(isTimerReady(Mangle_Cat_Timer) && opponent != NULL)
			{
				Mangle_Cat_Timer = 70;
				doCast(opponent, SPELL_MANGLE_CAT, true);
			}

			if(isTimerReady(Rake_Timer) && opponent != NULL)
			{
				Rake_Timer = 100;
				doCast(opponent, SPELL_RAKE, true);
			}

			if(isTimerReady(Shred_Timer) && opponent != NULL)
			{
				Shred_Timer = 120;
				doCast(opponent, SPELL_SHRED, true);
			}

			if(isTimerReady(Rip_Timer) && opponent != NULL)
			{
				Rip_Timer = 150;
				doCast(opponent, SPELL_RIP, true);
			}
		} //end doCatActions


		void doBalanceActions(const uint32 diff)
		{
			removeFeralForm();
			m_creature->setPowerType (POWER_MANA);

			if(isTimerReady(Moonfire_Timer) && opponent != NULL)
			{
				Moonfire_Timer = 150;
				doCast(opponent, SPELL_MOONFIRE, true);
			}

			if(isTimerReady(Starfire_Timer) && opponent != NULL)
			{
				Starfire_Timer = 200;
				doCast(opponent, SPELL_STARFIRE, true);
			}

			if(isTimerReady(Wrath_Timer) && opponent != NULL)
			{
				Wrath_Timer = 180;
				doCast(opponent, SPELL_WRATH, true);
			}

			if(isTimerReady(Fairie_Fire_Timer) && opponent != NULL)
			{
				Fairie_Fire_Timer = 200;
				doCast(opponent, SPELL_FAIRIE_FIRE, true);
			}
		} //end doBalanceActions


		bool CureTarget(Unit *target)
		{
			if (HasAuraIcon(target, 68 /*"Venom Spit"*/)
				//|| HasAuraName(target, "Venom Spit")
				//|| HasAuraName(target, "Poison"))
				&& !HasAuraName(target, SPELL_CURE_POISON))
			{
				removeFeralForm();
				doCast(target, SPELL_CURE_POISON);
			}
			return true;
		}

		void UpdateAI(const uint32 diff)
		{
			decrementTimers();

			if(IAmDead()) return;

			if(m_creature->HasUnitState(UNIT_STAT_CASTING))
				return;

			//self buff
			if(!m_creature->isInCombat())
			{
				if(!m_creature->HasAura(SPELL_THORNS))
				{
					removeFeralForm();
					doCast(m_creature, SPELL_THORNS, true);
				}
			}

			if(m_creature->GetPower(POWER_MANA) < 400 && isTimerReady(Potion_Timer))
			{
				 doCast(m_creature, REJUVEPOTION, true);
				 //m_creature->MonsterSay("MANA POTION", LANG_UNIVERSAL, NULL);
				 Potion_Timer = 150;
				 Oom_timer = 0;
			}
			if(m_creature->GetPower(POWER_MANA)/m_creature->GetMaxPower(POWER_MANA) < 10)
			{
				if(Oom_timer == 0)
				{
					//m_creature->MonsterSay("OOM", LANG_UNIVERSAL, NULL);
					Oom_timer = 1;
				}
			}

			//Heal master
			if((master->GetHealth()*100 / master->GetMaxHealth() < 90) && isTimerReady(Rejuvenation_Timer) && master->isAlive())
			{
				removeFeralForm();
				warstomp(diff);
				doCast(master, SPELL_REJUVENATION, true);
				Rejuvenation_Timer = 300;
				Heal_Timer = Heal_Timer + 30; //wait 3 seconds before casting a real heal
				Regrowth_Timer = Regrowth_Timer + 20; //wait 2 seconds before casting a regrowth
				//if(master->isInCombat() && master->getVictim() == NULL) return;
				return;
			}

			if((master->GetHealth()*100 / master->GetMaxHealth() < 90) && isTimerReady(Regrowth_Timer) && master->isAlive())
			{
				removeFeralForm();
				warstomp(diff);
				doCast(master, SPELL_REGROWTH, true);
				Regrowth_Timer = 200;
				Heal_Timer = Heal_Timer + 20; //wait 2 seconds before casting a real heal
				//if(master->isInCombat() && master->getVictim() == NULL) return;
				return;
			}

			if((master->GetHealth()*100 / master->GetMaxHealth() < 75) && isTimerReady(Heal_Timer) && master->isAlive()
			/* && CanCast(master, GetSpellStore()->LookupEntry(SPELL_HEALING_TOUCH))*/)
			{
				removeFeralForm();
				warstomp(diff);
				doCast(master, SPELL_HEALING_TOUCH, false);
				Heal_Timer = 100;
				return;
			}

			//
			//Heal myself
			//
			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 80 && !m_creature->HasAura(SPELL_REGROWTH, 1) && isTimerReady(Self_Regrowth_Timer))
			{
				removeFeralForm();
				warstomp(diff);
				doCast(m_creature, SPELL_REGROWTH, false);

				Self_Regrowth_Timer = 120;
				return;
			}
			if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 80 && !m_creature->HasAura(SPELL_REJUVENATION, 0) && isTimerReady(Self_Rejuvenation_Timer)
			/* && CanCast(m_creature, GetSpellStore()->LookupEntry(SPELL_REGROWTH))*/)
			{
				removeFeralForm();
				warstomp(diff);
				doCast(m_creature, SPELL_REJUVENATION, false);

				Self_Rejuvenation_Timer = 120;
				return;
			}
			if((m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 75) && isTimerReady(Self_Heal_Timer)
			/* && CanCast(master, GetSpellStore()->LookupEntry(SPELL_HEALING_TOUCH))*/)
			{
				removeFeralForm();
				warstomp(diff);

				doCast(m_creature, SPELL_HEALING_TOUCH, false);
				Self_Heal_Timer = 100;
				return;
			}


			//
			//Heal others
			//
			//check group members, this doesn't check bots/pets.  They will be done later.  Preference
			//goes to real players first.
			//
			Group::MemberSlotList const &a =((Player*)master)->GetGroup()->GetMemberSlots();
			for(Group::member_citerator itr = a.begin(); itr != a.end(); itr++)
			{
				Player *tPlayer = ((Player *)master)->GetObjPlayer(itr->guid);
				if(tPlayer == NULL) continue;

				//healing others
				if(tPlayer->isAlive() && isTimerReady(Others_Heal_Timer) && tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth() < 75)
				{
					//sLog->outError("Druid_Bot: healing someoneelse: %s it has %d HP", tPlayer->GetName(), tPlayer->GetHealth());
					doCast(tPlayer, SPELL_HEALING_TOUCH, false);
					Others_Heal_Timer = 250;
				}

				//rezzes
				if(tPlayer->isDead() && m_creature->getLevel() >= 20 &&
				//CanCast(tPlayer, GetSpellStore()->LookupEntry(SPELL_REBIRTH)) &&
				m_creature->GetDistance(tPlayer) < 40 && (isTimerReady(Rez_Timer)))
				{
					char *str = (char *)malloc(32);
					sprintf(str, "Rezzing %s", tPlayer->GetName());
					m_creature->MonsterSay(str, LANG_UNIVERSAL, NULL);
					free(str);
					doCast(tPlayer, SPELL_REBIRTH, false);
					Rez_Timer = 1600;
				}

				//buff group
				if(tPlayer->isAlive())
				{
					if(!HasAuraName(tPlayer, GetSpellName(SPELL_MARK_OF_THE_WILD)) && !HasAuraName(tPlayer, "Gift of the Wild") && isTimerReady(GC_Timer))
					{
						removeFeralForm();
						doCast(tPlayer, SPELL_MARK_OF_THE_WILD);
					}

					if(!HasAuraName(tPlayer, SPELL_THORNS) && isTimerReady(GC_Timer))
					{
						removeFeralForm();
						doCast(tPlayer, SPELL_THORNS);
					}
				}
			}

			if(isTimerReady(Noggenfogger_Timer))
			{
				uint64 m_rand = urand(1, 2);
				switch(m_rand)
				{
					case 1:
						doCast(m_creature, SPELL_NOGGENFOGGER_SKELETON, true);
						break;
					case 2:
						// Don't change forms b/c it crashes in GetModelForForm().  It checks
						// PLAYER_BYTES and since its an npc it does not have this value.
						//doCast(m_creature, SPELL_NOGGENFOGGER_SMALL, true);
						break;
				}
				Noggenfogger_Timer = 6000; //10 minutes
			}

			//The rest is combat stuff, so if not in combat just return
			opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
			if(!opponent && !m_creature->getVictim())
			{
				CureGroup(master);
				ResetOrGetNextTarget();
				return;
			}

			//default value
			float val2 = m_creature->GetStat(STAT_STRENGTH)*2.0f;

			//only go Feral if we are "small" from the noggenfogger spell
			if(m_creature->HasAura(SPELL_NOGGENFOGGER_SMALL))
			{
				//if the target is attacking us, we want to go bear
				if(opponent && opponent->getVictim() &&
				opponent->getVictim()->GetGUID() == m_creature->GetGUID())
				{
					//if we don't have bear yet
					if(!m_creature->HasAura(SPELL_BEAR_FORM))
					{
						m_creature->RemoveAurasDueToSpell(SPELL_CAT_FORM); //remove cat
						m_creature->RemoveAurasDueToSpell(SPELL_CAT_FORM_MOD);
						doCast(m_creature, SPELL_BEAR_FORM, true);
						doCast(m_creature, SPELL_BEAR_FORM_MOD, true);
						master->SetBotMustWaitForSpell1(3000);
						m_creature->setPowerType(POWER_RAGE);
						m_creature->SetMaxPower(POWER_RAGE, 1000);
						m_creature->SetPower(POWER_RAGE, 1000);
						val2 = m_creature->getLevel()*3.0f + m_creature->GetStat(STAT_STRENGTH)*2.0f;

						//update attack power based on form
						val2 = val2 + (val2 * (m_creature->getLevel()/50));
						m_creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, val2);
						m_creature->UpdateDamagePhysical(BASE_ATTACK);
					}
					doBearActions(diff);
				} else {
					//if we don't have cat yet
					if(!m_creature->HasAura(SPELL_CAT_FORM))
					{
						m_creature->RemoveAurasDueToSpell(SPELL_BEAR_FORM); //remove bear
						m_creature->RemoveAurasDueToSpell(SPELL_BEAR_FORM_MOD);
						doCast(m_creature, SPELL_CAT_FORM, true);
						doCast(m_creature, SPELL_CAT_FORM_MOD, true);
						master->SetBotMustWaitForSpell1(3000);
						m_creature->setPowerType(POWER_ENERGY);
						m_creature->SetMaxPower(POWER_ENERGY, 1000);
						m_creature->SetPower(POWER_ENERGY, 1000);
						m_creature->SetSpeed(MOVE_RUN, m_creature->GetSpeed(MOVE_RUN) - 0.1f, true);
						val2 = m_creature->getLevel()*5.0f + m_creature->GetStat(STAT_STRENGTH)*2.0f + m_creature->GetStat(STAT_AGILITY) - 20.0f;

						//update attack power based on form
						val2 = val2 + (val2 * (m_creature->getLevel()/50));
						m_creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, val2);
						m_creature->UpdateDamagePhysical(BASE_ATTACK);
					}
					doCatActions(diff);
				}
			//else go Balance if we are a skeleton from noggenfogger
			} else {
				val2 = m_creature->GetStat(STAT_STRENGTH)*2.0f;
				val2 = val2 + (val2 * (m_creature->getLevel()/50));
				m_creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, val2);
				m_creature->UpdateDamagePhysical(BASE_ATTACK);
				doBalanceActions(diff);
			}



			//now try to heal bots and pets.  DoSelectLowestHpFriendly will get
			//everyone in group including bots and pets.  Unfortunately it can
			//not be triggered for % of lost HP, so we just set it to -1000.
			//This means low level players wont be healed because they wont have
			//enough HP.
			Unit *target = DoSelectLowestHpFriendly(40, 1000);
			if(target && isTimerReady(Others_Heal_Timer))
			{
				doCast(target, SPELL_HEALING_TOUCH, false);
				Others_Heal_Timer = 50;
			} else {
				target = DoSelectLowestHpFriendly(40, 500);  //now try someone with less HP lost
				if(target && isTimerReady(Others_Heal_Timer))
				{
					if(!target->HasAura(SPELL_REGROWTH, 1))
					{
						doCast(target, SPELL_REGROWTH, false);
						Others_Heal_Timer = 100;
					}
				}
			}

			ScriptedAI::UpdateAI(diff);
		}
		void ReceiveBowEmote(Player *player)
		{
			((bot_druid_ai*)m_creature->AI())->removeFeralForm((Player*) m_creature->GetCharmer());
			((bot_druid_ai*)m_creature->AI())->doCast(player, SPELL_THORNS, true );
			((bot_druid_ai*)m_creature->AI())->doCast(player, SPELL_MARK_OF_THE_WILD, false );
		}

	}; //end druid_bot

};


void AddSC_druid_bot()
{
    new druid_bot();
}
