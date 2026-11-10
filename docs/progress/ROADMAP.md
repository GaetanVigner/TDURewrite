# Test Drive Unlimited: roadmap

Generated on 2026-11-10 with [PROGRESS.md](PROGRESS.md). The rewrite is done when nothing of the Xbox 360 or of ReXGlue's runtime is left under the game: every system call on `any360::sys`, graphics on `any360::gfx`, Any360 as the host, and the game's code as portable C++. Steps are in working order; each lists what it still needs.

## 1. System calls on any360::sys

23 of 327 imports run on `any360::sys`; 304 do not yet.

| Group | Calls to switch | Of which Any360 already has | Missing in Any360 |
|---|---|---|---|
| stateless | 33 | 33 | 0 |
| files | 15 | 15 | 0 |
| objects | 27 | 27 | 0 |
| threads | 35 | 35 | 0 |
| memory | 18 | 18 | 0 |
| xam | 164 | 139 | 25 |
| input | 4 | 4 | 0 |
| audio | 8 | 8 | 0 |

<details><summary>stateless: 33 calls</summary>

`DbgBreakPoint` `DbgPrint` `ExGetXConfigSetting` `KeBugCheck` `KeBugCheckEx` `KeEnableFpuExceptions` `RtlCaptureContext` `RtlCompareMemory` `RtlCompareMemoryUlong` `RtlFillMemoryUlong` `RtlImageXexHeaderField` `RtlInitAnsiString` `RtlInitUnicodeString` `RtlMultiByteToUnicodeN` `RtlNtStatusToDosError` `RtlRaiseException` `RtlTimeFieldsToTime` `RtlTimeToTimeFields` `RtlUnicodeToMultiByteN` `RtlUnwind` `XGetAVPack` `XGetGameRegion` `XGetLanguage` `XamGetExecutionId` `XamGetSystemVersion` `XamVoiceClose` `XamVoiceCreate` `XamVoiceHeadsetPresent` `XamVoiceSubmitPacket` `__C_specific_handler` `_vsnprintf` `sprintf` `vsprintf`

</details>

<details><summary>files: 15 calls</summary>

`NtCreateFile` `NtCreateIoCompletion` `NtDeviceIoControlFile` `NtFlushBuffersFile` `NtOpenFile` `NtQueryDirectoryFile` `NtQueryInformationFile` `NtQueryVolumeInformationFile` `NtReadFile` `NtReadFileScatter` `NtRemoveIoCompletion` `NtSetInformationFile` `NtSetIoCompletion` `NtWriteFile` `NtWriteFileGather`

</details>

<details><summary>objects: 27 calls</summary>

`KeInitializeSemaphore` `KeReleaseSemaphore` `KeResetEvent` `KeSetEvent` `KeWaitForMultipleObjects` `KeWaitForSingleObject` `NtCancelTimer` `NtClearEvent` `NtClose` `NtCreateEvent` `NtCreateMutant` `NtCreateSemaphore` `NtCreateTimer` `NtDuplicateObject` `NtPulseEvent` `NtReleaseMutant` `NtReleaseSemaphore` `NtSetEvent` `NtSetTimerEx` `NtSignalAndWaitForSingleObjectEx` `NtWaitForMultipleObjectsEx` `NtWaitForSingleObjectEx` `ObDereferenceObject` `ObLookupThreadByThreadId` `ObOpenObjectByName` `ObOpenObjectByPointer` `ObReferenceObjectByHandle`

</details>

<details><summary>threads: 35 calls</summary>

`ExCreateThread` `ExTerminateThread` `HalReturnToFirmware` `KeAcquireSpinLockAtRaisedIrql` `KeDelayExecutionThread` `KeEnterCriticalRegion` `KeGetCurrentProcessType` `KeLeaveCriticalRegion` `KeQueryBasePriorityThread` `KeQueryPerformanceFrequency` `KeQuerySystemTime` `KeRaiseIrqlToDpcLevel` `KeReleaseSpinLockFromRaisedIrql` `KeResumeThread` `KeSetAffinityThread` `KeSetBasePriorityThread` `KeSetDisableBoostThread` `KeTlsAlloc` `KeTlsFree` `KeTlsGetValue` `KeTlsSetValue` `KeTryToAcquireSpinLockAtRaisedIrql` `KfAcquireSpinLock` `KfLowerIrql` `KfReleaseSpinLock` `KiApcNormalRoutineNop` `NtQueueApcThread` `NtResumeThread` `NtSuspendThread` `RtlEnterCriticalSection` `RtlInitializeCriticalSection` `RtlInitializeCriticalSectionAndSpinCount` `RtlLeaveCriticalSection` `RtlTryEnterCriticalSection` `XexUnloadImageAndExitThread`

