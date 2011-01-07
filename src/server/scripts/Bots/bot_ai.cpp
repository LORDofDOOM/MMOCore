#include "ScriptPCH.h"
#include "bot_ai.h"
#include "Group.h"

bot_ai::bot_ai(Creature *creature): ScriptedAI(creature)
{
    m_creature = creature;
    prevCommandState = COMMAND_FOLLOW; // default
}

bot_ai::~bot_ai(){}

bool bot_ai::CureTarget (Unit *target){ return true; }
bool bot_ai::HealTarget (Unit *target, uint8 hp){ return true; }
void bot_ai::BuffTarget (Unit *target) {}


void bot_ai::BuffAndHealGroup(Player *gPlayer)
{
    if(m_creature->IsNonMeleeSpellCasted(true)) return; // if I'm already casting

    std::list<Unit*> unitList;
    gPlayer->GetRaidMember(unitList,30);
    if(!unitList.empty()){
        for (std::list<Unit*>::iterator itr = unitList.begin() ; itr!=unitList.end();++itr) {
            Player *tPlayer = ((Player *)master)->GetObjPlayer((*itr)->GetGUID());
            if(tPlayer == NULL) continue;
            if(tPlayer->isDead()) continue;
            if(m_creature->GetAreaId() != gPlayer->GetAreaId()) continue;
            //if(tPlayer->GetGUID() == GetPlayerBot()->GetGUID()) continue;
            //if(m_creature->GetDistance(tPlayer) > 30) continue;
            (HealTarget(tPlayer, tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth()));
            BuffTarget(tPlayer);
        }
    }
}

void bot_ai::RezGroup(uint32 REZZ, Player *gPlayer)
{
    if (REZZ==0) return;
    if(m_creature->IsNonMeleeSpellCasted(true)) return; // if I'm already casting

    Group::MemberSlotList const &a =(gPlayer)->GetGroup()->GetMemberSlots();
    for(Group::member_citerator itr = a.begin(); itr != a.end(); itr++)
    {
        Player *tPlayer = ((Player *)master)->GetObjPlayer(itr->guid);
        if(tPlayer == NULL) continue;
        if(tPlayer->isAlive()) continue;
        if(m_creature->GetAreaId() != gPlayer->GetAreaId()) continue;
        if(m_creature->GetDistance(tPlayer) > 30) continue;
        if(tPlayer->IsNonMeleeSpellCasted(true)) continue; //someone rezzing it already
        if(m_creature->IsNonMeleeSpellCasted(true)) continue; // if I'm already casting

        doCast(tPlayer, REZZ);
        return;
    }

}



bool bot_ai::IAmDead()
{
    if(master <= 0 || !master->GetGroup() || ( (master->isInCombat() || m_creature->isInCombat()) && m_creature->isDead() )) return true; //You're DEAD, stop thinking.
    return false;
}

void bot_ai::BotAttackStart(Unit *victim)
{
    AttackStart(victim);
    m_creature->AttackerStateUpdate(victim);
    m_creature->resetAttackTimer();
    if (master->GetBotCommandState() != COMMAND_ATTACK)
        prevCommandState = master->GetBotCommandState();
    master->SetBotCommandState(COMMAND_ATTACK);
}

bool bot_ai::gettingAttacked(AttackerSet m_attackers)
{
    if(!m_attackers.empty())
    {
        for(AttackerSet::iterator iter = m_attackers.begin(); iter != m_attackers.end(); ++iter)
        {
            if(*iter && m_creature->GetDistance((*iter)) < 50 &&
               !master->IsInRaidWith(*iter) &&
               !master->IsInPartyWith(*iter) &&
               //if the thing to attack is a world invisible trigger, ex Glyph in UBRS,
               (*iter)->GetUInt32Value(UNIT_FIELD_DISPLAYID) != 11686)
            {
                BotAttackStart(*iter);
                return true;
            } //end if
        } //end for
    } // end if
    return false;
} //end gettingAttacked


