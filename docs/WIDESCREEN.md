# Widescreen mod

`blinx.enhancement.widescreen` is a default-off, data-only bundled package.
Its `widescreen` feature selects the trusted, statically linked
`blinx.widescreen` plugin. There is one configurable aspect, from 4:3 through
32:9; 16:9, 21:9 and 32:9 are presets. Adaptive follows the actual window's
client size within those limits and initially opens at 21:9.

```powershell
py -3 tools/game.py run --widescreen adaptive
py -3 tools/game.py run --widescreen 21:9
py -3 tools/game.py run --widescreen 3440:1440
```

No option enables the feature implicitly. The launcher clears inherited mod
environment, resolves the bundled manifest to a deterministic plan, and records
its catalog/plan hashes. Invalid aspect, package/plugin identity or original
projection guard fails before mod effects. Packages do not supply native code.
The shared Xbox launcher exposes these controls through its existing mod
provider and persists the owner's selection alongside the other visual mods.

The original perspective setup passes a negative 4:3 aspect at `2C86A`, builds
the matrix at `39D910`, and calls SetTransform at `2C893`. The plugin adjusts
that matrix data at the API boundary before the game's following matrix copy.
The game's CPU projection consumers consequently use the same wider matrix.
The original code/image and generated function bodies are unchanged. The
[original patch author's Blinx entry](https://ps2wide.wordpress.com/xbox/)
independently identifies this perspective literal; this implementation uses
the trusted mod boundary instead of patching the XBE.

The main dispatcher selects its current screen using the low seven bits at
`50A57C`; gameplay is screen 9. `50A5C4` is the pending screen and must not be
used to select the renderer's current frame. Gameplay widens; menus and
stage selection retain centered 4:3 presentation. Results (screen 10) now
widens the replay's 3D camera while keeping the overlay centered in 4:3. Identified
overlay sprites and text keep their proportions, with health/time and the
bottom collected-crystal queue at left, monster icons and bottom trash inventory at right, and time
controls/dialogue/pause UI centered. The bottom queue was initially centered;
owner playtest evidence exposed that gap and established independent left,
center and right groups in the lower HUD strip.
Populated QA additionally found that the circular slot artwork uses a padded
X=0..256 quad even though its visible circles occupy the smaller left group.
That quad now shares the crystal icons' left anchor; the adjacent rewind
button starts farther right and retains its centered anchor.
Full-screen
history passes and ordinary world geometry retain their own raster path.

Widescreen alone retains the original 640x480 internal render buffers and
anamorphically presents them. The separate optional visual-quality mod supports
native 1080p–2160p internal rendering; guest resource metadata remains original.
See [visual quality mods](GRAPHICS-MODS.md) for the current launcher controls.
Refresh-rate/FPS changes are separate.

Bounded runtime evidence: `wide-stage-006` renders the same Stage 1 paused scene
at 16:9, 21:9 and 32:9, then reaches original Stage 1 Results, authenticated save
and Stage 2 unlock with explicit HP/position cheats. This is assisted coverage.
The portable C contract suite checks disabled behavior, six aspect profiles,
projection/proportional anchors/menu scope and invalid plan rejection. Actual
GPU backend, FVF snap and float-blend checks pass.

Owner playtest follow-up (2026-10-18, `beads-xbox.1.7.12`): guest display Reset
preserves the player's host-window size, position and maximized state. The
backend regression checks this across Native, 1080p, 2160p and supersampling.
The private mod also distinguishes the depth-tested visibility proxies sent by
the immediate sprite helper from edge-anchored HUD. These proxies follow the
camera even in a HUD corner or when their bounds cover the original frame.
Type-4 projected world markers are selected by the original program pair at
`6678A8`; their centers already use the wide projection, so only their pixel
width is corrected. Other depth-tested geometry remains untouched.

A short isolated Stage 1 run reaches original Results with the wide replay
and centered overlay. Focused portable contracts cover replay/menu separation,
proxy placement at both old horizontal boundaries, marker width and unchanged
marker center. The owner's enemy/door edge cases and health-circle appearance
remain a human playtest gate; these corrections do not claim full-scene or
all-stage visual qualification. Keep remaining concrete cases on this issue.

Actual populated-HUD captures in `campaign-024/025` verify three crystals with
their slot frames and three original owned trash objects at 32:9/21:9. These
are assisted original Round 2 Stage 1 entries with the entrance/mission overlay
still visible, not complete unobstructed scene evidence. The native six-profile
suite now includes the measured padded frame and its neighboring centered
button; both CTest entries pass (0.62 seconds). The original executable bytes
and generated CPU bodies remain unchanged by this mod correction.

Optional developer draw tracing can restrict its bounded output to HUD draws
with `BLINX_MOD_DRAW_TRACE_UI_ONLY=1`, or begin only after a populated inventory
with `populated`. This diagnostic is disabled during ordinary launches.

Deferred enhancement questions: unvisited HUD/effect producers and remaining
fringe-culling cases in later scenes. Resolution, controller artwork and shared
launcher controls are now implemented; see GRAPHICS-MODS.md. Keep adding
concrete newly discovered questions here or in
the campaign ledger; do not treat this bounded first-stage proof as full-game
visual qualification. No netplay or savestate compatibility is asserted.

Owner-requested follow-up (2026-10-18), tracked in `beads-xbox.1.7.4`: add a
separate default-off visual-quality mod with genuine internal rendering at
1080p, 1440p, 1800p and 2160p, preserving the selected aspect through 32:9.
These are height presets; 21:9/32:9 produce wider images than their 16:9 names.
Keep output/window size independent and retain the original Native choice.
Antialiasing, supersampling and texture filtering should be separate controls.
Only working, supported options belong in the UI. The owner will playtest
performance; keep a catalog visibility policy so selected expensive options
can then be hidden without removing the implementation or changing quality
silently during play. The implementation and shared launcher controls are now
available; owner performance playtesting remains separate.

Higher FPS is a separate mod: prefer real faster updates only when game speed,
physics, timers, audio and the five time controls remain correct. Optional
interpolated presentation must preserve simulation timing and be labeled.
Resolution/AA should not wait on FPS research. The shared implementation order
and resource/copy/readback acceptance are in
[the renderer plan](../xboxlle/docs/BLINX-ENHANCED-RENDERER.md#resolution-quality-and-frame-rate-request-2026-10-18).
Resolution and blended presentation interpolation are implemented. Real faster
simulation remains deferred and unavailable in the launcher.
