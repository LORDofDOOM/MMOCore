/*
Name : PlayerbotPriest.cpp
Complete: maybe around 55%

Limitations:    - Talent build decision is made by key talent spells, which makes them viable only after level 50-ish.. Behaviour does not change though..
                - Holy And Disc builds do not cast any offensive spells requiring cast time..(To compensate for the fact that Healing decision is not that intelligent)
                - Priest breaks her own CCs.. Need a check for bots to not attack CC ed mobs..
                - Wand usage is not very smooth..


Authors : SwaLLoweD
Version : 0.40
*/
#include "PlayerbotPriestAI.h"

class PlayerbotAI;
PlayerbotPriestAI::PlayerbotPriestAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}
PlayerbotPriestAI::~PlayerbotPriestAI(){}

void PlayerbotPriestAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    //heals
    RENEW = ai->getSpellIdExact("Renew");
    FLASH_HEAL = ai->getSpellIdExact("Flash Heal");
    if (!FLASH_HEAL) FLASH_HEAL = ai->getSpellIdExact("Lesser Heal");
    HEAL = ai->getSpellIdExact("Greater Heal");
    if (!HEAL) HEAL = ai->getSpellIdExact("Heal");
    if (!HEAL) HEAL = FLASH_HEAL;
    BINDING_HEAL = ai->getSpellIdExact("Binding Heal");
    PO_MENDING = ai->getSpellIdExact("Prayer of Mending");
    DESPERATE_PRAYER = ai->getSpellIdExact("Desperate Prayer");
    PO_HEALING = ai->getSpellIdExact("Prayer of Healing");
    CIRCLE_OF_HEALING = ai->getSpellIdExact("Circle of Healing");
    DIVINE_HYMN = ai->getSpellIdExact("Divine Hymn");
    RESURRECTION = ai->getSpellIdExact("Resurrection");
    HYMN_OF_HOPE = ai->getSpellIdExact("Hymn of Hope");
    CURE_DISEASE = ai->getSpellIdExact("Abolish Disease");
    if (!CURE_DISEASE) CURE_DISEASE = ai->getSpellIdExact("Abolish Disease");
    DISPEL_MAGIC = ai->getSpellIdExact("Dispel Magic");
    MASS_DISPEL = ai->getSpellIdExact("Mass Dispel");

    //Holy Offensive
    SMITE = ai->getSpellIdExact("Smite");
    HOLY_FIRE = ai->getSpellIdExact("Holy Fire");
    PENANCE = ai->getSpellIdExact("Penance");
    HOLY_NOVA = ai->getSpellIdExact("Holy Nova");

    //Shadow Offensive
    MIND_BLAST = ai->getSpellIdExact("Mind Blast");
    SW_PAIN = ai->getSpellIdExact("Shadow Word: Pain");
    DEVOURING_PLAGUE = ai->getSpellIdExact("Devouring Plague");
    MIND_FLAY = ai->getSpellIdExact("Mind Flay");
    VAMPIRIC_EMBRACE = ai->getSpellIdExact("Vampiric Embrace");
    VAMPIRIC_TOUCH = ai->getSpellIdExact("Vampiric Touch");
    SW_DEATH = ai->getSpellIdExact("Shadow Word: Death");
    MIND_SEAR = ai->getSpellIdExact("Mind Sear");
    MANA_BURN = ai->getSpellIdExact("Mana Burn");
    SHADOWFIEND = ai->getSpellIdExact("Shadowfiend");

    //CC - Breaker
    PSYCHIC_SCREAM = ai->getSpellIdExact("Psychic Scream");
    PSYCHIC_HORROR = ai->getSpellIdExact("Psychic Horror");
    MIND_SOOTHE = ai->getSpellIdExact("Mind Soothe");
    SHACKLE_UNDEAD = ai->getSpellIdExact("Shackle Undead");
    SILENCE = ai->getSpellIdExact("Silence");
    MIND_CONTROL = ai->getSpellIdExact("Mind Control");

    //buffs
    PW_SHIELD = ai->getSpellIdExact("Power Word: Shield");
    INNER_FIRE = ai->getSpellIdExact("Inner Fire");
    GUARDIAN_SPIRIT = ai->getSpellIdExact("Guardian Spirit");
    FADE = ai->getSpellIdExact("Fade");
    INNER_FOCUS = ai->getSpellIdExact("Inner Focus");
    POWER_INFUSION = ai->getSpellIdExact("Power Infusion");
    PAIN_SUPPRESSION = ai->getSpellIdExact("Pain Suppression");
    SHADOWFORM = ai->getSpellIdExact("Shadowform");
    DISPERSION = ai->getSpellIdExact("Dispersion");
    LIGHTWELL = ai->getSpellIdExact("Lightwell");

    PW_FORTITUDE = ai->getSpellIdExact("Power Word: Fortitude");
    DIVINE_SPIRIT = ai->getSpellIdExact("Divine Spirit");
    SHADOW_PROTECTION = ai->getSpellIdExact("Shadow Protection");
    PO_FORTITUDE = ai->getSpellIdExact("Prayer of Fortitude");
    PO_SPIRIT = ai->getSpellIdExact("Prayer of Spirit");
    PO_SHADOW_PROTECTION = ai->getSpellIdExact("Prayer of Shadow Protection");
    FEAR_WARD = ai->getSpellIdExact("Fear Ward");

    SHOOT = ai->getSpellIdExact("Shoot");

    TALENT_DISC = PAIN_SUPPRESSION;
    TALENT_HOLY = CIRCLE_OF_HEALING;
    TALENT_SHADOW = SHADOWFORM;

    uint8 talentCounter = 0;
    if (TALENT_DISC) talentCounter++;
    if (TALENT_HOLY) talentCounter++;
    if (TALENT_SHADOW) talentCounter++;
    if (talentCounter > 1) { TALENT_DISC = 0; TALENT_HOLY = 0; TALENT_SHADOW = 0; } //Unreliable Talent detection.
    #pragma endregion
}