void bot_ai::ResetOrGetNextTarget()
{
    if (master->GetBotMustDie()) return;
    uint64 targetGUID = 0;

    // check if anyone has raid target
    //targetGUID = getTargetWithIcon();
    Group *group = master->GetGroup();
    targetGUID = group->GetTargetWithIconByGroup (m_creature->GetGUID());


    if (targetGUID && targetGUID!=master->GetGUID())
    {

        Unit * target = m_creature->GetCreature(*master, targetGUID);
        if (target && target->isAlive() && target->IsHostileTo(master) && target->isInCombat() /*&& m_creature->IsWithinDist(target, 30)*/)
        {
            BotAttackStart(target);
            return;
        }
    }

    AttackerSet m_attackers = master->getAttackers();

    //check if anyone is attacking master
    if(gettingAttacked(m_attackers)) return;

    //check if anyone is attacking me
    m_attackers = m_creature->getAttackers();
    if(gettingAttacked(m_attackers)) return;

    //check if master has a victim
    if(master->getVictim() && master->getVictim()->IsHostileTo(master))
    {
        if(m_creature->IsWithinDist(m_creature->getVictim(), 50))
        {
            BotAttackStart(master->getVictim());
            return;
        }
    }

    //lastly check a random victim, including bots, pets, etc
    Unit *target = DoSelectLowestHpFriendly(30);
    if(target != NULL && target->isAlive() && !target->IsHostileToPlayers())
    {
        m_attackers = target->getAttackers();
        if(gettingAttacked(m_attackers)) {
            return;
        }

    }

    //if there is no one to attack, make sure we are following master
    if(m_creature->getVictim() == NULL &&
        m_creature->GetCharmInfo()->GetCommandState() != COMMAND_STAY &&
        master->GetDistance(m_creature) > 20 &&
        !master->IsBeingTeleported())
    {
        if (!master->isAlive())
            master->SetBotCommandState(COMMAND_STAY);
        else if (master->GetBotCommandState()==COMMAND_ATTACK)
            master->SetBotCommandState(prevCommandState);


        return;
    }
}

std::string bot_ai::GetSpellName(uint32 spellId)
{
    if (spellId==0) return "";

    int loc = master->GetSession()->GetSessionDbcLocale();
    const SpellEntry *const pSpellInfo = GetSpellStore()->LookupEntry (spellId);
    if (pSpellInfo != NULL) {
        const std::string  name = pSpellInfo->SpellName[loc];
        return name;
    }
    return "";
} // end GetSpellName


bool bot_ai::HasAuraIcon (Unit *unit, uint32 SpellIconID, uint64 casterGuid)
{
    int loc = master->GetSession()->GetSessionDbcLocale();;
    if(unit == NULL) return false;
    Unit *target = unit;
    if(target->isDead()) return false;

    Unit::AuraMap &vAuras = (Unit::AuraMap&)target->GetOwnedAuras();

    //save the map of auras b/c it can crash if an aura goes away while looping
    UNORDERED_MAP<uint64, Aura*> auraMap;
    for(Unit::AuraMap::const_iterator iter = vAuras.begin(); iter!= vAuras.end(); ++iter)
    {
         Aura *aura = iter->second;
        (auraMap)[iter->first] = aura;
    }

    // now search our new map
    for(UNORDERED_MAP<uint64, Aura*>::iterator itr = auraMap.begin(); itr!= auraMap.end(); ++itr)
    {
        const SpellEntry *spellInfo = itr->second->GetSpellProto();
        uint32 spelliconId = spellInfo->SpellIconID;
//sLog->outError ("bot_ai.HasAuraICON: %s has icon %u",spellInfo->SpellName[master->GetSession()->GetSessionDbcLocale()],  spellInfo->SpellIconID);

        if(spelliconId==SpellIconID)
        {
//sLog->outError ("bot_ai.HasAuraICON: %s has icon %u",spellInfo->SpellName[master->GetSession()->GetSessionDbcLocale()],  spellInfo->SpellIconID);
            if(casterGuid == 0){ //don't care who casted it
                return true;
            } else if(casterGuid == itr->second->GetCasterGUID()){ //only if correct caster casted it
                return true;
            }
        }
    }
    return false;
}

