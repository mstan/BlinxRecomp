# Original NTSC Blinx

## Character transparency and tester diagnostics - 2026-10-22

The shared framework now preserves whisker/fur alpha through later lighting
stages that have no texture. An isolated native shop capture shows clean face,
ear fur and thin whiskers, and an enhanced adaptive gameplay smoke reaches
the stage with the same fix. Authored transparent/opaque BC2 pixel checks and
Dashboard material regressions pass. The shared fix adds no draw passes or
interpreter fallback. General empty-sampler equivalence remains explicitly
tracked in the framework deferral ledger.

This pin also retains per-launch logs and adds optional bounded controller-input
reproduction with separate initial save copies. It is input-boundary replay,
not a deterministic full-machine checkpoint. The earlier projectile continuation
recovery remains included; the exact owner projectile-hit retest is pending.


## Targeted Retry effect follow-up - 2026-10-19

Automatic Retry's tape streaks and pale tint now span adaptive 21:9/32:9
gameplay. The mod identifies the noise through its live title catalogue;
paired draw captures identify the tint's separate full-screen immediate quad.
Its first submission precedes time state 4, so the exemption follows its
geometry/depth contract across the lifecycle. Three focused game tests pass;
muted position-assisted falls run original death, Retry and recovery at both
aspects without writing success or time-control flags. Final owner visual
acceptance remains open alongside the enemy/door/health-circle cases. The
intermittent long loading/audio stall was not reproduced or fixed by this change. See
[widescreen evidence](WIDESCREEN.md) and [performance limits](PERFORMANCE.md).

## Entrance performance follow-up - 2026-10-18

The current visual mods support 1080p through 2160p, AA, supersampling and
filtering; older 640x480-only milestone entries below are historical.
Stage 1-1 entrance slowdown prompted a measured texture-owner lookup
optimization in framework `dae6ce8`. A short original-input comparison at
1080p/adaptive 21:9 uses about 7.5% less CPU time per game update. Sustained
FPS and the remaining entrance slowdown are still open; effects and simulation
timing are preserved. See [performance evidence](PERFORMANCE.md).

## Native CPU assessment - 2026-10-18

Fast `play` now pins framework `d48bec6` and selects a reviewed host-idle
service in the original main-frame poll. Guest register state, counter
publication, comparison and scheduler priority/readiness are preserved;
reference builds retain original polling. The measured movement poll spends
about 99.7% fewer CPU cycles, but mixed total CPU timings do not establish a
whole-title FPS gain. Register-address caching and forced huge-function
optimization were measured and rejected. There is still no x86 interpreter
fallback in this fast profile. See [the assessment](PERFORMANCE.md).

The native candidate reaches original Stage 8 Results/scoring/save and a fresh
authenticated reload. Focused contracts and disabled-reference machine-code
equivalence pass. The game-owned build passes 33 focused checks, loads its
authentic QA save and runs 280 original Stage 1 updates without guest diagnostic
writes or guards; exhaustive campaign/LLE validation was not repeated.
Resolution presets through 2160p, antialiasing/supersampling and higher FPS are
separate mod follow-up; the current internal render size remains 640x480.

## Optimized gameplay - 2026-10-18

`--mode play` selects the optimized native HLE gameplay configuration. The same
stationary Stage 1 sample now uses 38.5-39.1 CPU ms/update versus 54.3 previously;
observed main-loop throughput rises from 18.4 to 26.1-27.1 updates/s. These are short
samples under changing host load, not display FPS or whole-campaign guarantees.
The separately retained LLE modes remain available. See
[performance evidence](PERFORMANCE.md) for scope and identities.

The compact native dispatch index is initialized, and recovery now derives its
count from the actual 8,872-row inventory rather than retaining the stale 8,863
bound. Focused CPU/runtime/dispatch checks pass; the optimized title reaches
original Stage 1 Results/save and authenticates that save in a fresh process.
The prior assisted campaign route was tested before this compiler change and
has not been exhaustively rerun on the optimized binary. Deeper CPU flags/FP
semantics, full native original-kernel execution and complete save states remain
independent work; this performance batch does not close those scopes.

