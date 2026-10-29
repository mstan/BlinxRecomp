# Playable public alpha — 2026.10.29.1

**Tested through World 2. End-to-end completion is not guaranteed. Please
[file GitHub issues](https://github.com/mstan/BlinxRecomp/issues).**

This Windows x64 alpha adds a RAM-backed disposable FATX utility cache to the
accepted native architecture build. Native saves and console identity remain
persistent. The 750 MiB utility volume allocates pages on demand. In one short
matched Stage 1 pair, process writes fell from 28.7 MB to 4.2 KB over 15 seconds,
with about 570 MiB more private memory and essentially 30 original updates/s in
both modes. This measures process I/O, not physical SSD writes, and establishes
no general CPU or FPS speedup. Thanks to apexJCL for the design inspiration.

Includes adaptive widescreen, resolution through 2160p, FXAA, supersampling,
filtering, controller selection, camera inversion and blended 60/120 Hz
presentation at the original simulation rate. Optional off-by-default gameplay
mods remove the ten-minute stage limit, expand stock to 20, or keep infinite
time controls while suppressing non-heart crystals (except the final boss).
Hearts are unchanged. See README and docs/CHEATS.md for reserve/save limits.

The ZIP includes Start Game.exe, launcher, native runtime, embedded Python,
runtime DLLs, corresponding project source and prepared dependency source.
Supply your own supported NTSC-U ISO/XISO or complete extracted files.
No game media, keys, EEPROM, personal saves or used QA profiles are bundled.
Preserve your data folder, console identity, external saves and imported-disc
cache when updating. Linux playable releases are deferred.
