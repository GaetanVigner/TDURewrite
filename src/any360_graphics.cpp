// The GPU through Any360 (any360::sys's graphics kernel, any360::gfx's command processor and Vulkan renderer) instead
// of the runtime's, behind the any360_gpu setting. Every option of any360::host::rexglue::GraphicsOptions is a setting
// (--name=value on the command line).
#include "any360_graphics.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <string>

#include "any360/host/rexglue.h"

REXCVAR_DEFINE_BOOL(any360_gpu, false, "GPU",
                    "Run the GPU through Any360 (any360::sys + any360::gfx) instead of the runtime's");
REXCVAR_DEFINE_STRING(any360_gpu_device, "", "GPU", "Any360: a substring of the Vulkan device name to prefer");
REXCVAR_DEFINE_INT32(any360_gpu_vblank_hz, 0, "GPU",
                     "Any360: vertical blank interrupts a second (0: 60 with vsync, 1000 without). TDU draws a frame "
                     "every 2: 120 for 60 FPS, 240 for 120 FPS");
REXCVAR_DEFINE_INT32(any360_gpu_scale, 1, "GPU", "Any360: render targets at this many host pixels a guest pixel (1-4)");
REXCVAR_DEFINE_INT32(any360_gpu_msaa, 1, "GPU", "Any360: host MSAA, samples a pixel (1, 2, 4, 8)");
REXCVAR_DEFINE_STRING(any360_gpu_pipeline_cache, "any360_pipelines.bin", "GPU",
                      "Any360: the file pipelines are kept in across runs (empty: none)");
REXCVAR_DEFINE_BOOL(any360_gpu_async_pipelines, true, "GPU",
                    "Any360: pipelines compiled on threads of their own (a draw waits for none, so a scene seen the first "
                    "time shows its objects a moment late; off: the frame waits, a stutter instead)");
REXCVAR_DEFINE_BOOL(any360_gpu_show_fps, false, "GPU",
                    "Any360: a frame rate counter over the frames (FPS, average and slowest frame, GPU time)");
REXCVAR_DEFINE_BOOL(any360_gpu_embed, true, "GPU",
                    "Any360: frames drawn inside the game's window (off: a window of their own)");
REXCVAR_DEFINE_BOOL(any360_gpu_async_renderer, true, "GPU",
                    "Any360: the renderer on a thread of its own (the command processor reads the stream meanwhile)");
REXCVAR_DEFINE_INT32(any360_gpu_record_threads, -1, "GPU",
                     "Any360: threads that record draws (-1: from the cores, 0: none, the renderer's thread does)");
REXCVAR_DEFINE_INT32(any360_gpu_spin, -1, "GPU",
                     "Any360: -1 the command processor and the renderer spin a moment when waiting, 0 they sleep at once");
REXCVAR_DEFINE_BOOL(any360_gpu_writes_after_draws, false, "GPU",
                    "Any360: memory writes and interrupts once the draws before them have run (off: as the runtime)");
REXCVAR_DEFINE_BOOL(any360_gpu_fences_after_draws, true, "GPU",
                    "Any360: EVENT_WRITE_SHD fences once the draws before them have run (as the console)");
REXCVAR_DEFINE_BOOL(any360_gpu_fences_when_captured, true, "GPU",
                    "Any360: a fence once the draws before it no longer read guest memory (off: once the GPU ran them)");
REXCVAR_DEFINE_BOOL(any360_gpu_late_resolve_reads, true, "GPU",
                    "Any360: textures built from per-level resolves (TDU's reflection maps) read the frame before "
                    "instead of waiting for the GPU (off: exact, ~1/3 slower in heavy scenes)");
REXCVAR_DEFINE_STRING(any360_gpu_profile, "", "GPU",
                      "Any360: a file for samples of every thread (where the time goes; empty: none). With any360_gpu "
                      "off, the runtime's GPU is sampled and its frame rate reported the same way, for comparison");
REXCVAR_DEFINE_STRING(any360_gpu_capture, "", "GPU",
                      "Any360: a capture file for gfx_replay, recorded when <file>.go appears (empty: none)");
REXCVAR_DEFINE_INT32(any360_gpu_capture_kicks, 60, "GPU", "Any360: ring kicks a capture records");

// The GPU system calls, overridden in the executable: the runtime's until StartGraphics succeeds, Any360's after.
ANY360_REXGLUE_GRAPHICS_EXPORTS()
// The game does not hook VdSwap itself, so it is bound here too (the runtime's swaps are counted through it when
// any360_gpu is off).
ANY360_REXGLUE_BIND_GRAPHICS(VdSwap)

namespace tdu {

void StartAny360GraphicsIfAsked() {
  const auto log = [](const std::string& line) { REXLOG_INFO("{}", line); };
  if (!REXCVAR_GET(any360_gpu)) {
    // The runtime's GPU, measured as the binding measures its own: the same profile and frame rate report.
    if (!REXCVAR_GET(any360_gpu_profile).empty()) {
      any360::host::rexglue::StartSampling(REXCVAR_GET(any360_gpu_profile));
      any360::host::rexglue::ReportRuntimeFrames(log);
    }
    return;
  }
  any360::host::rexglue::GraphicsOptions options;
  options.gpu = REXCVAR_GET(any360_gpu_device);
  options.vblank_hz = double(REXCVAR_GET(any360_gpu_vblank_hz));
  options.resolution_scale = uint32_t(std::clamp(REXCVAR_GET(any360_gpu_scale), 1, 4));
  options.msaa_samples = uint32_t(std::clamp(REXCVAR_GET(any360_gpu_msaa), 1, 8));
  options.pipeline_cache_path = REXCVAR_GET(any360_gpu_pipeline_cache);
  options.async_pipelines = REXCVAR_GET(any360_gpu_async_pipelines);
  options.show_fps = REXCVAR_GET(any360_gpu_show_fps);
  options.embed = REXCVAR_GET(any360_gpu_embed);
  options.async_renderer = REXCVAR_GET(any360_gpu_async_renderer);
  options.record_threads = REXCVAR_GET(any360_gpu_record_threads);
  options.spin = REXCVAR_GET(any360_gpu_spin);
  options.writes_after_draws = REXCVAR_GET(any360_gpu_writes_after_draws);
  options.fences_after_draws = REXCVAR_GET(any360_gpu_fences_after_draws);
  options.fences_when_captured = REXCVAR_GET(any360_gpu_fences_when_captured);
  options.late_resolve_reads = REXCVAR_GET(any360_gpu_late_resolve_reads);
  options.profile_path = REXCVAR_GET(any360_gpu_profile);
  options.capture_path = REXCVAR_GET(any360_gpu_capture);
  options.capture_kicks = uint32_t(std::max(1, REXCVAR_GET(any360_gpu_capture_kicks)));
  options.log = log;
  std::string error;
  if (any360::host::rexglue::StartGraphics(options, error)) {
    REXLOG_INFO("Any360: the GPU runs through any360::sys and any360::gfx");
  } else {
    REXLOG_ERROR("Any360: GPU binding failed, the runtime's GPU stays: {}", error);
  }
}

}  // namespace tdu