## Current gameplay and enhancement milestone ? 2026-10-17

| Area | Current evidence / remaining work |
| --- | --- |
| Bounded attract families | 9/9 under the framework's documented qualification scope; historical partial counts below are superseded. This is not full campaign qualification. |
| CPU and HLE | 8872 native dispatch entries in the current fast profile, with original ahead-of-time CPU bodies and native services. No CPU interpreter fallback in this profile. Missing coverage stops explicitly; dispatch count is not a campaign-completion percentage. |
| New Game and Stage 1 | Original callback/extent repairs and native file metadata updates allow saves, tutorials, movement, combat, doors, retry and original Results/save. Stage 1 exit coverage explicitly uses HP/position cheats. |
| Round 1 completion | Stages 2/3 and the first boss reach original Results/save and unlock Round 2. Source-reviewed HP/position/hit-state cheats accelerate this route; contact damage and every puzzle are not established by those assists. Stage 2 progress also authenticated on a fresh process. |
| Round 2 | All three normal stages and its boss reached original Results/save, unlocking Round 3. `43D90` recovery through `441B0` preserves `44000`; boss assistance uses source-reviewed death-AI state. |
| Round 3 | All three normal stages and its boss reached original Results/save, unlocking Round 4. Fixed-function TEMP routing exposed a real HLE renderer gap, now corrected and GPU-tested on both prepared backends. Boss death-AI/position assistance retains the original portal and completion route. |
| Round 4 | All three normal stages and the boss reached original Results/save. Stage 3 needed an additional Tom-Tom HP assist and time for original death effects before the goal teleport. Boss HP/grounded-wound assistance retained the original death/portal; a negative Y offset placed Blinx above the arena for the exit. |
| Round 5 | All three normal stages and the boss reached original Results/save, unlocking Round 6. Tom-Tom HP assists and settled death effects are included in the normal-stage batch. Main boss wound assistance leaves its separate appendages untouched; original code owns death and portal. |
| Round 6 | All three normal stages and the boss reached original Results/save, unlocking Round 7. Includes reached kind 12 death path and original 210-tick boss death sequence after explicit source-reviewed death-AI setup. Review harness host suspension avoids ignored Start input during intros; no checkpoint restore is implied. |
| Round 7 | Everwinter stages and boss reached original Results/save and Round 8 unlock; new enemy kinds 9/10 and the boss's original lethal-HP death route execute. Original stage indices 28-31 are explicitly distinguished from UI round order. |
| Round 8 | All three Forge of Hours stages and its boss reached original Results/save and unlocked Round 9 Momentopolis after G8B8, original callback and BUMPENVMAP repairs. Shared renderer corrections have focused real GPU coverage; exact 19-byte `B95B0/B2930` recoveries avoid common-header/original-body churn. Statically recovered `B9FD0` is not a live coverage claim. Boss assistance includes a death-position correction; natural airborne lethal-hit behavior remains deferred as `.1.7.8`. |
| Round 9 and ending | Four original rematches, pre-final shop, final-boss death, rescue scene, ending movie, signed save and title return. Completed save authenticates on fresh launch. Native triangle fans fix the reached draw guard, with PHOBOS GPU 4/4, guest ABI 1/1 and SPOON GPU 2/2. Final HP/wounded-AI assist bypasses its variant-zero/one five-HP floor; natural phases/forms and ability-dependent combat remain unqualified. |
| Diagnostic checkpoints | Framework `76a7ce4` adds restricted generated native continuation records, with fresh-process nested-call restore passing 9 focused tests. Default fast profile unchanged. This is not a Blinx save state and does not yet shorten campaign load time; thread/device/storage serialization remains open. |
| Playable time abilities | All five controls activate through original input, consume supplied inventory and complete their lifecycle. REC includes original recording, rewind, clone playback and stable cleanup. Precise history parity, moving clone paths and ability-specific puzzles/combat are not established by these smoke checks. |
| Shop | Retry checkout, cancellation, over-budget rejection and fresh-process persistence pass. Original pre-final shop also buys TS-4000 Star Lv.2 for 2,000G (12,202G to 10,202G), showing its purchased model. The ending saves afterward; fresh gameplay weapon persistence and other item types remain open. |
| Widescreen | Default-off static mod, configurable/adaptive 4:3 through 32:9. Actual same-scene 16:9/21:9/32:9 captures; original Stage 1 Results/save and Stage 2 unlock at 21:9. HUD anchors proportionally; menus/results retain 4:3. Internal resolution remains 640x480. |
| Populated HUD | Actual 21:9/32:9 captures verify crystal icons and padded circular slot frames together at left, time controls centered and three-object trash inventory at right. The reached 0..256 slot-frame quad is corrected within the mod; six-profile native checks retain the adjacent rewind button's center anchor. |
| Campaign QA | Assisted main-route smoke pass complete: all normal stages/bosses, final rematches/ending/save, all five control lifecycles and two shop purchase classes. Exact evidence in beads-xbox.1.7.7; broader natural combat, puzzles and optional inventory deferred as .1.7.9. This does not imply exhaustive natural-gameplay qualification. |
| Enhancement deferrals | Higher internal resolution/FPS, later-scene fringe culling/effect producers and shared recomp-ui provider integration remain separate; see WIDESCREEN.md. |