</details>

<details><summary>memory: 18 calls</summary>

`ExAllocatePoolWithTag` `ExFreePool` `KeLockL2` `KeUnlockL2` `MmAllocatePhysicalMemory` `MmAllocatePhysicalMemoryEx` `MmFreePhysicalMemory` `MmGetPhysicalAddress` `MmQueryAddressProtect` `MmQueryAllocationSize` `MmSetAddressProtect` `NtAllocateVirtualMemory` `NtFreeVirtualMemory` `NtQueryVirtualMemory` `RtlFreeAnsiString` `RtlUnicodeStringToAnsiString` `XamAlloc` `XamFree`

</details>

<details><summary>xam: 164 calls</summary>

`ExRegisterTitleTerminateNotification` `NetDll_WSACancelOverlappedIO` `NetDll_WSACleanup` `NetDll_WSACloseEvent` `NetDll_WSACreateEvent` `NetDll_WSAEventSelect` `NetDll_WSAGetLastError` `NetDll_WSAGetOverlappedResult` `NetDll_WSARecv` `NetDll_WSARecvFrom` `NetDll_WSAResetEvent` `NetDll_WSASend` `NetDll_WSASendTo` `NetDll_WSASetEvent` `NetDll_WSASetLastError` `NetDll_WSAStartup` `NetDll_WSAWaitForMultipleEvents` `NetDll_XNetCleanup` `NetDll_XNetConnect` `NetDll_XNetCreateKey` `NetDll_XNetDnsLookup` `NetDll_XNetDnsRelease` `NetDll_XNetGetBroadcastVersionStatus` `NetDll_XNetGetConnectStatus` `NetDll_XNetGetDebugXnAddr` `NetDll_XNetGetEthernetLinkStatus` `NetDll_XNetGetOpt` `NetDll_XNetGetTitleXnAddr` `NetDll_XNetInAddrToServer` `NetDll_XNetInAddrToString` `NetDll_XNetInAddrToXnAddr` `NetDll_XNetQosGetListenStats` `NetDll_XNetQosListen` `NetDll_XNetQosLookup` `NetDll_XNetQosRelease` `NetDll_XNetQosServiceLookup` `NetDll_XNetRandom` `NetDll_XNetRegisterKey` `NetDll_XNetServerToInAddr` `NetDll_XNetSetOpt` `NetDll_XNetStartup` `NetDll_XNetTsAddrToInAddr` `NetDll_XNetUnregisterInAddr` `NetDll_XNetUnregisterKey` `NetDll_XNetXnAddrToInAddr` `NetDll_XNetXnAddrToMachineId` `NetDll_XnpCapture` (stub) `NetDll_XnpConfig` (stub) `NetDll_XnpConfigUPnP` (stub) `NetDll_XnpEthernetInterceptRecv` (stub) `NetDll_XnpEthernetInterceptSetCallbacks` (stub) `NetDll_XnpEthernetInterceptXmit` (stub) `NetDll_XnpGetActiveSocketList` (stub) `NetDll_XnpGetConfigStatus` (stub) `NetDll_XnpGetVlanXboxName` (stub) `NetDll_XnpLoadConfigParams` (stub) `NetDll_XnpLoadMachineAccount` (stub) `NetDll_XnpLogonClearQEvent` (stub) `NetDll_XnpLogonGetQFlags` (stub) `NetDll_XnpLogonGetQVals` (stub) `NetDll_XnpLogonGetStatus` (stub) `NetDll_XnpLogonSetPState` (stub) `NetDll_XnpLogonSetQEvent` (stub) `NetDll_XnpLogonSetQFlags` (stub) `NetDll_XnpLogonSetQVals` (stub) `NetDll_XnpNoteSystemTime` (stub) `NetDll_XnpRegisterKeyForCallerType` (stub) `NetDll_XnpSaveConfigParams` (stub) `NetDll_XnpSaveMachineAccount` (stub) `NetDll_XnpSetVlanXboxName` (stub) `NetDll_XnpUnregisterKeyForCallerType` (stub) `NetDll___WSAFDIsSet` `NetDll_accept` `NetDll_bind` `NetDll_closesocket` `NetDll_connect` `NetDll_getpeername` `NetDll_getsockname` `NetDll_getsockopt` `NetDll_inet_addr` `NetDll_ioctlsocket` `NetDll_listen` `NetDll_recv` `NetDll_recvfrom` `NetDll_select` `NetDll_send` `NetDll_sendto` `NetDll_setsockopt` `NetDll_shutdown` `NetDll_socket` `Refresh` `XMsgCancelIORequest` `XMsgInProcessCall` `XMsgStartIORequest` `XMsgStartIORequestEx` `XMsgSystemProcessCall` `XNetLogonGetMachineID` `XNetLogonGetTitleID` `XNotifyGetNext` `XNotifyPositionUI` `XamContentClose` `XamContentCreate` `XamContentCreateDeviceEnumerator` `XamContentCreateEnumerator` `XamContentCreateEx` `XamContentDelete` `XamContentFlush` `XamContentGetCreator` `XamContentGetDeviceData` `XamContentGetDeviceName` `XamContentGetDeviceState` `XamContentGetLicenseMask` `XamContentGetThumbnail` `XamContentInstall` `XamContentLaunchImage` `XamContentResolve` `XamContentSetThumbnail` `XamCreateEnumeratorHandle` `XamEnumerate` `XamGetPrivateEnumStructureFromHandle` `XamLoaderLaunchTitle` `XamLoaderTerminateTitle` `XamNotifyCreateListener` `XamParseGamerTileKey` `XamReadTileToTexture` `XamSessionCreateHandle` `XamSessionRefObjByHandle` `XamShowAchievementsUI` `XamShowDeviceSelectorUI` `XamShowDirtyDiscErrorUI` `XamShowFriendRequestUI` `XamShowFriendsUI` `XamShowGameInviteUI` `XamShowGamerCardUIForXUID` `XamShowKeyboardUI` `XamShowMarketplaceUI` `XamShowMessageBoxUI` `XamShowMessageBoxUIEx` `XamShowMessageComposeUI` `XamShowMessagesUI` `XamShowPlayerReviewUI` `XamShowPlayersUI` `XamShowQuickChatUI` `XamShowSigninUI` `XamShowSigninUIp` `XamShowVoiceMailUI` `XamUserAreUsersFriends` `XamUserCheckPrivilege` `XamUserCreateAchievementEnumerator` `XamUserCreateStatsEnumerator` `XamUserGetMembershipTierFromXUID` `XamUserGetName` `XamUserGetOnlineCountryFromXUID` `XamUserGetSigninState` `XamUserGetXUID` `XamUserReadProfileSettings` `XamUserWriteProfileSettings` `XamWriteGamerTile` `XexCheckExecutablePrivilege` `XexGetModuleHandle` `XexGetModuleSection` `XexGetProcedureAddress` `XexLoadImage` `XexUnloadImage`

