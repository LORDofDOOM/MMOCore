DELETE FROM `creature_template` WHERE `entry`=36672;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES('36672','0','0','0','0','0','1126','11686','0','0','Coldflame','','','0','80','80','0','21','21','0','1','1.14286','1','0','2','2','0','24','1','0','0','1','33554432','8','0','0','0','0','0','1','1','0','10','1024','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','0','0','0','0','0','0','0','0','1','0','0','128','npc_coldflame','12340');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (72705, 69147, 69140);
INSERT INTO `spell_script_names` VALUES
('72705', 'spell_marrowgar_coldflame'),
('69147', 'spell_marrowgar_coldflame_trigger'),
('69140', 'spell_marrowgar_coldflame');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (69075, 70834, 70835, 70836);
INSERT INTO `spell_script_names` VALUES 
(69075, 'spell_marrowgar_bone_storm'),
(70834,    'spell_marrowgar_bone_storm'),
(70835,    'spell_marrowgar_bone_storm'),
(70836,    'spell_marrowgar_bone_storm');

-- UPDATEs for Lady Deathwhisper

UPDATE `creature_template` SET `ScriptName`='boss_lady_deathwhisper' WHERE `entry`=36855;
UPDATE `creature_template` SET `ScriptName`='npc_cult_fanatic' WHERE `entry` IN (37890,38009,38135);
UPDATE `creature_template` SET `ScriptName`='npc_cult_adherent' WHERE `entry` IN(37949,38010,38136);
UPDATE `creature_template` SET `ScriptName`='npc_vengeful_shade' WHERE `entry`=38222;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (70903,71236) AND `ScriptName`='spell_cultist_dark_martyrdom';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(70903,'spell_cultist_dark_martyrdom'),
(71236,'spell_cultist_dark_martyrdom');

-- Unused spell scripts for Festergut's Pungent Blight are removed
DELETE FROM `spell_script_names` WHERE `spell_id` IN (69195, 71219, 73031, 73032);

-- DELETE FROM `creature` WHERE `id` = 37824;
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES('201165','37824','631','15','1','0','0','4388.1','3213.29','408.74','3.83972','86400','0','0','25200','0','0','0','0','0','0');
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES('201508','37824','631','15','1','11686','0','4388.1','3213.29','408.74','3.83972','86400','0','0','25200','0','0','0','0','0','0');
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES('201304','37824','631','15','1','11686','0','4324.28','3215.04','408.705','5.58505','86400','0','0','25200','0','0','0','0','0','0');

-- Correct script names for Blood prince council event IN Icecrown Citadel
UPDATE `creature_template` SET `ScriptName` = 'boss_prince_keleseth_icc' WHERE `entry` = 37972;
UPDATE `creature_template` SET `ScriptName` = 'boss_prince_valanar_icc' WHERE `entry` = 37970;
UPDATE `creature_template` SET `ScriptName` = 'boss_prince_taldaram_icc' WHERE `entry` = 37973;

-- Fix Blood Prince Council immunities
UPDATE `creature_template` SET `mechanic_immune_mask`=650854399 WHERE `entry` IN (37970, 37972, 37973, 38401, 38784, 38785, 38399, 38769, 38770, 38400, 38771, 38772);

-- Blood Prince Council UPDATE
UPDATE `creature_template` SET `ScriptName` = 'npc_shock_vortex' WHERE `entry` = 38422;
DELETE FROM `creature` WHERE `id` = 38557;
UPDATE `creature_template` SET `ScriptName` = 'npc_kinetic_bomb_target' WHERE `entry` = 38458;

-- Blood Queen Lana'thel 
UPDATE `creature_template` SET `ScriptName` = 'boss_blood_queen_lana_thel' WHERE `entry` = 37955;

