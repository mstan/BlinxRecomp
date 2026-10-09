# Original NTSC Blinx

The pre-split working attract milestone is preserved in xboxlle
`milestone/blinx-attract-20261008`. The current pin qualifies bounded input and
display: original XID reports/USB transfers and original display IRQ/DPC/waits,
real modeled scanout, interval Reset and teardown. Runtime `4a5dd48` also
qualifies bounded memory allocation/query, residency, page relocation and
resource lifetime. Runtime `9f151c6` additionally corrects native asynchronous
file rejection and notification from original Nt/FATX/IDE execution evidence.
Runtime `526c68d` now connects original file/cache/namespace/IDE execution to
the title over a separate installed HDD, with real persistence. The family's
remaining contract qualification stays open.
HLE remains the performance path.
Earlier qualification records below remain historical evidence.

The game-owned `files-lle` mode builds on this pin. Its shared transport test
reads all 44,724,224 installed XBE bytes through the original kernel and disk
model, matching the independent extracted file. The full title regression for
that source/configuration is the framework run described below. Game-owned
files-LLE validation currently covers compilation and the shared-transport byte
comparison.

| Area | Current evidence / remaining work |
| --- | --- |
| Native attract | Movies, Demo play, Start interruption, title return and reentry observed; latest game-owned regression ran 190.078 seconds with 19 captures and no terminal guard |
| Cadence | Accepted 28.5â€“28.8 fps debt is tracked in `beads-xbox.1.100`; no new speed claim from the repository split |
| Calendar LLE | Original kernel conversion functions execute in title calls; qualified alongside native counterpart |
| Input LLE | Original initialization, enumeration and XID reports execute through pinned DMA and IRQ/DPC delivery. Quiet framework run: 190.156s, 19 captures, Demo play, Start interruption, title return and reentry. Original/native hotplug/reopen/Start and live capabilities/zero-rumble qualification also pass; broader peripheral and exact hardware polling timing remain unqualified. |
| Display LLE | Original driver and maintained NV2A/PIC pass waits, paced/immediate flips, independent scanout pixels and three complete device lifetimes. Combined original input/display reaches Demo, interruption, reentry and the subsequent movie in a 510s framework run. |
| Memory LLE | Bounded gate `.106` qualified: original virtual/pool/contiguous services, queries/statistics, loaded-page backing and physical relocation. 50 original cases, 46/46 LLE tests and 300.390s actual attract regression. Pool-backed texture headers remove roughly 2 MiB of guest overhead in the 520-texture fixture. |
| LLE hardening | Six service-family gates remain open in `beads-xbox.1.102`; bounded input `.104`, display `.105` and memory `.106` are complete. Files/cache/installed media `.108` is active. LLE throughput is not an acceptance requirement. |
| File LLE | Original Nt/FATX/cache/IDE title route: final 240.157s framework run, 23 captures, no guards; Demo, Start/title return, Demo reentry and continued gameplay. Complete installed XBE bytes independently match, and separate-process 9,001-byte write/flush/read/delete passes. `files-lle` mode exposes this selection; broader contract/endurance work remains open. |
| Retained validation debt | Latest native suite 40/40 passes (17.03s). Shared-device suite 46/46 passes (22.44s); the previously intermittent event-timeout assertion `.99` is not considered fixed by one passing run. Long-run host-memory growth remains `.107`; bounded guest-lifetime checks do not establish an endurance plateau. |
| New Game | Known post-save fault, `beads-xbox.1.101`, deferred until the current correctness phase |
| JSRF | Serial follow-up after Blinx; no game-specific work begun by this split |
| Dashboard choice and storage | Required architecture, `.22`: original dashboard remains supported and its assets are optional for native launch. The private files-LLE route now uses a genuine persistent FATX HDD; the default native backend still uses host directories. General shared-profile provisioning and the combined launch selector remain unfinished. |
| Planned launch/save options | Direct game launch remains available; consider an optional permissive open-source dashboard, preserve the supplied retail boot/logo experience, and make the console/save target configurable for eventual sharing across recomps. No dashboard donor selected or shared-profile implementation claimed. |

