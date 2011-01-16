-- Stranglethorn Fishing Extravaganza
-- Anadiendo Textos.
DELETE FROM `script_texts`  WHERE entry IN (-1000356,-1000357,-1000358);
INSERT INTO `script_texts` (entry,content_default,sound,type,language,emote,comment) VALUES
(-1000356, 'Let the Fishing Tournament BEGIN!', 0, 6, 0, 0, 'riggle SAY_START'),
(-1000357, 'We have a winner! $N is the Master Angler!', 0, 6, 0, 0, 'riggle SAY_WINNER'),
(-1000358, 'And the Tastyfish have gone for the week! I will remain for another hour to allow you to turn in your fish!', 0, 6, 0, 0, 'riggle SAY_END');
-- Script Asignado A NPC
UPDATE creature_template SET ScriptName='npc_riggle_bassbait' WHERE entry=15077;
-- Fix Tiempo de evento Fishing Extravaganza
UPDATE `game_event` SET `start_time`='2009-06-14 14:00:00', `occurence`=10080, `length`=120 where entry=15;
-- Spawn Creaturas y go's Involucrados.
DELETE FROM `creature` WHERE `id`IN (15078,15079,15077);
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(54687, 15078, 0, 1, 1, 0, 1711, -14438, 473.661, 15.3137, 3.65362, 25, 0, 0, 3200, 0, 0, 0),
(54688, 15079, 0, 1, 1, 0, 0, -14440, 477.446, 15.25, 3.71802, 25, 0, 0, 2600, 0, 0, 0),
(3849539, 15077, 0, 1, 1, 0, 13, -14439.3, 475.42, 15.892, 3.68503, 25, 0, 0, 3700, 0, 0, 0);
DELETE FROM `gameobject` WHERE `id`=180403;
INSERT INTO `gameobject` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(100391, 180403, 0, 1, 1, -14439.3, 475.42, 15.2791, 3.68503, 0, 0, 0.963311, -0.268388, 25, 0, 1);
-- Clean Antiguos Objects y Creaturas.
DELETE FROM `game_event_creature` WHERE `guid` IN (54687,54688,3849539);
DELETE FROM `game_event_gameobject`WHERE `guid`=100391;
-- Creando Evento.
DELETE FROM `game_event` WHERE `entry`=35;
INSERT INTO `game_event` (`entry`, `start_time`, `end_time`, `occurence`, `length`, `holiday`, `description`) VALUES 
(35, '2009-03-29 14:00:00', '2020-12-31 00:00:00', 10080, 180, 0, 'Fishing Extravaganza Questgivers');
-- Anadiendo Npc's/go's del evento involucrados.
DELETE FROM `game_event_creature` WHERE abs(`event`) = 35;
DELETE FROM `game_event_gameobject` WHERE abs(`event`) = 35;
INSERT INTO `game_event_creature` (`guid`, `event`) VALUES 
(54687,35), 
(54688,35), 
(3849539,35);
INSERT INTO `game_event_gameobject` (`guid`, `event`) VALUES (100391,35);
-- Clean Quest Actuales en DB
DELETE FROM `creature_questrelation` WHERE `quest` IN (8193,8194,8225,8224,8221);
-- Anadiendo Quest a Evento
-- Riggle Bassbait Quest: Master Angler
-- Jang Quest: Apprentice Angler
-- Fishbot 5000 Quest: Rare Fish - Brownell's Blue Striped Racer
-- Fishbot 5000 Quest: Rare Fish - Dezian Queenfish
-- Fishbot 5000 Quest: Rare Fish - Keefer's Angelfish
DELETE FROM `game_event_creature_quest` WHERE `event` IN (15,35);
INSERT INTO `game_event_creature_quest` (`id`,`quest`,`event`) VALUES 
(15077,8193,35), 
(15078,8194,35), 
(15079,8225,35), 
(15079,8224,35), 
(15079,8221,35);