/*
Name :    PlayerbotClassAI.cpp
Notes:    Does not really work with peldor's own classbot AIs
        Contains many improvements and hacks to overcome some difficulites
Known
Problems:    - Contains hardcoded values,  for an example check group heal, individual heal decision
            - ai->getSpellIdExact func, *although works more accurately* is probably slower and hackish
            - FindMainTankRaid func, includes a db query making it a resource hog
            - canCast func, does not check for every possible problem, can cause AI stuck.. Should be inside PlayerbotAI class
            - castSpell func is redundant and should be placed in PlayerbotAI class, sets private variable m_ai->m_CurrentlyCastingSpellId which is made public as a hack..

Authors : SwaLLoweD
Version : 0.40
*/

#include "PlayerbotClassAI.h"
#include "Common.h"
#include "Spell.h"
#include "Group.h"

PlayerbotClassAI::PlayerbotClassAI(Player *const master, Player *const bot, PlayerbotAI *const ai): m_master(master), m_bot(bot), m_ai(ai), rezSpamTimer(0)
{
    threatThreshold = 75;            // Threat % threshold for dps to lower tps
    offensiveSpellThreshold = 70;    // Mana % threshold for healers to use offensive spells

    // first aid
    RECENTLY_BANDAGED = 11196; // first aid check

    // RACIALS
    R_ARCANE_TORRENT = ai->getSpellIdExact("Arcane Torrent");
    R_BERSERKING = ai->getSpellIdExact("Berserking");
    R_BLOOD_FURY = ai->getSpellIdExact("Blood Fury");
    R_CANNIBALIZE = ai->getSpellIdExact("Cannibalize");
    R_ESCAPE_ARTIST = ai->getSpellIdExact("Escape Artist");
    R_EVERY_MAN_FOR_HIMSELF = ai->getSpellIdExact("Every Man for Himself");
    R_GIFT_OF_NAARU = ai->getSpellIdExact("Gift of the Naaru");
    R_SHADOWMELD = ai->getSpellIdExact("Shadowmeld");
    R_STONEFORM = ai->getSpellIdExact("Stoneform");
    R_WAR_STOMP = ai->getSpellIdExact("War Stomp");
    R_WILL_OF_FORSAKEN = ai->getSpellIdExact("Will of the Forsaken");

    mainTank = NULL;
    m_pulling = false;
}
PlayerbotClassAI::~PlayerbotClassAI(){}

void PlayerbotClassAI::DoNextCombatManeuver(Unit *){}

void PlayerbotClassAI::DoNonCombatActions(){}

void PlayerbotClassAI::LoadSpells(){}

void PlayerbotClassAI::Pull(){}

bool PlayerbotClassAI::BuffPlayer(Unit *target){ return false; }

bool PlayerbotClassAI::FindMount(){ return true; }

bool PlayerbotClassAI::Unmount(){ return true; }

bool PlayerbotClassAI::HealTarget (Unit *target, uint8 hp){ return false; }

bool PlayerbotClassAI::HealGroup (Unit *target, uint8 hp, uint8 &countNeedHeal){ return false; }

bool PlayerbotClassAI::CureTarget (Unit *target){ return false; }

bool PlayerbotClassAI::RezTarget (Unit *target){ return false; }

bool PlayerbotClassAI::IsMounted(){ return m_bot->IsMounted(); }

bool PlayerbotClassAI::CastSpell(uint32 spellId, Unit *target, bool checkFirst, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck, bool triggered)
{return m_ai->CastSpell(spellId, target, checkFirst, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck, triggered); }
bool PlayerbotClassAI::CastSpell(const SpellEntry * pSpellInfo, Unit *target, bool checkFirst, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck, bool triggered)
{return m_ai->CastSpell(pSpellInfo, target, checkFirst, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck, triggered);}

bool PlayerbotClassAI::CanCast(uint32 spellId, Unit *target, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck)
{return m_ai->CanCast(spellId, target, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck);}

