#include "ScriptPCH.h"
#include "bot_shaman_ai.h"
#include "Group.h"

class shaman_bot : public CreatureScript
{
public:
    shaman_bot() : CreatureScript("shaman_bot") { }

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new shaman_botAI(pCreature);
    }

struct shaman_botAI : public bot_ai
{
    shaman_botAI(Creature *c) : bot_ai(c)
    {
        Reset();
    }

    int32 GC_Timer; //global cooldown
    int32 Heal_Timer;
    int32 Lesser_Healing_Timer;
    int32 Self_Lesser_Healing_Timer;
    int32 Flame_Shock_Timer;
    int32 Earth_Shock_Timer;
    int32 Lightning_Bolt_Timer;
    int32 Others_Heal_Timer;
    int32 Oom_timer;
    int32 Potion_Timer;
    int32 Rez_Timer;
    int32 Earth_Totem_Timer;
    int32 Water_Totem_Timer;
    int32 Fire_Totem_Timer;
    int32 Wind_Totem_Timer;

    Unit *mobsTarget;
    Unit *opponent;

    void Reset()
    {
        GC_Timer = 0;
        Heal_Timer = 0;
        Lesser_Healing_Timer = 0;
        Self_Lesser_Healing_Timer = 0;
        Flame_Shock_Timer = 20;
        Lightning_Bolt_Timer = 60;
        Earth_Shock_Timer = 150;
        Others_Heal_Timer = 0;
        Oom_timer = 0;

        Earth_Totem_Timer = 0;
        Fire_Totem_Timer = 0;
        Water_Totem_Timer = 0;
        Wind_Totem_Timer = 0;

        Potion_Timer = 0;
        Rez_Timer = 0;

        opponent = NULL;

        if (master) {
            setStats(CLASS_SHAMAN, m_creature->getRace(), master->getLevel());
       }
    }

    bool isTimerReady(int32 timer)
    {
        if(timer <= 0 && GC_Timer <= 0) return true;
        else                            return false;
    } //end isTimerReady

    void doCast(Unit *victim, uint32 spellId, bool triggered = false)
    {
        if(spellId == 0) return;
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        GC_Timer = 40;
        DoCast(victim, spellId, triggered);
    } //end doCast

    void decrementTimers()
    {
        if(GC_Timer > 0)             --GC_Timer;
        if(Heal_Timer > 0)           --Heal_Timer;
        if(Others_Heal_Timer > 0)    --Others_Heal_Timer;
        if(Flame_Shock_Timer > 0)    --Flame_Shock_Timer;
        if(Earth_Shock_Timer > 0)    --Earth_Shock_Timer;
        if(Lightning_Bolt_Timer > 0) --Lightning_Bolt_Timer;
        if(Rez_Timer > 0)            --Rez_Timer;
        if(Potion_Timer > 0)         --Potion_Timer;
        if(Earth_Totem_Timer > 0)    --Earth_Totem_Timer;
        if(Fire_Totem_Timer > 0)     --Fire_Totem_Timer;
        if(Water_Totem_Timer > 0)    --Water_Totem_Timer;
        if(Wind_Totem_Timer > 0)     --Wind_Totem_Timer;
    } //end decrementTImers

    void KilledUnit(Unit *)
    {
        ((Player*)master)->SetBotCommandState(COMMAND_FOLLOW);
    }

    void Aggro(Unit *who){}

    void EnterEvadeMode(){ Oom_timer = 0; }

    void UpdateAI(const uint32 diff)
    {
        decrementTimers();

        if(IAmDead()) return;

        if(m_creature->GetPower(POWER_MANA) < 400 &&
        isTimerReady(Potion_Timer))
        {
            doCast(m_creature, MANAPOTION);
            Potion_Timer = 150;
        }
        if(m_creature->GetPower(POWER_MANA)/m_creature->GetMaxPower(POWER_MANA) < 10)
        {
            if(Oom_timer == 0)
            {
                //MonsterSay("OOM", LANG_UNIVERSAL, NULL);
                Oom_timer = 1;
            }
        }

        BuffAndHealGroup(master);

        // Heal myself
        HealTarget (m_creature, m_creature->GetHealth()*100 / m_creature->GetMaxHealth());

        //the rest are combat so return if not fighting
        opponent = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);
        if(!opponent && !m_creature->getVictim())
        {
            DoNonCombatActions();
            ResetOrGetNextTarget();
            return;
        }

