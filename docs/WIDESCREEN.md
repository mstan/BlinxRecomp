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
The Xbox shared launcher theme exists; its mod-provider UI is not yet connected
to this title. This bounded catalog adapter can feed that provider without
creating a separate title-specific mod manager.

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
used to select the renderer's current frame. Gameplay widens while menus,
Results and stage selection retain centered 4:3 presentation. Identified
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

The implementation retains the original 640x480 internal render buffers and
anamorphically presents them. At wider aspects, horizontal detail in UI and
the scene is consequently limited. Higher internal resolution is a separate
enhancement; the mod does not silently change guest resource sizes or feedback
allocations. Refresh-rate/FPS changes are also separate.

Bounded runtime evidence: `wide-stage-006` renders the same Stage 1 paused scene
at 16:9, 21:9 and 32:9, then reaches original Stage 1 Results, authenticated save
and Stage 2 unlock with explicit HP/position cheats. This is assisted coverage.
The portable C contract suite checks disabled behavior, six aspect profiles,
projection/proportional anchors/menu scope and invalid plan rejection. Actual
GPU backend, FVF snap and float-blend checks pass.

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

Deferred enhancement questions: unvisited HUD/effect producers, fringe culling
in later scenes, higher internal resolution, controller artwork, and the shared
launcher mod UI. Keep adding concrete newly discovered questions here or in
the campaign ledger; do not treat this bounded first-stage proof as full-game
visual qualification. No netplay or savestate compatibility is asserted.
