# Original NTSC Blinx

The pre-split working attract milestone is preserved in xboxlle
`milestone/blinx-attract-20261008`. The current pin `3d04e82` adds original
interrupt dispatch and the shared OHCI/PIC clock checkpoint. Earlier qualification
records below remain historical evidence; the input family is still open.

| Area | Current evidence / remaining work |
| --- | --- |
| Native attract | Movies, Demo play, Start interruption, title return and reentry observed; latest game-owned regression ran 190.657 seconds with 19 captures and no terminal guard |
| Cadence | Accepted 28.5–28.8 fps debt is tracked in `beads-xbox.1.100`; no new speed claim from the repository split |
| Calendar LLE | Original kernel conversion functions execute in title calls; qualified alongside native counterpart |
| Input LLE | Original page/USB/PIC services now invoke Blinx's original ISR and DPC. Actual startup stops at untranslated `CLI` in the original DPC; interrupt deferral/EOI/eligibility and idle delivery pass fixtures. Controller reports and LLE interruption remain open. |
| LLE hardening | Nine broad service-family gates remain open in `beads-xbox.1.102`; input `.104` is active; batch related dependency checks to reduce repeated setup/title runs |
| New Game | Known post-save fault, `beads-xbox.1.101`, deferred until the current correctness phase |
| JSRF | Serial follow-up after Blinx; no game-specific work begun by this split |
| Dashboard choice and storage | Required architecture, `.22`: original dashboard remains supported and its assets are optional for native launch; provision/import a genuine virtual HDD and retain real persistent saves. Host-directory persistence is working but shared FATX image access and the combined launch selector are unfinished. |
| Planned launch/save options | Direct game launch remains available; consider an optional permissive open-source dashboard, preserve the supplied retail boot/logo experience, and make the console/save target configurable for eventual sharing across recomps. No dashboard donor selected or shared-profile implementation claimed. |

Detailed source/evidence is maintained with the implementation in
[INPUT-LLE.md](../xboxlle/pilots/native-title/INPUT-LLE.md),
[CALENDAR-LLE.md](../xboxlle/pilots/native-title/CALENDAR-LLE.md) and
[BLINX-BURNDOWN.md](../xboxlle/docs/BLINX-BURNDOWN.md).

The game repository adopts the existing generation manifest only after checking
every generated file, the original image, CPU donor, all seven binding records
and both original-function recovery records. The generated output remains
ignored; these files are never pushed. The original framework worktree and its
qualification captures are retained for comparison.

Fresh full regeneration is available through the shared preparation command but
is not claimed as validated by the initial repository split. Validation uses
the hash-verified existing output, compiled through this repo's own CMake entry.

## Game-repository qualification - 2026-10-08

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
