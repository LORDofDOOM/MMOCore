-- @DorianGrey
-- This, together with the code-additions from the commit, should fix the following quests.
-- http://www.wowhead.com/quest=10446 The Final Code (Alliance)
-- http://www.wowhead.com/quest=10447 The Final Code (Horde)
-- Due to the overall complexity, I've decided not to use a SmartAI-script, since interaction of a trigger-npc and a gameobject is required.
-- The quest itself gets completed by the script, which not only avoid the usage of a bunny, but also avoid come curious behavior due to the quest conditions.

-- Enable script on trigger-npc.
UPDATE `creature_template` SET `ScriptName`='npc_mana_bomb_trigger' WHERE `entry`=20767;
-- Enable script on gameobject. 
UPDATE `gameobject_template` SET `ScriptName`='go_mana_bomb' WHERE `entry`=184725;
-- Script-texts required by the trigger-npc.
DELETE FROM `script_texts` WHERE `entry` IN (-1000472,-1000473,-1000474,-1000475,-1000476) AND `npc_entry`=20767; 
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
(20767,-1000472,'5...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,14,0,0,'mana bomb SAY_COUNT_5'),
(20767,-1000473,'4...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,14,0,0,'mana bomb SAY_COUNT_4'),
(20767,-1000474,'3...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,14,0,0,'mana bomb SAY_COUNT_3'),
(20767,-1000475,'2...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,14,0,0,'mana bomb SAY_COUNT_2'),
(20767,-1000476,'1...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,14,0,0,'mana bomb SAY_COUNT_1');

