UPDATE gameobject_template SET size=2.151325 WHERE entry=195451;
UPDATE gameobject_template SET size=2.151325 WHERE entry=195452;
UPDATE gameobject_template SET size=3.163336 WHERE entry=195223;
UPDATE creature_template SET speed_run=1.142857 WHERE entry=36154;
UPDATE creature_template SET speed_run=1.142857 WHERE entry=36169;

-- Canon de la canonniиre de l'Alliance
-- http://fr.wowhead.com/npc=34929
UPDATE `creature_template` SET `spell1`=69495,`VehicleId`='452' WHERE `entry` =34929;

-- Canon de la canonniиre de la Horde
-- http://fr.wowhead.com/npc=34935
UPDATE `creature_template` SET `spell1`=68825,`VehicleId`='453' WHERE `entry` =34935;

-- Canon du donjon
-- http://fr.wowhead.com/npc=34944
UPDATE `creature_template` SET `VehicleId`=160,`spell1`=67452,`spell2`='68169' WHERE `entry` =34944;

-- Catapulte
-- http://fr.wowhead.com/npc=34793
UPDATE `creature_template` SET `VehicleId`=438,`spell1`=66218,`spell2`=66296 WHERE `entry`=34793;

-- Dйmolisseur
-- http://fr.wowhead.com/npc=34775
UPDATE `creature_template` SET `VehicleId`='509',`spell1`='67442',`spell2`='68068' WHERE `entry` =34775;

-- Engin de siиge
-- http://fr.wowhead.com/npc=34776
UPDATE `creature_template` SET `VehicleId`=447,`spell1`=67816,`spell2`=69502 WHERE `entry`=34776;

-- Engin de siиge
-- http://fr.wowhead.com/npc=35069
UPDATE `creature_template` SET `VehicleId`=436,`spell1`=67816,`spell2`=69502 WHERE `entry`=35069;

-- Lanceur de glaive
-- http://fr.wowhead.com/npc=34802
UPDATE `creature_template` SET `VehicleId`=447,`spell1`=68827,`spell2`=69515 WHERE `entry`=34802;

-- Lanceur de glaive
-- http://fr.wowhead.com/npc=35273
UPDATE `creature_template` SET `VehicleId`=447,`spell1`=68827,`spell2`=69515 WHERE `entry`=35273;

-- Tourelle de flammes
-- http://fr.wowhead.com/npc=34778

UPDATE `creature_template` SET `spell1`='68832' WHERE `entry` =34778;
-- Tourelle de flammes
-- http://fr.wowhead.com/npc=36356
UPDATE `creature_template` SET `spell1`='68832' WHERE `entry` =36356;

-- Tourelle de siиge
-- http://fr.wowhead.com/npc=34777
UPDATE `creature_template` SET `spell1`=67462,`spell2`=69505 WHERE `entry`=34777;

-- Tourelle de siиge
-- http://fr.wowhead.com/npc=36355
UPDATE `creature_template` SET `spell1`=67462,`spell2`=69505 WHERE `entry`=36355;

-- Catapult speed
UPDATE creature_template SET `speed_run`=2.428571,`speed_walk`=2.8 WHERE `entry`=34793;

-- Overlord Agmar &   script
UPDATE creature_template SET `ScriptName`='boss_isle_of_conquest' WHERE `entry` IN (34924,34922);
	
