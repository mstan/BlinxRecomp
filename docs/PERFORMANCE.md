# Optimized native gameplay - 2026-10-18

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
The [framework audit](../xboxlle/docs/NATIVE-HLE-OPTIMIZATION-2026-10-18.md)
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
