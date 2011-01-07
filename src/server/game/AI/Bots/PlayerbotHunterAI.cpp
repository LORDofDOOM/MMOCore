/*
Name : PlayerbotHunterAI.cpp
Complete: maybe around 70%

Limitations:    - Talent build decision is made by key talent spells, which makes them viable only after level 50-ish.. Behaviour does not change though..
                - AI always assumes pet is the tank if there are no higher hp people in group than the hunter..
                - Possible threat build / reduce race between pet and hunter if attacking to same target.. Needs checking
                - Possible target changing loop between pet and hunter if attacking to same target and getting aggro repeatedly.. Needs checking
                - Disarm and Nature resist aspect, Disengage, Scorpid sting are not used right now..

Authors : SwaLLoweD
Version : 0.40
*/

#include "PlayerbotHunterAI.h"


class PlayerbotAI;

PlayerbotHunterAI::PlayerbotHunterAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
    m_petSummonFailed = false;
    LoadSpells();
}

PlayerbotHunterAI::~PlayerbotHunterAI() {}

void PlayerbotHunterAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    // PET CONTROL
    PET_SUMMON = ai->getSpellIdExact("Call Pet");
    PET_DISMISS = ai->getSpellIdExact("Dismiss Pet");
    PET_REVIVE = ai->getSpellIdExact("Revive Pet");
    PET_MEND = ai->getSpellIdExact("Mend Pet");
    PET_FEED = 1539; //ai->getSpellIdExact("Feed Pet");
    KILL_COMMAND = ai->getSpellIdExact("Kill Command");
    INTIMIDATION = ai->getSpellIdExact("Intimidation");
    BESTIAL_WRATH = ai->getSpellIdExact("Bestial Wrath");

    // PET SPELL (master does not have these spells anymore)
    GROWL = ai->getSpellIdExact("Growl");
    COWER = ai->getSpellIdExact("Cower");
    BAD_ATTITUDE = ai->getSpellIdExact("Bad Attitude");
    SONIC_BLAST = ai->getSpellIdExact("Sonic Blast");
    NETHER_SHOCK = ai->getSpellIdExact("Nether Shock");
    DEMORALIZING_SCREECH = ai->getSpellIdExact("Demoralizing Screech");

    // RANGED ATTACK
    AUTO_SHOT = ai->getSpellIdExact("Auto Shot");
    ARCANE_SHOT = ai->getSpellIdExact("Arcane Shot");
    EXPLOSIVE_SHOT = ai->getSpellIdExact("Explosive Shot");
    STEADY_SHOT = ai->getSpellIdExact("Steady Shot");
    AIMED_SHOT = ai->getSpellIdExact("Aimed Shot");
    SCATTER_SHOT = ai->getSpellIdExact("Scatter Shot");
    KILL_SHOT = ai->getSpellIdExact("Kill Shot");
    CHIMERA_SHOT = ai->getSpellIdExact("Chimera Shot");
    CONCUSSIVE_SHOT = ai->getSpellIdExact("Concussive Shot");
    DISTRACTING_SHOT = ai->getSpellIdExact("Distracting Shot");
    SILENCING_SHOT = ai->getSpellIdExact("Silencing Shot");

    // STINGS
    SERPENT_STING = ai->getSpellIdExact("Serpent Sting");
    SCORPID_STING = ai->getSpellIdExact("Scorpid Sting");
    WYVERN_STING = ai->getSpellIdExact("Wyvern Sting");
    VIPER_STING = ai->getSpellIdExact("Viper Sting");

    // DEBUFF
    HUNTERS_MARK = ai->getSpellIdExact("Hunter's Mark");
    SCARE_BEAST = ai->getSpellIdExact("Scare Beast");

    //AOE
    VOLLEY = ai->getSpellIdExact("Volley");
    MULTI_SHOT = ai->getSpellIdExact("Multi Shot");

    //MELEE
    RAPTOR_STRIKE = ai->getSpellIdExact("Raptor Strike");
    WING_CLIP = ai->getSpellIdExact("Wing Clip");
    MONGOOSE_BITE = ai->getSpellIdExact("Mongoose Bite");
    COUNTERATTACK = ai->getSpellIdExact("Counterattack");

    //TRAP
    FREEZING_TRAP = ai->getSpellIdExact("Freezing Trap");
    IMMOLATION_TRAP = ai->getSpellIdExact("Immolation Trap");
    FROST_TRAP = ai->getSpellIdExact("Frost Trap");
    EXPLOSIVE_TRAP = ai->getSpellIdExact("Explosive Trap");
    SNAKE_TRAP = ai->getSpellIdExact("Snake Trap");
    ARCANE_TRAP = ai->getSpellIdExact("Arcane Trap");
    FREEZING_ARROW = ai->getSpellIdExact("Freezing Arrow");
    BLACK_ARROW = ai->getSpellIdExact("Black Arrow");

    //BUFF
    TRUESHOT_AURA = ai->getSpellIdExact("Trueshot Aura");
    DETERRENCE = ai->getSpellIdExact("Deterrence");
    FEIGN_DEATH = ai->getSpellIdExact("Feign Death");
    DISENGAGE = ai->getSpellIdExact("Disengage");
    RAPID_FIRE = ai->getSpellIdExact("Rapid Fire");
    READINESS = ai->getSpellIdExact("Readiness");
    MISDIRECTION = ai->getSpellIdExact("Misdirection");

    //ASPECT
    ASPECT_OF_THE_HAWK = ai->getSpellIdExact("Aspect of the Dragonhawk");
    ASPECT_OF_THE_MONKEY = ASPECT_OF_THE_HAWK;
    if (!ASPECT_OF_THE_HAWK) ASPECT_OF_THE_HAWK = ai->getSpellIdExact("Aspect of the Hawk");
    if (!ASPECT_OF_THE_MONKEY) ASPECT_OF_THE_MONKEY = ai->getSpellIdExact("Aspect of the Monkey");
    ASPECT_OF_THE_VIPER = ai->getSpellIdExact("Aspect of the Viper");

    TALENT_MM = TRUESHOT_AURA;
    TALENT_BM = BESTIAL_WRATH;
    TALENT_SURVIVAL = WYVERN_STING;

    uint8 talentCounter = 0;
    if (TALENT_MM) talentCounter++;
    if (TALENT_BM) talentCounter++;
    if (TALENT_SURVIVAL) talentCounter++;
    if (talentCounter > 1) { TALENT_MM = 0; TALENT_BM = 0; TALENT_SURVIVAL = 0; } //Unreliable Talent detection.

    #pragma endregion
}

