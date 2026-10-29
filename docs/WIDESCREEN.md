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

The owner's next playtest exposed a missing target case: normal gameplay
renders its 3D scene into a texture before the `9D9DB` backbuffer copy. The
initial correction's default-target guard excluded those later scene draws.
Identified depth-tested camera proxies and type-4 world markers now receive
their placement on both targets. Offscreen HUD and effect submissions keep
their original placement. The contract suite includes both target paths;
in-game enemy/door fringe visibility and circle appearance remain an owner
acceptance gate rather than being inferred from those contracts alone.

An isolated 1080p/21:9 follow-up (`cull-followup-002`) places the three actual
Stage 1 green enemies at left, center and right using explicit position cheats.
Their original renderer remains active; no visibility or completion flags are
changed. Both outer enemy bodies are visible beyond the centered 4:3 span
(approximately X=137..503 in the 640-pixel readback). The build also reaches
the stage with zero fault guards. This is a bounded assisted smoke check;
the owner's natural camera route, static-door case and exact ring appearance
still require confirmation.

Automatic Retry's horizontal tape streaks now fill the wide frame. A targeted
position-assisted fall reaches the original death handler and Retry state 4,
replays the position history, restores the spawn and returns to normal state 0.
The actual producer is six centered 640x12 sprites per frame from `33811`,
with depth disabled. They use `o_noise`: its original catalogue record is at
`1E21F0`, and its live owner slot is stored at `1E220C` (observed slot 24,
superseding an earlier unverified index-23 hypothesis). The plugin resolves
that slot through `8D2860` and compares the current texture binding before
exempting these full-width strips from proportional HUD placement. Other
textures, geometry, offscreen effects and menus keep their existing behavior.

Retained `retry-target-003/004` evidence compares the old centered span near
X=136..503 with corrected streaks across X=0..639 in renderer readbacks.
The final candidate uses the owner's 1080p/FXAA/8x plan and adaptive clients
1680x720 and 2560x720 (21:9 and 32:9); diagnostic PNGs are 640x480 readbacks,
not native-resolution screenshots. Each original Retry lifecycle recovers,
with no death/time-control/completion flag writes or fault guards. The three
game CTest entries pass, including live-resource replacement, invalid slot,
other-texture, proxy and offscreen controls across six aspects. This targeted
check does not repeat campaign coverage. Final owner appearance confirmation
and the existing enemy/door/health-circle acceptance remain open.
Review found a second defect: a pale tint still occupied the old 4:3 span.
Paired before/after draw captures (`retry-tint-009/frames/draw-1.png` and
`draw-2.png`) identify its first introduction at `30C0A`, not the later
`9FDA6` history pass. The immediate helper submits a centered 640x480 FLOAT3
quad with all Z=1, depth test/write 1/0, to the default target. This exact
full-screen raster contract now retains full-width mapping; other geometry,
world depths, depth writes and offscreen proxies retain their prior mapping.
The tint starts one frame before Retry state 4 (`retry-fixed-012` records
time state 0 at frame 1028, then state 4 at 1029). Restricting the exemption
to state 4 leaves the previous narrow tint carried through frame history.
The live blank texture is not present in the title owner table, so no guessed
catalogue identity is used for this producer.

The final `retry-fixed-012/retry-21x9.png` and `retry-32x9.png` readbacks show
both streaks and tint across the wide frame, with existing HUD anchors retained.
Both actual original death/Retry/recovery cycles pass; three focused game
CTest entries pass (0.51 seconds). No base recompiled function or graphics
resource/raster semantics are changed. Final owner appearance acceptance
remains open; this bounded check does not claim full-game visual coverage.

Owner review on the integrated normal-audio candidate:

- Move the camera past the former 4:3 boundaries: nearby enemy bodies and the
  static door should remain visible until they leave the actual wide view.
- Inspect an enemy health/target circle while moving the camera: its center
  should track the enemy and its displayed shape should remain circular.
- Trigger an ordinary death/Retry: streaks and tint should cover the wide
  frame, the HUD should keep its anchors, and gameplay should recover.

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
The `effects` filter records small full-width strips, with caller, target,
frame, time state and texture bindings. The `raster` filter selects exact
default-target full-screen `30C0A` quads across their lifecycle.
`BLINX_MOD_DRAW_TRACE_TRIGGER` can defer capture
until a named file exists, avoiding a full trace before the target effect.

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
[the renderer plan](../xboxlle/docs/reference/graphics/BLINX-ENHANCED-RENDERER.md#resolution-quality-and-frame-rate-request-2026-10-18).
Resolution and blended presentation interpolation are implemented. Real faster
simulation remains deferred and unavailable in the launcher.

## Mechanical covers and inventory trim (2026-10-25)

Stage entry/exit follows the original cover animation controls from `A4CA0`,
including the held panels around Stage Select. The mechanical atlas
`h_flame_all_1layer` and its glow fill the adaptive raster throughout the
animation; the separate loading label and menu UI retain their proportions.
Stage Select's mechanical frame fills the window while its controls retain
the centered original layout. Other menus without those covers remain 4:3.
The same projection adjustment applies to original camera submissions during
these transitions, including a cover shown before the first gameplay camera.
Original loading work, progress, input and audio are unchanged.

The original four/nine-slot inventory frame has a decorative continuation
below the trash icons. In the wide HUD the two frame pieces left a dangling
line at the bottom right. These identified decorations are clipped outside the
wide raster; actual item icons, quantities and the crystal/time-control groups
remain. Native 4:3 retains the original decoration. Resource ownership is
resolved live, so reloading a stage cannot retain stale texture handles.

Focused contracts cover six aspect profiles, first-camera ordering, partial
covers, held panels, glow, the proportional loading label, unchanged offscreen
producers and unrelated icons, and live resource replacement. A muted native
Round 2 Stage 1 check captures original entry/exit back to Stage Select and
populated inventory at 21:9 and 32:9, exiting cleanly. The inventory setup uses
three source-qualified pickup cheats; completion flags are not changed.
Owner visual acceptance of this candidate remains pending.

Owner follow-up: the main framing and trim changes look good. Stage Select's
lower-right panel still overlapped the Stage 1 marker slightly. Its corner
now moves toward the proportional menu layout while the right edge remains at the
window edge. This adjustment blends out using the original closing
progress, so the fully closed shutters retain their original seams. Other cover
pieces and loading/gameplay framing are unchanged.

## Stage Select animation and Gold notifications (2026-10-26)

Both live `ssl_rXX` preview images keep their full-raster mapping while the
original round-change animation slides and zooms them. This prevents a partial
preview from suddenly narrowing and exposing the preceding image. The Round
word and digit retain proportional widths and share the label slot's anchor
inside the stretched upper-right panel.

The Gold pickup banner, total panel and number glyphs share a left anchor
throughout the original ten-entry stack. Previously the third entry crossed
above the bottom HUD band's boundary and reverted to the centered 4:3 region.
Other HUD groups and native 4:3 placement retain their existing behavior.

Focused contracts cover all nine preview resources, both Round atlas variants,
the ten currency rows and neighboring UI across the six aspect profiles. Native
captures reproduced all three reported defects before their corrections. Gold
notification validation uses original UI-only notification entries and timers
in a disposable profile; it does not alter money, completion flags or saves.
