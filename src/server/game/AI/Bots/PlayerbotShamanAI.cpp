#include "PlayerbotShamanAI.h"
class PlayerbotAI;
PlayerbotShamanAI::PlayerbotShamanAI(Player *const master, Player *const bot, PlayerbotAI *const ai): PlayerbotClassAI(master, bot, ai)
{
    foodDrinkSpamTimer = 0;
    LoadSpells();
}
PlayerbotShamanAI::~PlayerbotShamanAI(){}

void PlayerbotShamanAI::LoadSpells() {
    PlayerbotAI *ai = GetAI();
    if (!ai) return;
    #pragma region SpellId Fill
    //totems
    HEALING_STREAM_TOTEM = ai->getSpellIdExact("Healing Stream Totem");
    MANA_SPRING_TOTEM = ai->getSpellIdExact("Mana Spring Totem");
    MANA_TIDE_TOTEM = ai->getSpellIdExact("Mana Tide Totem");
    CLEANSING_TOTEM = ai->getSpellIdExact("Cleansing Totem");
    FIRE_RESISTANCE_TOTEM = ai->getSpellIdExact("Fire Resistance Totem");

    WINDFURY_TOTEM = ai->getSpellIdExact("Windfury Totem");
    WRATH_OF_AIR_TOTEM = ai->getSpellIdExact("Wrath of Air Totem");
    GROUNDING_TOTEM = ai->getSpellIdExact("Grounding Totem");
    NATURE_RESISTANCE_TOTEM = ai->getSpellIdExact("Nature Resistance Totem");

    STRENGTH_OF_EARTH_TOTEM = ai->getSpellIdExact("Strength of Earth Totem");
    EARTHBIND_TOTEM = ai->getSpellIdExact("Earthbind Totem");
    STONESKIN_TOTEM = ai->getSpellIdExact("Stoneskin Totem");
    STONECLAW_TOTEM = ai->getSpellIdExact("Stoneclaw Totem");
    TREMOR_TOTEM = ai->getSpellIdExact("Tremor Totem");
    EARTH_ELEMENTAL_TOTEM = ai->getSpellIdExact("Earth Elemental Totem");

    FLAMETONGUE_TOTEM = ai->getSpellIdExact("Flametongue Totem");
    TOTEM_OF_WRATH = ai->getSpellIdExact("Totem of Wrath");
    SEARING_TOTEM = ai->getSpellIdExact("Searing Totem");
    MAGMA_TOTEM = ai->getSpellIdExact("Magma Totem");
    FIRE_ELEMENTAL_TOTEM = ai->getSpellIdExact("Fire Elemental Totem");
    FROST_RESISTANCE_TOTEM = ai->getSpellIdExact("Frost Resistance Totem");

    TOTEMIC_RECALL = ai->getSpellIdExact("Totemic Recall");
    CALL_OF_THE_ELEMENTS = ai->getSpellIdExact("Call of the Elements");
    CALL_OF_THE_ANCESTORS = ai->getSpellIdExact("Call of the Ancestors");
    CALL_OF_THE_SPIRITS = ai->getSpellIdExact("Call of the Spirits");

    //restoration
    HEAL = ai->getSpellIdExact("Healing Wave");
    LESSER_HEAL = ai->getSpellIdExact("Lesser Healing Wave");
    CHAIN_HEAL = ai->getSpellIdExact("Chain Heal");
    RIPTIDE = ai->getSpellIdExact("Riptide");
    ANCESTRAL_SPIRIT = ai->getSpellIdExact("Ancestral Spirit");
    CLEANSE_SPIRIT = ai->getSpellIdExact("Cleanse Spirit");
    if (CLEANSE_SPIRIT) CLEANSE_SPIRIT = ai->getSpellIdExact("Cure Toxins");

    //offensive spells
    LIGHTNING_BOLT = ai->getSpellIdExact("Lightning Bolt");
    CHAIN_LIGHTNING = ai->getSpellIdExact("Chain Lightning");
    FIRE_NOVA = ai->getSpellIdExact("Fire Nova");
    THUNDERSTORM = ai->getSpellIdExact("Thunderstorm");
    LAVA_BURST = ai->getSpellIdExact("Lava Burst");
    EARTH_SHOCK = ai->getSpellIdExact("Earth Shock");
    WIND_SHEAR = ai->getSpellIdExact("Wind Shear");
    FLAME_SHOCK = ai->getSpellIdExact("Flame Shock");
    FROST_SHOCK = ai->getSpellIdExact("Frost Shock");
    PURGE = ai->getSpellIdExact("Purge");
    HEX  = ai->getSpellIdExact("Hex");

    //buffs
    LIGHTNING_SHIELD = ai->getSpellIdExact("Lightning Shield");
    WATER_SHIELD = ai->getSpellIdExact("Water Shield");
    EARTH_SHIELD = ai->getSpellIdExact("Earth Shield");
    HEROISM = ai->getSpellIdExact("Heroism");
    if (HEROISM) HEROISM = ai->getSpellIdExact("Bloodlust");
    ELEMENTAL_MASTERY = ai->getSpellIdExact("Elemental Mastery");
    NATURES_SWIFTNESS = ai->getSpellIdExact("Nature's Swiftness");

    WINDFURY_WEAPON = ai->getSpellIdExact("Windfury Weapon");
    FLAMETONGUE_WEAPON = ai->getSpellIdExact("Flametongue Weapon");
    FROSTBRAND_WEAPON = ai->getSpellIdExact("Frostbrand Weapon");
    ROCKBITER_WEAPON = ai->getSpellIdExact("Rockbiter Weapon");
    EARTHLIVING_WEAPON = ai->getSpellIdExact("Earthliving Weapon");

    WATER_BREATHING = ai->getSpellIdExact("Water Breathing");
    WATER_WALKING = ai->getSpellIdExact("Water Walking");

    //melee
    LAVA_LASH = ai->getSpellIdExact("Lava Lash");
    STORMSTRIKE = ai->getSpellIdExact("Stormstrike");
    SHAMANISTIC_RAGE = ai->getSpellIdExact("Shamanistic Rage");
    FERAL_SPIRIT = ai->getSpellIdExact("Feral Spirit");

    GHOST_WOLF = ai->getSpellIdExact("Ghost Wolf");

    EXHAUSTION = 57723; // heroism debuff
    SATED = 57724; // bloodlust debuff
    //MAELSTROM_WEAPON = 0; // We want the triggered aura, not the talent spell
    uint32 mwtrigger = ai->getSpellIdExact("Maelstrom Weapon",true);
    if (mwtrigger)
    {
        SpellEntry const *mwtSpell = GetSpellStore()->LookupEntry(mwtrigger);
        if (mwtSpell && mwtSpell->EffectTriggerSpell[0] > 0) MAELSTROM_WEAPON = mwtSpell->EffectTriggerSpell[0];
    }

    TALENT_ELEMENTAL = ELEMENTAL_MASTERY;
    TALENT_ENHANCEMENT = STORMSTRIKE;
    TALENT_RESTO = EARTH_SHIELD;

    uint8 talentCounter = 0;
    if (TALENT_ELEMENTAL) talentCounter++;
    if (TALENT_ENHANCEMENT) talentCounter++;
    if (TALENT_RESTO) talentCounter++;
    //if (talentCounter > 1) { TALENT_ELEMENTAL = 0; TALENT_ENHANCEMENT = 0; TALENT_RESTO = 0; } //Unreliable Talent detection.
    #pragma endregion
}
void PlayerbotShamanAI::DoNextCombatManeuver(Unit *pTarget)
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

    switch(ai->GetScenarioType())
    {
        case SCENARIO_DUEL:
            ((ai->GetHealthPercent() < 80 && CastSpell(LESSER_HEAL)) ||
            CastSpell(LIGHTNING_BOLT, pTarget));
            return;
    }

    // Cast CC breakers if any match found (include any dispels first)  does not work yet
    //uint32 ccSpells[4] = { R_ESCAPE_ARTIST, R_EVERY_MAN_FOR_HIMSELF, R_WILL_OF_FORSAKEN, R_STONEFORM };
    //if (ai->GetManaPercent() < 35) { ccSpells[0] = 0; ccSpells[1] = 0; } //We dont have any mana to waste...
    //if (castSelfCCBreakers(ccSpells)) {  } // Most of them don't trigger gcd


    #pragma region Choose Actions
    // Choose actions accoring to talents
    if (m_tank->GetGUID() == m_bot->GetGUID()) { m_role=BOT_ROLE_TANK; } // Hey! I am Main Tank
    else if (TALENT_ENHANCEMENT) { m_role = BOT_ROLE_DPS_MELEE; }
    else if (TALENT_ELEMENTAL) { m_role = BOT_ROLE_DPS_RANGED; }
    else if (TALENT_RESTO) { m_role = BOT_ROLE_SUPPORT; }
    else { m_role = BOT_ROLE_DPS_MELEE; } //Unknown build or low level.. Mainly attack

    // if i am under attack and if i am not tank or offtank: change target if needed
    if (m_tank->GetGUID() != m_bot->GetGUID() && isUnderAttack()  )
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

    // Choose Weapon Enchant
    if (ChangeWeaponEnchants()) return;

	if (TALENT_ELEMENTAL){ if (!m_bot->HasAura(WATER_SHIELD) && CastSpell(WATER_SHIELD,m_bot)) { return; }}
	if (TALENT_ENHANCEMENT){ if (!m_bot->HasAura(LIGHTNING_SHIELD) && CastSpell(LIGHTNING_SHIELD,m_bot)) { return; }}
	if (TALENT_RESTO){ if (!m_bot->HasAura(WATER_SHIELD) && CastSpell(WATER_SHIELD,m_bot)) { return; }}
    // Choose shield
	/*
    if (EARTH_SHIELD && ai->GetHealthPercent() < 80 && isUnderAttack()) { if (CastSpell(EARTH_SHIELD,m_bot)) { return; } }
    else if (WATER_SHIELD && ai->GetManaPercent() < 40) { if (CastSpell(WATER_SHIELD,m_bot)) { return; } }
    else if (LIGHTNING_SHIELD &&
        ( isUnderAttack() || m_tank->GetGUID() == m_bot->GetGUID() )  && !(m_bot->HasAura(WATER_SHIELD) && ai->GetManaPercent() < 80)
        ) { if (CastSpell(LIGHTNING_SHIELD,m_bot)) { return; } }
    else if (CastSpell(WATER_SHIELD,m_bot)) { return; }
	*/
    // If there's a cast stop
    if(m_bot->HasUnitState(UNIT_STAT_CASTING)) return;

    switch(m_role)
    {
        #pragma region BOT_ROLE_TANK / BOT_ROLE_OFFTANK
        case BOT_ROLE_TANK:
        case BOT_ROLE_OFFTANK:
             if (!TALENT_ELEMENTAL && !TALENT_RESTO) { TakePosition(pTarget); }
            else { TakePosition(pTarget,BOT_ROLE_DPS_RANGED); } // mob will come to you sooner or later no need to hurry

            // Do support stuff
            if (!m_bot->isMoving() && ChangeTotems(m_role)) { return; }
            if (ai->GetManaPercent() > 70 && DoSupportRaid(m_bot)) { return; }

            break;
        #pragma endregion

        #pragma region BOT_ROLE_DPS_MELEE
        case BOT_ROLE_DPS_MELEE:
            TakePosition(pTarget);
            // Do support stuff
            if (!m_bot->isMoving() && ChangeTotems(m_role)) { return; }
            if (ai->GetManaPercent() > 40 && DoSupportRaid(m_bot)) { return; }
            break;
        #pragma endregion

        #pragma region BOT_ROLE_DPS_RANGED
        case BOT_ROLE_DPS_RANGED:
            TakePosition(pTarget);
            // Do support stuff
            if (!m_bot->isMoving() && ChangeTotems(m_role)) { return; }
            if (ai->GetManaPercent() > 40 && DoSupportRaid(m_bot)) { return; }
            break;
        #pragma endregion

        #pragma region BOT_ROLE_SUPPORT
        case BOT_ROLE_SUPPORT:
            TakePosition(pTarget);
            // Do support stuff
            if (!m_bot->isMoving() && ChangeTotems(m_role)) { return; }
            if (DoSupportRaid(m_bot)) { return; }
            //heal pets and bots
            Unit *target = DoSelectLowestHpFriendly(40, 1000);
            if(target && target->isAlive() && HealTarget(target, target->GetHealth()*100 / target->GetMaxHealth()) ) { return; }

            break;
        #pragma endregion
    }
    #pragma region ShamanCommon


    //Defensive Stuff
    if (m_tank->GetGUID() != m_bot->GetGUID() && pVictim && pVictim->GetGUID() == m_bot->GetGUID() )
    {
        if (pDist > 5 && CastSpell(FROST_SHOCK, pTarget)) { return; }
        if ((pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_BEAST || pTarget->GetCreatureType() == (uint32) CREATURE_TYPE_HUMANOID) && CastSpell(HEX, pTarget)) { return; } // no gcd
        if (CastSpell(WIND_SHEAR, pTarget)) { } // no gcd
    }
    if (m_bot->getRace() == (uint8) RACE_BLOODELF && pDist < 8 && pTarget->IsNonMeleeSpellCasted(true) && CastSpell(R_ARCANE_TORRENT, pTarget)) { } //no gcd
    if (pTarget->IsNonMeleeSpellCasted(true) && CastSpell(WIND_SHEAR, pTarget)) { } //no gcd
    if (m_bot->getRace() == (uint8) RACE_TAUREN && pVictim && pVictim->GetGUID() == m_bot->GetGUID() && pDist < 8 && CastSpell(R_WAR_STOMP, pTarget)) { return; }

    //Catch
    if (pTarget->HasUnitMovementFlag(UNIT_FLAG_FLEEING))
    {
        if (CastSpell(FROST_SHOCK,pTarget)) return;
    }


    //Buff and restores
    if ( ( (ai->GetHealthPercent() < 60 && isUnderAttack()) ||
        (ai->GetManaPercent() < 30) ) && CastSpell(SHAMANISTIC_RAGE, m_bot)) { return; }
    if (m_bot->getRace() == (uint8) RACE_TROLL && CastSpell(R_BERSERKING,m_bot)) {} // no GCD
    if (m_bot->getRace() == (uint8) RACE_ORC && CastSpell(R_BLOOD_FURY,m_bot)) {} // no GCD
    if (!m_bot->HasAura(HEROISM) && !m_bot->HasAura(EXHAUSTION) && !m_bot->HasAura(SATED) && CastSpell(HEROISM,m_bot)) { return; }
    if (m_role != BOT_ROLE_SUPPORT && CastSpell(NATURES_SWIFTNESS, m_bot)) { } //healers keep it for healing no gcd
    else if (CastSpell(ELEMENTAL_MASTERY, m_bot)) { } //no gcd

    // If at threat limit, use WIND_SHEAR to reduce threat
    if (pThreat > threatThreshold && m_tank->GetGUID() != m_bot->GetGUID() && !isUnderAttack())
    {
        if (m_tank->getVictim() && m_tank->getVictim()->GetGUID() != pTarget->GetGUID()) // I am attacking wrong target!!
        {
            m_bot->SetSelection(m_tank->getVictim()->GetGUID());
            return;
        }
        else
        {
            if (CastSpell(WIND_SHEAR,pTarget)) { return; } //Lets see if we can manage
            else { return; } //use no spells and wait threat to be reduced
        }
    }


	if (TALENT_ELEMENTAL)
	{
		if (CastSpell(ELEMENTAL_MASTERY, m_bot)) { } //no gcd
		if (!pTarget->HasAura(FLAME_SHOCK,m_bot->GetGUID()) && CastSpell(FLAME_SHOCK,pTarget)) { return; }
		if (CastSpell(LAVA_BURST,pTarget)) { return; }
		if (CastSpell(CHAIN_LIGHTNING,pTarget)) { return; }
		if (CastSpell(LIGHTNING_BOLT,pTarget)) { return; }
	}

    //dps
    if (MAELSTROM_WEAPON)
    {
        Aura *maelaura = m_bot->GetAura(MAELSTROM_WEAPON);
        if (maelaura && maelaura->GetStackAmount() == 5)
        {
            if ((isUnderAttack(m_tank,3) || m_tank->GetGUID() == m_bot->GetGUID()) && CastSpell(CHAIN_LIGHTNING,pTarget,true,true)) { return; }
            if (CastSpell(LIGHTNING_BOLT,pTarget,true,true)) { return; }
        }
    }
	if (CastSpell(FLAME_SHOCK,pTarget)) { return; }
    if (CastSpell(STORMSTRIKE,pTarget,true,true)) { return; }

    //if (!TALENT_ENHANCEMENT && CanCast(LAVA_BURST,pTarget,true) && pTarget->HasAura(FLAME_SHOCK,m_bot->GetGUID()) && CastSpell(LAVA_BURST,pTarget,false)) { return; }
	if (CastSpell(FERAL_SPIRIT,m_bot)) { return; }
    if (CanCast(EARTH_SHOCK,pTarget,true) && (pTarget->HasAura(STORMSTRIKE,m_bot->GetGUID()) || pTarget->HasAura(FLAME_SHOCK,m_bot->GetGUID()) ) && CastSpell(EARTH_SHOCK,pTarget)) { return; }
    //if (CanCast(FLAME_SHOCK,pTarget) && CastSpell(FLAME_SHOCK,pTarget)) { return; }
	if (CastSpell(LAVA_LASH,pTarget,true,true)) { return; }
	if (CastSpell(FIRE_NOVA,pTarget)) { return; }
    //if ((isUnderAttack(m_tank,4) || m_tank->GetGUID() == m_bot->GetGUID()) && CastSpell(FIRE_NOVA,pTarget)) { return; }
    if (ai->GetManaPercent() > 60 && castDispel(PURGE,pTarget)) { return; } //PURGE but dont overpurge

    #pragma endregion


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

