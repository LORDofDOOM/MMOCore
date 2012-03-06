-- #832
UPDATE `creature_template` SET `unit_flags` = `unit_flags` &~ 256 WHERE `entry` = 24518;

-- #1015
-- Fix Black Pearl loottable / i.e., Big-mouth Clam where it can be found in
-- we have literally not creature_loot_template for this and only 1 out of 4 fishing spots.

DELETE FROM `fishing_loot_template` WHERE `item` = 7973;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(16, 7973, 8, 1, 0, 1, 1), -- Azshara
(440, 7973, 6, 1, 0, 1, 1), -- Tanaris
(47, 7973, 5, 1, 0, 1, 1), -- The Hinterlands
(357, 7973, 1.6, 1, 0, 1, 1); -- Feralas

DELETE FROM `creature_loot_template` WHERE `item` = 7973;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(4370, 7973, 70.99, 1, 0, 1, 1), -- Strashaz Sorceress
(16072, 7973, 69.99, 1, 0, 1, 1), -- Tidelord Rrurgaz
(4368, 7973, 68.38, 1, 0, 1, 1), -- Strashaz Myrmidon
(6137, 7973, 67.67, 1, 0, 1, 1), -- Arkkoran Pincer
(4364, 7973, 67.36, 1, 0, 1, 1), -- Strashaz Warrior
(4366, 7973, 66.83, 1, 0, 1, 1), -- Strashaz Serpent Guard
(6138, 7973, 66.64, 1, 0, 1, 1), -- Arkkoran Oracle
(4403, 7973, 66.38, 1, 0, 1, 1), -- Muckshell Pincer
(4404, 7973, 66.15, 1, 0, 1, 1), -- Muckshell Scrabbler
(4371, 7973, 65.88, 1, 0, 1, 1), -- Strashaz Siren
(6350, 7973, 65.59, 1, 0, 1, 1), -- Makrinni Razorclaw
(6135, 7973, 65.31, 1, 0, 1, 1), -- Arkkoran Clacker
(6136, 7973, 64.74, 1, 0, 1, 1), -- Arkkoran Muckdweller
(6351, 7973, 64.19, 1, 0, 1, 1), -- Storm Bay Oracle
(6196, 7973, 60.21, 1, 0, 1, 1), -- Spitelash Myrmidon
(6372, 7973, 59, 1, 0, 1, 1), -- Makrinni Snapclaw
(8409, 7973, 58.33, 1, 0, 1, 1), -- Caravan Master Tset
(6195, 7973, 57.86, 1, 0, 1, 1), -- Spitelash Siren
(5467, 7973, 57.63, 1, 0, 1, 1), -- Deep Dweller
(6371, 7973, 57.24, 1, 0, 1, 1), -- Storm Bay Warrior
(6194, 7973, 56.81, 1, 0, 1, 1), -- Spitelash Serpent Guard
(8408, 7973, 56.72, 1, 0, 1, 1), -- Warlord Krellian
(6144, 7973, 55.88, 1, 0, 1, 1), -- Son of Arkkoroc
(6143, 7973, 52.64, 1, 0, 1, 1), -- Servant of Arkkoroc
(5466, 7973, 52.39, 1, 0, 1, 1), -- Coast Strider
(6370, 7973, 52.11, 1, 0, 1, 1), -- Makrinni Scrabbler
(7885, 7973, 51.57, 1, 0, 1, 1), -- Spitelash Battlemaster
(6193, 7973, 50.96, 1, 0, 1, 1), -- Spitelash Screamer
(6190, 7973, 50.54, 1, 0, 1, 1), -- Spitelash Warrior
(752, 7973, 50.01, 1, 0, 1, 1), -- Marsh Oracle
(4390, 7973, 49.85, 1, 0, 1, 1), -- Elder Murk Thresher
(5333, 7973, 48.6, 1, 0, 1, 1), -- Hatecrest Serpent Guard
(4389, 7973, 47.93, 1, 0, 1, 1), -- Murk Thresher
(4374, 7973, 47.61, 1, 0, 1, 1), -- Strashaz Hydra
(7886, 7973, 47.49, 1, 0, 1, 1), -- Spitelash Enchantress
(5336, 7973, 47.13, 1, 0, 1, 1), -- Hatecrest Sorceress
(6349, 7973, 47.09, 1, 0, 1, 1), -- Great Wavethrasher
(5327, 7973, 47, 1, 0, 1, 1), -- Coast Crawl Snapclaw
(751, 7973, 46.65, 1, 0, 1, 1), -- Marsh Flesheater
(9916, 7973, 46.62, 1, 0, 1, 1), -- Jarquia
(750, 7973, 46.56, 1, 0, 1, 1), -- Marsh Inkspewer
(950, 7973, 45.87, 1, 0, 1, 1), -- Swamp Talker
(6347, 7973, 45.7, 1, 0, 1, 1), -- Young Wavethrasher
(8136, 7973, 45.29, 1, 0, 1, 1), -- Lord Shalzaru
(747, 7973, 45.25, 1, 0, 1, 1), -- Marsh Murloc
(5334, 7973, 45.12, 1, 0, 1, 1), -- Hatecrest Myrmidon
(5328, 7973, 44.21, 1, 0, 1, 1), -- Coast Crawl Deepseer
(6348, 7973, 43.98, 1, 0, 1, 1), -- Wavethrasher
(8213, 7973, 42.48, 1, 0, 1, 1), -- Ironback
(6140, 7973, 41.79, 1, 0, 1, 1), -- Hetaera
(6352, 7973, 41.05, 1, 0, 1, 1), -- Coralshell Lurker
(13896, 7973, 40.98, 1, 0, 1, 1), -- Scalebeard
(6369, 7973, 40.65, 1, 0, 1, 1), -- Coralshell Tortoise
(2505, 7973, 40.52, 1, 0, 1, 1), -- Saltwater Snapjaw
(1491, 7973, 40.1, 1, 0, 1, 1), -- Zanzil Naga
(4388, 7973, 40.03, 1, 0, 1, 1), -- Young Murk Thresher
(1494, 7973, 39.71, 1, 0, 1, 1), -- Negolash
(5431, 7973, 39.46, 1, 0, 1, 1), -- Surf Glider
(7977, 7973, 39.41, 1, 0, 1, 1), -- Gammerita
(1907, 7973, 38.88, 1, 0, 1, 1), -- Naga Explorer
(1493, 7973, 38.79, 1, 0, 1, 1), -- Mok'rash
(1088, 7973, 38.69, 1, 0, 1, 1), -- Monstrous Crawler
(2544, 7973, 38.6, 1, 0, 1, 1), -- Southern Sand Crawler
(5359, 7973, 38.54, 1, 0, 1, 1), -- Shore Strider
(5360, 7973, 38.5, 1, 0, 1, 1), -- Deep Strider
(14639, 7973, 38.5, 1, 0, 1, 1), -- Zapped Deep Strider
(5331, 7973, 38.36, 1, 0, 1, 1), -- Hatecrest Warrior
(6649, 7973, 37.78, 1, 0, 1, 1), -- Lady Sesspira
(14603, 7973, 37.56, 1, 0, 1, 1), -- Zapped Shore Strider
(2541, 7973, 37.43, 1, 0, 1, 1), -- Lord Sakrasis
(13599, 7973, 36.69, 1, 0, 1, 1), -- Stolid Snapjaw
(5361, 7973, 36.47, 1, 0, 1, 1), -- Wave Strider
(14638, 7973, 36.38, 1, 0, 1, 1), -- Zapped Wave Strider
(5337, 7973, 36.12, 1, 0, 1, 1), -- Hatecrest Siren
(5335, 7973, 36.07, 1, 0, 1, 1), -- Hatecrest Screamer
(12207, 7973, 35.86, 1, 0, 1, 1), -- Thessala Hydra
(14123, 7973, 35.05, 1, 0, 1, 1), -- Steeljaw Snapper
(1492, 7973, 34.37, 1, 0, 1, 1), -- Gorlash
(922, 7973, 33.93, 1, 0, 1, 1), -- Silt Crawler
(5332, 7973, 33.71, 1, 0, 1, 1), -- Hatecrest Wave Rider
(14236, 7973, 32.59, 1, 0, 1, 1), -- Lord Angler
(4687, 7973, 32.37, 1, 0, 1, 1), -- Deepstrider Searcher
(5343, 7973, 32.2, 1, 0, 1, 1), -- Lady Szallah
(2779, 7973, 32.16, 1, 0, 1, 1), -- Prince Nazjak
(14446, 7973, 31.79, 1, 0, 1, 1), -- Fingat
(4686, 7973, 30.67, 1, 0, 1, 1), -- Deepstrider Giant
(14447, 7973, 29.43, 1, 0, 1, 1), -- Gilmorian
(6650, 7973, 28.88, 1, 0, 1, 1), -- General Fangferror
(5345, 7973, 24.35, 1, 0, 1, 1), -- Diamond Head
(7273, 7973, 21.26, 1, 0, 1, 1); -- Gahz'rilla

