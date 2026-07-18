#pragma once
// FovChanger.h
// SADECE
// Scope
// Flickering
// tasinmasi
#include "SDK.h"

namespace FovChng {

inline bool g_FovOn = false;
inline int  g_Fov   = 90;

// m_bIsScoped
// Scope
inline void Apply() {
    if (!g_FovOn) {
        // Kapaliysa
        if (!G.localPawn || G.localPawn <= 0x10000) return;
        uintptr_t camSvc = SafeRead<uintptr_t>(G.localPawn + Off::m_pCameraServices);
        if (!camSvc || camSvc <= 0x10000) return;
        SafeWrite<uint32_t>(camSvc + Off::m_iFOV_Cam, 0u);
        return;
    }

    if (!G.localPawn || G.localPawn <= 0x10000) return;

    // Scope
    bool isScoped = SafeRead<bool>(G.localPawn + Off::m_bIsScoped);
    if (isScoped) return;

    uintptr_t camSvc = SafeRead<uintptr_t>(G.localPawn + Off::m_pCameraServices);
    if (!camSvc || camSvc <= 0x10000) return;

    // Mevcut
    uint32_t cur = SafeRead<uint32_t>(camSvc + Off::m_iFOV_Cam);
    if (cur > 1 && cur < (uint32_t)g_Fov) return; // oyun

    // Normal
    SafeWrite<uint32_t>(camSvc + Off::m_iFOV_Cam, (uint32_t)g_Fov);
}

inline void Shutdown() {
    if (!G.localPawn || G.localPawn <= 0x10000) return;
    uintptr_t camSvc = SafeRead<uintptr_t>(G.localPawn + Off::m_pCameraServices);
    if (!camSvc || camSvc <= 0x10000) return;
    SafeWrite<uint32_t>(camSvc + Off::m_iFOV_Cam, 0u);
}

} // namespace
