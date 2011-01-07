/*
Name : PlayerbotDruidAI.cpp
Complete: maybe around 70%

Limitations:    - Talent build decision is made by key talent spells, which makes them viable only after level 50-ish.. Until then default behaviour is a combination of Feral/balance type..
                - Tanking bots should taunt if any group member is under attack, currently only saves master
                - Tree of life form transition is late and may never occur, due to healing bots attacking priority at full mana.
                - Boomkin's support roles are not fully covered.. For example -> off healing
                - Situations needing Abolish Disease casting : limited / non-existant..

Authors : SwaLLoweD, rrtn, Natsukawa
Version : 0.40
*/
#include "PlayerbotDruidAI.h"

class PlayerbotAI;

PlayerbotDruidAI::PlayerbotDruidAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}

PlayerbotDruidAI::~PlayerbotDruidAI(){}

void PlayerbotDruidAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
#pragma region SpellId Fill
    // Balance Spells
    MOONFIRE = ai->getSpellIdExact("Moonfire"); //attacks
    WRATH = ai->getSpellIdExact("Wrath");
    STARFIRE = ai->getSpellIdExact("Starfire");
    STARFALL = ai->getSpellIdExact("Starfall");
    FORCE_OF_NATURE = ai->getSpellIdExact("Force of Nature");
    TYPHOON = ai->getSpellIdExact("Typhoon");
    HURRICANE = ai->getSpellIdExact("Hurricane");
    INSECT_SWARM = ai->getSpellIdExact("Insect Swarm");

    CYCLONE = ai->getSpellIdExact("Cyclone");
    ROOTS = ai->getSpellIdExact("Entangling Roots");
    NATURES_GRASP = ai->getSpellIdExact("Nature's Grasp");

    HIBERNATE = ai->getSpellIdExact("Hibernate");
    FAERIE_FIRE = ai->getSpellIdExact("Faerie Fire");


    // Bear Form Spells
    MAUL = ai->getSpellIdExact("Maul");
    BASH = ai->getSpellIdExact("Bash");
    LACERATE = ai->getSpellIdExact("Lacerate");
    MANGLE_BEAR = ai->getSpellIdExact("Mangle (Bear)");
    SWIPE_BEAR = ai->getSpellIdExact("Swipe (Bear)");

    DEMORALIZING_ROAR = ai->getSpellIdExact("Demoralizing Roar");
    GROWL = ai->getSpellIdExact("Growl");
    CHALLENGING_ROAR = ai->getSpellIdExact("Challenging Roar");

    ENRAGE = ai->getSpellIdExact("Enrage");
    FERAL_CHARGE_BEAR = ai->getSpellIdExact("Feral Charge - Bear");
    FRENZIED_REGENERATION = ai->getSpellIdExact("Frenzied Regeneration");


    //Cat Attack type's
    RAKE = ai->getSpellIdExact("Rake"); //40 energy
    CLAW = ai->getSpellIdExact("Claw"); //45
    MANGLE_CAT = ai->getSpellIdExact("Mangle (Cat)"); //45
    SHRED = ai->getSpellIdExact("Shred");

    RIP = ai->getSpellIdExact("Rip"); //30
    FEROCIOUS_BITE = ai->getSpellIdExact("Ferocious Bite"); //35
    SAVAGE_ROAR = ai->getSpellIdExact("Savage Roar");
    MAIM = ai->getSpellIdExact("Maim"); //35

    FERAL_CHARGE_CAT = ai->getSpellIdExact("Feral Charge - Cat");
    COWER = ai->getSpellIdExact("Cower"); //20
    TIGERS_FURY = ai->getSpellIdExact("Tiger's Fury");

    // Feral General
    BERSERK = ai->getSpellIdExact("Berserk");
    FAERIE_FIRE_FERAL = ai->getSpellIdExact("Faerie Fire (Feral)"); //debuffs

    //buffs
    MARK_OF_THE_WILD = ai->getSpellIdExact("Mark of the Wild"); //buffs
    GIFT_OF_THE_WILD = ai->getSpellIdExact("Gift of the Wild");
    THORNS = ai->getSpellIdExact("Thorns");
    SURVIVAL_INSTINCTS = ai->getSpellIdExact("Survival Instincts");

    // Restoration Spells
    LIFEBLOOM = ai->getSpellIdExact("Lifebloom");
    REJUVENATION = ai->getSpellIdExact("Rejuvenation"); //heals
    REGROWTH = ai->getSpellIdExact("Regrowth");
    NOURISH = ai->getSpellIdExact("Nourish");
    SWIFTMEND = ai->getSpellIdExact("Swiftmend");
    HEALING_TOUCH = ai->getSpellIdExact("Healing Touch");
    INNERVATE = ai->getSpellIdExact("Innervate");
    WILD_GROWTH = ai->getSpellIdExact("Wild Growth");
    TRANQUILITY = ai->getSpellIdExact("Tranquility");
    NATURES_SWIFTNESS = ai->getSpellIdExact("Nature's Swiftness");

    CURE_POISON = ai->getSpellIdExact("Abolish Poison");
    if (!CURE_POISON) CURE_POISON = ai->getSpellIdExact("Cure Poison");

    REBIRTH    = ai->getSpellIdExact("Rebirth");
    REVIVE = ai->getSpellIdExact("Revive");

    BARKSKIN = ai->getSpellIdExact("Barkskin");

    //Druid Forms
    BEAR_FORM = ai->getSpellIdExact("Dire Bear Form");
    if (!BEAR_FORM) BEAR_FORM = ai->getSpellIdExact("Bear Form");
    CAT_FORM = ai->getSpellIdExact("Cat Form");
    MOONKIN_FORM = ai->getSpellIdExact("Moonkin Form");
    TREE_OF_LIFE_FORM = ai->getSpellIdExact("Tree of Life"); //33891;//learning spell has higher id..
    AQUATIC_FORM = ai->getSpellIdExact("Aquatic Form");
    TRAVEL_FORM = ai->getSpellIdExact("Travel Form");
    FLIGHT_FORM = ai->getSpellIdExact("Swift Flight Form");
    if (!FLIGHT_FORM) FLIGHT_FORM = ai->getSpellIdExact("Flight Form");


    TALENT_BALANCE    = MOONKIN_FORM;
    TALENT_RESTO    = SWIFTMEND;
    TALENT_FERAL    = MANGLE_CAT;

    uint8 talentCounter = 0;
    if (TALENT_BALANCE) talentCounter++;
    if (TALENT_FERAL) talentCounter++;
    if (TALENT_RESTO) talentCounter++;
    if (talentCounter > 1) { TALENT_BALANCE = 0; TALENT_RESTO = 0; TALENT_FERAL = 0; } //Unreliable Talent detection.
