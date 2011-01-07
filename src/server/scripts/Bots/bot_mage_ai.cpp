    /* ScriptData
    SDName: pvp_mage
    SD%Complete: 0
    SDComment: Original PVP mage by Brats reworked by Azrael with the help of Machiavelli
    SDCategory: Custom
    EndScriptData */
#include "ScriptPCH.h"
#include "bot_mage_ai.h"
#include "Group.h"

class mage_bot : public CreatureScript
{
public:
    mage_bot() : CreatureScript("mage_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new mage_botAI(pCreature);
    }

	struct mage_botAI : public bot_ai
	{
		mage_botAI(Creature *c) :bot_ai(c)
		{
			//master = NULL;
			Reset();
		}

		bool oom_spam;

		uint8 state;
		uint8 next_state;
		uint32 next_state_timer;
		Unit *opponent;

		void Reset()
		{
			Blizzard_cd = 0;
			FireBlast_cd = 0;
			BlastWave_cd = 0;
			CounterSpell_cd = 0;
			FrostNova_cd = 0;
			PoM_cd = 0;
			Ward_cd = 0;
			DragonBreath_cd = 0;
			Blink_cd = 0;
			Potion_cd = 0;
			Combustion_cd = 0;
			Evocation_cd = 0;
			FirstAid_cd = 0;
			GC_Timer = 0;
			CastedArcaneIntellect = false;
			CastedDampenMagic = false;
			CastedArmor1 = false;
			blink_timer = 10;
			blink = false;
			//wait = false;
			oom_spam = false;

			state = 1;
			next_state = 0;
			next_state_timer = 0;

			opponent = NULL;

			if (master) {
				setStats(CLASS_DRUID, m_creature->getRace(), master->getLevel());
		   }
		}

		void UpdateAI(const uint32 diff)
		{
			if(IAmDead()) return;

			ReduceCD(diff);

			if(!m_creature->isInCombat())
			{
				//buff master because master might be in different group
				if(!HasAuraName(master, GetSpellName(ARCANEINTELLECT)) &&
					master->isAlive() &&
					isTimerReady(GC_Timer))
						doCast(master, ARCANEINTELLECT, true);

				//buff myself
				if(!m_creature->HasAura(ARCANEINTELLECT) && isTimerReady(GC_Timer))
					doCast(m_creature, ARCANEINTELLECT, true);

				//check group members
				Group::MemberSlotList const &a = ((Player*)master)->GetGroup()->GetMemberSlots();
				for(Group::member_citerator itr = a.begin(); itr != a.end(); itr++)
				{
					Player *tPlayer = ((Player *)master)->GetObjPlayer(itr->guid);
					if(tPlayer == NULL) continue;

					//buff group
					if(tPlayer->isAlive() &&
						!m_creature->isInCombat() &&
						isTimerReady(GC_Timer) &&
						!HasAuraName(tPlayer, ARCANEINTELLECT))
							doCast(tPlayer, ARCANEINTELLECT, true);
				}

				//no other buffs till full mana
				if(m_creature->GetPower(POWER_MANA) == m_creature->GetMaxPower(POWER_MANA))
				{
					//sLog->outError("mana is %u, total mana is %u", m_creature->GetPower(POWER_MANA), m_creature->GetMaxPower(POWER_MANA));
					//if(!HasAuraName(m_creature, "Mana Shield"))
						//doCast(m_creature, MANASHIELD);

					if(!HasAuraName(m_creature, DAMPENMAGIC) && isTimerReady(GC_Timer))
					{
						 doCast(m_creature, DAMPENMAGIC, true);
						 GiveManaBack();
						 CastedDampenMagic = true;
						 //MonsterSay("DAMPEN MAGIC", LANG_UNIVERSAL, NULL);
					}

					if(!HasAuraName(m_creature, ICEARMOR) && isTimerReady(GC_Timer))
					{
						doCast(m_creature, ICEARMOR, true);
						GiveManaBack();
						//MonsterSay("ice armor", LANG_UNIVERSAL, NULL);
						CastedArmor1 = true;
					}
				}
			} //end if !isInCombat

			m_creature->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
			m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
			if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.5 && isTimerReady(Potion_cd))
			{
				doCast(m_creature, HEALINGPOTION);
				Potion_cd = 1500;
			}
			if(m_creature->GetPower(POWER_MANA) < m_creature->GetMaxPower(POWER_MANA)*0.3 && isTimerReady(Evocation_cd))
			{
				doCast(m_creature, EVOCATION);
				Evocation_cd = EVOCATION_CD;
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
				return;
			}

