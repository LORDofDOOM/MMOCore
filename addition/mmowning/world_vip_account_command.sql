DELETE FROM `command` WHERE name IN ('vip', 'vip add');
INSERT INTO `command` VALUES
('vip', 0, 'Syntax: .vip\r\n\r\nCheck if your account is VIP.'),
('vip add', 3, 'Syntax: .vip add $account $days\r\n\r\nSet or update a VIP account.');

-- UPDATE `command` SET security = 0 WHERE name IN ('lookup tele', 'tele', 'bank');
UPDATE `command` SET security = 0 WHERE name IN ('bank');
