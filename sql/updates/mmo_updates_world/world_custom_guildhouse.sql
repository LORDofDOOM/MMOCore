/**
* Records Guildhouse Keeper
*/
DELETE FROM creature_template WHERE `entry`=13;
INSERT INTO `creature_template` VALUES ('13', '0', '0', '0', '0', '0', '26789', '0', '0', '0', 'MMOwning Gildenhäuser', 'Guildhouse Keeper', '', '0', '80', '80', '0', '35', '35', '1', '1.48', '1.14286', '0.75', '0', '181', '189', '0', '158', '1', '1400', '1900', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '1', '3', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', 'guildmaster', '0');

DELETE FROM trinity_string WHERE `entry`=11500;
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES (11500, 'Deine Gilde besitzt noch kein Gildenhaus.');
DELETE FROM `command` WHERE `name`='gh';
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('gh', 0, 'Syntax: .gh Teleportiert dich zu eurem Gildenhaus (Kann nicht im Kampf, Flug oder auf einem Mount verwendet werden)');