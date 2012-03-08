-- Allow players to unbind instance
UPDATE command SET security = '0' WHERE name = 'instance unbind';