#pragma endregion
}

void PlayerbotDruidAI::DoNextCombatManeuver(Unit *pTarget)
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


    #pragma region Select behaviour
    if (m_tank->GetGUID() == m_bot->GetGUID()) // Hey! I am Main Tank
    {
        if (TALENT_FERAL && BEAR_FORM) { m_role = BOT_ROLE_TANK; } //Just Keep Tanking dont even change forms for healing
        else
        {
            if (TALENT_BALANCE) {
                if ((ai->GetHealthPercent() <= 40 || masterHP <30 ) && (ai->GetManaPercent() >= 40)) { m_role = BOT_ROLE_SUPPORT; }
                else if (OwnPartyHP < 20 && ai->GetManaPercent() >= 30) { m_role = BOT_ROLE_SUPPORT; }
                else if (ai->GetManaPercent() < 25 ) { m_role = BOT_ROLE_TANK; }
                else { m_role = BOT_ROLE_DPS_RANGED; }
            }
            else //I am both healer and tank?? Hmm
            {
                if ((ai->GetHealthPercent() <= 70 || masterHP <70 ) && (ai->GetManaPercent() >= 50)) { m_role = BOT_ROLE_SUPPORT; }
                else if (OwnPartyHP < 20 && ai->GetManaPercent() >= 30) { m_role = BOT_ROLE_SUPPORT; }
                else if (ai->GetManaPercent() < 15 ) { m_role = BOT_ROLE_TANK; }
                else { m_role = BOT_ROLE_DPS_RANGED; }
            }
        }
    }
    else if (isUnderAttack() && !( ai->GetForm() == FORM_MOONKIN || ai->GetForm() == FORM_TREE)  ) // if i am under attack
    {
        // Keep being in Cat Form if you can reduce threat
        if (ai->GetForm() == FORM_CAT && CastSpell(COWER,pTarget)) {return; }
        else if (TALENT_RESTO && ai->GetManaPercent() > 10 ) { m_role = BOT_ROLE_SUPPORT; }
        else { m_role = BOT_ROLE_OFFTANK; }
    }
    else if (TALENT_FERAL && CAT_FORM) { // If has any feral forms at all
        if ((ai->GetHealthPercent() <= 40 || masterHP <40 ) && (ai->GetManaPercent() >= 40)) { m_role = BOT_ROLE_SUPPORT; }
        else if (OwnPartyHP < 30 && ai->GetManaPercent() >= 30) { m_role = BOT_ROLE_SUPPORT; }
        else{ m_role = BOT_ROLE_DPS_MELEE; }
    }
    else if (TALENT_BALANCE) {
        if ((ai->GetHealthPercent() <= 50 || masterHP <40 ) && (ai->GetManaPercent() >= 10)) { m_role = BOT_ROLE_SUPPORT; }
        else if (OwnPartyHP < 40 && ai->GetManaPercent() >= 30) { m_role = BOT_ROLE_SUPPORT; }
        else { m_role = BOT_ROLE_DPS_RANGED; }
    }
    else if (TALENT_RESTO)    { m_role = BOT_ROLE_SUPPORT; }
    else
    {
        // Unknown build or low level : Do not change forms rapidly..
        if ( (ai->GetManaPercent() < 30 && BEAR_FORM) || ( (ai->GetForm() == FORM_CAT || ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR) && ai->GetManaPercent() < 70 )  ) m_role = BOT_ROLE_DPS_MELEE;
        else { m_role = BOT_ROLE_DPS_RANGED; }
    }

    if (!isUnderAttack() && m_tank->GetGUID() != m_bot->GetGUID())
    {
        // Select Attacking target
        if (pVictim && pVictim->GetGUID() == m_bot->GetGUID() && pDist <= 2) {} //if my target is attacking me continue
        else
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
    #pragma endregion

    // If there's a cast stop
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) return;
    // Return to normal form from non combat forms
    if (ai->GetForm() == FORM_NONE || ai->GetForm() == FORM_CAT || ai->GetForm() == FORM_TREE || ai->GetForm() == FORM_MOONKIN || ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR )  { } //Those are valid incombat auras
    else if (ai->GetForm() != FORM_NONE && ChangeForm(1)) { } //return to caster form

    switch(m_role)
    {
        #pragma region BOT_ROLE_DPS_MELEE
        case BOT_ROLE_DPS_MELEE:
            //ai->TellMaster("DruidCombat");

            // Do caster form stuff
            if (ai->GetForm() == FORM_NONE)
            {
                //We have little mana probably cant change form
                if (ai->GetManaPercent() < 20 && CastSpell (INNERVATE, m_bot) ) { return; }
                else if(m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) {  return;}
                else if(DoSupportRaid(GetMaster(),false,false,false)) return;
                else if(m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup() && DoSupportRaid(m_bot,false,false,false)) { return; }
            }

            if (CAT_FORM) { if (ChangeForm(CAT_FORM)) { return; } }
            else if (BEAR_FORM) { if (ChangeForm(BEAR_FORM)) { return; } }
            else if (ai->GetForm() != FORM_NONE && ChangeForm(1)) {  } //Normal Form

            TakePosition(pTarget);
            break;
        #pragma endregion

        #pragma region BOT_ROLE_TANK / BOT_ROLE_OFFTANK
        case BOT_ROLE_OFFTANK:
        case BOT_ROLE_TANK: // It is a tank druid or a defending druid

            // Do what you must before getting attacked...
            if (ai->GetForm() == FORM_NONE)
            {
                // Non tank stuff to avoid
                if (m_tank->GetGUID() != m_bot->GetGUID())
                {
                    if (ROOTS && !pTarget->HasAura(CYCLONE) && !pTarget->HasAura(HIBERNATE) && CastSpell(ROOTS, pTarget)) { return; }
                    if (CYCLONE && pDist > 5 && !pTarget->HasAura(ROOTS) && !pTarget->HasAura(HIBERNATE) && CastSpell(CYCLONE, pTarget)) { return; }
                    if (HIBERNATE && pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_BEAST && !pTarget->HasAura(ROOTS) && !pTarget->HasAura(CYCLONE) && CastSpell(HIBERNATE, pTarget)) { return; }
                    //if (m_bot->getRace() == (uint8) RACE_NIGHTELF && isUnderAttack() && CastSpell(R_SHADOWMELD, m_bot)) { return; }
                }
                // Things to do wheter Tank or not
                if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; } //no gcd
                if (ai->GetManaPercent() < 20 && CastSpell (INNERVATE, m_bot) ) { return; } //We have little mana probably cant change form
            }
            TakePosition(pTarget);

            if (ChangeForm(BEAR_FORM)) { return; }

            // if i am main tank, protect master by taunt
            if(m_tank->GetGUID() == m_bot->GetGUID())
            {
                // Taunt if needed (Only for master)
                Unit *curAtt = GetAttackerOf(GetMaster());
                if (curAtt)
                {
                    if (isUnderAttack(GetMaster(),2) && CastSpell(CHALLENGING_ROAR, curAtt)) { return; }
                    if (CastSpell(GROWL, curAtt))  { return; }
                }
                // My target is not attacking me, taunt..
                if (pVictim && pVictim->GetGUID() != m_bot->GetGUID() && CastSpell(GROWL, pTarget) )  { return; }
            }
            break;
        #pragma endregion

        #pragma region BOT_ROLE_DPS_RANGED
        case BOT_ROLE_DPS_RANGED:
            if ( ai->GetManaPercent() < 20 && CastSpell (INNERVATE, m_bot)) { return; }

            // Do caster form stuff
            if (ai->GetForm() == FORM_NONE)
            {
                if(DoSupportRaid(GetMaster())) return;
                else if(m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup() && DoSupportRaid(m_bot)) { return; }
            }

            if (MOONKIN_FORM) { if (ChangeForm(MOONKIN_FORM)) { return; } }
            else if (ai->GetForm() != FORM_NONE && ChangeForm(1)) { } //Normal Form

            TakePosition(pTarget);

            // BUFF UP
            if(DoSupportRaid(GetMaster(),false,false,false)) return;
            else if(m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup() && DoSupportRaid(m_bot,false,false,false)) { return; }

            break;
        #pragma endregion

        #pragma region BOT_ROLE_SUPPORT
        case BOT_ROLE_SUPPORT:
            if ( ai->GetManaPercent() < 20 && CastSpell (INNERVATE,m_bot)) { return; }
            //Get to tree form only if you will no longer cast attack spells
            if( TREE_OF_LIFE_FORM && (ai->GetManaPercent() < offensiveSpellThreshold || isUnderAttack()) )
            {
                 if (ChangeForm(TREE_OF_LIFE_FORM)) { return; }
            }
            else if (ai->GetForm() != FORM_NONE && ChangeForm(1)) { }  //Normal Form no gcd

            TakePosition(pTarget);

            //RezGroup(REBIRTH, GetMaster());
            if (DoSupportRaid(GetMaster())) { return; }
            if (m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup() && DoSupportRaid(m_bot)) { return; }
            //heal pets and bots
            Unit *target = DoSelectLowestHpFriendly(30, 1000);
            if(target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth()) ) { return; }

            break;
        #pragma endregion
    }

    #pragma region DruidCommon
    // Common Dps and protection routine
    if (ai->GetHealthPercent() <= 70 && CastSpell(BARKSKIN,m_bot)) { return; }
    if (isUnderAttack() && CastSpell(NATURES_GRASP,m_bot)) { return; }

    if (ai->GetForm() == FORM_CAT)
    {
        // If at threat limit, use Cower to reduce threat
        if (pThreat > threatThreshold && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack())
        {
            if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
            {
                m_bot->SetSelection(m_tank->getVictim()->GetGUID());
                return;
            }
            else
            {
                if (CastSpell(COWER,pTarget)) { return; } //Lets see if we can manage
                else { return; } //use no spells and wait threat to be reduced
            }
        }
        if (CastSpell(FERAL_CHARGE_CAT,pTarget)) { return; }
        if (m_bot->GetComboPoints() >= 1 && pTarget->IsNonMeleeSpellCasted(true) && CastSpell(MAIM, pTarget)) { return; }

        if (CastSpell(BERSERK, m_bot)) { return; }
        if (ai->GetHealthPercent() <= 75 && CastSpell(SURVIVAL_INSTINCTS, m_bot)) { return; }
        if (isUnderAttack() && CastSpell(NATURES_GRASP, m_bot)) { return; }
        if (CastSpell(FAERIE_FIRE_FERAL, pTarget)) { return; }

        if (m_bot->GetComboPoints() < 5)
        {
            if (CastSpell(RAKE, pTarget)) { return; }
            if (CastSpell(MANGLE_CAT, pTarget)) { return; }
            if (!pTarget->HasInArc(M_PI,m_bot) && CastSpell(SHRED, pTarget)) { return; }
            if (ai->GetEnergyAmount() > 65 && CastSpell(MANGLE_CAT, pTarget)) { return; } //Spam mangle if cannot cast shred
            if (ai->GetEnergyAmount() > 65 && CastSpell(CLAW, pTarget) ) { return; } //Spam Claw if there is no mangle
            // if (CanCast(COWER, pTarget) && CastSpell(COWER, pTarget)) { return; } //if still nothing, use COWER to reduce threat
        }
        else
        {
            if (CastSpell(SAVAGE_ROAR)) { return; }
            if (CastSpell(RIP, pTarget)) { return; }
            if (ai->GetEnergyAmount() >= 65 && CastSpell(FEROCIOUS_BITE, pTarget)) { return; } //maxhit for feracious bite
        }
        if (CastSpell(TIGERS_FURY, m_bot)) { return; } //if nothing is ready yet, use tigers fury
    }
    else if (ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR)
    {
        // If at threat limit, stop
        if (pThreat > threatThreshold && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack() )
        {
            //Change to tank's target
            if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) { m_bot->SetSelection(m_tank->getVictim()->GetGUID()); }
            return; //use no spells and wait threat to be reduced
        }
        if (CastSpell(FERAL_CHARGE_BEAR,pTarget)) { return; }
        if (CastSpell(BASH, pTarget,true,true)) { return; } //Need check for immunity
        if (CastSpell(BERSERK, m_bot)) { return; }
        if (CastSpell(DEMORALIZING_ROAR, pTarget)) { return; }
        if (ai->GetHealthPercent() > 90 && ai->GetRageAmount() < 50 && CastSpell(ENRAGE, m_bot)) { return; }
        if (ai->GetHealthPercent() <= 75 && CastSpell(SURVIVAL_INSTINCTS, m_bot)) { return; }
        if ( ( ai->GetHealthPercent() <= 30 || (ai->GetHealthPercent() < 85 && m_tank->GetGUID() != m_bot->GetGUID())  )
            && CastSpell(FRENZIED_REGENERATION)) { return; }
        if (CastSpell(FAERIE_FIRE_FERAL, pTarget)) { return; }
        if (CastSpell(MANGLE_BEAR, pTarget)) { return; }
        if ((ai->GetRageAmount() > 70 || m_tank->GetGUID() == m_bot->GetGUID()) && CastSpell(SWIPE_BEAR, pTarget)) { return; }
        if (ai->GetRageAmount() > 50 && CastSpell(MAUL, pTarget)) {} // Low Priority, Next Attack effect
        if (ai->GetRageAmount() > 60 && CastSpell(LACERATE, pTarget)) { return; } //Currently applies only 1
    }
    else
    {
        //Defensive stuff
        if (m_tank->GetGUID() != m_bot->GetGUID() && pVictim && pVictim->GetGUID() == m_bot->GetGUID() )
        {
            if (ROOTS && !pTarget->HasAura(CYCLONE) && !pTarget->HasAura(HIBERNATE) && CastSpell(ROOTS, pTarget)) { return; }
            if (CYCLONE && pDist > 5 && !pTarget->HasAura(ROOTS) && !pTarget->HasAura(HIBERNATE) && CastSpell(CYCLONE, pTarget)) { return; }
            if (HIBERNATE && pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_BEAST && !pTarget->HasAura(ROOTS) && !pTarget->HasAura(CYCLONE) && CastSpell(HIBERNATE, pTarget)) { return; }
            //if (m_bot->getRace() == (uint8) RACE_NIGHTELF && isUnderAttack() && CastSpell(R_SHADOWMELD, m_bot)) { return; }
            if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; }
        }

        if (CastSpell(FAERIE_FIRE, pTarget)) { return; }

        // If at threat limit, stop
        if (pThreat > threatThreshold && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack() )
        {
            //Change to tank's target
            if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) { m_bot->SetSelection(m_tank->getVictim()->GetGUID()); }
            return; //use no spells and wait threat to be reduced
        }
        // Continue attacking if theres excess mana (for healers)
        if (m_role == BOT_ROLE_SUPPORT && ai->GetManaPercent() < offensiveSpellThreshold) { return; }
        if (m_role != BOT_ROLE_SUPPORT && CastSpell(NATURES_SWIFTNESS, m_bot)) { } //only balance no gcd

        if (m_bot->HasAura(NATURES_SWIFTNESS) && CastSpell(STARFIRE, pTarget)) { return; }
        if (CastSpell(INSECT_SWARM, pTarget)) { return; }
        if (CastSpell(TYPHOON, pTarget)) { return; }
        if (isUnderAttack(m_tank,4) && CastSpell(HURRICANE, pTarget)) { ai->SetIgnoreUpdateTime(8); return; }
        if (isUnderAttack(m_tank,5) && CastSpell(FORCE_OF_NATURE, m_bot)) { return; }
        if (isUnderAttack(m_tank,4) && CastSpell(STARFALL, pTarget)) { return; }
        if (CastSpell(MOONFIRE, pTarget)) { return; }
        if (CastSpell(WRATH, pTarget)) { return; }
        if (CastSpell(STARFIRE, pTarget)) { return; }
    }

    // If there is nothing else to do buff UP
    if (m_role == BOT_ROLE_DPS_MELEE) //Those already healed and buffed or should never buff in combat
    {
        if (DoSupportRaid(GetMaster(),false,false,false)) { return; }
        if (m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup() && DoSupportRaid(m_bot,false,false,false)) { return; }
    }


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

void PlayerbotDruidAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //buff and heal raid
    if (DoSupportRaid(GetMaster())) { return; }
    if (m_bot->GetGroup() && GetMaster()->GetGroup() != m_bot->GetGroup() && DoSupportRaid(m_bot)) { return; }

    //heal pets and bots
    Unit *target = DoSelectLowestHpFriendly(30, 1000);
    if (target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth())) { return; }

    //mana/hp check
    //Don't bother with eating, if low on hp, just let it heal themself
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (ai->GetManaPercent() < 10 && CastSpell (INNERVATE, m_bot)) { return; } //Need mana fast
    if (m_bot->GetHealth() < m_bot->GetMaxHealth() &&
        (ai->GetForm() != FORM_CAT && ai->GetForm() != FORM_MOONKIN && ai->GetForm() != FORM_DIREBEAR && ai->GetForm() != FORM_BEAR)
        && CastSpell(REGROWTH,m_bot)) { return; }
    if (ai->GetManaPercent() < 50) { ai->Feast(); }

} //end DoNonCombatActions

bool PlayerbotDruidAI::BuffPlayer(Unit *target)
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return false; }

    if(!target || target->isDead()) { return false; }

    if (CanCast(THORNS,target,0,0,1) && !HasAuraName(target, THORNS)) {
        // Decide if it is worth to change form
        if( /*m_bot->HasAura(MOONKIN_FORM) ||*/ m_bot->HasAura(CAT_FORM) || m_bot->HasAura(BEAR_FORM))
        {
            if(GetAI()->GetManaPercent() >= 80 ) { ChangeForm(1); }
            else { return false; }
        }
        return CastSpell(THORNS, target, false);
    }
    if (CanCast(MARK_OF_THE_WILD,target,0,0,1) && !HasAuraName(target, GIFT_OF_THE_WILD) && !HasAuraName(target, MARK_OF_THE_WILD)) {
        // Decide if it is worth to change form
        if(/*m_bot->HasAura(MOONKIN_FORM) ||*/ m_bot->HasAura(CAT_FORM) || m_bot->HasAura(BEAR_FORM))
        {
            if(GetAI()->GetManaPercent() >= 70 ) { ChangeForm(1); }
            else return false;
        }
        return CastSpell(MARK_OF_THE_WILD, target, false);
    }
    return false;
}

