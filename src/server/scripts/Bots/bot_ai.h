#ifndef _BOT_AI_H
#define _BOT_AI_H

#define SPELL_LEVEL ((int)m_creature->getLevel()/10)

#define BANDAGE 27031
#define MANAPOTION 28499
#define REJUVEPOTION 28517
#define HEALINGPOTION 28495
#define POTIONCD 60000
#define FIRSTAID 60000
#define DRINK 1137

#define master ((Player*)m_creature->GetCharmer())

typedef std::set<Unit *> AttackerSet;

class bot_ai : public ScriptedAI
{
    public:
        bot_ai(Creature *c);

        virtual ~bot_ai();

        //Cure the target
        virtual bool CureTarget (Unit *target);

        //Heal the target
        virtual bool HealTarget (Unit *target, uint8 hp);

        // Buff target
        virtual void BuffTarget(Unit *target);

        virtual void ReceiveBowEmote(Player *player);


        // Cycles through the group to heal/buff/rezz
        void BuffAndHealGroup(Player *gPlayer);
        void RezGroup(uint32 REZZ, Player *gPlayer);

        //Debug method to list the auras currently active.
        //Use to find what spells were casted
        bool listAuras(Unit *unit);

        //More generalized method than HasAura().  It looks for
        //any rank of the spell and it doesn't care which
        //spell effect you want.  If it has the spell aura than
        //it returns true
        bool HasAuraName(Unit *unit, std::string spell, uint64 casterGuid=0);
        bool HasAuraName(Unit *unit, uint32 spellId, uint64 casterGuid=0);
        bool HasAuraIcon (Unit *unit, uint32 SpellIconID, uint64 casterGuid=0);

        std::string GetSpellName(uint32 spell_id);

        void doCast(Unit *victim, uint32 spellId, bool triggered = false);
        bool isTimerReady(int32 timer);

        void ResetOrGetNextTarget();

        void ReceiveEmote(Player *player, uint32 emote);

        void DoSay(const std::string& text, const uint32 language,uint64 receiver,Player *player);

        bool IAmDead();

        void GiveManaBack(uint8 amount=25);

        void CureGroup(Unit *pTarget);

        void Feast();

        void BotAttackStart(Unit *victim);

        Creature *m_creature;

        int32 GC_Timer; //global cooldown
        uint32 FirstAid_cd;
        uint32 Potion_cd;

    private:
        bool gettingAttacked(AttackerSet m_attackers);

        CommandStates prevCommandState;

    protected:
        void setStats(uint32 myclass, uint32 myrace, uint32 mylevel);
};
#endif