-- #1014
-- Fix Reinforced Junkbox pickpocket loottable
-- Wowhead counts over 200 NPCs for it to be stolen from, Aowow only 4

UPDATE `creature_template` SET `pickpocketloot` = `entry` WHERE `entry` IN (29570,23670,27270,31160,26461,28023,30147,29885,27288,26705,28496,27580,25801,28916,26681,23675,27570,28917,28750,27732,23669,30696,28748,27551,32284,31321,27701,32250,27805,32507,26922,29407,28965,27122,26073,27926,32236,27965,27960,24635,25713,27224,26409,27237,27006,26769,27340,27105,29836,27018,28861,27615,30725,23678,27284,26801,41038,29404,26655,24644,28108,26480,26923,27676,28101,24116,29826,29697,25601,26623,30144,23794,28918,26943,25430,28601,25427,29819,26694,23674,26348,28565,30864,32267,28961,32238,25800,23663,25712,26202,30179,25429,24398,32467,24015,27554,26828,25432,28022,27410,38032,26708,27370,27961,31693,25448,26679,23665,28575,27360,24249,24954,26496,28538,26624,26802,27286,27355,27799,23672,23660,23653,28641,26425,30597,26891,24216,25678,27279,23644,23992,27005,25768,28848,34839,23666,27927,29237,27401,27287,26639,31139,27969,26803,28836,27859,26830,26553,29654,26762,31161,26344,29413,27424,29614,23655,27797,30409,23963,32257,31738,29553,25839,23658,26481,29427,27278,26455,28402,28081,24485,27823,27343,31691,26410,23662,23796,30243,29793,27007,27334,27533,32268,27729,32502,26669,24469,27232,26295,31258,31155,23983,23866,24871,23656,28747,24461,25684,26357,28837);