bool PlayerbotDruidAI::HealTarget(Unit *target, uint8 hp)
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return false; }
    if (!target || target->isDead()) { return false; }

    // Decide if it is worth to change form
    if (ai->GetForm() == FORM_MOONKIN || ai->GetForm() == FORM_CAT || ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR)
    {
        if (hp < 75 && GetAI()->GetManaPercent() >= 70 ) { ChangeForm(1); }
        else if (hp < 40 && GetAI()->GetManaPercent() >= 50) { ChangeForm(1); }
        else if (hp < 25 && GetAI()->GetManaPercent() >= 30) { ChangeForm(1); }
        else return false;
    }

    // if(m_bot->HasAura(TRAVEL_FORM)) ChangeForm(1);

    if(hp < 60 && m_bot->HasAura(NATURES_SWIFTNESS) && CastSpell(HEALING_TOUCH, target)) { return true; }
    if(hp < 90 && CastSpell(LIFEBLOOM, target)) { return true; }
    if(hp < 80 && CastSpell(REJUVENATION, target)) { return true; }
    if(hp < 60 && CastSpell(REGROWTH, target)) { return true; }
    if(hp < 70 && CanCast(NOURISH,target) &&
        (HasAuraName(target,REJUVENATION,m_bot->GetGUID()) || HasAuraName(target,LIFEBLOOM,m_bot->GetGUID()) || HasAuraName(target,REGROWTH,m_bot->GetGUID()))
        ) { return CastSpell(NOURISH, target, false); }
    if(hp < 50 && CanCast(SWIFTMEND,target) &&
        (HasAuraName(target,REJUVENATION,m_bot->GetGUID()) || HasAuraName(target,REGROWTH,m_bot->GetGUID()))
        ) { return CastSpell(SWIFTMEND, target, false); }
    if(hp < 40 && m_bot->isInCombat() && CastSpell(NATURES_SWIFTNESS, m_bot)) { } // NO gcd
    if(hp < 40 && CastSpell(HEALING_TOUCH, target)) { return true; }
    return false;
} //end HealTarget

