/*
Name : PlayerbotWarrior.cpp
Complete: maybe around 75%

Limitations:    - Talent build decision is made by key talent spells, which makes them viable only after level 50-ish.. Until then default behaviour is Blood dps/offtank type
                - Tanking bots should taunt if any group member is under attack, currently only saves master
                - Situations needing Intervene casting : limited / non-existant..
                - Intervene / Piercing Howl / Hamstring are not used..

Authors : SwaLLoweD
Version : 0.40
*/

#include "PlayerbotWarriorAI.h"

class PlayerbotAI;
PlayerbotWarriorAI::PlayerbotWarriorAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}
PlayerbotWarriorAI::~PlayerbotWarriorAI(){}

void PlayerbotWarriorAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
   #pragma region SpellId Fill
    //Defensive Stance
    SHIELD_WALL = ai->getSpellIdExact("Shield Wall");
    REVENGE = ai->getSpellIdExact("Revenge");
    SHIELD_BLOCK = ai->getSpellIdExact("Shield Block");
    DISARM = ai->getSpellIdExact("Disarm");
    INTERVENE = ai->getSpellIdExact("Intervene");

    //Berserker Stance
    RECKLESSNESS = ai->getSpellIdExact("Recklessness");
    WHIRLWIND = ai->getSpellIdExact("Whirlwind");
    PUMMEL = ai->getSpellIdExact("Pummel");
    INTERCEPT = ai->getSpellIdExact("Intercept");

    //Battle Stance
    RETALIATION = ai->getSpellIdExact("Retaliation");
    CHARGE = ai->getSpellIdExact("Charge");
    OVERPOWER = ai->getSpellIdExact("Overpower");
    SHATTERING_THROW = ai->getSpellIdExact("Shattering Throw");

    //Mixed Attacks
    REND = ai->getSpellIdExact("Rend");                                // 1 2
    THUNDER_CLAP = ai->getSpellIdExact("Thunder Clap");
    SPELL_REFLECTION = ai->getSpellIdExact("Spell Reflection");
    SHIELD_BASH = ai->getSpellIdExact("Shield Bash");
    EXECUTE = ai->getSpellIdExact("Execute");                        // 1 3
    HAMSTRING = ai->getSpellIdExact("Hamstring");
    SWEEPING_STRIKES = ai->getSpellIdExact("Sweeping Strikes");
    VICTORY_RUSH = ai->getSpellIdExact("Victory Rush");


    //General attacks
    HEROIC_STRIKE = ai->getSpellIdExact("Heroic Strike");
    MORTAL_STRIKE = ai->getSpellIdExact("Mortal Strike");
    BLOODTHIRST = ai->getSpellIdExact("Bloodthirst");
    SHIELD_SLAM = ai->getSpellIdExact("Shield Slam");
    SHOCKWAVE = ai->getSpellIdExact("Shockwave");
    SLAM = ai->getSpellIdExact("Slam");
    CLEAVE = ai->getSpellIdExact("Cleave");
    BLADESTORM = ai->getSpellIdExact("Bladestorm");
    HEROIC_THROW = ai->getSpellIdExact("Heroic Throw");
    CONCUSSION_BLOW = ai->getSpellIdExact("Concussion Blow");
    SUNDER_ARMOR = ai->getSpellIdExact("Sunder Armor");
    DEMORALIZING_SHOUT = ai->getSpellIdExact("Demoralizing Shout");
    INTIMIDATING_SHOUT = ai->getSpellIdExact("Intimidating Shout");
    PIERCING_HOWL = ai->getSpellIdExact("Piercing Howl");
    DEVASTATE = ai->getSpellIdExact("Devastate");


    //buffs
    COMMANDING_SHOUT = ai->getSpellIdExact("Commanding Shout");
    BATTLE_SHOUT = ai->getSpellIdExact("Battle Shout");
    VIGILANCE = ai->getSpellIdExact("Vigilance");
    BERSERKER_RAGE = ai->getSpellIdExact("Berserker Rage");
    ENRAGED_REGENERATION = ai->getSpellIdExact("Enraged Regeneration");
    BLOODRAGE = ai->getSpellIdExact("Bloodrage");
    LAST_STAND = ai->getSpellIdExact("Last Stand");
    HEROIC_FURY = ai->getSpellIdExact("Heroic Fury");
    DEATH_WISH = ai->getSpellIdExact("Death Wish");


    //Stances
    DEFENSIVE_STANCE = ai->getSpellIdExact("Defensive Stance");
    BATTLE_STANCE = ai->getSpellIdExact("Battle Stance");
    BERSERKER_STANCE = ai->getSpellIdExact("Berserker Stance");


    //Taunts
    TAUNT = ai->getSpellIdExact("Taunt");
    CHALLENGING_SHOUT = ai->getSpellIdExact("Challenging Shout");
    MOCKING_BLOW = ai->getSpellIdExact("Mocking Blow");

    //Special
    SLAMM = 46916; //Instant Slam (Blood Surge)

    TALENT_ARMS = MORTAL_STRIKE;
    TALENT_FURY = BLOODTHIRST;
    TALENT_PROT = DEVASTATE;

    SHOOT = ai->getSpellIdExact("Shoot");

    uint8 talentCounter = 0;
    if (TALENT_ARMS) talentCounter++;
    if (TALENT_FURY) talentCounter++;
    if (TALENT_PROT) talentCounter++;
    if (talentCounter > 1) { TALENT_ARMS = 0; TALENT_FURY = 0; TALENT_PROT = 0; } //Unreliable Talent detection.
    #pragma endregion
}

