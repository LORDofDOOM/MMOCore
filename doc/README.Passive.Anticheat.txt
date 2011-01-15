Passive Anticheat:

I have been writing this Passive Anticheat, mainly for speed and fly hacks. Please don't expect a excellent quaility of detection in a server-side anticheat, since the retail way for hacks detection is client-side using WARDEN. There are vars like the time between the movements packets that can be altered by the lag, so false reports can be made. 

How do I detect cheaters?

The best way to detect cheaters is controling: the amount of warnings and the average, for example if player A has 153 warnings but he has it that amount for 5 minutes, and then increases just to 160 -> it's a false report. In the other hand, if it increases to 180 in 2 minutes, probably it is a cheater. To sum up, this anticheat is for helping you to detect possible cheaters and investigate them ingame for a confirmation.


Exceptions for Speed-Hack Detection

To avoid false reports we skip this cases. If it is:

GameMaster
In flight
In transport
UnderWater
using /follow command.



How does the speed hack detection work?

Distance 2D is calculated, server position up to new position from the packet.
It's calculated the diff time between the new packet (movement packet time) and the last packet.
The server-side speed rate is calculated (this is not modified if you are cheating).
The client-side speed rate is calculated.
Finally, it's compared the server-side speed rate against the client-side speed-rate, if the last var is higher then a report is elaborated.


How does the fly hack detection work?

The movementflags are checked, for example if the player has the movementflag which allows the fly movements and doesn´t has an aura to do it, we make a report.


How does the walk on water hack detection work?

The movementflags are checked, for example if the player has the movementflag which allows the walk on water movements and doesn´t has an aura to do it, we make a report.


When are the warnings reseted?

When the dailies quest reset.


Commands:
.anticheat global: returns the top three cheat reports amount, and the top
three with highest average.
.anticheat delete: deletes a especific player' reports or all reports from
tables.
.anticheat handle: turns off/on the anticheat system.
.anticheat player: returns the amount and average of a especific player.


Config Options:
Anticheat.Enable: Self Explanatory.



TO-DO:
Use Prepared-Statements. (DONE)
Add more commands.
Add more config options.
Better way to reset warnings.
Better way to get the time.
