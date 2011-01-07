#include "bot_ai.h"


//Cooldown/Timers
#define CHARGE_CD 100
#define DEATHWISH_CD 180000
#define MORTALSTRIKE_CD 7000
#define OVERPOWER_CD 5000
#define RETALIATION_RECKLESSNESS_SHIELDWALL_CD 1800000
#define BERSERKERRAGE_CD 30000
#define INTERCEPT_CD 15000
#define INTIMIDATINGSHOUT_CD 120000
#define PUMMEL_CD 10000
#define WHIRLWIND_CD 10000
#define BLOODRAGE_CD 60000
#define DISARM_CD 60000
#define INTERVENE_CD 30000
#define BATTLESHOUT_CD 30000
#define SHIELDBASH_CD 12000
#define SPELLREFLECTION_CD 10000
#define PVPTRINKET_CD 120000
#define TAUNT_CD 8000
#define SUNDER_CD 8000

//others
#define PVPTRINKET 42292
#define YELLRAGE_CD 5000

#define SPELL_NOGGENFOGGER_SMALL    16595
#define SPELL_NOGGENFOGGER_SKELETON    16591

//spells
#define TAUNT              355
#define TAUNT_VISUAL       34105
#define CHALLENGING_SHOUT   1161

/*
 *  Some spells NPCs can't seem to cast.  So for those that I couldn't
 *  get to work, I used an NPC equivalent version.  Most of these don't
 *  have different levels so it is one spell for all levels.  This
 *  leads it to be over/under powered in some levels.  Oh well.
 *  An example is BattleShout.
 */

//Defensive Stance
#define DEFENSIVESTANCE     71
#define DISARM              676 //DISARM_A[SPELL_LEVEL]
#define BLOODRAGE           29131 //2687 original warrior spell


//Berserker Stance
#define BERSERKERSTANCE     7366 //2458 original warrior spell
#define BERSERKERRAGE       18499

//#define BATTLESHOUT       26043
#define COMMANDINGSHOUT     469
#define BATTLESHOUT         BATTLESHOUT_A[SPELL_LEVEL]
//#define EXECUTE           EXECUTE_A[SPELL_LEVEL]
#define EXECUTE             38959 //25236 original warrior spell
#define WHIRLWIND           WHIRLDWIND_A[SPELL_LEVEL]
//#define PUMMEL            PUMMEL_A[SPELL_LEVEL]
#define PUMMEL              15615 //6554 original warrior spell
//#define DEMORALIZINGSHOUT 29584 //25203 original warrior spell
#define DEMORALIZINGSHOUT   29584 //25203 original warrior spell
#define INTERCEPT           27577 //25275 original warrior spell


//Battle Stance
#define BATTLESTANCE        7165 //2457 original warrior one
//#define CHARGE            CHARGE_A[SPELL_LEVEL]
#define CHARGE              37511 //11578 original warrior one
#define HAMSTRING           HAMSTRING_A[SPELL_LEVEL]
#define REND                REND_A[SPELL_LEVEL]
#define MORTALSTRIKE        MORTALSTRIKE_A[SPELL_LEVEL]
#define SUNDER              SUNDER_A[SPELL_LEVEL]


//uint32 BATTLESHOUT_A[] = { 6673, 5242, 6192, 11549, 11550, 11551, 25289, 2048, 47436 };
uint32 BATTLESHOUT_A[] = { 9128, 9128, 9128, 27578, 27578, 26043, 26043, 26043, 26043 };
uint32 REND_A[] = { 772, 6546, 6547, 6548, 11572, 11573, 11574, 25208, 46845 };
uint32 CHARGE_A[] = { 100, 100, 6178, 11578, 11578, 11578, 11578, 11578, 11578 };
uint32 HAMSTRING_A[] = { 1715, 1715, 1715, 7372, 7372, 7373, 7373, 25212, 25212 };
uint32 EXECUTE_A[] = { 1715, 1715, 1715, 7372, 7372, 7373, 7373, 25212, 25212 };
uint32 WHIRLDWIND_A[] = { 0, 0, 0, 0, 1680, 1680, 1680, 1680, 1680 };
uint32 PUMMEL_A[] = { 0, 0, 0, 0, 6552, 6552, 6552, 6552, 6552 };
uint32 MORTALSTRIKE_A[] = { 0, 0, 0, 0, 12294, 21552, 21553, 25248, 47485 };
uint32 SUNDER_A[] = { 0, 7386, 7405, 8380, 11596, 11597, 25225, 47467, 47467 };