bool PlayerbotClassAI::CanCast(const SpellEntry * pSpellInfo, Unit *target, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck)
{return m_ai->CanCast(pSpellInfo, target, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck);}

bool PlayerbotClassAI::listAuras(Unit *u)
{
    int loc = 0;
    Unit *target = u;
    typedef std::pair<uint32, uint8> spellEffectPair;
    typedef std::multimap< spellEffectPair, Aura*> AuraMap;
    Unit::AuraMap &vAuras = target->GetOwnedAuras();
    for(Unit::AuraMap::const_iterator itr = vAuras.begin(); itr!=vAuras.end(); ++itr)
    {
        //SpellEntry const *spellInfo = (*itr).second->GetSpellProto();
        const SpellEntry *spellInfo = itr->second->GetSpellProto();
        const std::string name = spellInfo->SpellName[loc];
        sLog->outDebug("aura = %u %s", spellInfo->Id, name.c_str());
    }
    return true;
};//end listAuras

bool PlayerbotClassAI::HasAuraName (Unit *unit, uint32 spellId, uint64 casterGuid)
{
    const SpellEntry *const pSpellInfo = GetSpellStore()->LookupEntry (spellId);
    if(!pSpellInfo) return false;
    int loc = m_bot->GetSession()->GetSessionDbcLocale();
    const std::string  name = pSpellInfo->SpellName[loc];
    if(name.length() == 0) return false;
    return HasAuraName(unit, name, casterGuid);
}

bool PlayerbotClassAI::HasAuraName (Unit *target, std::string spell, uint64 casterGuid)
{
    int loc = m_bot->GetSession()->GetSessionDbcLocale();
    typedef std::pair<uint32, uint8>spellEffectPair;
    typedef std::multimap<spellEffectPair, Aura*>AuraMap;

    Unit::AuraMap &vAuras = target->GetOwnedAuras();
    for(Unit::AuraMap::const_iterator itr = vAuras.begin(); itr!=vAuras.end(); ++itr)
    {
        //SpellEntry const *spellInfo = (*itr).second->GetSpellProto();
        const SpellEntry *spellInfo = itr->second->GetSpellProto();
        const std::string name = spellInfo->SpellName[loc];
        if(!spell.compare(name))
        //if(!strcmp(name.c_str(),spell.c_str()))
        {
            if(casterGuid == 0) //don't care who casted it
                return true;
            else if(casterGuid == itr->second->GetCasterGUID()) //only if correct caster casted it
                return true;
        }
    }
    return false;
};

bool PlayerbotClassAI::castDispel (uint32 dispelSpell, Unit *dTarget, bool checkFirst, bool castExistingAura, bool skipFriendlyCheck, bool skipEquipStanceCheck)
{
    if (dispelSpell == 0 || !dTarget ) return false;
    //if (!canCast(dispelSpell, dTarget, true)) return false; //Needless cpu cycles wasted, usually a playerbot can cast a dispell
    const SpellEntry *dSpell = GetSpellStore()->LookupEntry(dispelSpell);
    if (!dSpell) return false;

    for (uint8 i = 0 ; i < MAX_SPELL_EFFECTS ; ++i)
    {
        if (dSpell->Effect[i] != (uint32)SPELL_EFFECT_DISPEL) continue;
        uint32 dispel_type = dSpell->EffectMiscValue[i];
        uint32 dispelMask  = GetDispellMask(DispelType(dispel_type));
        Unit::AuraMap const& auras = dTarget->GetOwnedAuras();
        for (Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); itr++)
        {
            Aura * aura = itr->second;
            AuraApplication * aurApp = aura->GetApplicationOfTarget(dTarget->GetGUID());
            if (!aurApp)
                continue;

            if ((1<<aura->GetSpellProto()->Dispel) & dispelMask)
            {
                if(aura->GetSpellProto()->Dispel == DISPEL_MAGIC)
                {
                    bool positive = aurApp->IsPositive() ? (!(aura->GetSpellProto()->AttributesEx & SPELL_ATTR0_UNK7)) : false;

                    // do not remove positive auras if friendly target
                    //               negative auras if non-friendly target
                    if(positive == dTarget->IsFriendlyTo(GetPlayerBot()))
                        continue;
                }
                // If there is a successfull match return, else continue searching.
                if (CastSpell(dSpell, dTarget, checkFirst, castExistingAura, skipFriendlyCheck, skipEquipStanceCheck)) { return true; }
            }
        }
    }
    return false;
}

