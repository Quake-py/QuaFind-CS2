#pragma once
// SkyChanger.h
// PS
// Pattern
// Pattern
#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include "hook/include/MinHook.h"
#include "PatternScan.h"    // PS
#pragma comment(lib,"psapi.lib")

namespace SkyChng {

// Konfig
inline bool  g_SkyOn  = false;
inline int   g_SkyIdx = 0;

struct SkyEntry { const char* display; const char* name; };
static const SkyEntry kSkies[] = {
    {"Varsayilan (Harita)",  ""},
    {"Dust 2",               "sky_dust2_01"},
    {"Mirage",               "sky_mirage_01"},
    {"Inferno",              "sky_inferno_01"},
    {"Nuke",                 "sky_nuke_01"},
    {"Cache",                "sky_cache_01"},
    {"Overcast 1",           "sky_overcast_01"},
    {"Overcast 2",           "sky_overcast_02"},
    {"Office",               "sky_office_01"},
    {"Gunduz 1",             "sky_cs15_daylight01_hdr"},
    {"Gunduz 2",             "sky_cs15_daylight02_hdr"},
    {"Gunduz 3",             "sky_cs15_daylight03_hdr"},
    {"Gunduz 4",             "sky_cs15_daylight04_hdr"},
    {"Gece",                 "sky_cs15_night01"},
    {"Gece 2",               "sky_cs15_night01b"},
    {"Vertigo",              "sky_vertigo_01"},
    {"cs_tibet",             "sky_tibet"},
    {nullptr, nullptr}
};
static constexpr int kSkyCount = 17;

// Durum
inline bool      g_Ready       = false;
inline bool      g_SkyHooked   = false;
inline void*     g_pSceneSys   = nullptr;
inline DWORD     g_lastAttempt = 0;
inline uintptr_t dbg_fnAddr    = 0;
inline int       dbg_mhRes     = -99;

// Hook
typedef void(__fastcall* FnUpdateSkybox)(void*, const char*);
inline FnUpdateSkybox oUpdateSkybox = nullptr;

// UpdateSkybox
void __fastcall hkUpdateSkybox(void* pThis, const char* pszSky) {
    if (!oUpdateSkybox) return;
    if (pThis) g_pSceneSys = pThis; // her
    const char* sky = pszSky;
    if (g_SkyOn && g_SkyIdx > 0 && kSkies[g_SkyIdx].name && kSkies[g_SkyIdx].name[0])
        sky = kSkies[g_SkyIdx].name;
    oUpdateSkybox(pThis, sky);
}

// EnsureHook
inline void EnsureHook() {
    // g_pSceneSys
    // 48
    // 0
    if (!g_pSceneSys) {
        auto r = PS::Scan("scenesystem.dll",
            "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 0D ? ? ? ?",
            18,   // matchOffset
            0,    // ripExtraBytes
            true  // resolveRip
        );
        if (r.found)
            __try { g_pSceneSys = *(void**)r.ptr; } __except(1) {}
    }

    if (g_SkyHooked) { g_Ready = true; return; }
    DWORD now = GetTickCount();
    if (now - g_lastAttempt < 2000) return;
    g_lastAttempt = now;

    // Pattern
    // v1
    auto r = PS::Scan("client.dll",
        "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 55 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 48 83 B9",
        0, 0, false);
    // v2
    if (!r.found) r = PS::Scan("client.dll",
        "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 55 41 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ?",
        0, 0, false);
    // v3
    if (!r.found) r = PS::Scan("client.dll",
        "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 55 41 57 48 8D 68 ? 48 81 EC ? ? ? ?",
        0, 0, false);
    // v4
    if (!r.found) r = PS::Scan("client.dll",
        "48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 55 41 57 48 8D 68 ?",
        0, 0, false);

    dbg_fnAddr = r.found ? r.ptr : 0;
    if (!r.found) return;

    dbg_mhRes = (int)MH_CreateHook((void*)r.ptr, (void*)hkUpdateSkybox, (void**)&oUpdateSkybox);
    if (dbg_mhRes == 0) {
        MH_EnableHook((void*)r.ptr);
        g_SkyHooked = true;
        g_Ready = true;
    }
}

// ForceUpdate
inline void ForceUpdate() {
    if (!oUpdateSkybox || !g_pSceneSys) return;
    const char* sky = (g_SkyOn && g_SkyIdx > 0 && kSkies[g_SkyIdx].name[0])
        ? kSkies[g_SkyIdx].name : "sky_dust2_01";
    __try { oUpdateSkybox(g_pSceneSys, sky); } __except(1) {}
}

// Shutdown
inline void Shutdown() {
    g_Ready = g_SkyHooked = false;
    g_pSceneSys = nullptr;
}

// Stubs
inline void Init()      {}
inline void Init(void*) {}
inline bool HandleDrawCall(void*,unsigned,unsigned,unsigned,int,unsigned,unsigned,void*) { return false; }
inline void DrawSkyPass(void* a, void* b, void* c, void* d=nullptr) { (void)a;(void)b;(void)c;(void)d; }
inline void SaveDSV(void*)  {}
inline void FrameEnd()      {}
inline bool  g_SkyColorOn  = false;
inline float g_SkyColor[4] = {0.3f, 0.5f, 1.f, 1.f};
inline void* g_pSceneSysPtr= nullptr;
inline void* g_DepthSRV    = nullptr;
inline float g_Threshold   = 0.005f;
inline bool  g_LogActive   = false;
inline int   g_LogFrames   = 0;
inline bool  g_CubeCheck   = false;
inline int   g_SkyStride   = 12;
inline void* g_SavedDSV    = nullptr;
inline void* g_DSS         = nullptr;

} // namespace
