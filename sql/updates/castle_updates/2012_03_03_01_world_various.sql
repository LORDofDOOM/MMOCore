-- Change these crusader cache spawns to dynamic spawns in core because of LoS
DELETE FROM `gameobject` WHERE `id` IN (195631, 195632, 195633, 195635);

-- Fix count of items dropped by blood prince council / Rat der Blutprinzen in instance / Instanz Icecrown Citadel / ICC / Eiskronenzitadelle
UPDATE `creature_loot_template` SET `maxcount` = 2 WHERE `entry` = 37970 AND `item` = 1;

-- Try to fix NPC 16343 Shadowpine Oracle / Orakel der Waldschattentrolle getting stuck in combat
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 16343;
INSERT INTO `creature_ai_scripts` (id, creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(1634301, 16343, 2, 0, 100, 0, 25, 0, 0, 0, 25, 0, 0, 0, 1, -47, 0, 0, 0, 0, 0, 0, 'Shadowpine Oracle - Flee at 25% HP');

-- Adjust spawntimes for npcs 29602, 30291 Gefr‰ﬂiger Jormungar / Ravenous Jormungar
UPDATE `creature` SET `spawntimesecs` = 300 WHERE `id` IN (29605, 30291);

-- Fix paladin spell 31789 Righteous Defense / Rechtschaffende Verteidigung
DELETE FROM `spell_script_names` WHERE `spell_id` = 31789;
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(31789, 'spell_pal_righteous_defense');

-- Fix quest 13828 Den Nahkampf beherrschen / Mastery Of Melee, closes #783
UPDATE `quest_template` SET `RequiredNpcOrGo2` = 33341, `RequiredNpcOrGoCount2` = 5 WHERE `Id` = 13828;

-- disable argent pony bridle, to prevent it from being lost upon using, closes #1058
DELETE FROM `disables` WHERE `sourceType` = 0 AND `entry` = 67056;
INSERT INTO `disables` (`sourceType`, `entry`, `flags`, `params_0`, `params_1`, `comment`) VALUES
(0, 67056, 1, 0, 0, 'Prevent the loss of the item, that is currently not working');

-- Fix some objects open / close state in instances (bugged doors, Upper Blackrockspire for example)
UPDATE `gameobject` SET `state` = 0 WHERE `guid` IN (87844, 87845, 87854, 87855);

-- Fix questchain The Ring of Blood / Der Ring des Blutes Nagrand Arena quest chain, give credit for external requirement correctly
UPDATE `smart_scripts` SET `target_type` = 16 WHERE `entryorguid` IN (18398, 18399, 18400, 18401, 18402) AND `event_type` = 6 AND `target_type` = 24;

-- Fix NPC 12922 Imp / Wichtel summoned by NPC dealing too much melee damage
-- And add disabled script for creature 3199 Burning Blade Cultist / Kultist der Brennenden Klinge
UPDATE `creature_template` SET `mindmg` = 5, `maxdmg` = 6, `attackpower` = 28 WHERE `entry` = 12922;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 3199;
INSERT INTO `creature_ai_scripts` (id, creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(319901, 3199, 1, 0, 100, 0, 1000, 1000, 0, 0, 11, 11939, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Burning Blade Cultist - Summon Imp on Spawn'),
(319902, 3199, 0, 0, 100, 1, 6000, 13000, 18000, 23000, 11, 11962, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Burning Blade Cultist - Cast Immolate');