void PlayerbotShamanAI::DoNonCombatActions()
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

    //Buffs
    if (ChangeWeaponEnchants()) { return; }
    if (CastSpell(WATER_SHIELD,m_bot)) { return; }
    if (CastSpell(EARTH_SHIELD,m_bot)) { return; }

    //mana/hp check
    //Don't bother with eating, if low on hp, just let it heal themself
    if (m_bot->getRace() == (uint8) RACE_UNDEAD_PLAYER && ai->GetHealthPercent() < 75 && CastSpell(R_CANNIBALIZE,m_bot)) { return; }
    if (m_bot->GetHealth() < m_bot->GetMaxHealth() && CastSpell(LESSER_HEAL,m_bot)) { return; }
	if (ai->GetManaPercent() < 50) { ai->Feast(); }

} //end DoNonCombatActions
bool PlayerbotShamanAI::HealTarget(Unit *target, uint8 hp)
{
    if(!target || target->isDead()) return false;
    Player *m_bot = GetPlayerBot();

    if(hp < 30 && m_bot->isInCombat() && CastSpell(NATURES_SWIFTNESS, m_bot)) {} // NO gcd
    if(hp < 60 && CanCast(HEAL,target,true) && m_bot->HasAura(NATURES_SWIFTNESS) && CastSpell(HEAL, target, false)) { return true; }
    if(hp < 30 && CastSpell(LESSER_HEAL,target,true,true)) { return true; }
    if(hp < 40 && m_bot->getRace() == (uint8) RACE_DRAENEI && CastSpell(R_GIFT_OF_NAARU,target)) {} // no GCD
    if(hp < 65 && CanCast(EARTH_SHIELD,target) && !m_bot->HasAura(EARTH_SHIELD,m_bot->GetGUID()) && CastSpell(EARTH_SHIELD,target,false)) { return true; }
    if(hp < 65 && CastSpell(HEAL,target,true,true)) { return true; }
    if(hp < 85 && CastSpell(LESSER_HEAL,target,true,true)) { return true; }

    return false;
} //end HealTarget

