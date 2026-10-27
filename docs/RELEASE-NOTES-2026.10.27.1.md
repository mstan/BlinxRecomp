# Windows preview - 2026.10.27.1

This release enables the qualified native preparation and whole-program
compilation package. In the matched draw-heavy 2-1 NAS workload, CPU time per
original update fell from 62.126 ms to 54.975 ms (about 11.5%), with about 8.4%
higher original-update throughput. These are approximate workload/host results,
not a universal FPS increase; original game timing is preserved.

Adaptive widescreen, 1080p, 2x2 supersampling, FXAA, 8x filtering, 60 Hz blended
presentation, controller selection and existing correctness fixes are retained.
The shared display-only vblank wait correction is unconditional.

Validation includes native save writing and independent fresh-process loading,
actual gameplay, all five time-control activation/stock/recovery checks and
focused GPU/resource/callback gates. This smoke does not establish REC clone
playback or exhaustive campaign coverage. The owner accepted the final game
visually and audibly. Full samples and limits are in the framework qualification
data and this repository's performance notes.

Keep your complete existing `data` folder and external save/HDD profile when
updating. Original game/console files, private progress and review data are not
included. The supported original NTSC-U executable remains unchanged.
