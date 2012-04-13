-- Key Vendor
DELETE FROM creature_template WHERE entry = 900001;
-- insert into `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values
-- ('900001','0','0','0','0','0','17822','0','0','0','MMOwning Schlüsselmeister','Miscellaneous',NULL,'0','80','80','0','35','35','4225','1','1.14286','1','3','500','5000','0','0','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','0','0');

REPLACE into `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values
('900001','0','0','0','0','0','17822','0','0','0','MMOwning Schlüsselmeister','Miscellaneous',NULL,'0','75','75','0','35','35','128','1','1.14286','1','0','0','0','0','0','1','0','0','1','512','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','1');

DELETE FROM npc_vendor WHERE entry = 900001;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`) VALUES 
-- Schlüssel zur sengenden Schlucht
('900001','0','5396','0','0','0'),
-- Werkstattschlüssel
-- ('900001','0','6893','0','0','0'),
-- Der Scharlachrote Schlüssel
('900001','0','7146','0','0','0'),
-- Schlüssel zur Schattenschmiede
('900001','0','11000','0','0','0'),
-- Reliktkastenschlüssel
-- ('900001','0','11078','0','0','0'),
-- Schlüssel zur Stadt
('900001','0','12382','0','0','0'),
-- Silberdietrich
-- ('900001','0','15869','0','0','0'),
-- Golddietrich
-- ('900001','0','15870','0','0','0'),
-- Echtsilberdietrich
-- ('900001','0','15871','0','0','0'),
-- Arkanitdietrich
-- ('900001','0','15872','0','0','0'),
-- Mondsichelschlüssel
('900001','0','18249','0','0','0'),
-- Gordokfesselschlüssel
-- ('900001','0','18250','0','0','0'),
-- Schlüssel für die Gordokhoftür
-- ('900001','0','18266','0','0','0'),
-- Schlüssel für die Gordokinnentür
-- ('900001','0','18268','0','0','0'),
-- Schlüssel für einen Skarabäuskasten
-- ('900001','0','21761','0','0','0'),
-- Schlüssel für einen großen Skarabäuskasten
-- ('900001','0','21762','0','0','0'),
-- Der Schlüssel des Meisters
('900001','0','24490','0','0','0'),
-- Schlüssel des Schattenlabyrinths
('900001','0','27991','0','0','0'),
-- Schlüssel der zerschmetterten Hallen
('900001','0','28395','0','0','0'),
-- Flammengeschmiedeter Schlüssel
('900001','0','30622','0','0','0'),
-- Schlüssel des Kessels
('900001','0','30623','0','0','0'),
-- Schlüssel der Auchenai
('900001','0','30633','0','0','0'),
-- Warpgeschmiedeter Schlüssel
('900001','0','30634','0','0','0'),
-- Schlüssel der Zeit
('900001','0','30635','0','0','0'),
-- Flammengeschmiedeter Schlüssel
('900001','0','30637','0','0','0'),
-- Schlüssel zur Arkatraz
('900001','0','31084','0','0','0'),
-- Schlüssel der Stürme
('900001','0','31704','0','0','0'),
-- Mit Essenz erfüllter Mondstein
('900001','0','32449','0','0','0'),
-- Der Schlüssel zur Violetten Festung
('900001','0','42482','0','0','0'),
-- Titandietrich
-- ('900001','0','43853','0','0','0'),
-- Kobaltdietrich
-- ('900001','0','43854','0','0','0'),
-- Heroischer Schlüssel der fokussierenden Iris
('900001','0','44581','0','0','0'),
-- Schlüssel der fokussierenden Iris
('900001','0','44582','0','0','0'),
-- Schlüssel des Himmlischen Planetariums
('900001','0','45796','0','0','0'),
-- Heroischer Schlüssel des Himmlischen Planetariums
('900001','0','45798','0','0','0');

UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 5396;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 6893;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 7146;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 11000;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 11078;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 12382;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 15869;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 15870;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 15871;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 15872;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 18249;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 18250;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 18266;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 18268;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 21761;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 21762;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 24490;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 27991;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 28395;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 30622;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 30623;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 30633;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 30634;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 30635;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 30637;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 31084;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 31704;
UPDATE item_template SET BuyPrice = '15000000' WHERE entry = 32449;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 42482;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 43853;
-- UPDATE item_template SET BuyPrice = '5000000' WHERE entry = 43854;
UPDATE item_template SET BuyPrice = '25000000' WHERE entry = 44581;
UPDATE item_template SET BuyPrice = '15000000' WHERE entry = 44582;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 45796;
UPDATE item_template SET BuyPrice = '10000000' WHERE entry = 45798;