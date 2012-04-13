DELETE FROM `trinity_string` WHERE `entry` BETWEEN '11610' AND '11615';
INSERT INTO `trinity_string` (`entry`, `content_default`) VALUES
('11610', 'Spieler %s hat dir verboten das Arena Match zu beobachten.'),
('11611', 'Spieler %s ist kein Teilnehmer eines Arena Matches.'),
('11612', 'Du kannst nicht Mitglied einer Arena oder BG Gruppe sein.'),
('11613', 'Du hast bereits einen Spectator Status.'),
('11614', 'Arena Spectators System ist deaktiviert.'),
('11615', 'Du kannst das Chat nicht verwenden wenn du im speactate Modus bist.');

DELETE FROM `command` WHERE `name` = 'spectate player';
INSERT INTO `command` VALUES ('spectate player', '0', 'Syntax: .spectate player $player\r\n\r\nLässt dich das Arena Match von $player beobachten');