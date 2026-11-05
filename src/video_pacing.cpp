// Bink videos at their own speed with the frame rate unlocked. TDU's video player advances one video frame a presented
// frame, and presents on vertical blanks: with Any360's vertical blanks faster than 60 a second (any360_gpu_vblank_hz,
// what lifts the game's 30 FPS cap), the intro video played that much faster. While the player handles frames (its
// Bink wrapper, hooked in config/overrides.toml), the blanks go back to 60 a second; 300 ms after the last one, back
// to the configured rate.
#include <atomic>
#include <chrono>
#include <thread>

#include <rex/cvar.h>

#include "any360/host/rexglue.h"

REXCVAR_DECLARE(int32_t, any360_gpu_vblank_hz);

namespace {

using Clock = std::chrono::steady_clock;
std::atomic<int64_t> g_last_frame{0};  // Clock ticks of the last video frame
std::atomic<bool> g_playing{false};

void Watch() {
  for (;;) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    if (!g_playing.load()) continue;
    const int64_t since = Clock::now().time_since_epoch().count() - g_last_frame.load();
    if (since > std::chrono::duration_cast<Clock::duration>(std::chrono::milliseconds(300)).count()) {
      g_playing = false;
      any360::host::rexglue::SetVblankHz(0);  // the configured rate
    }
  }
}

}  // namespace

// The midasm hook: a video frame.
void tdu_video_frame() {
  if (REXCVAR_GET(any360_gpu_vblank_hz) <= 60) return;  // nothing unlocked: videos already at their speed
  g_last_frame = Clock::now().time_since_epoch().count();
  if (g_playing.exchange(true)) return;
  static std::once_flag watcher;
  std::call_once(watcher, [] { std::thread(Watch).detach(); });
  any360::host::rexglue::SetVblankHz(60.0);
}
