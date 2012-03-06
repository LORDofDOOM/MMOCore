-- #1071: glyph of shadowflame should only proc when dealing damage to the target (fixing the range issue)
UPDATE `spell_proc_event` SET `procEx` = procEx|262144 WHERE`entry` = 63310;
