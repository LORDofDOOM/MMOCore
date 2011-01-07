#include "PlayerbotPaladinAI.h"

class PlayerbotAI;
PlayerbotPaladinAI::PlayerbotPaladinAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}
PlayerbotPaladinAI::~PlayerbotPaladinAI(){}

void PlayerbotPaladinAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    //heals
    FLASH_OF_LIGHT = ai->getSpellIdExact("Flash of Light");
    HOLY_LIGHT = ai->getSpellIdExact("Holy Light");
    HOLY_SHOCK = ai->getSpellIdExact("Holy Shock");
    CLEANSE = ai->getSpellIdExact("Cleanse");
    if (!CLEANSE) CLEANSE = ai->getSpellIdExact("Purify");
    LOH = ai->getSpellIdExact("Lay on Hands");
    SACRED_SHIELD = ai->getSpellIdExact("Sacred Shield");
    BEACON_OF_LIGHT = ai->getSpellIdExact("Beacon of Light");
    DIVINE_FAVOR = ai->getSpellIdExact("Divine Favor");
    REDEMPTION = ai->getSpellIdExact("Redemption");

    //Damages
    JOL = ai->getSpellIdExact("Judgement of Light");
    JOW = ai->getSpellIdExact("Judgement of Wisdom");
	JOJ = ai->getSpellIdExact("Judgement of Justice");
    HAMMER_OF_WRATH = ai->getSpellIdExact("Hammer of Wrath");
    EXORCISM = ai->getSpellIdExact("Exorcism");
    HOLY_WRATH = ai->getSpellIdExact("Holy Wrath");
    CONSECRATION = ai->getSpellIdExact("Consecration");
    AVENGERS_SHIELD = ai->getSpellIdExact("Avenger's Shield");
    SHIELD_OF_RIGHTEOUSNESS = ai->getSpellIdExact("Shield of Righteousness");
    HOTR = ai->getSpellIdExact("Hammer of the Righteous");
    CRUSADER_STRIKE = ai->getSpellIdExact("Crusader Strike");
    DIVINE_STORM = ai->getSpellIdExact("Divine Storm");

    //CC
    HAMMER_OF_JUSTICE = ai->getSpellIdExact("Hammer of Justice");
    REPENTANCE = ai->getSpellIdExact("Repentance");

    //Self buffs
    SOL = ai->getSpellIdExact("Seal of Light");
    SOW = ai->getSpellIdExact("Seal of Wisdom");
    SOR = ai->getSpellIdExact("Seal of Righteousness");
    SOC = ai->getSpellIdExact("Seal of Command");
	SOV = ai->getSpellIdExact("Seal of Vengeance");
	if (!SOV) SOV = ai->getSpellIdExact("Seal of Corruption");
    DIVINE_PLEA = ai->getSpellIdExact("Divine Plea");
    HOLY_SHIELD = ai->getSpellIdExact("Holy Shield");
    RIGHTEOUS_FURY = ai->getSpellIdExact("Righteous Fury");
    DIVINE_SHIELD = ai->getSpellIdExact("Divine Shield");
    if (!DIVINE_SHIELD) DIVINE_SHIELD = ai->getSpellIdExact("Divine Protection");
    AVENGING_WRATH = ai->getSpellIdExact("Avenging Wrath");

    //AURAS
    DEVOTION_AURA = ai->getSpellIdExact("Devotion Aura");
    RETRIBUTION_AURA = ai->getSpellIdExact("Retribution Aura");
    CONCENTRATION_AURA = ai->getSpellIdExact("Concentration Aura");
    FIRE_AURA = ai->getSpellIdExact("Fire Resistance Aura");
    FROST_AURA = ai->getSpellIdExact("Frost Resistance Aura");
    SHADOW_AURA = ai->getSpellIdExact("Shadow Resistance Aura");
    CRUSADER_AURA = ai->getSpellIdExact("Crusader Aura");

    //Blessings
    BOW = ai->getSpellIdExact("Blessing of Wisdom");
    BOM = ai->getSpellIdExact("Blessing of Might");
    BOS = ai->getSpellIdExact("Blessing of Sanctuary");
    BOK = ai->getSpellIdExact("Blessing of Kings");
    GBOW = ai->getSpellIdExact("Greater Blessing of Wisdom");
    GBOM = ai->getSpellIdExact("Greater Blessing of Might");
    GBOS = ai->getSpellIdExact("Greater Blessing of Sanctuary");
    GBOK = ai->getSpellIdExact("Greater Blessing of Kings");

    //Hands
    HOF = ai->getSpellIdExact("Hand of Freedom");
    HOR = ai->getSpellIdExact("Hand of Reckoning");
    HOS = ai->getSpellIdExact("Hand of Salvation");
    HOP = ai->getSpellIdExact("Hand of Protection");
    DIVINE_SACRIFICE = ai->getSpellIdExact("Divine Sacrifice");

    //Taunt
    RIGHTEOUS_DEFENSE = ai->getSpellIdExact("Righteous Defense");

    FORBEARANCE = 25771;
	AOW = 53488;

    TALENT_RETRI = CRUSADER_STRIKE;
    TALENT_PROT = HOLY_SHIELD;
    TALENT_HOLY = HOLY_SHOCK;

    uint8 talentCounter = 0;
    if (TALENT_RETRI) talentCounter++;
    if (TALENT_PROT) talentCounter++;
    if (TALENT_HOLY) talentCounter++;
    //if (talentCounter > 1) { TALENT_RETRI = 0; TALENT_PROT = 0; TALENT_HOLY = 0; } //Unreliable Talent detection.

    #pragma endregion
}

