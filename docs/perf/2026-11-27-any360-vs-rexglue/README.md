# Any360 against ReXGlue, uncapped (2026-11-27)

Test Drive Unlimited, same build (TDURewrite 7d62a9e + the screenshot option, Any360 2f9a9d1), same route, frame
rate uncapped on both sides, 5 alternating launches a variant. Ryzen 7 7800X3D (all 16 logical processors: the runtime resets the launch's affinity mask at startup), RTX 5070 Ti,
1280x720 (scale 1, no MSAA on both).

| | Average FPS | Median period | 10% low | 1% low | Private memory |
|---|---:|---:|---:|---:|---:|
| **Any360** | **115.0** | **115.6** | **110.3** | **101.2** | **3.3 GB** (flat) |
| ReXGlue | 71.7 | 77.8 | 54.2 | 34.1 | 17.2 GB mean, 19.5 GB max (growing ~4 GB a minute) |

Rank test, Any360 against ReXGlue: steady FPS z +8.34, 10% low z +8.29, 1% low z +8.33 (35 and 75 periods).

## Variants

- Any360: `--any360_gpu=true --any360_sys_groups=stateless,files,objects,threads,memory,xam,input,audio
  --any360_gpu_vblank_hz=1000 --vsync=false`
- ReXGlue: `--any360_sys_groups=input --any360_xdk_natives=false --vsync=false` (the runtime's own GPU and kernel; the
  `input` group only so the input script drives it)
- Both: `--any360_input_script=15:start,20:start,25:start,30:start,100:a,160:b,166:b,175:rt/90000` (intro skipped,
  "Continuer partie", the house menu left, the throttle held: the car drives off the drive into the trees and stays
  against one, the island streaming around it). Both reach driving about 165 s after start.

## Caps

- TDU waits 2 vertical blanks a frame. Any360: 1000 blanks a second asked; they arrive about every 2 ms, so the menus
  stop at 240 FPS; driving runs at 104-121, far below. ReXGlue with `--vsync=false` gives blanks every ms: 500 FPS in
  the menus, 38-111 driving. Neither sits on a cap in the measured window.

## Method

- `launch.ps1` (machine lock, a mask of cores 0-7 that the runtime resets to all 16 at startup, each launch killed after 262 s), `runs.txt` (variant and log per launch),
  `logs/` (the full logs).
- `python -I tools/frame_lows.py runs.txt logs --after 185` (Any360 81b3a4b or later): the periods logged 185 s or
  more after the log's first line (driving), Any360's diagnostics-off lines and the runtime GPU's VdSwap lines.
  Output: `frame_lows.txt`.
- ReXGlue's private memory grows during the whole launch (1.2 GB at the menus, 18-19.5 GB at 260 s); its frame times
  get worse with it.
