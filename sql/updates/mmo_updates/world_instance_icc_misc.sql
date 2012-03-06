-- Fix Marrowgar flame run speed
UPDATE `creature_template` SET `speed_walk` = 0.5, `speed_run` = 0.5 WHERE `entry` = 36672;

-- Fix NPC 36725, Bruthüter der Nerub'ar spawn to bottom
DELETE FROM `creature` WHERE `id` = 36725;
INSERT INTO `creature` (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType, npcflag, unit_flags, dynamicflags) VALUES
(36725, 631, 15, 1, 30885, 0, -307.451, 2219.95, 42.0073, 5.95157, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 30885, 0, -310.799, 2202.42, 42.0142, 0.034907, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 30885, 0, -220.382, 2177.07, 37.9852, 0.855211, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 30885, 0, -249.854, 2219.67, 42.5644, 5.95157, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 30885, 0, -250.613, 2203.8, 42.5645, 0.034907, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 0, 0, -212.81, 2202.31, 35.2336, 0.248573, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 30885, 0, -216.486, 2245.4, 37.9852, 5.18363, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0),
(36725, 631, 15, 1, 30885, 0, -211.29, 2219.42, 35.2336, 0.15708, 86400, 0, 0, 315000, 3994, 0, 0, 0, 0, 0);

-- Fix boss immunities of first two bosses 10 normal
UPDATE `creature_template` SET `mechanic_immune_mask` = 650853247 WHERE `entry` IN (36855, 36612);

-- Scriptname associations for trash
UPDATE `creature_template` SET `ScriptName` = 'npc_NerubarBroodkeeper' WHERE `entry` = 36725;
UPDATE `creature_template` SET `ScriptName` = 'npc_TheDamned' WHERE `entry` = 37011;
UPDATE `creature_template` SET `ScriptName` = 'npc_ServantoftheThrone' WHERE `entry` = 36724;
UPDATE `creature_template` SET `ScriptName` = 'npc_AncientSkeletalSoldier' WHERE `entry` = 37012;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathboundWard' WHERE `entry` = 37007;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerAttedant' WHERE `entry` = 36811;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerDisciple' WHERE `entry` = 36807;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerHighPriest' WHERE `entry` = 36829;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerServant' WHERE `entry` = 36805;
UPDATE `creature_template` SET `ScriptName` = 'npc_DeathspeakerZealot' WHERE `entry` = 36808;

-- Link respawns with bosses in first wing
DELETE FROM `linked_respawn` WHERE `guid` IN (SELECT `guid` FROM `creature` WHERE `map` = 631);
DELETE FROM `linked_respawn` WHERE `linkedGuid` IN (SELECT `guid` FROM `creature` WHERE `map` = 631);
INSERT INTO `linked_respawn` (guid, linkedGuid, linkType)
SELECT `guid`, (SELECT `guid` FROM `creature` WHERE `id` = 36612 LIMIT 1), 0 FROM `creature` WHERE `id` IN (37011, 36724, 37007, 37012, 36725);
INSERT INTO `linked_respawn` (guid, linkedGuid, linkType)
SELECT `guid`, (SELECT `guid` FROM `creature` WHERE `id` = 36855 LIMIT 1), 0 FROM `creature` WHERE `id` IN (36829, 36811, 36808, 36807, 36805);

-- Remove item 50775 Verderbte versilberte Gamaschen / Corrupted Silverplate Leggings from trash loot, should only drop from marrowgar
DELETE FROM `reference_loot_template` WHERE `item` = 50775 AND `entry` = 35071;