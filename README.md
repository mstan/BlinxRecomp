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

The bounded attract qualification is 9/9 under the documented scope; full
campaign coverage is separate. New Game, tutorials, movement, combat and the
original Stage 1 exit/Results/save transition now run. All three Round 1 stages
and its first boss subsequently reached original Results/save. Round 2's three
stages/boss and all of Round 3 also reached original Results/save with explicit
HP/position/AI-state cheats. All three Round 4 normal stages and its boss also
saved, followed by all of Rounds 5 and 6; later content remains open. These assists do not prove
every route or natural damage producer. Progress also authenticated on a fresh
load. The fast HLE profile uses ahead-of-time compiled
original CPU code and native services; missing native paths stop rather than
silently entering an interpreter.

The default-off [widescreen mod](docs/WIDESCREEN.md) supports configurable and
adaptive aspects through 32:9, with proportional HUD anchoring. The same Stage 1
scene has actual 16:9, 21:9 and 32:9 captures, followed by original Results/save
and Stage 2 unlock at 21:9. Gameplay widens; menus retain centered 4:3. Internal
rendering remains 640x480. Later stages, bosses, time abilities and shop purchases
are being exercised with authorized QA cheats in a separate muted instance.
See [current validation](docs/STATUS.md) and the framework's
[burndown](xboxlle/docs/BLINX-BURNDOWN.md).

Dashboard support is required architecture, with an original dashboard route
and an accelerated/direct-title choice. Supplied dashboard files should be
optional. Both routes must use actual persistent console storage, provisioned
as a real virtual HDD when none is supplied. Saves remain real reads, writes
and flushes; HLE can accelerate those services. The default native pilot uses
persistent host directories; the private files-LLE route now uses a real FATX
HDD image. General shared-profile provisioning and the combined dashboard
selector remain tracked requirements in
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
framework run reaches Demo play, Start/title return, reentry and subsequent
movies. The bounded memory gate includes original queries/statistics, loaded-page
residency, physical-page relocation and shared CPU/DMA/resource ownership.
Long-run host-memory retention remains tracked separately. See the
[memory contract and limits](xboxlle/pilots/native-title/MEMORY-LLE.md).

`--mode files-lle` selects original Nt/FATX/cache/IDE services and original
utility-drive code over a separate writable installed-title HDD, plus original
memory services. Renderer, audio and input remain native. Build the private
framework `xlle_storage_reference` target and configure `files_library`,
`files_dependencies` and `files_hdd` in local paths. Provision the complete
matching title under `E:\Titles\4D530013`; the framework's
`tools/native_hdd_install.py` records full hash/readback evidence. Its private
offline FATX dependency and preparation caveat are documented in
[FILES-LLE.md](xboxlle/pilots/native-title/FILES-LLE.md).
This mode uses the HDD for actual files, cache and saves; native launch keeps
its existing host-directory backend. Use `--seconds 240 --capture --replay
tests/attract-files-lle.txt` for the focused interruption/reentry check. The
bounded filesystem family is qualified against the native replacement;
unexercised filesystem modes and long-run endurance remain explicitly scoped.

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
