# BlinxRecomp

This is the private original NTSC Blinx game repository in the Xbox ecosystem.
Read README.md and game.toml first. Global owner instructions still apply.

- Game identity, revision metadata, seeds, input replays and build/launch glue
  belong here. Shared CPU, kernel, hardware, renderer, audio and service code
  belongs in the xboxlle framework. Prefer the sibling framework worktree for
  changes, commit there, then update this submodule and game.toml together.
- The framework is pinned to a tested commit. Do not silently follow a branch.
- Central Beads: F:/Software/beads/issues. Game epic beads-xbox.1.7;
  repository split beads-xbox.1.7.5; shared LLE burndown beads-xbox.1.102 and
  controller family beads-xbox.1.104. Never initialize Beads in this repo.
- Maintain the LLE reference floor and match observable HLE contracts. No stubs,
  fabricated completions, live switching or duplicate production execution.
  Bundle related checks and perform actual title validation per coherent batch.
- Keep owned game/kernel/media/EEPROM/capture data and generated original code
  in ignored local/build paths. Private visibility does not change license scope.
- Work solo unless the owner authorizes agents for the current task.