bool PlayerbotShamanAI::HealGroup (Unit *target, uint8 hp, uint8 &countNeedHeal)
{
    Player *m_bot = GetPlayerBot();
    if (countNeedHeal < 2) { return false; }
    Unit *rTarget = DoSelectLowestHpFriendly(30,500);
    if (!rTarget || rTarget->isDead() || rTarget->GetHealth() * 100 / rTarget->GetMaxHealth() > 80 ) { return false; }

    if (hp < 65 && RIPTIDE && rTarget->HasAura(RIPTIDE,m_bot->GetGUID()) && CastSpell(CHAIN_HEAL, rTarget)) { return true; }
    if (hp < 85 && CastSpell(RIPTIDE, rTarget)) { return true; }
    if (hp < 75 && CastSpell(CHAIN_HEAL, rTarget,true,true)) { return true; }

    return false;
}

bool PlayerbotShamanAI::CureTarget(Unit *target)
//Cures the target
{
    Player *m_bot = GetPlayerBot();

    if(!target || target->isDead()) { return false; }
    if (castDispel(CLEANSE_SPIRIT, target)) return true;

    return false;
}

bool PlayerbotShamanAI::RezTarget (Unit *target)
{
    if(!target || target->isAlive()) return false;
    Player *m_bot = GetPlayerBot();
    if (target->IsNonMeleeSpellCasted(true)) { return false; } //Already resurrected
    if (m_bot->isInCombat()) { return false; }

    if (!CanCast(ANCESTRAL_SPIRIT,target)) return false;
    std::string msg = "Rezzing ";
    msg += target->GetName();

    GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
    return CastSpell(ANCESTRAL_SPIRIT, target,false);
}

