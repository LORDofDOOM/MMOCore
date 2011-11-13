-- -----------------------------------------------------------------------------------------------------------------------------
-- # Defining what the entries and numbers are. Change if something overwrites something in your database

SET @TELENPC := 300005; -- # (1)

-- -----------------------------------------------------------------------------------------------------------------------------
-- # Delete code (For re running)

DELETE FROM creature_template WHERE entry=@TELENPC;

-- -----------------------------------------------------------------------------------------------------------------------------
-- # TeleNPC

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
(300005, 0, 0, 0, 0, 0, 21572, 0, 0, 0, 'MMOwning Teleporter', '', 'Directions', 0, 83, 83, 0, 35, 35, 3, 1, 1.14286, 1.25, 1, 509, 683, 0, 805, 1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 371, 535, 135, 7, 138936390, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 'npc_teleport', 1);

-- -----------------------------------------------------------------------------------------------------------------------------
-- # TeleNPC spawns

SET @TELENPC := 300005; -- # Must be the same NPC ID!
DELETE FROM creature WHERE ID = @TELENPC ;
ALTER TABLE creature AUTO_INCREMENT=200000;
INSERT INTO creature (id,map,spawnMask,phaseMask,modelid,position_x,position_y,position_z,orientation,spawntimesecs,spawndist,curhealth,curmana) 
VALUES (@TELENPC,0,1,1,0,-13180.5,342.503,43.1936,4.32977,25,0,13700,6540),
(@TELENPC,530,1,1,22951,-3862.69,-11645.8,-137.629,2.38273,25,0,13700,6540),
(@TELENPC,0,1,1,22951,-4918.48,-985.03,501.451,2.03055,25,0,13700,6540),
(@TELENPC,0,1,1,22951,-8845.09,624.828,94.2999,0.44062,25,0,13700,6540),
(@TELENPC,1,1,1,0,1599.25,-4375.85,10.0872,5.23641,25,0,13700,6540),
(@TELENPC,1,1,1,0,-1277.65,72.9751,128.742,5.95567,25,0,13700,6540),
(@TELENPC,0,1,1,0,1637.21,240.132,-43.1034,3.13147,25,0,13700,6540),
(@TELENPC,530,1,1,0,9741.67,-7454.19,13.5572,3.14231,25,0,13700,6540),
(@TELENPC,571,1,1,0,5807.06,506.244,657.576,5.54461,25,0,13700,6540),
(@TELENPC,1,1,1,22951,9866.83,2494.66,1315.88,5.9462,25,0,13700,6540),
(@TELENPC,0,1,1,0,-14279.8,555.014,8.90011,3.97606,25,0,13700,6540),
(@TELENPC,530,1,1,0,-1888.65,5355.88,-12.4279,1.25883,25,0,13700,6540);

-- -----------------------------------------------------------------------------------------------------------------------------
-- # Rune circle spawns

SET @RUNE := 194394; -- # GO ID
DELETE FROM gameobject WHERE ID=@RUNE and guid>'199999';
ALTER TABLE gameobject AUTO_INCREMENT=200000;
INSERT INTO gameobject (id,map,spawnMask,phaseMask,position_x,position_y,position_z,orientation,rotation2,rotation3,spawntimesecs,state) 
VALUES (@RUNE,1,1,1,1601.08,-4378.69,9.9846,2.14362,0.878068,0.478536,25,1),
(@RUNE,0,1,1,-14281.9,552.564,8.90382,0.860144,0.416936,0.908936,25,1),
(@RUNE,0,1,1,-8842.09,626.358,94.0868,3.61363,0.972276,-0.233836,25,1),
(@RUNE,0,1,1,-4919.94,-982.083,501.46,2.03055,0.849626,0.527386,25,1),
(@RUNE,1,1,1,9869.91,2493.58,1315.88,5.9462,0.167696,-0.985839,25,1),
(@RUNE,530,1,1,-3864.92,-11643.7,-137.644,2.38273,0.928875,0.370392,25,1),
(@RUNE,530,1,1,-1887.62,5359.09,-12.4279,4.40435,0.758205,0.652017,25,1),
(@RUNE,571,1,1,5809.55,503.975,657.526,5.54461,0.360952,-0.932584,25,1),
(@RUNE,530,1,1,9738.28,-7454.19,13.5605,3.14231,1,-0.000358625,25,1),
(@RUNE,0,1,1,1633.75,240.167,-43.1034,3.13147,0.999987,0.00506132,25,1),
(@RUNE,0,1,1,-13181.8,339.356,42.9805,1.18013,0.556415,0.830904,25,1),
(@RUNE,1,1,1,-1274.45,71.8601,128.159,2.80623,0.985974,0.166898,25,1);

