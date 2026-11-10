// The controller calls, one at a time. The runtime's input system refreshes its device list (a std::vector it clears
// and refills) inside each of them without a lock; Test Drive Unlimited reads the pads on one thread while it drives
// the rumble on another, and the two refreshes freed the same list twice: heap corruption that crashed the game much
// later (found with the AddressSanitizer build, build.ps1 -Asan). Each override takes one lock and calls the runtime's
// own implementation, or any360::sys's when the input group is switched to it (any360_sys_groups).
#include <mutex>

#include "any360/host/rexglue.h"
#include "any360/host/system.h"

namespace {

std::mutex g_input;

void CallLocked(const char* name, any360::host::rexglue::GuestFunction any360,
                any360::host::rexglue::GuestFunction runtime, PPCContext& ctx, uint8_t* base) {
  std::lock_guard lock(g_input);
  any360::host::rexglue::CallSystemExport(any360::host::rexglue::SysGroup::kInput, name, any360, runtime, ctx, base);
}

}  // namespace

#define TDU_INPUT_LOCKED(name)                                                                \
  extern "C" void any360_##name(PPCContext& ctx, uint8_t* base);                              \
  extern "C" void __imp__##name(PPCContext& __restrict ctx, uint8_t* base) {                  \
    static const auto runtime = any360::host::rexglue::ReferenceExport("__imp__" #name);      \
    CallLocked(#name, &any360_##name, runtime, ctx, base);                                    \
  }

TDU_INPUT_LOCKED(XamInputGetState)
TDU_INPUT_LOCKED(XamInputSetState)
TDU_INPUT_LOCKED(XamInputGetCapabilities)
TDU_INPUT_LOCKED(XamInputGetKeystrokeEx)
