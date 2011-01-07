    /* ScriptData
    SDName: pvp_warrior
    SD%Complete: 0
    SDComment: paytheo help from Gasilli
    SDCategory: Custom
    EndScriptData */
#include "ScriptPCH.h"
//#include "../../game/Player.h"
#include "bot_warrior_ai.h"
#include "Group.h"

bool castDemoralizingShout;
bool battleStance;
bool defensiveStance;
bool berserkerStance;

class warrior_bot : public CreatureScript
{
public:
    warrior_bot() : CreatureScript("warrior_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new warrior_botAI(pCreature);
    }

	struct warrior_botAI : public bot_ai
	{
		warrior_botAI(Creature *c) : bot_ai(c)
		{
			Reset();
		}

		uint32 charge_cd;
		uint32 deathWish_cd;
		uint32 mortalStrike_cd;
		uint32 overpower_cd;
		uint32 retaliation_recklessness_shieldwall_cd;
		uint32 berserkerRage_cd;
		uint32 challengingShout_cd;
		uint32 battleShout_cd;
		uint32 intercept_cd;
		uint32 intimidatingShout_cd;
		uint32 pummel_cd;
		uint32 whirlwind_cd;
		uint32 bloodrage_cd;
		uint32 disarm_cd;
		uint32 intervene_cd;
		uint32 shieldBash_cd;
		uint32 spellReflection_cd;
		uint32 potion_cd;
		uint32 firstAid_cd;
		uint32 pvpTrinket_cd;
		uint32 taunt_cd;
		uint32 sunder_cd;
		uint32 rage;
		uint32 yellRage;
		uint32 GCD;
		int32 Noggenfogger_Timer;


		void Reset()
		{
			charge_cd = 0;
			deathWish_cd = 0;
			mortalStrike_cd = 0;
			overpower_cd = 0;
			retaliation_recklessness_shieldwall_cd = 0;
			berserkerRage_cd = 0;
			challengingShout_cd = 0;
			battleShout_cd = 0;
			intercept_cd = 0;
			intimidatingShout_cd = 0;
			pummel_cd = 0;
			whirlwind_cd = 0;
			bloodrage_cd = 0;
			disarm_cd = 0;
			intervene_cd = 0;
			shieldBash_cd = 0;
			spellReflection_cd = 0;
			potion_cd = 0;
			firstAid_cd = 0;
			pvpTrinket_cd = 0;
			taunt_cd = 0;
			sunder_cd = 0;
			rage = 0;
			yellRage = 0;
			GCD = 0;
			castDemoralizingShout = false;
			battleStance = true;
			defensiveStance = false;
			berserkerStance = false;

			Noggenfogger_Timer = 0;
			m_creature->setPowerType(POWER_RAGE);

			if (master) {
				setStats(CLASS_WARRIOR, m_creature->getRace(), master->getLevel());
			}
		}

		void EnterEvadeMode(){}

		void doCast(Unit *victim, uint32 spellId, bool triggered = false)
		{
			if(spellId == 0) return;

			GCD = 2000;
			DoCast(victim, spellId, triggered);
		} //end doCast

		bool isTimerReady(int32 timer, uint32 diff)
		{
			if(timer <= 0 && GCD < diff)    return true;
			else                            return false;
		}


		void UpdateAI(const uint32 diff)
		{
			ReduceCD(diff);

			if(IAmDead()) return;

			//Use Noggenfogger potion if a tauren
			if(master->GetBotRace() == RACE_TAUREN &&
			isTimerReady(Noggenfogger_Timer, diff))
			{
				uint64 m_rand = urand(1, 2);
				switch(m_rand)
				{
					case 1:
							doCast(m_creature, SPELL_NOGGENFOGGER_SKELETON, true);
							break;
					case 2:
							doCast(m_creature, SPELL_NOGGENFOGGER_SMALL, true);
							break;
				}
				Noggenfogger_Timer = 6000; //10 minutes
			}


			Unit *opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
			if(!opponent && !m_creature->getVictim())
			{
				ResetOrGetNextTarget();
				if(rage > 0)
				{
					--rage;
					//m_creature->SetPower(POWER_RAGE, rage*10);
				}
				return;
			}

			rage = m_creature->GetPower(POWER_RAGE);

			m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
			m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);

