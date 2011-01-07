/*
Name : PlayerbotDeathknightAI.cpp
Complete: maybe around 65%

Limitations:    - Talent build decision is made by key talent spells, which makes them viable only after level 50-ish.. Until then default behaviour is Blood dps/offtank type
                - Tanking bots should taunt if any group member is under attack, currently only saves master
                - Situations needing Death grip casting : limited / non-existant..

Authors : SwaLLoweD, rrtn
Version : 0.40
*/

#include "PlayerbotDeathKnightAI.h"

class PlayerbotAI;
PlayerbotDeathKnightAI::PlayerbotDeathKnightAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}

PlayerbotDeathKnightAI::~PlayerbotDeathKnightAI() {}

void PlayerbotDeathKnightAI::LoadSpells(){
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    // Unholy
    PLAGUE_STRIKE = ai->getSpellIdExact("Plague Strike");
    DEATH_STRIKE = ai->getSpellIdExact("Death Strike");
    SCOURGE_STRIKE  = ai->getSpellIdExact("Scourge Strike");

    // Frost
    ICY_TOUCH = ai->getSpellIdExact("Icy Touch");
    OBLITERATE = ai->getSpellIdExact("Obliterate");

    // Blood
    BLOOD_STRIKE = ai->getSpellIdExact("Blood Strike");
    HEART_STRIKE  = ai->getSpellIdExact("Heart Strike");
    RUNE_TAP = ai->getSpellIdExact("Rune Tap");
    DARK_COMMAND = ai->getSpellIdExact("Dark Command");

    // AOE
    HOWLING_BLAST = ai->getSpellIdExact("Howling Blast");
    BLOOD_BOIL = ai->getSpellIdExact("Blood Boil");
    PESTILENCE = ai->getSpellIdExact("Pestilence");
    CORPSE_EXPLOSION = ai->getSpellIdExact("Corpse Explosion");
    DEATH_AND_DECAY = ai->getSpellIdExact("Death and Decay");

    // Rune attacks
    FROST_STRIKE = ai->getSpellIdExact("Frost Strike");
    DEATH_COIL = ai->getSpellIdExact("Death Coil");
    RUNE_STRIKE = ai->getSpellIdExact("Rune Strike");

    // CC Interrupt
    DEATH_GRIP = ai->getSpellIdExact("Death Grip");
    CHAINS_OF_ICE = ai->getSpellIdExact("Chains of Ice");
    MIND_FREEZE = ai->getSpellIdExact("Mind Freeze");
    HUNGERING_COLD = ai->getSpellIdExact("Hungering Cold");
    STRANGULATE = ai->getSpellIdExact("Strangulate");

    // Debuffs
    FROST_FEVER = 55095; //ai->getSpellIdExact("Frost Fever",true);
    BLOOD_PLAGUE = 55078; //ai->getSpellIdExact("Blood Plague",true);
    CRYPT_FEVER = ai->getSpellIdExact("Crypt Fever",true);
    EBON_PLAGUE = ai->getSpellIdExact("Ebon Plague",true);
    MARK_OF_BLOOD = ai->getSpellIdExact("Mark of Blood");

    // Buffs
    HORN_OF_WINTER = ai->getSpellIdExact("Horn of Winter");
    BONE_SHIELD = ai->getSpellIdExact("Bone Shield");
    VAMPIRIC_BLOOD = ai->getSpellIdExact("Vampiric Blood");
    HYSTERIA = ai->getSpellIdExact("Hysteria");
    UNBREAKABLE_ARMOR = ai->getSpellIdExact("Unbreakable Armor");
    ANTI_MAGIC_SHELL = ai->getSpellIdExact("Anti Magic Shell");
    ANTI_MAGIC_ZONE = ai->getSpellIdExact("Anti Magic Zone");
    ICEBOUND_FORTITUDE = ai->getSpellIdExact("Icebound Fortitude");
    EMPOWER_WEAPON = ai->getSpellIdExact("Empower Rune Weapon");
    LICHBORNE = ai->getSpellIdExact("Lichborne");

    // Summons
    RAISE_DEAD = ai->getSpellIdExact("Raise Dead");
    ARMY_OF_THE_DEAD = ai->getSpellIdExact("Army of the Dead");
    SUMMON_GARGOYLE = ai->getSpellIdExact("Summon Gargoyle");
    GHOUL_FRENZY = ai->getSpellIdExact("Ghoul Frenzy");
    DEATH_PACT = ai->getSpellIdExact("Death Pact");
    DANCING_WEAPON = ai->getSpellIdExact("Dancing Rune Weapon");

    // Presences
    BLOOD_PRESENCE = ai->getSpellIdExact("Blood Presence");
    FROST_PRESENCE = ai->getSpellIdExact("Frost Presence");
    UNHOLY_PRESENCE  = ai->getSpellIdExact("Unholy Presence");

    // Talent
    TALENT_BLOOD = HEART_STRIKE;
    TALENT_FROST = FROST_STRIKE;
    TALENT_UNHOLY = SCOURGE_STRIKE;

    uint8 talentCounter = 0;
    if (TALENT_BLOOD) talentCounter++;
    if (TALENT_FROST) talentCounter++;
    if (TALENT_UNHOLY) talentCounter++;
    if (talentCounter > 1) { TALENT_BLOOD = 0; TALENT_FROST = 0; TALENT_UNHOLY = 0; } //Unreliable Talent detection.
    #pragma endregion
}
void PlayerbotDeathKnightAI::DoNextCombatManeuver(Unit *pTarget)
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
    std::ostringstream out;


    if (!m_pulling)
    {
        m_role = BOT_ROLE_DPS_MELEE;
        #pragma region Choose Role/Presence

        // Choose Presence
        if (m_tank->GetGUID() == m_bot->GetGUID()) // Hey! I am Main Tank
        {
            if (CastSpell(FROST_PRESENCE,m_bot)) { m_role = BOT_ROLE_TANK; return; }
        }
        else if (isUnderAttack()) // I am under attack
        {
            if (pVictim && pVictim->GetGUID() == m_bot->GetGUID() && pDist <= 2)  {} // My target is almost up to me, no need to search
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
            //if (m_bot->getRace() == (uint8) RACE_NIGHTELF && CastSpell(R_SHADOWMELD,m_bot) ) { return; }
            if (CastSpell(FROST_PRESENCE,m_bot)) { m_role = BOT_ROLE_OFFTANK; return; }
        }
        else if (TALENT_UNHOLY)
        {
            if (CastSpell(UNHOLY_PRESENCE,m_bot)) return;
        }
        else if (CastSpell(BLOOD_PRESENCE,m_bot)) return;
        #pragma endregion
    }

    // Cast CC breakers if any match found  (does not work yet)
    // uint32 ccSpells[6] = { LICHBORNE, ICEBOUND_FORTITUDE, R_ESCAPE_ARTIST, R_EVERY_MAN_FOR_HIMSELF, R_WILL_OF_FORSAKEN, R_STONEFORM };
    // if (castSelfCCBreakers(ccSpells)) { } //most of them dont have gcd

    TakePosition(pTarget);


   if (m_pulling) {
        if (GetAI()->CastSpell(DEATH_GRIP,pTarget)) {
            m_pulling = false;
            GetAI()->SetCombatOrder(ORDERS_NONE);
            GetAI()->Follow(*GetMaster());
            GetAI()->SetIgnoreUpdateTime(2);

            if (m_bot->GetPet()) pet->SetReactState (REACT_DEFENSIVE);
         }
          return;
    }

    // If there's a cast stop
    if(m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }

    #pragma region Buff Heal Interrupt
    //Buff UP
    if (CastSpell(HORN_OF_WINTER,m_bot)) { return; }
    if (CastSpell(BONE_SHIELD,m_bot)) { return; }

    //HEAL UP && PROTECT UP
    if (ai->GetHealthPercent() < 80 && ai->GetHealthPercent() > 20 && CastSpell(VAMPIRIC_BLOOD,m_bot)) { }  //NO GCD
    if (ai->GetHealthPercent() < 65 && CastSpell(RUNE_TAP,m_bot)) { } //NO GCD
    if (CanCast(DEATH_STRIKE,pTarget,true) && ai->GetHealthPercent() < 90 &&
        (pTarget->HasAura(FROST_FEVER,m_bot->GetGUID()) ||pTarget->HasAura(BLOOD_PLAGUE,m_bot->GetGUID()) )
        && CastSpell(DEATH_STRIKE,pTarget,false) ) {return;}
    if (m_bot->getRace() == (uint8) RACE_DWARF && ai->GetHealthPercent() < 75 && CastSpell(R_STONEFORM,m_bot)) { } //no gcd
    if (m_bot->getRace() == (uint8) RACE_DRAENEI && ai->GetHealthPercent() < 55 && CastSpell(R_GIFT_OF_NAARU,m_bot)) { return; } //no Gcd, but has cast
    if (pet && ai->GetHealthPercent() < 50 && CastSpell(DEATH_PACT,m_bot)) { return; }
    if (pet && ai->GetHealthPercent() < 60 && CastSpell(MARK_OF_BLOOD,pTarget)) { return; }
    if (ai->GetHealthPercent() < 65 && CastSpell(ICEBOUND_FORTITUDE,m_bot)) { } //No GCD
    if (ai->GetHealthPercent() < 65 && CastSpell(UNBREAKABLE_ARMOR,m_bot)) { return; }

    //Break spells being cast
    if (pTarget->IsNonMeleeSpellCasted(true))
    {
        if (CastSpell(MIND_FREEZE,pTarget)) {} // No GCD
        if (CastSpell(STRANGULATE,pTarget)) { return; }
        if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
        if (CastSpell(ANTI_MAGIC_ZONE,m_bot)) { return; }
        if (CastSpell(ANTI_MAGIC_SHELL,m_bot)) {} //NO GCD
    }

    //Catch
    if (pTarget->HasUnitMovementFlag(UNIT_FLAG_FLEEING))
    {
        if (CastSpell(DEATH_GRIP,pTarget)) return;
        if (CastSpell(CHAINS_OF_ICE,pTarget)) return;
    }
    #pragma endregion

    #pragma region Taunt / Threat
    // if i am main tank, protect master by taunt
    if(m_tank->GetGUID() == m_bot->GetGUID())
    {
        // Taunt if needed (Only for master)
        Unit *curAtt = GetAttackerOf(GetMaster());
        if (curAtt && CastSpell(DARK_COMMAND, curAtt))  { }     //No gcd
        // My target is not attacking me, taunt..
        if (pVictim && pVictim->GetGUID() != m_bot->GetGUID() && CastSpell(DARK_COMMAND, pTarget) )  { } // No gcd
    }

    // If not in Frost Presence slow down due to threat
    if (pThreat > threatThreshold && !m_bot->HasAura(FROST_PRESENCE) && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack())
    {
        if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
        {
            m_bot->SetSelection(m_tank->getVictim()->GetGUID());
            return;
        }
        else { return; } //DK has no threat reducing spells, just slow down
    }
    #pragma endregion

    #pragma region Dps
    //Dps up
    if (CastSpell(EMPOWER_WEAPON,m_bot)) {} //NO GCD
    if (ai->GetHealthPercent() > 90 && CastSpell(HYSTERIA,m_bot)) {} //NO GCD
    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} //no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} //no GCD

    // Use up excess Runic Power
    if (ai->GetRunicPower() > 60 && CastSpell(FROST_STRIKE,pTarget)) { return; }
    else if (ai->GetRunicPower() > 60 && CastSpell(DEATH_COIL,pTarget,true,true,true)) { return; }
    if ((isUnderAttack() || ai->GetRunicPower() > 70) && CastSpell(RUNE_STRIKE,pTarget)) {} //Next attack spell

    // Build Diseases
    if (!pTarget->HasAura(FROST_FEVER,m_bot->GetGUID()) && CastSpell(ICY_TOUCH,pTarget)) { return; }
    if (!pTarget->HasAura(BLOOD_PLAGUE,m_bot->GetGUID()) && CastSpell(PLAGUE_STRIKE,pTarget)) { return; }

    // Use AOEs summons
    if (isUnderAttack(m_tank,4) && CastSpell(DEATH_AND_DECAY,pTarget)) { return; }
    if (isUnderAttack(m_tank,4) && CastSpell(HOWLING_BLAST,pTarget)) { return; }
    if (CanCast(PESTILENCE,pTarget,true) && isUnderAttack(m_tank,4) &&
        (pTarget->HasAura(FROST_FEVER,m_bot->GetGUID()) && pTarget->HasAura(BLOOD_PLAGUE,m_bot->GetGUID()) )
        && CastSpell(PESTILENCE,pTarget,false)) { return; }
    if (CanCast(BLOOD_BOIL,pTarget,true) && isUnderAttack(m_tank,4) &&
        (pTarget->HasAura(FROST_FEVER,m_bot->GetGUID()) || pTarget->HasAura(BLOOD_PLAGUE,m_bot->GetGUID()) )
        && CastSpell(BLOOD_BOIL,pTarget,false)) { return; }
    if (m_bot->getRace() == (uint8) RACE_TAUREN && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; } //no gcd but is cast
    if (isUnderAttack(m_tank,6) && CastSpell(ARMY_OF_THE_DEAD,m_bot)) { return; }
    if (isUnderAttack(m_tank,4) && CastSpell(SUMMON_GARGOYLE,pTarget)) { return; } //This should be somewhat different

    // Use standard damage spells
    if (CastSpell(HEART_STRIKE,pTarget,true,true)) { return; }
    if (CastSpell(BLOOD_STRIKE,pTarget)) { return; }
    if (TALENT_FROST && CastSpell(OBLITERATE,pTarget)) { return; }
    else if (TALENT_UNHOLY && CastSpell(SCOURGE_STRIKE,pTarget)) { return; }
    else if (CastSpell(DEATH_STRIKE,pTarget)) { return; }
    #pragma endregion

} // end DoNextCombatManeuver

void PlayerbotDeathKnightAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //Buff UP
    if (CastSpell(HORN_OF_WINTER,m_bot)) { return; }
    if (CastSpell(BONE_SHIELD,m_bot)) { return; }

    //mana/hp check
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (m_bot->GetHealth() < m_bot->GetMaxHealth() && CastSpell(RUNE_TAP,m_bot)) { return; } //no gcd but lets give the others a time to heal
    if (ai->GetHealthPercent() < 30) { ai->Feast(); }
    //Item* fItem = ai->FindBandage();
    /* if (pItem == NULL && fItem != NULL && !m_bot->HasAura(RECENTLY_BANDAGED, 0) && ai->GetHealthPercent() < 70)
    {
        ai->TellMaster("I could use first aid.");
        ai->UseItem(*fItem);
        ai->SetIgnoreUpdateTime(8);
        return;
    }  */
} // end DoNonCombatActions

void PlayerbotDeathKnightAI::Pull()
{
    Unit* pTarget = ObjectAccessor::GetUnit(*GetMaster(), GetMaster()->GetSelection());
    if (pTarget==NULL || pTarget->IsFriendlyTo(GetMaster()))
    {
        GetPlayerBot()->Say("Invalid target", LANG_UNIVERSAL);
        m_pulling = false;
        GetAI()->Follow(*GetMaster());
        return;
    }

    m_role = BOT_ROLE_DPS_RANGED;
    m_pulling = true;
    GetAI()->SetIgnoreUpdateTime(0);

    if (GetPlayerBot()->GetPet()) GetPlayerBot()->GetPet()->SetReactState (REACT_PASSIVE);
}
