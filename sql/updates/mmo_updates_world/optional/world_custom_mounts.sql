UPDATE `creature_template` SET `modelid1` = 28890 WHERE `entry`=32335;  #  gyro flyer
UPDATE `creature_template` SET `modelid2` = 28890 WHERE `entry`=32335;  #  gyro flyer
UPDATE `creature_template` SET `modelid3` = 28890 WHERE `entry`=32335;  #  gyro flyer
UPDATE `creature_template` SET `modelid4` = 28890 WHERE `entry`=32335;  #  gyro flyer
UPDATE `creature_template` SET `VehicleId`=336, `Iconname`='vehichleCursor' WHERE `entry`=32335;  # Make it a dual riding vehicle..hack

UPDATE `creature_template` SET `modelid1` = 23365 WHERE `entry`=31124;  #  Ally Jet Plane
UPDATE `creature_template` SET `modelid2` = 25333 WHERE `entry`=31124;  #  Horde Jet Plane
UPDATE `creature_template` SET `modelid3` = 23365 WHERE `entry`=31124;  #  Ally Jet Plane
UPDATE `creature_template` SET `modelid4` = 25333 WHERE `entry`=31124;  #  Horde Jet Plane
UPDATE `creature_template` SET `VehicleId`=336, `Iconname`='vehichleCursor' WHERE `entry`=31124;  # Make it a dual riding vehicle..hack
-- UPDATE `creature_template` SET `name`='Brutal Nether Drake',`subname`='',`IconName`='vehichleCursor',`unit_class`=4,`unit_flags`=16384,`AIName`='',`ScriptName`='' WHERE `entry`=31124;
-- UPDATE `creature_template` SET `name`='Brutal Nether Drake',`subname`='',`IconName`='vehichleCursor',`spell1`=62358,`spell2`=62359,`spell3`=64677,`AIName`='',`ScriptName`='' WHERE `entry`=31124;

UPDATE `creature_template` SET `modelid1` = 2490 WHERE `entry`=22510;   #  Goblin Rocket Mount
UPDATE `creature_template` SET `modelid2` = 2490 WHERE `entry`=22510;   #  Goblin Rocket Mount
UPDATE `creature_template` SET `modelid3` = 27829 WHERE `entry`=22510;  #  Horde Toy Rocket Car Mount
UPDATE `creature_template` SET `modelid4` = 27829 WHERE `entry`=22510;  #  Horde Toy Rocket Car Mount

UPDATE `creature_template` SET `modelid1` = 31623 WHERE `entry`=22511;  #  Unmanned spider robot
UPDATE `creature_template` SET `modelid2` = 31623 WHERE `entry`=22511;  #  Unmanned spider robot
UPDATE `creature_template` SET `modelid3` = 31684 WHERE `entry`=22511;  #  Unmanned robot
UPDATE `creature_template` SET `modelid4` = 31684 WHERE `entry`=22511;  #  Unmanned robot

UPDATE `creature_template` SET `modelid1` = 24946 WHERE `entry`=22512;  #  Huge Boat
UPDATE `creature_template` SET `modelid2` = 24946 WHERE `entry`=22512;  #  Huge Boat
UPDATE `creature_template` SET `modelid3` = 24946 WHERE `entry`=22512;  #  Huge Boat
UPDATE `creature_template` SET `modelid4` = 24946 WHERE `entry`=22512;  #  Huge Boat
-- UPDATE `creature_template` SET `VehicleId`=338, `Iconname`='vehichleCursor' WHERE `entry`=22512;  # Make it a tri-riding vehicle

UPDATE `creature_template` SET `modelid1` = 31094 WHERE `entry`=22513;  #  Celestial Bear
UPDATE `creature_template` SET `modelid2` = 31094 WHERE `entry`=22513;  #  Celestial Bear
UPDATE `creature_template` SET `modelid3` = 31094 WHERE `entry`=22513;  #  Celestial Bear
UPDATE `creature_template` SET `modelid4` = 31094 WHERE `entry`=22513;  #  Celestial Bear

UPDATE `creature_template` SET `modelid1` = 28605 WHERE `entry`=22514;  #  White Horse Mount
UPDATE `creature_template` SET `modelid2` = 28605 WHERE `entry`=22514;  #  White Horse Mount
UPDATE `creature_template` SET `modelid3` = 29129 WHERE `entry`=22514;  #  White \ Blue Horse Mount
UPDATE `creature_template` SET `modelid4` = 29129 WHERE `entry`=22514;  #  White \ Blue Horse Mount

-- UPDATE `creature_template` SET `modelid1` = 31007 WHERE `entry`=18375;  #  Flying version of Undead Frosting Horse with wings
-- UPDATE `creature_template` SET `modelid2` = 31007 WHERE `entry`=18375;  #  Flying version of Undead Frosting Horse with wings
-- UPDATE `creature_template` SET `modelid3` = 31007 WHERE `entry`=18375;  #  Flying version of Undead Frosting Horse with wings
-- UPDATE `creature_template` SET `modelid4` = 31248 WHERE `entry`=18375;  #  Flying version of Undead Frosting Horse with wings

UPDATE `creature_template` SET `modelid1` = 28400 WHERE `entry`=18362;  #  Ghostly gryphon Flying Mount
UPDATE `creature_template` SET `modelid2` = 28400 WHERE `entry`=18362;  #  Ghostly gryphon Flying Mount
UPDATE `creature_template` SET `modelid3` = 28400 WHERE `entry`=18362;  #  Ghostly gryphon Flying Mount
UPDATE `creature_template` SET `modelid4` = 28400 WHERE `entry`=18362;  #  Ghostly gryphon Flying Mount

-- UPDATE `creature_template` SET `modelid1` = 31957 WHERE `entry`=18376;  #  Flying version of Celestial Steed
-- UPDATE `creature_template` SET `modelid2` = 31957 WHERE `entry`=18376;  #  Flying version of Celestial Steed
UPDATE `creature_template` SET `modelid1` = 31803 WHERE `entry`=18376;  #  Flying version of Celestial Steed
UPDATE `creature_template` SET `modelid2` = 31803 WHERE `entry`=18376;  #  Flying version of Celestial Steed
UPDATE `creature_template` SET `modelid3` = 31803 WHERE `entry`=18376;  #  Burning Hippograph Flying Mount
UPDATE `creature_template` SET `modelid4` = 31803 WHERE `entry`=18376;  #  Burning Hippograph Flying Mount

UPDATE `creature_template` SET `modelid1` = 31157 WHERE `entry`=27637;  #  Armored Blue Dragon Flying Mount
UPDATE `creature_template` SET `modelid2` = 31157 WHERE `entry`=27637;  #  Armored Blue Dragon Flying Mount
UPDATE `creature_template` SET `modelid3` = 31156 WHERE `entry`=27637;  #  Armored Blue Dragon Flying Mount
UPDATE `creature_template` SET `modelid4` = 31156 WHERE `entry`=27637;  #  Armored Blue Dragon Flying Mount


# Don't forget to check your creature_model_info to see if these model ids exist!
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);
INSERT IGNORE INTO `creature_model_info` VALUES (31157,0,0,2,0);

# Not Used -- UPDATE `creature_template` SET `modelid2` = 29642 WHERE `entry`=;  #  Ally Battle Vehicle - might be used in upcomng events
# Not Used -- UPDATE `creature_template` SET `modelid4` = 29734 WHERE `entry`=;  #  Hode - Red / Gold Battle vehicle

## -- To Reverse out of this look for the reverse sql in the mount folder.