bool PlayerbotShamanAI::BuffPlayer(Unit *target)
{
    //std::string msg = "Mana totem, coming right up.";
    //GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
    //if(!HasAuraName(GetPlayerBot(), "Mana Spring")) { CastSpell(MANA_SPRING_TOTEM, GetPlayerBot()); } return true;
    return false;
}
#pragma region Change Totems
bool PlayerbotShamanAI::ChangeTotems(uint32 mode)
{
    uint32 earth=0, fire=0, water=0, air=0;

    PlayerbotAI *ai = GetAI();
    if(!ai) return false;
    Player *m_bot = GetPlayerBot();
    if(!m_bot || m_bot->isDead()) return false;

    Unit *pTarget = m_bot->GetSelectedUnit();
    Unit *pVictim = NULL;
    if (m_bot->GetSelectedUnit()->IsFriendlyTo(m_bot)) pTarget = NULL;
    if (pTarget) pVictim = pTarget->getVictim();

    //Defaults
    if (!HasAuraName(m_bot,"Horn of Winter") )earth = STRENGTH_OF_EARTH_TOTEM;
    if (!earth) earth = STONESKIN_TOTEM;
    if (!earth) earth = EARTHBIND_TOTEM;
    fire = TOTEM_OF_WRATH;
    if (!fire) fire = FLAMETONGUE_TOTEM;
    if (!fire) fire = SEARING_TOTEM;
    water = MANA_SPRING_TOTEM;
    if (!water) water = HEALING_STREAM_TOTEM;
    if (TALENT_ELEMENTAL || TALENT_RESTO) air = WRATH_OF_AIR_TOTEM;
    else air = WINDFURY_TOTEM;

    //Target reactive stuff
    if (pTarget)
    {
        if (GROUNDING_TOTEM && pTarget->IsNonMeleeSpellCasted(true)) air = GROUNDING_TOTEM;
    }

    if (STONESKIN_TOTEM && isUnderAttack()) earth = STONESKIN_TOTEM;

    uint32 totz[4] = {earth, fire, water, air};

    for (int i = 0; i < 4; i++)
    {
        if (!totz[i]) continue;
        SpellEntry const *tSpell = GetSpellStore()->LookupEntry(totz[i]);
        if (!tSpell) continue;
        uint32 tEntry = (uint32) tSpell->EffectMiscValue[0];
        if (!tEntry) continue;
        CreatureInfo const *totemEntry = GetCreatureTemplateStore(tEntry);
        if (!tEntry) continue;

        if (CanCast(totz[i], m_bot) && !m_bot->FindNearestCreature(tEntry,30)) { return CastSpell(totz[i],m_bot,false); }
    }
    return false;
}
#pragma endregion
#pragma region ChangeWeaponEnchants
bool PlayerbotShamanAI::ChangeWeaponEnchants()
{
    uint32 mhEnch = 0, ohEnch = 0;

    PlayerbotAI *ai = GetAI();
    if(!ai) return false;
    Player *m_bot = GetPlayerBot();
    if(!m_bot || m_bot->isDead()) return false;


    // Choose Weapon Enchant
    if (TALENT_RESTO) { mhEnch = EARTHLIVING_WEAPON; }
    else if (TALENT_ELEMENTAL){ mhEnch = FLAMETONGUE_WEAPON; }
    else
    {
        if (WINDFURY_WEAPON)
        {
            mhEnch = WINDFURY_WEAPON;
            if (m_bot->haveOffhandWeapon())
            {
                if (LAVA_LASH) ohEnch = FLAMETONGUE_WEAPON;
                else ohEnch = WINDFURY_WEAPON;
            }
		}
    else if (FLAMETONGUE_WEAPON)
    {
        mhEnch = FLAMETONGUE_WEAPON;
        if (m_bot->haveOffhandWeapon()) ohEnch = FLAMETONGUE_WEAPON;
    }

    }

    Item* weap;
    uint32 enchant_id = 0;
    SpellEntry const *tSpell;
    bool castedsomething = false;

    if (mhEnch)
    {
        weap = m_bot->GetWeaponForAttack(BASE_ATTACK);
        if (weap)
        {
            tSpell = GetSpellStore()->LookupEntry(mhEnch);
            if (tSpell && tSpell->EffectMiscValue[0] > 0)
            {
                enchant_id = (uint32) tSpell->EffectMiscValue[0];
                if (enchant_id && weap->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) != enchant_id && sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                {
                    m_bot->ApplyEnchantment(weap,TEMP_ENCHANTMENT_SLOT, false); //Remove old enchantment effect
                    weap->SetEnchantment(TEMP_ENCHANTMENT_SLOT, enchant_id, 1800 * 1000, 0); //Add for 30 mins
                    m_bot->ApplyEnchantment(weap, TEMP_ENCHANTMENT_SLOT, true); //Add new effect
                    castedsomething = true;
                }
            }
        }
    }

    if (ohEnch)
    {
        weap = m_bot->GetWeaponForAttack(OFF_ATTACK);
        if (weap)
        {
            tSpell = GetSpellStore()->LookupEntry(ohEnch);
            if (tSpell && tSpell->EffectMiscValue[0] > 0)
            {
                enchant_id = (uint32) tSpell->EffectMiscValue[0];
                if (enchant_id && weap->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) != enchant_id && sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                {
                    m_bot->ApplyEnchantment(weap,TEMP_ENCHANTMENT_SLOT, false); //Remove old enchantment effect
                    weap->SetEnchantment(TEMP_ENCHANTMENT_SLOT, enchant_id, 1800 * 1000, 0); //Add for 30 mins
                    m_bot->ApplyEnchantment(weap, TEMP_ENCHANTMENT_SLOT, true); //Add new effect
                    castedsomething = true;
                }
            }
        }
    }
    return castedsomething;

}
#pragma endregion
