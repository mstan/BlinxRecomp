# BlinxRecomp

Private original **NTSC-U Blinx: The Time Sweeper** game repository, using the
[xboxlle framework](https://github.com/mstan/xboxlle). The Xbox workspace follows
the PSX ecosystem layout:

```text
F:/Projects/xboxlle/
  xboxlle/                 shared framework checkout
  BlinxRecomp/             this game repository
    xboxlle/               framework submodule pinned to a reviewed commit
    config/revisions/      game identity, binding fingerprints and discovery evidence
    build/                 ignored generated code and executables
    local/                 ignored paths, state and validation evidence
```

Shared services stay in the framework. This repository owns the title profile,
revision selection, seeds, original-function recovery records, validation input
and build/launch entry points. The existing hash-bound binding generators still
live in the framework; their selected output fingerprints are owned here. New
titles get sibling repositories and reuse those shared services.

The working milestone has opening movies, real-time Demo play, Start interruption,
menu return and attract reentry. Accepted cadence debt is about 28.5–28.8 fps;
New Game is deferred. Full LLE input remains in progress: original startup now
creates three real driver objects and reaches page pinning. Neither that progress
nor this repository split claims complete LLE input or final attract qualification.
See [current validation](docs/STATUS.md) and the framework's
[serial burndown](xboxlle/docs/BLINX-BURNDOWN.md).

## Build and run on this machine

Python 3.11+, VS 2022 C++, CMake and Git are required. The framework submodule
and prepared donor dependencies retain their own notices and scope.

```powershell
git submodule update --init
# Populate ignored local/paths.json from local-paths.example.json.
py -3 tools/game.py doctor
py -3 tools/game.py build
py -3 tools/game.py test
py -3 tools/game.py run
```

The default `attract` mode uses native renderer/audio/input and the qualified
calendar/device reference implementations. `--mode input-lle` selects original
XAPI input and currently stops explicitly at the recorded page-pinning dependency.
Selections occur at build time. No runtime HLE/LLE switching is performed.

Owned NTSC inputs and private generated output are required. To reuse the current
hash-verified local output without retranslating it:

```powershell
py -3 tools/game.py adopt --from-dir ../wt-native-title-pilot/build/blinx-movie
```

For regeneration, `py -3 tools/game.py prepare` invokes the pinned Phobos-backed
framework generator with this repo's seeds, extents and selected bindings. It
requires the matching local donor checkout and original XDK symbol analysis.
Prepared renderer/DSP/ADPCM and Unicorn paths point to existing local dependency
builds; their preparation commands remain in the framework's
[native pilot documentation](xboxlle/pilots/native-title/README.md).

A bounded regression writes logs/captures under ignored `local/runs/`:

```powershell
py -3 tools/game.py run --headless --seconds 190 --capture --replay tests/attract-interrupt.txt
```

The report distinguishes a harness deadline from a title exit or guard. Captures
and stage transitions must be inspected before calling a sequence validated.

Issues use the single central Beads database. Game epic: `beads-xbox.1.7`;
shared service correctness: `beads-xbox.1.102`. No repository-local issue database.

## License and attribution

Owner-original repository material follows the existing Xbox ecosystem's
PolyForm Noncommercial policy; see LICENSE and NOTICE. This does not relicense
dependencies. The current reference executable has unresolved combined-license
restrictions documented in [LICENSE-SCOPE.md](LICENSE-SCOPE.md).
[THIRD_PARTY_ATTRIBUTION.md](THIRD_PARTY_ATTRIBUTION.md) identifies the shared
attribution inventory. Game assets, original code and executable releases are
not included in this repository.
