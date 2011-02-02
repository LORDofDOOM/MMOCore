-- The_Storm_Peaks_Full.sql
-- -------------------------------------------
-- Chains Sons Of Hodir
-- -------------------------------------------
-- -------------------------------------------
-- Chains Sons Of Hodir
-- -------------------------------------------
-- Fix Quest http://www.wowhead.com/quest=12851 "Going Bearback"
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` =29598; 
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `quest_start`, `quest_start_active`, `quest_end`, `cast_flags`, `aura_required`, `aura_forbidden`, `user_type`) VALUES
(29598, 54768, 12851, 1, 12851, 1, 0, 0, 0);-- Icefang Summon.
UPDATE `creature_template` SET `spell1` = '54897' , `spell2` =  '54907' WHERE `entry` = 29602; -- Spells Icefang
UPDATE `creature` SET `spawntimesecs` = 30 WHERE `id` IN (29358,29351); -- Mobs Respawn Rapido (Elites).
UPDATE `creature_template` SET `ScriptName` = 'npc_icefang' WHERE `entry` IN (29358,29351); -- Set Scriptname.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 54798; -- A adida Condiciones Correspondientes
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) 
VALUES
(13,54798,18,1,29351),
(13,54798,18,1,29358);
-- Fix Quest: "Cold Hearted" (12856) http://www.wowhead.com/quest=12856
UPDATE `creature` SET `spawntimesecs` = 30 WHERE `id` = 29708;-- Respawn Rapido "Captive Protodrake"
UPDATE `creature_template` SET `spell1` = 55046, `faction_A` = 35, `faction_H` = 35, `InhabitType` = 4 WHERE `entry` = 29709; -- Protodrake Misc updates
-- ScriptNames
UPDATE `creature_template` SET `ScriptName` = 'npc_freed_protodrake' WHERE `entry` = 29709;
UPDATE `creature_template` SET `ScriptName` = 'npc_captive_protodrake' WHERE `entry` = 29708;
UPDATE `creature_template` SET `ScriptName` = 'npc_brunnhildar_prisoner' WHERE `entry` = 29639; 
UPDATE `creature_template` SET `ScriptName` = 'npc_freed_protodrake' WHERE `entry` = 29709;
UPDATE `spell_area` SET `quest_end` = 12983 WHERE `area` = 4437 AND `quest_end` = 13063;
-- Fix Quest: Quest: The Drakkensryd (12886)
UPDATE `creature_template` SET `InhabitType` = 4, `ScriptName` = 'npc_hyldsmeet_protodrake' WHERE `entry` = 29679;-- The Warm-Up & Into the Pit & Back to the Pit
-- Spells  WoWWiki - http://www.wowwiki.com/Warbear_Matriarch
UPDATE `creature_template` SET
 `spell4` = 54459, -- Maul
 `spell5` = 54458, -- Smash
 `spell6` = 54460, -- Charge
 `speed_run` = 3   -- old: 1,14286
WHERE `entry` = 29918;
-- The Warm-Up
-- Give KillCredit at 10% HP
REPLACE INTO `creature_ai_scripts` VALUES (
-- id, creature_id
2935201, 29352,
-- event_type EVENT_T_HP, event_inverse_phase_mask, event_chance, event_flags EFLAG_REPEATABLE
2, 0, 100, 1,
-- event_param1 HPMax%, event_param2 HPMin%, event_param3 RepeatMin, event_param4 RepeatMax
10, 1, 1, 1,
-- action1_type ACTION_T_KILLED_MONSTER, action1_param1 Creature-ID, action1_param2 Target-Type TARGET_T_ACTION_INVOKER, action1_param3,
33, 30221, 1, 0,
-- action2_type, action2_param1, action2_param2, action2_param3
24, 0, 0, 0,
-- action3_type, action3_param1, action3_param2, action3_param3
0, 0, 0, 0,
-- comment
'Storm Peaks: The Warm-Up - KillCredit for Kirgaraak at 10% HP');
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` = 29352;
-- Prequest  12924
UPDATE `quest_template` SET `NextQuestId` = 12924, `ExclusiveGroup` = -12915 WHERE `entry` IN (12915, 12956);
--  12886 Start Script
UPDATE `quest_template` SET `StartScript` = 12886 WHERE `entry` = 12886;
DELETE FROM `quest_start_scripts` WHERE `id` = 12886;
INSERT INTO `quest_start_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`)
VALUES
(12886,0,15,55253,0,0,0,0,0,0);
-- Fix Quest: The Last of Her Kind (12983)
-- ScriptNames
UPDATE `creature_template` SET `ScriptName` = 'npc_injured_icemaw' WHERE `entry` = 29563;
UPDATE `creature_template` SET `ScriptName` = 'npc_harnessed_icemaw' WHERE `entry` = 30468;
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 29592;
-- Spawn anvil
DELETE FROM `gameobject` WHERE `id` = 192060;
INSERT INTO `gameobject` (`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`)
VALUES
(192060, 571, 1, 1, 7220.96, -3650.65, 819.39, 2.59262, 0, 0, 0.962565, 0.271053, 300, 0, 1);
DELETE FROM `spell_area` WHERE `spell` = 55858 AND `area` = 4438;
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_start_active`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`)
VALUES 
(55858, 4438, 12924, 1, 0, 0, 0, 2, 1);
UPDATE `gossip_menu_option` SET `option_id` = 3 WHERE `menu_id` = 10281;
-- Spawn Npc
DELETE FROM `creature` WHERE `id`=29563;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(29563, 571, 1, 1, 0, 0, 7315.48, -2050.65, 761.353, 4.04369, 300, 0, 0, 12600, 0, 0, 0, 0, 0, 0);