Source and retained private evidence: framework branch
`fix/blinx-new-game-20261017`; runs `stage-003`, `stage-004` and
`wide-stage-006`, `campaign-007`, `campaign-010` and subsequent runs under its
ignored `build/blinx/gameplay-coverage-20261017`.
The widescreen suite checks disabled behavior, six aspect/anchor/menu profiles,
six resolver guards and four native invalid-plan guards. GPU backend, FVF snap
and float-blend checks pass. All thirteen recovered original-function extent
records are retained in the game-owned revision metadata; explicit tail-jump
support and existing extent guards pass 13 focused checks.
Framework `c993798` additionally passes CURRENT/TEMP GPU checks on PHOBOS and
SPOON across three device lifetimes, plus the existing PHOBOS backend/FVF/blend
checks. Doctor verifies the adopted framework pin, generated hashes and bindings.
Twenty-four normal stages and eight pre-final bosses have assisted original
Results/save proof. Four rematches and the final boss additionally reach the
original ending/save/title route; fresh-process authentication of completed save
`ef9bfdad233b6a4eae751978bcbf4738b5f7a54c31eed2a582c6556923c37b2c`
passes. All five abilities have activation/lifecycle smoke coverage, including
REC's full clone-playback cleanup. The populated crystal-slot frame mismatch is
fixed and verified at 21:9/32:9. Other shop item types and natural combat
mechanics remain separate scope; see the campaign ledger for exact assists.

## Historical repository-split snapshot ? 2026-10-08

The following earlier counts and New Game deferrals describe that older pin.

