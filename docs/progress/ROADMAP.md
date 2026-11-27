# Test Drive Unlimited: roadmap

Generated on 2026-11-28 with [PROGRESS.md](PROGRESS.md). The rewrite is done when nothing of the Xbox 360 or of ReXGlue's runtime is left under the game: every system call on `any360::sys`, graphics on `any360::gfx`, Any360 as the host, and the game's code as portable C++. Steps are in working order; each lists what it still needs.

## 1. System calls on any360::sys

303 of 327 imports run on `any360::sys`; 24 do not yet.

| Group | Calls to switch | Of which Any360 already has | Missing in Any360 |
|---|---|---|---|
| xam | 24 | 0 | 24 |

<details><summary>xam: 24 calls</summary>

`NetDll_XnpCapture` (stub) `NetDll_XnpConfig` (stub) `NetDll_XnpConfigUPnP` (stub) `NetDll_XnpEthernetInterceptRecv` (stub) `NetDll_XnpEthernetInterceptSetCallbacks` (stub) `NetDll_XnpEthernetInterceptXmit` (stub) `NetDll_XnpGetActiveSocketList` (stub) `NetDll_XnpGetConfigStatus` (stub) `NetDll_XnpGetVlanXboxName` (stub) `NetDll_XnpLoadConfigParams` (stub) `NetDll_XnpLoadMachineAccount` (stub) `NetDll_XnpLogonClearQEvent` (stub) `NetDll_XnpLogonGetQFlags` (stub) `NetDll_XnpLogonGetQVals` (stub) `NetDll_XnpLogonSetPState` (stub) `NetDll_XnpLogonSetQEvent` (stub) `NetDll_XnpLogonSetQFlags` (stub) `NetDll_XnpLogonSetQVals` (stub) `NetDll_XnpNoteSystemTime` (stub) `NetDll_XnpRegisterKeyForCallerType` (stub) `NetDll_XnpSaveConfigParams` (stub) `NetDll_XnpSaveMachineAccount` (stub) `NetDll_XnpSetVlanXboxName` (stub) `NetDll_XnpUnregisterKeyForCallerType` (stub)

</details>

## 2. Graphics on any360::gfx

The title draws through `any360::gfx`: opt-in on `master`: `--any360_gpu=true` (`--any360_gpu_scale=2 --any360_gpu_msaa=4`; `--any360_gpu_vblank_hz=1000` lifts the 30 FPS lock); uncapped, driving at scale 1 on an RTX 5070 Ti: 115 FPS (1% low 101), 3.3 GB private, against 72 FPS (1% low 34) and 17-19 GB growing for the runtime's GPU (2026-11-27, 5 launches each).

