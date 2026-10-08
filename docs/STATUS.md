# Original NTSC Blinx

The pre-split working attract milestone is preserved in xboxlle
`milestone/blinx-attract-20261008`. Runtime qualification uses `d056bd5`.
The current pin `50d344e` adds only README/contract/burndown documentation;
the source used for the qualified executable is unchanged.

| Area | Current evidence / remaining work |
| --- | --- |
| Native attract | Movies, Demo play, Start interruption, title return and reentry observed; most recent framework run was 190.187 seconds with 19 captures and no terminal guard |
| Cadence | Accepted 28.5–28.8 fps debt is tracked in `beads-xbox.1.100`; no new speed claim from the repository split |
| Calendar LLE | Original kernel conversion functions execute in title calls; qualified alongside native counterpart |
| Input LLE | Original XAPI creates three real kernel device objects and an 8192-byte contiguous buffer, then stops at `MmLockUnlockBufferPages`; USB transfer/input delivery unqualified |
| LLE hardening | Nine broad service-family gates remain open in `beads-xbox.1.102`; input `.104` is active; batch related dependency checks to reduce repeated setup/title runs |
| New Game | Known post-save fault, `beads-xbox.1.101`, deferred until the current correctness phase |
| JSRF | Serial follow-up after Blinx; no game-specific work begun by this split |
| Dashboard choice and storage | Required architecture, `.22`: original dashboard remains supported and its assets are optional for native launch; provision/import a genuine virtual HDD and retain real persistent saves. Host-directory persistence is working but shared FATX image access and the combined launch selector are unfinished. |

Detailed source/evidence is maintained with the implementation in
[INPUT-LLE.md](../xboxlle/pilots/native-title/INPUT-LLE.md),
[CALENDAR-LLE.md](../xboxlle/pilots/native-title/CALENDAR-LLE.md) and
[BLINX-BURNDOWN.md](../xboxlle/docs/BLINX-BURNDOWN.md).

The game repository adopts the existing generation manifest only after checking
every generated file, the original image, CPU donor, all seven binding records
and both original-function recovery records. The generated output remains
ignored; these files are never pushed. The original framework worktree and its
qualification captures are retained for comparison.

Fresh full regeneration is available through the shared preparation command but
is not claimed as validated by the initial repository split. Validation uses
the hash-verified existing output, compiled through this repo's own CMake entry.

## Game-repository qualification - 2026-10-08

- `py -3 tools/game.py build` produced `build/attract/framework/Release/BlinxRecomp.exe`.
- `py -3 tools/game.py test`: all 40 tests pass (17.90 seconds).
- The game-owned launcher ran 190.157 seconds, producing 19 captures with no
  terminal guard. Captures 9/12/16 show Demo play, the title screen following
  Start interruption, and Demo play reentry. The harness killed its own child
  at the deadline; recorded exit 1 is not a crash.
- Executable SHA-256:
  `b3106fd20f8061183796a2071866b11996841d56afcb1188ae7f58d15b502e43`.
- Private evidence: `local/runs/20261008-112424-attract/`, including `run.json`
  and `title.log`; test/build logs are in `local/`.
- Index review includes 19 source/configuration blobs and the declared framework
  gitlink. The generic framework hygiene scanner flags submodules for review;
  this exact URL/pin was reviewed, and the framework history scan reports 2,636
  unique blobs with zero findings. Owned media, generated code, local state and
  dependency binaries remain ignored and outside the push.

This validates repository/build/launcher integration and preservation of the
working attract path. It does not close outstanding fidelity or cadence issues.