void PlayerbotWarriorAI::DoNextCombatManeuver(Unit *pTarget)
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

    if (!m_pulling){
        #pragma region Choose Role / Stance

        m_role = BOT_ROLE_DPS_MELEE;

        // Choose Stance
        if (m_tank->GetGUID() == m_bot->GetGUID()) // Hey! I am Main Tank
        {
            if (ChangeStance(DEFENSIVE_STANCE)) { m_role = BOT_ROLE_TANK; return; }  //m_bot->GetShield(true)
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
            //if (m_bot->getRace() == (uint8) RACE_NIGHTELF && CanCast(R_SHADOWMELD,m_bot) && CastSpell(R_SHADOWMELD,m_bot) ) { return; }
            if (m_bot->GetShield(true)) { if (ChangeStance(DEFENSIVE_STANCE)) { m_role = BOT_ROLE_OFFTANK; return; } }
            else if (ChangeStance(BATTLE_STANCE)) { return; }
        }
        else if (ai->GetHealthPercent() > 90)
        {
            if (ChangeStance(BERSERKER_STANCE)) { return; }
        }
        else if (ai->GetForm() != FORM_BERSERKERSTANCE || ai->GetHealthPercent() < 70 ) { if (ChangeStance(BATTLE_STANCE)) { return; } }
        #pragma endregion
    }

    // Cast CC breakers if any match found  (does not work yet)
    // uint32 ccSpells[7] = { HEROIC_FURY, BERSERKER_RAGE, BLADESTORM, R_ESCAPE_ARTIST, R_EVERY_MAN_FOR_HIMSELF, R_WILL_OF_FORSAKEN, R_STONEFORM };
    // if (castSelfCCBreakers(ccSpells)) { } //most of them dont have gcd

    TakePosition(pTarget);

    // If there's a cast stop
    if(m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }

    if (m_pulling) {
        if (GetAI()->CastSpell(SHOOT,pTarget)) {
            m_pulling = false;
            GetAI()->SetCombatOrder(ORDERS_NONE);
            GetAI()->Follow(*GetMaster());
            GetAI()->SetIgnoreUpdateTime(2);
         }
          return;
    }

    #pragma region Buff Heal Interrupt
    //Buff UP
    if (!m_bot->HasAura(BATTLE_SHOUT) && CastSpell(BATTLE_SHOUT,m_bot)) { return; }
    if (!m_bot->HasAura(BATTLE_SHOUT,m_bot->GetGUID()) && !m_bot->HasAura(COMMANDING_SHOUT) && CastSpell(COMMANDING_SHOUT,m_bot)) { return; }


    //HEAL UP && PROTECT UP
    if (ai->GetHealthPercent() <= 85 && CastSpell(SHIELD_BLOCK, m_bot)) { } //no GCD
    if (ai->GetHealthPercent() <= 45 && CastSpell(SHIELD_WALL, m_bot)) { return; }
    if (ai->GetHealthPercent() < 55 &&
        (m_bot->HasAura(BERSERKER_RAGE) || m_bot->HasAura(BLOODRAGE) || m_bot->HasAura(DEATH_WISH)) //There are other spells that count as enrage
        && CastSpell(ENRAGED_REGENERATION,m_bot)) { return; }
    if (ai->GetHealthPercent() < 25 && CastSpell(INTIMIDATING_SHOUT, m_bot)) { return; }
    if (ai->GetHealthPercent() <= 75 && CastSpell(LAST_STAND, m_bot)) { return; }
    if (m_bot->getRace() == (uint8) RACE_DWARF && ai->GetHealthPercent() < 75 && CastSpell(R_STONEFORM,m_bot)) { } //no gcd
    if (m_bot->getRace() == (uint8) RACE_DRAENEI && ai->GetHealthPercent() < 55 && CastSpell(R_GIFT_OF_NAARU,m_bot)) { return; } //no Gcd, but has cast

    //Break spells being cast
    if (pTarget->IsNonMeleeSpellCasted(true))
    {
        if (pVictim && pVictim->GetGUID() == m_bot->GetGUID() && CastSpell(SPELL_REFLECTION,pTarget)) { return; }
        if (m_bot->HasAura(SPELL_REFLECTION))
        {
            if (CastSpell(SHIELD_BASH,pTarget)) {} // No GCD
            else if (CastSpell(PUMMEL,pTarget)) { return; }
            else if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
        }
    }
    #pragma endregion

    #pragma region Taunt / Threat
    // if i am main tank, protect master by taunt
    if(m_tank->GetGUID() == m_bot->GetGUID())
    {
        // Taunt if needed (Only for master)
        Unit *curAtt = GetAttackerOf(GetMaster());
        if (curAtt)
        {
            if (isUnderAttack(GetMaster(),2) && CastSpell(CHALLENGING_SHOUT, curAtt)) { return; }
            if (CastSpell(TAUNT, curAtt,true,true))  { return; }
            if (CastSpell(VIGILANCE, GetMaster())) { return; }
            if (CastSpell(TAUNT, curAtt))  { return; }
            if (CastSpell(MOCKING_BLOW, curAtt)) { return; }
        }
        // My target is not attacking me, taunt..
        if (pVictim && pVictim->GetGUID() != m_bot->GetGUID())
        {
            if (CastSpell(VIGILANCE, pVictim)) { return; }
            if (CastSpell(TAUNT, pTarget))  { return; }
            if (CastSpell(MOCKING_BLOW, pTarget)) { return; }
        }
    }

    // If not in Defensive Stance slow down due to threat
    if (pThreat > threatThreshold && ai->GetForm() != FORM_DEFENSIVESTANCE && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack() )
    {
        if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
        {
            m_bot->SetSelection(m_tank->getVictim()->GetGUID());
            return;
        }
        else { return; } //Warrior has no threat reducing spells, just slow down
    }
    #pragma endregion

    #pragma region Dps

    //Ranged Stuff (Openers)
    if (CastSpell(CHARGE,pTarget)) { } //no GCD
    else if (CastSpell(INTERCEPT,pTarget)) { } //no GCD
    if (pDist > MELEE_RANGE && ai->GetForm() == FORM_DEFENSIVESTANCE && CastSpell(HEROIC_THROW,pTarget)) { return; } //High threat
    if (pDist > MELEE_RANGE && CastSpell(SHATTERING_THROW,pTarget)) { return; }

    //Catch
    if (pTarget->HasUnitMovementFlag(UNIT_FLAG_FLEEING))
    {
        if (CastSpell(HAMSTRING,pTarget)) return;
        if (CastSpell(PIERCING_HOWL,pTarget)) return;
    }


    //Dps up
    if (ai->GetHealthPercent() > 90 && ai->GetRageAmount() < 20 && CastSpell(BLOODRAGE,m_bot))  { return; }
    if (isUnderAttack() && CastSpell(RETALIATION,m_bot)) { return; }
    if (ai->GetHealthPercent() > 90 && CastSpell(DEATH_WISH,m_bot)) { return; }
    if (ai->GetHealthPercent() > 80 && CastSpell(RECKLESSNESS,m_bot)) { return; }
    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} //no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} //no GCD

    //Tank only stuff
    if ((ai->GetForm() == FORM_DEFENSIVESTANCE || ai->GetRageAmount() > 85) && CastSpell(THUNDER_CLAP)) { return; } //High threat
    if ((ai->GetForm() == FORM_DEFENSIVESTANCE || ai->GetRageAmount() > 75) && CastSpell(HEROIC_STRIKE)) {} //nogcd high threat

    //Finishing Move / Timed moves
    if (ai->GetHealthPercent(*pTarget) < 20 && CastSpell(EXECUTE,pTarget)) { return; }
    if (CastSpell(VICTORY_RUSH,pTarget)) { return; }

    //AOE
    if (CastSpell(SHOCKWAVE,pTarget)) { return; }
    if ((isUnderAttack(m_tank,3) || m_tank->GetGUID() == m_bot->GetGUID()) && CastSpell(CLEAVE,pTarget)) {} //no GCD
    if (isUnderAttack(m_tank,3) && CastSpell(SWEEPING_STRIKES,m_bot)) {} //no GCD
    if (isUnderAttack(m_tank,4) && CastSpell(BLADESTORM,m_bot)) { return; }
    if (isUnderAttack(m_tank,4) && CastSpell(WHIRLWIND,pTarget)) { return; }

    //Main dps
    if (m_bot->HasAura(SLAMM) && CastSpell(SLAM,pTarget)) { return; }  //instant slam only
    if (CastSpell(REVENGE,pTarget)) { return; } //Def stance only
    if (CastSpell(OVERPOWER,pTarget)) { return; }
    if (CastSpell(SHIELD_SLAM,pTarget)) { return; }
    if (CastSpell(BLOODTHIRST,pTarget)) { return; }
    if (CastSpell(MORTAL_STRIKE,pTarget)) { return; }


    //Support/Debuff
    if (CastSpell(DEMORALIZING_SHOUT,pTarget)) { return; }
    if (DEVASTATE) { if (CastSpell(DEVASTATE,pTarget,1,1)) { return; } }  //High threat
    else if (CastSpell(SUNDER_ARMOR)) { return; } //Only 1 - High threat
    if (CastSpell(CONCUSSION_BLOW,pTarget)) { return; }
    if (CastSpell(REND,pTarget)) { return; }
    if (CastSpell(DISARM,pTarget)) { return; }
    #pragma endregion

} //end DoNextCombatManeuver

