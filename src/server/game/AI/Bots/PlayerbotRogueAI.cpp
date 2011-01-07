#include "PlayerbotRogueAI.h"
#include "Spell.h"
class PlayerbotAI;
PlayerbotRogueAI::PlayerbotRogueAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}
PlayerbotRogueAI::~PlayerbotRogueAI(){}

void PlayerbotRogueAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    //Damage spells
    BACKSTAB = ai->getSpellIdExact("Backstab");
    SINISTER_STRIKE = ai->getSpellIdExact("Sinister Strike");
    MUTILATE = ai->getSpellIdExact("Mutilate");
    HEMORRHAGE = ai->getSpellIdExact("Hemorrhage");
    GHOSTLY_STRIKE = ai->getSpellIdExact("Ghostly Strike");
    RIPOSTE = ai->getSpellIdExact("Riposte");
    SHIV = ai->getSpellIdExact("Shiv");
    FAN_OF_KNIVES = ai->getSpellIdExact("Fan of Knives");

    //Finishing Moves
    EVISCERATE = ai->getSpellIdExact("Eviscerate");
    RUPTURE = ai->getSpellIdExact("Rupture");
    KIDNEY_SHOT = ai->getSpellIdExact("Kidney Shot");
    ENVENOM = ai->getSpellIdExact("Envenom");
    SLICE_AND_DICE = ai->getSpellIdExact("Slice and Dice");
    EXPOSE_ARMOR = ai->getSpellIdExact("Expose Armor");
    DEADLY_THROW = ai->getSpellIdExact("Deadly Throw");

    //Buffs
    STEALTH = ai->getSpellIdExact("Stealth");
    VANISH = ai->getSpellIdExact("Vanish");
    EVASION = ai->getSpellIdExact("Evasion");
    CLOAK_OF_SHADOWS = ai->getSpellIdExact("Cloak of Shadows");
    SPRINT = ai->getSpellIdExact("Sprint");
    COLD_BLOOD = ai->getSpellIdExact("Cold Blood");
    HUNGER_FOR_BLOOD = ai->getSpellIdExact("Hunger for Blood");
    BLADE_FLURRY = ai->getSpellIdExact("Blade Flurry");
    ADRENALINE_RUSH = ai->getSpellIdExact("Adrenaline Rush");
    KILLING_SPREE = ai->getSpellIdExact("Killing Spree");
    SHADOW_DANCE = ai->getSpellIdExact("Shadow Dance");

    //Openers
    CHEAP_SHOT = ai->getSpellIdExact("Cheap Shot");
    GARROTE = ai->getSpellIdExact("Garrote");
    AMBUSH = ai->getSpellIdExact("Ambush");

    //Others
    GOUGE = ai->getSpellIdExact("Gouge");
    BLIND = ai->getSpellIdExact("Blind");
    DISMANTLE = ai->getSpellIdExact("Dismantle");
    SAP = ai->getSpellIdExact("Sap");
    KICK = ai->getSpellIdExact("Kick");
    PREPARATION = ai->getSpellIdExact("Preparation");
    PREMEDITATION = ai->getSpellIdExact("Premeditation");
    SHADOWSTEP = ai->getSpellIdExact("Shadowstep");
    FEINT = ai->getSpellIdExact("Feint");
    TRICKS_OF_THE_TRADE = ai->getSpellIdExact("Tricks of the Trade");

    SHOOT = ai->getSpellIdExact("Shoot");
    THROW = ai->getSpellIdExact("Throw");

    TALENT_ASSASSINATION = MUTILATE;
    TALENT_COMBAT = ADRENALINE_RUSH;
    TALENT_SUBTELTY = PREMEDITATION;

    //uint8 talentCounter = 0;
    //if (TALENT_ASSASSINATION) talentCounter++;
    //if (TALENT_COMBAT) talentCounter++;
    //if (TALENT_SUBTELTY) talentCounter++;
    //if (talentCounter > 1) { TALENT_ASSASSINATION = 0; TALENT_COMBAT = 0; TALENT_SUBTELTY = 0; } //Unreliable Talent detection.
#pragma endregion
}