bool bot_ai::HasAuraName (Unit *unit, uint32 spellId, uint64 casterGuid)
{
    const SpellEntry *const pSpellInfo = GetSpellStore()->LookupEntry (spellId);

    if(!pSpellInfo) return false;

    int loc = master->GetSession()->GetSessionDbcLocale();
    const std::string  name = pSpellInfo->SpellName[loc];
    if(name.length() == 0) return false;

    return HasAuraName(unit, name, casterGuid);
}


bool bot_ai::HasAuraName (Unit *unit, std::string spell, uint64 casterGuid)
{
    if (spell.length()==0) return false;

    int loc = master->GetSession()->GetSessionDbcLocale();;
    if(unit == NULL) return false;
    Unit *target = unit;
    if(target->isDead()) return false;

    Unit::AuraMap &vAuras = (Unit::AuraMap&)target->GetOwnedAuras();

    //save the map of auras b/c it can crash if an aura goes away while looping
    UNORDERED_MAP<uint64, Aura*> auraMap;
    for(Unit::AuraMap::const_iterator iter = vAuras.begin(); iter!= vAuras.end(); ++iter)
    {
         Aura *aura = iter->second;
        (auraMap)[iter->first] = aura;
    }

    // now search our new map
    for(UNORDERED_MAP<uint64, Aura*>::iterator itr = auraMap.begin(); itr!= auraMap.end(); ++itr)
    {
        const SpellEntry *spellInfo = itr->second->GetSpellProto();
        const std::string name = spellInfo->SpellName[loc];
        if(!spell.compare(name))
        {
            if(casterGuid == 0){ //don't care who casted it
                return true;
            } else if(casterGuid == itr->second->GetCasterGUID()){ //only if correct caster casted it
                return true;
            }
        }
    }

    return false;
}

bool bot_ai::listAuras (Unit *unit)
{
    int loc = 0;
    Unit *target = unit;
    sLog->outError ("ListAuras for %s", unit->GetName());
    Unit::AuraMap &vAuras = (Unit::AuraMap&)target->GetOwnedAuras();
    for(Unit::AuraMap::const_iterator itr = vAuras.begin(); itr!=vAuras.end(); ++itr)
    {
        SpellEntry const *spellInfo = itr->second->GetSpellProto();
        const std::string name = spellInfo->SpellName[loc];
        sLog->outError("aura = %u %s", spellInfo->Id, name.c_str());
    }
    return false;
}

void bot_ai::doCast(Unit *victim, uint32 spellId, bool triggered)
{
    if(spellId == 0) return;
    if (!isTimerReady(GC_Timer)) return;
    if (m_creature->IsNonMeleeSpellCasted(true)) return;

    m_creature->SetStandState(UNIT_STAND_STATE_STAND);
    GC_Timer = 20;
    DoCast(victim, spellId, triggered);
} //end doCast


bool bot_ai::isTimerReady (int32 timer)
{
    if(timer <= 0 && GC_Timer <= 0) return true;
    else                            return false;
} //end isTimerReady


//Since a lot of spell's mana requirement is calculated
//from the base mana, it will be wrong for NPCs because base mana
//is also total mana.  So it uses up too much mana.  So just
//arbitrary give 25% mana back.
//TODO: calculate the correct cost based on the spell used
void bot_ai::GiveManaBack(uint8 amount)
{
    uint32 maxPower =  m_creature->GetMaxPower(POWER_MANA);
    uint32 x =  m_creature->GetPower(POWER_MANA) + maxPower*amount/100;
    m_creature->SetPower(POWER_MANA, x>maxPower ? maxPower : x);
}

