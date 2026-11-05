// The GPU through Any360 (any360::sys's graphics kernel, any360::gfx's command processor and Vulkan renderer) instead
// of the runtime's, behind the any360_gpu setting.
#include "any360_graphics.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <algorithm>
#include <string>

#include "any360/host/rexglue.h"

REXCVAR_DEFINE_BOOL(any360_gpu, false, "GPU",
                    "Run the GPU through Any360 (any360::sys + any360::gfx, its own window) instead of the runtime's");
REXCVAR_DEFINE_STRING(any360_gpu_device, "", "GPU", "Any360: a substring of the Vulkan device name to prefer");
REXCVAR_DEFINE_STRING(any360_gpu_pipeline_cache, "any360_pipelines.bin", "GPU",
                      "Any360: the file pipelines are kept in across runs (empty: none)");
REXCVAR_DEFINE_INT32(any360_gpu_msaa, 1, "GPU", "Any360: host MSAA, samples a pixel (1, 2, 4, 8)");
REXCVAR_DEFINE_BOOL(any360_gpu_async_pipelines, true, "GPU",
                    "Any360: pipelines compiled on threads of their own (a draw waits for none, so a scene seen the first "
                    "time shows its objects a moment late; off: the frame waits, a stutter instead)");
REXCVAR_DEFINE_INT32(any360_gpu_scale, 1, "GPU", "Any360: render targets at this many host pixels a guest pixel (1-4)");
REXCVAR_DEFINE_STRING(any360_gpu_capture, "", "GPU",
                      "Any360: a capture file for gfx_replay, recorded when <file>.go appears (empty: none)");
REXCVAR_DEFINE_INT32(any360_gpu_capture_kicks, 60, "GPU", "Any360: ring kicks a capture records");
REXCVAR_DEFINE_STRING(any360_gpu_profile, "", "GPU",
                      "Any360: a file for samples of every thread (where the time goes; empty: none)");
REXCVAR_DEFINE_INT32(any360_gpu_vblank_hz, 0, "GPU",
                     "Any360: vertical blank interrupts a second (0: 60 with vsync, 1000 without)");

// The GPU system calls, overridden in the executable: the runtime's until StartGraphics succeeds, Any360's after.
ANY360_REXGLUE_GRAPHICS_EXPORTS()

namespace tdu {

void StartAny360GraphicsIfAsked() {
  if (!REXCVAR_GET(any360_gpu)) return;
  any360::host::rexglue::GraphicsOptions options;
  options.gpu = REXCVAR_GET(any360_gpu_device);
  options.pipeline_cache_path = REXCVAR_GET(any360_gpu_pipeline_cache);
  options.async_pipelines = REXCVAR_GET(any360_gpu_async_pipelines);
  options.msaa_samples = uint32_t(std::clamp(REXCVAR_GET(any360_gpu_msaa), 1, 8));
  options.resolution_scale = uint32_t(std::clamp(REXCVAR_GET(any360_gpu_scale), 1, 4));
  options.vblank_hz = double(REXCVAR_GET(any360_gpu_vblank_hz));
  options.capture_path = REXCVAR_GET(any360_gpu_capture);
  options.capture_kicks = uint32_t(std::max(1, REXCVAR_GET(any360_gpu_capture_kicks)));
  options.profile_path = REXCVAR_GET(any360_gpu_profile);
  options.log = [](const std::string& line) { REXLOG_INFO("{}", line); };
  std::string error;
  if (any360::host::rexglue::StartGraphics(options, error)) {
    REXLOG_INFO("Any360: the GPU runs through any360::sys and any360::gfx");
  } else {
    REXLOG_ERROR("Any360: GPU binding failed, the runtime's GPU stays: {}", error);
  }
}

}  // namespace tdu
