// dear
// This

// Implemented
// X
// X
// X
// X
// X

// You
// Prefer
// If
// Read

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE

IMGUI_IMPL_API bool     ImGui_ImplWin32_Init(void* hwnd);
IMGUI_IMPL_API bool     ImGui_ImplWin32_InitForOpenGL(void* hwnd);
IMGUI_IMPL_API void     ImGui_ImplWin32_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplWin32_NewFrame();

// Win32
// -
// -
// -

#if 0
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

// DPI-related
// -
// -
// -
// but
// neither
IMGUI_IMPL_API void     ImGui_ImplWin32_EnableDpiAwareness();
IMGUI_IMPL_API float    ImGui_ImplWin32_GetDpiScaleForHwnd(void* hwnd);       // HWND
IMGUI_IMPL_API float    ImGui_ImplWin32_GetDpiScaleForMonitor(void* monitor); // HMONITOR

// Transparency
// -
// -
IMGUI_IMPL_API void     ImGui_ImplWin32_EnableAlphaCompositing(void* hwnd);   // HWND

#endif // ifndef