bool PlayerbotDruidAI::HealGroup(Unit *target, uint8 hp, uint8 &countNeedHeal)
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return false; }
    if (!target || target->isDead()) { return false; }

    if (countNeedHeal < 2) { return false; }

    // Decide if it is worth to change form
    if (ai->GetForm() == FORM_MOONKIN || ai->GetForm() == FORM_CAT || ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR)
    {
        if (hp > 70) { return false; }
        if (!CanCast(TRANQUILITY,target,0,0,1) && !WILD_GROWTH) { return false; }
        if (!WILD_GROWTH && hp > 35) { return false; }
        if (hp < 65 && GetAI()->GetManaPercent() >= 70 ) { ChangeForm(1); }
        else if (hp < 40 && GetAI()->GetManaPercent() >= 50) { ChangeForm(1); }
        else if (hp < 25 && GetAI()->GetManaPercent() >= 30) { ChangeForm(1); }
        else return false;
    }

    if (hp < 36 && m_bot->isInCombat() && CanCast(TRANQUILITY,target))
    {
            bool sc = CastSpell(TRANQUILITY, target, false);
            if (sc) GetAI()->SetIgnoreUpdateTime(10);
            return sc;
    }
    if (hp < 75 && CastSpell(WILD_GROWTH,target)) { return true; }
    return false;
}