void PlayerbotPaladinAI::DoNextCombatManeuver(Unit *pTarget)
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

    uint8 reqHeal = 0;
    uint8 OwnPartyHP = GetHealthPercentRaid(m_bot, reqHeal);


    // Fill mana if needed
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && ai->GetManaPercent() < 20 && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
    if (ai->GetManaPercent() < 30 && CastSpell (DIVINE_PLEA, m_bot)) { return; }

    // If hp is too low divine shield
    if (ai->GetHealthPercent() < 20 && (!m_bot->HasAura(DIVINE_SHIELD) || !m_bot->HasAura(HOP) || !m_bot->HasAura(SACRED_SHIELD)))
    {
        if (!m_bot->HasAura(FORBEARANCE))
        {
            if (CastSpell(DIVINE_SHIELD,m_bot)) { return; }
            if (CastSpell(HOP,m_bot)) { return; }
        }
        else if (CastSpell(SACRED_SHIELD,m_bot)) { return; }
    }

    // if i am under attack and if i am not tank or offtank: change target if needed
    if (m_tank->GetGUID() != m_bot->GetGUID() && !TALENT_PROT && isUnderAttack() )
    {
        // Keep hitting but reduce threat
        if (CastSpell(HOS,m_bot,true,true)) { }
        //else if (m_bot->getRace() == (uint8) RACE_NIGHTELF && CastSpell(R_SHADOWMELD,m_bot)) { return; }
        else //I cannot reduce threat so
        {
            if (pVictim && pVictim->GetGUID() == m_bot->GetGUID() && pDist <= 2) {  } // My target is almost up to me, no need to search
            else //Have to select nearest target
            {
                Unit *curAtt = GetNearestAttackerOf(m_bot);
                if (curAtt && curAtt->GetGUID() != pTarget->GetGUID())
                {
                    m_bot->SetSelection(curAtt->GetGUID());
                    //ai->AddLootGUID(curAtt->GetGUID());
                    DoNextCombatManeuver(curAtt); //Restart new update to get variables fixed..
                    return;
                }
            }
            //my target is attacking me
        }
    }


    #pragma region Choose Actions
    // Choose actions accoring to talents
    if (m_tank->GetGUID() == m_bot->GetGUID()) // Hey! I am Main Tank
    {
        if (TALENT_PROT) { m_role=BOT_ROLE_TANK; } //Just Keep Tanking
        else
        {
            if (TALENT_RETRI) {
                if ((ai->GetHealthPercent() <= 40 || masterHP <40 ) && (ai->GetManaPercent() >= 40)) { m_role = BOT_ROLE_SUPPORT; }
                else if (OwnPartyHP < 40 && ai->GetManaPercent() >= 30) { m_role = BOT_ROLE_SUPPORT; }
                else { m_role = BOT_ROLE_TANK; } //have no shield but can tank if you think so
            }
            else if (TALENT_HOLY) //I am both healer and tank?? Hmm
            {
                if ((ai->GetHealthPercent() <= 70 || masterHP <70 ) && (ai->GetManaPercent() >= 50))m_role = BOT_ROLE_SUPPORT;
                else if (OwnPartyHP < 20 && ai->GetManaPercent() >= 30) { m_role = BOT_ROLE_SUPPORT; }
                else m_role = BOT_ROLE_TANK;
            }
            else { m_role = BOT_ROLE_TANK; } //Unknown build or low level
        }
    }
    else if (TALENT_RETRI) {
        if ((ai->GetHealthPercent() <= 40 || masterHP <40 ) && (ai->GetManaPercent() >= 40)) { m_role = BOT_ROLE_SUPPORT; }
        else if (OwnPartyHP < 40 && ai->GetManaPercent() >= 30) {m_role = BOT_ROLE_SUPPORT;}
        else { m_role = BOT_ROLE_DPS_MELEE; }
    }
    else if (TALENT_PROT) {
        if ((ai->GetHealthPercent() <= 30 || masterHP <40 ) && (ai->GetManaPercent() >= 20)) { m_role = BOT_ROLE_SUPPORT; }
        else if (OwnPartyHP < 40 && ai->GetManaPercent() >= 40) { m_role = BOT_ROLE_SUPPORT; }
        else { m_role = BOT_ROLE_OFFTANK; }
    }
    else if (TALENT_HOLY) { m_role = BOT_ROLE_SUPPORT; }
    else { m_role = BOT_ROLE_DPS_MELEE; } //Unknown build or low level.. Mainly attack

	//takepos
    if (m_role == BOT_ROLE_SUPPORT || m_role == BOT_ROLE_HEALER) TakePosition(pTarget,BOT_ROLE_DPS_MELEE,0.5f);
    else TakePosition(pTarget,m_role);

    // If there's a cast stop
    if(m_bot->HasUnitState(UNIT_STAT_CASTING)) return;

    Unit *target = DoSelectLowestHpFriendly(40, 1000);
    switch(m_role)
    {
        #pragma region BOT_ROLE_SUPPORT
        case BOT_ROLE_SUPPORT:

            ChangeAura(CONCENTRATION_AURA);
            if (!TALENT_PROT && m_tank->GetGUID() != m_bot->GetGUID()) m_bot->RemoveAurasDueToSpell(RIGHTEOUS_FURY);
            // Choose Seal
            if (SOW && ai->GetManaPercent() <= 30) {    if (CastSpell(SOW,m_bot)) { return; } }
            else if (m_bot->HasAura(SOW) && ai->GetManaPercent() < 85) { } // Paladin was striving for mana, keep until he got most of his mana back
            else if(SOL && ai->GetHealthPercent() < 40) { if(CastSpell(SOL,m_bot)) { return; } }
            else if(CastSpell(SOR, m_bot)) { return; }

            if (!m_bot->HasAura(FORBEARANCE) && CastSpell(AVENGING_WRATH,m_bot)) { } // no gcd

            if (DoSupportRaid(m_bot)) { return; }
            //heal pets and bots
            if(target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth()) ) { return; }

            if (ai->GetManaPercent() <= 80 && CastSpell(JOW,pTarget,true,true)) { return; }

            // Use Spells only if mana is sufficient..
            if(ai->GetManaPercent() < offensiveSpellThreshold ) return;

        break;
        #pragma endregion

        #pragma region BOT_ROLE_TANK / BOT_ROLE_OFFTANK
        case BOT_ROLE_TANK:
        case BOT_ROLE_OFFTANK:

            ChangeAura(DEVOTION_AURA);
            if (CastSpell(RIGHTEOUS_FURY,m_bot)) { return; }
            // Choose Seal
            if (SOW && ai->GetManaPercent() <= 30) { if (CastSpell(SOW,m_bot)) { return; } }
            else if (m_bot->HasAura(SOW) && ai->GetManaPercent() < 85) { } // Paladin was striving for mana, keep until he got most of his mana back
            else if (SOL && ai->GetHealthPercent() < 40) { if (CastSpell(SOL,m_bot)) { return; } }
            else if (CastSpell(SOR,m_bot)) { return; }

            // We are tank/offtank threat is not an issiue;
            // Use taunts only if helping target is not main tank..
            // Taunt if needed (Only for master)
            if(GetMaster()->GetGUID() != m_tank->GetGUID())
            {
                // Taunt if needed (Only for master)
                Unit *curAtt = GetAttackerOf(GetMaster());
                if (curAtt)
                {
                    if (isUnderAttack(GetMaster(),2) && CastSpell(RIGHTEOUS_DEFENSE, GetMaster())) { return; }
                    if (CastSpell(HOR, curAtt,true,true))  { } //No GCD
                }
            }
            // My target is not attacking me, taunt..
            if ( m_tank->GetGUID() == m_bot->GetGUID() && pVictim && pVictim->GetGUID() != m_bot->GetGUID() && CastSpell(HOR, pTarget,true,true) )  { } //NO GCD

            // Tank specials
            if (TALENT_PROT && ai->GetManaPercent() < 90 && CastSpell (DIVINE_PLEA, m_bot)) { return; } //Prot paladin always uses this..
            if (pVictim && pVictim->GetGUID() == m_bot->GetGUID() && CastSpell(HOLY_SHIELD,m_bot)) { return; }
            if (CastSpell(AVENGERS_SHIELD,pTarget,true,true)) { return; }
            if (CastSpell(HOTR,pTarget,true,true)) { return; }
            if (CastSpell(HOLY_WRATH,pTarget,true,true)){ return; }
            if (CastSpell(CONSECRATION,pTarget)) { return; }
            if (m_bot->getRace() == (uint8) RACE_DWARF && CastSpell(R_STONEFORM,m_bot)) { return; }

            if (DoSupportRaid(m_bot)) { return; }
            //heal pets and bots
            if(target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth()) ) { return; }


        break;
		#pragma endregion

		#pragma region BOT_ROLE_DPS_MELEE
		case BOT_ROLE_DPS_MELEE:

			ChangeAura(RETRIBUTION_AURA);
			if (!TALENT_PROT && m_tank->GetGUID() != m_bot->GetGUID()) m_bot->RemoveAurasDueToSpell(RIGHTEOUS_FURY);
			if (CastSpell(SOV,m_bot)) { return; }

			if (CastSpell (HAMMER_OF_JUSTICE, pTarget)) { return; }
			if (!m_bot->HasAura(FORBEARANCE) && CastSpell(AVENGING_WRATH,m_bot)) {} //no gcd
			if (CastSpell(JOW,pTarget)) { return; }
			if (CastSpell(DIVINE_STORM, pTarget)) { return; }
			if (CastSpell(CRUSADER_STRIKE, pTarget)) { return; }
			if (GetAI()->GetHealthPercent(*pTarget)<20 && CastSpell(HAMMER_OF_WRATH, pTarget)) { return; }
			if (CastSpell(CONSECRATION,pTarget)) { return; }
			if (m_bot->HasAura(AOW) && CastSpell(EXORCISM,pTarget)) { return; }
			if (CastSpell(HOLY_WRATH,pTarget)) { return; }

		break;
        #pragma endregion

    }
    #pragma region PaladinCommon
    // Shared dps spells
    if (pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_HUMANOID && pTarget->IsNonMeleeSpellCasted(true) && CastSpell (REPENTANCE, pTarget)) { return; }
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && pTarget->IsNonMeleeSpellCasted(true) && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} // no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} // no GCD

    // If at threat limit, stop
    if(pThreat > threatThreshold && !TALENT_PROT && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack())
    {
        if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
        {
            m_bot->SetSelection(m_tank->getVictim()->GetGUID());
            return;
        }
        else
        {
            if (CastSpell(HOS,m_bot)) { return; } //Lets see if we can manage with HOS
            else { return; } //use no spells and wait threat to be reduced
        }
    }
    // Continue attacking if theres excess mana (for healers)
    if (m_role == BOT_ROLE_SUPPORT && ai->GetManaPercent() < offensiveSpellThreshold) { return; }

    if (GetAI()->GetHealthPercent(*pTarget)<20 && CastSpell(HAMMER_OF_WRATH, pTarget,true,true)) { return; } //no gcd but cast
    if (CastSpell (HAMMER_OF_JUSTICE, pTarget)) { return; }
    if (CanCast(JOW,pTarget,true) &&
        ( ( ai->GetManaPercent() <= 70 && ai->GetHealthPercent() > 90)
        || ( ai->GetManaPercent() <= 50 && ai->GetHealthPercent() > 75)
        || ( ai->GetManaPercent() <= 20 && ai->GetHealthPercent() > 20) )
        && CastSpell(JOW,pTarget,false)) { return; }
    else if (CastSpell(JOL,pTarget),true,true) { return; }
    if (CastSpell(SHIELD_OF_RIGHTEOUSNESS,pTarget,true,true)) { return; }
    if (CastSpell (DIVINE_STORM, pTarget,true,true)) { return; }
    if (CastSpell (CRUSADER_STRIKE, pTarget,true,true)) { return; }
    if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; } //no GCD but cast
    if (isUnderAttack(m_tank,4) && CastSpell(HOLY_WRATH,pTarget,true,true)) { return; }
    if (isUnderAttack(m_tank,4) && CastSpell(CONSECRATION,pTarget)) { return; }
    if (CastSpell(HOLY_SHOCK,pTarget,true,true)) { return; }
    if (m_role != BOT_ROLE_SUPPORT && ai->GetManaPercent() > 60 && OwnPartyHP < 65 && DoSupportRaid(m_bot)) { return; } //if there is spare time and mana, do healz and other stuff..
    else if (m_role != BOT_ROLE_SUPPORT && ai->GetManaPercent() > 30 && DoSupportRaid(m_bot,30,false,false,false,true,false)) { return; }
    if (CastSpell(EXORCISM,pTarget,true,true)) { return; }


    // drink potion if support / healer (Other builds simply overuse mana and waste mana pots)
    if(ai->GetManaPercent() < 5 && (m_role == BOT_ROLE_SUPPORT || m_role == BOT_ROLE_HEALER) )
    {
        Item *pItem = ai->FindPotion();
        if(pItem != NULL)
        {
            if (pItem->GetSpell() && m_bot->HasSpellCooldown(pItem->GetSpell()) ) { return; } //pot is in cooldown
            ai->UseItem(*pItem);
        }
    }
    #pragma endregion

} //end DoNextCombatManeuver

void PlayerbotPaladinAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //buff and heal raid
    if (DoSupportRaid(m_bot)) { return; }

    //heal pets and bots
    Unit *target = DoSelectLowestHpFriendly(40, 1000);
    if (target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth())) { return; }

    //mana/hp check
    //Don't bother with eating, if low on hp, just let it heal themself
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (m_bot->GetHealth() < m_bot->GetMaxHealth() && CastSpell(FLASH_OF_LIGHT,m_bot)) { return; }
    if (ai->GetManaPercent() < 70) { ai->Feast(); }
} //end DoNonCombatActions

bool PlayerbotPaladinAI::HealTarget(Unit *target, uint8 hp)
{
    if(!target || target->isDead()) return false;
    Player *m_bot = GetPlayerBot();

    if(hp < 10 && m_bot->isInCombat() && CastSpell(LOH, target)) { return true; }
    if(hp < 10 && m_bot->isInCombat() && CastSpell(SACRED_SHIELD,target)) { return true; }
    if(hp < 15 && m_bot->isInCombat() && CastSpell(HOP,target)) { return true; }
    if(hp < 20 && m_bot->isInCombat() && CastSpell(BEACON_OF_LIGHT,target)) { return true; }
    if(hp < 30 && CastSpell(HOLY_SHOCK,target,true,true,true)) { return true; }
    if(hp < 30 && m_bot->isInCombat() && CanCast(DIVINE_FAVOR,m_bot,true) && CanCast(HOLY_LIGHT,target,true) ) { CastSpell(DIVINE_FAVOR, m_bot,false); return CastSpell(HOLY_LIGHT,target,false); } //No gcd
    if(hp < 30 && CastSpell(FLASH_OF_LIGHT,target,true,true)) { return true; }
    if(hp < 40 && m_bot->getRace() == (uint8) RACE_DRAENEI && CastSpell(R_GIFT_OF_NAARU,target)) { return true; } // no GCD but has cast
    if(hp < 65 && CastSpell(HOLY_LIGHT,target,true,true)) { return true; }
    if(hp < 85 && CastSpell(FLASH_OF_LIGHT,target,true,true)) { return true; }
    if(hp < 95 && m_bot->isInCombat() && CastSpell(BEACON_OF_LIGHT,target)) { return true; }

    return false;
} //end HealTarget

