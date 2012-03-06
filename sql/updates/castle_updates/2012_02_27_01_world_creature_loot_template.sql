-- insert missing jewelcrafting recipe (design: infused twilight opal) for anub'arak 5hc
DELETE FROM creature_loot_template WHERE item = 41796;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE
(31610, 41796, 100, 1, 0, 1, 1);