bool PlayerbotClassAI::castSelfCCBreakers (uint32 castList[])
{
    uint32 dispelSpell = 0;
    Player *dTarget = GetPlayerBot();


            /* dispelSpell = (uint32) R_ESCAPE_ARTIST; // this is script effect,
            Unit::AuraMap const& auras = dTarget->GetOwnedAuras();
            for (Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); itr++)
            {
                Aura * aura = itr->second;
                AuraApplication * aurApp = aura->GetApplicationOfTarget(dTarget->GetGUID());
                if (!aurApp)
                    continue;

                if ( ( aura->GetSpellProto()->Mechanic == MECHANIC_SNARE ) || ( aura->GetSpellProto()->Mechanic == MECHANIC_ROOT ) )
                {
                    if(aura->GetSpellProto()->Dispel == DISPEL_MAGIC)
                    {
                        bool positive = aurApp->IsPositive() ? (!(aura->GetSpellProto()->AttributesEx & SPELL_ATTR0_UNK7)) : false;

                        // do not remove positive auras if friendly target
                        //               negative auras if non-friendly target
                        if(positive == dTarget->IsFriendlyTo(caster))
                            continue;
                    }
                    return castSpell(dispelSpell, dTarget);
                }
            }
            return false;  */

        // racial abilities
    /*  if( GetPlayerBot()->getRace() == RACE_BLOODELF && !pTarget->HasAura( ARCANE_TORRENT,0 ) && castSpell( ARCANE_TORRENT,pTarget ) ) {
         //GetPlayerBot()->Say("Arcane Torrent!", LANG_UNIVERSAL);
    } else if( GetPlayerBot()->getRace() == RACE_HUMAN && (GetPlayerBot()->HasUnitState( UNIT_STAT_STUNNED ) || GetPlayerBot()->HasAuraType( SPELL_AURA_MOD_FEAR ) || GetPlayerBot()->HasAuraType( SPELL_AURA_MOD_DECREASE_SPEED ) || GetPlayerBot()->HasAuraType( SPELL_AURA_MOD_CHARM )) && castSpell( EVERY_MAN_FOR_HIMSELF, GetPlayerBot() ) ) {
        //GetPlayerBot()->Say("EVERY MAN FOR HIMSELF!", LANG_UNIVERSAL);
    } else if( GetPlayerBot()->getRace() == RACE_UNDEAD_PLAYER && (GetPlayerBot()->HasAuraType( SPELL_AURA_MOD_FEAR ) || GetPlayerBot()->HasAuraType( SPELL_AURA_MOD_CHARM )) && castSpell( WILL_OF_THE_FORSAKEN, GetPlayerBot() ) ) {
       // GetPlayerBot()->Say("WILL OF THE FORSAKEN!", LANG_UNIVERSAL);
    } else if( GetPlayerBot()->getRace() == RACE_DWARF && GetPlayerBot()->HasAuraState( AURA_STATE_DEADLY_POISON ) && castSpell( STONEFORM, GetPlayerBot() ) ) {
        //GetPlayerBot()->Say("STONEFORM!", LANG_UNIVERSAL);
    } else if( GetPlayerBot()->getRace() == RACE_GNOME && (GetPlayerBot()->HasUnitState( UNIT_STAT_STUNNED ) || GetPlayerBot()->HasAuraType( SPELL_AURA_MOD_DECREASE_SPEED )) && castSpell( ESCAPE_ARTIST, GetPlayerBot() ) ) {
       // GetPlayerBot()->Say("ESCAPE ARTIST!", LANG_UNIVERSAL);
    } */

    for (uint8 j = 0; j <  sizeof (castList); j++)
    {
        dispelSpell = castList[j];
        if (dispelSpell == 0 || !dTarget->HasSpell(dispelSpell) || !CanCast(dispelSpell, dTarget, true)) continue;
        SpellEntry const *dSpell = GetSpellStore()->LookupEntry(dispelSpell);
        if (!dSpell) continue;

        for (uint8 i = 0 ; i < MAX_SPELL_EFFECTS ; ++i)
        {
            if (dSpell->Effect[i] != (uint32)SPELL_EFFECT_DISPEL && dSpell->Effect[i] != (uint32)SPELL_EFFECT_APPLY_AURA) continue;
            if (dSpell->Effect[i] == (uint32)SPELL_EFFECT_APPLY_AURA && (
                (dSpell->EffectApplyAuraName[i] != (uint32) SPELL_AURA_MECHANIC_IMMUNITY) ||
                (dSpell->EffectApplyAuraName[i] != (uint32) SPELL_AURA_DISPEL_IMMUNITY)
                )) continue;

            Unit::AuraMap const& auras = dTarget->GetOwnedAuras();
            for (Unit::AuraMap::const_iterator itr = auras.begin(); itr != auras.end(); itr++)
            {
                Aura * aura = itr->second;
                AuraApplication * aurApp = aura->GetApplicationOfTarget(dTarget->GetGUID());
                if (!aurApp) continue;

                if (aura->GetSpellProto() && (
                    (dSpell->Effect[i] == (uint32)SPELL_EFFECT_DISPEL  && ((1<<aura->GetSpellProto()->Dispel) & GetDispellMask(DispelType(dSpell->EffectMiscValue[i]))) )
                    || (dSpell->EffectApplyAuraName[i] == (uint32) SPELL_AURA_MECHANIC_IMMUNITY && ( GetAllSpellMechanicMask(aura->GetSpellProto()) & ( 1 << dSpell->EffectMiscValue[i]) ) )
                    || (dSpell->EffectApplyAuraName[i] == (uint32) SPELL_AURA_DISPEL_IMMUNITY && ( (1<<aura->GetSpellProto()->Dispel) & GetDispellMask(DispelType(dSpell->EffectMiscValue[i])) ) )
                    ) )
                {
                    if(aura->GetSpellProto()->Dispel == DISPEL_MAGIC)
                    {
                        bool positive = aurApp->IsPositive() ? (!(aura->GetSpellProto()->AttributesEx & SPELL_ATTR0_UNK7)) : false;
                        if(positive)continue;
                    }
                    return CastSpell(dispelSpell, dTarget, false);
                }
            }
        }
    }
    return false;
}

