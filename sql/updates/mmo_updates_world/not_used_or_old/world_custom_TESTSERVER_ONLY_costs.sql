-- All to 0 (testserver)
UPDATE IGNORE npc_vendor SET ExtendedCost = 0;
UPDATE IGNORE item_template SET BuyPrice = 1;
UPDATE IGNORE item_template SET SellPrice = 1;