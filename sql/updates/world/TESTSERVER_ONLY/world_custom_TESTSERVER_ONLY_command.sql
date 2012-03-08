-- Allow players to unbind instance
UPDATE command SET security = '0' WHERE name = 'instance unbind';
-- Allow players modify speed
UPDATE command SET security = '0' WHERE name = 'modify aspeed';
UPDATE command SET security = '0' WHERE name = 'modify speed';
UPDATE command SET security = '0' WHERE name = 'repairitems';
UPDATE command SET security = '0' WHERE name = 'revive';
UPDATE command SET security = '0' WHERE name = 'maxskill';
UPDATE command SET security = '0' WHERE name = 'additem';