-- Valithria Dreamwalker
UPDATE `creature_template` SET `unit_flags` = 33554432 WHERE `entry` IN (38186, 38429);
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 38429;
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 37985;
UPDATE `creature_template` SET `ScriptName` = 'npc_dreamcloud_icc' WHERE `entry` = 38421;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (71301, 71977);
INSERT INTO `spell_script_names` VALUES (71301, 'spell_valithria_summon_portal'), (71977, 'spell_valithria_summon_portal');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (70873, 71941);
INSERT INTO `spell_script_names` VALUES (70873, 'spell_valithria_vigor'), (71941, 'spell_valithria_vigor');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (71157, 29306);
INSERT INTO `spell_script_names` VALUES (71157,  'spell_rotface_plagued_zombie_infected_wound'), (29306, 'spell_rotface_plagued_zombie_infected_wound');
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `id` IN (38008, 38752);
-- DELETE FROM `creature` WHERE `id` = 38752 AND NOT (`guid` = 10718);
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `guid` = 10718;
DELETE FROM `creature_template` WHERE `entry` = 38752;
INSERT INTO `creature_template` VALUES (38752, 0, 0, 0, 0, 0, 26623, 0, 0, 0, 'Green Dragon Combat Trigger', '', '', 0, 80, 
80, 2, 16, 16, 0, 1, 1.14286, 1, 1, 1, 2, 0, 1, 1, 2000, 2000, 1, 32832, 8, 0, 0, 0, 0, 0, 1, 2, 126, 7, 8, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, '', 0, 3, 1.35, 1, 1, 0, 0, 0, 0, 0, 0, 0,  0, 1, 358, 803160063, 0, 'npc_icc_combat_stalker', 12340);
REPLACE INTO `spell_script_names` VALUES (70766, 'spell_dream_state');

-- Valithria Dreamwalker UPDATEs
UPDATE `creature_template` SET `ScriptName` = 'npc_column_of_frost_icc' WHERE `entry` = 37918;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_gluttonous_abomination' WHERE `entry` = 37886;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_rot_worm' WHERE `entry` = 37907;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (70912, 70913, 70914, 70915, 70916);
INSERT INTO `spell_script_names` VALUES
(70912,'spell_valithria_summon_adds'),
(70913,'spell_valithria_summon_adds'),
(70914,'spell_valithria_summon_adds'),
(70915,'spell_valithria_summon_adds'),
(70916,'spell_valithria_summon_adds');
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_blistering_zombie' WHERE `entry` = 37934;
UPDATE `creature_template` SET `Scriptname` = 'npc_icc_valithria_supressor' WHERE `entry` = 37863;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_risen_archmage' WHERE `entry` = 37868;
UPDATE `creature_template` SET `spell1` = 0, `spell2` = 0, `Ainame` = '' WHERE `entry` IN (37868, 37863, 37934);
UPDATE `creature_template` SET `Ainame` = '' WHERE `entry` IN (37868, 37934, 37886, 37863);
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_blazing_skeleton' WHERE `entry` = 36791;

-- Valithria Dreamwalker UPDATEs
UPDATE `creature_template` SET `ScriptName` = 'npc_column_of_frost_icc' WHERE `entry` = 37918;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_gluttonous_abomination' WHERE `entry` = 37886;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_mana_void' WHERE `entry` = 38068;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_rot_worm' WHERE `entry` = 37907;
DELETE FROM `spell_script_names` WHERE `spell_id` IN (70912, 70913, 70914, 70915, 70916);
INSERT INTO `spell_script_names` VALUES
(70912,'spell_valithria_summon_adds'),
(70913,'spell_valithria_summon_adds'),
(70914,'spell_valithria_summon_adds'),
(70915,'spell_valithria_summon_adds'),
(70916,'spell_valithria_summon_adds');
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_blistering_zombie' WHERE `entry` = 37934;
UPDATE `creature_template` SET `Scriptname` = 'npc_icc_valithria_supressor' WHERE `entry` = 37863;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_risen_archmage' WHERE `entry` = 37868;
UPDATE `creature_template` SET `spell1` = 0, `spell2` = 0, `Ainame` = '' WHERE `entry` IN (37868, 37863, 37934);
UPDATE `creature_template` SET `Ainame` = '' WHERE `entry` IN (37868, 37934, 37886, 37863);
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_valithria_blazing_skeleton' WHERE `entry` = 36791;