void PlayerbotWarriorAI::DoNonCombatActions()
{
    PlayerbotAI *ai = GetAI();
    Player *m_bot = GetPlayerBot();
    if (!m_bot || !ai || m_bot->isDead()) { return; }

    //If Casting or Eating/Drinking return
    if (m_bot->HasUnitState(UNIT_STAT_CASTING)) { return; }
    if (m_bot->getStandState() == UNIT_STAND_STATE_SIT) { return; }

    //Buff Up
    if (!m_bot->HasAura(BATTLE_SHOUT) && CastSpell(BATTLE_SHOUT,m_bot)) { return; }
    if (!m_bot->HasAura(BATTLE_SHOUT,m_bot->GetGUID()) && !m_bot->HasAura(COMMANDING_SHOUT) && CastSpell(COMMANDING_SHOUT,m_bot)) { return; }

    if (GetMaster()->isAlive() && CastSpell(VIGILANCE, GetMaster())) { return; }

    //want to start off in battle stance so we can CHARGE
    //if(ai->GetRageAmount() < 20 && ai->GetForm() != FORM_BATTLESTANCE && ChangeStance(BATTLE_STANCE)) { return; }

    //mana/hp check
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (ai->GetHealthPercent() < 75) { ai->Feast(); }
} //end DoNonCombatActions

