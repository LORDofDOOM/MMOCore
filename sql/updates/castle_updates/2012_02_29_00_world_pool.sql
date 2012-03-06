-- Gretta the Arbiter (Storm Peaks, Brunhilldar)
-- Daily Quest Pooling
-- Source: http://www.wowwiki.com/Gretta_the_Arbiter
SET @pool_id := 20310;

DELETE FROM `pool_template` WHERE `entry` = @pool_id;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@pool_id, 1, 'Gretta the Arbiter - Daily Quests');

DELETE FROM `pool_quest` WHERE `entry` IN (13424, 13423, 13422, 13425);
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(13424, @pool_id, 'Back to the Pit'),
(13423, @pool_id, 'Defending Your Title'),
(13422, @pool_id, 'Maintaining Discipline'),
(13425, @pool_id, 'The Aberrations Must Die');
