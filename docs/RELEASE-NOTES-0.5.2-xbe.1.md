# Blinx 0.5.2-xbe.1 — experimental XBE compatibility test

This Windows prerelease attempts other Blinx executables, including USA disc
rips rejected by 0.5.1. It uses the existing compiled native game program with
the selected XBE. A different executable can stop or behave incorrectly; this
does not establish support for another revision.

1. Extract the ZIP into a separate folder and open **Start Game.exe**.
2. Choose **Skip** for console setup, then select your ISO/XISO or `default.xbe`.
3. An unknown Blinx executable shows **Experimental XBE - unverified**. Start
   normally and report whether it reaches the title screen and gameplay.
4. If it stops, use **Copy diagnostic ZIP** or **Open report folder** in the
   report window. After a normal exit the diagnostic location is also shown.
   Attach that ZIP, plus a brief description of what happened.

Reports include the actual XBE hash, public header metadata, section hashes,
differences from the supported file, selected mods and the runtime failure.
They exclude original game files, console keys and native saves. No scripts
are needed from the tester.

Unknown executables use persistent, separate test saves. Existing normal saves,
external save folders and HDD images are preserved and are not used by these
attempts. Keep the experimental package's `data` folder when repeating a test.

Fresh packages retain adaptive **Fit to screen** and the 0.5.1 graphics defaults.
This remains an early beta: the regular playtest scope is through World 2,
and end-to-end completion is not guaranteed. The stable 0.5.1 release remains
available separately.
