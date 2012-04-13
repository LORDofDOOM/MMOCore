-- Quest Soporte 11795/11887 
-- Emergency Protocol: Section 8.2, Paragraph C
-- Emergency Supplies
DELETE FROM `creature_ai_scripts` WHERE creature_id = 25841;
UPDATE `creature_template` SET IconName = 'Speak', npcflag = 1, gossip_menu_id = 0, AIName = '', ScriptName = 'npc_fizzcrank_recon_pilot', unit_flags = 0x00000000, flags_extra = 2, dynamicflags = 36  WHERE entry = 25841;
DELETE FROM `creature_template_addon` WHERE entry = 25841;
INSERT INTO `creature_template_addon` VALUES (25841, 0, 0, 7, 0, 65, '');
UPDATE `creature` SET DeathState = 0 WHERE id = 25841;

-- [11942]Words of Power.sql
-- Quest Soporte 11942
-- Fix Quest Words of Power (11942).-
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`IN(26073,26076);