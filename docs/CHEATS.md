# Gameplay cheats

Optional launcher mods in the **Gameplay** group (`blinx.enhancement.cheats`),
off by default. They are compiled into the trusted registry
(`src/mods/cheats.inc`) and selected through title switches; original CPU
bodies are unchanged.

| Mod | Behavior |
| --- | --- |
| No stage time limit | Stages never fail at 10:00. The final 30-second countdown warning never appears; the elapsed timer and Results keep real time. |
| More time-control stock | Hold up to 20: the original 10 slots plus a 10-control reserve. A pickup that would drop the oldest control sends it to the reserve, which refills free slots once no control is active. HUD badges count slots plus reserve (the badge art stops at 10 per icon); the 10 slot pips stay full at 10 or more. The reserve lasts until the game is closed; only the 10 slots are saved. |
| Infinite time controls | Using a control keeps it in the same slot. Each stage starts with, and keeps, one of each of the five controls. Non-heart crystals never spawn (placed or dropped) except in the final boss (stage 39). Heart crystals are unchanged. |

The switches are independent; enabling both is allowed. No mod writes hearts:
they carry between stages through the original save (`1A820` stores
`max(hearts,0)` at `save+2C`; `A5AB0` reloads it).

## Source facts (NTSC-U, XBE `e6b7cd8a...`)

- Stage clock `50C2BC` counts frames upward. `A6740` calls the shared fail
  routine `A2620` once it reaches `.rdata` `1C960C` (17989.0) and clamps it to
  `1C9608` (17999.0). These are their only readers. Time history indexes
  modulo frame%30 and second%300, so the clock may run past 18000. The cheat
  raises both constants through the host's read-only image override, which
  restores Xbox page protection.
- `A18C0` draws the countdown only when `18000-clock < 3A2`, then the elapsed
  readout. The cheat shows the countdown a zero clock and restores the real
  clock at its `2A0B0` call from `A1A63`.
- Time-control stock: count `50A39A`, records `{kind,duration}` at `50A39B`,
  serial IDs at `50A3B0`, effect handles at `50A3C4`, capacity `50A5C8`
  (save value, minimum 3; the original uses 10 itself). Kinds 2-6 are
  PAUSE/SLOW/FF/REW/REC. `C26E0` adds controls from four crystals and drops
  the oldest when full; `C2A20` activates the slot `[50A5C0]` and `A6740`
  removes it inline. Inserted records use the original loaders' shape (new
  serial from `62F0A0`, handles -1).
- `A0B90` draws 10 capacity pips from slots `[0,capacity)` and per-kind badges
  by counting `[0,count)`; badge digits are 24-px cells of a 256-px atlas (1-10).
  More stock appends reserve kinds after the slots for that call only and
  restores the overlapped ID bytes on exit.
- Crystals are items `59-64`, color `(kind-59)%6`, color 3 being the heart.
  Every item is allocated by `C5720`, including enemy drops (`C5E60`). The
  Infinite cheat removes a new non-heart crystal exactly as the original
  pickup does: inactive byte, `40D7B0` count, then `C2490` unlink.

Hooks are the static title functions in the framework's
`tools/native_profiles/blinx-ntsc-u-title.json` (hash-bound bodies).

## Validation

`blinx_cheats_*` CTests model each mode, invalid selections and an
unwritable image. Muted native QA on stage 1-1 (`cheats_qa.py`, retained in
the task workspace) covers the off baseline failing at 17989, the clock
passing 18195 without fail or countdown, a stocked stage, Infinite REW use,
14 placed crystals removed with hearts kept, and More stock overflow plus
reserve refill through the original pickup and activation. Enemy-drop
removal is covered by the shared `C5720` path and the contract test, not by
a live kill. Time controls held at stage end are saved as usual.