bool PlayerbotClassAI::DoSupportRaid(Player *gPlayer, float radius, bool dResurrect, bool dGroupHeal, bool dHeal, bool dCure, bool dBuff)
{
    bool needHeal = false;
    if (dGroupHeal || dHeal)
    {
        uint8 cntNeedHeal = 0;
        uint8 raidHPPercent = GetHealthPercentRaid(gPlayer, cntNeedHeal);
        if (dGroupHeal && raidHPPercent <=90 && cntNeedHeal > 1)
        {
            if (HealGroup(gPlayer, raidHPPercent, cntNeedHeal)) return true;
        }
        if (raidHPPercent < 60 ) needHeal = true;
    }
    //std::list<Unit*> unitList;
    //gPlayer->GetRaidMember(unitList,30);
    Group *pGroup = gPlayer->GetGroup();
    if (!pGroup) return false;
    for (GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Unit* tPlayer = itr->getSource();
        if(!tPlayer || gPlayer->IsHostileTo(tPlayer)) continue;
        if(GetPlayerBot()->GetAreaId() != tPlayer->GetAreaId()) continue;
        if(!m_bot->IsWithinDistInMap(tPlayer, radius)) { continue; }
        if(tPlayer->isDead()) // May be we can rez
        {
            if(!dResurrect) continue;
            if(needHeal) continue; //First heal others needing heal
            if(tPlayer->GetGUID() == GetPlayerBot()->GetGUID()) continue;
            if(tPlayer->IsNonMeleeSpellCasted(true)) continue; //Already rez
            if(RezTarget(tPlayer)) { return true; }
            else continue;
        }
        if (dHeal)
        {
            uint8 tarHPPercent = tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth();
            if (tarHPPercent < 100 && HealTarget(tPlayer, tarHPPercent)) return true;
        }
        if (needHeal && dHeal) continue; //First heal others needing heal
        if (dCure && CureTarget(tPlayer)) return true;
        if (dBuff && BuffPlayer(tPlayer)) return true;
    }
    return false;
}