-- Sindragosa UPDATEs
-- Rimefang and Spinestalker positions are corrected according to http://www.youtube.com/watch?v=OtprVV-3q1w
-- Creature templates
UPDATE `creature_template` SET `difficulty_entry_1`=38139 WHERE `entry`=37531;
UPDATE `creature_template` SET `difficulty_entry_1`=38151 WHERE `entry`=37532;
UPDATE `creature_template` SET `difficulty_entry_2`=0,`difficulty_entry_3`=0 WHERE `entry`=37533;
UPDATE `creature_template` SET `difficulty_entry_2`=0,`difficulty_entry_3`=0 WHERE `entry`=37534;
UPDATE `creature_template` SET `difficulty_entry_1`=38320,`difficulty_entry_2`=38321,`difficulty_entry_3`=38322 WHERE `entry`=36980;
UPDATE `creature_template` SET `minlevel`=82,`maxlevel`=82,`unit_class`=1,`faction_A`=14,`faction_H`=14,`unit_flags`=`unit_flags`|33554432,`baseattacktime`=2000,`flags_extra`=`flags_extra`|128 WHERE `entry`=38223; -- Icy Blast
UPDATE `creature_template` SET `minlevel`=80,`maxlevel`=80,`unit_class`=1,`faction_A`=2209,`faction_H`=2209,`dynamicflags`=8,`baseattacktime`=2000,`equipment_id`=523,`speed_walk`=1,`speed_run`=1.42857 WHERE `entry` IN (37531,38139); -- Frostwarden Handler
UPDATE `creature_template` SET `minlevel`=80,`maxlevel`=80,`unit_class`=2,`faction_A`=16,`faction_H`=16,`dynamicflags`=8,`baseattacktime`=2000,`speed_run`=1 WHERE `entry` IN (37532,38151); -- Frostwing Whelp
UPDATE `creature_template` SET `minlevel`=82,`maxlevel`=82,`unit_class`=2,`faction_A`=21,`faction_H`=21,`unit_flags`=`unit_flags`|64,`baseattacktime`=2000,`speed_walk`=2,`speed_run`=1.5873,`MovementType`=2,`InhabitType`=5 WHERE `entry` IN (37533,38220); -- Rimefang
UPDATE `creature_template` SET `minlevel`=82,`maxlevel`=82,`unit_class`=2,`faction_A`=21,`faction_H`=21,`unit_flags`=`unit_flags`|64,`baseattacktime`=2000,`speed_walk`=2,`speed_run`=1.5873,`MovementType`=2,`InhabitType`=5 WHERE `entry` IN (37534,38219); -- Spinestalker
UPDATE `creature_template` SET `minlevel`=83,`maxlevel`=83,`unit_class`=1,`faction_A`=2068,`faction_H`=2068,`unit_flags`=0,`baseattacktime`=1500,`speed_walk`=4.8,`speed_run`=4.28571 WHERE `entry` IN (36853,38265,38266,38267); -- Sindragosa
UPDATE `creature_template` SET `minlevel`=80,`maxlevel`=80,`unit_class`=2,`faction_A`=14,`faction_H`=14,`unit_flags`=`unit_flags`|33554944,`baseattacktime`=2000,`flags_extra`=`flags_extra`|128 WHERE `entry`=37186; -- Frost Bomb
UPDATE `creature_template` SET `minlevel`=80,`maxlevel`=80,`unit_class`=1,`faction_A`=14,`faction_H`=14,`baseattacktime`=2000,`speed_run`=1 WHERE `entry` IN (36980,38320,38321,38322); -- Ice Tomb
UPDATE `creature_template` SET `RegenHealth`=0 WHERE `entry` IN (36980,38320,38321,38322); -- Ice Tomb health regen

-- Linked respawns
-- SET @GUID := 151761;
-- SET @SPINESTALKER := 207211;
-- SET @RIMEFANG := 207210;
-- DELETE FROM `linked_respawn` WHERE guid IN (@SPINESTALKER,@RIMEFANG) OR `guid` BETWEEN @GUID+22 AND @GUID+51;
-- INSERT INTO `linked_respawn` (`guid`,`linkedGuid`,`linkType`) VALUES
-- (@SPINESTALKER,@SPINESTALKER,0), -- Spinestalker
-- (@RIMEFANG,@RIMEFANG,0), -- Rimefang
-- (@GUID+22,@SPINESTALKER,0), --  Frostwarden Handler
-- (@GUID+23,@RIMEFANG,0), --  Frostwarden Handler
-- (@GUID+24,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+25,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+26,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+27,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+28,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+29,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+30,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+31,@SPINESTALKER,0), --  Frostwing Whelp
-- @GUID+32,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+33,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+34,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+35,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+36,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+37,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+38,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+39,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+40,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+41,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+42,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+43,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+44,@SPINESTALKER,0), --  Frostwing Whelp
-- (@GUID+45,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+46,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+47,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+48,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+49,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+50,@RIMEFANG,0), --  Frostwing Whelp
-- (@GUID+51,@RIMEFANG,0); --  Frostwing Whelp