bool PlayerbotDruidAI::CureTarget(Unit *target)
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return false; }
    if (!target || target->isDead()) { return false; }
    // Decide if it is worth to change form (they cange forms even if theres nothing to cure..)
    if (ai->GetForm() == FORM_CAT || ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR)
    {
        //if(GetAI()->GetManaPercent() >= 80 ) { ChangeForm(1); }
        //else { return false; }
        return false;
    }
    if (castDispel(CURE_POISON, target)) { return true; }
    // if(HasAuraName(target, "Venom Spit") || HasAuraName(target, "Poison")) return CastSpell(CURE_POISON, target);

    return false;
} //end HealTarget

bool PlayerbotDruidAI::RezTarget (Unit *target)
{
    if(!target || target->isAlive()) return false;
    Player *m_bot = GetPlayerBot();
    if (target->IsNonMeleeSpellCasted(true)) { return false; } //Already resurrected

    if (m_bot->isInCombat())
    {
        if (!CanCast(REBIRTH,target)) return false;
        Unit *m_tank = FindMainTankInRaid(m_bot);
        if (!m_tank) m_tank = m_bot;
        if (target->GetGUID() != m_tank->GetGUID() &&
            (target->getClass() != (uint8) CLASS_PRIEST || target->getClass() != (uint8) CLASS_DRUID || target->getClass() != (uint8) CLASS_PALADIN) ) return false;
        std::string msg = "Rezzing ";
        msg += target->GetName();
       // msg += " with ";
       // msg += *REZZSpell->SpellName;
        GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
        return CastSpell(REBIRTH, target);
    }
    else
    {
        if (!CanCast(REVIVE,target)) return false;
        std::string msg = "Rezzing ";
        msg += target->GetName();
       // msg += " with ";
       // msg += *REZZSpell->SpellName;
        GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
        return CastSpell(REVIVE, target);
    }
    return false;
}