        //Cast totems.
        if(m_creature->isInCombat() &&
        isTimerReady(Earth_Totem_Timer) &&
        !master->HasAura(SPELL_STONESKIN_AURA, 0))
        {
            doCast(m_creature, SPELL_STONESKIN_TOTEM);
            GiveManaBack();
            Earth_Totem_Timer = 90;
            return;
        }

        if(m_creature->isInCombat() &&
        isTimerReady(Fire_Totem_Timer))
        {
            doCast(m_creature, SPELL_SEARING_TOTEM);
            GiveManaBack();
            Fire_Totem_Timer = 180;
            return;
        }

        if(m_creature->isInCombat() &&
        isTimerReady(Wind_Totem_Timer))
        {
            doCast(m_creature, SPELL_WINDFURY_TOTEM);
            GiveManaBack();
            Wind_Totem_Timer = 180;
            return;
        }

        if(m_creature->isInCombat())
        {
            switch(master->getClass())
            {
                case CLASS_WARRIOR:
                case CLASS_DEATH_KNIGHT:
                case CLASS_ROGUE:
                {
                    if(isTimerReady(Water_Totem_Timer) &&
                    !master->HasAura(SPELL_HEALINGSTREAM_AURA))
                    {
                        doCast(m_creature, SPELL_HEALINGSTREAM_TOTEM);
                        Water_Totem_Timer = 90;
                        GiveManaBack();
                        return;
                    }
                    break;
                }
                default: //everyone else gets a mana totem
                {
                    if(isTimerReady (Water_Totem_Timer) &&
                    !master->HasAura(SPELL_MANASPRING_AURA, 0))
                    {
                        doCast(m_creature, SPELL_MANASPRING_TOTEM);
                        Water_Totem_Timer = 90;
                        GiveManaBack();
                        return;
                    }
                }
            } //end switch

        }


        if(isTimerReady(Flame_Shock_Timer))
        {
            doCast(opponent, SPELL_FLAME_SHOCK);
            Flame_Shock_Timer = 120;
            return;
        }

        if(isTimerReady(Lightning_Bolt_Timer))
        {
            doCast(opponent, SPELL_LIGHTNING_BOLT);
            Lightning_Bolt_Timer = 180;
            return;
        }

        if(isTimerReady(Earth_Shock_Timer))
        {
            doCast(opponent, SPELL_EARTH_SHOCK);
            Earth_Shock_Timer = 250;
            return;
        }

        //now try to heal bots and pets.  DoSelectLowestHpFriendly will get
        //everyone in group including bots and pets.  Unfortunately it can
        //not be triggered for % of lost HP, so we just set it to -1000.
        //This means low level players wont be healed because they wont have
        //enough HP.
        Unit *target = DoSelectLowestHpFriendly(40, 1000);
        if(target)
        {
            if(CanCast(target, GetSpellStore()->LookupEntry (SPELL_CHAIN_HEAL)))
            {
                doCast(target, SPELL_CHAIN_HEAL, false);
                Others_Heal_Timer = 50;
            }
        } else {
            target = DoSelectLowestHpFriendly(40, 500); //now try someone with less HP lost
            if(target)
            {
                if(CanCast(target, GetSpellStore()->LookupEntry (SPELL_CHAIN_HEAL)))
                {
                    doCast(target, SPELL_CHAIN_HEAL, false);
                    Others_Heal_Timer = 100;
                }
            }
        }

