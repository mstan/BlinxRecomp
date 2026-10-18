# Visual quality and frame-rate mods

The native renderer can render at 1080p, 1440p, 1800p or 2160p, with optional
fast approximate AA, 2×2/3×3 supersampling and anisotropic filtering. These are
independent, default-off mods. The existing widescreen mod retains proportional
HUD anchors and supports adaptive/fixed aspect through 32:9.

Build the game and shared Xbox-themed launcher with the reviewed dependencies
in `local/paths.json`:

```
python tools/game.py build --mode play
python tools/game.py build-launcher
python tools/game.py run --mode play --launcher
```

`launcher_ui` must be the exact revision in `game.toml`; `launcher_sdl_sdk`
points at the current validated SDL2 VC SDK folder containing `cmake` and its
license. Preparation records the clean UI revision and explicit host profile
adapter without changing its source checkout. The launcher shows only controls
the host consumes. Catalogs remain data; the game verifies its original XBE
identity and resolved choices before execution.

After building the launcher, a normal interactive `run` opens it automatically.
The Skip launcher checkbox retains the chosen mods and volume for subsequent
launches. `--launcher` always reopens settings. `--no-launcher` bypasses saved
preferences and uses explicit CLI options, for example:

```
python tools/game.py run --mode play --no-launcher --widescreen adaptive --internal-resolution 1080 --antialiasing fxaa --anisotropy 8
```

The normal owner-facing run has audio enabled. Automated headless/replay runs
are muted. Owner preferences live in ignored `local/mod-selection.json`.

The dashboard includes the shared recomp-ui **Player 1** source card and live
SDL controller detection. Choose Keyboard, None, or a named connected gamepad.
The selected gamepad GUID persists across launcher/game processes; gameplay
opens that device through the same SDL2 controller mapping and waits for it to
reconnect rather than silently substituting another controller. Identical
models sharing a GUID use the first matching device, as in the shared UI.
Keyboard retains the existing Xbox host mapping. Binding remaps are not offered.
The optional SDL SDK setting enables this transport; builds without it retain
the existing XInput/keyboard path. Explicit SDL selection requires an SDL build.

The internal height is independent of window size. The initial aspect selects
the internal width; adaptive projection/presentation still follows subsequent
window resizing. 2160p ultrawide costs more than 4K at 16:9. Supersampling
multiplies that cost. Unsupported combinations exceeding the renderer's maximum
dimension are rejected before allocation. No automatic quality changes occur.

Frame rate is one mutually exclusive mode choice:

- **Original:** the game keeps its original updates and presentation.
- **Blended interpolation:** an independent host worker targets 60/120 displayed
  frames per second while the game's updates, timers, input, sound and time
  controls keep their original cadence. It adds one source frame of latency and
  can show double edges. Busy rendering can miss presentation deadlines.
- **Actual higher-rate game updates:** retained as a distinct generic Xbox mod
  mode, currently unavailable for Blinx. Changing the frame wait alone does not
  prove correct movement, physics, audio, input and time-control durations.
  Its title timing adapter must be qualified before this choice becomes visible.

The renderer never feeds interpolated output into the game's front-buffer
readback, depth, feedback or recording history. Menus, movies, pause, stage
changes and Reset clear or bypass presentation history. AA filters the composed
HUD and may soften text; it does not change the HUD anchors.

Owner performance feedback can hide expensive or unwanted choices through
`mods/ui-visibility.json`, independently of their implementation. For example,
hide 2160p by adding
`"xboxlle.enhancement.visuals/resolution/height": ["2160"]` to `hidden_choices`.
Hide one FPS mode similarly with
`"xboxlle.enhancement.frame-rate/frame-rate/mode": ["interpolated"]` or
`["actual"]`. Hidden active choices are disabled on the next launch, so nothing
keeps running invisibly. Framework and game visibility policies both apply.

Framework architecture, targeted validation and explicit follow-up triggers:
`xboxlle/docs/NATIVE-VISUAL-MODS-2026-10-18.md`.

The pinned framework `4892b6be800adfb30f5fbd9029ae9cdb51c25745` passes
47 focused runtime/GPU checks, the game-owned six-test contract/GPU batch and
four compiled launcher/provider checks. An assisted Stage 1 run at native
1080p with AA/filtering reached the original Results/save transition; the
game-owned build then freshly authenticated that save and ran 180 gameplay
updates with interpolation enabled and no guard stops. These checks establish
bounded integration, not a performance guarantee at every preset. Under the
loaded four-core test, interpolation missed its presentation target; owner
feedback should compare original timing first. A new automated ability-input
sequence did not activate its first ability, so that attempt is inconclusive.
Prior campaign ability coverage is retained rather than relabeled as new proof.
