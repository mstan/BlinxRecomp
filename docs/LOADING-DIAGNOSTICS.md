# Targeted loading/audio diagnosis

The intermittent long Stage 1 load remains unresolved. The owner report has a
25.169-second file-open span; short isolated entries do not establish a fix.
Keep existing timing, audio buffering and graphics resource semantics while
collecting a repeated slow occurrence.

The explicit launch diagnostic keeps the normal launcher, preferences, saves
and audible owner playtest behavior:

```powershell
py -3 tools/game.py run --mode play --diagnose-loading
```

It enables existing graphics service totals, sparse native GPU and file
timings, and a bounded 120-second audio capture/refill timeline. Automated
headless/replay launches still mute the mastering voice while preserving
emulation. The diagnostic is disabled by default and does not change the
selected visual preset, simulation timing, resource work or completion rules.
It adds observation overhead, so this is not a performance benchmark.

Evidence is written to the run's ignored `local/runs/` directory:

- `title.log`: service totals, original file-open timestamps and at most 256
  slow GPU records plus 256 slow file records, each above 50 ms. The file
  record includes time acquiring the outer media owner lock; remaining wall
  time can still include inner locks, host scheduling and actual I/O. Thread
  CPU is bracketed for the file service, with an explicit availability flag.
  Slow synchronous reads additionally report actual NT-read entry/return,
  wall time and bracketed thread CPU, or identify a cache hit with no NT call.
  Per-file I/O-lock acquisition and the existing transfer-log duration have
  separate sampled flags; zero with a false flag means unmeasured. These
  fields use the same slow-service threshold and record cap; no per-read
  diagnostic line is added below that threshold. Formatting occurs after
  releasing ownership locks. NT-call wall time still includes host scheduling;
  cache/position work and guest-CPU reacquisition are not assigned to storage.
  The nested read/lock/log fields require framework commit `312e818` or later;
  an already-running playtest executable may predate them.
  GPU records carry an explicitly cumulative end-of-call thread CPU sample,
  not an asserted CPU cost for that individual call.
- `loading-audio.f32` and `.clock.csv`: at most 120 seconds of actual six-channel
  48 kHz float32 mixer output and its processing clock (about 132 MiB maximum
  PCM), without playing a separate diagnostic sound.
- `loading-audio.csv`: bounded, buffered voice ownership, caller writes,
  queued buffers, sample cursors and thread CPU observations. Check timeline
  overflow and capture completion in the log before interpreting a gap.
- `run.json`: executable identity, exact mod plan and diagnostic settings.

GPU, file and audio times use the same absolute 100-nanosecond QPC clock.
Compare the actual slow call interval with voice empty/refill and producer
write intervals. A high wall time with little thread CPU does not itself name
a driver, disk or lock as the cause. Missing records can mean that an interval
fell outside the observed service, below the threshold, after the capture
window, or beyond a record cap. No full campaign or qualification sweep is
needed for this diagnosis.

For a repeated slow occurrence, correlate these records with a bounded host
trace containing CPU samples, context switches/wait stacks and File/Disk I/O.
Retain the exact executable and matching symbols actually available from its
toolchain (for example MinGW/DWARF or a matching map); do not assume an MSVC
PDB. Verify symbol resolution on that artifact before interpreting guest/native
stacks. This capture diagnoses waits, not sustained performance.

For an isolated developer Retry investigation, the existing bounded draw
readback also accepts `XLLE_PILOT_FRAME_CAPTURE_TRIGGER`. It starts its first
32 default-target post-draw images only after that file exists. An optional
`XLLE_PILOT_FRAME_CAPTURE_CALLER` nonzero uint32 literal instead selects paired
before/after images for that caller, within the same 32-image cap. Invalid
caller values stop with an explicit diagnostic. Each image's
log records caller, shader, target storage extent, guest source extent and
mod X placement. A capture-only read-only query also reports available native
viewport, scissor state and sampled-image backing extent; unavailable fields
remain explicit. With no trigger configured, the original startup capture
behavior is preserved. Readbacks and profiling must not be used as a speed
measurement.