The pre-split working attract milestone is preserved in xboxlle
`milestone/blinx-attract-20261008`. The current pin qualifies bounded input and
display: original XID reports/USB transfers and original display IRQ/DPC/waits,
real modeled scanout, interval Reset and teardown. Runtime `4a5dd48` also
qualifies bounded memory allocation/query, residency, page relocation and
resource lifetime. Runtime `9f151c6` additionally corrects native asynchronous
file rejection and notification from original Nt/FATX/IDE execution evidence.
Runtime `526c68d` now connects original file/cache/namespace/IDE execution to
the title over a separate installed HDD, with real persistence. Runtime
`52bdfed` completes the bounded filesystem family: independent original
metadata/directory/error/namespace cases, matching native corrections and
actual native title regression. Five of nine family gates remain open.
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
| Native attract | Movies, Demo play, Start interruption, title return and reentry observed; latest game-owned regression ran 190.109 seconds with 19 captures and no terminal guard |
| Cadence | Accepted 28.5â€“28.8 fps debt is tracked in `beads-xbox.1.100`; no new speed claim from the repository split |
| Calendar LLE | Original kernel conversion functions execute in title calls; qualified alongside native counterpart |
| Input LLE | Original initialization, enumeration and XID reports execute through pinned DMA and IRQ/DPC delivery. Quiet framework run: 190.156s, 19 captures, Demo play, Start interruption, title return and reentry. Original/native hotplug/reopen/Start and live capabilities/zero-rumble qualification also pass; broader peripheral and exact hardware polling timing remain unqualified. |
| Display LLE | Original driver and maintained NV2A/PIC pass waits, paced/immediate flips, independent scanout pixels and three complete device lifetimes. Combined original input/display reaches Demo, interruption, reentry and the subsequent movie in a 510s framework run. |
| Memory LLE | Bounded gate `.106` qualified: original virtual/pool/contiguous services, queries/statistics, loaded-page backing and physical relocation. 50 original cases, 46/46 LLE tests and 300.390s actual attract regression. Pool-backed texture headers remove roughly 2 MiB of guest overhead in the 520-texture fixture. |
| LLE hardening | Five service-family gates remain open in `beads-xbox.1.102`; bounded input `.104`, display `.105`, memory `.106` and files/cache/installed media `.108` are complete. Next is GPU completion/callbacks/visibility. LLE throughput is not an acceptance requirement. |
| File LLE | Original Nt/FATX/cache/IDE title route: final 240.157s framework run, 23 captures, no guards; Demo, Start/title return and reentry. Exact whole-XBE bytes and separate-process persistence pass. 84 original contract cases plus 64 real lifetimes qualify metadata/directory/error/namespace effects; native corrections pass the final game replay. `files-lle` selects the maintained floor; broader filesystem modes/endurance remain explicit. |
| Retained validation debt | Latest game-owned suite 40/40 passes (17.84s). Framework native suite 37/38 passes (17.61s); the sole failure is the previously tracked event-timeout assertion `.99`. Long-run host-memory growth remains `.107`; bounded guest-lifetime checks do not establish an endurance plateau. |
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

## Bounded filesystem family qualification - 2026-10-08

Runtime `52bdfed` passes game doctor/build and **40/40 tests (17.84s)**.
`local/runs/20261008-192931-attract/` runs **190.109s**, with 19 captures and no
guards. Inspected captures 8/12/17/19 show Demo play, title return after Start,
Demo reentry and continued gameplay. Exit 1 is intentional deadline termination.
Executable SHA-256:
`7649af349ff879e8d2e899654b353f421d94d85c78da68262a4a15cdc63b08b8`.
The final pin includes the subsequent evidence-only documentation update;
runtime source and the validated build are unchanged by that update.

Original execution exposed desktop assumptions in native metadata, directory
and symbolic-link effects. Shared corrections preserve rejected IOSBs, report
FATX lengths, reproduce supported query layouts/rejections and truncated link
copies, and retain real directory cursor/ownership work. Together with the
original title route and persistence results, this closes bounded `.108`.
Malformed sub-header directory overflow, internal cache costs and metadata-set
completion counts have explicit tolerances; no general disk-fault/cancellation
or full-console scheduling qualification is claimed. See FILES-LLE.md.

## Original file notification correction - 2026-10-08 (historical)

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


## October 22: Stage 1-2 projectile hit continuation

Owner shooting at an enemy reached the native missing-target stop at
`0x000403C0`. The original conditional branch at `0x000403B0` enters an effect
creation continuation that the discovery inventory omitted. Recovery retains
the original 245 bytes through the restoring return at `0x000404B4`, including
the existing overlapping callable entries, with no HLE replacement or
interpreter fallback. The revision extent inventory records the input hash and
all source boundaries so full preparation regenerates the fix.

Validation: original extent hashes and complete decoding pass; the former
missing-target guard is retired and all direct callees in the recovered body
already have native implementations. Optimized build and owner projectile-hit
retest are recorded under `beads-xbox.1.7.15`. The separate shared tester replay
work is `beads-xbox.1.191` and does not gate this playtest handoff.