Detailed source/evidence is maintained with the implementation in
[INPUT-LLE.md](../xboxlle/pilots/native-title/INPUT-LLE.md),
[DISPLAY-LLE.md](../xboxlle/pilots/native-title/DISPLAY-LLE.md),
[MEMORY-LLE.md](../xboxlle/pilots/native-title/MEMORY-LLE.md),
[FILES-LLE.md](../xboxlle/pilots/native-title/FILES-LLE.md),
[CALENDAR-LLE.md](../xboxlle/pilots/native-title/CALENDAR-LLE.md) and
[BLINX-BURNDOWN.md](../xboxlle/docs/BLINX-BURNDOWN.md).

The game repository adopts the existing generation manifest only after checking
every generated file, the original image, CPU donor, all eight binding records
and all five original-function recovery records. The generated output remains
ignored; these files are never pushed. The original framework worktree and its
qualification captures are retained for comparison.

Fresh full regeneration is available through the shared preparation command but
is not claimed as validated by the initial repository split. Validation uses
the hash-verified existing output, compiled through this repo's own CMake entry.

## Original file notification correction - 2026-10-08

Updated milestone on framework `526c68d`: game doctor/build and 40/40 native
tests pass (17.03s). `local/runs/20261008-190843-attract/` runs 190.078s with
19 captures and no guards; inspected captures 8/12/17 show Demo, title after
Start and Demo reentry. Exit 1 is the intentional deadline termination.
Executable SHA-256:
`caedeacfd916418873dd51be5ac0c8f41320897e9492d69809cd7b84bb58dc0d`.
This adopts both the original-files route and the native open/EOF IOSB fixes.
The new mode builds successfully, with its own exact whole-XBE transport test.
Family `.108` stays open for the remaining metadata/error/directory/mount
contract pass and completion limits documented by the framework.

Runtime `9f151c6` with its reference-test/documentation follow-up: game doctor
and build pass; **40/40 native tests pass (16.52s)**. An immediate asynchronous
file error now preserves IOSB and leaves notification reset; accepted requests
retain real completion. Explicit-event requests notify that event, while
requests without an event notify the file. The original kernel/FATX/IDE route
establishes these observations independently of the native implementation.

`local/runs/20261008-182422-attract/` records **190.094s**, 19 captures and no
guards. Inspected captures 8/12/17 show Demo play, title return after Start and
Demo reentry. The harness intentionally stops at the deadline (exit 1).
Executable SHA-256:
`e04263c4697cf244cad361b4f86ea28cd8b50f882e4f32a559d3c87f189d9f77`.
This checks the native correction and preserves attract behavior. It does not
close the files family or add a cadence/endurance claim.

## Full memory-family integration - 2026-10-08

Runtime `4a5dd48` with its documentation-only follow-up: game doctor and build
pass. Native tests pass **39/40 (16.19s)**; the unchanged event wait minimum-duration
assertion fails, retained in `.99` for the scheduling/time family. Memory and
graphics ownership tests pass, including two complete texture release/reuse
cycles and 2,150,400 committed bytes for 520 live native textures.

`local/runs/20261008-174209-attract/` records **190.125s**, 19 captures and no
guards. Inspected captures 8/12/17 show Demo play, title return after Start and
Demo reentry. The harness intentionally stops its own child at the deadline
(exit 1). Executable SHA-256:
`aadec0a35ea9d9e488bd2addad5a4cba579a0498f006e6133da3999ac359d0d9`.
This is a bounded regression, not a new speed or endurance claim. The separate
framework original-memory run records 300.390s through Demo/interruption/reentry
and subsequent movies; its identities and explicit limits are in MEMORY-LLE.md.

## Game-repository qualification - 2026-10-08

