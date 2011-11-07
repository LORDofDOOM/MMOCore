-- [Faction]The_Oracles.sql
-- ------------------------------
-- Faction: The Oracles
-- ------------------------------
UPDATE `creature_template` SET `ScriptName`='npc_high_oracle_soo_say' WHERE `entry`='28027'; -- Scriptname npc http://www.wowhead.com/npc=28027#comments
UPDATE `creature_template` SET `ScriptName`='npc_invisible_oracle_treasure_trigger' WHERE `entry`='19656'; -- Scriptname npc http://www.wowhead.com/npc=19656
UPDATE `creature_template` SET `ScriptName`='npc_generic_oracle_treasure_seeker' WHERE `entry` IN (28120, 28121, 28122);-- Script Quest http://www.wowhead.com/quest=12578 - http://www.wowhead.com/quest=12704 - Quest Soporte : http://www.wowhead.com/quest=12572
UPDATE `creature_template` SET `ScriptName`='npc_oracle_frenzyheart_switch' WHERE `entry` IN (28667, 28668); -- Script Npc Cambio Faction.Oracles<->Frenzyheart
UPDATE `creature_template` SET `ScriptName`= 'npc_frenzyheart_zepik' WHERE `entry` = 28216; -- Script Quest http://www.wowhead.com/quest=12536
UPDATE `creature_template` SET `ScriptName`= 'npc_mosswalker_victim' WHERE `entry` = 28113; --  http://www.wowhead.com/quest=12580 - http://www.wowhead.com/quest=12580#comments