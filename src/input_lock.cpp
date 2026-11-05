// The controller calls, one at a time. The runtime's input system refreshes its device list (a std::vector it clears
// and refills) inside each of them without a lock; Test Drive Unlimited reads the pads on one thread while it drives
// the rumble on another, and the two refreshes freed the same list twice: heap corruption that crashed the game much
// later (found with the AddressSanitizer build, build.ps1 -Asan). Each override takes one lock and calls the runtime's
// own implementation.
#include <mutex>

#include "any360/host/rexglue.h"

namespace {

std::mutex g_input;

void CallLocked(any360::host::rexglue::GuestFunction runtime, PPCContext& ctx, uint8_t* base) {
  std::lock_guard lock(g_input);
  runtime(ctx, base);
}

}  // namespace

#define TDU_INPUT_LOCKED(name)                                                                \
  extern "C" void __imp__##name(PPCContext& __restrict ctx, uint8_t* base) {                  \
    static const auto runtime = any360::host::rexglue::ReferenceExport("__imp__" #name);      \
    CallLocked(runtime, ctx, base);                                                           \
  }

TDU_INPUT_LOCKED(XamInputGetState)
TDU_INPUT_LOCKED(XamInputSetState)
TDU_INPUT_LOCKED(XamInputGetCapabilities)
TDU_INPUT_LOCKED(XamInputGetKeystrokeEx)