bool PlayerbotClassAI::TakePosition(Unit *followTarget, BotRole bRole, float bDist, float bMinDist, float bMaxDist, float bAngle, Unit *faceTarget)
{
    bool doFollow = true;
    bool omitAngle = false;
    bool angleIsAutoSet = false;
    if (!bAngle) angleIsAutoSet = true;
    if (bAngle < 0) bAngle += 2 * M_PI;
    //if (bAngle > 2 * M_PI) bAngle -= 2 * M_PI; //Do not send values higher than 2 PI, lower than -2 PI
    bool rval = false;
    if (followTarget == NULL) { followTarget = GetMaster(); if (followTarget == NULL) { return false; } }
    if (faceTarget == NULL) { faceTarget = followTarget; }
    if (bRole == BOT_ROLE_NONE) { bRole = ( (m_role == BOT_ROLE_NONE) ? BOT_ROLE_DPS_MELEE : m_role);  }
    //Default values
    Unit *pVictim = followTarget->getVictim();
    if (pVictim && pVictim->GetGUID() == m_bot->GetGUID()) //if target is attacking me
    {
        if (bRole == BOT_ROLE_TANK || bRole == BOT_ROLE_OFFTANK || bRole == BOT_ROLE_DPS_MELEE)
        {
            //Move to target
            if (!bDist || bDist > 0.7f) bDist = 0.7f;
            if (bMinDist < 0 || bMinDist > 1) bMinDist = 0;
            if (bMaxDist <= 0 || bMaxDist > MELEE_RANGE) bMaxDist = MELEE_RANGE;
            bAngle = 0;
        }
        else {doFollow = false;} //Do not move, creature will come
    }
    else
    {
        // calculating distance to follow
        switch (bRole)
        {
            case BOT_ROLE_TANK:
            case BOT_ROLE_OFFTANK:
                if (!bDist) { bDist = 0.7f; bMinDist = 0; bMaxDist = MELEE_RANGE; bAngle = 0;}
                break;
            case BOT_ROLE_HEALER:
            case BOT_ROLE_SUPPORT:
                if (!bDist) { bDist = urand(12, 14); bMinDist = 10; bMaxDist = 18; bAngle = ((urand(0,1) * 90 ) + urand(110,160)) * M_PI / 180; }
                break;
            case BOT_ROLE_DPS_RANGED:
                if (!bDist) { bDist = urand(18, 24); bMinDist = 10;  bMaxDist = 26; bAngle = ((urand(0,1) * 90 ) + urand(110,160)) * M_PI / 180; }
                break;
            default:
                if (!bDist) { bDist = 0.7f; bMinDist = 0.1f; bMaxDist = MELEE_RANGE; bAngle = ((urand(0,1) * 90 ) + urand(110,160)) * M_PI / 180; }
                break;
        }
    }
    //Do not try to go behind if ranged and creature is not boss like
    if (bDist > MELEE_RANGE && followTarget->GetTypeId() != TYPEID_PLAYER)
    {
        const CreatureInfo *cInfo = ((Creature*) followTarget)->GetCreatureInfo();
        if (!cInfo || cInfo->rank != 3) { omitAngle = true; }
    }

    //Move
    if (doFollow)
    {
        float curDist = m_bot->GetDistance(followTarget);
        if (m_pulling ||
            (!m_bot->isMoving() &&
            ((curDist > bMaxDist || curDist < bMinDist)  //Outside range boundries
            || (!omitAngle && ((!followTarget->HasInArc(M_PI,m_bot)) ^ (bAngle > 0.5f * M_PI && bAngle < 1.5f * M_PI)))) )//is at right position front/behind?
            )
        {
            //m_bot->GetMotionMaster()->Clear();
            //sLog->outError("Bot[%u] is moving, curDist[%f], bDist[%f], bminDist[%f], bMaxDist[%f], bAngle[%f], InFront[%u]", m_bot->GetGUIDLow(), curDist, bDist,bMinDist, bMaxDist, bAngle, followTarget->HasInArc(M_PI,m_bot));
            if (angleIsAutoSet && omitAngle) { m_bot->GetMotionMaster()->MoveChase(followTarget, bDist); }
            else { m_bot->GetMotionMaster()->MoveChase(followTarget, bDist, bAngle); }
            rval |= true;
        }
    }
    //Face your faceTarget
    if (!m_bot->HasInArc(M_PI/16, faceTarget) && !m_bot->isMoving() ) { m_bot->SetFacingToObject(faceTarget); rval |= true; }
    return rval;
}

