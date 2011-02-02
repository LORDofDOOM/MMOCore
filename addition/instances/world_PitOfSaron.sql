DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=12993 AND `type`=11;
INSERT INTO `achievement_criteria_data` (`criteria_id`,`type`,`value1`,`value2`,`ScriptName`) VALUES
(12993,11,0,0, 'achievement_doesnt_go_to_eleven');

DELETE FROM `areatrigger_scripts` where `entry`=5633;
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES
(5633,'at_tyrannus_event_starter');

UPDATE `creature_template` SET `Scriptname`='' WHERE `entry` IN (36840,31260,36892,36842,37728,36896,37713,37712,36788,36879,37711,38487,36841);

DELETE FROM `script_texts` WHERE `entry` BETWEEN -1658070 AND -1658050;
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
(36658,-1658050,'Your pursuit shall be in vain, adventurers, for the Lich King has placed an army of undead at my command! Behold!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16755,1,0,0,'Tyrannus SAY_AMBUSH_1'),
(36658,-1658051,'Persistent whelps! You will not reach the entrance of my lord\'s lair! Soldiers, destroy them!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16756,1,0,0,'Tyrannus SAY_AMBUSH_2'),
(36658,-1658052,'Rimefang! Trap them within the tunnel! Bury them alive!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16757,1,0,0,'Tyrannus SAY_GAUNTLET_START'),
(36658,-1658053,'Alas, brave, brave adventurers, your meddling has reached its end. Do you hear the clatter of bone and steel coming up the tunnel behind you? That is the sound of your impending demise.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16758,1,0,0,'Tyrannus SAY_TYRANNUS_INTRO_1'),
(37592,-1658054,'Heroes! We will hold off the undead as long as we can, even to our dying breath. Deal with the Scourgelord!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17150,1, 0,0,'Gorkun SAY_GORKUN_INTRO_2'),
(36658,-1658055,'Ha, such an amusing gesture from the rabble. When I have finished with you, my master\'s blade will feast upon your souls. Die!', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16759,1,0,0,'Tyrannus SAY_TYRANNUS_INTRO_3'),
(36658,-1658056,'I shall not fail The Lich King! Come and meet your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16760,1, 0,0,'Tyrannus SAY_AGGRO'),
(36658,-1658057,'Such a shameful display... You are better off dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16761,1,0,0,'Tyrannus SAY_SLAY_1'),
(36658,-1658058,'Perhaps you should have stayed in the mountains!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16762,1,0,0,'Tyrannus SAY_SLAY_2'),
(36658,-1658059,'Impossible! Rimefang... Warn...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16763,1,0,0,'Tyrannus SAY_DEATH'),
(36658,-1658060,'Rimefang, destroy this fool!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16764,1,0,0,'Tyrannus SAY_MARK_RIMEFANG_1'),
(36658,-1658061,'The frostwyrm Rimefang gazes at $N and readies an icy attack!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3, 0,0,'Tyrannus SAY_MARK_RIMEFANG_2'),
(36658,-1658062,'Power... overwhelming!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16765,1,0,0,'Tyrannus SAY_DARK_MIGHT_1'),
(36658,-1658063,'Scourgelord Tyrannus roars and swells with dark might!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3, 0,0,'Tyrannus SAY_DARK_MIGHT_2'),
(37592,-1658064,'Brave champions, we owe you our lives, our freedom... Though it be a tiny gesture in the face of this enormous debt, I pledge that from this day forth, all will know of your deeds, and the blazing path of light you cut through the shadow of this dark citadel.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1, 0,0,'Gorkun SAY_GORKUN_OUTRO_1'),
(37592,-1658065,'This day will stand as a testament not only to your valor, but to the fact that no foe, not even the Lich King himself, can stand when Alliance and Horde set aside their differences and ---',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1, 0,0,'Gorkun SAY_GORKUN_OUTRO_2'),
(36993,-1658066,'Heroes, to me!', NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16614,1,0,0,'Jaina SAY_JAYNA_OUTRO_3'),
(36990,-1658067,'Take cover behind me! Quickly!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17037,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_3'),
(36993,-1658068,'The Frost Queen is gone. We must keep moving - our objective is near.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16615, 0,0,0,'Jaina SAY_JAYNA_OUTRO_4'),
(36990,-1658069,'I thought he\'d never shut up. At last, Sindragosa silenced that long-winded fool. To the Halls of Reflection, champions! Our objective is near... I can sense it.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17036, 0,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_4'),
(36993,-1658070,'I... I could not save them... Damn you, Arthas! DAMN YOU!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16616, 0,0,0,'Jaina SAY_JAYNA_OUTRO_5');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (68786,70336) AND `ScriptName`='spell_garfrost_permafrost';
DELETE FROM `spell_script_names` WHERE `spell_id`=69012 AND `ScriptName`='spell_krick_explosive_barrage';
DELETE FROM `spell_script_names` WHERE `spell_id`=69263 AND `ScriptName`='spell_ick_explosive_barrage';
DELETE FROM `spell_script_names` WHERE `spell_id`=44851 AND `ScriptName`='spell_exploding_orb_hasty_grow';
DELETE FROM `spell_script_names` WHERE `spell_id`=68987 AND `ScriptName`='spell_krick_pursuit';
DELETE FROM `spell_script_names` WHERE `spell_id` IN (69029,70850) AND `ScriptName`='spell_krick_pursuit_confusion';
DELETE FROM `spell_script_names` WHERE `spell_id`=69275 AND `ScriptName`='spell_tyrannus_mark_of_rimefang';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(68786,'spell_garfrost_permafrost'),
(70336,'spell_garfrost_permafrost'),
(69012,'spell_krick_explosive_barrage'),
(69263,'spell_ick_explosive_barrage'),
(44851,'spell_exploding_orb_hasty_grow'),
(68987,'spell_krick_pursuit'),
(69029,'spell_krick_pursuit_confusion'),
(70850,'spell_krick_pursuit_confusion'),
(69275,'spell_tyrannus_mark_of_rimefang');

DELETE FROM `vehicle_accessory` WHERE `entry` IN (36476,36661,36891);
INSERT INTO `vehicle_accessory` (`entry`,`accessory_entry`,`seat_id`,`minion`,`description`) VALUES
(36476,36477,0,0,'Krick and Ick'),
(36661,36658,0,0,'Scourgelord Tyrannus and Rimefang'),
(36891,31260,0,0,'Ymirjar Skycaller on Drake');

DELETE FROM `creature` WHERE `id` IN (36610,36731,36477,36658,31260); -- Delete Temp Trigger Spawns

-- DB Data (souce: SNIFF)
UPDATE `creature_template` SET `exp`=2,`minlevel`=80,`maxlevel`=80,`unit_flags`=0x02008000,`faction_A`=14,`faction_H`=14,`flags_extra`=`flags_extra`|0x80 WHERE `entry`=37670; -- Frostblade
UPDATE `creature_template` SET `exp`=2,`minlevel`=80,`maxlevel`=80,`flags_extra`=`flags_extra`|0x80 WHERE `entry`=36610; -- Exploding Orb
UPDATE `creature_template` SET `exp`=2,`minlevel`=80,`maxlevel`=80,`scale`=1.1,`flags_extra`=`flags_extra`|0x80 WHERE `entry`=36731; -- Icy Blast
UPDATE `creature_template` SET `speed_run`=1.857143,`speed_walk`=3.2,`unit_flags`=0x00000040,`faction_A`=14, `faction_H`=14 WHERE `entry`=36658; -- Scourgelord Tyrannus
UPDATE `creature_template` SET `speed_run`=1.5873,`speed_walk`=2,`unit_flags`=0x00000040 WHERE `entry`=36661; -- Rimefang
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|0x80 WHERE `entry`=36495; -- Forgemaster Putridus Invisible Stalker
UPDATE `creature_template` SET `VehicleId`=560 WHERE `entry`=37626; -- Iceborn Protodrake (1)

-- SAI for Plagueborn Horror
SET @ENTRY := 36879;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,5000,5000,10000,10000,11,69581,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Pustulant Flesh on Random Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,5000,5000,10000,10000,11,70273,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Pustulant Flesh on Random Target (Heroic)'),
(@ENTRY,0,2,0,0,0,100,6,8000,8000,8000,8000,11,70274,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Toxic Waste on Random Target'),
(@ENTRY,0,3,0,2,0,100,6,15,15,0,0,11,69582,0,0,0,0,0,1,0,0,0,0,0,0,0, 'Cast Blight Bomb self at 15pct Health');

-- SAI for Hungering Ghoul
SET @ENTRY := 37711;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,6,4000,6000,8000,12000,11,70393,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Devour Flesh on current Target');

-- SAI for Deathwhisper Shadowcaster
SET @ENTRY := 37712;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,100,100,3000,3000,11,70386,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Shadow Bolt on current Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,100,100,3000,3000,11,70387,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Shadow Bolt on current Target (Heroic)');

-- SAI for Deathwhisper Torturer
SET @ENTRY := 37713;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,6,10000,10000,10000,10000,11,70392,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Black Brand on current Target'),
(@ENTRY,0,1,0,0,0,100,6,6000,6000,13000,13000,11,70391,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Curse of Agony on Random Target');

-- SAI for Ymirjar Wrathbringer
SET @ENTRY := 36840;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,7000,7000,8000,8000,11,69603,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Blight on Random Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,7000,7000,8000,8000,11,70285,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Blight on Random Target (Heroic)');

-- SAI for Stonespine Gargoyle
SET @ENTRY := 36896;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,1000,1000,5000,5000,11,69520,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Gargoyle Strike on current Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,1000,1000,5000,5000,11,70275,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Gargoyle Strike on current Target (Heroic)'),
(@ENTRY,0,2,0,2,0,100,6,10,10,0,0,11,69575,0,0,0,0,0,1,0,0,0,0,0,0,0, 'Cast Stoneform self at 10pct Health');

-- SAI for Wrathbone Sorcerer
SET @ENTRY := 37728;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,3000,3000,8000,8000,11,75330,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Shadow Bolt on current Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,3000,3000,8000,8000,11,75331,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Shadow Bolt on current Target (Heroic)');

-- SAI for Wrathbone Coldwraith
SET @ENTRY := 36842;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,100,100,3000,3000,11,69573,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Frostbolt on current Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,100,100,3000,3000,11,70277,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Frostbolt on current Target (Heroic)'),
(@ENTRY,0,2,0,0,0,100,2,9000,9000,15000,15000,11,69574,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Freezing Circle on current Target (Non-Heroic)'),
(@ENTRY,0,3,0,0,0,100,4,9000,9000,15000,15000,11,70276,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Freezing Circle on current Target (Heroic)');

-- SAI for Deathwhisper Necrolyte
SET @ENTRY := 36788;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,100,100,3000,3000,11,69577,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Shadow Bolt on current Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,100,100,3000,3000,11,70270,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Shadow Bolt on current Target (Heroic)'),
(@ENTRY,0,2,0,0,0,100,2,9000,9000,24000,24000,11,69578,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Conversion Beam on Random Target (Non-Heroic)'),
(@ENTRY,0,3,0,0,0,100,4,9000,9000,24000,24000,11,70269,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Conversion Beam on Random Target (Heroic)');

-- SAI for Fallen Warrior
SET @ENTRY := 36841;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,6,5000,5000,7000,8000,11,69579,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Arcing Slice to current Target'),
(@ENTRY,0,1,0,0,0,100,6,15000,15000,22000,22000,11,61044,0,0,0,0,0,1,0,0,0,0,0,0,0, 'Cast Demoralizing Shout'),
(@ENTRY,0,2,0,0,0,100,6,22000,22000,25000,25000,11,69580,0,0,0,0,0,1,0,0,0,0,0,0,0, 'Cast Shield Block');

-- SAI for Fallen Warrior 2
SET @ENTRY := 38487;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,6,5000,5000,7000,8000,11,69579,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Arcing Slice to current Target'),
(@ENTRY,0,1,0,0,0,100,6,15000,15000,22000,22000,11,61044,0,0,0,0,0,1,0,0,0,0,0,0,0, 'Cast Demoralizing Shout'),
(@ENTRY,0,2,0,0,0,100,6,22000,22000,25000,25000,11,69580,0,0,0,0,0,1,0,0,0,0,0,0,0, 'Cast Shield Block');

-- SAI for Ymirjar Skycaller
SET @ENTRY := 31260;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,6,4000,4000,8000,8000,11,70292,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Glacial Strike on current Target'),
(@ENTRY,0,1,0,2,0,100,6,50,50,0,0,11,70291,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Frostblade on Random Target at 50pct Health');

-- SAI for Frostblade
SET @ENTRY := 37670;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,6,1000,1000,0,0,11,70306,0,0,0,0,0,2,0,0,0,0,0,0,0, 'Cast Frostblade self');

-- SAI for Ymirjar Deathbringer
SET @ENTRY := 36892;
UPDATE `creature_template` SET `AIName`= 'SmartAI',`ScriptName`="" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,0,0,100,2,4000,4000,8000,8000,11,69528,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Empowered Shadow Bolt on Random Target (Non-Heroic)'),
(@ENTRY,0,1,0,0,0,100,4,4000,4000,8000,8000,11,70281,0,0,0,0,0,5,0,0,0,0,0,0,0, 'Cast Empowered Shadow Bolt on Random Target (Heroic)');