bool PlayerbotHunterAI::HasPet(Player* bot)
{
    QueryResult result = CharacterDatabase.PQuery("SELECT * FROM character_pet WHERE owner = '%u' AND (slot = '%u' OR slot = '%u')",bot->GetGUIDLow(),PET_SAVE_AS_CURRENT,PET_SAVE_NOT_IN_SLOT);

    if(result)
        return true; //hunter has current pet
    else
        return false; //hunter either has no pet or stabled
}// end HasPet

void PlayerbotHunterAI::DoNextCombatManeuver(Unit *pTarget)
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

 //   switch (ai->GetScenarioType())
//    {
//        case PlayerbotAI::SCENARIO_DUEL:
 //           ai->CastSpell(RAPTOR_STRIKE);
 //           return;
//    }

    // ------- Non Duel combat ----------


    #pragma region Choose Target
    // Choose Target
    if (isUnderAttack()) // I am under attack
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

    #pragma endregion

    #pragma region Pet Actions
    // Pet's own Actions
    if( pet && pet->isAlive() )
    {
        // Setup pet
        if (pet->GetCharmInfo()->IsAtStay()) {pet->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW); }

        //Heal pet
        if ( ( ((float)pet->GetHealth()/(float)pet->GetMaxHealth()) < 0.5f )
        && ( PET_MEND>0 && !pet->getDeathState() != ALIVE && pVictim != m_bot
        && CastSpell(PET_MEND,m_bot) )) { return; }

        // Set pet to attack hunter's attacker > its own attackers > hunter's target
        if (!pet->getVictim()) { pet->AI()->AttackStart(pTarget); }
        else if (isUnderAttack(m_bot)) { pet->AI()->AttackStart(pTarget); }  //Always help hunter if she's under attack
        else if (pet->getVictim()->GetGUID() != pTarget->GetGUID() && !isUnderAttack(pet)) { pet->AI()->AttackStart(pTarget); }
        else if (isUnderAttack(pet)) // Pet is under attack and hunter has no attackers
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
            //Actions to do under attack (Always tank it, and try to kill it, until someone (!= hunter) takes aggro back)
            //Hunter should help her pet whether main tank or not, unless she's being attacked (BEWARE Targeting Loop possibility)
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
            if (GROWL) pet->GetCharmInfo()->SetSpellAutocast(GROWL,true); //Autocast growl
            if (BAD_ATTITUDE) pet->GetCharmInfo()->SetSpellAutocast(BAD_ATTITUDE,true);
            if (COWER) pet->GetCharmInfo()->SetSpellAutocast(COWER,false);
            if (CastSpell(INTIMIDATION,m_bot)) { return; }
        }
        else
        {
            if (GROWL) pet->GetCharmInfo()->SetSpellAutocast(GROWL,false); //Do not try to get aggro
            if (BAD_ATTITUDE) pet->GetCharmInfo()->SetSpellAutocast(BAD_ATTITUDE,false);
            if (COWER) pet->GetCharmInfo()->SetSpellAutocast(COWER,true); //Autocast cower
        }
        // NORMAL PET dps attacks
        if (petThreat < threatThreshold || pet->GetGUID() == m_tank->GetGUID() || isUnderAttack(m_bot))
        {
            if (CastSpell(KILL_COMMAND,m_bot)) { }
            else if (CastSpell(BESTIAL_WRATH,m_bot)) { }
        }
        // NETHERSHOCK DEMORALIZINGSCREECH
    }
    #pragma endregion

    // If there's a cast stop
    if(m_bot->HasUnitState(UNIT_STAT_CASTING)) return;

    // Cast CC breakers if any match found  (does not work yet)
    // uint32 ccSpells[4] = { R_ESCAPE_ARTIST, R_EVERY_MAN_FOR_HIMSELF, R_WILL_OF_FORSAKEN, R_STONEFORM };
    // if (castSelfCCBreakers(ccSpells)) { } //most of them dont have gcd

    #pragma region Evasive manuevers
    // Do evasive manuevers if under attack
    if (isUnderAttack())
    {
        if (m_tank->GetGUID() == m_bot->GetGUID()) { } // i am tank and my pet is probably dead, so i have to face the attackers
        else if (CastSpell(FEIGN_DEATH,m_bot)) { return; } //avoid attack
        //else if (m_bot->getRace() == (uint8) RACE_NIGHTELF && CastSpell(R_SHADOWMELD,m_bot) ) { return; }
        else if (CastSpell(CONCUSSIVE_SHOT,pTarget)) { return; }
        else if (CastSpell(WYVERN_STING,pTarget)) { return; }
        else if (CastSpell(SCATTER_SHOT,pTarget)) { return; }
        else if (CastSpell(FREEZING_ARROW,pTarget)) { return; }
        else if (CastSpell(MISDIRECTION,m_tank)) { return; }
        else if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget) ) { return; } //no gcd but is cast
        else if (pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_BEAST && CastSpell(SCARE_BEAST,pTarget)) { return; }
        else if (pDist <= 2 && CastSpell(FREEZING_TRAP,pTarget)) { return; }
    }
    #pragma endregion

    //Select combat mode
    m_role = BOT_ROLE_DPS_RANGED;
    if ((isUnderAttack()  && pDist <= ATTACK_DISTANCE) || !m_bot->GetUInt32Value(PLAYER_AMMO_ID) ) { m_role = BOT_ROLE_DPS_MELEE; }

    TakePosition(pTarget);

    #pragma region Buff / Protect
    //Buff UP
    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot) ) {  } //no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot) ) { } //no GCD
    if (CastSpell(TRUESHOT_AURA, m_bot)) { return; }
    if (CastSpell(RAPID_FIRE,m_bot)) { return; }
    if (CastSpell(HUNTERS_MARK,pTarget)) { return; }
    if ((ai->GetHealthPercent() < 80 || ai->GetManaPercent() < 60 ) && CastSpell(READINESS,m_bot)) { } //no gcd


    //Protect yourself if needed
    if (m_bot->getRace() == (uint8) RACE_DWARF && ai->GetHealthPercent() < 75 && CastSpell(R_STONEFORM,m_bot) ) { } //no gcd
    if (ai->GetHealthPercent() < 20 && CastSpell(DETERRENCE,m_bot)) {} //No GCD
    if (m_bot->getRace() == (uint8) RACE_DRAENEI && ai->GetHealthPercent() < 55 && CastSpell(R_GIFT_OF_NAARU,m_bot)) { return;  }

    //Break Spells
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && ( pTarget->IsNonMeleeSpellCasted(true) || ai->GetManaPercent() < 20 ) && CastSpell(R_ARCANE_TORRENT, pTarget) ) { } //no gcd
    if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(SILENCING_SHOT, pTarget) ) { return; }
    if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(SCATTER_SHOT, pTarget) ) { return; }

    //Catch
    if (pTarget->HasUnitMovementFlag(UNIT_FLAG_FLEEING))
    {
        if (CastSpell(WING_CLIP,pTarget)) return;
        if (CastSpell(CONCUSSIVE_SHOT,pTarget)) return;
        if (CastSpell(SCATTER_SHOT, pTarget) ) { return; }
    }
    #pragma endregion

    //Do combat
    switch (m_role)
    {
        #pragma region BOT_ROLE_DPS_MELEE
        case BOT_ROLE_DPS_MELEE:
            if (AUTO_SHOT) { m_bot->InterruptNonMeleeSpells( true, AUTO_SHOT ); } //Stop autoshot
            if (CastSpell(ASPECT_OF_THE_MONKEY,m_bot)) { return; } //Get Monkey aspect

            if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; } //no gcd but is cast

            // Threat control
            if (pThreat < threatThreshold || m_tank->GetGUID() == m_bot->GetGUID() || m_bot->HasAura(MISDIRECTION) ) { } //Continue attack
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
                else if (CastSpell(FEIGN_DEATH,m_bot)) { return; }
                else { return; } // No more threat reducing spells, just slow down
            }

            if (CastSpell(RAPTOR_STRIKE,pTarget,true,true)) {} //No gcd
            if (CastSpell(MONGOOSE_BITE,pTarget,true,true)) { return; } // Cannot be sure if casted or not
            else if (CastSpell(COUNTERATTACK,pTarget,true,true)) { return; } // Cannot be sure if casted or not
            if (CastSpell(WING_CLIP,pTarget)) { return; }
            if (isUnderAttack(m_tank,6) && CastSpell(SNAKE_TRAP,m_bot)) { return; }
            if (isUnderAttack(m_tank,4) && CastSpell(EXPLOSIVE_TRAP,m_bot)) { return; }
            if (CastSpell(IMMOLATION_TRAP,m_bot)) { return; }
            break;
        #pragma endregion

        #pragma region BOT_ROLE_DPS_RANGED
        case BOT_ROLE_DPS_RANGED:
            if (m_pulling) {
                if (GetAI()->CastSpell(CONCUSSIVE_SHOT,pTarget) ||
                    GetAI()->CastSpell(AUTO_SHOT,pTarget)) {
                    m_pulling = false;
                    GetAI()->SetCombatOrder(ORDERS_NONE);
                    GetAI()->Follow(*GetMaster());
                    GetAI()->SetIgnoreUpdateTime(2);

                    if(HasPet(GetPlayerBot()))
                        m_bot->GetPet()->SetReactState(REACT_DEFENSIVE);
                }
                return;
            }
            if (AUTO_SHOT && !m_bot->FindCurrentSpellBySpellId(AUTO_SHOT)) { ai->CastSpell(AUTO_SHOT,pTarget); } //Start autoshot
            if (!(ai->GetManaPercent() < 85 && m_bot->HasAura(ASPECT_OF_THE_VIPER)) && CastSpell(ASPECT_OF_THE_HAWK,m_bot)) { return; } //Get Hawk aspect
            if ((ai->GetManaPercent() < 25) && CastSpell(ASPECT_OF_THE_VIPER,m_bot,true,false,true)) { return; } //Build up mana

            // if i am main tank, protect master by taunt
            if(m_tank->GetGUID() == m_bot->GetGUID())
            {
                // Taunt if needed (Only for master)
                Unit *curAtt = GetAttackerOf(GetMaster());
                if (curAtt && CastSpell(DISTRACTING_SHOT, curAtt))  { return; }
                // My target is not attacking me, taunt..
                if (pVictim && pVictim->GetGUID() != m_bot->GetGUID() && CastSpell(DISTRACTING_SHOT, pTarget) )  { return; }
            }
            // If i am not tank, transfer threat to tank or pet..
            else
            {
                if (CastSpell(MISDIRECTION,m_tank)) { return; }
                if (pet && pet->isAlive() && CastSpell(MISDIRECTION,pet)) { return; }

                // Threat control
                if (pThreat < threatThreshold || m_bot->HasAura(MISDIRECTION) ) { } //Continue attack
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
                    else if (CastSpell(FEIGN_DEATH,m_bot)) { return; }
                    else { return; } // No more threat reducing spells, just slow down
                }
            }

            // DO dps
            if (ai->GetHealthPercent(*pTarget) < 20 && CastSpell(KILL_SHOT,pTarget)) { return; }
            if (isUnderAttack(m_tank,4) && CastSpell(MULTI_SHOT,pTarget)) { return; }
            if (isUnderAttack(m_tank,4) && CastSpell(VOLLEY,pTarget)) { GetAI()->SetIgnoreUpdateTime(7); return; }
            if (CanCast(CHIMERA_SHOT,pTarget) &&
                (pTarget->HasAura(VIPER_STING,m_bot->GetGUID()) || pTarget->HasAura(SERPENT_STING,m_bot->GetGUID()) )
                && CastSpell(CHIMERA_SHOT,pTarget,false) ) { return; }
            if (ai->GetManaPercent() < 60 && ai->GetManaPercent(*pTarget) > 4 && CastSpell(VIPER_STING,pTarget)) { return; }
            if (!pTarget->HasAura(VIPER_STING,m_bot->GetGUID()) && CastSpell(SERPENT_STING,pTarget)) { return; }
            if (CastSpell(ARCANE_SHOT,pTarget)) { return; }
            if (CastSpell(BLACK_ARROW,pTarget)) { return; }
            if (CastSpell(EXPLOSIVE_SHOT,pTarget)) { return; }
            if (CastSpell(STEADY_SHOT,pTarget)) { return; }
            break;
        #pragma endregion
    }

    /*// drink potion if support / healer (Other builds simply overuse mana and waste mana pots)
    if(ai->GetManaPercent() < 5 && (m_role == BOT_ROLE_SUPPORT || m_role == BOT_ROLE_HEALER) )
    {
        Item *pItem = ai->FindPotion();
        if(pItem != NULL)
        {
            if (pItem->GetSpell() && m_bot->HasSpellCooldown(pItem->GetSpell()) ) { return; } //pot is in cooldown
            ai->UseItem(*pItem);
        }
    }*/
} // end DoNextCombatManeuver

void PlayerbotHunterAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    // buff group
    if (CastSpell(TRUESHOT_AURA, m_bot)) { return; }

    //mana/hp check
    //Don't bother with eating, if low on hp, just let it heal themself
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (ai->GetManaPercent() < 20 || ai->GetHealthPercent() < 30) { ai->Feast(); }

    #pragma region Check Pet
    // check for pet
    if( PET_SUMMON>0 && !m_petSummonFailed && HasPet(m_bot) )
    {
        // we can summon pet, and no critical summon errors before
        Pet *pet = m_bot->GetPet();
        if( !pet )
        {
            // summon pet
            if( PET_SUMMON>0 && ai->CastSpell(PET_SUMMON,m_bot) )
                ai->TellMaster( "summoning pet." );
            else
            {
                m_petSummonFailed = true;
                ai->TellMaster( "summon pet failed!" );
            }
        }
        else if( pet->getDeathState() != ALIVE )
        {
            // revive pet
            if( PET_REVIVE>0 && ai->GetManaPercent()>=80 && ai->CastSpell(PET_REVIVE,m_bot) )
                ai->TellMaster( "reviving pet." );
        }
        else if( ((float)pet->GetHealth()/(float)pet->GetMaxHealth()) < 0.5f )
        {
            // heal pet when health lower 50%
            if( PET_MEND>0 && !pet->getDeathState() != ALIVE && !pet->HasAura(PET_MEND,0) && ai->GetManaPercent()>=13 && ai->CastSpell(PET_MEND,m_bot) )
                ai->TellMaster( "healing pet." );
        }
        else if(pet->GetHappinessState() != HAPPY) // if pet is hungry
        {
            Unit *caster = (Unit*)m_bot;
            // list out items in main backpack
            for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
            {
                Item* const pItem = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
                if (pItem)
                {
                    const ItemPrototype* const pItemProto = pItem->GetProto();
                    if (!pItemProto )
                        continue;
                    if(pet->HaveInDiet(pItemProto)) // is pItem in pets diet
                    {
                        //sLog.outDebug("Food for pet: %s",pItemProto->Name1);
                        caster->CastSpell(caster,51284,true); // pet feed visual
                        uint32 count = 1; // number of items used
                        int32 benefit = pet->GetCurrentFoodBenefitLevel(pItemProto->ItemLevel); // nutritional value of food
                        m_bot->DestroyItemCount(pItem,count,true); // remove item from inventory
                        m_bot->CastCustomSpell(m_bot,PET_FEED,&benefit,NULL,NULL,true); // feed pet
                        ai->TellMaster( "feeding pet." );
                        ai->SetIgnoreUpdateTime(10);
                        return;
                    }
                }
            }
            // list out items in other removable backpacks
            for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
            {
                const Bag* const pBag = (Bag*) m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
                if (pBag)
                {
                    for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
                    {
                        Item* const pItem = m_bot->GetItemByPos(bag, slot);
                        if (pItem)
                        {
                            const ItemPrototype* const pItemProto = pItem->GetProto();
                            if (!pItemProto )
                                continue;
                            if(pet->HaveInDiet(pItemProto)) // is pItem in pets diet
                            {
                                //sLog.outDebug("Food for pet: %s",pItemProto->Name1);
                                caster->CastSpell(caster,51284,true); // pet feed visual
                                uint32 count = 1; // number of items used
                                int32 benefit = pet->GetCurrentFoodBenefitLevel(pItemProto->ItemLevel); // nutritional value of food
                                m_bot->DestroyItemCount(pItem,count,true); // remove item from inventory
                                m_bot->CastCustomSpell(m_bot,PET_FEED,&benefit,NULL,NULL,true); // feed pet
                                ai->TellMaster( "feeding pet." );
                                ai->SetIgnoreUpdateTime(10);
                                return;
                            }
                        }
                    }
                }
            }
            if( pet->HasAura(PET_MEND, 0) && !pet->HasAura(PET_FEED, 0))

                ai->TellMaster( "..no pet food!" );
                ai->SetIgnoreUpdateTime(7);
        }
    #pragma endregion
    }
} // end DoNonCombatActions

void PlayerbotHunterAI::Pull()
{
    if (!AUTO_SHOT) return;

    // check ammo
    uint32 ammo_id = GetPlayerBot()->GetUInt32Value(PLAYER_AMMO_ID);
    if (!ammo_id) {
        GetPlayerBot()->Say("I'm out of ammo.", LANG_UNIVERSAL);
        return;
    }

    Unit* pTarget = ObjectAccessor::GetUnit(*GetMaster(), GetMaster()->GetSelection());
    if (pTarget==NULL || pTarget->IsFriendlyTo(GetMaster()))
    {
        GetPlayerBot()->Say("Invalid target", LANG_UNIVERSAL);
        GetAI()->Follow(*GetMaster());
        return;
    }

    m_role = BOT_ROLE_DPS_RANGED;
    m_pulling = true;
    GetAI()->SetIgnoreUpdateTime(0);

    if(GetPlayerBot()->GetPet())
        GetPlayerBot()->GetPet()->SetReactState(REACT_PASSIVE);
}