-- -----------------------------------------------------------------------------------------------------------------------------
-- # Summon effect
-- # If you want the npc to *cast* the spell, use these as values: VALUES (@TELENPC,0,0,0,0,0,'30540 0');

DELETE FROM creature_template_addon WHERE Entry = @TELENPC ;
INSERT INTO creature_template_addon (entry,mount,bytes1,bytes2,emote,path_id,auras) 
VALUES (@TELENPC,0,0,0,0,0,'35766 0');

-- -----------------------------------------------------------------------------------------------------------------------------
-- # TeleNPC Tables

DROP TABLE IF EXISTS `custom_npc_tele_category`;
-- ----------------------------
-- Table structure for custom_npc_tele_category
-- ----------------------------
CREATE TABLE `custom_npc_tele_category` (
  `id` int(6) unsigned NOT NULL default '0',
  `name` varchar(255) NOT NULL default '',
  `flag` tinyint(3) unsigned NOT NULL default '0',
  `data0` bigint(20) unsigned NOT NULL default '0',
  `data1` int(6) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `custom_npc_tele_category` VALUES ('1', '1. Städte (Kosten 5G)', '0', '1', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('2', '1. Städte (Kosten 5G)', '0', '2', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('3', '2. Instanzen 10-40 (Kosten 10G)', '0', '0', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('4', '3. Instanzen 41-69 (Kosten 20G)', '0', '0', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('5', '4. Instanzen 70-80 (Kosten 40G)', '0', '0', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('6', '5. Raids (Kosten 80G)', '0', '0', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('7', '6. Areana (Kosten 50G)', '0', '0', '0');
INSERT INTO `custom_npc_tele_category` VALUES ('8', '7. Shopping', '2', '3', '0');

DROP TABLE IF EXISTS `custom_npc_tele_destination`;
-- ----------------------------
-- Table structure for custom_npc_tele_destination
-- ----------------------------
CREATE TABLE `custom_npc_tele_destination` (
  `id` int(6) unsigned NOT NULL auto_increment,
  `name` char(100) NOT NULL default '',
  `pos_X` float NOT NULL default '0',
  `pos_Y` float NOT NULL default '0',
  `pos_Z` float NOT NULL default '0',
  `map` smallint(5) unsigned NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `level` tinyint(3) unsigned NOT NULL default '0',
  `cost` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=99 DEFAULT CHARSET=utf8;


DROP TABLE IF EXISTS `custom_npc_tele_association`;
-- ----------------------------
-- Table structure for custom_npc_tele_association
-- ----------------------------
CREATE TABLE `custom_npc_tele_association` (
  `cat_id` int(6) unsigned NOT NULL default '0',
  `dest_id` int(6) unsigned NOT NULL default '0',
  PRIMARY KEY  (`cat_id`,`dest_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------

INSERT INTO `custom_npc_tele_destination` VALUES ('1', '01. Flammenschlund (15-16)', '1810.38', '-4408.05', '-18.8377', '1', '5.18594', '8', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '1');
INSERT INTO `custom_npc_tele_destination` VALUES ('2', '02. Höhlen des Wehklagens (17-20)', '-722.53', '-2226.3', '16.94', '1', '2.71', '12', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '2');
INSERT INTO `custom_npc_tele_destination` VALUES ('3', '03. Die Todesminen (17-20)', '-11212', '1658.58', '25.67', '0', '1.45', '12', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '3');
INSERT INTO `custom_npc_tele_destination` VALUES ('4', '04. Burg Schattenfang (18-21)', '-240.11', '1548.83', '76.89', '0', '1.13981', '13', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '4');
INSERT INTO `custom_npc_tele_destination` VALUES ('5', '05. Tiefschwarze Grotte (21-24)', '4249.99', '740.1', '-25.67', '1', '1.34062', '17', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '5');
INSERT INTO `custom_npc_tele_destination` VALUES ('6', '06. Das Verlies (22-25)', '-8774.25', '838.965', '91.9284', '0', '0.689341', '18', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '6');
INSERT INTO `custom_npc_tele_destination` VALUES ('7', '07. Kral der Klingenhauer (24-27)', '-4484.04', '-1739.4', '86.47', '1', '1.23', '16', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '7');
INSERT INTO `custom_npc_tele_destination` VALUES ('8', '08. Gnomeregan (25-28)', '-5163.54', '925.42', '257.17', '0', '1.57423', '20', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '8');
INSERT INTO `custom_npc_tele_destination` VALUES ('9', '09. Das scharlachrote Kloster: Friedhof (26-36)', '2913.01', '-802.75', '160.33', '0', '0.339072', '25', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '9');
INSERT INTO `custom_npc_tele_destination` VALUES ('10', '10. Das scharlachrote Kloster: Bibliothek (29-39)', '2872.08', '-820.06', '160.33', '0', '3.46299', '26', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '10');
INSERT INTO `custom_npc_tele_destination` VALUES ('11', '11. Hügel der Klingenhauer (34-37)', '-4645.08', '-2470.85', '85.53', '1', '4.39', '28', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '11');
INSERT INTO `custom_npc_tele_destination` VALUES ('12', '12. Das scharlachrote Kloster: Waffenkammer (32-42)', '2885.54', '-825.1', '160.33', '0', '5.12803', '28', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '12');
INSERT INTO `custom_npc_tele_destination` VALUES ('13', '13. Das scharlachrote Kloster: Kathedrale (35-45)', '2907.48', '-816.101', '160.33', '0', '5.0652', '32', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '13');
INSERT INTO `custom_npc_tele_destination` VALUES ('14', '14. Uldaman (37-40)', '-6119.7', '-2957.3', '204.11', '0', '0.03', '32', '100000');
INSERT INTO `custom_npc_tele_association` VALUES ('3', '14');
INSERT INTO `custom_npc_tele_destination` VALUES ('15', '01. Zul\'Farrak (43-46)', '-6839.39', '-2911.03', '8.87', '1', '0.41', '37', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '15');
INSERT INTO `custom_npc_tele_destination` VALUES ('16', '02. Maraudon (45-48)', '-1433.33', '2955.34', '96.21', '1', '4.82', '37', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '16');
INSERT INTO `custom_npc_tele_destination` VALUES ('17', '03. Versunkener Tempel (47-50)', '-10346.9', '-3851.9', '-43.41', '0', '6.09', '42', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '17');
INSERT INTO `custom_npc_tele_destination` VALUES ('18', '04. Schwarzfelstiefen (53-56)', '-7179.33', '-918.7', '165.49', '0', '4.73078', '43', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '18');
INSERT INTO `custom_npc_tele_destination` VALUES ('19', '05. Düsterbruch: Ost (55-60)', '-3981.41', '781.85', '161.004', '1', '4.6741', '50', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '19');
INSERT INTO `custom_npc_tele_destination` VALUES ('20', '06. Düsterbruch: West (57-60)', '-3829.17', '1250.51', '160.22', '1', '3.09938', '52', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '20');
INSERT INTO `custom_npc_tele_destination` VALUES ('21', '07. Düsterbruch: Nord (57-60)', '-3520.64', '1077.71', '161.13', '1', '4.68176', '52', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '21');
INSERT INTO `custom_npc_tele_destination` VALUES ('22', '08. Stratholme (55-65)', '3263.54', '-3379.46', '143.59', '0', '0', '51', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '22');
INSERT INTO `custom_npc_tele_destination` VALUES ('23', '09. Scholomance (55-65)', '1219.01', '-2604.66', '85.61', '0', '0.5', '51', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '23');
INSERT INTO `custom_npc_tele_destination` VALUES ('24', '10. Höllenfeuerzitadelle: Höllenfeuerbollwerk (59-62)', '-359.69', '3069.36', '-15.1135', '530', '1.8637', '55', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '24');
INSERT INTO `custom_npc_tele_destination` VALUES ('25', '11. Höllenfeuerzitadelle: Der Blutkessel (61-63)', '-300.137', '3160.08', '31.6914', '530', '2.25967', '56', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '25');
INSERT INTO `custom_npc_tele_destination` VALUES ('26', '12. Der Echsenkessel: Die Sklavenunterkünfte (62-64)', '721.256', '7008.33', '-73.4791', '530', '0.450804', '57', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '26');
INSERT INTO `custom_npc_tele_destination` VALUES ('27', '13. Der Echsenkessel: Der Tiefensumpf (63-65)', '779.66', '6767.33', '-71.77', '530', '4.7', '58', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '27');
INSERT INTO `custom_npc_tele_destination` VALUES ('28', '14. Auchindoun: Managruft (64-66)', '-3104.17', '4945.52', '-101.507', '530', '0.054137', '59', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '28');
INSERT INTO `custom_npc_tele_destination` VALUES ('29', '15. Auchindoun: Auchenaikrypta (65-67)', '-3362.04', '5209.85', '-101.05', '530', '1.52677', '60', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '29');
INSERT INTO `custom_npc_tele_destination` VALUES ('30', '16. Höhlen der Zeit: Vorgebirge des Alten Hügellands (66-68)', '-8367.11', '-4059.21', '-208.31', '1', '0.108172', '61', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '30');
INSERT INTO `custom_npc_tele_destination` VALUES ('31', '17. Auchindoun: Sethekkhallen (67-68)', '-3361.91', '4678.85', '-101.048', '530', '4.68897', '62', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '31');
INSERT INTO `custom_npc_tele_destination` VALUES ('32', '18. Der Echsenkessel: Die Dampfkammer (67-75)', '815.701', '6928.7', '-80.0444', '530', '1.46476', '63', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '32');
INSERT INTO `custom_npc_tele_destination` VALUES ('33', '19. Auchindoun: Schattenlabyrinth (67-75)', '-3627.03', '4942.75', '-101.049', '530', '3.12505', '64', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '33');
INSERT INTO `custom_npc_tele_destination` VALUES ('34', '20. Höllenfeuerzitadelle: Die zerschmetterten Hallen (67-75)', '-308.4', '3072.7', '-3.65004', '530', '1.75536', '64', '200000');
INSERT INTO `custom_npc_tele_association` VALUES ('4', '34');
INSERT INTO `custom_npc_tele_destination` VALUES ('35', '01. Höhlen der Zeit: Der schwarze Morast (68-75)', '-8752.98', '-4205.76', '-209.5', '1', '2.23792', '65', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '35');
INSERT INTO `custom_npc_tele_destination` VALUES ('36', '02. Festung der Stürme: Die Mechanar (67-75)', '2880.38', '1562.57', '248.88', '530', '3.84869', '65', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '36');
INSERT INTO `custom_npc_tele_destination` VALUES ('37', '03. Festung der Stürme: Die Botanika (67-75)', '3396.79', '1495.11', '179.56', '530', '5.68591', '65', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '37');
INSERT INTO `custom_npc_tele_destination` VALUES ('38', '04. Festung der Stürme: Die Arkatraz (68-75)', '3304.99', '1349.93', '502.29', '530', '4.99889', '65', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '38');
INSERT INTO `custom_npc_tele_destination` VALUES ('39', '05. Terrasse der Magister (68-75)', '12889', '-7320.27', '65.5023', '530', '4.43379', '65', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '39');
INSERT INTO `custom_npc_tele_destination` VALUES ('40', '06. Burg Utgarde: Burg Utgarde (69-72)', '1224.31', '-4862.99', '41.2493', '571', '0.265729', '65', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '40');
INSERT INTO `custom_npc_tele_destination` VALUES ('41', '07. Der Nexus: Der Nexus (71-73)', '3880.71', '6984.41', '73.761', '571', '0.093386', '66', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '41');
INSERT INTO `custom_npc_tele_destination` VALUES ('42', '08. Azjol-Nerub: Azjol-Nerub (72-74)', '3692.49', '2157.36', '34.9193', '571', '2.54456', '67', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '42');
INSERT INTO `custom_npc_tele_destination` VALUES ('43', '09. Azjol-Nerub: Ahn\'kahet: Das Alte Königreich (73-75)', '3647.05', '2045.8', '1.78771', '571', '4.33625', '68', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '43');
INSERT INTO `custom_npc_tele_destination` VALUES ('44', '10. Feste Drak\'Tharon (74-76)', '4774.2', '-2032.44', '229.37', '571', '1.56251', '69', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '44');
INSERT INTO `custom_npc_tele_destination` VALUES ('45', '11. Die Violette Festung (75-77)', '5695.6', '505.81', '652.68', '571', '4.11793', '70', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '45');
INSERT INTO `custom_npc_tele_destination` VALUES ('46', '12. Gundrak (76-78)', '6929.22', '-4443.09', '450.52', '571', '0.748567', '71', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '46');
INSERT INTO `custom_npc_tele_destination` VALUES ('47', '13. Ulduar: Die Hallen des Steins (77-79)', '8923.25', '-1014.92', '1039.61', '571', '1.53846', '72', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '47');
INSERT INTO `custom_npc_tele_destination` VALUES ('48', '14. Ulduar: Die Hallen der Blitze (79-80)', '9128.79', '-1338.15', '1061.4', '571', '5.43122', '75', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '48');
INSERT INTO `custom_npc_tele_destination` VALUES ('49', '15. Der Nexus: Das Oculus (79-80)', '3879.32', '6984.34', '106.32', '571', '3.06023', '75', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '49');
INSERT INTO `custom_npc_tele_destination` VALUES ('50', '16. Höhlen der Zeit: Das Ausmerzen von Stratholme (79-80)', '-8752.93', '-4444.03', '-199.009', '1', '4.34869', '75', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '50');
INSERT INTO `custom_npc_tele_destination` VALUES ('51', '17. Burg Utgarde: Turm Utgarde (79-80)', '1253.46', '-4853.82', '215.73', '571', '3.4309', '75', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '51');
INSERT INTO `custom_npc_tele_destination` VALUES ('52', '18. Prüfung des Champions (79-80)', '8575.03', '792.278', '558.514', '571', '3.16778', '75', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '52');
INSERT INTO `custom_npc_tele_destination` VALUES ('53', '20. Eiskronenzitadelle: Die Seelenschmiede (79-80)', '5672.19', '2002.17', '798.182', '571', '5.47579', '80', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '53');
INSERT INTO `custom_npc_tele_destination` VALUES ('54', '19. Eiskronenzitadelle: Grube von Saron (79-80)', '5591.99', '2010.32', '798.182', '571', '3.92227', '80', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '54');
INSERT INTO `custom_npc_tele_destination` VALUES ('55', '21. Eiskronenzitadelle: Hallen der Reflexion (79-80)', '5629.33', '1986.66', '800.027', '571', '4.72024', '80', '400000');
INSERT INTO `custom_npc_tele_association` VALUES ('5', '55');
INSERT INTO `custom_npc_tele_destination` VALUES ('56', '01. Schwarzfelsspitze  (54-60) (10)', '-7535.43', '-1212.04', '285.45', '0', '5.29', '49', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '56');
INSERT INTO `custom_npc_tele_destination` VALUES ('58', '03. Zul\'Gurub (56-60+) (20)', '-11916.7', '-1212.82', '92.2868', '0', '4.6095', '50', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '58');
INSERT INTO `custom_npc_tele_destination` VALUES ('59', '04. Geschmolzener Kern (60-63) (40)', '1121.45', '-454.317', '-101.33', '230', '3.5', '55', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '59');
INSERT INTO `custom_npc_tele_destination` VALUES ('60', '05. Pechschwingenhort (60-63) (40)', '-7665.55', '-1102.49', '400.679', '469', '0', '60', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '60');
INSERT INTO `custom_npc_tele_destination` VALUES ('61', '06. Ruinen von Ahn\'Qiraj (60-63) (20)', '-8409.03', '1498.83', '27.3615', '1', '2.49757', '60', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '61');
INSERT INTO `custom_npc_tele_destination` VALUES ('62', '07. Ahn\'Qiraj (60-63) (40)', '-8245.84', '1983.74', '129.072', '1', '0.936195', '60', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '62');
INSERT INTO `custom_npc_tele_destination` VALUES ('63', '08. Karazhan (70-73) (10)', '-11118.8', '-2010.84', '47.0807', '0', '0', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '63');
INSERT INTO `custom_npc_tele_destination` VALUES ('64', '09. Gruuls Unterschlupf (70-73) (25)', '3539.01', '5082.36', '1.69107', '530', '0', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '64');
INSERT INTO `custom_npc_tele_destination` VALUES ('65', '10. Höllenfeuerzitadelle: Magtheridons Kammer (70-73) (25)', '-317.43', '3095.03', '-116.42', '530', '5.1927', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '65');
INSERT INTO `custom_npc_tele_destination` VALUES ('66', '11. Zul\'Aman (70-73) (10)', '6846.95', '-7954.5', '170.028', '530', '4.61501', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '66');
INSERT INTO `custom_npc_tele_destination` VALUES ('67', '12. Der Echsenkessel: Höhle des Schlangenschreins (70-73) (25)', '795.188', '6865.64', '-64.8004', '530', '6.2096', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '67');
INSERT INTO `custom_npc_tele_destination` VALUES ('68', '13. Festung der Stürme: Festung der Stürme (70-73) (25)', '3090.03', '1402.73', '188.81', '530', '4.60991', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '68');
INSERT INTO `custom_npc_tele_destination` VALUES ('69', '14. Höhlen der Zeit: Hyjalgipfel (70-73) (25)', '-8184.08', '-4196.08', '-171.38', '1', '1.24845', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '69');
INSERT INTO `custom_npc_tele_destination` VALUES ('70', '15. Der Schwarze Tempel (70-73) (25)', '-3610.72', '324.988', '37.4', '530', '3.28298', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '70');
INSERT INTO `custom_npc_tele_destination` VALUES ('71', '16. Sonnenbrunnenplateau (70-73) (25)', '12574.1', '-6774.81', '15.09', '530', '3.13788', '70', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '71');
INSERT INTO `custom_npc_tele_destination` VALUES ('72', '17. Naxxramas (80-83) (10&25 man) ', '3668.71', '-1262.45', '243.62', '571', '4.785', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '72');
INSERT INTO `custom_npc_tele_destination` VALUES ('73', '18. Wyrmruhtempel: Das Obsidiansanktum (80-83) (10&25)', '3516.29', '269.49', '-114.15', '571', '3.19766', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '73');
INSERT INTO `custom_npc_tele_destination` VALUES ('74', '19. Archavons Kammer (10&25 man) (80-83)', '5478.16', '2840.41', '418.67', '571', '6.26748', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '74');
INSERT INTO `custom_npc_tele_destination` VALUES ('75', '20. Der Nexus: Das Auge der Ewigkeit (80-83) (10&25)', '3878.58', '6979.56', '152.04', '571', '2.70778', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '75');
INSERT INTO `custom_npc_tele_destination` VALUES ('76', '21. Ulduar (80-83) (10&25)', '9329.55', '-1114.16', '1245.14', '571', '6.24202', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '76');
INSERT INTO `custom_npc_tele_destination` VALUES ('77', '22. Prüfung des Kreuzfahrers (80-83) (10&25)', '8515.45', '728.919', '558.247', '571', '1.59726', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '77');
INSERT INTO `custom_npc_tele_destination` VALUES ('78', '23. Onyxias Hort (80-83) (10&25)', '-4707.44', '-3726.82', '54.6723', '1', '3.8', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '78');
INSERT INTO `custom_npc_tele_destination` VALUES ('79', '24. Icecrown Citadel (80-83) (10&25)', '5790', '2071.47', '636.065', '571', '3.62348', '80', '800000');
INSERT INTO `custom_npc_tele_association` VALUES ('6', '79');
INSERT INTO `custom_npc_tele_destination` VALUES ('80', '1. Sturmwind', '-8842.09', '626.358', '94.0867', '0', '0', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '80');
INSERT INTO `custom_npc_tele_destination` VALUES ('81', '2. Eisenschmiede', '-4919.94', '-982.083', '501.46', '0', '5.30771', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '81');
INSERT INTO `custom_npc_tele_destination` VALUES ('82', '3. Darnassus', '9869.91', '2493.58', '1315.88', '1', '0.240982', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '82');
INSERT INTO `custom_npc_tele_destination` VALUES ('83', '4. Die Exodar', '-3864.92', '-11643.7', '-137.644', '530', '0', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '83');
INSERT INTO `custom_npc_tele_destination` VALUES ('84', '1. Orgrimmar', '1601.08', '-4378.69', '9.9846', '1', '0.15315', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '84');
INSERT INTO `custom_npc_tele_destination` VALUES ('85', '2. Donnerfels', '-1274.45', '71.8601', '128.159', '1', '4.919', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '85');
INSERT INTO `custom_npc_tele_destination` VALUES ('86', '3. Unterstadt', '1633.75', '240.167', '-43.1034', '0', '3.15534', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '86');
INSERT INTO `custom_npc_tele_destination` VALUES ('87', '4. Silbermond', '9738.28', '-7454.19', '13.5605', '530', '0', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '87');
INSERT INTO `custom_npc_tele_destination` VALUES ('88', '5. Shattrath', '-1887.62', '5359.09', '-12.4279', '530', '3.40391', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '88');
-- INSERT INTO `custom_npc_tele_destination` VALUES ('89', '6. Insel von Quel\'Danas', '12947.4', '-6893.31', '5.68398', '530', '3.09154', '0', '50000');
-- INSERT INTO `custom_npc_tele_association` VALUES ('1', '89');
INSERT INTO `custom_npc_tele_destination` VALUES ('90', '7. Dalaran', '5809.55', '503.975', '657.526', '571', '1.70185', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '90');
INSERT INTO `custom_npc_tele_destination` VALUES ('91', '5. Shattrath', '-1887.62', '5359.09', '-12.4279', '530', '3.40391', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '91');
-- INSERT INTO `custom_npc_tele_destination` VALUES ('92', '6. Insel von Quel\'Danas', '12947.4', '-6893.31', '5.68398', '530', '3.09154', '0', '50000');
-- INSERT INTO `custom_npc_tele_association` VALUES ('2', '92');
INSERT INTO `custom_npc_tele_destination` VALUES ('93', '7. Dalaran', '5809.55', '503.975', '657.526', '571', '1.70185', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '93');
INSERT INTO `custom_npc_tele_destination` VALUES ('94', 'Halle der Händler', '-2677.91', '2907.24', '178.69', '1', '5.69152', '0', '500000');
INSERT INTO `custom_npc_tele_association` VALUES ('8', '94');
INSERT INTO `custom_npc_tele_destination` VALUES ('95', '1. Arena der Gurubashi', '-13261.3', '168.294', '35.0792', '0', '1.00688', '19', '500000');
INSERT INTO `custom_npc_tele_association` VALUES ('7', '95');
INSERT INTO `custom_npc_tele_destination` VALUES ('96', '2. Der Zirkel des Blutes', '2839.44', '5930.17', '11.1002', '530', '3.16284', '0', '500000');
INSERT INTO `custom_npc_tele_association` VALUES ('7', '96');
INSERT INTO `custom_npc_tele_destination` VALUES ('97', '3. Der Ring der Prüfung', '-1999.94', '6581.71', '11.32', '530', '2.3', '0', '500000');
INSERT INTO `custom_npc_tele_association` VALUES ('7', '97');
INSERT INTO `custom_npc_tele_destination` VALUES ('98', '4. Düsterbruch Arena', '-3739.86', '1093.8', '131.968', '1', '0.155619', '0', '500000');
INSERT INTO `custom_npc_tele_association` VALUES ('7', '98');


INSERT INTO `custom_npc_tele_destination` VALUES ('99', '8. Beutebucht', '-14281.9', '552.564', '8.90422', '0', '1.70185', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('1', '99');
INSERT INTO `custom_npc_tele_destination` VALUES ('100', '8. Beutebucht', '-14281.9', '552.564', '8.90422', '0', '1.70185', '0', '50000');
INSERT INTO `custom_npc_tele_association` VALUES ('2', '100');