-- #988: Heroic Presence (Draenei racial) stacks - but it mustn't.
-- @DorianGrey:
-- We had already fixed this before, but it got lost, somehow.
-- I've extended the previous fix to deal with both auras.
-- @see http://old.wowhead.com/spell=6562
-- @see http://old.wowhead.com/spell=28878
-- Who was stupid enough to created two auras doing the same thing ? ...

DELETE FROM spell_group WHERE `id` = 6562;
INSERT INTO spell_group (`id`,`spell_id`) VALUES (6562, 6562),(6562, 28878);

DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 6562;
INSERT INTO `spell_group_stack_rules` (group_id, stack_rule) VALUES (6562, 1);