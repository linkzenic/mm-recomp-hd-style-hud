# HD Style Controller HUD

HD Style Controller HUD updates the in-game controller HUD for Zelda 64: Recompiled with a cleaner modern button layout, white button glyphs, and refreshed action text.

The mod depends on Interface Helper and is not compatible with other controller HUD mods. Disable other controller mods before enabling this mod.

## Features

- HD-style A/B/C button layout.
- Optional A/B layout swap.
- White controller button glyphs.
- Updated action text for common A-button and B-button prompts.
- RT64 texture replacements bundled in the release package.

## Installation

Download the latest `.nrm` from the GitHub Releases page and place it in your Zelda 64: Recompiled `mods` folder.

## Building

This repository contains the source mod code and RT64 texture package. To rebuild the code module, use the same Recomp mod toolchain used by Zelda 64: Recompiled.

The Makefile expects Majora's Mask decomp headers at `mm-decomp/` by default. You can also point it at an existing checkout:

```sh
make \
  CC=/opt/homebrew/opt/llvm/bin/clang \
  LD=/opt/homebrew/bin/ld.lld \
  MM_DECOMP_DIR=/path/to/mm-decomp
```

Then run `RecompModTool` against `mod.toml` and combine the generated `mod.json`, `mod_binary.bin`, and `mod_syms.bin` with the contents of `package/`.

## Credits

This mod uses the public Interface Helper API pattern from:

- Interface Helper by tomtee
- Modern Controller Overhaul by tomtee

Those projects are distributed under CC0 1.0 Universal. This mod depends on Interface Helper at runtime rather than modifying Zelda 64: Recompiled itself.
