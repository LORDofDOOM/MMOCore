#ifndef _PLAYERBOTPRIESTAI_H
#define _PLAYERBOTPRIESTAI_H

#include "PlayerbotClassAI.h"

//class Player;

class PlayerbotPriestAI : PlayerbotClassAI
{
public:
    PlayerbotPriestAI(Player *const master, Player *const bot, PlayerbotAI *const ai);
    virtual ~PlayerbotPriestAI();

    virtual void LoadSpells();

    //all combat actions go here
    void DoNextCombatManeuver(Unit *);

    //all non combat actions go here, ex buffs, heals, rezzes
    void DoNonCombatActions();

    //buff a specific player, usually a real PC who is not in group
    bool BuffPlayer(Unit *target);

    //Heals the target based off its HP
    bool HealTarget(Unit *target, uint8 hp);

    bool HealGroup (Unit *target, uint8 hp, uint8 &countNeedHeal);

    //Cures the target
    bool CureTarget(Unit *target);

    bool RezTarget (Unit *target);

private:
    //heals
    uint32 RENEW, FLASH_HEAL, HEAL, BINDING_HEAL, PO_MENDING, DESPERATE_PRAYER, PO_HEALING, CIRCLE_OF_HEALING, DIVINE_HYMN, RESURRECTION, HYMN_OF_HOPE, CURE_DISEASE, DISPEL_MAGIC, MASS_DISPEL;

    //Holy Offensive
    uint32 SMITE, HOLY_FIRE, PENANCE, HOLY_NOVA;

    //Shadow Offensive
    uint32 MIND_BLAST, SW_PAIN, DEVOURING_PLAGUE, MIND_FLAY, VAMPIRIC_EMBRACE, VAMPIRIC_TOUCH, SW_DEATH, MIND_SEAR, MANA_BURN, SHADOWFIEND;

    //CC - Breaker
    uint32 PSYCHIC_SCREAM, PSYCHIC_HORROR, MIND_SOOTHE, SHACKLE_UNDEAD, SILENCE, MIND_CONTROL;

    //buffs
    uint32 PW_SHIELD, INNER_FIRE, GUARDIAN_SPIRIT, FADE, INNER_FOCUS, POWER_INFUSION, PAIN_SUPPRESSION, SHADOWFORM, DISPERSION, LIGHTWELL,
        PW_FORTITUDE, DIVINE_SPIRIT, SHADOW_PROTECTION, PO_FORTITUDE, PO_SPIRIT, PO_SHADOW_PROTECTION, FEAR_WARD;

    uint32 SHOOT;

    uint32 TALENT_DISC, TALENT_HOLY, TALENT_SHADOW;
};



#endif