void bot_ai::CureGroup (Unit *pTarget)
{
    Group::MemberSlotList const &a =((Player*)pTarget)->GetGroup()->GetMemberSlots();
    for(Group::member_citerator itr = a.begin(); itr != a.end(); itr++)
    {
        Player *tPlayer = ((Player *)master)->GetObjPlayer(itr->guid);
        if(tPlayer == NULL) continue;
        if(tPlayer->isDead()) continue;
        if(m_creature->GetDistance(tPlayer) > 25) continue;
        (CureTarget(tPlayer));
    }

}

void bot_ai::Feast()
{
    uint8 myClass = m_creature->getClass();

    //if low on mana, take a drink (only check for classes with custom AI)
    //because they are the only ones currently using mana
    if(myClass == CLASS_SHAMAN || myClass == CLASS_DRUID ||
        myClass == CLASS_PRIEST || myClass == CLASS_MAGE ||
        myClass == CLASS_WARLOCK || myClass == CLASS_PALADIN)
    {
        if(m_creature->GetPower(POWER_MANA)*100/m_creature->GetMaxPower(POWER_MANA) < 80 && !m_creature->HasAura(1137) && master->GetBotMustWaitForSpell3() <= 0 && !m_creature->isInCombat())
        {
            m_creature->CastSpell(m_creature, 1137, true);
            master->SetBotMustWaitForSpell3(1000);
            m_creature->SetStandState(1);
//            m_creatureTimer = 5000; //set longer delay so it wont stand up right away
            return;
        }
    }
    //if drinking, have to fake mana regen because charmed NPCs
    //do not regen mana
    if(m_creature->HasAura(1137))
    {
        uint32 addvalue = 0;
        uint32 maxValue = m_creature->GetMaxPower(POWER_MANA);
        uint32 curValue = m_creature->GetPower(POWER_MANA);

        if(curValue <= maxValue)
        {
            addvalue = maxValue/20;
            m_creature->ModifyPower(POWER_MANA, addvalue);
            //return;
        }
    }
    if(m_creature->HasAura(1137) && m_creature->GetPower(POWER_MANA) >= m_creature->GetMaxPower(POWER_MANA))
        m_creature->RemoveAurasDueToSpell(1137);

    //eat
    if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 80 && !m_creature->HasAura(10256) && master->GetBotMustWaitForSpell3() <= 0 && !m_creature->isInCombat())
    {
        master->SetBotMustWaitForSpell3(1000);
        m_creature->CastSpell(m_creature, 10256, true);
        m_creature->SetStandState(1);
//        m_creatureTimer = 5000; //set longer delay so it wont stand up right away
        return;
    }

    //if eating, have to fake regen because charmed NPCs
    //do not regen
    if(m_creature->HasAura(10256))
    {
        uint32 addvalue = 0;
        uint32 maxValue = m_creature->GetMaxHealth();
        uint32 curValue = m_creature->GetHealth();

        if(curValue <= maxValue)
        {
            addvalue = maxValue/20;
            m_creature->SetHealth(curValue + addvalue);
            //return;
        }
    }

    if(m_creature->GetHealth() == m_creature->GetMaxHealth() && m_creature->HasAura(10256))
        m_creature->RemoveAurasDueToSpell(10256);
} // end Feast