/*bool PlayerbotDruidAI::FindMount() {
    if (TRAVEL_FORM) {
        if (GetPlayerBot())    CastSpell(TRAVEL_FORM, GetPlayerBot());
        return true;
    } else return false;
}

bool PlayerbotDruidAI::Unmount() {
    GetPlayerBot()->RemoveAurasDueToSpell(TRAVEL_FORM);
    return true;
}

bool PlayerbotDruidAI::IsMounted() {
    return GetPlayerBot()->IsMounted() || HasAuraName(GetPlayerBot(), TRAVEL_FORM);
} */

bool PlayerbotDruidAI::ChangeForm(uint32 form)
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return false; }
    if (!form) return false;

    if (form == 1 && ai->GetForm() == FORM_NONE) return false;

    if (form != 1)
    {
        if (!CanCast(form,m_bot,0,0,1)) return false;
        if (m_bot->HasAura(form)) { return false; }
    }

    if (ai->GetForm() == FORM_TREE) m_bot->RemoveAurasDueToSpell(TREE_OF_LIFE_FORM);
    else if (ai->GetForm() == FORM_CAT) m_bot->RemoveAurasDueToSpell(CAT_FORM);
    else if (ai->GetForm() == FORM_MOONKIN) m_bot->RemoveAurasDueToSpell(MOONKIN_FORM);
    else if (ai->GetForm() == FORM_DIREBEAR || ai->GetForm() == FORM_BEAR) m_bot->RemoveAurasDueToSpell(BEAR_FORM);
    else if (ai->GetForm() == FORM_TRAVEL) m_bot->RemoveAurasDueToSpell(TRAVEL_FORM);
    else if (ai->GetForm() == FORM_FLIGHT || ai->GetForm() == FORM_FLIGHT_EPIC)    m_bot->RemoveAurasDueToSpell(FLIGHT_FORM);
    else if (ai->GetForm() == FORM_AQUA) m_bot->RemoveAurasDueToSpell(AQUATIC_FORM);

    if (form == 1) { return true; }

    return CastSpell(form,m_bot,false);
}


