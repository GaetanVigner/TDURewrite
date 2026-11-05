#pragma once

namespace tdu {

// With any360_gpu set, the GPU goes through Any360 instead of the runtime's. Call before the title's first Vd* call
// (OnPostSetup).
void StartAny360GraphicsIfAsked();

}  // namespace tdu