void bot_ai::setStats(uint32 myclass, uint32 myrace, uint32 mylevel) {
    if (myrace==0) return;

    //sLog->outError ("bot_ai::setStats");
    //DATABASE STATS
    PlayerLevelInfo info;
    master->GetBotLevelInfo(myrace,myclass,mylevel,&info);

    PlayerClassLevelInfo classInfo;
    // master->GetBotClassLevelInfo(m_creature->getClass(),m_creature->getLevel(),&classInfo);

    m_creature->SetLevel(mylevel);
    UnitMods unitMod = UNIT_MOD_ATTACK_POWER;


    //player -> m_creature->UpdateAllStats
    for (int i = STAT_STRENGTH; i < MAX_STATS; i++)
    {
        //float value = m_creature->GetTotalStatValue(Stats(i));
        //m_creature->SetStat(Stats(i), (int32)value);
        m_creature->SetStat(Stats(i), info.stats[i]);
    }

    m_creature->UpdateAttackPowerAndDamage();
    float val2 = 0.0f;
    float level = float(mylevel);


    uint16 index = UNIT_FIELD_ATTACK_POWER;
    uint16 index_mod = UNIT_FIELD_ATTACK_POWER_MODS;
    uint16 index_mult = UNIT_FIELD_ATTACK_POWER_MULTIPLIER;

    float mLevelMult = 0.0;
    if (level>=40) mLevelMult = 1.5;
    if (level>=50) mLevelMult = 2.0;
    if (level>=60) mLevelMult = 3.0;
    if (level>=70) mLevelMult = 4.0;
    if (level>=80) mLevelMult = 5.0;

    switch(myclass)
    {
    case CLASS_WARRIOR:       val2 = level * mLevelMult*4.0f + m_creature->GetStat(STAT_STRENGTH)*2.0f                   ; break;
    case CLASS_DEATH_KNIGHT:  val2 = level * mLevelMult*4.0f + m_creature->GetStat(STAT_STRENGTH)*2.0f                   ; break;
    case CLASS_PALADIN:       val2 = level * mLevelMult*3.0f + m_creature->GetStat(STAT_STRENGTH)*2.0f                    - 20.0f; break;
    case CLASS_ROGUE:         val2 = level * mLevelMult*6.0f + m_creature->GetStat(STAT_STRENGTH) + m_creature->GetStat(STAT_AGILITY); break;
    case CLASS_HUNTER:        val2 = level * mLevelMult*2.0f + m_creature->GetStat(STAT_STRENGTH) + m_creature->GetStat(STAT_AGILITY) - 20.0f; break;
    case CLASS_SHAMAN:        val2 = level * mLevelMult*2.0f + m_creature->GetStat(STAT_STRENGTH)*2.0f                    - 20.0f; break;
    case CLASS_DRUID:         val2 = m_creature->GetStat(STAT_STRENGTH)*2.0f - 20.0f; break;
    case CLASS_MAGE:          val2 =              m_creature->GetStat(STAT_STRENGTH)  - 10.0f; break;
    case CLASS_PRIEST:        val2 =              m_creature->GetStat(STAT_STRENGTH)  - 10.0f; break;
    case CLASS_WARLOCK:       val2 =              m_creature->GetStat(STAT_STRENGTH)  - 10.0f; break;
    }

//sLog->outError ("val2 = %f", val2);
//sLog->outError ("str = %f", m_creature->GetStat(STAT_STRENGTH));
    //CreatureInfo const *cinfo = sObjectMgr.GetCreatureTemplate(m_creature->GetEntry());
    //m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, cinfo->maxdmg/mylevel);

    //Custom weapon system
    val2 = abs(val2 + (val2 * (level/80)));
   // sLog->outError ("custom val2 = %f", val2);
    m_creature->SetModifierValue(unitMod, BASE_VALUE, val2);
    float base_attPower  = m_creature->GetModifierValue(unitMod, BASE_VALUE) * m_creature->GetModifierValue(unitMod, BASE_PCT) * mLevelMult;

    float attPowerMod = m_creature->GetModifierValue(unitMod, TOTAL_VALUE);

    float attPowerMultiplier = m_creature->GetModifierValue(unitMod, TOTAL_PCT) - 1.0f;
    attPowerMod = attPowerMod ?  attPowerMod : 1;
    attPowerMultiplier = attPowerMultiplier ?  attPowerMultiplier : 0.1f;

    m_creature->SetUInt32Value(UNIT_FIELD_ATTACK_POWER, (uint32)base_attPower);           //UNIT_FIELD_(RANGED)_ATTACK_POWER field
    m_creature->SetUInt32Value(UNIT_FIELD_ATTACK_POWER_MODS, (uint32)attPowerMod);         //UNIT_FIELD_(RANGED)_ATTACK_POWER_MODS field
    m_creature->SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, attPowerMultiplier);          //UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field

    m_creature->UpdateDamagePhysical(BASE_ATTACK);

    //sLog->outError ("udpating BASE ATTACK");
    //sLog->outError ("mLevelMult = %f", mLevelMult);
    //sLog->outError ("bot_ai - base_attPower = %f", base_attPower);
    //sLog->outError ("actpower multiplier = %f", attPowerMultiplier);
    //sLog->outError ("actpower val2 = %f", val2);
    //sLog->outError ("\tmin damage = %f", uint32(val2 * attPowerMultiplier)+level);
    //sLog->outError ("\tmax damange = %f",uint32(val2 * attPowerMultiplier)*2+level);
    //sLog->outError ("\tmax damange 2 = %f",uint32(val2 * attPowerMultiplier)*mLevelMult);
    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, uint32(val2 * attPowerMultiplier)+level);
    m_creature->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, uint32(val2 * attPowerMultiplier)*2+level);

    m_creature->UpdateDamagePhysical(BASE_ATTACK);
    m_creature->SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,uint32(val2 * attPowerMultiplier)+level);
    m_creature->SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,uint32(val2 * attPowerMultiplier)*2+level);
