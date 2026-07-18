// PatternScan.h
// Pattern
// Her
#pragma once
#include <windows.h>
#include <cstdint>
#include <cstdio>

namespace PS {
//
// BYTE
//
static bool Match(const uint8_t* data, const char* pat) {
  for (const char* p = pat; *p; data++) {
    if (*p == ' ') { p++; continue; }
    if (*p == '?') {
      p++; if (*p == '?') p++;
      continue;
    }
    auto hex = [](char c) -> uint8_t {
      if (c >= '0' && c <= '9') return c - '0';
      if (c >= 'A' && c <= 'F') return c - 'A' + 10;
      if (c >= 'a' && c <= 'f') return c - 'a' + 10;
      return 0;
    };
    uint8_t byte = (hex(p[0]) << 4) | hex(p[1]);
    if (*data != byte) return false;
    p += 2;
  }
  return true;
}

static size_t PatLen(const char* pat) {
  size_t n = 0; bool inByte = false;
  for (const char* p = pat; *p; p++) {
    if (*p == ' ')   { inByte = false; continue; }
    if (*p == '?')   { n++;            continue; }
    if (!inByte)     { n++; inByte = true; }
  }
  return n;
}

struct Result { uintptr_t ptr = 0; bool found = false; };

// DLL
static Result Scan(
    const char* modName,
    const char* pattern,
    ptrdiff_t   matchOffset   = 0,
    ptrdiff_t   ripExtraBytes = 0,
    bool        resolveRip    = true
) {
  HMODULE hMod = GetModuleHandleA(modName);
  if (!hMod) return {};

  auto* dos = (IMAGE_DOS_HEADER*)hMod;
  auto* nt  = (IMAGE_NT_HEADERS*)((uint8_t*)hMod + dos->e_lfanew);
  auto* sec = IMAGE_FIRST_SECTION(nt);

  uintptr_t base = (uintptr_t)hMod;
  uintptr_t scanStart = 0, scanSize = 0;

  // En
  for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++, sec++) {
    if ((sec->Characteristics & IMAGE_SCN_MEM_EXECUTE) &&
        (sec->Characteristics & IMAGE_SCN_MEM_READ)) {
      uintptr_t s = base + sec->VirtualAddress;
      uintptr_t sz = sec->Misc.VirtualSize;
      if (sz > scanSize) { scanStart = s; scanSize = sz; }
    }
  }
  if (!scanStart || !scanSize) return {};

  size_t pLen = PatLen(pattern);

  __try {
    for (uintptr_t i = scanStart; i < scanStart + scanSize - pLen; i++) {
      if (!Match((uint8_t*)i, pattern)) continue;

      uintptr_t matchAddr = i + matchOffset;

      if (resolveRip) {
        int32_t disp = *(int32_t*)matchAddr;
        uintptr_t rip = matchAddr + 4 + ripExtraBytes;
        return { rip + (uintptr_t)disp, true };
      } else {
        return { matchAddr, true };
      }
    }
  } __except(EXCEPTION_EXECUTE_HANDLER) {}

  return {};
}

static uintptr_t ToOffset(uintptr_t absAddr, const char* modName) {
  uintptr_t base = (uintptr_t)GetModuleHandleA(modName);
  if (!base || absAddr < base) return 0;
  return absAddr - base;
}

