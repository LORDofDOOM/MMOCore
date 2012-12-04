
CREATE TABLE `mail_external` (
  `id` int(20) unsigned NOT NULL auto_increment,
  `receiver` int(20) unsigned NOT NULL,
  `subject` varchar(200) default 'Support Message',
  `message` varchar(500) default 'Support Message',
  `money` int(20) unsigned NOT NULL default '0',
  `item` int(20) unsigned NOT NULL default '0',
  `item_count` int(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=10059 DEFAULT CHARSET=utf8;