//sLog->outError ("\t%s - min damage after setting it = %f", m_creature->GetName(), m_creature->GetFloatValue(UNIT_FIELD_MINDAMAGE));
//sLog->outError ("\t%s - MINDAMAGE after setting it = %f", m_creature->GetName(), m_creature->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE));


    m_creature->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, uint32(val2 * attPowerMultiplier));
    //player -> m_creature->UpdateAttackPowerAndDamage(true); // ---> STUPID CREATURE CAN NOT BE RANGED


    float value = 0.0f;
    unitMod = UNIT_MOD_ARMOR;

    value  = m_creature->GetModifierValue(unitMod, BASE_VALUE);         // base armor (from items)
    value *= m_creature->GetModifierValue(unitMod, BASE_PCT);           // armor percent from items
    value += m_creature->GetStat(STAT_AGILITY) * 2.0f;                  // armor bonus from stats
    value += m_creature->GetModifierValue(unitMod, TOTAL_VALUE);

    value *= m_creature->GetModifierValue(unitMod, TOTAL_PCT);
    m_creature->UpdateAttackPowerAndDamage();

    //Custom armor system
    value = value + (value * (level/100));

   // m_creature->SetArmor(int32(value));
    m_creature->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, value);
    //m_creature->UpdateArmor();

}

void bot_ai::ReceiveEmote(Player *player, uint32 emote)
{
    if (emote == TEXTEMOTE_DANCE)
        DoSay("I love to dance!",LANG_UNIVERSAL,m_creature->GetGUID(), player);

    // debug to see what auras are on bot
    if (emote == TEXTEMOTE_BONK)
        listAuras(m_creature);

    // debug to see what auras are on me
    if (emote == TEXTEMOTE_SALUTE)
        listAuras(player);

    if (emote == TEXTEMOTE_STAND)
    {
        if (m_creature->GetCharmerGUID() != player->GetGUID())
        {
            m_creature->HandleEmoteCommand(TEXTEMOTE_RUDE);
            return;
        }
        player->SetBotCommandState (COMMAND_STAY);
        DoSay("Standing Still.",LANG_UNIVERSAL,m_creature->GetGUID(), player);
    }

    if (emote == TEXTEMOTE_WAVE)
    {
        if (m_creature->GetCharmerGUID() != player->GetGUID())
        {
            m_creature->HandleEmoteCommand(TEXTEMOTE_RUDE);
            return;
        }
        player->SetBotCommandState (COMMAND_FOLLOW);
        DoSay("Following!",LANG_UNIVERSAL,m_creature->GetGUID(), player);
    }

    // buff the requester
    if (emote == TEXTEMOTE_BOW)
    {
        ReceiveBowEmote(player);
    }

} // end ReceiveEmote

void bot_ai::ReceiveBowEmote(Player *player)
{
       // DoSay("I have no buffs for you!",LANG_UNIVERSAL,m_creature->GetGUID(), player);
}

void bot_ai::DoSay(const std::string& text, const uint32 language,uint64 receiver,Player *player)
{

}