//
// CS2-DUMPER
//
// Pelite
// 4
// 4
// 4
// u4
//
static void ScanAllOffsets() {
  printf("[SCAN] Başlatıldı (cs2-dumper pattern'leri)...\n");
  const char* cl = "client.dll";

  // dwCSGOInput
  // pelite
  {
    auto r = Scan(cl, "48 89 05 ? ? ? ? 0F 57 C0 0F 11 05", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwCSGOInput    = 0x%llX  %s\n",
             off, off==Off::dwCSGOInput?"OK":"DIFF!");
      if (off > 0x100000 && off < 0x8000000) Off::dwCSGOInput = off;
    } else printf("[SCAN] dwCSGOInput    NOT FOUND\n");
  }

  // dwEntityList
  // pelite
  {
    auto r = Scan(cl, "48 89 0D ? ? ? ? E9 ? ? ? ? CC", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwEntityList   = 0x%llX  %s\n",
             off, off==Off::dwEntityList?"OK":"DIFF!");
      if (off > 0x100000 && off < 0x8000000) Off::dwEntityList = off;
    } else printf("[SCAN] dwEntityList   NOT FOUND\n");
  }

  // dwGlowManager
  // pelite
  {
    auto r = Scan(cl, "48 8B 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC CC 8B 41", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwGlowManager  = 0x%llX  %s\n",
             off, off==Off::dwGlowManager?"OK":"DIFF!");
      if (off > 0x100000 && off < 0x8000000) Off::dwGlowManager = off;
    } else printf("[SCAN] dwGlowManager  NOT FOUND\n");
  }

  // dwViewMatrix
  // pelite
  {
    auto r = Scan(cl, "48 8D 0D ? ? ? ? 48 C1 E0 06", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwViewMatrix   = 0x%llX  %s\n",
             off, off==Off::dwViewMatrix?"OK":"DIFF!");
      if (off > 0x100000 && off < 0x8000000) Off::dwViewMatrix = off;
    } else printf("[SCAN] dwViewMatrix   NOT FOUND\n");
  }

  // dwLocalPlayerController
  // pelite
  {
    auto r = Scan(cl, "48 8B 05 ? ? ? ? 41 89 BE", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwLocalCtrl    = 0x%llX\n", off);
      if (off > 0x100000 && off < 0x8000000) Off::dwLocalPawnCtrl = off;
    } else printf("[SCAN] dwLocalCtrl    NOT FOUND\n");
  }

  // dwPlantedC4
  // pelite
  {
    auto r = Scan(cl, "48 8B 15 ? ? ? ? 41 FF C0 48 8D 4C 24 ? 44 89 05", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwPlantedC4    = 0x%llX  %s\n",
             off, off==Off::dwPlantedC4?"OK":"DIFF!");
      if (off > 0x100000 && off < 0x8000000) Off::dwPlantedC4 = off;
    } else printf("[SCAN] dwPlantedC4    NOT FOUND\n");
  }

  // dwGameRules
  // pelite
  {
    auto r = Scan(cl, "48 89 1D ? ? ? ? FF 15 ? ? ? ? 84 C0", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwGameRules    = 0x%llX\n", off);
    } else printf("[SCAN] dwGameRules    NOT FOUND\n");
  }

  // dwGlobalVars
  // pelite
  {
    auto r = Scan(cl, "48 89 15 ? ? ? ? 48 89 42", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwGlobalVars   = 0x%llX\n", off);
    } else printf("[SCAN] dwGlobalVars   NOT FOUND\n");
  }

  // dwPrediction
  // pelite
  // Sonra
  // pelite
  {
    auto rPred = Scan(cl, "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC 40 53 56 41 54", 3, 0);
    if (rPred.found) {
      uintptr_t predOff = ToOffset(rPred.ptr, cl);
      printf("[SCAN] dwPrediction   = 0x%llX\n", predOff);

      // imdi
      auto rInner = Scan(cl, "4C 39 B6 ? ? ? ? 74 ? 44 88 BE", 3, 0, false);
      if (rInner.found) {
        uint32_t innerOff = *(uint32_t*)rInner.ptr;
        uintptr_t pawnOff = predOff + (uintptr_t)innerOff;
        printf("[SCAN] dwLocalPawn    = Pred+0x%X = 0x%llX  %s\n",
               innerOff, pawnOff, pawnOff==Off::dwLocalPawn?"OK":"DIFF!");
        if (pawnOff > 0x100000 && pawnOff < 0x8000000) Off::dwLocalPawn = pawnOff;
      } else printf("[SCAN] dwLocalPawn    inner NOT FOUND\n");
    } else printf("[SCAN] dwPrediction   NOT FOUND\n");
  }

  // dwViewAngles
  // cs2-dumper
  // pelite
  // Bu
  {
    auto r = Scan(cl, "F2 42 0F 10 84 28 ? ? ? ?", 6, 0, false);
    if (r.found) {
      uint32_t vaInner = *(uint32_t*)r.ptr;
      // dwViewAngles
      uintptr_t vaOff = Off::dwCSGOInput + (uintptr_t)vaInner;
      printf("[SCAN] dwViewAngles   = CSGOInput+0x%X = 0x%llX  %s\n",
             vaInner, vaOff, vaOff==Off::dwViewAngles?"OK":"DIFF!");
      if (vaOff > 0x100000 && vaOff < 0x8000000) Off::dwViewAngles = vaOff;
    } else printf("[SCAN] dwViewAngles   inner NOT FOUND\n");
  }



  // dwForceJump
  // pelite
  {
    auto r = Scan(cl, "48 89 05 ? ? ? ? 0F 11 05 ? ? ? ? 48 89 05", 3, 0);
    if (r.found) {
      uintptr_t off = ToOffset(r.ptr, cl);
      printf("[SCAN] dwForceJump    = 0x%llX  %s\n",
             off, off==Off::dwForceJump?"OK":"DIFF!");
      if (off > 0x100000 && off < 0x8000000) Off::dwForceJump = off;
    } else printf("[SCAN] dwForceJump    NOT FOUND (hardcoded)\n");
  }

  // Sonu
  printf("[SCAN] === Aktif Offsetler ===\n");
  printf("  dwCSGOInput    = 0x%llX\n", Off::dwCSGOInput);
  printf("  dwEntityList   = 0x%llX\n", Off::dwEntityList);
  printf("  dwGlowManager  = 0x%llX\n", Off::dwGlowManager);
  printf("  dwViewMatrix   = 0x%llX\n", Off::dwViewMatrix);
  printf("  dwLocalPawn    = 0x%llX\n", Off::dwLocalPawn);
  printf("  dwViewAngles   = 0x%llX\n", Off::dwViewAngles);
  printf("  dwForceJump    = 0x%llX\n", Off::dwForceJump);
  printf("  [ThirdPerson] ObserverServices @ pawn+0x%X, iObserverMode @ svc+0x%X\n",
         (unsigned)Off::m_pObserverServices, (unsigned)Off::m_iObserverMode);
  printf("[SCAN] Tamamlandi.\n");
}

} // namespace
