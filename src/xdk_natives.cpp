// The C runtime's copies and fills as host routines (Any360's native replacements of library functions,
// any360/host/xdk_natives.h) instead of recompiled loops of byte-swapped loads and stores: the three the game calls
// most (about 815, 805 and a few dozen call sites). Each keeps the function's own code for what is not plain memory
// or a plain forward copy. --any360_xdk_natives=false runs the functions' own code; --any360_xdk_verify=true runs
// both on every call and compares them (how these bindings were checked).
#include "any360/host/xdk_natives.h"

// memcpy: the long copy with prefetch; saves its destination at r1 - 8.
ANY360_XDK_BIND(82645BF0, CrtMemcpy)
// The plain forward copy: bytes to alignment, then words.
ANY360_XDK_BIND(82646010, CrtCopyForward)
// memset.
ANY360_XDK_BIND(826460C0, CrtMemset)
