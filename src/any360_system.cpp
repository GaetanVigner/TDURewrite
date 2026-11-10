// Every system call through any360::sys (github.com/GaetanVigner/Any360), each group behind the any360_sys_groups
// setting (comma-separated: stateless, files, objects, threads, memory, xam, input, audio; empty: the runtime
// answers them all). The settings, the mounts a console title expects and the audio hardware are Any360's
// (any360/host/settings.h); this file only defines the overrides, which must live in the executable.
#include "any360/host/settings.h"

// The four controller calls are overridden in input_lock.cpp (one at a time on the runtime, any360::sys's when the
// input group is on).
#define ANY360_REXGLUE_SKIP_XamInputGetState
#define ANY360_REXGLUE_SKIP_XamInputSetState
#define ANY360_REXGLUE_SKIP_XamInputGetCapabilities
#define ANY360_REXGLUE_SKIP_XamInputGetKeystrokeEx

// Every system call any360::sys provides: the runtime's unless its group is switched. The video calls are the
// graphics binding's (any360_graphics.cpp).
#include "any360/host/system_exports.inc"
