## Set Emblem of Heroism Badges
SET @lootid_anomalus     = (SELECT lootid FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26763));
SET @lootid_telestra     = (SELECT lootid FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26731));
SET @lootid_keristrasza  = (SELECT lootid FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26723));
SET @lootid_ormorok      = (SELECT lootid FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26794));

UPDATE creature_loot_template SET item = 40752 WHERE item = 47241 AND entry IN (@lootid_anomalus,@lootid_telestra,@lootid_keristrasza,@lootid_ormorok);
UPDATE reference_loot_template SET item = 40752 WHERE item = 47241 AND entry = 35034;

## Set Damage for Commander Kolurg to Commander Stoutbeards Values
SET @mindmg = (SELECT mindmg      FROM creature_template WHERE entry = 26796);
SET @maxdmg = (SELECT maxdmg      FROM creature_template WHERE entry = 26796);
SET @attack = (SELECT attackpower FROM creature_template WHERE entry = 26796);
UPDATE creature_template SET mindmg = @mindmg, maxdmg = @maxdmg, attackpower = @attack WHERE entry = 26798;

SET @mindmg = (SELECT mindmg      FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26796));
SET @maxdmg = (SELECT maxdmg      FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26796));
SET @attack = (SELECT attackpower FROM creature_template WHERE entry = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26796));
SET @kolurg_diff1 = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 26798);
UPDATE creature_template SET mindmg = @mindmg, maxdmg = @maxdmg, attackpower = @attack WHERE entry = @kolurg_diff1;

## First Boss should be interruptable
UPDATE creature_template SET mechanic_immune_mask = mechanic_immune_mask &~ 33554432 WHERE entry IN (26731,30510);

UPDATE creature_template SET AIName = '', ScriptName = 'boss_magus_telestra_arcane' WHERE entry = 26929;

DELETE FROM creature WHERE id IN (26798,26796,27949,27947);
INSERT INTO creature (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(26796,576,2,1,24366,0,424.185,185.37,-35.004,3.263,3600,0,0,337025,0,0,0), -- alli boss hero
(27949,576,1,1,24366,0,424.185,185.37,-35.004,3.263,3600,0,0,337025,0,0,0); -- alli commander non hero

## Remove Keristrasza's Broken Heart from loot Table
DELETE FROM creature_loot_template WHERE item = 43665;

## Spikes at Ormorok unselectable and unattackable
SET @spike_h = (SELECT difficulty_entry_1 FROM creature_template WHERE entry = 27099);
UPDATE creature_template SET unit_flags = unit_flags|2|33554432 WHERE entry IN (27099,@spike_h);

## Change Position of Trashmobs near Ormorok
UPDATE creature SET position_x = '323.541779', position_y = '-240.492249', position_z = '-14.088820', orientation = '2.964224' WHERE guid = '126444';
UPDATE creature SET position_x = '323.179108', position_y = '-242.347137', position_z = '-14.088820', orientation = '2.948516' WHERE guid = '126606';
UPDATE creature SET position_x = '324.616272', position_y = '-234.996307', position_z = '-14.088820', orientation = '3.152720' WHERE guid = '126605';
UPDATE creature SET position_x = '317.004852', position_y = '-237.360901', position_z = '-14.088820', orientation = '3.231260' WHERE guid = '126445';

## Remove Reputation from Horde/Alliance Commander (nonhero) - old: 250
DELETE FROM creature_onkill_reputation WHERE creature_id IN (27947,27949);
