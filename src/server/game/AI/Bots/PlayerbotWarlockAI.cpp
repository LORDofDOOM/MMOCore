/*
Name : PlayerbotWarlockAI.cpp
Complete: maybe around 60%

Limitations:    - Talent build decision is made by key talent spells, which makes them viable only after level 50-ish.. Behaviour does not change though..
                - Curse checks are slow, later all curses should be looked up in one loop
                - Need a function to lookup pet known spells for better pet handling
                - Warlock do not summon other pets than Fel hunter/Imp

Authors : SwaLLoweD
Version : 0.40
*/
#include "PlayerbotWarlockAI.h"

class PlayerbotAI;
PlayerbotWarlockAI::PlayerbotWarlockAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}
PlayerbotWarlockAI::~PlayerbotWarlockAI(){}

void PlayerbotWarlockAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    //CURSES
    CURSE_OF_ELEMENTS = ai->getSpellIdExact("Curse of Elements");
    CURSE_OF_WEAKNESS = ai->getSpellIdExact("Curse of Weakness");
    CURSE_OF_AGONY = ai->getSpellIdExact("Curse of Agony");
    CURSE_OF_RECKLESSNESS = ai->getSpellIdExact("Curse of Recklessness");
    CURSE_OF_TONGUES = ai->getSpellIdExact("Curse of Tongues");
    CURSE_OF_DOOM = ai->getSpellIdExact("Curse of Doom");
    CURSE_OF_EXHAUSTION = ai->getSpellIdExact("Curse of Exhaustion");


    //AFFLICTION
    CORRUPTION = ai->getSpellIdExact("Corruption");
    DRAIN_SOUL = ai->getSpellIdExact("Drain Soul");
    DRAIN_LIFE = ai->getSpellIdExact("Drain Life");
    DRAIN_MANA = ai->getSpellIdExact("Drain Mana");
    SIPHON_LIFE = ai->getSpellIdExact("Siphon Life");
    UNSTABLE_AFFLICTION = ai->getSpellIdExact("Unstable Affliction");
    HAUNT = ai->getSpellIdExact("Haunt");
    SEED_OF_CORRUPTION = ai->getSpellIdExact("Seed of Corruption");
    DEATH_COIL = ai->getSpellIdExact("Death Coil");


    //DESTRUCTION
    SHADOW_BOLT = ai->getSpellIdExact("Shadow Bolt");
    IMMOLATE = ai->getSpellIdExact("Immolate");
    INCINERATE = ai->getSpellIdExact("Incinerate");
    SEARING_PAIN = ai->getSpellIdExact("Searing Pain");
    CONFLAGRATE = ai->getSpellIdExact("Conflagrate");
    SOUL_FIRE = ai->getSpellIdExact("Soul Fire");
    SHADOWBURN = ai->getSpellIdExact("Shadowburn");
    SHADOWFURY = ai->getSpellIdExact("Shadowfury");
    CHAOS_BOLT = ai->getSpellIdExact("Chaos Bolt");
    SHADOWFLAME = ai->getSpellIdExact("Shadowflame");
    RAIN_OF_FIRE = ai->getSpellIdExact("Rain of Fire");
    HELLFIRE = ai->getSpellIdExact("Hellfire");


    //DEMONOLOGY
    DEMON_ARMOR = ai->getSpellIdExact("Demon Armor");
    if (!DEMON_ARMOR) DEMON_ARMOR = ai->getSpellIdExact("Demon Skin");
    FEL_ARMOR = ai->getSpellIdExact("Fel Armor");
    SOULSHATTER = ai->getSpellIdExact("Soulshatter");
    HEALTH_FUNNEL = ai->getSpellIdExact("Health Funnel");
    DARK_PACT = ai->getSpellIdExact("Dark Pact");
    SOUL_LINK = ai->getSpellIdExact("Soul Link");
    DEMONIC_EMPOWERMENT = ai->getSpellIdExact("Demonic Empowerment");
    METAMORPHOSIS = ai->getSpellIdExact("Metamorphosis"); //Original is learn spell
    SUMMON_IMP = ai->getSpellIdExact("Summon Imp");
    SUMMON_VOIDWALKER = ai->getSpellIdExact("Summon Voidwalker");
    SUMMON_SUCCUBUS = ai->getSpellIdExact("Summon Succubus");
    SUMMON_FELHUNTER = ai->getSpellIdExact("Summon Felhunter");
    SUMMON_FELGUARD = ai->getSpellIdExact("Summon Felguard");


    //CC
    FEAR = ai->getSpellIdExact("Fear");
    HOWL_OF_TERROR = ai->getSpellIdExact("Howl of Terror");
    BANISH = ai->getSpellIdExact("Banish");
    ENSLAVE_DEMON = ai->getSpellIdExact("Enslave Demon");


    //Buff
    UNENDING_BREATH = ai->getSpellIdExact("Unending Breath");
    DETECT_INVISIBILITY = ai->getSpellIdExact("Detect Invisibility");
    SHADOW_WARD = ai->getSpellIdExact("Shadow Ward");


    //Other
    LIFE_TAP = ai->getSpellIdExact("Life Tap");
    CREATE_SOULSTONE = ai->getSpellIdExact("Create Soulstone");


    SOUL_SHARD = 6265; //Soul Shard Item id
    P_BACKLASH = 34936; //Backlash proc
    P_NIGHTFALL= 17941; //Nightfall proc
    SHOOT = ai->getSpellIdExact("Shoot");

    TALENT_DEMONOLOGY = SUMMON_FELGUARD;
    TALENT_AFFLICTION = UNSTABLE_AFFLICTION;
    TALENT_DESTRUCTION = CONFLAGRATE;

    uint8 talentCounter = 0;
    if (TALENT_DEMONOLOGY) talentCounter++;
    if (TALENT_AFFLICTION) talentCounter++;
    if (TALENT_DESTRUCTION) talentCounter++;
    if (talentCounter > 1) { TALENT_DEMONOLOGY = 0; TALENT_AFFLICTION = 0; TALENT_DESTRUCTION = 0; } //Unreliable Talent detection.
    #pragma endregion
}

