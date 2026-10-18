# Gameplay performance follow-up

The 2026-10-17 widescreen playtest is a functional bring-up build. Its native
pilot target still forces `/Od` on MSVC (`-O0` on the other compiler path),
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
the cost of widescreen has not been isolated. QA stays suspended during this
playtest rather than competing with the owner's copy.

Next bounded comparison: render the same paused gameplay scene in one adaptive
binary at 4:3 and 21:9, with one game running, and compare frame counts/CPU costs.
Keep camera, simulation, backend and internal resolution fixed. Then profile
the dominant path and prepare a separate optimized native candidate, retaining
the reference binary. Guard against optimizer-exposed generated-code defects;
compare meaningful ABI/service checks and actual gameplay/Results/save, rather
than launching a new broad or long qualification loop for each change.

Higher internal resolution and uncapped gameplay/FPS are not part of this
first fix. LLE reference throughput remains irrelevant to correctness; this is
the native HLE play profile's performance debt. Track it in
`beads-xbox.1.100`, separately from the completed bounded qualification gate.
