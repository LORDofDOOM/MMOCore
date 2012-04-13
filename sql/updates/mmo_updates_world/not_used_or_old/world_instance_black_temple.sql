-- Fix instance / Instanz Black Temple / BT / Der Schwarze Tempel - several issues
DELETE FROM `creature_addon` WHERE `guid` = 84716;
DELETE FROM `waypoint_data` WHERE `id` = 847160;
UPDATE `creature_template` SET `faction_A` = 1866, `faction_H` = 1866 WHERE `entry` = 22990;
UPDATE `creature_template` SET `faction_A` = 1813, `faction_H` = 1813 WHERE `entry` IN (22847, 22849, 22845, 22846);
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 23318;
INSERT INTO `creature_ai_scripts` (id, creature_id, event_type, event_inverse_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action1_type, action1_param1, action1_param2, action1_param3, action2_type, action2_param1, action2_param2, action2_param3, action3_type, action3_param1, action3_param2, action3_param3, comment) VALUES
(2331801, 23318, 2, 0, 100, 3, 15, 0, 20000, 20000, 11, 41177, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Rogue - Cast Eviscerate at 15% HP'),
(2331802, 23318, 0, 0, 100, 3, 15000, 15000, 22000, 22000, 11, 41978, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Rogue - Cast Debilitating Poison');
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `id` IN (22990, 23421);