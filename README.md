# Test Drive Unlimited Rewrite

<p align="center">
<img src="docs/intro.jpg" width="49%" alt="Test Drive Unlimited's airport intro, running recompiled on PC"/>
<img src="docs/title-any360.jpg" width="49%" alt="The title screen, drawn by Any360's Vulkan renderer"/>
</p>

**Test Drive Unlimited (Xbox 360, 2006), statically recompiled to native PC code and rewritten, step by step, onto [Any360](https://github.com/GaetanVigner/Any360).** It continues the TDU rewrite project as a recompilation of the Xbox 360 version with ReXGlue v0.10.0, then moves it off the emulation layers.

**No game data and no game binaries are here.** You need your own copy of the game (European disc, title 494707D4); the recompiled C++ is generated on your machine from your disc's `default.xex`.

## Where the rewrite stands

<!-- progress:begin -->
[![Test Drive Unlimited: rewrite progress](docs/progress/progress.svg)](docs/progress/PROGRESS.md)

| | Where it stands |
|---|---|
| **System calls** | 302 of 327 on `any360::sys` (opt-in) |
| **Graphics** | `any360::gfx`, a Vulkan renderer handling 283 of 323 Xbox 360 GPU features (opt-in) |
| **Host** | ReXGlue's loader, threads and window; Any360's own host is not started |
| **Game code** | 0 of 31,877 functions rewritten in C++ (0%) |

Updated 2026-11-11. Every system call, module and rewritten function: [PROGRESS.md](docs/progress/PROGRESS.md). What is missing, step by step: [ROADMAP.md](docs/progress/ROADMAP.md). Function by function: [functions.csv](docs/progress/functions.csv).
<!-- progress:end -->

## What this is

The game's PowerPC executable is translated ahead of time into C++ ([ReXGlue](https://github.com/rexglue/rexglue-sdk)) and compiled for the PC, so the game's own code runs directly on the processor. Under it, something has to stand in for the console: its system calls, its GPU, the libraries the game was linked with. In a recompilation port that is an emulation layer. Here it is replaced piece by piece by native code, and the game's own functions are then rewritten in C++, each proven against the original, until the result is an ordinary portable program.

| Step | What runs under the game | State here |
|---|---|---|
| Recompiled | ReXGlue's runtime: emulated kernel, emulated GPU | `default.xex` fully translated (45,102 functions; 53 the analyzer misses are declared in [config/](config/)); boots to the title, plays its intro videos and drives on the island, with sound and gamepad |
| System calls | `any360::sys`, native implementations of the Xbox 360 system calls | all eight groups, opt-in on `master`: `--any360_sys_groups=stateless,files,objects,threads,memory,xam,input,audio`; checked from boot through the intro videos to the title screen (no call reaches ReXGlue's kernel there); driving, saves and sound still to check in game |
| Graphics | `any360::gfx`, a Vulkan renderer fed by the game's own draw commands | opt-in on `master`: `--any360_gpu=true` (`--any360_gpu_scale=2 --any360_gpu_msaa=4`, `--any360_gpu_vblank_hz=240` for up to 120 FPS); 83-115 FPS driving at 2x with 4x MSAA on an RTX 5070 Ti, against 54-85 for the runtime's GPU at 2x |
| Library functions | `any360::xdk`, host versions of the XDK functions linked into the game | five memory functions (`memcpy`, its forward copy, `memmove`, `memset`, the vector copy), compared with the game's own code on every call in game (14 M calls past the title screen for the first three) |
| Game code | C++ rewritten function by function, checked against the recompiled original | not started (the count is in the table above) |

Every system call, module and function, and what is missing in order: [docs/progress/PROGRESS.md](docs/progress/PROGRESS.md), [docs/progress/ROADMAP.md](docs/progress/ROADMAP.md).

## Any360

[Any360](https://github.com/GaetanVigner/Any360) is the toolkit this rewrite is built on: a system layer, a renderer and a library of native functions written once for every Xbox 360 game, with the tools that measure how much of a game runs on them. A game is a test bench for it, not its target: what one game needed is there for the next, and each piece is checked in the games before it is trusted.

- **`any360::sys`**: the console's system calls (files, threads, memory, profile and saves, audio with a native XMA decoder, input), each tested against the reference runtime.
- **`any360::gfx`**: the Xbox 360 GPU's command stream drawn by a Vulkan renderer (resolution scale, host anti-aliasing, frame rates above the console's).
- **`any360::xdk`**: the functions games link from the XDK (the C runtime's `memcpy`, `memmove`, `memset`, the vector copy), each with a reference model, unit tests and a benchmark, and compared with the game's own code on every call in game before it is bound.

The games rewritten on it, each with the same report of where it stands:

- [Crackdown](https://github.com/GaetanVigner/CrackdownRewrite)
- [The Simpsons Game](https://github.com/GaetanVigner/TheSimpsonsGameRewrite)
- [Test Drive Unlimited](https://github.com/GaetanVigner/TDURewrite) (this repository)

How it is designed and how far it is: [the project page](https://github.com/GaetanVigner/Any360), [its progress](https://github.com/GaetanVigner/Any360/blob/main/docs/progress.svg), [the rule native functions follow](https://github.com/GaetanVigner/Any360/blob/main/docs/NATIVE_FUNCTIONS.md).

## Running it on Any360

Both bindings are opt-in:

```
tdu.exe --any360_gpu=true --any360_sys_groups=stateless,files,objects,threads,memory,xam,input,audio
```

`--any360_gpu=true` draws the whole game through `any360::gfx` (`--any360_gpu_scale=2 --any360_gpu_msaa=4`; `--any360_gpu_vblank_hz=240` for frame rates above 60); `--any360_sys_groups` puts the Xbox system calls on `any360::sys` (checked to the title screen; driving, saves and sound still to check). The library functions run on `any360::xdk` by default (`--any360_xdk_natives=false` runs the game's own; `--any360_xdk_verify=true` runs both on every call and compares them).

## Building

Windows, Visual Studio 2022 with the C++ and Clang tools, an install of the ReXGlue SDK v0.10.0 (`-Sdk`, the folder with `bin\rexglue.exe`, `include\` and `lib\`) and an [Any360](https://github.com/GaetanVigner/Any360) checkout (`-Any360`).

1. Extract your disc into `game\` (for example with [extract-xiso](https://github.com/XboxDev/extract-xiso)), so that `game\default.xex` exists.
2. `powershell -File codegen.ps1 -Sdk <sdk>` generates the C++ into `generated\default\`.
3. `powershell -File build.ps1 -Sdk <sdk> -Any360 <any360>` builds `out\build\tdu.exe`.
4. Run `out\build\tdu.exe`. It finds `game\` next to itself or in the repository; logs are written to `out\build\logs\`.

Code reached only through vtables or data pointers is invisible to the codegen: [tools/find-missing-functions.py](tools/find-missing-functions.py) scans the decrypted image for such targets and writes them to [config/discovered-functions.toml](config/discovered-functions.toml). The project's first page, with why it was started, is kept in [docs/LEGACY_README.md](docs/LEGACY_README.md).

## Credits

- Test Drive Unlimited by Eden Games, published by Atari (2006).
- The TDU rewrite project's earlier authors (Grien25, CRACKbomber, testdriveupgrade) and [XenonRecompUnlimited](https://github.com/testdriveupgrade/XenonRecompUnlimited).
- [ReXGlue SDK](https://github.com/rexglue/rexglue-sdk), the recompiler and runtime (BSD 3-Clause).
- [Any360](https://github.com/GaetanVigner/Any360) for the system layer, the renderer and the native library functions.

## Legal and licence

An unofficial fan project, not affiliated with or endorsed by Atari or Eden Games. It is not a replacement for the game: you still need to own a copy, and nothing of the game is in this repository.

AI tools (Claude Code) are used for code analysis, development, testing and documentation of this rewrite; changes are directed and tested by the maintainer.
