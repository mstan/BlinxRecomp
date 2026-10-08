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
menu return and attract reentry. Accepted cadence debt is about 28.5â€“28.8 fps;
New Game is deferred. The original-input route now executes initialization,
enumeration and controller reports through original page/allocation services,
OHCI/PIC and IRQ/DPC delivery. A quiet 190-second run demonstrates Demo play,
Start interruption, title return and Demo play reentry. Input is now qualified for the bounded port-0 automatic-polling contract,
including original/native hotplug, capabilities and feedback. Final attract
endurance and the remaining eight LLE families remain open.
See [current validation](docs/STATUS.md) and the framework's
[serial burndown](xboxlle/docs/BLINX-BURNDOWN.md).

Dashboard support is required architecture, with an original dashboard route
and an accelerated/direct-title choice. Supplied dashboard files should be
optional. Both routes must use actual persistent console storage, provisioned
as a real virtual HDD when none is supplied. Saves remain real reads, writes
and flushes; HLE can accelerate those services. The current pilot uses persistent
host directories and does not yet complete that shared disk-image requirement
or expose the combined dashboard selector. These remain tracked requirements in
the [console launch/storage contract](xboxlle/docs/CONSOLE-LAUNCH-STORAGE.md).
That contract also records an optional permissive open-source default dashboard,
the original retail boot/logo route with supplied compatible software, and a
configurable console/save profile intended to support one shared drive across
recomp projects. These are planned capabilities; direct game launch must remain
available without dashboard interaction.

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
XAPI input plus the shared OHCI/DMA floor, including original interrupt/report
delivery and the demonstrated interruption/reentry sequence.
Selections occur at build time. No runtime HLE/LLE switching is performed.

`--mode display-lle` keeps native input/rendering/audio and selects the original
display driver over maintained NV2A/PIC. Its actual scanout, blocking waits,
paced/immediate flips and interval Reset have reached Demo play, Start/title
return and reentry. It is a slow correctness route. Build the framework's
private `xlle_nv2a_reference` target in an existing full-system LLE build and
set `display_library` and `display_dependencies` in local paths, then use the
normal build/run commands with that mode. With capture enabled, `scanout-*.ppm`
comes through the modeled hardware; `present-*.ppm` records the native frame.
`--mode input-display-lle` selects both original input and display. Its framework
qualification reaches Demo play, Start/title return, reentry and the subsequent
movie. Use `--seconds 510 --replay tests/attract-input-display-lle.txt` for the
observed slower reference; timing can require adjustment on another host. See the
[display contract and limits](xboxlle/pilots/native-title/DISPLAY-LLE.md).

`--mode memory-lle` selects original general virtual-memory services and the
existing original pool/contiguous/page bridge, with native input/display. The
framework run reaches Demo play, Start/title return and reentry. This is the VM
slice of the ongoing memory-family audit; statistics, residency and the remaining
resource contracts are still being hardened. See the
[memory contract and limits](xboxlle/pilots/native-title/MEMORY-LLE.md).

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
