# Blinx performance observations

## Native service preparation and compiler optimization - 2026-10-27

The candidate framework pin reuses prepared native draw work, immutable shader
stream plans and validated vertex-owner index hints. Qualified MSVC x64 Release
play builds also request whole-program optimization; other hosts and development
profiles retain normal compilation. Graphics features and original game timing
remain unchanged.

Matched Round 2 Stage 1 samples on the CPU-bound NAS cost 62.126 ms per original
update for the delivered build and 54.975 ms for the combined candidate (about
11% less). The compiler increment over the renderer candidate is about 7%.
Original update throughput rises from 15.324 to 16.607/s there; the simulation
target remains unchanged. Initial reference A/A variation is 2.38%; later
background load rises and delivered endpoints drift about 3%. These are retained
workload/host observations, not whole-game or local-host FPS guarantees.

The candidate activates and recovers all five time controls, reaches original
Round 2 Stage 1 Results/save after explicit HP/position assistance and normal
dialogue acknowledgement, then fresh-loads those new native save bytes into
Stage 1 gameplay. The final pinned executable also passes fresh Stage 2-1 entry.
The local IPO GPU gates pass with reuse enabled and disabled. Shared NAS fixture
failures reproduced by the frozen framework baseline are tracked separately in
`beads-xbox.1.203`, without relaxing their assertions.

