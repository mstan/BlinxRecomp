# Windows controller hotfix - October 23, 2026

Controllers that appeared connected but gave no input now use the shared SDL2
Windows notification pump. This corrects the native title's missing RawInput
message dispatch while retaining the selected device and hot-plug behavior.
The fix is compiled into the game; Steam and global environment settings do
not need to change.

The health-circle correction, stage-entry improvements, camera-invert options
and previous crash/transparency fixes remain included. Gameplay keeps its
original timing. Existing campaign and optional-mechanics limits still apply.

Extract the complete Windows ZIP and open Start Game.exe. Preserve the existing
data folder, console profile and selected save storage when updating. Personal
saves and original game/console inputs are excluded from the clean package.
Matching project/framework and prepared dependency source is included under
source/.