void PlayerbotPriestAI::DoNextCombatManeuver(Unit *pTarget)
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

    /*
    switch(ai->GetScenarioType())
    {
        case SCENARIO_DUEL:
            (ai->HasAura(SCREAM, pTarget) && ai->GetHealthPercent() < 60 && CastSpell(HEAL)) ||
            CastSpell(PAIN) ||
            (ai->GetHealthPercent() < 80 && CastSpell(RENEW)) ||
            (m_bot->GetDistance(pTarget) <= 5 && CastSpell(SCREAM)) ||
            CastSpell(MIND_BLAST) ||
            (ai->GetHealthPercent() < 20 && CastSpell(FLASH_HEAL)) ||
            CastSpell(SMITE);
            return;
    }*/


    //------- Non Duel combat ----------


    // Cast CC breakers if any match found (include any dispels first)  does not work yet
    //uint32 ccSpells[6] = { DISPEL_MAGIC, CURE_DISEASE, DISPERSION, R_ESCAPE_ARTIST, R_EVERY_MAN_FOR_HIMSELF, R_WILL_OF_FORSAKEN, R_STONEFORM };
    //if (ai->GetManaPercent() < 35) { ccSpells[0] = 0; ccSpells[1] = 0; } //We dont have any mana to waste...
    //if (castSelfCCBreakers(ccSpells)) {  } // Most of them don't trigger gcd



    #pragma region Choose Actions
    // Choose actions accoring to talents
    if (m_tank->GetGUID() == m_bot->GetGUID()) // Hey! I am Main Tank
    {
        m_role = BOT_ROLE_DPS_RANGED;
    }
    else if (TALENT_SHADOW) {
        if ((ai->GetHealthPercent() <= 40 || masterHP <40 ) && (ai->GetManaPercent() >= 30)) { m_role = BOT_ROLE_SUPPORT; }
        else if (OwnPartyHP < 40 && ai->GetManaPercent() >= 40) {m_role = BOT_ROLE_SUPPORT;}
        else { m_role = BOT_ROLE_DPS_RANGED; }
    }
    else { m_role = BOT_ROLE_SUPPORT; } //Unknown build or low level.. Mainly attack

    // if i am under attack and if i am not tank or offtank: change target if needed
    if (m_tank->GetGUID() != m_bot->GetGUID() && isUnderAttack() )
    {
        // Keep hitting but reduce threat
        if (CastSpell(FADE,m_bot)) { return; }
        else if (CastSpell(PAIN_SUPPRESSION,m_bot)) { return; }
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
    #pragma endregion

    TakePosition(pTarget);
    // If there's a cast stop
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }


    if (m_role == BOT_ROLE_DPS_RANGED && CastSpell(SHADOWFORM,m_bot)) { return; }
    if (m_role == BOT_ROLE_SUPPORT && ai->GetForm() == FORM_SHADOW) { m_bot->RemoveAurasDueToSpell(SHADOWFORM); }

    //Buff
    if (CastSpell(INNER_FIRE,m_bot)) { } //nogcd
    if (CastSpell(POWER_INFUSION,m_bot)) { return; }
    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} //no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} //no GCD


    if (ai->GetForm() != FORM_SHADOW)
    {
        if (PO_MENDING && ai->GetHealthPercent(*m_tank) < 90 && !HasAuraName(m_tank, "Prayer of Mending") && CastSpell(PO_MENDING,m_tank)) { return; } //MEND tank first
        if (DoSupportRaid(m_bot)) { return; }
        //heal pets and bots
        Unit *target = DoSelectLowestHpFriendly(30, 1000);
        if(target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth()) ) { return; }
    }
    if (ai->GetForm() == FORM_SPIRITOFREDEMPTION) { return; } //You're dead..


    //PROTECT
    if (m_tank->GetGUID() != m_bot->GetGUID() && pVictim && pVictim->GetGUID() == m_bot->GetGUID() )
    {
        if (PSYCHIC_HORROR && CastSpell(PSYCHIC_HORROR, pTarget)) { return; }
        if (PSYCHIC_SCREAM && CastSpell(PSYCHIC_SCREAM, pTarget)) { return; }
        if (SHACKLE_UNDEAD && pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_UNDEAD && CastSpell(SHACKLE_UNDEAD, pTarget)) { return; }
        //if (m_bot->getRace() == (uint8) RACE_NIGHTELF && isUnderAttack() && CastSpell(R_SHADOWMELD, m_bot)) { return; }
    }
    if (ai->GetHealthPercent() < 20 && CastSpell(DESPERATE_PRAYER)) { return; }
    if (ai->GetHealthPercent() < 30 && CastSpell(PW_SHIELD)) { return; }
    if (m_bot->getRace() == (uint8) RACE_DWARF && ai->GetHealthPercent() < 75 && CastSpell(R_STONEFORM,m_bot)) { } //no gcd
    if (ai->GetHealthPercent() < 60 && CastSpell(PAIN_SUPPRESSION,m_bot)) { return; }
    if (ai->GetHealthPercent() < 50 && ai->GetManaPercent() < 10 && CastSpell(DISPERSION,m_bot)) { return; }
    if (ai->GetManaPercent() < 10 && CastSpell (HYMN_OF_HOPE, m_bot)) { return; }
    if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; } //no gcd but is cast


    //Break Spells
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && (pTarget->IsNonMeleeSpellCasted(true) || ai->GetManaPercent() < 40) && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
    else if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(SILENCE, pTarget)) { return; }


    // If at threat limit, try to reduce threat
    if (pThreat > threatThreshold && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack())
    {
        if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
        {
            m_bot->SetSelection(m_tank->getVictim()->GetGUID());
            return;
        }
        else
        {
            if (CastSpell(PAIN_SUPPRESSION,m_bot)) { return; } //Lets see if we can manage
            else if (CastSpell(FADE,m_bot)) { return; }
            else if (m_bot->FindCurrentSpellBySpellId(SHOOT)) { m_bot->InterruptNonMeleeSpells( true, SHOOT ); return; } //Disable wand
            else { return; } //use no spells and wait threat to be reduced
        }
    }

    //WAND
    if (ai->GetManaPercent() < 5 ||
        (m_role != BOT_ROLE_DPS_RANGED && SHOOT && !m_bot->FindCurrentSpellBySpellId(SHOOT) && ai->CastSpell(SHOOT,pTarget) )
        ) { return; } //Start autoshot

    // Continue spell attacking if theres excess mana (for healers)
    if (m_role == BOT_ROLE_SUPPORT && ai->GetManaPercent() < offensiveSpellThreshold) { return; }

    if (ai->GetHealthPercent(*pTarget) > 95) { return; } // dont dps too early

    if (CastSpell(VAMPIRIC_EMBRACE,pTarget)) { return; }
    if (CastSpell(VAMPIRIC_TOUCH,pTarget)) { return; }
    if (CastSpell(DEVOURING_PLAGUE,pTarget)) { return; }
    if (CastSpell(SW_PAIN,pTarget)) { return; }

    if (ai->GetForm() == FORM_SHADOW)
    {
        if (castDispel(DISPEL_MAGIC, pTarget)) { return; } //Dispel buffs if any
        if (CastSpell(MIND_BLAST,pTarget)) { return; }
        if (CastSpell(MIND_FLAY,pTarget)) { return; }
        if (isUnderAttack(m_tank,4) && CastSpell(MIND_SEAR,pTarget)) { return; }
    }

    if (ai->GetForm() == FORM_NONE && m_role == BOT_ROLE_DPS_RANGED)
    {
        if (CastSpell(PENANCE,pTarget)) { return; }
        if (CastSpell(MIND_BLAST,pTarget)) { return; }
        if (CastSpell(HOLY_FIRE,pTarget)) { return; }
        if (CastSpell(SMITE,pTarget)) { return; }
        if (isUnderAttack(m_tank,4) && CastSpell(MIND_SEAR,pTarget)) { return; }
        if (isUnderAttack(m_tank,4) && CastSpell(HOLY_NOVA,pTarget)) { return; }
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
} //end DoNextCombatManeuver

void PlayerbotPriestAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //Own Buffs
    if (CastSpell(INNER_FIRE,m_bot)) { return; }

    //buff and heal raid
    if (DoSupportRaid(m_bot)) { return; }

    //heal pets and bots
    Unit *target = DoSelectLowestHpFriendly(30, 1000);
    if (target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth())) { return; }

    //mana/hp check
    //Don't bother with eating, if low on hp, just let it heal themself
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (m_bot->GetHealth() < m_bot->GetMaxHealth() && CastSpell(FLASH_HEAL,m_bot)) { return; }
    if (ai->GetManaPercent() < 50) { ai->Feast(); }

} //end DoNonCombatActions


bool PlayerbotPriestAI::HealTarget(Unit *target, uint8 hp)
{
    if(!target || target->isDead()) return false;
    Player *m_bot = GetPlayerBot();

    if (hp < 30 && m_bot->isInCombat() && CastSpell(GUARDIAN_SPIRIT,target)) { } //nogcd
    if (hp < 30 && CastSpell(PENANCE,target,true,false,true)) { return true; } //Channeling Dual purpose
    if (hp < 35 && m_bot->isInCombat() && CastSpell(PW_SHIELD,target)) { return true; } //Check weakened soul
    if (hp < 80 && hp > 50 && GetAI()->GetHealthPercent() < 80 && CastSpell (BINDING_HEAL,target)) { return true; }
    if (hp < 85 && CastSpell(RENEW,target)) { return true; }
    if (hp < 40 && GetPlayerBot()->getRace() == (uint8) RACE_DRAENEI && CastSpell(R_GIFT_OF_NAARU,target)) { return true; } // no GCD, but has cast
    if (hp < 55 && hp > 35 && CastSpell(HEAL,target)) { return true; }
    if (hp < 75 && CastSpell(FLASH_HEAL,target)) { return true; }

    return false;
} //end HealTarget