			if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.2 &&
				potion_cd<diff && GCD < diff)
			{
				doCast(m_creature, HEALINGPOTION);
				potion_cd = POTIONCD;
			}

			if(!m_creature->HasAura(BATTLESHOUT) && rage > 10 && GCD < diff &&
				battleShout_cd<diff)
			{
				doCast(m_creature, BATTLESHOUT);
				//rage -= 10;
				battleShout_cd = BATTLESHOUT_CD;
			}

			//if(rage > 100)
			//{
				//rage = 0;
			//}

			if(bloodrage_cd < diff       &&
				m_creature->isInCombat() &&
				opponent                 &&
				GCD < diff)
			{
				doCast(m_creature, BLOODRAGE);
				bloodrage_cd = BLOODRAGE_CD;
				//rage += 10;
			}

			if(!m_creature->SelectVictim() || !m_creature->getVictim())
			{
				ResetOrGetNextTarget();
				return;
			}

			BreakCC(diff);
			Attack(diff);

			ScriptedAI::UpdateAI(diff);
		}


		void AttackStart(Unit *u)
		{
			Aggro(u);
			ScriptedAI::AttackStart(u);
		}

		void Aggro(Unit *who)
		{
			//Unit *opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
			Unit *opponent = who;

			if(!opponent) return;

			if((m_creature->GetDistance(opponent) > 15) &&
			(m_creature->GetDistance(opponent) < 25) &&
			charge_cd <= 0)
			{
				doCast(opponent, CHARGE);
				charge_cd = CHARGE_CD;
			}

			if(opponent->getClass() == CLASS_ROGUE ||
			opponent->getClass() == CLASS_WARRIOR  ||
			opponent->getClass() == CLASS_SHAMAN   ||
			opponent->getClass() == CLASS_DRUID    ||
			opponent->getClass() == CLASS_PALADIN  ||
			opponent->getClass() == CLASS_HUNTER)
				castDemoralizingShout = true;
		} //end Aggro

		void KilledUnit(Unit *Victim)
		{
			master->SetBotCommandState(COMMAND_FOLLOW);
			//DoPlaySoundToSet(m_creature, 8852);
			//m_creature->Yell("Haha! Im just getting warmed up!", LANG_UNIVERSAL, NULL);
		}

		void JustDied(Unit *Killer)
		{
			DoPlaySoundToSet(m_creature, 8860);
			master->SetBotCommandState(COMMAND_FOLLOW);
			//m_creature->Yell("An honorable.. death..", LANG_UNIVERSAL, NULL);
		}

		void BreakCC(const uint32 diff)
		{
			 if(pvpTrinket_cd < diff && GCD < diff)
			 {
				 if(m_creature->HasAuraType(SPELL_AURA_MOD_ROOT)  ||
				 m_creature->HasAuraType(SPELL_AURA_MOD_CONFUSE)  || //dragons breath/blind/poly
				 m_creature->HasAura(8983)                        || //Druid bash rank 3
				 m_creature->HasAura(27006)                       || //Druid pounce rank 4
				 m_creature->HasAura(33786)                       || //Druid cyclone
				 m_creature->HasAura(22570, 1)                    || //Druid maim
				 m_creature->HasAura(10308)                       || //Paladin hammer of justice rank 4
				 m_creature->HasAura(30414, 1)                    || //Warlock shadowfury rank 3
				 m_creature->HasAura(6215)                        || //Warlock fear rank 3 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
				 m_creature->HasAura(17928)                       || //Warlock howlofterror rank 3 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
				 m_creature->HasAura(10890)                       || //Priest psychic scream rank 4 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
				 m_creature->HasAura(14902)                       || //Rogue Cheap shot
				 m_creature->HasAura(8643)                        || //Rogue Kidney shot Rank 2
				 m_creature->HasAura(38764, 2)                    || //Rogue Gouge Rank 6 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
				 m_creature->HasAura(12809))                         //Warrior concussion blow
				{
					doCast(m_creature, PVPTRINKET); //I think it would be better to instead of applying individual spells that apply the
					pvpTrinket_cd = PVPTRINKET_CD;  //effect SPELL_AURA_MOD_STUN, just add that type and start removing bad choices e.g. impact.
				}

				if(m_creature->HasAura(11297) && m_creature->GetDistance(m_creature->getVictim()) < 10)
				{   //if warrior sapped and creature is less then 10 yards from warrior, cast pvp trinket and attempt to demoralizing shout him out of stealth
					doCast(m_creature, PVPTRINKET);
					pvpTrinket_cd = PVPTRINKET_CD;
					castDemoralizingShout = true;
				}
			}
		} //BreakCC

		void Attack(const uint32 diff)
		{
			//Unit *opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
			Unit *opponent = m_creature->getVictim();
			if(!opponent) return;

			if((m_creature->GetDistance(opponent) > 13)      &&
			(m_creature->GetDistance(opponent) < 25)         &&
			!m_creature->HasAuraType(SPELL_AURA_MOD_ROOT)    &&
			!m_creature->HasAuraType(SPELL_AURA_MOD_STUN)    &&
			!m_creature->HasAuraType(SPELL_AURA_MOD_CONFUSE) &&
			(intercept_cd < diff)                            &&
			(rage > 10)                                      &&
			(GCD < diff))
			{
				if(berserkerStance == true)
				{
					doCast(opponent, INTERCEPT, true);
					intercept_cd = INTERCEPT_CD;
					//rage -= 10;
				} else stanceChange(3);
			}

			if(disarm_cd < diff                                           &&
				(opponent->GetHealth()*100/opponent->GetMaxHealth()) < 80 &&
				rage > 15                                                 &&
				!HasAuraName(opponent, GetSpellName(DISARM))              &&
				GCD < diff)
			{
				if(opponent->getClass() == CLASS_ROGUE  ||
					opponent->getClass() == CLASS_WARRIOR   ||
					opponent->getClass() == CLASS_SHAMAN    ||
					opponent->getClass() == CLASS_PALADIN)
				{
					if(defensiveStance == true)
					{
						doCast(opponent, DISARM, true);
						//rage -= 15;
						disarm_cd = DISARM_CD;
					} else stanceChange(2);
				}
			}

			//opponent is not attacking me so try to taunt it
			if(opponent->getVictim() &&
			   opponent->getVictim()->GetGUID() != m_creature->GetGUID() &&
			   taunt_cd < diff &&
			   GCD < diff)
			{
				if(battleStance != true)
				{
					doCast(m_creature, DEFENSIVESTANCE, true);
					defensiveStance = true;
				}
				doCast(opponent, TAUNT, true);
				doCast(m_creature, TAUNT_VISUAL, true);
				taunt_cd = TAUNT_CD;
			}

	/*        if(sunder_cd < diff && GCD < diff)
			{
	sLog->outError ("SUNDER");
				doCast(opponent, SUNDER, true);
				//rage -= 15;
				sunder_cd = SUNDER_CD;
			}
	*/
			if((opponent->GetHealth()*100/opponent->GetMaxHealth()) < 15 &&
				rage > 15                                                    &&
				GCD < diff)
			{
				if(battleStance == true || berserkerStance == true)
				{
					int damage = (rage*4 + (m_creature->getLevel()*10))/2;

					m_creature->CastCustomSpell(opponent, EXECUTE, &damage, NULL, NULL, false, NULL, NULL);
					//rage = 0;
					//m_creature->SetPower(POWER_RAGE, 0);
					GCD = 20;
				} else stanceChange(5);
			}

			if(mortalStrike_cd < diff               &&
			   rage > 15                               &&
			   GCD < diff)
			{
				doCast(opponent, MORTALSTRIKE, true);
				mortalStrike_cd = MORTALSTRIKE_CD;
				//rage -= 15;
			}

			if(castDemoralizingShout == true         &&
			   !opponent->HasAura(DEMORALIZINGSHOUT)    &&
			   rage < 10 && GCD < diff)
			{
				doCast(opponent, DEMORALIZINGSHOUT, true);
				//rage -= 10;
				castDemoralizingShout = false;
			}

			if(opponent->IsNonMeleeSpellCasted(true) &&
			   pummel_cd < diff                         &&
			   rage > 10                                &&
			   GCD < diff)
			{
				if(berserkerStance == true)
				{
					doCast(opponent, PUMMEL, true);
					pummel_cd = PUMMEL_CD;
					//rage -= 10;
				} else stanceChange(3);
			}

			if(whirlwind_cd < diff                &&
			   rage > 25                             &&
			   GCD < diff)
			{
				if(berserkerStance == true)
				{
					doCast(opponent, WHIRLWIND, true);
					whirlwind_cd = WHIRLWIND_CD;
					//rage -= 25;
				} else stanceChange(3);
			}

			if(!opponent->HasAura(REND)                 &&
			   rage > 10                                &&
			   GCD < diff)
			{
				if(battleStance == true || defensiveStance == true)
				{
					doCast(opponent, REND, true);
					//rage -= 10;
				} else stanceChange(1);
			}

			if(!opponent->HasAura(HAMSTRING, 1)         &&
				rage > 10                               &&
				GCD < diff)
			{
				if(battleStance == true ||
				berserkerStance == true)
				{
					doCast(opponent, HAMSTRING, true);
					//rage -= 10;
				} else stanceChange(5);
			}

		}

		void stanceChange(uint32 stance)
		{
		 //if(rage > 20)
			 //rage = 20;

			if(stance == 5)
			{
				switch(rand()%2)
				{
				case 0:
					stance = 1; break;
				case 1:
					stance = 3; break;
				}
			}

			battleStance = false;
			defensiveStance = false;
			berserkerStance = false;

			switch(stance)
			{
			case 1:
				doCast(m_creature, BATTLESTANCE);
				battleStance = true;
				break;
			case 2:
				doCast(m_creature, DEFENSIVESTANCE);
				defensiveStance = true;
				break;
			case 3:
				doCast(m_creature, BERSERKERSTANCE);
				berserkerStance = true;
				break;
			}
		}

		void ReduceCD(const uint32 diff)
		{
			if(!(deathWish_cd < diff))                  deathWish_cd -= diff;
			if(!(mortalStrike_cd < diff))               mortalStrike_cd -= diff;
			if(!(overpower_cd < diff))                 overpower_cd -= diff;
			if(!(retaliation_recklessness_shieldwall_cd < diff))  retaliation_recklessness_shieldwall_cd -= diff;
			if(!(berserkerRage_cd < diff))              berserkerRage_cd -= diff;
			if(!(intercept_cd < diff))                  intercept_cd -= diff;
			if(!(intimidatingShout_cd < diff))          intimidatingShout_cd -= diff;
			if(!(pummel_cd < diff))                     pummel_cd -= diff;
			if(!(whirlwind_cd < diff))                  whirlwind_cd -= diff;
			if(!(bloodrage_cd < diff))                  bloodrage_cd -= diff;
			if(!(disarm_cd < diff))                     disarm_cd -= diff;
			if(!(intervene_cd < diff))                  intervene_cd -= diff;
			if(!(shieldBash_cd < diff))                 shieldBash_cd -= diff;
			if(!(spellReflection_cd < diff))            spellReflection_cd -= diff;
			if(!(potion_cd < diff))                     potion_cd -= diff;
			if(!(yellRage < diff))                      yellRage -= diff;
			if(!(firstAid_cd < diff))                   firstAid_cd -= diff;
			if(!(taunt_cd < diff))                      taunt_cd -= diff;
			if(!(sunder_cd < diff))                     sunder_cd -= diff;
			if(!(battleShout_cd < diff))                battleShout_cd -= diff;
			if(!(GCD < diff))                           GCD -= diff;
			else                                        GCD = 0;

			if(charge_cd > 0)                           --charge_cd; //this is treated different
			if(Noggenfogger_Timer >= 0)                --Noggenfogger_Timer;

		}
	};
};

void AddSC_warrior_bot()
{
    new warrior_bot();
}