			//Armour(diff);
			CheckSpellSteal(diff);
			DoNormalAttack(diff);
			Counter(diff);
		}

		void Aggro(Unit *who){}

		void Armour(const uint32 diff)
		{
			Unit *opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);

			if(!CastedArmor1 && (opponent->getClass() == CLASS_MAGE || opponent->getClass() == CLASS_PRIEST || opponent->getClass() == CLASS_WARLOCK) && isTimerReady(GC_Timer))
			{
				doCast(m_creature, MAGEARMOR, true);
				CastedArmor1 = true;
				//MonsterSay("MAGE Armor", LANG_UNIVERSAL, NULL);
			}
			else if(!CastedArmor1 && isTimerReady(GC_Timer))
			{
				doCast(m_creature, ICEARMOR, true);
				//MonsterSay("ice armor 2", LANG_UNIVERSAL, NULL);
				CastedArmor1 = true;
			} else {
				doCast(m_creature, ICEARMOR, true);
				//MonsterSay("ice armor 3", LANG_UNIVERSAL, NULL);
				CastedArmor1 = true;
				return;
			}
		} //end Armour


		void JustDied(Unit *Killer)
		{
			master->SetBotCommandState(COMMAND_FOLLOW);
			state = 1;
			next_state = 0;
			next_state_timer = 0;
		}

		void AttackStart(Unit *u)
		{
			Aggro(u);
			ScriptedAI::AttackStartCaster(u, 31);
			m_creature->AddThreat(u, 0.001f);
		}

		void KilledUnit(Unit *)
		{
			((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
			state = 1;
			next_state = 0;
			next_state_timer = 0;
		}

		void Counter(const uint32 diff)
		{
			if(opponent == NULL) return;
			if(opponent->isDead()) return;
			if(opponent->IsNonMeleeSpellCasted(true) && isTimerReady(CounterSpell_cd))
			{
				doCast(opponent, COUNTERSPELL);
				CounterSpell_cd = COUNTERSPELL_CD;
			}
		}

		void CheckSpellSteal(const uint32 diff)
		{
			if(opponent == NULL) return;
			if(opponent->isDead()) return;
			if(   //Druids:
				  HasAuraName(opponent, "Mark of the Wild") ||
				  HasAuraName(opponent, "Rejuvenation") ||
				  HasAuraName(opponent, "Regrowth") ||
				  //opponent->HasAura(33763) || //lifebloom
				  //Mage:
				  //opponent->HasAura(12043) || //POM
				  //Warlock:
				  HasAuraName(opponent, "Demon Armor") ||
				  HasAuraName(opponent, "Fel Armor") ||
				  //Priest:
				  HasAuraName(opponent, "Power Word: Shield") ||
				  HasAuraName(opponent, "Power Word: Fortitude"))
			{
				if(isTimerReady(GC_Timer))
					doCast(opponent, SPELLSTEAL);
			}
		}



		void DoNormalAttack(const uint32 diff)
		{
			AttackerSet m_attackers = master->getAttackers();
			if(opponent == NULL) return;
			if(opponent->isDead()) return;
			bool defensive = false;

			//opponent is attacking me, go defensive, ie point blank spells
			if(opponent->getVictim() && opponent->getVictim()->GetGUID() == m_creature->GetGUID())
				defensive = true;

			//sLog->outError("%s: state = %u, next_state = %u, next_state_time = %u", m_creature->GetName(), state, next_state, next_state_timer);

			switch(state)
			{
				case 0:
					if(next_state_timer <= 0)
						state = next_state;
					else
						--next_state_timer;

					break;
				case 1:
					if(opponent->GetHealth()*100 > 80)
						//state = 2;
						state = 6;
					else
						state = 5;
					break;
				case 2:
					if(isTimerReady(GC_Timer))
					{
						//MonsterSay("POLYMORPH", LANG_UNIVERSAL, NULL);
						//doCast(opponent, POLYMORPH);
						//next_state_timer = 21;
						next_state_timer = 2;
						next_state = 3;
						state = 0;
					}
				case 3:
					if(opponent->HasAura(POLYMORPH) || m_creature->HasAura(POM))
					{
						next_state_timer = (m_creature->HasAura(POM)) ? 15 : 61;
						int damage = 957 + rand()%(1215-957+1) + 1.15*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, PYROBLAST, &damage, NULL, NULL, false, NULL, NULL);
						doCast(opponent, PYROBLAST);
						//MonsterSay("PYROBLAST", LANG_UNIVERSAL, NULL);
						//GC_Timer = 1500;
						next_state = 4;
						state = 0;
					} else state = 5;
					break;
				case 4:
					if(isTimerReady(PoM_cd))
					{
						int damage = 957 + rand()%(1215-957+1) + 1.15*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, PYROBLAST, &damage, NULL, NULL, false, NULL, NULL);
						doCast(opponent, PYROBLAST);
						//MonsterSay("PYROBLAST", LANG_UNIVERSAL, NULL);
						PoM_cd = POM_CD;
						next_state = 5;
						state = 0;
						next_state_timer = 5;
					}
					else state = 5;
					break;
				case 5:
					if(m_creature->GetPower(POWER_MANA)*100/m_creature->GetMaxPower(POWER_MANA) < 25.0)
						break;

					if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.3 && isTimerReady(FirstAid_cd))
					{
						doCast(opponent, FROSTNOVA);
						GiveManaBack();
						FrostNova_cd = FROSTNOVA_CD;
						//MonsterSay("FROSTNOVA", LANG_UNIVERSAL, NULL);
						next_state = 8;
						next_state_timer = 15;
						state = 0;
					}
					if(LIVINGBOMB && isTimerReady(Living_Bomb_cd))
					{
						doCast(opponent, LIVINGBOMB);
						GiveManaBack();
						Living_Bomb_cd = LIVING_BOMB_CD;
						//MonsterSay("LIVING BOMB", LANG_UNIVERSAL, NULL);
						next_state = 7;
						next_state_timer = 2;
						state = 0;
					}
					else if(isTimerReady(Scorch_cd))
					{
						//MonsterSay("SCORCH", LANG_UNIVERSAL, NULL);
						doCast(opponent, SCORCH);
						Scorch_cd = SCORCH_CD;
						next_state = 7;
						next_state_timer = 5;
						state = 0;
					}
					else if(isTimerReady(GC_Timer))
					{
						int damage = 377 + rand()%(407-377+1) + 0.2128*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, ARCANEEXPLOSION, &damage, NULL, NULL, false, NULL, NULL);
						if(ARCANEEXPLOSION > 0 && defensive)
							 doCast(opponent, ARCANEEXPLOSION);
						else
							 doCast(opponent, FIREBALL);
						GiveManaBack();
						//MonsterSay("Arcane Explosion", LANG_UNIVERSAL, NULL);
						next_state = 7;
						next_state_timer = 2;
						state = 0;
					}
					/*if(isTimerReady(Combustion_cd))
					{
						doCast(m_creature, COMBUSTION);
						Combustion_cd = COMBUSTION_CD;
						//MonsterSay("COMBUSTION", LANG_UNIVERSAL, NULL);
						next_state = 6;
						next_state_timer = 15;
						state = 0;
					}
					else if(BLIZZARD && isTimerReady (Blizzard_cd) && m_attackers.size() > 1)
					{
						doCast(opponent, 62576);
						//GiveManaBack();
						Blizzard_cd = BLIZZARD_CD;
						//MonsterSay("Blizzard", LANG_UNIVERSAL, NULL);
						//sLog->outError("blizzard - %u", BLIZZARD);
						//GiveManaBack();
						next_state = 6;
						next_state_timer = 100;
						state = 0;
					}*/
					else if(FIREBLAST && isTimerReady(FireBlast_cd) && !defensive)
					{
						int damage = 677 + rand()%(802-677+1) + 0.4286*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, FIREBLAST, &damage, NULL, NULL, false, NULL, NULL);
						doCast(opponent, FIREBLAST);
						GiveManaBack();
						FireBlast_cd = FIREBLAST_CD;
						//MonsterSay("FIREBLAST", LANG_UNIVERSAL, NULL);
						next_state = 7;
						next_state_timer = 3;
						state = 0;
					}
					else if(BLASTWAVE && isTimerReady(BlastWave_cd) && defensive && !opponent->HasAura(BLASTWAVE))
					{
						int damage = 628 + rand()%(739-628+1) + 0.1357*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, BLASTWAVE, &damage, NULL, NULL, false, NULL, NULL);
						doCast(opponent, BLASTWAVE);
						GiveManaBack();
						BlastWave_cd = BLASTWAVE_CD;
						//MonsterSay("BLAST WAVE", LANG_UNIVERSAL, NULL);
						next_state = 7;
						next_state_timer = 2;
						state = 0;
					}
					else if(DRAGONBREATH && isTimerReady(DragonBreath_cd) && defensive && !opponent->HasAura(DRAGONBREATH))
					{
						int damage = 693 + rand()%(806-693+1) + 0.1357*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, DRAGONBREATH, &damage, NULL, NULL, false, NULL, NULL);
						doCast(opponent, DRAGONBREATH);
						GiveManaBack();
						//MonsterSay("Dragon Breath", LANG_UNIVERSAL, NULL);
						DragonBreath_cd = DRAGONBREATH_CD;
						next_state = 6;
						next_state_timer = 2;
						state = 0;
					}

					break;
				case 6:
					if(isTimerReady(GC_Timer))
					{
						int damage = 645 + rand()%(822-645+1) + 1.00*BONUS_DAMAGE;
						//m_creature->CastCustomSpell(opponent, FIREBALL, &damage, NULL, NULL, false, NULL, NULL);
						doCast(opponent, FIREBALL);
						GiveManaBack();
						next_state = 5;
						//MonsterSay("Fireball", LANG_UNIVERSAL, NULL);
						next_state_timer = 3;
						state = 0;
					}
					break;
				case 7:
					if(//HasAuraName(opponent, "Ice Block") ||
						  HasAuraName(opponent, ICEBLOCK) ||
						  HasAuraName(opponent, "Divine Shield") ||
						  opponent->HasAura(5573)) //divine protection
					{
						doCast(m_creature, BANDAGE);
						FirstAid_cd = FirstAid_cd;
						state = 9;
					}
					else state = 5;
					break;
				case 8:
					doCast(opponent, BLINK);   //Needs some serious testing
					//blink is not working, so comment out for now
					//blink = true;
					//m_creature->Say("BLINK", LANG_UNIVERSAL, NULL);
					//GC_Timer = 1500;
					//MonsterSay("Blink", LANG_UNIVERSAL, NULL);
					next_state = 9;
					next_state_timer = 5;
					state = 0;
					break;
				case 9:
					/*if(blink)
					{
						wait = true;
						wait_timer = 60;
						doCast(m_creature, BANDAGE);
						FirstAid_cd = FirstAid_cd;
						//m_creature->Say("BANDAGE", LANG_UNIVERSAL, NULL);
						blink = false;
						//GC_Timer = 1500;
						next_state = 10;
						next_state_timer = 35;
					} else*/
						next_state = 5;
					state = 0;
					break;
				case 10:
					if(!m_creature->HasAura(BANDAGE))
						state = 5;
					break;
				default:
					state = 1;
			}
		}

		void ReduceCD(const uint32 diff)
		{
			if(Living_Bomb_cd > 0)  --Living_Bomb_cd;
			if(FireBlast_cd > 0)    --FireBlast_cd;
			if(Blizzard_cd > 0)     --Blizzard_cd;
			if(BlastWave_cd > 0)    --BlastWave_cd;
			if(CounterSpell_cd > 0) --CounterSpell_cd;
			if(FrostNova_cd > 0)    --FrostNova_cd;
			if(PoM_cd > 0)          --PoM_cd;
			if(Ward_cd > 0)         --Ward_cd;
			if(Scorch_cd > 0)       --Scorch_cd;
			if(DragonBreath_cd > 0) --DragonBreath_cd;
			if(Blink_cd > 0)        --Blink_cd;
			if(Combustion_cd > 0)   --Combustion_cd;
			if(Potion_cd > 0)       --Potion_cd;
			if(Evocation_cd > 0)    --Evocation_cd;
			if(FirstAid_cd >0)      --FirstAid_cd;
			if(GC_Timer > 0)        --GC_Timer;
		}

		void ReceiveBowEmote(Player *player)
		{
			((mage_botAI*)m_creature->AI())->doCast(player, ARCANEINTELLECT, true );
		}
	};
};

void AddSC_mage_bot()
{
    new mage_bot();
}
