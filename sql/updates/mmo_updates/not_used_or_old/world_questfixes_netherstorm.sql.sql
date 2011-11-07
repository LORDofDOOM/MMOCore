-- -------------------------
-- Netherstorm FULL
-- -------------------------

-- [10427]Quest_Creatures_Of_The_Eco_Domes.sql
-- Fix Quest Creatures of the Eco-Domes 10427
UPDATE `creature_template` SET `ScriptName`='mob_Talbuk' WHERE entry IN (20610,20777);

-- [10409]Quest_Deathblow_to_the_Legion.sql
-- Fix Quest Deathblow to the Legion http://www.wowhead.com/quest=10409
UPDATE `quest_template` SET `SpecialFlags` = 0, `ReqCreatureOrGOCount1` = 1, `ReqCreatureOrGOId1` = 20132 WHERE `entry` = 10409;