void PlayerbotWarlockAI::DoNextCombatManeuver(Unit *pTarget)
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

    Pet *pet = m_bot->GetPet();
    if (m_tank->GetGUID() == m_bot->GetGUID() && pet && pet->isAlive() && pet->isInCombat()) { m_tank = pet; }
    uint8 petThreat = 0;
    if (pet) { GetThreatPercent(pTarget,pet); }

    /*switch(ai->GetScenarioType())
    {
        case SCENARIO_DUEL:
            if(SHADOW_BOLT > 0) CastSpell(SHADOW_BOLT);
            return;
    }*/

    //------- Non Duel combat ----------

    //ai->Follow(*GetMaster()); //don't want to melee mob

    #pragma region Choose Target
    // Choose actions accoring to talents (WARLOCK is always ranged dps)
    m_role = BOT_ROLE_DPS_RANGED;

    // if i am under attack and if i am not tank or offtank: change target if needed
    if (m_tank->GetGUID() != m_bot->GetGUID() && isUnderAttack() )
    {
        // Keep hitting but reduce threat
        if (CastSpell(SOULSHATTER,m_bot)) { return; }
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

    #pragma region Pet Actions
    // Pet's own Actions
    if( pet && pet->isAlive() )
    {
        // Setup pet
        if (pet->GetCharmInfo()->IsAtStay()) {pet->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW); }
        //if (pet->HasSpell(BLOOD_PACT) && ); //Cast Blood Pact

        if ( ( ((float)pet->GetHealth()/(float)pet->GetMaxHealth()) < 0.5f )
        && ( HEALTH_FUNNEL>0 && !pet->getDeathState() != ALIVE && pVictim != m_bot
        && CastSpell(HEALTH_FUNNEL,m_bot) )) { return; } //Heal pet

        // Set pet to attack warlock's attacker > its own attackers > warlock's target
        if (!pet->getVictim()) { pet->AI()->AttackStart(pTarget); }
        else if (isUnderAttack(m_bot)) { pet->AI()->AttackStart(pTarget); }  //Always help warlock if he's under attack
        else if (pet->getVictim()->GetGUID() != pTarget->GetGUID() && !isUnderAttack(pet)) { pet->AI()->AttackStart(pTarget); }
        else if (isUnderAttack(pet)) // Pet is under attack and warlock has no attackers
        {
            if ( pet->getVictim()->getVictim() && pet->getVictim()->getVictim()->GetGUID() == pet->GetGUID() && pDist <= 2) {  } // My target is almost up to me, no need to search
            else //Have to select nearest target
            {
                Unit *curAtt = GetNearestAttackerOf(pet,true);
                if (curAtt && (!pet->getVictim() || curAtt->GetGUID() != pet->getVictim()->GetGUID()))
                {
                    pet->AI()->AttackStart(curAtt); //Attack nearest attacker
                }
            }
            //Actions to do under attack (Always tank it, and try to kill it, until someone (!= warlock) takes aggro back)
            //Warlock should help her pet whether main tank or not, unless he's being attacked (BEWARE Targeting Loop possibility)
            if (pet->getVictim() && !isUnderAttack(m_bot) && pet->getVictim()->GetGUID() != pTarget->GetGUID())
            {
                m_bot->SetSelection(pet->getVictim()->GetGUID());
                DoNextCombatManeuver(pet->getVictim()); //Restart new update to get variables fixed..
                return;
            }

        }
        // Pet tanking behaviour
        if (pet->GetGUID() == m_tank->GetGUID() || isUnderAttack(m_bot) || isUnderAttack(pet))
        {
            //need pet tanking spells
            //if (GROWL) pet->GetCharmInfo()->SetSpellAutocast(GROWL,true); //Autocast growl
        }
        else
        {
            //if (GROWL) pet->GetCharmInfo()->SetSpellAutocast(GROWL,false); //Do not try to get aggro
        }
        // NORMAL PET dps attacks
        if (petThreat < threatThreshold || pet->GetGUID() == m_tank->GetGUID() || isUnderAttack(m_bot))
        {
            //if (CastSpell(KILL_COMMAND,m_bot)) { }
        }
    }
    #pragma endregion

    TakePosition(pTarget);
    // If there's a cast stop
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }

    //Buff
    if (isUnderAttack()) { if (CastSpell (DEMON_ARMOR, m_bot)) { return; } }
    else if (FEL_ARMOR) { if (CastSpell(FEL_ARMOR,m_bot)) { return; } }
    else if (CastSpell(DEMON_ARMOR, m_bot)) { return; }
    if (CastSpell(METAMORPHOSIS,m_bot)) { return; }
    if (CastSpell(DEMONIC_EMPOWERMENT,m_bot)) { return; }

    //Protect
    if (m_tank->GetGUID() != m_bot->GetGUID() && pVictim && pVictim->GetGUID() == m_bot->GetGUID() )
    {
        if (CastSpell(SOULSHATTER, m_bot)) { return; }
        if (pTarget->GetCreatureType() == CREATURE_TYPE_DEMON && CastSpell(BANISH,pTarget)) { return; }
        //if (m_bot->getRace() == (uint8) RACE_NIGHTELF && isUnderAttack() && CastSpell(R_SHADOWMELD, m_bot)) { return; }
    }
    if (isUnderAttack() && CastSpell(FEAR, pTarget)) { return; }
    if (isUnderAttack() && CastSpell(HOWL_OF_TERROR, pTarget)) { return; }
    if (isUnderAttack() && CastSpell(SHADOWFURY, pTarget)) { return; }
    if (m_bot->getRace() == (uint8) RACE_DWARF && ai->GetHealthPercent() < 75 && CastSpell(R_STONEFORM,m_bot)) { } //no gcd
    if (m_bot->getRace() == (uint8) RACE_DRAENEI && ai->GetHealthPercent() < 55 && CastSpell(R_GIFT_OF_NAARU,m_bot)) { return; } //no Gcd, but has cast
    if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; } //no gcd but is cast
    //Void Walker shield?
    if (ai->GetHealthPercent() < 70 && CastSpell(DEATH_COIL,pTarget)) { return; }
    if (ai->GetHealthPercent() < 70 && CastSpell(DRAIN_LIFE,pTarget)) { return; }
    if (ai->GetManaPercent() < 70 && ai->GetManaPercent(*pTarget) > 10 && CastSpell(DRAIN_MANA,pTarget)) { return; }
    if (ai->GetManaPercent() < 50 && pet && ai->GetManaPercent(*pet) > 50 && CastSpell(DARK_PACT,pet,1,0,1)) { return; }
    if (ai->GetManaPercent() < 30 && ai->GetHealthPercent() > 60 && CastSpell(LIFE_TAP,m_bot,1,0,1)) { return; }
    //Use Health stone

    //Break spells
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && (pTarget->IsNonMeleeSpellCasted(true) || ai->GetManaPercent() < 40) && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
    else if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(CURSE_OF_TONGUES, pTarget)) { return; }

    //Catch
    if (pTarget->HasUnitMovementFlag(UNIT_FLAG_FLEEING))
    {
        if (CastSpell(CURSE_OF_EXHAUSTION,pTarget)) return;
    }

    // Threat control
    if (pThreat < threatThreshold || m_tank->GetGUID() == m_bot->GetGUID() ) { } //Continue attack
    else
    {
        if (pet && isUnderAttack(pet) && pet->getVictim() && pet->getVictim()->GetGUID() != pTarget->GetGUID()) //Should be helping pet
        {
            m_bot->SetSelection(pet->getVictim()->GetGUID());
            return;
        }
        else if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
        {
            m_bot->SetSelection(m_tank->getVictim()->GetGUID());
            return;
        }
        else if (CastSpell(SOULSHATTER,m_bot)) { return; }
        else if (m_bot->FindCurrentSpellBySpellId(SHOOT)) { m_bot->InterruptNonMeleeSpells( true, SHOOT ); return; } //Disable wand
        else { return; } // No more threat reducing spells, just slow down
    }



    //Urgent DPS
    if ((m_bot->HasAura(P_NIGHTFALL) || m_bot->HasAura(P_BACKLASH)) && CastSpell(SHADOW_BOLT,pTarget)) { return; }
    if (INCINERATE && pTarget->HasAura(IMMOLATE,m_bot->GetGUID()) && CastSpell(INCINERATE,pTarget)) { return; }
    if (CONFLAGRATE && (pTarget->HasAura(IMMOLATE,m_bot->GetGUID()) || pTarget->HasAura(SHADOWFLAME,m_bot->GetGUID())) && CastSpell(CONFLAGRATE,pTarget)) { return; }

    if (ai->GetHealthPercent(*pTarget) < 2 && CastSpell(SHADOWBURN,pTarget)) { return; }
    if (ai->GetHealthPercent(*pTarget) < 5 && m_bot->GetItemCount(SOUL_SHARD) < 20 && CastSpell(DRAIN_SOUL,pTarget)) { return; }

    //Dps up
    if (ai->GetHealthPercent(*pTarget) > 95) { return; } // dont dps too early

    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} //no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} //no GCD
    if (CastSpell(HAUNT,pTarget)) { return; }

    //AOE
    if (isUnderAttack(m_tank,4) && CastSpell(SHADOWFLAME,pTarget)) { return; }
    if (isUnderAttack(m_tank,4) && CastSpell(RAIN_OF_FIRE,pTarget)) { return; }

    //Dps Main
    if(CURSE_OF_ELEMENTS) { if (!CheckCurse(pTarget) && CastSpell(CURSE_OF_ELEMENTS, pTarget)) { return; } } //curse of elements trumps any other curses
    else if (CURSE_OF_AGONY) { if (!CheckCurse(pTarget) && CastSpell(CURSE_OF_AGONY, pTarget)) { return; } }

    if (SEED_OF_CORRUPTION && isUnderAttack(m_tank,4)) { if (CastSpell(SEED_OF_CORRUPTION,pTarget)) { return;} }
    else if (CastSpell(CORRUPTION,pTarget)) { return; }

    if (CastSpell(CHAOS_BOLT,pTarget)) { return; }

    if (UNSTABLE_AFFLICTION) { if (CastSpell(UNSTABLE_AFFLICTION,pTarget)) { return; } }
    else if (CastSpell(IMMOLATE,pTarget)) { return; }

    if (CastSpell(SHADOW_BOLT,pTarget)) { return; }




    //Use healthstone??
    // drink poition
    if(ai->GetManaPercent() < 5 )
    {
        Item *pItem = ai->FindPotion();
        if(pItem != NULL)
        {
            if (pItem->GetSpell() && m_bot->HasSpellCooldown(pItem->GetSpell()) ) { return; } //pot is in cooldown
            ai->UseItem(*pItem);
        }
    }

} //end DoNextCombatManeuver

void PlayerbotWarlockAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //buff and heal raid
    if (DoSupportRaid(m_bot,30,0,0,0,0,1)) { return; }

    //Own Buffs
    if (CastSpell (FEL_ARMOR, m_bot)) { return; }
    if (!FEL_ARMOR && CastSpell(DEMON_ARMOR, m_bot)) { return; }
    if (SOUL_LINK && m_bot->GetPet() && !m_bot->HasAuraType(SPELL_AURA_SPLIT_DAMAGE_PCT) && CastSpell(SOUL_LINK,m_bot)) { return; }

    if(m_bot->GetPet() == NULL) {
        if (SUMMON_FELGUARD)
            CastSpell(SUMMON_FELGUARD, m_bot);
        else
            CastSpell(SUMMON_IMP, m_bot);
    } else {
    m_bot->GetPet()->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW);
    m_bot->GetPet()->GetCharmInfo()->SetIsCommandAttack(false);
    }

    //Create Healthstone?

    //mana/hp check
    //Don't bother with eating, if low on hp, just let it heal themself
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (ai->GetManaPercent() < 50 && ai->GetHealthPercent() > 60 && CastSpell (LIFE_TAP, m_bot)) { return; }
    if (ai->GetManaPercent() < 50 || ai->GetHealthPercent() < 50) { ai->Feast(); }


} //end DoNonCombatActions

