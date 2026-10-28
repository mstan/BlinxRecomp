# Windows preview - 2026.10.27.3

Adds optional gameplay cheats under **Mods > Gameplay > Blinx Cheats**. All
are off by default, independent and can be combined:

- **No stage time limit**: stages never fail at 10:00 and the final
  30-second countdown never appears. The elapsed timer and Results keep real
  time.
- **More time-control stock**: hold up to 20 time controls, the game's 10
  slots plus a 10-control reserve that refills slots as you use them. Icon
  counts include the reserve (up to 10 per icon). The reserve lasts until the
  game is closed; only the 10 slots are saved.
- **Infinite time controls**: always hold one of each time control, and using
  one never uses it up. Only heart crystals appear, except in the final boss.

Hearts are never changed; they still carry between stages through your save.
Graphics, timing and save behavior are otherwise unchanged from 2026.10.27.1.

Validation: contract tests for each cheat, combinations, invalid selections
and the read-only constant override; muted native stage 1-1 checks of the
original 10:00 fail without the cheat and its removal with it, a stocked
stage, kept controls after use, removed non-heart crystals with hearts kept,
reserve overflow/refill and the combined HUD counts; and an owner playtest.
Enemy-drop crystal removal is covered by the shared spawn path and contract
test rather than a live kill. Details: [cheats](CHEATS.md).

Keep your complete existing `data` folder and external save/HDD profile when
updating. Original game/console files and private progress are not included.
