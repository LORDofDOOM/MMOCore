UPDATE `quest_template` SET `NextQuestId` = 13215 WHERE `id` = 13214;
UPDATE `quest_template` SET `PrevQuestId` = 13214, `NextQuestId` = 13216 WHERE `id` = 13215;
UPDATE `quest_template` SET `PrevQuestId` = 13215, `NextQuestId` = 13217 WHERE `id` = 13216;
UPDATE `quest_template` SET `PrevQuestId` = 13216, `NextQuestId` = 13218 WHERE `id` = 13217;
UPDATE `quest_template` SET `PrevQuestId` = 13217, `NextQuestId` = 13219 WHERE `id` = 13218;
UPDATE `quest_template` SET `PrevQuestId` = 13218 WHERE `id` = 13219;

DELETE FROM quest_start_scripts WHERE id IN (13214,13215,13216,13216,13217,13218,13218,13219);
INSERT INTO `quest_start_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(13214, 15, 10, 31191, 180000, 0, 8190.40, 3569.77, 629.37, 4.82),
(13214, 15, 10, 31192, 180000, 0, 8180.69, 3567.77, 629.71, 5.24),
(13214, 15, 10, 31193, 180000, 0, 8176.58, 3561.77, 628.68, 5.50),
(13214, 15, 10, 31194, 180000, 0, 8170.96, 3556.77, 630.37, 5.50),
(13214, 15, 10, 31195, 180000, 0, 8165.47, 3551.62, 633.06, 5.75),
(13214, 15, 10, 31196, 180000, 0, 8163.07, 3545.77, 632.40, 5.70),
(13215, 15, 10, 31222, 180000, 0, 8170.96, 3556.77, 630.37, 5.50),
(13216, 15, 10, 31242, 180000, 0, 8170.96, 3556.77, 630.37, 5.50),
(13217, 15, 10, 31271, 180000, 0, 8170.96, 3556.77, 630.37, 5.50),
(13218, 15, 10, 31277, 180000, 0, 8170.96, 3556.77, 630.37, 5.50),
(13219, 15, 10, 14688, 180000, 0, 8170.96, 3556.77, 630.37, 5.50);

UPDATE `quest_template` SET `RequiredNpcOrGo1` = 31191, `RequiredNpcOrGoCount1` = 1, `DetailsEmote1` = 4, `EmoteOnComplete` = 4, `StartScript` = 13214, `SpecialFlags` = 0 WHERE `id` = 13214;
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 31222, `RequiredNpcOrGoCount1` = 1, `DetailsEmote1` = 4, `EmoteOnComplete` = 4, `StartScript` = 13215, `SpecialFlags` = 0 WHERE `id` = 13215;
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 31242, `RequiredNpcOrGoCount1` = 1, `DetailsEmote1` = 4, `EmoteOnComplete` = 4, `StartScript` = 13216, `SpecialFlags` = 0 WHERE `id` = 13216;
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 31271, `RequiredNpcOrGoCount1` = 1, `DetailsEmote1` = 4, `EmoteOnComplete` = 4, `StartScript` = 13217, `SpecialFlags` = 0 WHERE `id` = 13217;
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 31277, `RequiredNpcOrGoCount1` = 1, `DetailsEmote1` = 4, `EmoteOnComplete` = 4, `StartScript` = 13218, `SpecialFlags` = 0 WHERE `id` = 13218;
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 14688, `RequiredNpcOrGoCount1` = 1, `DetailsEmote1` = 4, `EmoteOnComplete` = 4, `StartScript` = 13219, `SpecialFlags` = 0 WHERE `id` = 13219;

UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `mindmg` = 300, `maxdmg` = 800, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 32484;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 15586, `spell2` = 25054, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31191;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 15496, `spell2` = 41057, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31192;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61041, `spell2` = 46182, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31193;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 14873, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31194;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 34829, `spell2` = 44475, `spell3` = 15620, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31195;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61044, `spell2` = 58461, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31196;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61055, `spell2` = 22884, `spell2` = 38243, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1600, `attackpower` = 100, `baseattacktime` = 1500 WHERE `entry` = 31222;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61165, `spell2` = 57635, `spell3` = 61170, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 800, `maxdmg` = 1200, `attackpower` = 300, `baseattacktime` = 1500 WHERE `entry` = 31242;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61070, `spell2` = 61065, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 1200, `maxdmg` = 1800, `attackpower` = 800, `baseattacktime` = 1500 WHERE `entry` = 31271;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61133, `spell2` = 13737, `spell3` = 61139, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 1000, `maxdmg` = 1700, `attackpower` = 800, `baseattacktime` = 1500 WHERE `entry` = 31277;
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14, `spell1` = 61162, `spell2` = 61145, `spell3` = 61163, `InhabitType` = 1, `movementId` = 0, `MovementType` = 1, `InhabitType` = 3, `movementId` = 199, `mindmg` = 6000, `maxdmg` = 900, `attackpower` = 500, `baseattacktime` = 1500 WHERE `entry` = 14688;
DELETE FROM creature_ai_scripts WHERE creature_id IN (31191,31192,31193,31194,31195,31196,31222,31242,31271,31277,14688);
INSERT INTO `creature_ai_scripts` VALUES
( 3119101, 31191, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 15586, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119102, 31191, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 25054, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119201, 31192, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 15496, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119202, 31192, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 41057, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119301, 31193, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61041, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119302, 31193, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 46182, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119401, 31194, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 14873, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119501, 31195, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 34829, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119502, 31195, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 44475, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119503, 31195, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 15620, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119601, 31196, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61044, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3119602, 31196, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 58461, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3122201, 31222, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61055, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3122202, 31222, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 22884, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3122203, 31222, 0, 0, 100, 0, 8000, 7000, 17000, 0, 11, 38243, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3124201, 31242, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61165, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3124202, 31242, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 57635, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3127101, 31271, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61070, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3127102, 31271, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 61065, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3127701, 31277, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61133, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3127702, 31277, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 13737, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 3127703, 31277, 0, 0, 100, 0, 5000, 7000, 17000, 0, 11, 61139, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 1468801, 14688, 0, 0, 100, 0, 10000, 15000, 20000, 0, 11, 61162, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 1468802, 14688, 0, 0, 100, 0, 45000, 50000, 67000, 0, 11, 61145, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0),
( 1468803, 14688, 0, 0, 100, 0, 8000, 15000, 20000, 0, 11, 61163, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);