DELETE FROM `pickpocketing_loot_template` WHERE `item` = 43575;
INSERT INTO `pickpocketing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(29570, 43575, 100, 1, 0, 1, 1), -- Nascent Val'kyr
(23670, 43575, 80, 1, 0, 1, 1), -- Winterskorn Elder
(27270, 43575, 72.73, 1, 0, 1, 1), -- Rotting Storm Giant
(31160, 43575, 71.43, 1, 0, 1, 1), -- Rokir
(26461, 43575, 70, 1, 0, 1, 1), -- Scourge Corpserender
(28023, 43575, 69.23, 1, 0, 1, 1), -- Rotting Abomination
(30147, 43575, 67.8, 1, 0, 1, 1), -- Garhal
(29885, 43575, 66.67, 1, 0, 1, 1), -- Mildred the Cruel
(27288, 43575, 66.67, 1, 0, 1, 1), -- Death Knight Champion
(26705, 43575, 66.67, 1, 0, 1, 1), -- Snowplain Disciple
(28496, 43575, 65.63, 1, 0, 1, 1), -- Chulo the Mad
(27580, 43575, 63.64, 1, 0, 1, 1), -- Selas
(25801, 43575, 62.5, 1, 0, 1, 1), -- Nedar, Lord of Rhinos
(28916, 43575, 62.5, 1, 0, 1, 1), -- Tiri
(26681, 43575, 61.9, 1, 0, 1, 1), -- Grumbald One-Eye
(23675, 43575, 61.54, 1, 0, 1, 1), -- Iron Rune Runemaster
(27570, 43575, 61.11, 1, 0, 1, 1), -- Venture Co. Straggler
(28917, 43575, 61.11, 1, 0, 1, 1), -- Yara
(28750, 43575, 60, 1, 0, 1, 1), -- Blight Geist
(27732, 43575, 60, 1, 0, 1, 1), -- Master Necromancer
(23669, 43575, 60, 1, 0, 1, 1), -- Winterskorn Oracle
(30696, 43575, 58.33, 1, 0, 1, 1), -- Corpulent Horror
(28748, 43575, 58.33, 1, 0, 1, 1), -- Serpent-Touched Berserker
(27551, 43575, 57.14, 1, 0, 1, 1), -- Enraged Apparition
(32284, 43575, 57.14, 1, 0, 1, 1), -- Scourge Soulbinder
(31321, 43575, 57.14, 1, 0, 1, 1), -- Skeletal Runesmith
(27701, 43575, 56.67, 1, 0, 1, 1), -- Gjalerhorn Worker
(32250, 43575, 56.25, 1, 0, 1, 1), -- Overseer Faedris
(27805, 43575, 56.1, 1, 0, 1, 1), -- Necrolord Horus
(32507, 43575, 55.56, 1, 0, 1, 1), -- Cultist Acolyte
(26922, 43575, 54.55, 1, 0, 1, 1), -- Overseer Lochli
(29407, 43575, 53.47, 1, 0, 1, 1), -- Snowblind Devotee
(28965, 43575, 53.33, 1, 0, 1, 1), -- Titanium Thunderer
(27122, 43575, 52.63, 1, 0, 1, 1), -- Overseer Deathgaze
(26073, 43575, 52, 1, 0, 1, 1), -- High Priest Talet-Kha
(27926, 43575, 51.72, 1, 0, 1, 1), -- Thorvald
(32236, 43575, 51.52, 1, 0, 1, 1), -- Dark Subjugator
(27965, 43575, 51.47, 1, 0, 1, 1), -- Dark Rune Shaper
(27960, 43575, 50.5, 1, 0, 1, 1), -- Dark Rune Warrior
(24635, 43575, 50.38, 1, 0, 1, 1), -- Dragonflayer Harpooner
(25713, 43575, 50, 1, 0, 1, 1), -- Blue Drakonid Supplicant
(27224, 43575, 50, 1, 0, 1, 1), -- Forgotten Knight
(26409, 43575, 50, 1, 0, 1, 1), -- Rune-Smith Durar
(27237, 43575, 50, 1, 0, 1, 1), -- Commander Jordan
(27006, 43575, 50, 1, 0, 1, 1), -- Bonesunder
(26769, 43575, 50, 1, 0, 1, 1), -- Anok'ra the Manipulator
(27340, 43575, 50, 1, 0, 1, 1), -- Bloodpaw Marauder
(27105, 43575, 50, 1, 0, 1, 1), -- Kreug Oathbreaker
(29836, 43575, 50, 1, 0, 1, 1), -- Drakkari Battle Rider
(27018, 43575, 50, 1, 0, 1, 1), -- Shade of Arugal
(28861, 43575, 50, 1, 0, 1, 1), -- Mam'toth Disciple
(27615, 43575, 49.43, 1, 0, 1, 1), -- Scourge Deathspeaker
(30725, 43575, 49.28, 1, 0, 1, 1), -- Jotunheim Sleep-Watcher
(23678, 43575, 48.89, 1, 0, 1, 1), -- Chill Nymph
(27284, 43575, 48.88, 1, 0, 1, 1), -- Risen Wintergarde Defender
(26801, 43575, 48.78, 1, 0, 1, 1), -- Horde Ranger
(41038, 43575, 48.65, 1, 0, 1, 1), -- Undead Officer
(29404, 43575, 48.63, 1, 0, 1, 1), -- Savage Hill Scavenger
(26655, 43575, 48.57, 1, 0, 1, 1), -- High Cultist Zangus
(24644, 43575, 48.39, 1, 0, 1, 1), -- Harpoon Master Yavus
(28108, 43575, 48.28, 1, 0, 1, 1), -- Bonescythe Ravager
(26480, 43575, 48.15, 1, 0, 1, 1), -- Magnataur Youngling
(26923, 43575, 47.83, 1, 0, 1, 1), -- Overseer Brunon
(27676, 43575, 47.78, 1, 0, 1, 1), -- Silverbrook Defender
(28101, 43575, 47.73, 1, 0, 1, 1), -- Blighted Corpse
(24116, 43575, 47.69, 1, 0, 1, 1), -- Winterskorn Scout
(29826, 43575, 47.37, 1, 0, 1, 1), -- Drakkari Medicine Man
(29697, 43575, 47.37, 1, 0, 1, 1), -- Drakuru Prophet
(25601, 43575, 47.06, 1, 0, 1, 1), -- Prince Valanar
(26623, 43575, 47.06, 1, 0, 1, 1), -- Scourge Brute
(30144, 43575, 46.88, 1, 0, 1, 1), -- Restless Frostborn Ghost
(23794, 43575, 46.81, 1, 0, 1, 1), -- North Fleet Medic
(28918, 43575, 46.67, 1, 0, 1, 1), -- Drek'Maz
(26943, 43575, 45.83, 1, 0, 1, 1), -- Battered Drakkari Berserker
(25430, 43575, 45.45, 1, 0, 1, 1), -- Magmothregar
(28601, 43575, 45.45, 1, 0, 1, 1), -- High Cultist Herenn
(25427, 43575, 45.24, 1, 0, 1, 1), -- Kaganishu
(29819, 43575, 45.07, 1, 0, 1, 1), -- Drakkari Lancer
(26694, 43575, 44.83, 1, 0, 1, 1), -- Ymirjar Dusk Shaman
(23674, 43575, 44.74, 1, 0, 1, 1), -- Iron Rune Sage
(26348, 43575, 44.66, 1, 0, 1, 1), -- Iron Thane Argrum
(28565, 43575, 44.44, 1, 0, 1, 1), -- Decaying Ghoul
(30864, 43575, 44.44, 1, 0, 1, 1), -- Shandaral Hunter Spirit
(32267, 43575, 44.44, 1, 0, 1, 1), -- Animated Laborer
(28961, 43575, 44.44, 1, 0, 1, 1), -- Titanium Siegebreaker
(32238, 43575, 44.21, 1, 0, 1, 1), -- Bitter Initiate
(25800, 43575, 44.12, 1, 0, 1, 1), -- Clam Master K
(23663, 43575, 44.09, 1, 0, 1, 1), -- Winterskorn Shield-Maiden
(25712, 43575, 44.07, 1, 0, 1, 1), -- Warbringer Goredrak
(26202, 43575, 44.01, 1, 0, 1, 1), -- Ziggurat Defender
(30179, 43575, 43.81, 1, 0, 1, 1), -- Twilight Apostle
(25429, 43575, 43.75, 1, 0, 1, 1), -- Magmoth Forager
(24398, 43575, 43.37, 1, 0, 1, 1), -- Steel Gate Excavator
(32467, 43575, 43.24, 1, 0, 1, 1), -- Skeletal Reaver
(24015, 43575, 42.99, 1, 0, 1, 1), -- Winterskorn Defender
(27554, 43575, 42.86, 1, 0, 1, 1), -- Injured Drakkari Refugee
(26828, 43575, 42.86, 1, 0, 1, 1), -- Magister Keldonus
(25432, 43575, 42.7, 1, 0, 1, 1), -- Mate of Magmothregar
(28022, 43575, 42.67, 1, 0, 1, 1), -- Carrion Eater
(27410, 43575, 42.36, 1, 0, 1, 1), -- Scourge Siegesmith
(38032, 43575, 42.31, 1, 0, 1, 1), -- Crown Sprayer
(26708, 43575, 42.18, 1, 0, 1, 1), -- Silverbrook Villager
(27370, 43575, 42.11, 1, 0, 1, 1), -- Vengeful Geist
(27961, 43575, 41.94, 1, 0, 1, 1), -- Dark Rune Worker
(31693, 43575, 41.94, 1, 0, 1, 1), -- Stormforged Saboteur
(25448, 43575, 41.82, 1, 0, 1, 1), -- Curator Insivius
(26679, 43575, 41.82, 1, 0, 1, 1), -- Silverbrook Trapper
(23665, 43575, 41.67, 1, 0, 1, 1), -- Winterskorn Raider
(28575, 43575, 41.67, 1, 0, 1, 1), -- Rhunok's Tormentor
(27360, 43575, 41.6, 1, 0, 1, 1), -- Smoldering Skeleton
(24249, 43575, 41.4, 1, 0, 1, 1), -- Dragonflayer Soulreaver
(24954, 43575, 41.3, 1, 0, 1, 1), -- Aged Magnataur
(26496, 43575, 41.18, 1, 0, 1, 1), -- Wind Trader Mu'fah
(28538, 43575, 41.18, 1, 0, 1, 1), -- Cultist Saboteur
(26624, 43575, 41.18, 1, 0, 1, 1), -- Wretched Belcher
(26802, 43575, 41.07, 1, 0, 1, 1), -- Alliance Ranger
(27286, 43575, 41.07, 1, 0, 1, 1), -- Dreadbone Invader
(27355, 43575, 41.03, 1, 0, 1, 1), -- Rothin the Decaying
(27799, 43575, 41.03, 1, 0, 1, 1), -- Scourge Technician
(23672, 43575, 40.91, 1, 0, 1, 1), -- Iron Rune Worker
(23660, 43575, 40.76, 1, 0, 1, 1), -- Dragonflayer Thane
(23653, 43575, 40.74, 1, 0, 1, 1), -- Winterskorn Spearman
(28641, 43575, 40.74, 1, 0, 1, 1), -- Blighted Corpse
(26425, 43575, 40.67, 1, 0, 1, 1), -- Drakkari Warrior
(30597, 43575, 40.65, 1, 0, 1, 1), -- Spiked Ghoul
(26891, 43575, 40.59, 1, 0, 1, 1), -- Undead Miner
(24216, 43575, 40.51, 1, 0, 1, 1), -- Dragonflayer Berserker
(25678, 43575, 40.38, 1, 0, 1, 1), -- Doctor Razorgrin
(27279, 43575, 40.24, 1, 0, 1, 1), -- Snowplain Shaman
(23644, 43575, 40, 1, 0, 1, 1), -- Mur'ghoul Flesheater
(23992, 43575, 40, 1, 0, 1, 1), -- Putrid Wight
(27005, 43575, 40, 1, 0, 1, 1), -- Chilltusk
(25768, 43575, 40, 1, 0, 1, 1), -- Lord Kryxix
(28848, 43575, 40, 1, 0, 1, 1), -- Prophet of Har'koa
(34839, 43575, 39.95, 1, 0, 1, 1), -- Kvaldir Mist Binder
(23666, 43575, 39.68, 1, 0, 1, 1), -- Winterskorn Berserker
(27927, 43575, 39.62, 1, 0, 1, 1), -- Dragonflayer Guardian
(29237, 43575, 39.58, 1, 0, 1, 1), -- Gundrak Fire-eater
(27401, 43575, 39.58, 1, 0, 1, 1), -- Risen Wintergarde Miner
(27287, 43575, 39.5, 1, 0, 1, 1), -- Mindless Wight
(26639, 43575, 39.47, 1, 0, 1, 1), -- Drakkari Shaman
(31139, 43575, 39.39, 1, 0, 1, 1), -- Pustulent Horror
(27969, 43575, 39.39, 1, 0, 1, 1), -- Dark Rune Giant
(26803, 43575, 39.39, 1, 0, 1, 1), -- Horde Cleric
(28836, 43575, 39.39, 1, 0, 1, 1), -- Stormforged Runeshaper
(27859, 43575, 39.39, 1, 0, 1, 1), -- Vanthryn the Merciless
(26830, 43575, 39.33, 1, 0, 1, 1), -- Risen Drakkari Death Knight
(26553, 43575, 39.31, 1, 0, 1, 1), -- Dragonflayer Fanatic
(29654, 43575, 39.29, 1, 0, 1, 1), -- Drakuru Blood Drinker
(26762, 43575, 39.29, 1, 0, 1, 1), -- Captain Emmy Malin
(31161, 43575, 39.29, 1, 0, 1, 1), -- Sapph
(26344, 43575, 39.23, 1, 0, 1, 1), -- Indu'le Warrior
(29413, 43575, 39.15, 1, 0, 1, 1), -- Snowblind Digger
(27424, 43575, 39.09, 1, 0, 1, 1), -- Conquest Hold Marauder
(29614, 43575, 39.02, 1, 0, 1, 1), -- Onslaught Darkweaver
(23655, 43575, 39, 1, 0, 1, 1), -- Winterskorn Bonegrinder
(27797, 43575, 38.98, 1, 0, 1, 1), -- Tattered Abomination
(30409, 43575, 38.71, 1, 0, 1, 1), -- Apprentice Osterkilgr
(23963, 43575, 38.71, 1, 0, 1, 1), -- Sergeant Lorric
(32257, 43575, 38.68, 1, 0, 1, 1), -- Scourge Converter
(31738, 43575, 38.61, 1, 0, 1, 1), -- Cultist Corrupter
(29553, 43575, 38.57, 1, 0, 1, 1), -- Garm Watcher
(25839, 43575, 38.55, 1, 0, 1, 1), -- Northsea Mercenary
(23658, 43575, 38.46, 1, 0, 1, 1), -- Dragonflayer Death Weaver
(26481, 43575, 38.46, 1, 0, 1, 1), -- Magnataur Alpha
(29427, 43575, 38.33, 1, 0, 1, 1), -- Captive Vrykul
(27278, 43575, 38.33, 1, 0, 1, 1), -- Snowplain Zealot
(26455, 43575, 38.27, 1, 0, 1, 1), -- Moonrest Highborne
(28402, 43575, 38.13, 1, 0, 1, 1), -- Claw of Har'koa
(28081, 43575, 38.06, 1, 0, 1, 1), -- Frenzyheart Scavenger
(24485, 43575, 37.98, 1, 0, 1, 1), -- Servitor Shade
(27823, 43575, 37.97, 1, 0, 1, 1), -- Naxxramas Dreadguard
(27343, 43575, 37.88, 1, 0, 1, 1), -- Bloodpaw Shaman
(31691, 43575, 37.78, 1, 0, 1, 1), -- Faceless Lurker
(26410, 43575, 37.78, 1, 0, 1, 1), -- Rune-Smith Kathorn
(23662, 43575, 37.66, 1, 0, 1, 1), -- Winterskorn Woodsman
(23796, 43575, 37.65, 1, 0, 1, 1), -- Iron Rune Binder
(30243, 43575, 37.55, 1, 0, 1, 1), -- Njorndar Spear-Sister
(29793, 43575, 37.5, 1, 0, 1, 1), -- Frostfeather Witch
(27007, 43575, 37.5, 1, 0, 1, 1), -- Iceshatter
(27334, 43575, 37.5, 1, 0, 1, 1), -- Onslaught Commander Iustus
(27533, 43575, 37.5, 1, 0, 1, 1), -- Frigid Geist
(32268, 43575, 37.5, 1, 0, 1, 1), -- Cult Taskmaster
(27729, 43575, 37.5, 1, 0, 1, 1), -- Enraging Ghoul
(32502, 43575, 37.5, 1, 0, 1, 1), -- Ravaged Ghoul
(26669, 43575, 37.39, 1, 0, 1, 1), -- Ymirjar Savage
(24469, 43575, 37.25, 1, 0, 1, 1), -- Magnataur Huntress
(27232, 43575, 37.25, 1, 0, 1, 1), -- Captain Shely
(26295, 43575, 37.25, 1, 0, 1, 1), -- Magnataur Patriarch
(31258, 43575, 37.24, 1, 0, 1, 1), -- Ymirheim Chosen Warrior
(31155, 43575, 37.18, 1, 0, 1, 1), -- Malefic Necromancer
(23983, 43575, 37.04, 1, 0, 1, 1), -- North Fleet Marine
(23866, 43575, 37.04, 1, 0, 1, 1), -- North Fleet Sailor
(24871, 43575, 37.04, 1, 0, 1, 1), -- Risen Vrykul Ancestor
(23656, 43575, 36.87, 1, 0, 1, 1), -- Dragonflayer Rune-Seer
(28747, 43575, 36.84, 1, 0, 1, 1), -- Quetz'lun Worshipper
(24461, 43575, 36.84, 1, 0, 1, 1), -- Chillmere Oracle
(25684, 43575, 36.7, 1, 0, 1, 1), -- Talramas Abomination
(26357, 43575, 36.62, 1, 0, 1, 1), -- Frostpaw Warrior
(28837, 43575, 36.59, 1, 0, 1, 1); -- Stormforged Sentinel

-- #1051
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` IN (35476, 35478);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 35476, 0, 0, 27, 0, 79, 4, 0, 0, 0, '', 'Drums of Battle - Do not allow usage at Level 80'),
(17, 0, 35478, 0, 0, 27, 0, 79, 4, 0, 0, 0, '', 'Drums of Restoration - Do not allow usage at Level 80');

