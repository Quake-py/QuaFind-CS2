/*
 *  MinHook - The Minimalistic API Hooking Library for x64/x86
 *  Copyright (C) 2009-2017 Tsuda Kageyu.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 *  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if !(defined _M_IX86) && !(defined _M_X64) && !(defined __i386__) && !(defined __x86_64__)
    #error MinHook supports only x86 and x64 systems.
#endif

#include <windows.h>

// MinHook
typedef enum MH_STATUS
{
    // Unknown
    MH_UNKNOWN = -1,

    // Successful.
    MH_OK = 0,

    // MinHook
    MH_ERROR_ALREADY_INITIALIZED,

    // MinHook
    MH_ERROR_NOT_INITIALIZED,

    // The
    MH_ERROR_ALREADY_CREATED,

    // The
    MH_ERROR_NOT_CREATED,

    // The
    MH_ERROR_ENABLED,

    // The
    // disabled.
    MH_ERROR_DISABLED,

    // The
    // and
    MH_ERROR_NOT_EXECUTABLE,

    // The
    MH_ERROR_UNSUPPORTED_FUNCTION,

    // Failed
    MH_ERROR_MEMORY_ALLOC,

    // Failed
    MH_ERROR_MEMORY_PROTECT,

    // The
    MH_ERROR_MODULE_NOT_FOUND,

    // The
    MH_ERROR_FUNCTION_NOT_FOUND
}
MH_STATUS;

// Can
// MH_QueueEnableHook
#define MH_ALL_HOOKS NULL

#ifdef __cplusplus
extern "C" {
#endif

    // Initialize
    // at
    MH_STATUS WINAPI MH_Initialize(VOID);

    // Uninitialize
    // ONCE
    MH_STATUS WINAPI MH_Uninitialize(VOID);

    // Creates
    // Parameters
    // pTarget
    // overridden
    // pDetour
    // the
    // ppOriginal
    // used
    // This
    MH_STATUS WINAPI MH_CreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal);

    // Creates
    // Parameters
    // pszModule
    // target
    // pszProcName
    // overridden
    // pDetour
    // the
    // ppOriginal
    // used
    // This
    MH_STATUS WINAPI MH_CreateHookApi(
        LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal);

    // Creates
    // Parameters
    // pszModule
    // target
    // pszProcName
    // overridden
    // pDetour
    // the
    // ppOriginal
    // used
    // This
    // ppTarget
    // with
    // This
    MH_STATUS WINAPI MH_CreateHookApiEx(
        LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal, LPVOID *ppTarget);

    // Removes
    // Parameters
    // pTarget
    MH_STATUS WINAPI MH_RemoveHook(LPVOID pTarget);

    // Enables
    // Parameters
    // pTarget
    // If
    // enabled
    MH_STATUS WINAPI MH_EnableHook(LPVOID pTarget);

    // Disables
    // Parameters
    // pTarget
    // If
    // disabled
    MH_STATUS WINAPI MH_DisableHook(LPVOID pTarget);

    // Queues
    // Parameters
    // pTarget
    // If
    // queued
    MH_STATUS WINAPI MH_QueueEnableHook(LPVOID pTarget);

    // Queues
    // Parameters
    // pTarget
    // If
    // queued
    MH_STATUS WINAPI MH_QueueDisableHook(LPVOID pTarget);

    // Applies
    MH_STATUS WINAPI MH_ApplyQueued(VOID);

    // Translates
    const char *WINAPI MH_StatusToString(MH_STATUS status);

#ifdef __cplusplus
}
#endif