bool PlayerbotPriestAI::HealGroup (Unit *target, uint8 hp, uint8 &countNeedHeal)
{
    Player *m_bot = GetPlayerBot();
    if (countNeedHeal < 2) { return false; }
    Unit *rTarget = DoSelectLowestHpFriendly(30,500);
    if (!rTarget || rTarget->isDead() || rTarget->GetHealth() * 100 / rTarget->GetMaxHealth() > 80 ) { return false; }

    // if (hp < 75 && CastSpell(PO_MENDING, rTarget)) { return true; } //save this for tank
    if (hp < 35 && m_bot->isInCombat() && CastSpell(DIVINE_HYMN,rTarget)) { /*GetAI()->SetIgnoreUpdateTime(9);*/ return true; }
    if (hp < 70 && CastSpell(CIRCLE_OF_HEALING,rTarget)) { return true; }
    if (hp < 75 && hp > 30 && countNeedHeal > 4 && CastSpell(PO_HEALING)) { return true; }
    if (hp < 65 && CastSpell(HOLY_NOVA,rTarget,true,false,true)) { return true; }

    return false;
}

//Cures the target
bool PlayerbotPriestAI::CureTarget(Unit *target)
{
    Player *m_bot = GetPlayerBot();

    if(!target || target->isDead()) { return false; }
    if (castDispel(DISPEL_MAGIC, target,true,false,true)) return true;
    if (castDispel(CURE_DISEASE, target)) return true;
    // if(HasAuraName(target, "Venom Spit") || HasAuraName(target, "Poison")) return CastSpell(CURE_POISON, target);
    return false;
}

bool PlayerbotPriestAI::RezTarget (Unit *target)
{
    if(!target || target->isAlive()) return false;
    Player *m_bot = GetPlayerBot();
    if (target->IsNonMeleeSpellCasted(true)) { return false; } //Already resurrected
    if (m_bot->isInCombat()) { return false; }

    if (!CanCast(RESURRECTION,target)) return false;
    std::string msg = "Rezzing ";
    msg += target->GetName();
   // msg += " with ";
   // msg += *REZZSpell->SpellName;
    GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
    return CastSpell(RESURRECTION, target,false);
}

bool PlayerbotPriestAI::BuffPlayer(Unit *target)
{
    if(!target || target->isDead()) return false;

    return (
        (!HasAuraName(target, PW_FORTITUDE) && !HasAuraName(target, PO_FORTITUDE) && CastSpell (PW_FORTITUDE, target)) ||
        (!HasAuraName(target, SHADOW_PROTECTION) && !HasAuraName(target, PO_SHADOW_PROTECTION) && CastSpell(SHADOW_PROTECTION, target)) ||
        (!HasAuraName(target, DIVINE_SPIRIT) && !HasAuraName(target, PO_SPIRIT) && CastSpell (DIVINE_SPIRIT, target)) ||
        (!HasAuraName(target, FEAR_WARD) && CastSpell (FEAR_WARD, target))
         );
}
