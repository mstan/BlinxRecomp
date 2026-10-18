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
| Round 1, Stage 2 | Original Stage 1 save authenticated/loaded; playable entry, original Results, save and Stage 3 unlock; updated save authenticated on fresh launch. | Six ordinary enemy HP cheats and original goal teleport; incidental stage routes remain unexercised. |
| Round 1, Stage 3 | Playable entry, original Results/save and boss unlock after recovering the original 19-byte `B2360` indirect dispatcher. | Six enemy HP cheats and original goal teleport; incidental routes remain unexercised. |
| Round 1, Boss | Original death route opened its goal portal; goal teleport reached original Boss Results/save and Round 2 unlock. | HP plus source-reviewed wounded-state cheats matching `64BC0/64290`; original `650A0/642F0/624D0` owned the death and portal. Contact/damage production is not established by this assistance. HP alone was insufficient and is not counted as a clear. |
| Round 2, Stages 1-3 | All three stages reached original Results/save and the next stage/boss unlock in one batch. | HP/goal cheats; Stage 1 needed original `43D90` movement/collision recovery through `441B0`, preserving `44000`. Eight/nine/nine enemies were assisted respectively. Unvisited routes remain unexercised. |
| Round 2, Boss | Original death animation, goal portal, Boss Results/save and Round 3 unlock. | HP alone did not clear it. Source-reviewed death-AI-state cheat follows `68EB0/68990/65730`; original `67440/68A80` runs the animation/portal. |
| Round 3, Stages 1-3 | All three stages reached original Results/save and the boss unlock in one batch after a shared renderer correction. | Twelve/fourteen/thirteen enemy HP cheats and original goal teleports. The reached fixed-function TEMP result register is now supported by the native renderer, with actual GPU checks on both prepared backends. Unvisited routes remain unexercised. |
| Round 3, Boss | Original death route, portal, Boss Results/save and Round 4 unlock. | HP and source-reviewed death-AI assistance matching `6C8E0/6C500/6A120`; original `6AF20/6C560` owns the death/portal. The original exit succeeds with a Y offset of +5 from the portal base; the base-only teleport did not establish contact. |
| Round 4, Stages 1-2 | Both reached original Results/save and the next stage unlock. | Sixteen enemy HP cheats per stage, original goal teleports; kind 13 objects are retained. New reached kinds include 8 and 11. Stage 1's longer original loading exceeded the first harness bound; the process continued correctly and the initialized stage then cleared. |
| Round 4, Stage 3 | Original Results transition, signed save and boss unlock. | Fifteen ordinary enemies plus one source-reviewed kind 13 Tom-Tom HP assist, five seconds for original death effects, then goal teleport with Y offset +5. Initial assists that retained the Tom-Tom ended in Game Over. The Results capture is a white transition frame, not a fully drawn results table; screen 10, changed signed save and boss unlock provide the retained completion evidence. |
| Round 4, Boss | Original death route, portal, Results transition, signed save and next-round unlock. | Actual boss kind 133, HP 25, draw `77810`; HP/grounded-wound assistance follows `77C50/76C90/78390`, while original `77570/76D60/76E10` owns death/portal. Positive Y teleport offsets fell below the arena and consumed Retries; a fresh original boss entry and Y offset -36 succeeded. Results capture is a transition view; original screen 10 and changed signed save are retained. No completion/save/unlock fields were written. |
| Round 5, Stages 1-3 | All three reached original Results/save and boss unlock in one batch. | Fourteen/sixteen/seventeen ordinary enemy HP assists plus one/three/zero Tom-Toms respectively, five seconds for original death effects and original goal teleports. Kind 13 assistance requires the reviewed `992D0` draw/contact handler. Entry captures are loading transitions and do not establish complete scene pixels. |
| Round 5, Boss | Original death route enabled portal, then original Results/save and Round 6 unlock. | Main kind `184`, HP 25, draw `71C20`; HP/wounded-AI assistance follows `73670/70C20`. Original `73320/736C0/720B0/70D20/6D6E0` owns death and portal. Separate `8084` appendages remain untouched. Goal teleport uses Y offset -36; no completion/save/unlock fields written. |
| Round 6, Stages 1-3 | All three reached original Results/save and boss unlock in one batch. | Fourteen/fifteen/fourteen ordinary enemy HP assists, plus one/zero/zero Tom-Toms. Newly reached kind 12 (50 HP, draw `60BA0`) follows its original HP death route. Original stage indices 20/21/22 are retained with captures and signed-save hashes. |
| Round 6, Boss | Original 210-tick death sequence enabled portal, then original Results/save and Round 7 unlock. | Main kind `186`, HP 30, draw `7AEC0`; explicit death-AI/cinematic data assistance matching `7D2E0/7CD90/78E10`, with original `7AA80/7CE80` owning animation/portal. Position assist Y -36; no completion/save/unlock fields written. First entry consumed Retries because Start was ignored during the intro. The review harness now suspends the owned muted host process and checks a stable guest counter, then resumes it for the actual test. This is not a save state. |
| Further normal stages | Not yet exercised. | Continue clearing verified enemies and teleporting to original goal gates. |
| Later boss stages | Not yet exercised. | Identify actual boss HP/update contracts before cheating damage; Round 1 assistance must not be blindly applied to different kinds. |
| Time abilities | Attract reference contracts are scoped elsewhere. | Exercise each playable ability, including rewind and fast-forward, with real gameplay state and authorized inventory cheats if needed. |
| Shop | Original Retry order/cancellation and checkout; balance 10,005G to 9,805G, Retry count to three. Original signed save changed; fresh process authenticated it, showed three Retries in gameplay and 9,805G in the shop selector. | Currency assistance used only carried stage coins; original Results transferred them to the bank. Over-budget checkout left the bank unchanged. Other item/upgrade types remain untested. Fresh reload evidence includes `campaign-014` capture 14 and `shop-reloaded`. |
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
