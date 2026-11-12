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
ANY360_XDK_BIND(827834F0, CrtStricmp)
ANY360_XDK_BIND(82644078, CrtStrstr)
// The C runtime's helpers on a double's bits: _get_exp and _set_exp (each is in the image twice). _finite
// (sub_82654358), _isnan (sub_82654378), _sptype (sub_8264C998) and _add_exp (sub_8264C938) have their replacements
// in Any360 too and are not bound: no run of the game has called them with both sides compared.
ANY360_XDK_BIND(8264C920, CrtGetExp)
ANY360_XDK_BIND(82646788, CrtGetExp)
ANY360_XDK_BIND(826467A0, CrtSetExp)
ANY360_XDK_BIND(8264C8F8, CrtSetExp)
// wcscmp (sub_82783FC8) and mbtowc (sub_8265D078) have their replacements in Any360 too and are not bound: no run
// of the game has called them with both sides compared. The strlen and strcmp built as loops of bytes
// (sub_82545950, sub_82545700) are Havok's own, inside its code: middleware stays as it is, they are not bound.
// XAPI's GetLastError (sub_82697068) and SetLastError (sub_82697018) have their replacements in Any360, checked here
// with both sides compared (171,899 and 171,972 calls, none different), and are not bound: five instructions each,
// they gain nothing as host routines (--any360_xdk_bench: 1.0x and 0.9x).
// The D3D runtime's GPU wait step (8 db16cyc pauses, then the device's stop flag, the read pointer and the hang
// check): its eight callers loop on it while the GPU has not passed a ring position or written a fence. It sleeps
// until the GPU moves on instead of spinning (any360_d3d_wait_sleep).
ANY360_XDK_BIND(826737B8, D3dGpuWaitStep)
// The D3D runtime's upload of the shader constants a mask marks as changed into the command buffer; this runtime
// keeps the buffer's write pointer at device + 40. XAudio's mix with a volume ramp (sub_826B50B8) has its
// replacement in Any360 too and is not bound: this build's recompiled vmaddfp rounds twice (a multiply, then an add)
// where the replacement rounds once as the console does, and the game barely calls it.
ANY360_XDK_BIND_WITH(8268DEF0, D3dDirtyConstants, 40)
// The same upload for the constants of 4 bytes a bit.
ANY360_XDK_BIND_WITH(8268DB50, D3dDirtyWords, 40)
// The D3D runtime's copy of the title's shader constants into the device.
ANY360_XDK_BIND(8266B808, D3dSetShaderConstants)
