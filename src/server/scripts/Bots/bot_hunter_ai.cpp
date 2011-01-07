#include "ScriptPCH.h"
#include "bot_hunter_ai.h"
#include "Group.h"

class hunter_bot : public CreatureScript
{
public:
    hunter_bot() : CreatureScript("hunter_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new hunter_botAI(pCreature);
    }

	struct hunter_botAI : public bot_ai
	{
		hunter_botAI(Creature *c) :bot_ai(c)
		{
			Reset();
			pet = NULL;
		}

		bool oom_spam;

		Unit *opponent;
		Creature *pet;

		int32 ArcaneShot_cd;
		int32 ChimeraShot_Timer;
		int32 SilencingShot_Timer;
		int32 AimedShot_Timer;

		void Reset()
		{
			oom_spam = false;

			opponent = NULL;

			GC_Timer = 0;
			ArcaneShot_cd = 0;
			ChimeraShot_Timer = 0;
			SilencingShot_Timer = 0;
			AimedShot_Timer = 0;

			if (master) {
				setStats(CLASS_HUNTER, m_creature->getRace(), master->getLevel());
		   }
		}

	   void CreatePet()
		{
			pet = master->GetBotsPet(60238);

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

			ReduceCD(diff);

			if(IAmDead()) return;

			if(!m_creature->isInCombat())
			{
				DoNonCombatActions();
			}

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

			DoNormalAttack(diff);
			ScriptedAI::UpdateAI(diff);

			//if low on health, drink a potion
			if(m_creature->GetHealth() < m_creature->GetMaxHealth()*0.6 && isTimerReady(Potion_cd))
			{
				doCast(m_creature, HEALINGPOTION);
				Potion_cd = 1500;
			}

			//if low on mana, drink a potion
			 if(m_creature->GetPower(POWER_MANA) < m_creature->GetMaxPower(POWER_MANA)*0.2)
			{
				if(isTimerReady(Potion_cd))
				{
					doCast(m_creature, MANAPOTION);
					//MonsterSay("MANA POTION", LANG_UNIVERSAL, NULL);
					Potion_cd = 1500;
				}
			 }

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
		}

		void Aggro(Unit *who){}

		void JustDied(Unit *Killer)
		{
			master->SetBotCommandState(COMMAND_FOLLOW);
		}

		void KilledUnit(Unit *)
		{
			((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
		}


		void AttackStart(Unit *u)
		{
			Aggro(u);
			ScriptedAI::AttackStartCaster(u, 33);
			m_creature->AddThreat(u, 0.001f);
			u->AddThreat(m_creature, 0.001f);
		}

		void DoNormalAttack(const uint32 diff)
		{
			AttackerSet m_attackers = master->getAttackers();
			if(opponent == NULL) return;
			if(opponent->isDead()) return;


			// try to get rid of enrage
			if (TRANQ_SHOT && HasAuraName(opponent, "Enrage")) {
				m_creature->InterruptNonMeleeSpells(true, AUTO_SHOT);
			   // m_creature->MonsterSay("Tranquility shot!", LANG_UNIVERSAL, NULL);
				doCast(opponent, TRANQ_SHOT);
				GiveManaBack();
			  //  doCast(opponent, AUTO_SHOT);
			  //  return;
			}

			// silence it
			if(SILENCING_SHOT && opponent->IsNonMeleeSpellCasted(true) && isTimerReady(SilencingShot_Timer))
			{
				doCast(opponent, SILENCING_SHOT);
				SilencingShot_Timer = 200;
				GiveManaBack();
			  //  doCast(opponent, AUTO_SHOT);
			  //  return;
			}

			// mark it
			if (!HasAuraName(opponent, "Hunter's Mark")) {
				doCast(opponent, HUNTERS_MARK);
				GiveManaBack();
			  //  doCast(opponent, AUTO_SHOT);
			  //  return;
			}

			// sting it
			if (SCORPID_STING && !opponent->HasAura(SCORPID_STING, m_creature->GetGUID())) {
				m_creature->InterruptNonMeleeSpells(true, AUTO_SHOT);
				doCast(opponent, SCORPID_STING);
			   // m_creature->MonsterSay("Scorpid Sting!", LANG_UNIVERSAL, NULL);
				GiveManaBack();
			   // doCast(opponent, AUTO_SHOT);
			   // return;
			}

			 if (CHIMERA_SHOT && isTimerReady(ChimeraShot_Timer)) {
				m_creature->InterruptNonMeleeSpells(true, AUTO_SHOT);
				doCast(opponent, CHIMERA_SHOT);
				ChimeraShot_Timer = 100;
			   // m_creature->MonsterSay("Chimera Sting!", LANG_UNIVERSAL, NULL);
				GiveManaBack();
			  //  doCast(opponent, AUTO_SHOT);
			  //  return;
			}

			if(ARCANE_SHOT && isTimerReady(ArcaneShot_cd))
			{
				m_creature->InterruptNonMeleeSpells( true, AUTO_SHOT );
				doCast(opponent, ARCANE_SHOT);
			   // m_creature->MonsterSay("Arcane shot!", LANG_UNIVERSAL, NULL);
				ArcaneShot_cd = 60;
				GiveManaBack();
			  //  doCast(opponent, AUTO_SHOT);
			  //  return;
			}

			if(AIMED_SHOT && isTimerReady(AimedShot_Timer))
			{
				m_creature->InterruptNonMeleeSpells( true, AUTO_SHOT );
				doCast(opponent, AIMED_SHOT);
			   // m_creature->MonsterSay("Arcane shot!", LANG_UNIVERSAL, NULL);
				AimedShot_Timer = 100;
				GiveManaBack();
			  //  doCast(opponent, AUTO_SHOT);
			  //  return;
			}


			doCast(opponent, AUTO_SHOT);

		}

		void DoNonCombatActions()
		{
			if (ASPECT_OF_THE_WILD && !HasAuraName(m_creature, "Aspect of the Wild")) {
				doCast(master, ASPECT_OF_THE_WILD);
			}
		}

		void ReduceCD(const uint32 diff)
		{
			if(GC_Timer > 0)        --GC_Timer;
			if(ArcaneShot_cd > 0)       --ArcaneShot_cd;
			if(ChimeraShot_Timer > 0)       --ChimeraShot_Timer;
			if(SilencingShot_Timer > 0)     --SilencingShot_Timer;
			if(AimedShot_Timer > 0)         --AimedShot_Timer;
		}

		void ReceiveEmote(Player *player, uint32 emote)
		{
			//debug to see what auras are on bot
			if(emote == TEXTEMOTE_BONK) ((hunter_botAI*)m_creature->AI())->listAuras(m_creature);

			//debug to see what auras are on me
			if(emote == TEXTEMOTE_SALUTE) ((hunter_botAI*)m_creature->AI())->listAuras(player);


			if(emote == TEXTEMOTE_STAND)
			{
				if(m_creature->GetCharmerGUID() != player->GetGUID())
				{
					m_creature->HandleEmoteCommand(TEXTEMOTE_RUDE);
					return;
				}

				player->SetBotCommandState (COMMAND_STAY);
			}
			if(emote == TEXTEMOTE_WAVE)
			{
				player->SetBotCommandState (COMMAND_FOLLOW);
			}


		}

	};
};

void AddSC_hunter_bot()
{
    new hunter_bot();
}