        ScriptedAI::UpdateAI(diff);

    } //end UpdateAI

    void DoNonCombatActions()
    {
        Feast();

        if(isTimerReady(GC_Timer) && !m_creature->HasAura(SPELL_LIGHTNING_SHIELD, 0))
            doCast(m_creature, SPELL_LIGHTNING_SHIELD);

        //Casts buffs
       // if(!m_creature->isInCombat())
       // {
            //if(!m_creature->HasAura(SPELL_WINDFURY_WEAPON, 0)) doCast(m_creature, SPELL_WINDFURY_WEAPON);
 //           if(isTimerReady(GC_Timer) && !m_creature->HasAura(SPELL_LIGHTNING_SHIELD, 0))
   //             doCast(m_creature, SPELL_LIGHTNING_SHIELD);
       // }


        //Heal/rez others
        //
        //check group members, this doesn't check bots/pets.  They will be done later.  Preference
        //goes to real players first.
        //
        //buff and heal group
        if(master->GetGroup())
        {
            RezGroup(SPELL_SHAMAN_REZZ, master);
            BuffAndHealGroup(master);
           // CureGroup(master);
        }


        /* TESTING */
        Group::MemberSlotList const &a =((Player*)master)->GetGroup()->GetMemberSlots();
        for(Group::member_citerator itr = a.begin(); itr != a.end(); itr++)
        {
            Player *tPlayer = ((Player *)master)->GetObjPlayer(itr->guid);
            if(tPlayer == NULL) continue;
            //healing others
            if(tPlayer->isAlive() &&
            isTimerReady(Others_Heal_Timer) &&
            tPlayer->GetGUID() != master->GetGUID() &&
            tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth() < 75 &&
            CanCast(tPlayer, GetSpellStore()->LookupEntry (SPELL_CHAIN_HEAL)))
            {
                doCast(tPlayer, SPELL_CHAIN_HEAL, false);
                Others_Heal_Timer = 100;
            }

            //rezzes
            if(tPlayer->isDead() &&
            !m_creature->isInCombat() &&
            //CanCast(tPlayer, GetSpellStore()->LookupEntry (SPELL_SHAMAN_REZZ)) &&
            m_creature->GetDistance(tPlayer) < 40 &&
            isTimerReady(Rez_Timer))
            {
                char *str = (char *)malloc(32);
                sprintf(str, "Rezzing %s", tPlayer->GetName());
                m_creature->MonsterSay(str, LANG_UNIVERSAL, NULL);
                free(str);
                doCast(tPlayer, SPELL_SHAMAN_REZZ, false);
                Rez_Timer = 160;
            }
        }
        /* TESTING */



/* TESTING */
        if((master->GetHealth()*100 / master->GetMaxHealth() < 90) && Lesser_Healing_Timer <= 0)
        {
            doCast(master, SPELL_LESSER_HEALING);
            Lesser_Healing_Timer = 90;
            Heal_Timer = Heal_Timer + 5; //wait 5 seconds before casting a real heal
            //if(master->isInCombat()) && master->getVictim() == NULL) return;
            return;
        } else if(Lesser_Healing_Timer >= 0) --Lesser_Healing_Timer;

        if((master->GetHealth()*100 / master->GetMaxHealth() < 75) && isTimerReady(Heal_Timer))
        {
            doCast(master, SPELL_CHAIN_HEAL);
            Heal_Timer = 10;
        }

        if(m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 90)
        {
            if(Self_Lesser_Healing_Timer <= 0)
            {
                doCast(m_creature, SPELL_LESSER_HEALING);
                Self_Lesser_Healing_Timer = 90;
                return;
            } else if(Self_Lesser_Healing_Timer >= 0)
                --Self_Lesser_Healing_Timer;
        }
/* TESTING */
        }

    bool HealTarget(Unit *target, uint8 hp)
    {
        if (!isTimerReady(GC_Timer)) return false;
        if (m_creature->IsNonMeleeSpellCasted(true)) return false;
        if(!target || target->isDead()) return false;

        if(hp < 90 && Lesser_Healing_Timer <= 0)
        {
            doCast(target, SPELL_LESSER_HEALING);
            Lesser_Healing_Timer = 90;
            Heal_Timer = Heal_Timer + 5; //wait 5 seconds before casting a real heal
            //if(master->isInCombat()) && master->getVictim() == NULL) return;
            return true;
        } else if(Lesser_Healing_Timer >= 0) --Lesser_Healing_Timer;

        if(hp < 75 && isTimerReady(Heal_Timer))
        {
            doCast(target, SPELL_CHAIN_HEAL);
            Heal_Timer = 10;
        }
        return true;
    } //end HealTarget

    void ReceiveBowEmote(Player *player)
    {
        doCast(m_creature, SPELL_MANASPRING_TOTEM);
    }


}; //end shaman_bot
};


void AddSC_shaman_bot()
{
    new shaman_bot();
}