-- Item 41556 12915 restriccion uso.
UPDATE `conditions` SET `ConditionTypeOrReference` = 8, `ConditionValue1` = 13064 WHERE `SourceEntry` = 41556 AND `SourceGroup` = 29375;
-- Seteo Faction npc Victorious Challenger - Agnetta Tyrsdottar
UPDATE `creature_template` SET `faction_A` = 2109, `faction_H` = 2109 WHERE `entry` IN (30012, 30154);
-- Llevan Montado un npc. (flag dead Eai cuando lo llamas).
UPDATE `creature_template` SET `flags_extra` = 2, `unit_flags` = `unit_flags`|2|256|512 WHERE `entry` = 30175;
-- Spawntime Update Npcs  Overseer Syra - Captive Vrykul - Exhausted Vrykul - Garhal - Agnetta Tyrsdottar - Victorious Challenger - Hyldsmeet Warbear - Kirgaraak
UPDATE `creature` SET `spawntimesecs` = 60 WHERE `id` = 29518;
UPDATE `creature` SET `spawntimesecs` = 120 WHERE `id` IN (29427, 30146, 30147);
UPDATE `creature` SET `spawntimesecs` = 30 WHERE `id` IN (30012, 30154, 30174, 29352);
-- Seteo  Snorri Spells y vechicleID
UPDATE `creature_template` SET `spell1` = 56750, `spell2` = 56753, `VehicleId` = 181 WHERE `entry` = 30124;
-- Update Seething Revenants Faction (Phase 4)
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `flags_extra` = 268435456 WHERE `entry` = 30120;
DELETE FROM `creature_ai_scripts` WHERE `id` = 3012002;
INSERT INTO `creature_ai_scripts` 
VALUES (3012002,30120,6,0,100,1,0,0,0,0,33,30125,6,0,41,5000,0,0,0,0,0,0,'Seething Revenant - Kill Credit');
-- Spell target restriccion
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 56753;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) 
VALUES
(17, 56753, 19, 30120, 0);
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 56753;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) 
VALUES (56753, -56750, 0, 'Snorri - Remove Gather Snow');
-- Hacer a Snorri Clickeable
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` = 30123;
INSERT INTO `npc_spellclick_spells` (`npc_entry`,`spell_id`,`quest_start`,`quest_start_active`,`quest_end`,`cast_flags`,`aura_required`,`aura_forbidden`,`user_type`) 
VALUES (30123, 55957, 12967, 1, 12967, 3, 0, 55962, 0);
-- Phase anvil (phase 4)
DELETE FROM `spell_area` WHERE `spell` = 55858 AND `area` IN (4439,4495);
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_start_active`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`)
VALUES 
(55858, 4439, 12924, 1, 12924, 0, 0, 2, 1),
(55858, 4495, 12924, 1, 12924, 0, 0, 2, 1);
-- Phase anvil and lake area (phase 8)
DELETE FROM `spell_area` WHERE `spell` = 55952 AND `area` IN (4439,4495);
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_start_active`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`)
VALUES 
(55952, 4439, 12924, 0, 0, 0, 0, 2, 1),
(55952, 4495, 12924, 0, 0, 0, 0, 2, 1);
-- Set Tundra Ram phasemask
UPDATE `creature` SET `phaseMask` = 13 WHERE `id` = 29958;
-- spawn njormeld related creatures
DELETE FROM `creature` WHERE `id` IN (30099, 30120, 30123);
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`)
VALUES
(30099, 571, 1, 4, 0, 0, 7175.17, -3514.41, 832.257, 6.16378, 30,  0, 0, 50400, 0, 0, 0, 0, 0, 0),
(30120, 571, 1, 4, 0, 0, 7211.01, -3663.32, 822.932, 1.06182, 30, 10, 0, 12600, 0, 0, 1, 0, 0, 0),
(30120, 571, 1, 4, 0, 0, 7234.67, -3649.07, 823.685, 2.62084, 30, 10, 0, 12600, 0, 0, 1, 0, 0, 0),
(30120, 571, 1, 4, 0, 0, 7240.39, -3678.53, 822.731, 1.94933, 30, 10, 0, 12600, 0, 0, 1, 0, 0, 0),
(30120, 571, 1, 4, 0, 0, 7258.67, -3641.53, 823.251, 3.15098, 30, 10, 0, 12600, 0, 0, 1, 0, 0, 0),
(30120, 571, 1, 4, 0, 0, 7215.29, -3623.86, 823.263, 2.5855,  30, 10, 0, 12600, 0, 0, 1, 0, 0, 0),
(30120, 571, 1, 4, 0, 0, 7199.37, -3638.62, 823.401, 4.25839, 30, 10, 0, 12600, 0, 0, 1, 0, 0, 0),
(30123, 571, 1, 4, 0, 0, 7167.15, -3543.37, 827.69,  6.25175, 30,  0, 0, 50400, 0, 0, 0, 0, 0, 0);
-- Spawn  Nifelem Anvil
DELETE FROM `gameobject` WHERE `id` = 192071;
INSERT INTO `gameobject` (`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`)
VALUES
(192071, 571, 1, 4, 7218.02, -2644.12, 809.814, 5.65174, 0, 0, 0.310502, -0.950573, 300, 0, 1);
-- Anvil questrelations
INSERT IGNORE INTO `gameobject_questrelation` VALUES (192071, 12981);
INSERT IGNORE INTO `gameobject_involvedrelation` VALUES (192071, 12981);
-- "Hot and Cold" (daily)
UPDATE `quest_template` SET `PrevQuestId` = 12967, `NextQuestInChain` = 0 WHERE `entry` = 12981;
-- "The Reckoning" prequests
UPDATE `quest_template` SET `NextQuestId` = 13047, `ExclusiveGroup` = -13005 WHERE `entry` IN (13005, 13035);
-- "Diametrically Opposed"
UPDATE `quest_template` SET `PrevQuestId` = 13047, `NextQuestInChain` = 0 WHERE `entry` = 13109;
-- dead iron giant looking dead
INSERT IGNORE INTO `creature_template_addon` VALUES (29914, 0, 0, 7, 0, 0, NULL);
-- spawntime and phasemask
UPDATE `creature` SET `spawntimesecs` = 120, `phaseMask` = 13 WHERE `guid` IN (116761, 116762, 116763);
-- make ignore aggro
UPDATE `creature_template` SET `flags_extra` = 268435458, `unit_flags` = `unit_flags`|2|256|512 WHERE `entry` = 29914;
DELETE FROM `creature` WHERE `id` = 29914 AND `phaseMask` = 12;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`)
VALUES 
('29914','571','1','12','0','0','7429.23','-3187.91','837.452','0.19249','120','0','0','50400','0','0','0','0','0','0'),
('29914','571','1','12','0','0','7359.28','-3176.45','837.452','1.31954','120','0','0','50400','0','0','0','0','0','0'),
('29914','571','1','12','0','0','7280.03','-3142.9','837.452','4.41243','120','0','0','50400','0','0','0','0','0','0'),
('29914','571','1','12','0','0','7515.58','-3137.05','837.452','5.47429','120','0','0','50400','0','0','0','0','0','0'),
('29914','571','1','12','0','0','7509.03','-3225.78','837.432','2.03661','120','0','0','50400','0','0','0','0','0','0'),
('29914','571','1','12','0','0','7259.32','-3221.84','837.452','1.06349','120','0','0','50400','0','0','0','0','0','0');
-- Quest: hot and cold
DELETE FROM `spell_scripts` WHERE `id` = 56099;
INSERT INTO `spell_scripts` (`id`,`effIndex`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`)
VALUES (56099, 0, 2, 15, 56101, 1, 0, 0, 0, 0, 0);
DELETE FROM `gameobject` WHERE `id` = 192124;
INSERT INTO `gameobject` (`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`)
VALUES 
('192124','571','1','8','7209.07','-3596.18','825.86','1.74533','0','0','0.766046','0.642786','300','0','1'),
('192124','571','1','8','7157.87','-3556.37','828.47','0.45728','0','0','0.226653','0.973976','300','0','1'),
('192124','571','1','8','7190.29','-3485.87','834.72','0.45728','0','0','0.226653','0.973976','300','0','1'),
('192124','571','1','8','7226.67','-3441.92','837.543','1.91341','0','0','0.817297','0.576217','300','0','1'),
('192124','571','1','8','7199.65','-3553.19','828.189','4.97657','0','0','0.607814','-0.794079','300','0','1');
-- Spawn brittle revenants (phase 8, lake area)
DELETE FROM `creature` WHERE `id` = 30160;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) 
VALUES
('30160','571','1','8','0','1831','7220.05','-3272.88','837.452','0.750886','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7452.03','-3081.92','837.452','5.68555','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7278.57','-3198.8','837.452','1.48602','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7266.87','-3109.61','837.451','3.05446','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7325.03','-3154.05','837.453','2.89895','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7385.27','-3100.49','837.457','4.75406','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7380.8','-3160.7','837.452','2.14418','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7432.12','-3202.25','837.452','2.19209','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7429.5','-3137.99','837.452','2.52197','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7493.77','-3132.1','837.465','4.20979','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7478.17','-3179.73','837.471','1.57163','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7528.51','-3189.26','837.448','2.75237','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7333.29','-3114.06','837.452','3.77953','90','10','0','12600','0','0','1','0','0','0'),
('30160','571','1','8','0','1831','7260.9','-3159.63','837.452','6.06896','90','10','0','12600','0','0','1','0','0','0');
-- Spawn seething revenants (phase 8, anvil area)
DELETE FROM `creature` WHERE `id` = 30387;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`)
VALUES
('30387','571','1','8','0','0','7228.41','-3681.01','823.549','1.27883','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7223.79','-3653.33','823.259','2.24487','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7247.47','-3651.63','822.965','3.68215','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7231.88','-3623.4','823.628','5.62444','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7201.15','-3623.22','823.489','5.63229','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7186.92','-3537.98','826.927','1.20658','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7201.47','-3487.47','832.464','4.3843','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7207.03','-3375.13','845.052','4.94489','180','7','0','12600','0','0','1','0','0','0'),
('30387','571','1','8','0','0','7218.05','-3434.39','837.901','1.38016','180','7','0','12600','0','0','1','0','0','0');
-- Quest 12985 "Forging a Head"  http://www.wowhead.com/quest=12985
-- Dead iron giant scriptname
UPDATE `creature_template` SET `unit_flags` = `unit_flags` &~2, `ScriptName` = 'npc_dead_irongiant' WHERE `entry` = 29914;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry` = 56227;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) 
VALUES
(13,56227,18,1,29914);
-- Stormforged ambusher basic loot
DELETE FROM `creature_loot_template` WHERE `entry` = 30208;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`)
VALUES
(30208, 42423, -50, 1, 0, 1, 1),
(30208, 42105, -33, 1, 0, 1, 1),
(30208, 42780,  40, 1, 0, 1, 1),
(30208, 33470,  30, 1, 0, 1, 4),
(30208, 43851,  20, 1, 0, 1, 1);
-- Quest 12987  http://www.wowhead.com/quest=12987 "Mounting Hodir's Helm"
-- Give credit
DELETE FROM `spell_scripts` WHERE `id` = 56278;
INSERT INTO `spell_scripts` (`id`,`effIndex`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) 
VALUES
(56278, 0, 3, 8, 30210, 0, 0, 0, 0, 0, 0);
-- "northern ice spike" in phase 4
UPDATE `gameobject` SET `phaseMask` = 4 WHERE `guid` = 99731;
-- Misc spell_area
-- Lokrila buff 
UPDATE `spell_area` SET `quest_end` = 0 WHERE `area` = 4437 AND `quest_end` = 12983;
-- Apply hibernal cavern phase 4 area aura
DELETE FROM `spell_area` WHERE `spell` = 55858 AND `area` = 4455;
INSERT INTO `spell_area` (`spell`,`area`,`quest_start`,`quest_start_active`,`quest_end`,`aura_spell`,`racemask`,`gender`,`autocast`)
VALUES 
(55858, 4455, 12915, 0, 0, 0, 0, 2, 1);
-- Quest 13006 http://www.wowhead.com/quest=13006 "Polishing the Helm"
-- "polishing the helm" prequest condition
UPDATE `quest_template` SET `PrevQuestId` = 12987 WHERE `entry` = 13006;
-- Helm questrelations
INSERT IGNORE INTO `gameobject_questrelation` VALUES (192080, 13006);
INSERT IGNORE INTO `gameobject_involvedrelation` VALUES (192080, 13006);
-- Spawn helm
DELETE FROM `gameobject` WHERE `id` = 192080;
INSERT INTO `gameobject` (`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`)
VALUES (192080, 571, 1, 4, 7389.34, -2726.2, 875.227, 2.82985, 0, 0, 0.987876, 0.155243, 300, 0, 1);
-- Oil faster respawn
UPDATE `creature` SET `phaseMask` = 4, `spawntimesecs` = 60 WHERE `id` = 30325;
-- Oil loot
UPDATE `creature_template` SET `lootid` = 30325 WHERE `entry` = 30325;
DELETE FROM `creature_loot_template` WHERE `entry` = 30325;
INSERT INTO `creature_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`)
VALUES
(30325, 42640, -100, 1, 0, 1, 1),
(30325, 39551,   80, 1, 0, 1, 1),
(30325, 39552,   20, 1, 0, 1, 1);
-- Quest 13011 http://www.wowhead.com/quest=13011 "Jormuttar is Soo Fat..."
-- item target restriction
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceEntry` = 42732;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) 
VALUES
(18, 42732, 24, 1, 30292);
-- spell 56562 script
DELETE FROM `spell_scripts` WHERE `id` = 56562;
INSERT INTO `spell_scripts` (`id`,`effIndex`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) 
VALUES
(56562, 0, 0, 18,     0, 0, 0, 0, 0, 0, 0),
(56562, 0, 0, 15, 56566, 1, 0, 0, 0, 0, 0);
-- "Icy crater" in phase 4
UPDATE `gameobject` SET `phaseMask` = 4 WHERE `guid` = 99730;
-- Lure jormuttar bunny
UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` = 30366;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 30366;
INSERT INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`)
VALUES
('3036601','30366','1','0','100','0','5000','10000','0','0','12','30340','0','120000','41','1000','0','0','0','0','0','0','Lure Jormuttar');

-- Fix Quest http://www.wowhead.com/quest=13061  Prepare for Glory Spawn Npc faltante
DELETE FROM `creature` WHERE `id` = 29975;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(29975, 571, 1, 1, 0, 0, 6925.96, -1533.25, 836.426, 4.56475, 300, 0, 0, 12175, 0, 0, 0, 0, 0, 0);