uint8 PlayerbotClassAI::GetThreatPercent(Unit *pTarget, Unit *pFrom)
{
    uint8 tPercent = 0;
    Unit *pVictim = pTarget->getVictim();
    if (!pVictim) return 100; //Not Attacking anyone yet, somehow..
    if (!pFrom) { pFrom = m_bot; }
    if (pVictim->GetGUID() == pFrom->GetGUID()) return 100; //I'm already being attacked, too late for alert, kill it..
    //if (m_tank->GetGUID() == m_bot->GetGUID()) {} //If I am not tank and there is a target

    ThreatManager &pthreatManager = pTarget->getThreatManager();
    float maxThreat = pthreatManager.getThreat(pTarget->getVictim()) ;
    if (maxThreat <= 0) { return 100; } //0 threat
    float curThreat = pthreatManager.getThreat(pFrom);
    return (curThreat * 100 / maxThreat);
}
//Gets if the unit is under attack by # of attackers
bool PlayerbotClassAI::isUnderAttack(Unit *pAttacked,const uint8 &minNumberOfAttackers)
{
    if (!pAttacked) { pAttacked = m_bot; if (!pAttacked) { return false; } }
    Unit::AttackerSet fAttackerSet = pAttacked->getAttackers();
    if (fAttackerSet.size() >= minNumberOfAttackers) { return true; }
    return false;
}

//Gets the first found attacker of Unit
Unit *PlayerbotClassAI::GetAttackerOf(Unit *pAttacked)
{
    if (!pAttacked) { pAttacked = m_bot; if (!pAttacked) { return NULL; } }
    Unit::AttackerSet fAttackerSet = pAttacked->getAttackers();
    if (fAttackerSet.size() <= 0) { return NULL; }
    return (*fAttackerSet.begin());
}
//Gets the first found attacker of Unit if not nearestToAttacked > finds the one nearest to bot
Unit *PlayerbotClassAI::GetNearestAttackerOf(Unit *pAttacked, bool nearestToAttacked)
{
    if (!pAttacked) { pAttacked = m_bot; if (!pAttacked) return NULL;}

    Unit::AttackerSet fAttackerSet = pAttacked->getAttackers();
    if (fAttackerSet.size() <= 0) { return NULL; }

    Unit *nearestTo = m_bot;
    if (nearestToAttacked) { nearestTo = pAttacked; }

    Unit *curAtt = NULL;
    float minDist = 30;


    for (Unit::AttackerSet::const_iterator itr = fAttackerSet.begin(); itr != fAttackerSet.end(); ++itr)
    {
        Unit *tAtt = (*itr);
        if (!tAtt) break; // Something is wrong.. How can a non existing mob attack?
        if (tAtt->isDead()) break;
        if (m_bot->GetDistance(tAtt) >= minDist) continue; //Get the nearest one
        curAtt = tAtt;
        minDist = tAtt->GetDistance(nearestTo);
    }
    return curAtt;

}