bool PlayerbotWarriorAI::ChangeStance(uint32 stance)
{
    if (stance == 0) return false;
    if (CastSpell(stance, GetPlayerBot())) { return true; }
    return false;
}

void PlayerbotWarriorAI::Pull()
{
    if (!SHOOT) return;

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
        m_pulling = false;
        GetAI()->Follow(*GetMaster());
        return;
    }

    m_role = BOT_ROLE_DPS_RANGED;
    m_pulling = true;
    GetAI()->SetIgnoreUpdateTime(0);
}

/*
void PlayerbotWarriorAI::BreakCC(const uint32 diff)
{
    if(pvpTrinket_cd < diff && GCD < diff)
    {
        if(m_creature->HasAuraType(SPELL_AURA_MOD_ROOT) ||
        m_creature->HasAuraType(SPELL_AURA_MOD_CONFUSE) || //dragons breath/blind/poly
        m_creature->HasAura(8983)                       || //Druid bash rank 3
        m_creature->HasAura(27006)                      || //Druid pounce rank 4
        m_creature->HasAura(33786)                      || //Druid cyclone
        m_creature->HasAura(22570, 1)                   || //Druid maim
        m_creature->HasAura(10308)                      || //Paladin hammer of justice rank 4
        m_creature->HasAura(30414, 1)                   || //Warlock shadowfury rank 3
        m_creature->HasAura(6215)                       || //Warlock fear rank 3 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
        m_creature->HasAura(17928)                      || //Warlock howlofterror rank 3 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
        m_creature->HasAura(10890)                      || //Priest psychic scream rank 4 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
        m_creature->HasAura(14902)                      || //Rogue Cheap shot
        m_creature->HasAura(8643)                       || //Rogue Kidney shot Rank 2
        m_creature->HasAura(38764, 2)                   || //Rogue Gouge Rank 6 **REMOVE THIS & IMPLEMENT IN BERSERKER RAGE**
        m_creature->HasAura(12809))                        //Warrior concussion blow
        {
            doCast(m_creature, PVPTRINKET); //I think it would be better to instead of applying individual spells that apply the
            pvpTrinket_cd = PVPTRINKET_CD;  //effect SPELL_AURA_MOD_STUN, just add that type and start removing bad choices e.g. impact.
        }

        if(m_creature->HasAura(11297) && m_creature->GetDistance(m_creature->getVictim()) < 10)
        {      //if warrior sapped and creature is less then 10 yards from war, cast pvp trinket and attempt to demo shout him out of stealth
            doCast(m_creature, PVPTRINKET);
            pvpTrinket_cd = PVPTRINKET_CD;
            castDemoralizingShout = true;
        }
    }
} //BreakCC
*/
