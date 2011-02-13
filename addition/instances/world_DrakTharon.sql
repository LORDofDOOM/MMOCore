-- Drak'Tharon Heroic Loot Emblems
UPDATE creature_loot_template SET item = 40752 WHERE entry IN (SELECT lootid FROM creature_template WHERE entry IN (31362,31350,31360,31349)) AND item = 47241;

-- Insert Trash Loot Blues
-- DELETE FROM creature_loot_template WHERE entry IN (
-- 31339, -- Drakkari Guardian
-- 31347, -- Ghoul Tormentor
-- 31337, -- Drakkari Bat
-- 31357, -- Scourge Brute
-- 31363, -- Wretched Belcher
-- 31336, -- Darkweb Recluse
-- 31359, -- Scourge Reanimator
-- 31343, -- Drakkari Scytheclaw
-- 31355, -- Risen Drakkari Warrior
-- 31354, -- Risen Drakkari Soulmage
-- 31342, -- Risen Drakkari Handler
-- 31351, -- Risen Drakkari Bat Rider
-- 31345, -- Drakkari Shaman
-- 31340, -- Drakkari Gutripper
-- 31352, -- Risen Drakkari Death Knight
-- 31338, -- Drakkari Commander
-- 31346) -- Flesheating Ghoul
--  AND item IN (
--  37799, -- Reanimator's Cloak
--  37800, -- Aviary Guardsman's Hauberk
--  37801);-- Waistguard of the Risen Knight

-- Chances from wowhead
Replace INTO creature_loot_template VALUES
-- Reanimator's Cloak
(31346, 37799, 1.2, 1, 0, 1, 1), -- Flesheating Ghoul
(31340, 37799, 1.1, 1, 0, 1, 1), -- Drakkari Gutripper
(31347, 37799, 1.1, 1, 0, 1, 1), -- Ghoul Tormentor
(31352, 37799, 1.1, 1, 0, 1, 1), -- Risen Drakkari Death Knight
(31354, 37799, 1.1, 1, 0, 1, 1), -- Risen Drakkari Soulmage
(31357, 37799, 1.1, 1, 0, 1, 1), -- Scourge Brute
(31359, 37799, 1.1, 1, 0, 1, 1), -- Scourge Reanimator
(31342, 37799, 1, 1, 0, 1, 1), -- Risen Drakkari Handler
(31355, 37799, 1, 1, 0, 1, 1), -- Risen Drakkari Warrior
(31363, 37799, 1, 1, 0, 1, 1), -- Wretched Belcher
(31351, 37799, 0.9, 1, 0, 1, 1), -- Risen Drakkari Bat Rider
(31338, 37799, 0.7, 1, 0, 1, 1), -- Drakkari Commander
(31336, 37799, 0.6, 1, 0, 1, 1), -- Darkweb Recluse
(31339, 37799, 0.5, 1, 0, 1, 1), -- Drakkari Guardian
(31343, 37799, 0.5, 1, 0, 1, 1), -- Drakkari Scytheclaw
(31345, 37799, 0.5, 1, 0, 1, 1); -- Drakkari Shaman
-- (31337, 37799, 0.1, 1, 0, 1, 1); -- Drakkari Bat

-- Aviary Guardsman's Hauberk
Replace INTO creature_loot_template VALUES
(31351, 37800, 1.5, 1, 0, 1, 1), -- Risen Drakkari Bat Rider
(31346, 37800, 1.2, 1, 0, 1, 1), -- Flesheating Ghoul
(31342, 37800, 1.2, 1, 0, 1, 1), -- Risen Drakkari Handler
(31340, 37800, 1.1, 1, 0, 1, 1), -- Drakkari Gutripper
(31352, 37800, 1.1, 1, 0, 1, 1), -- Risen Drakkari Death Knight
(31355, 37800, 1.1, 1, 0, 1, 1), -- Risen Drakkari Warrior
(31347, 37800, 1  , 1, 0, 1, 1), -- Ghoul Tormentor
(31354, 37800, 1  , 1, 0, 1, 1), -- Risen Drakkari Soulmage
(31359, 37800, 1  , 1, 0, 1, 1), -- Scourge Reanimator
(31363, 37800, 1  , 1, 0, 1, 1), -- Wretched Belcher
(31357, 37800, 0.9, 1, 0, 1, 1), -- Scourge Brute
(31338, 37800, 0.7, 1, 0, 1, 1), -- Drakkari Commander
(31336, 37800, 0.6, 1, 0, 1, 1), -- Darkweb Recluse
(31339, 37800, 0.5, 1, 0, 1, 1), -- Drakkari Guardian
(31345, 37800, 0.5, 1, 0, 1, 1), -- Drakkari Shaman
(31343, 37800, 0.4, 1, 0, 1, 1), -- Drakkari Scytheclaw
(31337, 37800, 0.1, 1, 0, 1, 1), -- Drakkari Bat
-- Waistguard of the Risen Knight
(31346, 37801, 1.2, 1, 0, 1, 1), -- Flesheating Ghoul
(31340, 37801, 1.1, 1, 0, 1, 1), -- Drakkari Gutripper
(31352, 37801, 1.1, 1, 0, 1, 1), -- Risen Drakkari Death Knight
(31342, 37801, 1.1, 1, 0, 1, 1), -- Risen Drakkari Handler
(31354, 37801, 1.1, 1, 0, 1, 1), -- Risen Drakkari Soulmage
(31355, 37801, 1.1, 1, 0, 1, 1), -- Risen Drakkari Warrior
(31357, 37801, 1.1, 1, 0, 1, 1), -- Scourge Brute
(31359, 37801, 1.1, 1, 0, 1, 1), -- Scourge Reanimator
(31347, 37801, 1  , 1, 0, 1, 1), -- Ghoul Tormentor
(31351, 37801, 1  , 1, 0, 1, 1), -- Risen Drakkari Bat Rider
(31363, 37801, 0.9, 1, 0, 1, 1), -- Wretched Belcher
(31338, 37801, 0.7, 1, 0, 1, 1), -- Drakkari Commander
(31343, 37801, 0.7, 1, 0, 1, 1), -- Drakkari Scytheclaw
(31336, 37801, 0.6, 1, 0, 1, 1), -- Darkweb Recluse
(31339, 37801, 0.5, 1, 0, 1, 1), -- Drakkari Guardian
(31345, 37801, 0.5, 1, 0, 1, 1), -- Drakkari Shaman
(31337, 37801, 0.1, 1, 0, 1, 1); -- Drakkari Bat

-- set LootIDs to own CreatureID
-- CALL `sp_set_npc_lootid_bylist` ('31339,31347,31337,31357,31363,31336,31359,31343,31355,31354,31342,31351,31345,31340,31352,31338,31346',NULL);

-- Make Drakkari Guardian ignoring Aggro
UPDATE creature_template SET unit_flags = unit_flags | 2 WHERE entry IN (26620,31339);