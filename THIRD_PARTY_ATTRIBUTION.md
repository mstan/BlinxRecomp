# Attribution

The shared implementation and its complete donor inventory are pinned through
the [xboxllr submodule](xboxlle/THIRD_PARTY_ATTRIBUTION.md). Its exact revision
is recorded by the gitlink and the matching `[framework]` entry in
[game.toml](game.toml), rather than by a separately maintained copy here.

This includes Phobos's CPU translator and private renderer references, sp00nznet
input mapping, native audio/DSP/ADPCM donors, Unicorn, and the xemu/Cxbx/Theseus
behavioral references. This repository adds game configuration and build/launch
glue; it does not copy additional donor implementation bodies. The framework
build stages the retained notices alongside the game executable.

Revision metadata records addresses/hashes for the owner's original NTSC image.
No original executable bodies, movies, audio or disc images are tracked.
The separately credited launcher cover and a curated documentation screenshot
are the image exceptions; see [Credits](CREDITS.md) and their linked notices.
See [LICENSE-SCOPE.md](LICENSE-SCOPE.md) for the current reference-build scope.
