# DS2Fix

Random fixes for Dark Souls II (WIP)

## How to use

1. go to the [release page](https://github.com/eur0pa/DS2Fix/releases) and download the latest version
2. unpack anywhere
3. run ds2fix.exe
4. either click 'Apply' before launching Dark Souls II or after, it doesn't matter

## Current fixes

###>30fps durability bug

Uses the formula shown [here](http://www.reddit.com/r/DarkSouls2/comments/2v87oz/durability_bug_not_solved_in_ps4xbox_one_version/cofw417). Dark Souls II has its physics engine calibrated to 30fps, and as such it will do 2 ticks for every cycle when running at 60fps (place a breakpoint @ 0x76FFDA and watch that being called twice); we'll get the durability value and recompute it for the current FPS snapshot (located @ DarkSoulsII.exe+0x11A36C4 + 0xC4).

## Incoming fixes

* +14 save corruption
* namecrash protection
