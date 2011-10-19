-- Custom feature, read syntax for explonation. 
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite debuff',1,'Syntax: .elite debuff - removes Resurrection Sickness and Deserter debuffs from owner');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite map', 1, 'Syntax: .elite map - reveals all maps to owner');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite bank', 1, 'Syntax: .elite bank - Show your bank inventory.');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite repair', 1, 'Syntax: .elite repair - repair all your items.');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite resettalents', 1, 'Syntax: .elite resettalents - reset talents to your character.');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite whispers', 1, 'Syntax: .elite whispers - Enable/disable accepting whispers by elite accounts from players.');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite taxi', 1, 'Syntax: .elite taxi - reveals all taxipaths to owner');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite home', 1, 'Syntax: .elite home - teleport owner to home(ignoring CD on Hearthstone)');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite capital', 1, 'Syntax: .elite capital - teleport owner to capital(Orgrimmar/Stormwind) depending on faction of player');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite capital', 1, 'Syntax: .elite capital - teleport owner to capital(Orgrimmar/Stormwind) depending on faction of player');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite status', 3, 'Syntax: .elite  status $account - Elite Status eines Accounts anzeigen');
REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite add', 3, 'Syntax: .elite add $account $days - Set or update a VIP account.');

-- REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite', 1, 'Elite Status.');

REPLACE INTO `trinity_string` VALUES (11778, 'You can\'t use command while being dead/stealthed or being in arena/bg or being in combat.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '?? ?? ?????? ???????????? ???????? ???????? ?? ?????/??, ? ???????/?????? ??? ?????? ???????.');
REPLACE INTO `trinity_string` VALUES (11779, 'You can\'t use command while being dead/stealthed or being in arena/bg or being in combat.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '?? ?? ?????? ???????????? ???????? ???????? ?? ?????/??/??????????, ? ???????/?????? ??? ?????? ???????.')

-- Not working on Myth-Core. Need full rewrite AuctionHouse system :(
-- REPLACE INTO `command` (`name`,`security`,`help`) VALUES ('elite auction', 1, 'Syntax: .elite auction - opens auctionhouse window to character.');