void PlayerbotRogueAI::DoNextCombatManeuver(Unit *pTarget)
{
    if (!pTarget || pTarget->isDead()) return;
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    Player *m_bot = GetPlayerBot();
    if (!m_bot || m_bot->isDead()) return;
    Unit *pVictim = pTarget->getVictim();
    Unit *m_tank = FindMainTankInRaid(GetMaster());
    if (!m_tank && m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup()) { FindMainTankInRaid(m_bot); }
    if (!m_tank) { m_tank = m_bot; }
    uint32 masterHP = GetMaster()->GetHealth()*100 / GetMaster()->GetMaxHealth();
    float pDist = m_bot->GetDistance(pTarget);
    uint8 pThreat = GetThreatPercent(pTarget);

    #pragma region Choose Target
    // Choose actions accoring to talents (ROGUE is always MELEE DPS)
    m_role = BOT_ROLE_DPS_MELEE;
    // if i am under attack and if i am not tank or offtank: change target if needed
    if (m_tank->GetGUID() != m_bot->GetGUID() && isUnderAttack() )
    {
        // Keep hitting but reduce threat
        if (CastSpell(TRICKS_OF_THE_TRADE,m_tank)) { return; }
        else if (CastSpell(VANISH,m_bot)) { return; }
        else //I cannot reduce threat so
        {
            if (pVictim && pVictim->GetGUID() == m_bot->GetGUID() && pDist <= 2) {  } // My target is almost up to me, no need to search
            else //Have to select nearest target
            {
                Unit *curAtt = GetNearestAttackerOf(m_bot);
                if (curAtt && curAtt->GetGUID() != pTarget->GetGUID())
                {
                    m_bot->SetSelection(curAtt->GetGUID());
                    DoNextCombatManeuver(curAtt); //Restart new update to get variables fixed..
                    return;
                }
            }
            //my target is attacking me
        }
    }
    #pragma endregion

    // wait until we actually reach our target b4 we actually do anything
    if (m_bot->GetDistance(pTarget)>10.0 &&
        !m_bot->HasAura(STEALTH) &&
        !m_bot->isInCombat() && CastSpell(STEALTH))
    { return; }

    TakePosition(pTarget);

    // If there's a cast stop
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }

    // wait until we actually reach our target b4 we actually do anything
    /*if (GetPlayerBot()->GetDistance(pTarget)>10.0 &&
        !HasAuraName(GetPlayerBot(),STEALTH) &&
        !GetPlayerBot()->isInCombat() && CastSpell(STEALTH))
    { return; }*/

    //Buff
    if (CastSpell(PREMEDITATION,m_bot)) { return; }
    if (CastSpell(COLD_BLOOD,m_bot)) { } //no gcd

    //PROTECT UP
    if (m_tank->GetGUID() != m_bot->GetGUID() && pVictim && pVictim->GetGUID() == m_bot->GetGUID() )
    {
        if (CastSpell(TRICKS_OF_THE_TRADE,m_tank)) { return; }
        if (CastSpell(FEINT,m_bot)) { return; }
        if (CastSpell(VANISH,m_bot)) { return; }
    }
    if (isUnderAttack() && pDist <= MELEE_RANGE && ai->GetHealthPercent() <= 85 && CastSpell(EVASION, m_bot)) { } //no GCD
    if (ai->GetHealthPercent() <= 55 && CastSpell(CLOAK_OF_SHADOWS, m_bot)) { return; }
    if (isUnderAttack() && ai->GetHealthPercent() <= 65 && CastSpell(GOUGE, m_bot)) { return; }
    if (isUnderAttack() && ai->GetHealthPercent() <= 45 && CastSpell(BLIND, m_bot)) { return; }
    if (m_bot->getRace() == (uint8) RACE_DWARF && ai->GetHealthPercent() < 75 && CastSpell(R_STONEFORM,m_bot)) { } //no gcd

    //Break spells
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && (pTarget->IsNonMeleeSpellCasted(true) || ai->GetManaPercent() < 40) && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
    else if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(KICK, pTarget)) { return; }
    else if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(GOUGE, pTarget)) { return; }
    else if (pTarget->IsNonMeleeSpellCasted(true) && m_bot->GetComboPoints() >= 1 && CastSpell(KIDNEY_SHOT, pTarget)) { return; }

    //Transfer threat
    if (m_tank->GetGUID() != m_bot->GetGUID() && CastSpell(TRICKS_OF_THE_TRADE,m_tank)) { return; }
	if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} //no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} //no GCD

    // sometimes we lose attack
    if (!m_bot->isInCombat()) {
        m_bot->Attack(pTarget, true);
    }

	if(TALENT_ASSASSINATION)
	{
		if (!m_bot->HasAura(HUNGER_FOR_BLOOD) && CastSpell(HUNGER_FOR_BLOOD,m_bot)) { return; }
		if (m_bot->GetComboPoints() < 5)
		{
		    if (CastSpell(MUTILATE, pTarget)) { return; }
		}
		else
		{
			if (!pTarget->HasAura(RUPTURE) && CastSpell(RUPTURE)) { return; }

		}
	}

    if(TALENT_COMBAT)
	{
		if (CastSpell(BLADE_FLURRY,m_bot)) { return; }
		if (ai->GetEnergyAmount() < 20 && CastSpell(ADRENALINE_RUSH,m_bot)) { return; }
		if (!CastSpell(ADRENALINE_RUSH) && CastSpell(KILLING_SPREE,m_bot,1,0,1)) { return; }
		if (m_bot->GetComboPoints() > 5)
		{
			if (!pTarget->HasAura(RUPTURE) && CastSpell(RUPTURE)) { return; }
		}
	}

    if(TALENT_SUBTELTY)
	{
		if (CastSpell(PREMEDITATION,m_bot)) {}
		if (CastSpell(SHADOW_DANCE,m_bot)) {}
		if (!CastSpell(SHADOW_DANCE,m_bot) && CastSpell(PREPARATION,m_bot)) { return; }
		if (m_bot->GetComboPoints() < 5)
		{
			if (m_bot->HasAura(SHADOW_DANCE) && !pTarget->HasInArc(M_PI,m_bot)) { if (CastSpell(AMBUSH, pTarget)) { return; } }
            if (CastSpell(GHOSTLY_STRIKE, pTarget)) { return; }
		}
		else
		{
			if (!pTarget->HasAura(RUPTURE) && CastSpell(RUPTURE)) { return; }
			if (!m_bot->HasAura(SLICE_AND_DICE) && CastSpell(SLICE_AND_DICE)) { return; }
			if (CastSpell(SHADOWSTEP,pTarget)) { }
		}
	}

    // defaults if not high enough do specialized attacks
    if (m_bot->GetComboPoints() < 5) {
        if (!pTarget->HasInArc(M_PI,m_bot)) { if (CastSpell(BACKSTAB, pTarget)) { return; } }
        if (!MUTILATE &&CastSpell(SINISTER_STRIKE,pTarget)) { return; } // Dont cast if we have mutilate, save energy for it
    } else {
        if (!m_bot->HasAura(SLICE_AND_DICE) && CastSpell(SLICE_AND_DICE)) {  return; }
		if (CastSpell(ENVENOM,pTarget)) { return; }
        if (CastSpell(EVISCERATE,pTarget)) { return; }
    }
} //end DoNextCombatManeuver


void PlayerbotRogueAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //Buff Up
    if (ChangeWeaponEnchants()) { return; }
} //end DoNonCombatActions

bool PlayerbotRogueAI::ChangeWeaponEnchants()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return false; }

    Item *weap;
    Item *poison;

    weap = m_bot->GetWeaponForAttack(BASE_ATTACK);
    if (weap && !weap->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
    {
        poison = GetAI()->FindPoisonForward();
        if(poison == NULL) return false;
        GetAI()->PoisonWeapon(*poison, poison->GetProto()->Spells[0].SpellId, TARGET_FLAG_ITEM, EQUIPMENT_SLOT_MAINHAND);
        return true;
    }
    weap = m_bot->GetWeaponForAttack(OFF_ATTACK);
    if (weap && !weap->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
    {
        poison = GetAI()->FindPoisonBackward();
        if(poison == NULL) return false;
        GetAI()->PoisonWeapon(*poison, poison->GetProto()->Spells[0].SpellId, TARGET_FLAG_ITEM, EQUIPMENT_SLOT_OFFHAND);
        return true;
    }
    return false;
}
