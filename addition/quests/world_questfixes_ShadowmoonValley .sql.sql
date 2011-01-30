-- Fix quest 10866 - Zuluhed the Whacked 
UPDATE `quest_template` SET `ReqCreatureOrGOId1` = -185156, `ObjectiveText1` = 'Karynaku freed', `ReqCreatureOrGOId2` = 11980 , `ObjectiveText2` = 'Zuluhed the Whacked slain'  WHERE entry = 10866;
-- Fix quest  Ally of the Netherwing
DELETE FROM `creature_involvedrelation` WHERE `quest` IN  (10870, 10871);
DELETE FROM `creature_questrelation` WHERE `quest` IN (10870, 10871);
INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES(22113,10870);
INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES(22112,10870);
UPDATE `quest_template` SET `RewChoiceItemId1` = 31492, `RewChoiceItemId2` = 31491, `RewChoiceItemId3` = 31490, `RewChoiceItemId4` = 31494, `RewChoiceItemId5` = 31493, `RewChoiceItemCount1` = 1, `RewChoiceItemCount2` = 1, `RewChoiceItemCount3` = 1, `RewChoiceItemCount4` = 1, `RewChoiceItemCount5` = 1 WHERE `entry` = 10870;