UPDATE `gameobject_template` SET `data10` = '1', `data12` = '1', `data13` = '1' WHERE `entry` = '201959';
UPDATE `gameobject_template` SET `data10` = '1', `data12` = '1', `data13` = '1' WHERE `entry` = '202338';
UPDATE `gameobject_template` SET `data10` = '1', `data12` = '1', `data13` = '1' WHERE `entry` = '202339';
UPDATE `gameobject_template` SET `data10` = '1', `data12` = '1', `data13` = '1' WHERE `entry` = '202340';

INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
('', '201959', '631', '1', '1', '4203.700195', '2484.830078', '364.955994', '3.141590', '', '', '', '', '-120', '', '1'),
('', '202338', '631', '4', '1', '4203,700195', '2484,830078', '364,955994', '3,141590', '', '', '', '', '-120', '', '1'),
('', '202339', '631', '2', '1', '4203,700195', '2484,830078', '364,955994', '3,141590', '', '', '', '', '-120', '', '1'),
('', '202340', '631', '8', '1', '4203,700195', '2484,830078', '364,955994', '3,141590', '', '', '', '', '-120', '', '1');