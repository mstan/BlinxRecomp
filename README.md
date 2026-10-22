# Blinx: The Time Sweeper Recompiled

Play Blinx on PC with adaptive widescreen, higher internal resolution and
controller support through an Xbox-themed launcher. BlinxRecomp uses
[xboxllr](https://github.com/mstan/xboxllr), the shared Xbox Low Level Recompiler.

![Blinx exploring a street in adaptive widescreen, with crystals at left and trash at right](docs/screenshots/adaptive-gameplay.jpg)

*An owner playtest of the widescreen development preview. [Image credits](docs/screenshots/NOTICE.md).*

[Features](#features) | [Getting started](#getting-started) |
[Controls and graphics](#controls-and-graphics) | [Credits](CREDITS.md)

## Features

| Feature | Available in the development preview |
| --- | --- |
| Adaptive widescreen | Fit gameplay to the window, or choose a fixed aspect through 32:9. The camera expands, with HUD elements anchored to the appropriate edges. |
| Higher resolution | Native internal heights of 1080p, 1440p, 1800p and 2160p, independently of the window size. |
| Image quality | Optional FXAA, 2x2 or 3x3 supersampling, and anisotropic texture filtering. |
| Smoother presentation | Optional blended 60/120 Hz presentation while gameplay keeps its original timing. |
| Controller selection | The shared recomp-ui Player 1 card detects connected controllers and remembers the selected device. Keyboard input is also available. |
| Original saves | Persistent in-game saves, including fresh-process loading of the assisted campaign's completed save. |
| Time controls | Original activation and lifecycle checks for all five time controls, including REC's clone playback. |

Visual enhancements are optional mods. Original timing remains the default;
interpolation can add double edges and one source frame of latency. Actual
higher-rate gameplay is not available yet. See the
[graphics guide](docs/GRAPHICS-MODS.md) for options and tradeoffs.

## Getting started

**Windows x64 development preview.** Self-contained downloads are being prepared;
native Linux builds are not available yet. Current setup still uses developer
tools and private preparation inputs. The goal for the release is to require
your own matching game dump and generate the remaining supported setup locally.

You need your own **Blinx: The Time Sweeper (NTSC-U)** game files. Other regions
and revisions have not been qualified. Original game files and generated game
code are not included here.

See the shared [setup and required-file guide](https://github.com/mstan/xboxllr/blob/master/docs/SETUP.md)
for supported XBE hashes, private input preparation, optional original-console
imports and save preservation. ISO-only setup is the release target, not yet
available. Missing console files should normally be irrelevant to new
single-player play once that virtual-profile path is qualified; existing Xbox
save imports can require the original identity. The game does not boot a
Microsoft kernel; today's owned-kernel requirement is offline signing setup.

For an already prepared development checkout, build the gameplay profile and
launcher, then open the launcher:

```powershell
python tools/game.py build --mode play
python tools/game.py build-launcher
python tools/game.py run --mode play --launcher
```

For first-time preparation, prerequisites and revision verification, follow the
[developer setup guide](docs/DEVELOPMENT.md). It also documents the separate
correctness-reference builds. The framework's
[release readiness plan](https://github.com/mstan/xboxllr/blob/master/docs/RELEASE-READINESS.md) tracks packaging,
platform support and distribution requirements.

## Controls and graphics

Select your controller in **Player 1**, choose enhancements in **Mods**, then
launch the game. Individual button remapping is not exposed in this preview.
Use `--launcher` to reopen settings if you previously selected Skip launcher.

Adaptive widescreen follows the window's aspect. Crystals sit at the left,
trash at the right, and time controls remain centered. Gameplay and the Results
replay expand; menus and the Results overlay retain their centered 4:3 layout.
Higher resolution and supersampling cost more GPU time, especially at ultrawide
aspects. Start with original presentation timing when comparing performance.

## Current playtest scope

An assisted campaign pass reached all nine rounds, the ending, original save
and title return. Cheats and teleports accelerated that pass; it does not prove
every natural combat encounter, puzzle or optional route. The owner has also
briefly playtested the widescreen preview. Remaining limits and reproducible
evidence are recorded in [current status](docs/STATUS.md) and the
[framework burndown](xboxlle/docs/BLINX-BURNDOWN.md).

The tested fast profile compiles the original CPU code ahead of time and uses
shared native services. It has no guest x86 interpreter fallback. Unreached
code can still expose an unsupported boundary; native coverage is not a promise
that every possible game path has been exercised.

## Credits and license

Development and integration: **Matthew Stanley**. Special thanks to
**sp00nz / sp00nznet**, **Phobos**, **NoRain** and the **xemu contributors** for
the recompiler, renderer, audio and hardware work this project draws from.
See [Credits](CREDITS.md) and the
[component attribution](THIRD_PARTY_ATTRIBUTION.md) for links and exact scope.

Owner-original material follows [PolyForm Noncommercial 1.0.0](LICENSE),
subject to [license scope](LICENSE-SCOPE.md). Third-party work retains its own
terms. The current private reference build has unresolved distribution
restrictions; this README does not announce a redistributable executable.
Blinx and its original assets belong to their respective rights holders.
