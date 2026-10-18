# Campaign gameplay coverage

This ledger is separate from the completed, bounded 9/9 attract-family gate.
Campaign tests may use explicit HP/position/currency/item cheats, as requested.
They must still reach original gameplay, Results and save operations. A cheated
exit does not establish an unassisted route or exercise every puzzle/trigger.
Owner playtests use separate processes and saves, with normal audio. Automated
QA is muted; never close or manipulate the owner's running game to collect QA.

| Area | Current observed result | Assistance / remaining scope |
| --- | --- | --- |
| Round 1, Stage 1 | New Game/save, tutorials, movement/jump, trash/crystal collection, two ordinary enemy kills, both gold doors, damage/Retry/Game Over/reentry; original Results/save and Stage 2 unlock. | Final enemy/exit proof used HP/position cheats. Additional wide run zeroed three enemies and teleported to the original goal; completion flags were not fabricated. |
| Round 1, Stage 2 | Original Stage 1 save authenticated/loaded; playable Stage 2 entry. | Exit, new stage-specific behavior and later saves remain to exercise. |
| Further normal stages | Not yet exercised. | Continue clearing verified enemies and teleporting to original goal gates. |
| Boss stages | Not yet exercised. | Identify actual boss HP/update contracts before cheating damage. |
| Time abilities | Attract reference contracts are scoped elsewhere. | Exercise each playable ability, including rewind and fast-forward, with real gameplay state and authorized inventory cheats if needed. |
| Shop | Not yet exercised. | Exercise purchases and persistence; authorized currency/items can accelerate coverage. |
| Widescreen gameplay | Same Stage 1 scene at 16:9, 21:9 and 32:9; original Results/save and Stage 2 unlock at 21:9. | Later HUD/effect producers and fringe visibility remain open enhancement questions. |

Private evidence is retained under the framework worktree's ignored
`build/blinx/gameplay-coverage-20261017`: `stage-002` through `stage-004` for
baseline coverage, `wide-stage-006` for the assisted wide route, and subsequent
`campaign-*` runs. Every cheat records the exact before/write/readback bytes
against the owned test process identity. Preserve unexpected native guards and
repair original function discovery or shared services in their owning layers.

Non-essential enhancement questions are listed in [WIDESCREEN.md](WIDESCREEN.md).
Add concrete new questions there as they are discovered. No unvisited stage or
unexercised ability is a pass by default.