Still to do: make it the default renderer (today ReXGlue's Xenos emulation is), then drop the emulated GPU from the build.

- A rare resolve into a format the renderer refuses (host format 37 to guest format 4).
- `any360_gpu_fences_after_draws=false` is faster but breaks this title's display: find what it waits on.

GPU features `any360::gfx` does not handle yet (40 of 323; none stops this title today, each is one a title may need):

| Family | Features |
|---|---|
| command processor | MEM_WRITE_CNTR, COND_EXEC, EVENT_WRITE_CFL, DRAW_INDX_2_BIN, SET_STATE, SET_BIN_BASE_OFFSET, IM_STORE |
| primitive types | kTriangleWithWFlags, kExplicitMajorModeForceStart, k2DCopyRectListV1, k2DCopyRectListV2, k2DCopyRectListV3, k2DFillRectList, k2DLineStrip, k2DTriStrip |
| render features | tessellation |
| texture formats | k_1_REVERSE, k_1, k_8_B, k_16_16_EDRAM, k_8_8_8_8_A, k_16_16_16_16_EDRAM, k_32, k_32_32, k_32_32_32_32, k_32_AS_8, k_32_AS_8_8, k_16_MPEG, k_16_16_MPEG, k_8_INTERLACED, k_32_AS_8_INTERLACED, k_32_AS_8_8_INTERLACED, k_16_INTERLACED, k_16_MPEG_INTERLACED, k_16_16_MPEG_INTERLACED, k_32_32_32_FLOAT, k_DXT3A_AS_1_1_1_1, k_8_8_8_8_GAMMA_EDRAM, k_2_10_10_10_FLOAT_EDRAM |
| vertex formats | kUndefined |

## 3. Any360 as the host

Shared by every title (Any360's own roadmap); the title follows once they land.

| Any360 step | What | State |
|---|---|---|
| 11 | third and fourth titles (Test Drive Unlimited, Sonic 06) | partial: Test Drive Unlimited boots to its title screen with all eight groups on `any360::sys` and no call reaching the runtime; gameplay, saves and sound not checked yet; Sonic 06 not started |
| 12 | paths no title took yet: `XMA*` decoding, STFS packages, several users, guide UIs, Live offline, xbdm | partial: 850 of 2,914 exports native with scenario tests; the reference stubs sorted ([docs/SYS_STUBS.md](https://github.com/GaetanVigner/Any360/blob/main/docs/SYS_STUBS.md)): none left to implement, all 114 Xenia stubs too answered natively (avatars, media, profile and user, content, information, Kinect, settings and others, guide screens, modules, input, network and notifications, the SMC and the last ones) (2 object and module calls, 10 content, network, callback and thread look-up calls, 9 language, memory, Xapi, IPTV and LZX decompression calls, 24 cryptography and key calls, 14 Win32 forms, 19 XAM information calls, 11 kernel calls, 16 guide and Kinect calls and 17 profile calls done), 255 debug only, 1,799 never seen |
| 13 | Any360's own host: XEX loader, address space, threads, guest dispatch, MMIO, devices | not started |
| 14 | the platform layer: window, input, audio, presenter on Windows and Linux (Steam Deck) | partial: SDL3 audio output only |
| 15 | graphics through `any360::gfx` only, no `rexgpu-xenos` | partial: three titles draw through `any360::gfx`, the GPU session's plan in [docs/GFX.md](https://github.com/GaetanVigner/Any360/blob/main/docs/GFX.md) |
| 16 | porting a new title: a converter served from GitHub (as AnyPS5) and built into the cross-platform launcher, from a XEX to a running build with no title-specific code in Any360 | not started |

## 4. Library and middleware code

SDK code linked into the executable is not decompiled: Any360 provides the same service natively and the whole range goes.

| Library range | Functions | Instructions | Replaced | Verified in the sandbox | Left |
|---|---|---|---|---|---|
| XDK audio (XAudio, XMA) | 1,388 | 53,897 | 6 | 0 | 1,382 |
| XDK Xbox Live and voice (XOnline, XHV) | 956 | 49,754 | 0 | 0 | 956 |
| XDK D3D runtime | 1,436 | 46,565 | 4 | 0 | 1,432 |
| C runtime (CRT) | 762 | 45,359 | 23 | 0 | 739 |
| XAPI (Win32 layer over the kernel and XAM) | 324 | 11,057 | 1 | 0 | 323 |

Third-party middleware the game links (physics, AI, scripting, video and the like) is neither the game's own code nor something Any360 provides. It is not rewritten function by function: it stays recompiled, or is replaced by the vendor's library for the target platform (or an open implementation) once the game's code around it is portable.

| Middleware | Functions | Instructions |
|---|---|---|
| Havok Physics | 8,156 | 284,969 |
| Bink video | 205 | 19,653 |

## 5. Game code as portable C++

31,875 of 31,875 game functions are still recompiled PowerPC. The list, function by function, is [functions.csv](functions.csv) (state `recompiled` or `verified`). By module, largest first:

| Module | Functions left | Instructions left | Rewritten | Verified |
|---|---|---|---|---|
| code 824xxxxx | 9,336 | 258,686 | 0 | 0 |
| code 829xxxxx | 3,669 | 259,575 | 0 | 0 |
| code 82Axxxxx | 3,557 | 259,803 | 0 | 0 |
| code 828xxxxx | 3,084 | 259,750 | 0 | 0 |
| code 82Bxxxxx | 2,983 | 127,646 | 0 | 0 |
| code 823xxxxx | 2,895 | 31,620 | 0 | 0 |
| code 826xxxxx | 2,318 | 103,367 | 0 | 0 |
| code 827xxxxx | 2,191 | 125,469 | 0 | 0 |
| code 825xxxxx | 1,842 | 62,650 | 0 | 0 |

Order of work: symbol recovery and library recognition first (they shrink this list), then leaf functions (no calls: each is checked alone), then their callers, module by module; every rewrite is bound over the recompiled function and proven equal in the guest sandbox before it replaces it (Any360's `pipeline/any360.py <title> select | decompile | bind | verify`).

## 6. This title

- Give `XamUserCreateStatsEnumerator` a real enumerator (or the title's error path) so `XamEnumerate` and `NtClose` stop reaching the runtime with a null handle; then check sound, the gamepad's rumble and a long session in game.
- Name what is left of the executable's library code: the ranges listed as not named in Any360's titles/tdu/modules.json (code shared with the other titles of unknown purpose, an XML parser, zlib, a JPEG path), then the game's own modules (Eden's ed* libraries carry their names in their strings).
- More native replacements of library functions (the CRT's string routines next), each under the rule of Any360's docs/NATIVE_FUNCTIONS.md: a reference model, unit tests, a benchmark, then `--any360_xdk_verify=true` and `--any360_xdk_bench=true` in game.
- A golden test target (guest sandbox) so rewritten game functions can be verified.

## Known problems

- A game thread spins at 90% of a core (`sub_826737B8`).
- Frame captures only trigger with `--any360_gpu_vblank_hz=1000` (the command processor is never idle at a kick otherwise).
- The intro videos once ran too fast above 60 Hz; not seen again since the video pacing fix.
- With `--any360_gpu_vblank_hz=1000` the vertical blanks arrive about every 2 ms: the menus stop at 240 FPS (the runtime's GPU, `--vsync=false`, reaches 500); driving runs far below it.
- The runtime's GPU (`--any360_gpu=false`) uncapped grows by ~4 GB of private memory a minute (18 GB after 4 minutes); Any360's GPU stays flat.
