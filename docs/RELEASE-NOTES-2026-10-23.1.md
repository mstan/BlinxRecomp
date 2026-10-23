# Windows preview hotfix - October 23, 2026

The damaged-enemy health circle now keeps its circular shape in adaptive
widescreen. Its original projected position is preserved. The same correction
applies to the original billboard quads that use this draw contract, including
their submissions into the scene texture.

The previous correction covered a different draw caller. A focused original
health-display fixture identified the actual queued caller and its live 2D
program, four float3 vertices, common depth and depth-write setting. The fix
uses those properties to avoid changing ordinary world meshes or HUD anchors.

Six aspect/projection/HUD/menu contracts, independent visual-plan tests and
neighboring shader/depth/topology rejection checks pass. Before/after native
21:9 captures show the original health display changing from an oval to a
circle. That component fixture teleports Blinx and creates the original health
task directly; it bypasses combat and does not claim a new combat-mechanics
pass. It uses disposable saves and does not modify the owner's progress.

This release also includes the accepted stage-entry presentation improvements
and independent horizontal/vertical camera inversion under Mods. The owner
has checked that all campaign stages load quickly. Blended higher-rate
presentation retains the original gameplay timing. Minor hitches and untested
gameplay paths remain possible.

Extract the full Windows ZIP and open **Start Game.cmd**. Original game files
are supplied separately. A new local console needs no kernel dump, EEPROM or
HDD image. Preserve your existing `data` folder and selected save storage when
updating. Matching project and dependency source and notices are included
locally under `source/`; private repository access is not needed.
