// Test Drive Unlimited - ReXGlue recompiled project.

#pragma once

#include <filesystem>
#include <string>

#include <rex/rex_app.h>

#ifdef _WIN32
#include <windows.h>
#endif

class TduApp : public rex::ReXApp {
 public:
  using rex::ReXApp::ReXApp;

  static std::unique_ptr<rex::ui::WindowedApp> Create(rex::ui::WindowedAppContext& ctx) {
    return std::unique_ptr<TduApp>(new TduApp(ctx, "tdu", PPCImageConfig));
  }

  // The runtime's GPU unless --gpu_plugin names another.
  void OnPreSetup(rex::RuntimeConfig& config) override {
    if (config.gpu_plugin.empty()) config.gpu_plugin = "xenos";
  }

  // Without --game_data_root: a game folder next to the executable, else the repository's (out/build/tdu.exe).
  void OnConfigurePaths(rex::PathConfig& paths) override {
    if (!paths.game_data_root.empty()) return;
    const std::filesystem::path exe_dir = ExecutableDirectory();
    for (const std::filesystem::path& dir : {exe_dir / "game", exe_dir / ".." / ".." / "game"}) {
      std::error_code error;
      if (std::filesystem::exists(dir / "default.xex", error)) {
        paths.game_data_root = std::filesystem::weakly_canonical(dir, error);
        return;
      }
    }
  }

 private:
  static std::filesystem::path ExecutableDirectory() {
#ifdef _WIN32
    wchar_t buffer[32768];
    const unsigned long n = GetModuleFileNameW(nullptr, buffer, 32768);
    return std::filesystem::path(std::wstring(buffer, n)).parent_path();
#else
    std::error_code error;
    return std::filesystem::read_symlink("/proc/self/exe", error).parent_path();
#endif
  }
};
