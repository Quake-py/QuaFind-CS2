#pragma once
// BombTimer.h
// SADECE
// m_bBombTicking
#include "SDK.h"

namespace BT {

inline bool on = false;

static constexpr ptrdiff_t kBombSite = 0x1164; // int32
static constexpr ptrdiff_t kC4Blow = 0x1190;   // GameTime_t
static constexpr ptrdiff_t kTimerLen = 0x1198; // float32
static constexpr ptrdiff_t kDefused = 0x11B4;  // bool

enum class State { IDLE, ACTIVE, DEFUSED, EXPLODED };
inline State state = State::IDLE;
inline int site = -1;
inline float timeLeft = 0.f;
inline float s_timerLen = 40.f;

static uintptr_t s_bombEnt      = 0;
static ULONGLONG s_plantTick    = 0;
static float     s_lastBlowTime = 0.f;
inline ULONGLONG s_endTick      = 0; // DEFUSED

static void Reset() {
  state = State::IDLE;
  site = -1;
  timeLeft = 0.f;
  s_plantTick = 0;
  s_bombEnt = 0;
}

// Entity
// timerLen
static bool IsC4Ent(uintptr_t ent) {
  if (!ent || ent <= 0x10000)
    return false;
  float timerLen = SafeRead<float>(ent + kTimerLen);
  if (timerLen < 5.f || timerLen > 120.f)
    return false;
  float blowTime = SafeRead<float>(ent + kC4Blow);
  if (blowTime <= 0.f || blowTime > 1e9f)
    return false;
  // Ayni
  if (s_lastBlowTime > 0.f && fabsf(blowTime - s_lastBlowTime) < 0.5f)
    return false;
  return true;
}

// dwPlantedC4
static uintptr_t FindC4() {
  if (!G.client || G.client <= 0x10000)
    return 0;
  uintptr_t el = SafeRead<uintptr_t>(G.client + Off::dwEntityList);
  uintptr_t c4Base = G.client + Off::dwPlantedC4;
  uintptr_t dataPtr = SafeRead<uintptr_t>(c4Base); // 0

  // Layout
  if (el > 0x10000) {
    int cnt = SafeRead<int>(c4Base + 0x8);
    if (cnt >= 1 && cnt <= 8 && dataPtr > 0x10000) {
      uint32_t h = SafeRead<uint32_t>(dataPtr);
      if (h && h != 0xFFFFFFFF) {
        uintptr_t ent = GetPawnByHandle(el, h);
        if (IsC4Ent(ent))
          return ent;
      }
    }
  }

  // Layout
  if (el > 0x10000) {
    int cnt = SafeRead<int>(c4Base + 0x10);
    if (cnt >= 1 && cnt <= 8 && dataPtr > 0x10000) {
      uint32_t h = SafeRead<uint32_t>(dataPtr);
      if (h && h != 0xFFFFFFFF) {
        uintptr_t ent = GetPawnByHandle(el, h);
        if (IsC4Ent(ent))
          return ent;
      }
    }
  }

  // Layout
  if (el > 0x10000) {
    uint32_t h = (uint32_t)(dataPtr & 0xFFFFFFFF); // alt
    if (h && h != 0xFFFFFFFF) {
      uintptr_t ent = GetPawnByHandle(el, h);
      if (IsC4Ent(ent))
        return ent;
    }
  }

  // Layout
  // pointer
  if (dataPtr > 0x10000 && IsC4Ent(dataPtr))
    return dataPtr;

  // Layout
  if (dataPtr > 0x10000) {
    uintptr_t ent = SafeRead<uintptr_t>(dataPtr);
    if (IsC4Ent(ent))
      return ent;
  }

  return 0;
}

inline void Tick() {
  if (!G.client || G.client <= 0x10000) {
    Reset();
    return;
  }

  // ACTIVE
  if (state == State::ACTIVE) {
    if (s_bombEnt > 0x10000 && SafeRead<bool>(s_bombEnt + kDefused)) {
      state    = State::DEFUSED;
      timeLeft = 0.f;
      s_endTick = GetTickCount64();
      return;
    }
    float elapsed = (float)(GetTickCount64() - s_plantTick) / 1000.f;
    timeLeft = s_timerLen - elapsed;
    if (timeLeft <= 0.f) {
      timeLeft  = 0.f;
      state     = State::EXPLODED;
      s_endTick = GetTickCount64();
    }
    return;
  }

  // IDLE
  uintptr_t ent = FindC4();
  if (!ent)
    return;

  float timerLen = SafeRead<float>(ent + kTimerLen);
  float blowTime = SafeRead<float>(ent + kC4Blow);
  int bombSite = SafeRead<int>(ent + kBombSite);
  if (bombSite < 0 || bombSite > 1)
    bombSite = 0;

  s_bombEnt = ent;
  s_plantTick = GetTickCount64();
  s_lastBlowTime = blowTime;
  s_timerLen = timerLen;
  site = bombSite;
  timeLeft = timerLen;
  state = State::ACTIVE;
}

} // namespace