-- fix broken RequiredNpcOrGoId1 in quest 13828 (Master of the Melee), Horde was already working
UPDATE quest_template SET RequiredNpcOrGo1 = 33973 WHERE id = 13828 LIMIT 1;

-- #1008: Twin Valkyrs Loot Balancing
-- reduce maxcount for eydis darkbane
UPDATE `creature_loot_template` SET `maxcount` = '1' WHERE `creature_loot_template`.`entry` =35347 AND `creature_loot_template`.`item`=1 LIMIT 1;
UPDATE `creature_loot_template` SET `maxcount` = '1' WHERE `creature_loot_template`.`entry` =35349 AND `creature_loot_template`.`item`=1 LIMIT 1;

-- raise maxcount for fjola lightbane
UPDATE `creature_loot_template` SET `maxcount` = '3' WHERE `creature_loot_template`.`entry` =35352 AND `creature_loot_template`.`item`=1 LIMIT 1;
UPDATE `creature_loot_template` SET `maxcount` = '3' WHERE `creature_loot_template`.`entry` =35350 AND `creature_loot_template`.`item`=1 LIMIT 1;

-- remove recipes from fjola
DELETE FROM `creature_loot_template` WHERE `entry` IN (35352, 35350) AND `mincountOrRef` IN (-34314, -34328);

