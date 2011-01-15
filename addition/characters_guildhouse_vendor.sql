/* Records Guildhouse Keeper */
DELETE FROM creature_template WHERE `entry`=13;
INSERT INTO `creature_template` VALUES ('13', '0', '0', '0', '0', '0', '26789', '0', '0', '0', 'MMOwning Gildenhäuser', 'Guildhouse Keeper', '', '0', '80', '80', '0', '35', '35', '1', '1.48', '1.14286', '0.75', '0', '181', '189', '0', '158', '1', '1400', '1900', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '100', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '1', '3', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', 'npc_guild_master', '0');

DELETE FROM trinity_string WHERE `entry`=11500;
INSERT INTO `trinity_string` (`entry`,`content_default`) VALUES (11500, 'Deine Gilde besitzt noch kein Gildenhaus.');
DELETE FROM `command` WHERE `name`='gh';
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('gh', 0, 'Syntax: .gh Teleportiert dich zu eurem Gildenhaus (Kann nicht im Kampf, Flug oder auf einem Mount verwendet werden)');

/*Table structure for table `guildhouses` */
CREATE TABLE `guildhouses` (
  `id` int(16) unsigned NOT NULL AUTO_INCREMENT,
  `guildId` bigint(20) NOT NULL DEFAULT '0',
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `map` int(16) NOT NULL,
  `comment` varchar(255) NOT NULL DEFAULT '',
  `price` bigint(20) NOT NULL DEFAULT '0',
  `faction` int(8) unsigned NOT NULL DEFAULT '3',
  `minguildsize` int(16) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

/*Table structure for table `guildhouses_add` */
CREATE TABLE `guildhouses_add` (
  `guid` int(32) unsigned NOT NULL,
  `type` int(16) unsigned NOT NULL,
  `id` int(16) unsigned NOT NULL,
  `add_type` int(16) unsigned NOT NULL,
  `comment` varchar(255) NOT NULL DEFAULT '',
  PRIMARY KEY (`guid`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

/*Table structure for table `guildhouses_addtype` */
CREATE TABLE `guildhouses_addtype` (
  `add_type` int(16) unsigned NOT NULL,
  `comment` varchar(255) NOT NULL DEFAULT '',
  `price` bigint(20) NOT NULL DEFAULT '1000',
  `minguildsize` int(16) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`add_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

CREATE TABLE `gh_guildadd` (
  `guildId` int(32) unsigned NOT NULL,
  `GuildHouse_Add` int(32) unsigned NOT NULL,
  PRIMARY KEY  (`guildId`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;
