-- fix quest The Aspirant's Challenge / Die Herausforderung des Streiters
-- set quest autocomplete to normal mode
UPDATE `quest_template` SET `Method` = 2 WHERE `Id` IN (13679,13680);
-- set correct killcredit
UPDATE `quest_template` SET `RequiredNpcOrGo1` = 38595 WHERE `Id` IN (13679,13680);
-- set missing next quest for horde
UPDATE `quest_template` SET `NextQuestID` = 13696 WHERE `Id` = 13680;
-- add missing lance for the Argent Valiant
UPDATE `creature_template` SET `equipment_id` = 1873 WHERE `entry` = 33448;
