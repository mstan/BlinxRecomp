# Windows preview ? 2026.10.28.1

This owner-accepted update reduces repeated native draw preparation, mapping queries and blend/depth/raster object creation while preserving the game's original behavior.

In one matched draw-heavy Stage2-1 workload on the NAS, three alternating pairs observed 10.7?19.3% less CPU per original gameplay update and 6.2?27.1% more original updates per second. Paired medians were about 15% and 27%. Shared-host load varied; these are workload-specific observations, not a universal FPS promise or interpolated-frame gains. Full measurements and limits are included in the pinned framework's docs/optimization/2026-10-28.

The release retains adaptive Fit to screen, graphics/controller options, native saves and all current optional gameplay cheats (off by default). Original time-control activation/stock/recovery, actual gameplay, assisted original Results/save and fresh loading of the newly written native save passed. The owner found the candidate visually and audibly good and snappy. Assisted checks do not establish exhaustive campaign or REC-clone coverage.

Keep your complete existing data folder, external saves/HDD profiles and imported-disc cache when updating. No original game or console files, private progress or QA profiles are bundled. The supported original NTSC-U executable is unchanged. Save states are build-specific; native in-game saves remain a separate protected contract.
