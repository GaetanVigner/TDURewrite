# Test Drive Unlimited Rewrite

<p align="center">
<img src="docs/intro.jpg" width="49%" alt="Test Drive Unlimited's airport intro, running recompiled on PC"/>
<img src="docs/title-any360.jpg" width="49%" alt="The title screen drawn by Any360's Vulkan renderer"/>
</p>

<!-- progress:begin -->
[![Test Drive Unlimited: rewrite progress](docs/progress/progress.svg)](docs/progress/PROGRESS.md)

| | Where it stands |
|---|---|
| **System calls** | 302 of 327 on `any360::sys` (opt-in) |
| **Graphics** | `any360::gfx`, a Vulkan renderer handling 283 of 323 Xbox 360 GPU features (opt-in) |
| **Host** | ReXGlue's loader, threads and window; Any360's own host is not started |
| **Game code** | 0 of 31,877 functions rewritten in C++ (0%) |

Updated 2026-11-10. Every system call, module and rewritten function: [PROGRESS.md](docs/progress/PROGRESS.md). What is missing, step by step: [ROADMAP.md](docs/progress/ROADMAP.md). Function by function: [functions.csv](docs/progress/functions.csv).
<!-- progress:end -->

**Test Drive Unlimited (Xbox 360, 2006), statically recompiled to native PC code.** The game's PowerPC executable is
translated to C++ with [ReXGlue](https://github.com/rexglue/rexglue-sdk) and built as a Windows program; the Xbox 360
system calls and GPU are provided by the ReXGlue runtime today and, step by step, by
[Any360](https://github.com/GaetanVigner/Any360), whose goal is a portable native codebase with no emulation layer left.
The picture above is that road: green is what already runs on the project's own code.

**This repository holds no game data and no game code.** You need your own copy of the game (European disc, title
494707D4); the recompiled C++ is generated on your machine from your disc's `default.xex`.

## Status

| | |
|---|---|
| Recompilation | `default.xex` fully translated (ReXGlue v0.10.0): 45,102 functions; 53 the analyzer misses are declared in [config/](config/) |
| Playable | boots to the title, plays the intro videos at their own speed and drives on the island, with sound and gamepad, on the ReXGlue runtime's GPU (Direct3D 12) or on Any360's |
| Graphics on Any360 | `--any360_gpu=true` draws the whole game through `any360::gfx` (Vulkan): resolution scale (`--any360_gpu_scale=2`), anti-aliasing (`--any360_gpu_msaa=4`), frame rates above 60 (`--any360_gpu_vblank_hz=240` gives up to 120 FPS). Driving at 2x with 4x MSAA: 83-115 FPS on an RTX 5070 Ti, against 54-85 for the runtime's GPU at 2x. Every Any360 option is a setting |
| System calls on Any360 | `--any360_sys_groups=stateless,files,objects,threads,memory,xam,input,audio` puts the Xbox system calls on `any360::sys` (files, threads, memory, profile, a native XMA decoder, input), as in [Crackdown](https://github.com/GaetanVigner/CrackdownRewrite) and [The Simpsons Game](https://github.com/GaetanVigner/TheSimpsonsGameRewrite). Checked from boot through the intro videos to the title screen, where no call reaches the ReXGlue kernel any more; driving, saves and sound are still to check. `--any360_sys_trace=true` logs the first call of each kind and any that still goes to the runtime |
| Game code | recompiled PowerPC, except the C runtime's `memcpy`, plain forward copy and `memset`, which run as host routines (Any360's native replacements of library functions; `--any360_xdk_verify=true` runs both on every call and compares them: 9.3 million calls, none different). Of the 45,102 functions, 4,864 are XDK library code (D3D runtime, audio, Xbox Live, CRT, XAPI) that Any360 replaces as units and 8,361 are middleware (Havok Physics, Bink), which leaves 31,877 of the game's own |

What is missing, in order, with the calls and functions each step needs: [docs/progress/ROADMAP.md](docs/progress/ROADMAP.md).

## Building (Windows)

Needs Visual Studio 2022 with the C++ and Clang tools, and an install of the ReXGlue SDK v0.10.0 (`-Sdk`, the folder with
`bin\rexglue.exe`, `include\` and `lib\`).

1. Extract your disc into `game\` (for example with [extract-xiso](https://github.com/XboxDev/extract-xiso)), so that
   `game\default.xex` exists. A junction to a folder elsewhere works too.
2. `powershell -File codegen.ps1 -Sdk <sdk>` generates the C++ into `generated\default\`.
3. `powershell -File build.ps1 -Sdk <sdk>` builds `out\build\tdu.exe`.
4. Run `out\build\tdu.exe`. It finds `game\` next to itself or in the repository; `--game_data_root <dir>` points
   elsewhere. Logs are written to `out\build\logs\`.

## Finding missing functions

Code reached only through vtables or data pointers is invisible to the codegen's analysis; calling it ends in
`Call to invalid or unregistered function`. [tools/find-missing-functions.py](tools/find-missing-functions.py) scans
the decrypted image for such targets and writes the confident ones to
[config/discovered-functions.toml](config/discovered-functions.toml) (`--apply-high`); the image comes from
Crackdown's `dump_image` tool (`dump_image game default.xex out/image/default_image.bin`). Hand-found ones go in
[config/overrides.toml](config/overrides.toml).

## Why
I have a wish to return the amazing moments, when we was young.
When I was a child it was one of my favourite games, but when I hadn't been enough competent in PC, due to I hadn't played online mode,
but when I remembered about this game, it was to late, servers was closed...
I think, what I'm not alone with this wish...
Now I'm a programmer (in specialities of C++ 3D graphics and nets) and I've no through out this idea out of my mind. 
Then, I'll try to close this goal.

## Advantages of this project:
1)We can understand how works online mode and remake servers for PC and XBox 360

2)Easier and better modding with understanding of source code

3)Port of this game to Mac and Linux (later)

4)Possibility to create global mods for PC and XBox verison 

5)360-version have some new cars, which we are easier to convert to PC-version (and add new cars)

6)360 Graphics sometimes a bit better, due to special shaders (like mw 360 vs PC version)

7)Sequel maybe modified (at this moment fan-made online is not exist on XBOX 360 version, only on PC and PS3 versions) 

In conclusion, we will add 4K, RTX, DLSS and etc. cool modern things and solve the problems with online, traffic AI and CPU-usage

**This project is not a replacement for the game (you still need to own a legal copy of the game); and does not provide any game assets!**
**You should dump your DVD and prepare for this port**

XenonRecompUnlimited - it's special fork of XenonRecomp for TDU engine, like experimental fork-version with fresh updates
 (unfortunattely at this moment XenonRecomp updates long-term :( ) https://github.com/testdriveupgrade/XenonRecompUnlimited