uint8 PlayerbotClassAI::GetHealthPercentRaid(Player *gPlayer, uint8 &countNeedHealing)
{
    uint8 validMemberCount=0;
    uint16 totalHPPercent=0;
    std::list<Unit*> unitList;
    gPlayer->GetRaidMember(unitList,30);
    if(!unitList.empty()){
      for (std::list<Unit*>::iterator itr = unitList.begin() ; itr!=unitList.end();++itr) {
        //Player *tPlayer = GetPlayerBot()->GetObjPlayer((*itr)->GetGUID());
        Unit *tPlayer = sObjectMgr->GetPlayer((*itr)->GetGUID());
        if(tPlayer == NULL) continue;
        if(tPlayer->isDead()) continue;
        if(GetPlayerBot()->GetAreaId() != tPlayer->GetAreaId()) continue;
        //if(tPlayer->GetGUID() == GetPlayerBot()->GetGUID()) continue;
        if(GetPlayerBot()->GetDistance(tPlayer) > 30) continue;
        uint8 fndHPPercent =  tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth();
        totalHPPercent+=fndHPPercent;
        validMemberCount++;
        if (fndHPPercent < 100) countNeedHealing++;

        //const std::string myname = GetPlayerBot()->GetName();
        //const std::string hisname = tPlayer->GetName();
        //sLog->outDebug("me = %s, checked= %s %u [%u / %u]", myname.c_str(), hisname.c_str(), fndHPPercent, tPlayer->GetHealth(), tPlayer->GetMaxHealth());

      }
    }
    if (validMemberCount == 0) return 100;
    return totalHPPercent / validMemberCount;
}

Unit *PlayerbotClassAI::DoSelectLowestHpFriendly(float range, uint32 MinHPDiff)
{
    Unit *pUnit = NULL;
    Trinity::MostHPMissingInRange u_check(GetPlayerBot(), range, MinHPDiff);
    Trinity::UnitLastSearcher<Trinity::MostHPMissingInRange> searcher(GetPlayerBot(), pUnit, u_check);

    GetPlayerBot()->VisitNearbyObject(range, searcher);

    return pUnit;
}

void PlayerbotClassAI::SetMainTank(Unit *tank)
{
    mainTank = tank;
}