INSERT IGNORE `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(12039, 'le donjon de l''alliance', NULL, 'le donjon de l''alliance', NULL, NULL, NULL, NULL, NULL, NULL),
(12038, 'le donjon de la Horde', NULL, 'le donjon de la Horde', NULL, NULL, NULL, NULL, NULL, NULL),
(12037, 'L''Alliance a pris le donjon de la Horde !', NULL, 'L''Alliance a pris le donjon de la Horde !', NULL, NULL, NULL, NULL, NULL, NULL),
(12036, 'La Horde a pris le donjon de l''Alliance !', NULL, 'La Horde a pris le donjon de l''Alliance !', NULL, NULL, NULL, NULL, NULL, NULL),
(12035, '%s l''emporte !', NULL, '%s l''emporte !', NULL, NULL, NULL, NULL, NULL, NULL),
(12034, '%s a attaquй le donjon de la Horde !', NULL, '%s a attaquй le donjon de la Horde !', NULL, NULL, NULL, NULL, NULL, NULL),
(12033, '%s a attaquй le donjon de l''Alliance !', NULL, '%s a attaquй le donjon de l''Alliance !', NULL, NULL, NULL, NULL, NULL, NULL),
(12032, 'La porte ouest du donjon de la Horde est dйtruite !', NULL, 'La porte ouest du donjon de la Horde est dйtruite !', NULL, NULL, NULL, NULL, NULL, NULL),
(12031, 'La porte est du donjon de la Horde est dйtruite !', NULL, 'La porte est du donjon de la Horde est dйtruite !', NULL, NULL, NULL, NULL, NULL, NULL),
(12030, 'La porte sud du donjon de la Horde est dйtruite !', NULL, 'La porte sud du donjon de la Horde est dйtruite !', NULL, NULL, NULL, NULL, NULL, NULL),
(12029, 'La porte ouest du donjon de l''Alliance est dйtruite !', NULL, 'La porte ouest du donjon de l''Alliance est dйtruite !', NULL, NULL, NULL, NULL, NULL, NULL),
(12028, 'La porte est du donjon de l''Alliance est dйtruite !', NULL, 'La porte est du donjon de l''Alliance est dйtruite !', NULL, NULL, NULL, NULL, NULL, NULL),
(12027, 'La porte nord du donjon de l''Alliance est dйtruite !', NULL, 'La porte nord du donjon de l''Alliance est dйtruite !', NULL, NULL, NULL, NULL, NULL, NULL),
(12026, 'The battle will begin in 15 seconds!', NULL, 'La bataille commencera dans 15 secondes.', NULL, NULL, NULL, NULL, NULL, NULL),
(12025, '$n has assaulted the %s', '', '$n a attaquй %s !', '', '', '', '', '', ''),
(12024, '$n has defended the %s', '', '$n a dйfendu %s', '', '', '', '', '', ''),
(12023, '$n claims the %s! If left unchallenged, the %s will control it in 1 minute!', NULL, '$n а attaquй %s! Si rien n\'est fait, %s le contrфlera dans 1 minute!', NULL, NULL, NULL, NULL, NULL, NULL),
(12022, 'Alliance', NULL, 'L''Alliance', NULL, NULL, NULL, NULL, NULL, NULL),
(12021, 'Horde', NULL, 'La Horde', NULL, NULL, NULL, NULL, NULL, NULL),
(12020, 'The %s has taken the %s', NULL, '%s a pris %s', NULL, NULL, NULL, NULL, NULL, NULL),
(12019, 'Workshop', NULL, 'l''atelier', NULL, NULL, NULL, NULL, NULL, NULL),
(12018, 'Docks', NULL, 'les docks', NULL, NULL, NULL, NULL, NULL, NULL),
(12017, 'Refinery', NULL, 'la raffinerie', NULL, NULL, NULL, NULL, NULL, NULL),
(12016, 'Quarry', NULL, 'la carriиre', NULL, NULL, NULL, NULL, NULL, NULL),
(12015, 'Hangar', NULL, 'le hangar', NULL, NULL, NULL, NULL, NULL, NULL),
(12014, 'The battle has begun!', NULL, 'Que la bataille commence !', NULL, NULL, NULL, NULL, NULL, NULL),
(12013, 'The battle will begin in 30 seconds!', NULL, 'La bataille commencera dans 30 secondes.', NULL, NULL, NULL, NULL, NULL, NULL),
(12012, 'The battle will begin in one minute!', NULL, 'La bataille commencera dans 1 minute.', NULL, NULL, NULL, NULL, NULL, NULL),
(12011, 'The battle will begin in two minutes!', NULL, 'La bataille commencera dans 2 minutes.', NULL, NULL, NULL, NULL, NULL, NULL);

INSERT IGNORE `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(66548, 66550, 0, 'Isle of Conquest (OUT>IN)'),
(66549, 66550, 0, 'Isle of Conquest (IN>OUT)'),
(66550, -66549, 2, 'Isle of Conquest Teleport (OUT>IN) Debuff limit'),
(66550, -66548, 2, 'Isle of Conquest Teleport (IN>OUT) Debuff limit');


-- Alliance Gunship Cannon
-- http://www.wowhead.com/npc=34929
UPDATE `creature_template` SET `spell1`=66518 WHERE `entry` =34929;
UPDATE `creature_template` SET `spell1`=69495 WHERE `entry` =35410;

-- Horde Gunship Cannon
-- http://www.wowhead.com/npc=34935
-- непонятно почему `VehicleId`='453' пока убрал
-- спелл 66529 может быть кривой
UPDATE `creature_template` SET `spell1`=66529 WHERE `entry` =34935;
UPDATE `creature_template` SET `spell1`=68825 WHERE `entry` =35427;

-- Keep Cannon
-- http://www.wowhead.com/npc=34944
-- непонятно почему `VehicleId`=160 пока убрал
UPDATE `creature_template` SET `spell1`=68170,`spell2`='66541' WHERE `entry` =34944;
UPDATE `creature_template` SET `spell1`=67452,`spell2`='68169' WHERE `entry` =35429;

-- Catapult
-- http://www.wowhead.com/npc=34793
-- `VehicleId`=438 убрал
-- на вовхеде нет героик спелов
UPDATE `creature_template` SET `spell1`=66218,`spell2`=66296 WHERE `entry`=34793;
UPDATE `creature_template` SET `spell1`=66218,`spell2`=66296 WHERE `entry`=35413;

-- Demolisher
-- http://www.wowhead.com/npc=34775
-- убрал `VehicleId`='509'
UPDATE `creature_template` SET `spell1`='67440',`spell2`='67441' WHERE `entry` =34775;
UPDATE `creature_template` SET `spell1`='68068',`spell2`='67442' WHERE `entry` =35415;

