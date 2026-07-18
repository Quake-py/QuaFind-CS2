// CrashLogger.h
// SetUnhandledExceptionFilter
// -
// -
// Log
#pragma once
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <atomic>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

namespace CL {

// Breadcrumb
static char s_crumb[256]  = "startup";
static char s_crumb2[256] = "-";
static std::atomic<int> s_frameCount{0};

inline void Crumb(const char* what, const char* detail = nullptr) {
    strncpy_s(s_crumb,  sizeof(s_crumb),  what,                  _TRUNCATE);
    strncpy_s(s_crumb2, sizeof(s_crumb2), detail ? detail : "-", _TRUNCATE);
}

inline void FrameTick() {
    s_frameCount.fetch_add(1, std::memory_order_relaxed);
}

// Log
static void GetLogPath(char* buf, int size) {
    char docs[MAX_PATH] = {};
    SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, 0, docs);
    if (!docs[0]) strncpy_s(docs, sizeof(docs), "C:\\Users\\Public", _TRUNCATE);
    snprintf(buf, size, "%s\\QuaFind_crash.log", docs);
}

// Crash
static void WriteCrashLog(EXCEPTION_POINTERS* ep) {
    char path[MAX_PATH];
    GetLogPath(path, sizeof(path));

    FILE* f = nullptr;
    fopen_s(&f, path, "a");
    if (!f) fopen_s(&f, "C:\\Users\\Public\\QuaFind_crash.log", "a");
    if (!f) return;

    time_t now = time(nullptr);
    char tbuf[64]; ctime_s(tbuf, sizeof(tbuf), &now);
    if (tbuf[strlen(tbuf)-1] == '\n') tbuf[strlen(tbuf)-1] = '\0';

    EXCEPTION_RECORD* er = ep ? ep->ExceptionRecord : nullptr;
    CONTEXT*          ct = ep ? ep->ContextRecord   : nullptr;

    fprintf(f, "\n============================================================\n");
    fprintf(f, "  QUAFIND — CRASH LOG\n");
    fprintf(f, "  Zaman   : %s\n", tbuf);
    fprintf(f, "  Frame   : %d\n", s_frameCount.load());
    fprintf(f, "------------------------------------------------------------\n");
    fprintf(f, "  Son islem : %s  /  %s\n", s_crumb, s_crumb2);
    fprintf(f, "------------------------------------------------------------\n");

    if (er) {
        fprintf(f, "  Exception Code    : 0x%08X", (unsigned)er->ExceptionCode);
        switch (er->ExceptionCode) {
            case EXCEPTION_ACCESS_VIOLATION:    fprintf(f, " (ACCESS_VIOLATION)\n");    break;
            case EXCEPTION_STACK_OVERFLOW:      fprintf(f, " (STACK_OVERFLOW)\n");      break;
            case EXCEPTION_ILLEGAL_INSTRUCTION: fprintf(f, " (ILLEGAL_INSTRUCTION)\n"); break;
            case EXCEPTION_INT_DIVIDE_BY_ZERO:  fprintf(f, " (DIVIDE_BY_ZERO)\n");      break;
            default:                            fprintf(f, "\n");                        break;
        }
        fprintf(f, "  Exception Address : 0x%p\n", er->ExceptionAddress);
        if (er->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && er->NumberParameters >= 2) {
            const char* op = er->ExceptionInformation[0]==0 ? "READ" :
                             er->ExceptionInformation[0]==1 ? "WRITE" : "EXECUTE";
            fprintf(f, "  AV Op             : %s at 0x%llX\n",
                    op, (unsigned long long)er->ExceptionInformation[1]);
        }
    }

    if (ct) {
#ifdef _WIN64
        fprintf(f, "  RIP=0x%016llX  RSP=0x%016llX\n", ct->Rip, ct->Rsp);
        fprintf(f, "  RAX=0x%016llX  RCX=0x%016llX\n", ct->Rax, ct->Rcx);
        fprintf(f, "  RDX=0x%016llX  RBX=0x%016llX\n", ct->Rdx, ct->Rbx);
#else
        fprintf(f, "  EIP=0x%08X  ESP=0x%08X\n", (unsigned)ct->Eip, (unsigned)ct->Esp);
#endif
    }

    if (er && er->ExceptionAddress) {
        HMODULE hm = nullptr;
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                               (LPCSTR)er->ExceptionAddress, &hm) && hm) {
            char mname[MAX_PATH] = {};
            GetModuleFileNameA(hm, mname, sizeof(mname));
            uintptr_t off = (uintptr_t)er->ExceptionAddress - (uintptr_t)hm;
            const char* base = strrchr(mname, '\\');
            fprintf(f, "  Module            : %s + 0x%llX\n",
                    base ? base+1 : mname, (unsigned long long)off);
            FreeLibrary(hm);
        }
    }
    fprintf(f, "============================================================\n");
    fclose(f);

    printf("[CRASH] Log kaydedildi: %s | Son: %s/%s Frame=%d\n",
           path, s_crumb, s_crumb2, s_frameCount.load());
}

// SetUnhandledExceptionFilter
// Sadece
static LPTOP_LEVEL_EXCEPTION_FILTER s_prevFilter = nullptr;

static LONG WINAPI CrashFilter(EXCEPTION_POINTERS* ep) {
    WriteCrashLog(ep);
    if (s_prevFilter) return s_prevFilter(ep); // breakpad
    return EXCEPTION_CONTINUE_SEARCH;
}

// Kur
inline void Install() {
    s_prevFilter = SetUnhandledExceptionFilter(CrashFilter);
    printf("[CrashLogger] SUEF kuruldu. Log: %%DOCUMENTS%%\\QuaFind_crash.log\n");
}

inline void Remove() {
    SetUnhandledExceptionFilter(s_prevFilter); // orijinali
    s_prevFilter = nullptr;
}

} // namespace

// Kisaltma
#define CRUMB(what)          CL::Crumb(what)
#define CRUMB2(what, detail) CL::Crumb(what, detail)