bool PlayerbotWarlockAI::BuffPlayer(Unit *target)
{
    if (!target || target->isDead()) return false;

    if (!HasAuraName(target, DETECT_INVISIBILITY) && CastSpell(DETECT_INVISIBILITY, target)) { return true; }
    if (!HasAuraName(target, UNENDING_BREATH) && CastSpell(UNENDING_BREATH, target)) { return true; }
    return false;
}

uint32 PlayerbotWarlockAI::CheckCurse(Unit *target)
{
    Player *m_bot = GetPlayerBot();
    if (!m_bot || m_bot->isDead() || !target || target->isDead() ) { return 0; }
    uint64 mGuid = m_bot->GetGUID();

    if (CURSE_OF_ELEMENTS && target->HasAura(CURSE_OF_ELEMENTS,mGuid)) { return CURSE_OF_ELEMENTS; }
    if (CURSE_OF_AGONY && target->HasAura(CURSE_OF_AGONY,mGuid)) { return CURSE_OF_AGONY; }
    if (CURSE_OF_TONGUES && target->HasAura(CURSE_OF_TONGUES,mGuid)) { return CURSE_OF_TONGUES; }
    if (CURSE_OF_WEAKNESS && target->HasAura(CURSE_OF_WEAKNESS,mGuid)) { return CURSE_OF_WEAKNESS; }
    if (CURSE_OF_DOOM && target->HasAura(CURSE_OF_DOOM,mGuid)) { return CURSE_OF_DOOM; }
    if (CURSE_OF_RECKLESSNESS && target->HasAura(CURSE_OF_RECKLESSNESS,mGuid)) { return CURSE_OF_RECKLESSNESS; }
    if (CURSE_OF_EXHAUSTION && target->HasAura(CURSE_OF_EXHAUSTION,mGuid)) { return CURSE_OF_EXHAUSTION; }
    return 0;
}

//void PlayerbotWarlockAI::BuffPlayer(Player *target){}

