-- [10111]Quest_Bring_The_Egg.sql
-- Quest Bring Me The Egg! http://www.wowhead.com/quest=10111
UPDATE `creature_template` SET `unit_flags` = 0, `flags_extra` = 0 WHERE `entry` = 19055;

-- Quest_CorkiGoneMissingAgain_9924.sql
-- Quest Soporte http://www.wowhead.com/quest=9924 "Corki's Gone Missing Again!"
DELETE FROM `script_texts` WHERE `npc_entry`=20812 AND `entry`=-1850071;
INSERT INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
('20812','-1850071','This is the last time I get caughht! I promise! Bye!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'0','0','0','0','');
UPDATE `gameobject_template` SET `Scriptname`='go_corki_cage' WHERE `entry`=182350;-- Corki's Prison GUID: 22796 ID: 182350

-- Quest_Help_9923.sql
-- Quest Soporte http://www.wowhead.com/quest=9923 "HELP!"
DELETE FROM `script_texts` WHERE `npc_entry`=18445;
INSERT INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
('18445','-1850070','Thanks,$R Im sure my dad will reward you greatly,bye',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'0','0','0','0','');
UPDATE `gameobject_template` SET `Scriptname`='go_help_cage' WHERE `entry`=182349;