-- Creature addon
UPDATE `creature_addon` SET `bytes1`=0 WHERE `guid` IN (@SPINESTALKER,@RIMEFANG);

-- Creature model data
UPDATE `creature_model_info` SET `gender`=1 WHERE `modelid`=30362;

-- Gameobject templates
UPDATE `gameobject_template` SET `flags`=33,`faction`=114 WHERE `entry`=202396; -- Ice Wall
UPDATE `gameobject_template` SET `flags`=32,`faction`=114 WHERE `entry`=201722; -- Ice Block

-- Gameobject spawns
DELETE FROM `gameobject` WHERE `id`=202396;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(121712,202396,631,15,1,4309.686,2491.27441,211.170792,0.209439442,0,0,0,0,120,0,0); -- Ice Wall

-- Conditions
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=70598;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceEntry`,`ConditionTypeOrReference`,`ConditionValue1`,`Comment`) VALUES
(13,70598,18,1, 'Sindragosa''s Fury - player targets');
-- Fragment of the Nightmare's Corruption drops 100% as of patch 3.0.2
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `item` IN (21146,21147,21148,21149);
-- Insert loot for Gently Shaken Gift
DELETE FROM `item_loot_template` WHERE `entry`=21271;
INSERT INTO `item_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(21271,21241,100,1,0,5,5);

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (12822,12996,12972,12989,12758,12955,13049,13060,13102,13133,13134,13135);
INSERT INTO `achievement_criteria_data` (`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
(12822,11,0,0, 'achievement_all_you_can_eat'), -- All You Can Eat (10 player)
(12822,12,0,0, ''), -- All You Can Eat (10 player)
(12996,11,0,0, 'achievement_all_you_can_eat'), -- All You Can Eat (10 player) Heroic
(12996,12,2,0, ''), -- All You Can Eat (10 player) Heroic
(12972,11,0,0, 'achievement_all_you_can_eat'), -- All You Can Eat (25 player)
(12972,12,1,0, ''), -- All You Can Eat (25 player)
(12989,11,0,0, 'achievement_all_you_can_eat'), -- All You Can Eat (25 player) Heroic
(12989,12,3,0, ''), -- All You Can Eat (25 player) Heroic
(12758,12,0,0, ''), -- The Frostwing Halls (10 player) Sindragosa
(12955,12,1,0, ''), -- The Frostwing Halls (25 player) Sindragosa
(13049,12,2,0, ''), -- Heroic: The Frostwing Halls (10 player) Sindragosa
(13060,12,3,0, ''), -- Heroic: The Frostwing Halls (25 player) Sindragosa
(13102,12,0,0, ''), -- Sindragosa kills (Icecrown 10 player)
(13133,12,1,0, ''), -- Sindragosa kills (Icecrown 25 player)
(13134,12,2,0, ''), -- Sindragosa kills (Heroic Icecrown 10 player)
(13135,12,3,0, ''); -- Sindragosa kills (Heroic Icecrown 25 player)
DELETE FROM `areatrigger_scripts` WHERE `entry`=5604;
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES
(5604,'at_sindragosa_lair'),
(5698, 'at_icc_saurfang_portal'),
(5649, 'at_icc_shutdown_traps');

DELETE FROM `creature_text` WHERE `entry`=36853;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(36853,0,0, 'You are fools to have come to this place! The icy winds of Northrend will consume your souls!',1,0,0,0,0,17007, 'Sindragosa - SAY_AGGRO'),
(36853,1,0, 'Suffer, mortals, as your pathetic magic betrays you!',1,0,0,0,0,17014, 'Sindragosa - SAY_UNCHAINED_MAGIC'),
(36853,2,0, '%s prepares to unleash a wave of blistering cold!',3,0,0,0,0,0, 'Sindragosa - EMOTE_WARN_BLISTERING_COLD'),
(36853,3,0, 'Can you feel the cold hand of death upon your heart?',1,0,0,0,0,17013, 'Sindragosa - SAY_BLISTERING_COLD'),
(36853,4,0, 'Aaah! It burns! What sorcery is this?!',1,0,0,0,0,17015, 'Sindragosa - SAY_RESPITE_FOR_A_TORMENTED_SOUL'),
(36853,5,0, 'Your incursion ends here! None shall survive!',1,0,0,0,0,17012, 'Sindragosa - SAY_AIR_PHASE'),
(36853,6,0, 'Now feel my master''s limitless power and despair!',1,0,0,0,0,17016, 'Sindragosa - SAY_PHASE_2'),
(36853,7,0, '%s fires a frozen orb towards $N!',3,0,0,0,0,0, 'Sindragosa - EMOTE_WARN_FROZEN_ORB'),
(36853,8,0, 'Perish!',1,0,0,0,0,17008, 'Sindragosa - SAY_KILL 1'),
(36853,8,1, 'A flaw of mortality...',1,0,0,0,0,17009, 'Sindragosa - SAY_KILL 2'),
(36853,9,0, 'Enough! I tire of these games!',1,0,0,0,0,17011, 'Sindragosa - SAY_BERSERK'),
(36853,10,0, 'Free...at last...',1,0,0,0,0,17010, 'Sindragosa - SAY_DEATH');
UPDATE `creature_template` SET `ScriptName`='boss_sindragosa' WHERE `entry`=36853;
UPDATE `creature_template` SET `ScriptName`='npc_ice_tomb' WHERE `entry`=36980;
UPDATE `creature_template` SET `ScriptName`='npc_spinestalker' WHERE `entry`=37534;
UPDATE `creature_template` SET `ScriptName`='npc_rimefang_icc' WHERE `entry`=37533;
UPDATE `creature_template` SET `ScriptName`='npc_sindragosa_trash' WHERE `entry` IN (37531,37532);
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (70117,70157);
INSERT INTO `spell_linked_spell` (`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
(70117,70122,1, 'Sindragosa - Icy Grip'),
(70157,69700,2, 'Sindragosa - Ice Tomb resistance');
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_frostwarden_handler_order_whelp';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_frostwarden_handler_focus_fire';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rimefang_icy_blast';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_s_fury';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_unchained_magic';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_instability';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_frost_beacon';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_ice_tomb';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_ice_tomb_dummy';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_ice_tomb_trap';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sindragosa_collision_filter';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(71357,'spell_frostwarden_handler_order_whelp'),
(71350,'spell_frostwarden_handler_focus_fire'),
(71376,'spell_rimefang_icy_blast'),
(70598,'spell_sindragosa_s_fury'),
(69762,'spell_sindragosa_unchained_magic'),
(69766,'spell_sindragosa_instability'),
(70126,'spell_sindragosa_frost_beacon'),
(69712,'spell_sindragosa_ice_tomb'),
(69675,'spell_sindragosa_ice_tomb_dummy'),
(70157,'spell_sindragosa_ice_tomb_trap'),
(69845,'spell_sindragosa_collision_filter'),
(71053,'spell_sindragosa_collision_filter'),
(71054,'spell_sindragosa_collision_filter'),
(71055,'spell_sindragosa_collision_filter'),
(70127,'spell_sindragosa_collision_filter'),
(72528,'spell_sindragosa_collision_filter'),
(72529,'spell_sindragosa_collision_filter'),
(72530,'spell_sindragosa_collision_filter'),
(70117,'spell_sindragosa_collision_filter');
DELETE FROM `spell_script_names` WHERE `spell_id` = 70122;
INSERT INTO `spell_script_names` VALUES (70122, 'spell_sindragosa_icy_grip');

-- Teleports
DELETE FROM `npc_text` WHERE `id` BETWEEN 800000 AND 800006;
INSERT INTO `npc_text` (`id`, `text0_0`) VALUES
(800000, 'Teleport to the Light\'s Hammer'),
(800001, 'Teleport to the Oratory of the Damned.'), 
(800002, 'Teleport to the Rampart of Skulls.'),
(800003, 'Teleport to the Deathbringer\'s Rise.'),
(800004, 'Teleport to the Upper Spire.'),
(800005, 'Teleport to the Sindragosa\'s Lair'),
(800006, 'Teleport to The Frozen Throne');
DELETE FROM `locales_npc_text` WHERE `entry` BETWEEN 800000 AND 800006;
INSERT INTO `locales_npc_text` (`entry`, `Text0_0_loc8`) VALUES
(800000, 'Молот света'),
(800001, 'Молельня проклятых'),
(800002, 'Черепной вал'),
(800003, 'Подъем смертоносного'),
(800004, 'Шпиль'),
(800005, 'Логово Королевы Льда'),
(800006, 'Ледяной трон');

-- Traps
UPDATE `gameobject_template` SET `ScriptName` = 'go_icc_spirit_alarm' WHERE `entry` IN (201814, 201815, 201816, 201817);
DELETE FROM `creature_addon` WHERE `guid` IN (93950, 101906, 101936, 101937, 101939, 101951, 101995, 101996, 115554, 115555, 115742, 115743);
DELETE FROM `waypoint_scripts` WHERE `dataint` = 38879;
DELETE FROM `waypoint_data` WHERE `action` IN (716, 717, 718, 719, 70, 721, 722, 747, 748, 749, 750);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (70536, 70545, 70546, 70547);
INSERT INTO `spell_script_names` VALUES
(70546, 'spell_icc_spirit_alarm'),
(70536, 'spell_icc_spirit_alarm'),
(70545, 'spell_icc_spirit_alarm'),
(70547, 'spell_icc_spirit_alarm');
REPLACE INTO `spell_script_names` VALUES (70461, 'spell_coldflame_trap');
UPDATE `gameobject_template` SET `ScriptName` = 'go_icc_plagueworks_valve' WHERE `entry` IN (201615, 201616);

DELETE FROM `spell_script_names` WHERE `spell_id` IN (70536, 70545, 70546, 70547);
INSERT INTO `spell_script_names` VALUES
(70546, 'spell_icc_spirit_alarm'),
(70536, 'spell_icc_spirit_alarm'),
(70545, 'spell_icc_spirit_alarm'),
(70547, 'spell_icc_spirit_alarm');
REPLACE INTO `spell_script_names` VALUES (70461, 'spell_coldflame_trap');
UPDATE `gameobject_template` SET `ScriptName` = 'go_icc_plagueworks_valve' WHERE `entry` IN (201615, 201616);
-- The Lich King
DELETE FROM `areatrigger_teleport` WHERE `id` = 5718;
UPDATE `creature_template` SET `ScriptName` = 'npc_shambling_horror_icc' WHERE `entry` = 37698;
UPDATE `creature_template` SET `ScriptName` = 'npc_raging_spirit_icc' WHERE `entry` = 36701;
REPLACE INTO `spell_script_names` VALUES
(72754, 'spell_lich_king_defile'),
(72429, 'spell_lich_king_tirion_mass_resurrection'),
(74115, 'spell_lich_king_pain_and_suffering');

-- Blood Prince entries FROM TrinityCore
DELETE FROM `creature_template` WHERE `entry` IN (37970, 37972, 37973, 38401, 38784, 38785, 38399, 38769, 38770, 38400, 38771, 38772);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('37970','38401','38784','38785','0','0','30858','0','0','0','Prince Valanar','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','37970','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','405','1','1','0','0','0','0','0','0','0','150','0','10092','0','1','boss_prince_valanar_icc','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('37972','38399','38769','38770','0','0','30857','0','0','0','Prince Keleseth','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','405','1','1','0','0','0','0','0','0','0','150','0','10077','0','1','boss_prince_keleseth_icc','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('37973','38400','38771','38772','0','0','30856','0','0','0','Prince Taldaram','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','405','1','1','0','0','0','0','0','0','0','150','0','10091','0','1','boss_prince_taldaram_icc','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38399','0','0','0','0','0','30857','0','0','0','Prince Keleseth (1)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','70','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1620','1','1','0','0','0','0','0','0','0','150','0','10077','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38400','0','0','0','0','0','30856','0','0','0','Prince Taldaram (1)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','70','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1620','1','1','0','0','0','0','0','0','0','150','0','10091','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38401','0','0','0','0','0','30858','0','0','0','Prince Valanar (1)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','2000','2000','1','536904000','8','0','0','0','0','0','371','535','100','6','108','38401','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1620','1','1','0','0','0','0','0','0','0','150','0','10092','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38769','0','0','0','0','0','30857','0','0','0','Prince Keleseth (2)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','546.75','1','1','0','0','0','0','0','0','0','150','0','10077','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38770','0','0','0','0','0','30857','0','0','0','Prince Keleseth (3)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','2187','1','1','0','0','0','0','0','0','0','150','0','10077','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38771','0','0','0','0','0','30856','0','0','0','Prince Taldaram (2)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','546.75','1','1','0','0','0','0','0','0','0','150','0','10091','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38772','0','0','0','0','0','30856','0','0','0','Prince Taldaram (3)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','0','0','1','536904000','8','0','0','0','0','0','371','535','135','6','108','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','2187','1','1','0','0','0','0','0','0','0','150','0','10091','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38784','0','0','0','0','0','30858','0','0','0','Prince Valanar (2)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','2000','2000','1','536904000','8','0','0','0','0','0','371','535','100','6','108','38784','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','546.75','1','1','0','0','0','0','0','0','0','150','0','10092','0','1','','12340');
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `Ainame`, `MovementType`, `inhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('38785','0','0','0','0','0','30858','0','0','0','Prince Valanar (3)','','','0','83','83','2','16','16','0','1.6','1.42857','1','3','509','683','0','805','35','2000','2000','1','536904000','8','0','0','0','0','0','371','535','100','6','108','38785','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','2187','1','1','0','0','0','0','0','0','0','150','0','10092','0','1','','12340');

-- DELETE FROM `creature` WHERE `id` IN (37970, 37972, 37973);
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) values('201577','37970','631','15','1','0','0','4680.29','2769.24','364.17','3.14159','86400','0','0','1','0','0','0','0','0','0');
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) values('201493','37972','631','15','1','0','0','4682.73','2783.42','364.17','3.14159','86400','0','0','1','0','0','0','0','0','0');
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) values('201286','37973','631','15','1','0','0','4682.89','2755.11','364.17','3.14159','86400','0','0','1','0','0','0','0','0','0');

-- Implement equipment_id fixes
UPDATE `creature_template` set `equipment_id` = 0 WHERE `Entry` IN (37970,37972,37973,38399,38400,38401,38769,38770,38771,38772,38784,38785); 
-- DELETE FROM `creature_addon` WHERE `GUID` IN (82289,88387,95000,138237,13828,138239,138287,138288,138289); 
-- DELETE FROM `conditions` WHERE `SourceEntry` IN (62912,62910,62297,62907,65667,63475,62549,62546);

-- Icecrown Citadel trash
UPDATE `creature_template` SET `ScriptName` = 'npc_deathspeaker_high_priest' WHERE `entry` = 36829;
UPDATE `creature_template` SET `ScriptName` = 'npc_val_kyr_herald' WHERE `entry` = 37098;
UPDATE `creature_template` SET `ScriptName` = 'npc_severed_essence' WHERE `entry` = 38410;
REPLACE INTO `spell_script_names` VALUES (69483, 'spell_icc_dark_reckoning');
UPDATE `creature_template` SET `ScriptName` = 'npc_the_damned' WHERE `entry` = 37011;
UPDATE `creature_template` SET `ScriptName` = 'npc_servant_of_the_throne' WHERE `entry` = 36724;
UPDATE `creature_template` SET `ScriptName` = 'npc_blighted_abomination' WHERE `entry` = 37022;
UPDATE `creature_template` SET `ScriptName` = 'npc_plague_scientist' WHERE `entry` = 37023;
UPDATE `creature_template` SET `ScriptName` = 'npc_decaying_colossus' WHERE `entry` = 36880;
UPDATE `creature_template` SET `ScriptName` = 'npc_pustulating_horror' WHERE `entry` = 10404;
UPDATE `creature_template` SET `ScriptName` = 'npc_ancient_skeletal_soldier' WHERE `entry` = 37012;
REPLACE INTO `spell_script_names` VALUES (70964, 'spell_valithria_vigor'); -- Just to ensure that auras stack
-- Set immunes on Spinestalker, Rimefang, Stinky, Precious, Decaying Colossus, Sister Svalna, Deathbound Ward
UPDATE `creature_template` SET `mechanic_immune_mask` = 667631615 WHERE `entry` IN (37533, 37534, 38219, 38220, 37217, 38103, 37025, 38064, 36880, 37655, 37126, 38258, 37007, 38031);

-- Putricide Mutated Abomination spell fix for 10h, 25n and 25h modes
UPDATE `creature_template` SET `spell1` = 70360, `spell2`= 72457, `spell3`= 70542, `VehicleId` = 591 WHERE `entry` = 38788;
UPDATE `creature_template` SET `spell1` = 70360, `spell2`= 72875, `spell3`= 70542, `VehicleId` = 591 WHERE `entry` = 38789;
UPDATE `creature_template` SET `spell1` = 70360, `spell2`= 72876, `spell3`= 70542, `VehicleId` = 591 WHERE `entry` = 38790;