-- #1068/#1069 Quest-Pooling and Classmasks for Argent Tournament
-- Sunreaver / Silver Covenant Quest Pooling
SET @pool_narasi := 20300;
SET @pool_savinia := @pool_narasi+1;
SET @pool_girana := @pool_narasi+2;
SET @pool_tylos := @pool_narasi+3;

DELETE FROM `pool_template` WHERE `entry` IN (@pool_narasi, @pool_savinia, @pool_girana, @pool_tylos);
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@pool_narasi, 1, 'Narasi Snowdawn <The Silver Covenant> - Daily Quests'),
(@pool_savinia, 1, 'Savinia Loresong <The Silver Covenant> - Daily Quests'),
(@pool_girana, 1, 'Girana the Blooded <The Sunreavers> - Daily Quests'),
(@pool_tylos, 1, 'Tylos Dawnrunner <The Sunreavers> - Daily Quests');

DELETE FROM `pool_quest` WHERE `entry` IN (SELECT quest FROM creature_questrelation WHERE id IN (34771, 34912, 34880, 34914));
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
-- Narasi Snowdawn
(14074, @pool_narasi, 'A Leg Up'),
(14152, @pool_narasi, 'Rescue at Sea'),
(14080, @pool_narasi, 'Stop The Aggressors'),
(14077, @pool_narasi, 'The Light''s Mercy'),
(14096, @pool_narasi, 'You''ve Really Done It This Time, Kul'),
-- Savinia
(14076, @pool_savinia, 'Breakfast of Champions'),
(14090, @pool_savinia, 'Gormok Wants His Snobolds'),
(14112, @pool_savinia, 'What Do You Feed a Yeti, Anyway?'),
-- Girana
(14143, @pool_girana, 'A Leg Up'),
(14136, @pool_girana, 'Rescue at Sea'),
(14140, @pool_girana, 'Stop The Aggressors'),
(14144, @pool_girana, 'The Light''s Mercy'),
(14142, @pool_girana, 'You''ve Really Done It This Time, Kul'),
-- Tylos
(14092, @pool_tylos, 'Breakfast of Champions'),
(14141, @pool_tylos, 'Gormok Wants His Snobolds'),
(14145, @pool_tylos, 'What Do You Feed a Yeti, Anyway?');

-- Argent Tournament: Separate Death-Knight quests from those of other classes
-- Set Quests to DK only
UPDATE `quest_template` SET `RequiredClasses` = 32 WHERE `id` IN 
(13863, 13864, -- Battle Before The Citadel
 13788, 13812, -- Threat From Above
 13793, 13814, -- Among The Champions
 13791, 13813); -- Taking Battle To The Enemy
 
-- Set quests to NON-DK only
UPDATE `quest_template` SET `RequiredClasses` = 1|2|4|8|16|64|128|256|1024 WHERE `id` IN 
(13790, 13811, -- Among the Champions
 13861, 13862, -- Battle Before The Citadel
 13682, 13809, -- Threat From Above
 13789, 13810); -- Taking Battle To The Enemy

-- no issue: fix gnomish army knife resurrection spells availability to grand master engineers only
-- Gnomish Army Knife Resurrection (54732) is restricted to Grand Master Engineers (51306)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` = 54732 LIMIT 1;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 54732, 0, 0, 25, 0, 51306, 0, 0, 0, 1335, '', 'Only Grand-Masters in Engineering can use the Gnomish Army Knife to resurrect players.');
