-- no issue: Fix Stranglethorn fever quest item drop; quest template does not request the item, so the item would never actually drop in the designated area.
UPDATE creature_loot_template SET ChanceOrQuestChance = ABS(ChanceOrQuestChance) WHERE item = 2799;
