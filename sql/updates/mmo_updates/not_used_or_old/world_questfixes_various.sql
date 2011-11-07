-- Questfix for Körper und Herz http://de.wowhead.com/quest=6001

REPLACE INTO `quest_template` VALUES
(6001, 2, -263, 0, 10, 0, 10, 0, 1101, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 5931, 0, 0, 0, 5, 15208, 1, 0, 'Body and Heart', 'Directly east of Auberdine lay many moonkin caves.  The one closest to Auberdine has a Moonkin Stone inside it.  Lunaclaw, the defender of the stone, will face you only when Cenarion Moondust is applied to it.  Lunaclaw possesses within it a strength that you must use as one of the Claw.$B$BYou''ve earned the right to use this Cenarion Moondust, $N... now show me that you''ve earned the right to possess the strength of body and the strength of heart needed as a druid!', 'Use the Cenarion Moondust on the Moonkin Stone of Auberdine to bring forth Lunaclaw.  From there, you must face Lunaclaw and earn the strength of body and heart it possesses.$B$BSpeak with Mathrengyl Bearwalker in Darnassus when you are done.', 'You have finally taken your large step into a much larger world, $N. I sense the teaching of the Great Bear Spirit within you, and I sense that you have received the strength that Lunaclaw possessed.$B$BThere are no further obstacles in your way... let me now teach you what it means to be a $C of the Claw!', NULL, 'Face Lunaclaw and earn the strength of body and heart it possesses.', 'Return to Mathrengyl Bearwalker at Cenarion Enclave in Darnassus.', '', '', '', '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12138, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 609, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 132300, 0, 19179, 112, 21600, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = '12138',`ReqCreatureOrGOCount1` = '1' WHERE `entry` =6002;
REPLACE INTO `creature_template` VALUES
(12138, 0, 0, 0, 12138, 0, 9389, 0, 0, 0, 'Lunaclaw', '', '', 0, 12, 12, 0, 14, 14, 0, 1.14, 1.14286, 1, 0, 13, 19, 0, 5, 1.2, 2000, 2000, 1, 32768, 0, 0, 0, 0, 0, 0, 10, 15, 4, 7, 0, 12138, 0, 0, 0, 0, 0, 0, 0, 0, 3391, 18986, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 'EventAI', 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 11723);
REPLACE INTO `creature_template_addon` VALUES(12138, 0, 0, 0, 4097, 0, NULL);

-- Questfix for Teile des Rätsels http://de.wowhead.com/quest=12926

UPDATE `quest_template` SET `SrcItemId` = '40971',
`SrcItemCount` = '1' WHERE `entry` =12926;

-- Questfix for Verflucht nochmal! http://de.wowhead.com/quest=11712

UPDATE `quest_template` SET `ReqCreatureOrGOId1` = '25814' WHERE `entry` =11712;
UPDATE `quest_template` SET `ReqSpellCast1` = '45980' WHERE `entry` =11712;

-- Questfix for Die geehrten Toten http://de.wowhead.com/quest=11593

UPDATE `creature_template` SET `AIName` = 'EventAI' WHERE `entry` =25351;
REPLACE INTO `creature_ai_scripts` VALUES (2535100, 25351, 8, 0, 100, 0, 45474, -1, 0, 0, 33, 25351, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'q11593 The Honored Dead');
REPLACE INTO `conditions` VALUES (13, 0, 45474, 0, 18, 1, 25351, 0, 0, '', NULL);
UPDATE `quest_template` SET `ReqSpellCast1`=45474 WHERE `entry`=11593;
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = '25351' WHERE `entry` =11593;

-- Questfix for Diesmal habt Ihr Euch wirklich selbst übertroffen http://de.wowhead.com/quest=14096 & http://de.wowhead.com/quest=14142

UPDATE `quest_template` SET `ReqCreatureOrGOId1`='34956', `ReqSpellCast1`='0', `ReqCreatureOrGOId2`='34716' WHERE `entry`='14096';
UPDATE `quest_template` SET `ReqCreatureOrGOId1`='34956', `ReqSpellCast1`='0', `ReqCreatureOrGOId2`='34716' WHERE `entry`='14142'; 

-- Questfix for Der Geschmackstest http://de.wowhead.com/quest=12645

UPDATE `quest_template` SET `ReqSpellCast1`=51962, `ReqSpellCast2`=51962, `ReqSpellCast3`=51962 WHERE `entry`=12645;

-- Questfix for Die Befehle des Schwarzen Ritters http://de.wowhead.com/quest=13663

UPDATE `creature_template` SET `speed_walk`='2',`spell1`='57403',`VehicleId`='200',`RegenHealth`='0',`ScriptName`='vehicle_knight_gryphon' WHERE (`entry`='33519'); 

-- Questfix for Söldner http://de.wowhead.com/quest=255

INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(1178, 0, 1, 1, 1122, 0, -4879.88, -3884.42, 303.659, 2.4162, 300, 0, 0, 417, 0, 0, 0),
(1178, 0, 1, 1, 1122, 0, -4884.67, -3813.78, 304.126, 1.87155, 300, 5, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4902.35, -3910.26, 300.735, 1.74533, 300, 0, 0, 417, 0, 0, 0),
(1178, 0, 1, 1, 1122, 0, -4948.55, -3748.52, 318.755, 0.36412, 300, 5, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4874.72, -3906.17, 302.602, 3.10669, 300, 0, 0, 417, 0, 0, 0),
(1178, 0, 1, 1, 1122, 0, -4880.8, -3747.4, 312.49, 0.888788, 300, 10, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4915.2, -3986.09, 296.665, 0.226805, 300, 3, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4963.93, -4022.95, 301.358, 3.79229, 300, 3, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -5082.26, -4080.96, 311.608, 1.07957, 300, 10, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4981.38, -3914.15, 305.894, 4.63279, 300, 3, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4958.24, -3946.78, 301.179, 3.28957, 300, 5, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -4957.42, -3915.39, 305.241, 1.86585, 300, 10, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -5111, -3845.37, 320.988, 4.76514, 300, 5, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -5009.46, -3890.06, 309.664, 2.09648, 300, 10, 0, 417, 0, 0, 1),
(1178, 0, 1, 1, 1122, 0, -5072.94, -3885.23, 321.944, 0.028778, 300, 5, 0, 417, 0, 0, 1);

-- Npc Lieutenant Commander Thalvos
UPDATE `creature_ai_scripts` SET `action1_param1` = 33900 WHERE `id` = 1697800 AND `creature_id` = 16978 -- Shroud of Death 10848