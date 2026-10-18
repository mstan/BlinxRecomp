# Campaign gameplay coverage

This ledger is separate from the completed, bounded 9/9 attract-family gate.
The assisted main-route smoke pass is complete. Broader natural combat,
ability-puzzle and optional inventory coverage is deferred in
`beads-xbox.1.7.9`, with concrete reopening triggers below.
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
| Round 7, Stages 1-3 (Everwinter) | All three reached original Results/save and boss unlock in one batch. | Sixteen/seventeen/seventeen ordinary HP assists plus one/zero/one Tom-Toms; newly reached kinds 9 and 10. Original stage indices 28/29/30 differ from the display order, verified through original Pause/Exit and the Round 7 selector before extending the guard. Capture after death-effect settling shows actual gameplay; initial loading captures are not visual passes. |
| Round 7, Boss | Original lethal-HP death route enabled portal, then original Results/save and Round 8 unlock. | Kind `188`, HP 40, draw `7F470`; HP/wounded-AI data assistance follows `822D0/81C80`, retaining hit direction. Original `7F230/81DA0/7E1A0/81F90` owns death and portal. Goal teleport Y -36; no completion/save/unlock fields written. Original stage index 31, signed-save hash and visually inspected Round 8 selector retained. |
| Round 8, Stages 1-3 (Forge of Hours) | All three reached original Results/save and boss unlock in one batch after reached native CPU/renderer fixes. | Eleven/thirteen/eleven ordinary HP assists plus two/one/zero Tom-Toms; original stage indices 32/33/34. Two-byte G8B8 layout/sampling and native unsigned BUMPENVMAP now have focused real GPU coverage. Measured original `B95B0/B2930` wrappers are recovered exactly; separately inventoried `B9FD0` is not a live coverage claim. No completion/save/unlock fields written. |
| Round 8, Boss | Original lethal wound/death opened the portal, then original Results/save and visibly confirmed Round 9 Momentopolis unlock. | Kind `189`, HP 40, draw `8A430`; wait for original multipart introduction to set phase one, then main HP/wounded-AI assistance matching `8A670/87040`. Original `8AC20/89340/87160/835B0` owns lethal death/portal; linked actors remain untouched. The assisted hit produced an invalid vertical destination; body/previous/death-target position assistance aligns it to the original arena portal height. Original timers, portal, completion, save and unlock fields are not written. Player goal teleport Y -36. Stage index 35 and signed save `4dc76c1f...` retained. This does not qualify natural airborne lethal hits. |
| Round 9, Momentopolis | Original intro and four rematches, pre-final shop, final-boss death, rescue scene, ending movie/audio decoding, signed save and title return. A fresh process authenticates the completed save and shows Round 9 Boss without the New label. | Cached native fans fix the reached draw guard. Rematches use kinds `185/188/186/189` at original indices `19/31/23/35`; their portal/movie transitions are not intermediate Results/save passes. Pre-positioning the Forge boss body before its wound lets its original death ray complete without writing the death destination. Final kind `18A`, stage 39, uses an explicit lethal-HP/wounded-AI assist matching `8F940/8DF20`; this bypasses the variant-zero/one five-HP floor while leaving variant byte C1 untouched. Original `94140/93170/8BC60` owns death, then stage 37 rescue, `end_a01.sfd/.aix`, screen 8 save and screen 1 title. Natural final-boss forms/phases remain unqualified. |
| Campaign route total | Twenty-four normal stages, eight pre-final bosses, four rematches and the final boss reach their original downstream transitions; the ending writes a completed save. | This is the assisted main route, not exhaustive puzzle, natural combat, optional content or every title branch coverage. No completion/save/unlock fields are fabricated. |
| Time abilities | PAUSE, SLOW, FF, REW and REC activate through original B/directional selection, consume inventory, display their effects and return to stable normal play. REC executes recording, rewind, clone playback and cleanup (`6 -> 4 -> 7 -> 0`); original cache files are written. | Only source-reviewed time-item count/type/duration records are supplied by cheats (`C26E0/C2A20`); active/pending phases, clocks and completion fields stay original. This is activation/lifecycle smoke coverage, not precise world-history parity, a moving clone path, every puzzle or boss interaction. Those broader scenarios remain explicit deferrals. |
| Shop | Original Retry order/cancellation, checkout and fresh-process persistence. Pre-final shop also purchases TS-4000 Star Lv.2 for 2,000G through the original PAY flow: 12,202G to 10,202G, quantity one and the new model displayed. | Currency assistance used only carried stage coins earlier; original Results funded the bank. Over-budget checkout leaves the bank unchanged. Weapon purchase is followed by the original ending save; its equipped gameplay model after fresh load still needs inspection. Clothing and other inventory types remain untested. |
| Widescreen gameplay | Same Stage 1 scene at 16:9, 21:9 and 32:9; original Results/save and Stage 2 unlock at 21:9. Populated HUD now checked at 21:9 and 32:9 in original Round 2 Stage 1. | The actual padded slot-frame quad at X=0..256 now shares the crystal icons' left anchor; the adjacent rewind button remains centered and the three-object trash inventory stays right. Native six-profile checks and actual title captures pass. Later effects/fringe visibility remain enhancement questions. |