bool PlayerbotPaladinAI::CureTarget(Unit *target)
{
    if (!target || target->isDead()) { return false; }
    if (castDispel(CLEANSE, target)) { return true; }
    return false;

} //end CureTarget

bool PlayerbotPaladinAI::BuffPlayer(Unit *target)
{
    if(!target || target->isDead()) return false;
    Player *m_bot = GetPlayerBot();

    // Check if target already has a blessing by me..
    if (HasAuraName(target,BOW,m_bot->GetGUID()) ||
    HasAuraName(target,BOK,m_bot->GetGUID()) ||
    HasAuraName(target,BOM,m_bot->GetGUID()) ||
    HasAuraName(target,BOS,m_bot->GetGUID()) ||
    HasAuraName(target,GBOW,m_bot->GetGUID()) ||
    HasAuraName(target,GBOK,m_bot->GetGUID()) ||
    HasAuraName(target,GBOM,m_bot->GetGUID()) ||
    HasAuraName(target,GBOS,m_bot->GetGUID())
    ) return false;

#pragma region Choose Buff > Class
    switch(target->getClass())
    {
        case CLASS_MAGE:
        case CLASS_WARLOCK:
            if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
            else if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
            else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
            break;
        case CLASS_PRIEST:
            if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
            else if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
            else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
            break;
        case CLASS_HUNTER:
            if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
            else if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
            else if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
            else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
            break;
        case CLASS_ROGUE:
            if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
            else if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
            else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
            break;
        case CLASS_WARRIOR:
        case CLASS_DEATH_KNIGHT:
            if (target->GetUnitDodgeChance() + target->GetUnitParryChance() > 40)
            {
                if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
                else if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
                else if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
            }
            else
            {
                if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
                else if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
                else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
            }
            break;
        case CLASS_DRUID:
        case CLASS_SHAMAN:
        case CLASS_PALADIN:
            if (target->GetMaxPower(target->getPowerType()) > target->GetMaxHealth())
            {
                if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
                else if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
                else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
                else if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
            }
            else if (target->GetUnitDodgeChance() + target->GetUnitParryChance() > 40)
            {
                if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
                else if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
                else if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
                else if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
            }
            else
            {
                if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
                else if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
                else if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
                else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
            }
            break;

        default:
                if (CanCast(GBOK,target) && !HasAuraName(target,BOK) && !HasAuraName(target,GBOK) ) return CastSpell(GBOK,target,false);
                else if (CanCast(GBOM,target) && !HasAuraName(target,BOM) && !HasAuraName(target,GBOM) ) return CastSpell(GBOM,target,false);
                else if (CanCast(GBOW,target) && !HasAuraName(target,BOW) && !HasAuraName(target,GBOW) ) return CastSpell(GBOW,target,false);
                else if (CanCast(GBOS,target) && !HasAuraName(target,BOS) && !HasAuraName(target,GBOS) ) return CastSpell(GBOS,target,false);
                break;
    }
#pragma endregion

    return false;
}

bool PlayerbotPaladinAI::RezTarget (Unit *target)
{
    if(!target || target->isAlive()) return false;
    Player *m_bot = GetPlayerBot();
    if (target->IsNonMeleeSpellCasted(true)) { return false; } //Already resurrected
    if (m_bot->isInCombat()) { return false; }

    if (!CanCast(REDEMPTION,target)) return false;
    std::string msg = "Rezzing ";
    msg += target->GetName();
    GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
    return CastSpell(REDEMPTION, target, false);
}

bool PlayerbotPaladinAI::ChangeAura(uint32 aura)
{
    Player *m_bot = GetPlayerBot();
    if(!aura) return false;

    if(!CanCast(aura,m_bot)) return false;

    if(m_bot->HasAura(aura))
    {
        if (aura == DEVOTION_AURA)
        {
            if (ChangeAura(FIRE_AURA)) return true;
            if (ChangeAura(FROST_AURA)) return true;
            if (ChangeAura(SHADOW_AURA)) return true;
            return true;
        }
        else return ChangeAura(DEVOTION_AURA);
    }
    return CastSpell(aura,m_bot,false);
}

