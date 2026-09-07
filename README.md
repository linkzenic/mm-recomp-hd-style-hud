# HD Style Controller HUD

This Recomp mod updates the in-game controller HUD for Zelda 64: Recompiled with a clean modern "HD Style" (a reference to TPHD and WWHD) button layout and refreshed action text.

The mod depends on Interface Helper and is not compatible with other controller HUD mods. Disable other controller mods before enabling this mod.

## Features

- HD style A/B/C button layout and glyphs suitable for modern controllers.
- Three action/attack layouts, including standard Nintendo placement.
- Updated action text font.
- RT64 texture replacements bundled in the release package.

## Installation

Download the latest `.nrm` from the GitHub Releases page and place it in your Zelda 64: Recompiled `mods` folder.

## Building

This repository contains the source mod code and RT64 texture package. To rebuild the code module, use the same Recomp mod toolchain used by Zelda 64: Recompiled.

The R-button cap is checked in as a generated asset. To regenerate it after editing its shape:

```sh
python3 -m pip install -r requirements-dev.txt
python3 tools/generate_r_button_cap.py
```

## Credits

This mod uses the public Interface Helper API pattern from:

- Interface Helper by tomtee

As well as modified code and inspiration from: 
- Modern Controller Overhaul by tomtee

This mod depends on Interface Helper at runtime.