Private evidence is retained under the framework worktree's ignored
`build/blinx/gameplay-coverage-20261017`: `stage-002` through `stage-004` for
baseline coverage, `wide-stage-006` for the assisted wide route, and subsequent
`campaign-*` runs. Every cheat records the exact before/write/readback bytes
against the owned test process identity. Preserve unexpected native guards and
repair original function discovery or shared services in their owning layers.

Non-essential enhancement questions are listed in [WIDESCREEN.md](WIDESCREEN.md).
Add concrete new questions there as they are discovered. No unvisited stage or
unexercised ability is a pass by default.

Deferred natural-gameplay questions: `beads-xbox.1.7.8` retains the Round 8 boss
death destination near `4.61138e19` after the explicit wounded-AI cheat. The
cause is not isolated between the assisted setup and translated raycast/result
handling. Reopen for a natural lethal-hit reproduction, a stuck/vanishing boss
report or direct evidence of a CPU/raycast contract error. The position-assisted
clear above proves the original downstream portal/Results/save, not that path.

Final-route evidence: muted `campaign-020`, executable SHA-256
`9a09d86b23c945c13a1d85f13c21fd5420a16a78c1f9d205c5bff975570bf785`,
framework runtime `a3accc1` plus evidence-only `6bb5fc0`. Original completed
6,000-byte save SHA-256:
`ef9bfdad233b6a4eae751978bcbf4738b5f7a54c31eed2a582c6556923c37b2c`.
`campaign-021/completed-save-reloaded` authenticates that unchanged save in a
fresh process with no guest-state writes. Its Stage 1 reentry likewise uses
original selector input. Initial entry pixels still show loading and cannot
establish the equipped weapon model.

Deferred natural final-boss mechanics (`beads-xbox.1.7.9`): the explicit lethal assist bypasses
the original five-HP floor and does not exercise all forms, phase transitions,
damage producers or ability-dependent combat. Retain those original routines;
reopen on a natural-play phase/ability softlock, missing or incorrect form,
damage-floor regression or a report that the encounter cannot be completed.
Downstream ending/save coverage does not qualify those mechanics.

Time-control evidence is retained in `campaign-021`: item kinds 2/3/4/5/6
map to PAUSE/SLOW/FF/REW/REC. Original active states are 1/2/3/5/6;
REC additionally transitions through rewind state 4 and clone playback state 7
before stable normal state 0 (counter 32104 onward). The first transient zero
before clone playback is not completion. Early `time-rewind-enter/recovered`
filenames are misnamed PAUSE captures; actual REW is `time-kind5-*`.
No phase or timer is cheated. Broader moving-clone/puzzle/combat scenarios
remain unexercised rather than being inferred from these smoke checks.

Populated-HUD evidence: `campaign-024` (32:9) and `campaign-025` (21:9),
original stage index 4. A missed selector pulse initially reached Round 2,
which is retained honestly in the proofs rather than relabeled Stage 1 Time
Square. Three original trash objects use their reviewed pickup owner/queue/
phase layout; three crystal types use the original queue indices. Executable
SHA-256 `57ca004c37364219658dd31af37b4a31112b377696abc590d07393ee84b056d1`.
The original entrance/mission overlay is visible; these captures establish HUD
alignment, not complete unobstructed scene pixels. Two native CTest entries
pass (0.62 seconds), including all six aspect/anchor/menu profiles and existing
invalid-plan guards. The actual padded frame and neighboring centered button
are regression fixtures. Original CPU bodies and completion logic are unchanged.
