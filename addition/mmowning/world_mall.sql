DELETE FROM command WHERE `name`='mall';
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('mall', 0, 'Syntax: .mall Teleportiert dich in die Hauptstadt deine Fraktion und belebt dich dort wieder, wenn du tot bist (Kann nicht im Kampf, Flug oder auf einem Mount verwendet werden)');

DELETE FROM command WHERE `name`='dala';
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('mall', 0, 'Syntax: .dala Teleportiert dich nach Dalaran und belebt dich dort wieder, wenn du tot bist (Kann nicht im Kampf, Flug oder auf einem Mount verwendet werden)');
