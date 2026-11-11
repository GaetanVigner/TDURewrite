// The C runtime's copies and fills and the XDK's vector copy as host routines (Any360's native replacements of
// library functions, any360/host/xdk_natives.h) instead of recompiled loops of byte-swapped loads and stores. Each
// keeps the function's own code for the calls it does not take (memory that is not plain, overlaps its code treats
// its own way). --any360_xdk_natives=false runs the functions' own code; --any360_xdk_verify=true runs
// both on every call and compares them (how these bindings were checked).
#include "any360/host/xdk_natives.h"

// memcpy: the long copy with prefetch; saves its destination at r1 - 8.
ANY360_XDK_BIND(82645BF0, CrtMemcpy)
// The plain forward copy: bytes to alignment, then words.
ANY360_XDK_BIND(82646010, CrtCopyForward)
// memset.
ANY360_XDK_BIND(826460C0, CrtMemset)
// memmove: memcpy below the source, a backward copy above it.
ANY360_XDK_BIND(82644930, CrtMemmove)
// The XDK's vector copy (16-byte vectors, cache lines zeroed ahead); 0x82BA94A0 is the global its byte loop
// subtracts from the count (1).
ANY360_XDK_BIND_WITH(82696950, XMemCpy, 0x82BA94A0)
// The C runtime's string functions. wcsncpy (sub_82644158) has the same replacement in Any360 and is not bound: no
// run of the game has called it with both sides compared.
ANY360_XDK_BIND(826431B0, CrtWcslen)
ANY360_XDK_BIND(82641450, CrtStrchr)
ANY360_XDK_BIND(82640DE0, CrtStrrchr)
ANY360_XDK_BIND(8263FF40, CrtStrncmp)
ANY360_XDK_BIND(826401C8, CrtStrncpy)
ANY360_XDK_BIND(82640228, CrtStrncat)
ANY360_XDK_BIND(82640DC0, CrtWcscpy)
ANY360_XDK_BIND(82640938, CrtTolower)
ANY360_XDK_BIND(82644140, CrtToupper)