Latest report checkpoint: framework `7e47c6c`, hash-verified refreshed generated
output and the four game-owned recovery extents. All **40/40 native tests pass
(17.75s)**. `local/runs/20261008-141612-attract/` records **190.094 seconds**,
19 captures and no guards. Inspected captures 8/12/17 show Demo play, the title
movie after Start and Demo play reentry. The harness stopped its own child at
the requested deadline, not a crash. Executable SHA-256:
`71f6c027d5be5dc5c9864204c953ca328a9234895dbe40d002dcb454cda1d35a`.
This preserves native performance behavior after the original report contract
corrections; it does not add an endurance or cadence measurement.

Initial repository-split evidence follows:

- `py -3 tools/game.py build` produced `build/attract/framework/Release/BlinxRecomp.exe`.
- `py -3 tools/game.py test`: all 40 tests pass (17.90 seconds).
- The game-owned launcher ran 190.157 seconds, producing 19 captures with no
  terminal guard. Captures 9/12/16 show Demo play, the title screen following
  Start interruption, and Demo play reentry. The harness killed its own child
  at the deadline; recorded exit 1 is not a crash.
- Executable SHA-256:
  `b3106fd20f8061183796a2071866b11996841d56afcb1188ae7f58d15b502e43`.
- Private evidence: `local/runs/20261008-112424-attract/`, including `run.json`
  and `title.log`; test/build logs are in `local/`.
- Index review includes 19 source/configuration blobs and the declared framework
  gitlink. The generic framework hygiene scanner flags submodules for review;
  this exact URL/pin was reviewed, and the framework history scan reports 2,636
  unique blobs with zero findings. Owned media, generated code, local state and
  dependency binaries remain ignored and outside the push.

This validates repository/build/launcher integration and preservation of the
working attract path. It does not close outstanding fidelity or cadence issues.

## Original page/USB checkpoint - 2026-10-08

Framework `c81a95c` passed 43/43 tests in the original-input/USB LLE configuration
and 40/40 in the native-input/USB-OFF configuration. Actual original-input startup
uses the shared OHCI model and stops explicitly at ordinal 129 (interrupt
priority), following original allocation/page services. Descriptor DMA, completed
TD/done-head writes and storage lifetime are fixture-qualified; controller report
and CPU interrupt delivery remain open. Details and bounded scope are recorded in
[INPUT-LLE.md](../xboxlle/pilots/native-title/INPUT-LLE.md).

This repository's default `attract` build was rebuilt and ran **190.125 seconds**
through its own launcher, with 19 captures and no terminal guard. Inspected
captures 8/12/16 show Demo play, the title screen after Start, and Demo play reentry.
The harness stopped its own process at the deadline (exit 1), not a crash.

- Private evidence: `local/runs/20261008-122549-attract/run.json` and `title.log`.
- Executable SHA-256:
  `84c4b810495911645e4583ca5547a43584751eea25bd8fdd885099f707cbb674`.
- `input-lle` selects `PILOT_USB=LLE`; default `attract` keeps USB `OFF`, native
  input and its existing allocator. This is build selection, not live switching.

This preserves the playable HLE milestone and advances its maintained input
floor. It does not close the input family or add a speed/endurance qualification.

## Original interrupt/clock checkpoint - 2026-10-08

Framework `3d04e82675fd7123d499f092ad652d8b4da26bee` reuses the maintained
OHCI/PIC models and original kernel interrupt gates. The original-input probe
now executes Blinx's original ISR at IRQL25 and enters its original USB DPC,
then stops explicitly at untranslated `CLI` (`0x00188a9a`). It previously stopped
at ordinal129 before interrupt setup. No instruction was discarded and no
interrupt success was fabricated.

The original-input framework build passes **45/45 tests (17.46s)**, including
real IRQ assertion, high-IRQL deferral/lowering, specific EOI, original ISR ABI,
nested original service calls, DPC exclusion/duplicate rejection and idle-CPU
delivery. The separate original-code oracle qualifies the kernel's behavior.
The batch fixed DPC dequeue before CPU ownership and incorrect guest-memory
charging for the host-only bridge. Attribution and remaining scope are in
[INPUT-LLE.md](../xboxlle/pilots/native-title/INPUT-LLE.md).