// is Resource heavy, do not spam or use heavily in loop
Unit *PlayerbotClassAI::FindMainTankInRaid(Player *gPlayer)
{
    // check if original main tank is still alive. No point regetting main
    // tank b/c chances are slim that it will not get reset in the middle of a fight.
    // But if main tank dies, try to find next best canidate
    if (mainTank!=NULL && mainTank->isAlive()) {
        return mainTank;
    }

    if (!gPlayer) return NULL;
    Group *pGroup = gPlayer->GetGroup();
    if (!pGroup) return NULL;
    uint64 pLeaderGuid = pGroup->GetLeaderGUID();

    Unit *pPlayer = NULL;

    // Check if set in raid
    if (pGroup->isRaidGroup())
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT memberGuid FROM group_member WHERE memberFlags='%u' AND guid = '%u'",MEMBER_FLAG_MAINTANK, pGroup->GetGUID());
        if(result)
        {
            uint64 pGuid = MAKE_NEW_GUID(result->Fetch()->GetInt32(),0,HIGHGUID_PLAYER);
            pPlayer = sObjectMgr->GetPlayer(pGuid);
            if (pPlayer && pGroup->IsMember(pGuid) && pPlayer->isAlive()){
                mainTank = pPlayer;
                return pPlayer;
            }
        }
    }


    // if could not find tank try assuming
    // Assume the one with highest health is the main tank
    uint32 maxhpfound=0;
    std::list<Unit*> unitList;
    gPlayer->GetRaidMember(unitList,30);
    if (!unitList.empty()){
      for (std::list<Unit*>::iterator itr = unitList.begin() ; itr!=unitList.end();++itr) {
        //Player *tPlayer = GetPlayerBot()->GetObjPlayer((*itr)->GetGUID());
        Unit *tPlayer = sObjectMgr->GetPlayer((*itr)->GetGUID());
        if (tPlayer == NULL) continue;
        if (tPlayer->isDead()) continue;
        if (GetPlayerBot()->GetAreaId() != tPlayer->GetAreaId()) continue;
        //if(tPlayer->GetGUID() == GetPlayerBot()->GetGUID()) continue;
        if (GetPlayerBot()->GetDistance(tPlayer) > 50) continue;
        if (tPlayer->GetMaxHealth() > maxhpfound) { maxhpfound = tPlayer->GetMaxHealth(); pPlayer=tPlayer; }
        // Also check pets
        if ( (tPlayer->getClass() == (uint8) CLASS_HUNTER || tPlayer->getClass() == (uint8) CLASS_WARLOCK) && IS_PET_GUID(tPlayer->GetPetGUID()) )
        {
            Pet* tpet = ObjectAccessor::GetPet(*tPlayer, tPlayer->GetPetGUID());
            if (!tpet || !tpet->IsInWorld() || !tpet->isDead()) continue;
            if (tpet->GetArmor() > tPlayer->GetArmor()) //Probably a tanking capable pet..
            {
                if (tpet->GetMaxHealth() > maxhpfound) { maxhpfound = tpet->GetMaxHealth(); pPlayer=tpet; }
                else if (tPlayer->GetGUID() == pPlayer->GetGUID()) {pPlayer = tpet;} //set pet as tank instead of owner
            }
        }
      }
    }

    mainTank = pPlayer;
    return pPlayer;
}

Unit *PlayerbotClassAI::FindMainAssistInRaid(Player *gPlayer)
{
    if (!gPlayer) return NULL;
    Group *pGroup = gPlayer->GetGroup();
    if (!pGroup) return NULL;
    uint64 pLeaderGuid = pGroup->GetLeaderGUID();


    Unit *pPlayer = NULL;

    // Check if set in raid
    if (pGroup->isRaidGroup())
    {
        QueryResult result = CharacterDatabase.PQuery("SELECT memberGuid FROM group_member WHERE memberFlags='%u' AND guid = '%u'",MEMBER_FLAG_MAINASSIST, pGroup->GetGUID());
          if(result)
        {
            uint64 pGuid = MAKE_NEW_GUID(result->Fetch()->GetInt32(),0,HIGHGUID_PLAYER);
            pPlayer = sObjectMgr->GetPlayer(pGuid);
            if (pPlayer && pGroup->IsMember(pGuid) && pPlayer->isAlive()){
                return pPlayer;
            }
        }
    }

    // default to main tank
    return FindMainTankInRaid(gPlayer);
}

Player * PlayerbotClassAI::FindMage(Player *gPlayer)
{
    Group::MemberSlotList const &groupSlot = gPlayer->GetGroup()->GetMemberSlots();
    for(Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
    {
        Player *tPlayer = sObjectMgr->GetPlayer(itr->guid);

        if(tPlayer == NULL) continue;
        if(tPlayer->GetGUID() == GetPlayerBot()->GetGUID()) continue;
        if(GetPlayerBot()->GetAreaId() != gPlayer->GetAreaId()) continue;
        if(GetPlayerBot()->GetDistance(tPlayer) > 30) continue;

        if (tPlayer->getClass() == CLASS_MAGE) return tPlayer;
    }
    return NULL;
}