-- Siege Engine
-- http://www.wowhead.com/npc=34776
-- `VehicleId`=447 убрал
UPDATE `creature_template` SET `spell1`=67796,`spell2`=67797 WHERE `entry`=34776;
UPDATE `creature_template` SET `spell1`=67816,`spell2`=69502 WHERE `entry`=35431;

-- Siege Engine
-- http://www.wowhead.com/npc=35069
-- `VehicleId`=436, убрал 
UPDATE `creature_template` SET `spell1`=67796,`spell2`=67797 WHERE `entry`=35069;
UPDATE `creature_template` SET `spell1`=67816,`spell2`=69502 WHERE `entry`=35433;

-- Glaive Thrower
-- http://www.wowhead.com/npc=34802
-- `VehicleId`=447 убрал
UPDATE `creature_template` SET `spell1`=66456,`spell2`=67195 WHERE `entry`=34802;
UPDATE `creature_template` SET `spell1`=68827,`spell2`=69515 WHERE `entry`=35419;

-- Glaive Thrower
-- http://www.wowhead.com/npc=35273
-- `VehicleId`=447, убрал
UPDATE `creature_template` SET `spell1`=67034,`spell2`=67195 WHERE `entry`=35273;
UPDATE `creature_template` SET `spell1`=68826,`spell2`=69515 WHERE `entry`=35421;

-- Flame Turret
-- http://www.wowhead.com/npc=34778
-- нет героик спела
UPDATE `creature_template` SET `spell1`='68832' WHERE `entry` =34778;
UPDATE `creature_template` SET `spell1`='68832' WHERE `entry` =35417;

-- Flame Turret
-- http://www.wowhead.com/npc=36356
-- нет героик спела
UPDATE `creature_template` SET `spell1`='68832' WHERE `entry` =36356;
UPDATE `creature_template` SET `spell1`='68832' WHERE `entry` =36358;

-- Siege Turret
-- http://www.wowhead.com/npc=34777
-- нет героик спела
UPDATE `creature_template` SET `spell1`=67462,`spell2`=69505 WHERE `entry`=34777;
UPDATE `creature_template` SET `spell1`=67462,`spell2`=69505 WHERE `entry`=35436;

-- Siege Turret
-- http://www.wowhead.com/npc=36355
-- нет героик спела
UPDATE `creature_template` SET `spell1`=67462,`spell2`=69505 WHERE `entry`=36355;
UPDATE `creature_template` SET `spell1`=67462,`spell2`=69505 WHERE `entry`=36357;

DELETE FROM `creature` WHERE `guid` IN (250194,250214,250212,250210,250208,250206,250203,250198,250196,250182,250182,250180,250186);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(250194, 23472, 628, 3, 1, 0, 0, 1139.92, -780.212, 48.6757, 3.16908, 300, 0, 0, 4120, 0, 0, 0),
(250214, 22515, 628, 3, 1, 0, 0, 323.214, -884.536, 48.9155, 1.59435, 300, 0, 0, 4120, 0, 0, 0),
(250212, 22515, 628, 3, 1, 0, 0, 325.953, -782.401, 48.9163, 4.65741, 300, 0, 0, 4120, 0, 0, 0),
(250210, 22515, 628, 3, 1, 0, 0, 393.258, -858.941, 48.9161, 3.22484, 300, 0, 0, 4120, 0, 0, 0),
(250208, 22515, 628, 3, 1, 0, 0, 1233.08, -840.021, 48.9176, 1.51268, 300, 0, 0, 4120, 0, 0, 0),
(250206, 22515, 628, 3, 1, 0, 0, 1162.9, -746.146, 48.6285, 6.1536, 300, 0, 0, 4120, 0, 0, 0),
(250203, 22515, 628, 3, 1, 0, 0, 1235.15, -689.807, 49.1079, 1.52132, 300, 0, 0, 4120, 0, 0, 0),
(250198, 23472, 628, 3, 1, 0, 0, 1235.61, -863.124, 48.9713, 4.7171, 300, 0, 0, 4120, 0, 0, 0),
(250196, 23472, 628, 3, 1, 0, 0, 1236.6, -663.616, 47.8887, 3.00572, 300, 0, 0, 4120, 0, 0, 0),
(250182, 23472, 628, 3, 1, 0, 0, 324.262, -745.282, 49.4923, 1.55901, 300, 0, 0, 4120, 0, 0, 0),
(250180, 23472, 628, 3, 1, 0, 0, 311.937, -918.894, 48.6856, 4.76736, 300, 0, 0, 4120, 0, 0, 0),
(250186, 23472, 628, 3, 1, 0, 0, 430.61, -857.491, 48.198, 0.074609, 300, 0, 0, 4120, 0, 0, 0);

-- Fix Exploit Isle Of conquest 
UPDATE creature_template SET `ScriptName`='bosses_isle_of_conquest' WHERE `entry` IN (34924,34922);