</details>

<details><summary>input: 4 calls</summary>

`XamInputGetCapabilities` `XamInputGetKeystrokeEx` `XamInputGetState` `XamInputSetState`

</details>

<details><summary>audio: 8 calls</summary>

`XAudioGetVoiceCategoryVolume` `XAudioGetVoiceCategoryVolumeChangeMask` `XAudioQueryDriverPerformance` `XAudioRegisterRenderDriverClient` `XAudioSubmitRenderDriverFrame` `XAudioUnregisterRenderDriverClient` `XMACreateContext` `XMAReleaseContext`

</details>

## 2. Graphics on any360::gfx

The title draws through `any360::gfx`: opt-in on `master`: `--any360_gpu=true` (`--any360_gpu_scale=2 --any360_gpu_msaa=4`, `--any360_gpu_vblank_hz=240` for up to 120 FPS); 83-115 FPS driving at 2x with 4x MSAA on an RTX 5070 Ti, against 54-85 for the runtime's GPU at 2x.

Still to do: make it the default renderer (today ReXGlue's Xenos emulation is), then drop the emulated GPU from the build.

- A rare resolve into a format the renderer refuses (host format 37 to guest format 4).
- `any360_gpu_fences_after_draws=false` is faster but breaks this title's display: find what it waits on.

GPU features `any360::gfx` does not handle yet (40 of 323; none stops this title today, each is one a title may need):

| Family | Features |
|---|---|
| command processor | MEM_WRITE_CNTR, COND_EXEC, EVENT_WRITE_CFL, DRAW_INDX_2_BIN, SET_STATE, SET_BIN_BASE_OFFSET, IM_STORE |
| primitive types | kTriangleWithWFlags, kExplicitMajorModeForceStart, k2DCopyRectListV1, k2DCopyRectListV2, k2DCopyRectListV3, k2DFillRectList, k2DLineStrip, k2DTriStrip |
| render features | tessellation |
| texture formats | k_1_REVERSE, k_1, k_8_B, k_16_16_EDRAM, k_8_8_8_8_A, k_16_16_16_16_EDRAM, k_32, k_32_32, k_32_32_32_32, k_32_AS_8, k_32_AS_8_8, k_16_MPEG, k_16_16_MPEG, k_8_INTERLACED, k_32_AS_8_INTERLACED, k_32_AS_8_8_INTERLACED, k_16_INTERLACED, k_16_MPEG_INTERLACED, k_16_16_MPEG_INTERLACED, k_32_32_32_FLOAT, k_DXT3A_AS_1_1_1_1, k_8_8_8_8_GAMMA_EDRAM, k_2_10_10_10_FLOAT_EDRAM |
| vertex formats | kUndefined |

## 3. Any360 as the host

Shared by every title (Any360's own roadmap); the title follows once they land.

| Any360 step | What | State |
|---|---|---|
| 11 | third and fourth titles (Test Drive Unlimited, Sonic 06) | not started |
| 12 | paths no title took yet: `XMA*` decoding, STFS packages, several users, guide UIs, Live offline, xbdm | partial: 602 of 2,914 exports native with scenario tests, none of the 2,294 reference stubs sorted |
| 13 | Any360's own host: XEX loader, address space, threads, guest dispatch, MMIO, devices | not started |
| 14 | the platform layer: window, input, audio, presenter on Windows and Linux (Steam Deck) | partial: SDL3 audio output only |
| 15 | graphics through `any360::gfx` only, no `rexgpu-xenos` | partial: three titles draw through `any360::gfx`, the GPU session's plan in [docs/GFX.md](https://github.com/GaetanVigner/Any360/blob/main/docs/GFX.md) |
| 16 | porting a new title: a converter served from GitHub (as AnyPS5) and built into the cross-platform launcher, from a XEX to a running build with no title-specific code in Any360 | not started |

## 4. Library code replaced as a unit

No library range is named yet for this title (titles/tdu/modules.json in Any360): the first step is recognising the SDK code (D3D runtime, CRT, audio and video middleware) so it is not decompiled as game code.

## 5. Game code as portable C++

45,102 of 45,102 game functions are still recompiled PowerPC. The list, function by function, is [functions.csv](functions.csv) (state `recompiled` or `verified`). By module, largest first:

| Module | Functions left | Instructions left | Rewritten | Verified |
|---|---|---|---|---|
| code 824xxxxx | 9,336 | 258,686 | 0 | 0 |
| code 825xxxxx | 7,326 | 257,298 | 0 | 0 |
| code 826xxxxx | 6,513 | 267,596 | 0 | 0 |
| code 827xxxxx | 5,534 | 258,193 | 0 | 0 |
| code 829xxxxx | 3,669 | 259,575 | 0 | 0 |
| code 82Axxxxx | 3,557 | 259,803 | 0 | 0 |
| code 82Bxxxxx | 3,188 | 147,299 | 0 | 0 |
| code 828xxxxx | 3,084 | 259,750 | 0 | 0 |
| code 823xxxxx | 2,895 | 31,620 | 0 | 0 |

Order of work: symbol recovery and library recognition first (they shrink this list), then leaf functions (no calls: each is checked alone), then their callers, module by module; every rewrite is bound over the recompiled function and proven equal in the guest sandbox before it replaces it (Any360's `pipeline/any360.py <title> select | decompile | bind | verify`).

## 6. This title

- Switch the system binding on, group by group, with an in-game check after each (stateless, files, objects, threads, memory, XAM, audio, input), as Crackdown and The Simpsons Game did.
- Name the library ranges of the executable (D3D runtime, CRT, Bink, audio middleware) in Any360's titles/tdu/modules.json.
- A golden test target (guest sandbox) so rewritten functions can be verified.

## Known problems

- A game thread spins at 90% of a core (`sub_826737B8`).
- Frame captures only trigger with `--any360_gpu_vblank_hz=1000` (the command processor is never idle at a kick otherwise).
- The intro videos once ran too fast above 60 Hz; not seen again since the video pacing fix.