This game's default native-input build passes **40/40 tests (17.06s)** on the
updated framework. No generated title body or revision binding changed.

The game-owned launcher completed **190.657 seconds**, with **19 captures and
no terminal guards**. Inspected captures 8/12/16 show Demo play, the title screen
after the replayed Start, and Demo play reentry. The harness terminated its own
child at the requested deadline (exit1), not a crash. Private evidence is in
`local/runs/20261008-130831-attract/`, including the run manifest, title log and
captures. This checks preserved native-input attract behavior after the shared
DPC ordering fix; it is not an LLE-input attract run.

Next serial dependency: correct interrupt-flag operations and the adjacent
original USB DPC timer/device reads. USB enumeration/reports, LLE Start/menu
interruption/reentry and the overall input family remain open. This checkpoint
does not add a cadence, endurance or full LLE-input sign-off.

## Bounded input gate - 2026-10-08

Framework `39a551b` adds the recovered original disconnect callback and
actual original stack reservation/PFN pinning for USB control transfers.
Original/native hotplug runs complete 70.094s/150.140s with captured menu return;
separate live API probes complete 60.093s/60.078s, both before and after reconnect,
with identical 25-byte capabilities and actual zero-rumble completion.
Framework qualification: 46/46 LLE tests (19.79s), 40/40 production-native tests
(17.71s). Production disables the separate API probe. The earlier game-owned
190s regression remains the visual attract evidence; these framework runs are
not relabeled as new game-owned attract runs. See the pinned INPUT-LLE.md for
identities, limits and independent original-software evidence.

`tests/input-hotplug.txt` provides the same disconnect/reconnect replay for either
build selection. The game profile adds the fifth original recovery extent and
adopts only hash-verified generated output.

Game integration on this pin: doctor passes; build succeeds; **40/40 tests pass (17.06s)**. No additional long attract replay was run for this pin update.

## Bounded display gate - 2026-10-08

Runtime `8740525` and its display qualification record are integrated together.
The game-owned native build passes **40/40 tests (20.33s)** and completes
**190.656s** with 19 captures and no guards. Captures 8/12/17 show Demo play,
title return after Start and Demo play reentry. The harness ends at its deadline
(exit 1); this is not a crash or a new cadence/endurance qualification.

Private evidence: `local/runs/20261008-161729-attract/`; executable SHA-256
`b9ce6dfd59751e9c601177ec8a86f1bbcb1cf09b1436292e7bb62069ea96d0d8`.

`display-lle` selects original display with native input; `input-display-lle`
selects both original services. The latter's framework qualification uses the
slower game-owned `tests/attract-input-display-lle.txt`, completes 510.328s and
reaches interruption/reentry and the subsequent movie. Geometry changes and
hardware field phase remain outside the bounded display claim. Host private
memory grows during the slow combined run; `.106` retains that observation for
the memory/resource audit. No new input or display workaround is used.

## Original virtual-memory checkpoint - 2026-10-08

Framework `c65629b` adds the build-selected original VM route and independent
38-case original-kernel observations. The whole memory family remains open;
`memory-lle` is an opt-in slice with native input/display. The generated title
and binding hashes are unchanged and doctor passes.

This game's native build initially passes **39/40 tests (16.00s)**. The graphics
clock test fails its strict whole-process handle-count equality once; the same
unchanged graphics ABI test then passes **three consecutive reruns** in
2.66/2.71/2.61s. The transient cause is not established and is retained in `.106`;
the assertion was not removed or relaxed.

The actual game-owned native regression completes **190.125s**, 19 captures,
no guards, deliberate deadline exit 1. Captures 8/12/17 show Demo play,
Start/title return and reentry. Private evidence:
`local/runs/20261008-165451-attract/`; executable SHA-256
`eed97b28dd47960081d495828c2b6a3596e9e7bbcb345f2490501e8ac4d26db1`.