Tracking and machine-readable raw evidence: central `beads-55wj`, campaign
`beads-2wy8.1`, and the
[historical framework validation](https://github.com/mstan/xboxllr/blob/5faaafad269d671b39737edbebaed66c81b2b9fb/docs/NATIVE-OPTIMIZATION-BURNDOWN-2026-10-26.md).
The owner visual/play check and merge handoff remain pending.

## Stage 2-2 entrance presentation - 2026-10-22

The shared interpolation worker could miss display ticks while busy game
graphics repeatedly held the renderer lock. A missed tick now becomes a
coalesced request that the renderer owner services at its next safe boundary.
Original game timing, effects and visual settings remain unchanged.

In the first two seconds after player allocation, a visible Stage 2-2 run
delivers 59.61 accepted native presentations/s with the fix, versus 33.15 with
the diagnostic control disabled in the identical executable. Game updates
remain 29.55 versus 29.13/s. Later sampled windows retain about 60 presentations/s.
This is smoother interpolation, not faster simulation or physical scanout
certification. The owner-reported entrance still needs a hands-on review.

Six focused GPU/renderer checks pass, including unchanged source pixels,
visibility-query isolation, FP environment preservation and close/recreate.
Host load varies; no total CPU gain or elimination of loading stalls is claimed.
Exact build identity, comparison and limits are in the
[framework presentation audit](../xboxlle/docs/reference/runtime/NATIVE-INTERPOLATION-COOPERATIVE-2026-10-22.md).
Tracked in `beads-xbox.1.7.20` and shared `beads-xbox.1.192`. The published
Windows preview `2026.10.22.1` predates this follow-up.

## Stage-entry texture preparation - 2026-10-22

The Windows hotfix retains the exact native build accepted in the owner's
Stage 2-1 playtest. Shared resource-write tracking lets unchanged texture
preparations be reused while writes, resource reuse and relevant state changes
invalidate them. Visual quality and original simulation timing are preserved.

In the bounded balanced Stage 2-1 comparison, process CPU cost fell from
35.575 to 29.770 ms per game update (about 16.3% less). Throughput rose from
27.995 to 29.914 updates/s, with similar sampled background CPU load. These
are game-update measurements, not interpolated presentation FPS or whole-game
performance guarantees. Separate cold-entry runs varied with host load.

The owner reported the accepted build was "much much smoother" and "vastly
improved," with a possible minor hitch or two. Residual hitches remain tracked
in `beads-xbox.1.7.18`; this release does not claim zero stutter. Exact evidence
and focused contract checks are in the
[framework assessment](https://github.com/mstan/xboxllr/blob/5faaafad269d671b39737edbebaed66c81b2b9fb/docs/BLINX-STAGE21-PERFORMANCE-ASSESSMENT-2026-10-22.md)
and [write-tracking audit](../xboxlle/docs/reference/runtime/NATIVE-RESOURCE-WRITE-WATCH-2026-10-22.md).

## Intermittent Stage 1 loading/audio report - 2026-10-18

The owner observed a 25.169-second `rd11.mit` to `01_StageStart.adx`
file-open span with choppy/echo-like audio. Isolated observations subsequently
measured 0.760 seconds and a 6.58-second selector/initialization route; these
are different measurements and do not establish a matched loading improvement.
The original slow run created no new title vertex programs, and its logged
HLSL compiler work totaled 11 ms. Driver/resource waits and populated rewind
history remain hypotheses; neither is a diagnosed cause.

The targeted Retry correction run `retry-target-004` uses the owner's
1080p/adaptive/FXAA/8x plan and original saved Stage 1 entry, with isolated
saves, muted output, normal priority and logical CPUs 12/15. Its corresponding
file-open span is 2.7065 seconds. It does not reproduce the long stall and is
not a controlled performance comparison. Original automatic Retry and recovery
also complete at 21:9 and 32:9. No loading/audio fix or sustained FPS gain is
claimed by the widescreen-effect correction. Issue `beads-xbox.1.100` remains
open; a repeated slow run needs call/wait and stream-refill correlation before
changing timing, audio buffering or graphics resource semantics.

The 2026-10-19 populated-cache QA005 check byte-copies the owner's 302 cache
files (574,159,884 bytes) into isolated QA state. Its corresponding entry takes
3.9257 seconds and does not reproduce the long stall. The bounded diagnostics
record 112 slow file-service intervals, up to 1006.8 ms, with near-zero calling
thread CPU and outer media-lock wait; no observed GPU call exceeds 50 ms.
These intervals can still include inner locks, host scheduling or actual I/O.
The 120-second audio timeline has zero overflow. This narrows observations
without explaining the earlier 12.4206-second feedback-draw gap or establishing
a loading fix. The opt-in [loading diagnostic](LOADING-DIAGNOSTICS.md) preserves
the selected settings and normal owner audio for a future slow occurrence.

## Stage 1 entrance follow-up - 2026-10-18

The reported entrance-ring slowdown reproduces as a graphics-heavy CPU
workload at adaptive 21:9, 1080p, FXAA and 8x anisotropy. Audio runs separately;
its continued playback does not establish that gameplay is keeping pace.
The measurements do not isolate particles as the sole cause.

Framework `dae6ce8` accelerates texture-owner lookup with a small hint cache.
Every hit checks the live owner; resource data, effects, draw order and game
timing remain unchanged. Four focused graphics checks pass, including real
texture pixels, collisions, table growth and two release/reuse cycles.

Sequential short runs with profiling/readback disabled, the same authentic
copied save and original input give 39.714 versus 36.751 process CPU ms/game
update near spawn (about 7.5% less). Main-loop throughput is 24.20 versus 26.50
updates/s; mean shared host CPU load is 50.1% versus 47.7%. These are bounded
observations, not sustained presentation FPS or whole-game guarantees. The
windows start three seconds after the first initialized spawn and last about
13 seconds, excluding selector/loading time. All automated runs are muted,
use isolated state and are retired. Owner preferences and saves are preserved.

Deferred graphics state and draw submission remain performance follow-ups in
`beads-xbox.1.100` / `beads-xbox.1.7.3`. No interpreter, timing multiplier,
particle suppression or lowered visual preset is introduced. Exact sample
identities and private evidence paths are recorded in the
[framework audit](../xboxlle/docs/reference/runtime/NATIVE-HLE-OPTIMIZATION-2026-10-18.md).

## Native CPU assessment and frame-poll idle - 2026-10-18

The fast `play` profile now adopts framework
`d48bec69f863cc0881175487a8ae3aefe51e643c` and selects the reviewed native
host-idle component for the original frame-counter poll at `AB801`.
The original comparison, guest registers/flags/stack, callback publication and
guest CPU readiness/priority remain authoritative. The host sleeps while
waiting for scheduler publication; it never fabricates a frame or makes a
lower-priority guest eligible early. Reference modes keep original polling.
This is build-time component selection, with no shipping runtime switch.

Measured Stage 1 movement wait CPU cost falls from 4,204,445 to 13,223 cycles
per original update (3.081% to 0.00995% of main-thread cycles). Actual elapsed
waiting remains. This removes spin work; **whole-title CPU/FPS improvement is
not established**. Unprofiled Stage 8 and a test-only same-image comparison
give mixed total timings, including worse candidate windows. Stationary cases
often have almost no unsatisfied wait. Full results and limitations are in
[the CPU assessment](../xboxlle/docs/reference/cpu/NATIVE-CPU-BOOKKEEPING-2026-10-18.md).

Guest register access is still real native work. Address-cache experiments
failed the performance gate and are archived. Fully optimizing the large
player body reduced its own cycles but implied only about 0.2-0.3% potential
main-thread savings at a five-minute compile cost, so that policy is also
rejected. The register-model header is unchanged. No x86 interpreter fallback
is introduced; broader CPU/FP, entry-barrier and movie-poll work stays on the
documented deferral list with concrete reopening triggers.

The source production candidate reaches original Round 3 Stage 1 Results,
scoring and signed save using explicit HP/position assists. A fresh process
authenticates that save without guest diagnostic writes or guards. Five poll
binding/compiled-C checks, 13 extent checks, four checkpoint checks and 32
framework runtime/ABI checks pass. With the option off, all 253 native code
sections and symbolic relocations in the affected unit match the original.

The game-owned build additionally passes 33 focused runtime/ABI/widescreen
checks, authenticates its copied original QA save through the selector, and
runs 280 original Stage 1 gameplay updates without guest diagnostic writes or
guards. Its actual affected-unit compiler command selects the idle component.
This is an integration smoke check, not another matched speed comparison; the
host-load sample was 100%. The muted QA process was retired afterward.
Its first harness deadline started while cold startup was still loading files;
the same live process continued successfully, without a rebuild or relaunch.
Tested game executable SHA256:
`272c09d270075f516c2f3d4befd3cc6ebb1388876c4693fe69d26269706d39e5`.
Only one generated CPU unit and its poll inventory changed. No full campaign
or LLE qualification was restarted. Performance issue `beads-xbox.1.100` stays
open for broader headroom; this assessment does not promise a sustained FPS.

Higher internal resolution, AA/supersampling and higher FPS are opt-in mod work
tracked separately in `beads-xbox.1.7.4`; see [the enhancement scope](WIDESCREEN.md).
The current build still renders internally at 640x480.

## Prior compiler optimization milestone - 2026-10-18

The native target now defaults to `PILOT_NATIVE_OPTIMIZATION=OPTIMIZED`:
MSVC `/O2 /fp:precise`, or GCC/Clang `-O2 -fno-strict-aliasing`. `BRINGUP`
preserves the previous unoptimized configuration. `game.toml` selects OPTIMIZED
explicitly. Build and launch `--mode play` for the fast native scheduling,
calendar, input, rendering and audio configuration; the existing reference
modes remain independently selectable. No runtime HLE/LLE switching was added.
The launcher also disables optional event tracing by default, matching the
measured configuration. Required GPU completion events remain enabled.

Matched stationary Round1 Stage1 samples use the same authenticated save,
100-update warmup and 180-update observation, one muted hidden game at a time,
four physical cores (logical8/10/12/15), normal priority. The adaptive mod uses
21:9 presentation; internal rendering remains 640x480. Position and pause state
remain unchanged. The counters below are game main-loop updates, not measured
desktop present FPS.

| Build | CPU ms/game update | Game updates/s |
| --- | ---: | ---: |
| Preserved `/Od` | 54.25 | 18.38 |
| Compiler-only `/O2` | 38.45 | 26.13 |
| `/O2` + compact lookup/count correction | 39.06 | 27.06 |

The compiler change removes about 29% of sampled CPU cost per update. The final
build remains about 28% below baseline. Host load was uncontrolled (logical CPU
samples 48.3%, 64.8%, 60.2%); the compact lookup's separate speed effect is not
resolved. This is not a whole-campaign speed guarantee or an isolated
widescreen-cost comparison.

The donor-derived compact lookup idea now uses a 12.3 MB native pointer index,
retaining valid outliers through binary search. Incremental recovery's obsolete
8,863-entry bound is corrected to the actual 8,872-row array size. No CPU register
layout or generated instruction body was changed. The current fast profile has
no x86 interpreter fallback.

Optimized validation passes 32 focused runtime/ABI checks, six dispatch checks,
13 extent checks and 26,621 comparisons on the actual dispatch inventory.
The final build reaches original Stage 1 Results/save using authorized HP and
position assists; a fresh process authenticates the new save and reaches the
selector without guest writes or guards. No full campaign rerun is claimed.

Final tested executable:
`a7ae3b2be65a568134db1032dc490d0c65b747cd2ca8b44acd4f7b732ecfb98f`.
The game repository adopts framework `8d3bce3` and builds its own
`build/play/framework/Release/BlinxRecomp.exe`, SHA-256
`5b7804fb678ab0fd04ff25c8484302f95358174f5e7153ee615b75d7bd076980`.
That build additionally passes 33 focused runtime/ABI/widescreen checks in
19.63 seconds and a fresh original-save load to the selector without guards or
guest diagnostic writes. Its hidden client size is verified as 1680x720.
The copied QA process is retired afterward; no owner playtest is launched.
The [framework audit](../xboxlle/docs/reference/runtime/NATIVE-HLE-OPTIMIZATION-2026-10-18.md)
records exact donor pins, binary identities, methodology, register constraints
and private evidence locations. The short native sample points toward graphics
state/resource synchronization for the next HLE optimization; it does not
justify erasing caller-visible registers or importing a replacement kernel.

Higher internal resolution and uncapped gameplay remain separate enhancements.
Performance issue `beads-xbox.1.100` remains open for the remaining headroom and
broader scene sampling. The bounded 9/9 correctness gate is unchanged.

Compiler audit follow-up: the existing generated `recomp_0025.c` emits C4293
for a carry expression containing a shift by -1 inside a constant-false
`if ((0)&31u)` guard. That expression is not evaluated for a zero shift count;
the warning alone does not establish executed undefined behavior. Retain it for
the next lifter/inventory cleanup pass rather than expanding this performance
batch. Any executable shift/count discrepancy reopens CPU correctness work.

The distant dispatch entry is preserved as an inventory mapping; lookup pointer
equality does not establish that every discovered entry is real CPU code.
Review that discovery if a title actually dispatches into the distant region,
or during the next discovery refresh. It is outside the compact allocation.

## Historical unoptimized playtest - 2026-10-17

The 2026-10-17 widescreen playtest was a functional bring-up build. Its native
pilot target forced `/Od` on MSVC (`-O0` on the other compiler path),
including Release. The CMake comment deliberately retained this setting for
large overlapping generated bodies during discovery. Native CPU code does not
imply optimized CPU code. This profile has no CPU interpreter fallback.

During the owner's 21:9 Stage 1 playtest, read-only samples of the original
main-loop counter measured approximately 10 loops/s with the muted QA copy
running and 14.4 loops/s after suspending QA. Owner process CPU consumption was
about 0.85 and 0.94 cores respectively. Samples were two and five seconds and
the interactive scene was uncontrolled: these are not a matched aspect-ratio
benchmark or a display-cadence measurement. They establish background contention
and a substantial remaining performance gap. CPU cost is a likely contributor;
the cost of widescreen has not been isolated. QA was suspended during the
owner's playtest, and resumed after the owner finished and authorized it.

Host topology inspection found the prior logical mask 12/13/14/15 covered only
two physical cores. Resumed automated QA now uses logical CPUs 8/10/12/15,
four distinct physical cores, at normal priority; its guest CPU remains 15.
This allocation correction has not been measured as a matched FPS improvement.

The optimized comparison above supersedes the earlier compiler bring-up plan.
An isolated aspect-ratio cost comparison is still open. LLE reference throughput
remains irrelevant to its correctness gate.
