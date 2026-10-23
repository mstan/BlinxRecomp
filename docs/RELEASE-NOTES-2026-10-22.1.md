# Windows preview hotfix - 2026-10-22.1

This hotfix ships the exact native executable accepted in the owner's latest
Stage 2-1 playtest, with the existing launcher and visual defaults.

- Reuse unchanged texture preparations using shared resource-write tracking.
  The owner reported substantially smoother stage-entry gameplay. A bounded
  Stage 2-1 comparison measured about 16.3% less process CPU per game update;
  occasional hitches remain under investigation.
- Correct character-face and whisker transparency, including the shop model.
- Recover the missing original projectile-effect continuation that could crash
  combat in Stage 1-2.
- Include opt-in tester diagnostics through **Start with Diagnostics.cmd**.
  Reports can contain local paths and input/session information; review them
  before sharing. Diagnostics are not enabled by ordinary launch.

Extract the Windows ZIP to a writable folder and open **Start Game.cmd**.
Supply your own supported NTSC-U game files; keep the complete extracted asset
folder beside the selected XBE. Optional console setup can be skipped for a new
local console. No large HDD image is generated automatically.

When updating, retain your previous package and its `data` folder, chosen save
storage and any `<image>.xboxllr` profile. Keep your original console identity
with your native saves. Save states are disposable and build-specific.

Adaptive widescreen, resolution, supersampling and interpolation defaults remain
available through Mods. Gameplay timing is unchanged; no guest interpreter
fallback is introduced. Existing assisted campaign coverage is retained;
this hotfix is not an exhaustive new campaign pass. Linux and direct ISO import
remain deferred, and Xbox Live is unavailable.

The release includes project source at its recorded recursive framework pin,
prepared renderer/launcher/DSP source, notices, provenance and checksums.
Original game files, console dumps, signing keys, saves and used profiles are
not bundled. Project contributions are GPL-3.0-or-later; see
[rights and license scope](../RIGHTS-NOTICE.md) and [credits](../CREDITS.md).
