#pragma once
#include "SDK.h"
#include <cmath>
#include <cstdio>
#include <windows.h>

//
// CONFIG
//
namespace Cfg {
// ESP
inline bool espOn = true;
inline bool espBox = true;
inline bool espName = true;
inline bool espHp = true;
inline bool espBones = true;
inline bool espHead = true;
inline bool espHeadCircle = true;
inline bool espSnap = false;
inline bool espWeapon = true;
inline bool espHint = false;
inline bool espFovCircle = true;
inline bool espTeam = false;
inline int boxStyle = 0;
inline int hpBarSide = 0;
inline float colBoxVis[3] = {0.2f, 1.f, 0.2f};
inline float colBoxOcc[3] = {0.6f, 0.2f, 0.9f};
inline float colBone[3] = {0.9f, 0.9f, 1.f};
inline float colHead[3] = {1.f, 0.3f, 0.3f};
inline float colSnap[3] = {1.f, 1.f, 0.f};
inline float colWeaponIcon[3] = {1.f, 1.f, 1.f};
inline float weaponIconSize  = 12.f; // 1-24

// Ekran
inline bool  screenFogOn              = false;
inline bool  screenFogTop             = true;
inline float colFogTop[4]             = {0.f,0.f,0.f,0.7f}; // RGBA
inline bool  screenFogBottom          = true;
inline float colFogBottom[4]          = {0.f,0.f,0.f,0.7f};
inline bool  screenFogLeft            = true;
inline float colFogLeft[4]            = {0.f,0.f,0.f,0.7f};
inline bool  screenFogRight           = true;
inline float colFogRight[4]           = {0.f,0.f,0.f,0.7f};

// Overlay
inline bool spectatorListOn  = false;
inline bool shortcutListOn   = false;
inline bool bombTimerOn      = false;
inline bool infoOverlayOn    = true;  // her
inline int  windowArrangeKey = 0;
// Pencere
inline float specWinX=20.f,   specWinY=300.f;
inline float scWinX=20.f,     scWinY=120.f;
inline float bombWinX=20.f,   bombWinY=500.f;
inline float infoWinX=-1.f,   infoWinY=-1.f; // -1

// Glow
inline bool glowOn = false;
inline int glowKey = 0;
inline bool glowTeam = false;
inline float glowVis[3] = {0.2f, 1.f, 0.2f};
inline float glowWall[3] = {0.8f, 0.2f, 1.f};

// Chams
inline bool chamsOn = false;
inline int chamsKey = 0;
inline bool chamsWall = true;
inline bool chamsTeam = false;
inline int chamsMat = 0; // 0
inline float chamsVis[3] = {0.f, 1.f, 0.4f};
inline float chamsOcc[3] = {0.8f, 0.f, 1.f};
// Chams
inline bool  chamsRainbowOn     = false;
inline float chamsRainbowSpd    = 0.2f;   // hue
inline bool  chamsRainbowCustom = false;  // false
inline int   chamsRainbowCnt    = 3;      // 2-4
inline float chamsRainbowCols[4][3] = {
  {1.f,0.f,0.f}, {0.f,0.5f,1.f}, {0.f,1.f,0.3f}, {1.f,0.8f,0.f}
};

// Silah

// Legit
inline bool aimOn = false;
inline int aimKey = 0;
inline bool aimNeedKey = false;
inline int aimBone = 0;
inline float aimFov = 120.f;
inline float aimSmooth = 8.f;
inline float aimDz = 20.f;       // Olu
inline bool aimLock = true;
inline bool aimWall = true;
inline bool aimShootTeam = false;
inline bool aimProxOn  = false;  // Yakinlik
inline float aimProxDist = 15.f; // Yakinlik

// Rage
inline bool rageOn = false;
inline int rageKey = 0;
inline bool rageNeedKey = false;
inline int rageBone = 0;
inline float rageFov = 999.f;
inline float rageSmooth = 1.f;
inline bool rageAuto = true;
inline float rageAutoDelay = 80.f;
inline bool rageShootTeam = false;

// Triggerbot
inline bool trigOn = false;
inline int trigKey = 0;
inline bool trigNeedKey = false;
inline float trigDelay = 50.f;
inline bool trigWall = true;
inline bool trigShootTeam = false;

// Misc
inline bool bhopOn = false;
inline int bhopKey = VK_SPACE;
inline bool autoStrafe = true;  // Auto-strafe
inline bool noFlash = false;
inline bool skyboxChangerOn = false;
inline float skyboxColor[3] = {1.f, 0.f, 0.f};

inline bool noScope = false;

// Crosshair
inline bool custXhair = false;
inline float custXhairSize = 10.f;
inline int custXhairGap = 3;
inline float custXhairThick = 1.5f;
inline float colXhair[3] = {0.1f, 1.f, 0.2f};

// Anti-Aim
inline bool aaOn = false;
inline int aaMode = 0;
inline float aaYawAdd = 180.f;
inline int aaKey = 0;

// Third

// Radar
inline bool radarHack = false;

// Grenade
inline bool grenadeESP = true;
inline bool molotovWall = true;
inline float colGrenadeT[3] = {1.f, 0.85f, 0.1f};
inline float colGrenadeCT[3] = {0.3f, 0.6f, 1.f};

// Dusman
inline bool  enemyIndicator       = false;
inline float enemyIndicatorCol[4] = {1.f, 0.15f, 0.15f, 0.90f}; // RGBA
inline float enemyIndicatorSize   = 10.f;  // Ucgen

// ESP
inline bool espDistOn = false;

// Dunya
inline bool  dx11WorldOn        = false;
inline float dx11WorldColor[4]  = {1.00f, 0.85f, 0.60f, 0.12f}; // RGBA

// Sky
inline bool  skyColorOn  = false;
inline float skyColor[3] = {0.22f, 0.45f, 0.80f};  // RGB
inline float skyAlpha    = 0.55f;                   // Opakl
inline float skyHeight   = 0.42f;                   // Ekranda

// No
inline bool  noSmoke      = false;

// Kar
inline bool  snowOn       = false;
inline int   snowDensity  = 1;   // 0
inline bool  particle3D   = false; // 3D

// Sakura
inline bool  sakuraOn        = false;
inline float sakuraColor[4]  = {1.00f, 0.55f, 0.70f, 0.85f};


// UI
inline bool menuOpen = true;

// Write
inline ULONGLONG firstPawnSeen = 0;
constexpr ULONGLONG WRITE_DELAY = 2000;
inline bool WriteSafe() {
  if (!firstPawnSeen)
    return false;
  return (GetTickCount64() - firstPawnSeen) >= WRITE_DELAY;
}
} // namespace

//
// RADAR
//
struct SpotSave {
  uintptr_t ss;
  uint32_t mask;
  bool spotted;
};
static SpotSave s_SpotSaves[64];
static int s_SpotCount = 0;

inline void RunRadarHack() {
  s_SpotCount = 0;
  if (!Cfg::radarHack || !G.client)
    return;
  if (!Cfg::WriteSafe())
    return;
  for (int i = 0; i < G.playerCount; i++) {
    auto &p = G.players[i];
    if (!p.pawn || p.health <= 0)
      continue;
    if (p.team == G.localTeam && G.localTeam)
      continue;
    uintptr_t ss = p.pawn + Off::m_entitySpottedState;
    s_SpotSaves[s_SpotCount] = {ss,
                                SafeRead<uint32_t>(ss + Off::m_bSpottedByMask),
                                SafeRead<bool>(ss + Off::m_bSpotted)};
    s_SpotCount++;
    SafeWrite<bool>(ss + Off::m_bSpotted, true);
    SafeWrite<uint32_t>(ss + Off::m_bSpottedByMask, 0xFFFFFFFF);
  }
}

inline void RestoreSpotted() {
  for (int i = 0; i < s_SpotCount; i++) {
    auto &s = s_SpotSaves[i];
    SafeWrite<bool>(s.ss + Off::m_bSpotted, s.spotted);
    SafeWrite<uint32_t>(s.ss + Off::m_bSpottedByMask, s.mask);
  }
  s_SpotCount = 0;
}

//
// BHOP
//
inline void RunBhop() {
  if (!Cfg::bhopOn || Cfg::menuOpen || !G.client || !G.localPawn)
    return;
  bool held = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
  uint32_t flags = SafeRead<uint32_t>(G.localPawn + Off::m_fFlags);
  bool onGround = (flags & 1) != 0;

  if (!held) {
    SafeWrite<int>(G.client + Off::dwForceJump, 4);
    return;
  }

  // Sub-tick
  SafeWrite<int>(G.client + Off::dwForceJump, onGround ? 65537 : 4);

  // Auto-Strafe
  // Havadayken
  if (!onGround && Cfg::autoStrafe) {
    Vec3 vel = SafeRead<Vec3>(G.localPawn + Off::m_vecVelocity);
    float speed2D = sqrtf(vel.x * vel.x + vel.y * vel.y);
    if (speed2D > 10.f) {
      // View
      uintptr_t va = G.client + Off::dwViewAngles;
      float yaw  = SafeRead<float>(va + 4); // QAngle
      float yawR = yaw * 0.01745329f;

      // Velocity
      float velYaw = atan2f(vel.y, vel.x);

      // Yaw
      float diff = yawR - velYaw;
      // normalize
      while (diff >  3.14159f) diff -= 6.28318f;
      while (diff < -3.14159f) diff += 6.28318f;

      // Sa
      if (diff > 0.f) {
        // Sola
        SafeWrite<int>(G.client + Off::dwForceLeft,  65537);
        SafeWrite<int>(G.client + Off::dwForceRight, 4);
      } else {
        // Sa
        SafeWrite<int>(G.client + Off::dwForceRight, 65537);
        SafeWrite<int>(G.client + Off::dwForceLeft,  4);
      }
    }
  }
}

//
// SPECTATOR
//
struct SpectatorInfo {
    char name[32];
    int targetSlot;
};

inline void GetSpectators(SpectatorInfo* list, int& count) {
    count = 0;
    if (!G.client || !G.localPawn) return;
    for (int i = 0; i < G.playerCount; i++) {
        auto& p = G.players[i];
        if (p.health > 0 || p.pawn == G.localPawn) continue;
        uintptr_t obsTarget = SafeRead<uintptr_t>(p.pawn + Off::m_hObserverTarget);
        if (obsTarget == G.localPawn) {
            strncpy_s(list[count].name, p.name, 31);
            list[count].targetSlot = i;
            count++;
        }
    }
}

//
// MOUSE
//
inline void MoveMouse(int dx, int dy) {
  INPUT inp{};
  inp.type = INPUT_MOUSE;
  inp.mi.dx = dx;
  inp.mi.dy = dy;
  inp.mi.dwFlags = MOUSEEVENTF_MOVE;
  SendInput(1, &inp, sizeof(INPUT));
}
inline void MouseDown() {
  INPUT i{};
  i.type = INPUT_MOUSE;
  i.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  SendInput(1, &i, sizeof(INPUT));
}
inline void MouseUp() {
  INPUT i{};
  i.type = INPUT_MOUSE;
  i.mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(1, &i, sizeof(INPUT));
}
inline void ClickMouse() {
  INPUT i[2]{};
  i[0].type = i[1].type = INPUT_MOUSE;
  i[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
  i[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
  SendInput(2, i, sizeof(INPUT));
}

//
// BONE
//
inline Vec3 BonePos(const PlayerSnap &p, int b) {
  switch (b) {
  case 0:
    return p.boneHead;
  case 1:
    return p.boneNeck;
  case 2:
    return p.boneChest;
  case 3:
    return p.bonePelvis;
  default:
    return p.boneHead;
  }
}

//
// AIMBOT
//
static int s_AimSlot = -1;
static ULONGLONG s_LastShot = 0;

inline void RunAimbot(bool on, int key, bool needKey, int bone, float fov,
                      float smooth, float dz, bool lock, bool isRage,
                      bool autoFire, bool shootTeam) {
  if (!on) { s_AimSlot = -1; return; }
  if (needKey && key && !(GetAsyncKeyState(key) & 0x8000)) { s_AimSlot = -1; return; }
  if (Cfg::menuOpen) return;

  Vec2 center = {G.W * 0.5f, G.H * 0.5f};
  const PlayerSnap *tgt = nullptr;

  // Kilitli
  if (lock && s_AimSlot >= 0) {
    for (int i = 0; i < G.playerCount; i++) {
      if (G.players[i].slot == s_AimSlot && G.players[i].health > 0) {
        Vec2 s;
        if (W2S(BonePos(G.players[i], bone), s, G.vm, G.W, G.H) &&
            Dist2D(s, center) < fov * 2.5f)
          tgt = &G.players[i];
        break;
      }
    }
  }

  if (!tgt) {
    // Yakinlik
    if (Cfg::aimProxOn) {
      float bestDist3D = Cfg::aimProxDist * 64.f; // mt
      for (int i = 0; i < G.playerCount; i++) {
        auto &p = G.players[i];
        if (p.health <= 0) continue;
        if (!shootTeam && p.team == G.localTeam && G.localTeam) continue;
        if (Cfg::aimWall && !p.isVisible) continue;
        Vec2 s;
        if (!W2S(BonePos(p, bone), s, G.vm, G.W, G.H)) continue;
        if (Dist2D(s, center) > fov) continue; // hala
        if (p.dist < bestDist3D) { bestDist3D = p.dist; tgt = &p; }
      }
    } else {
      // Normal
      float best = 1e9f;
      for (int i = 0; i < G.playerCount; i++) {
        auto &p = G.players[i];
        if (p.health <= 0) continue;
        if (!shootTeam && p.team == G.localTeam && G.localTeam) continue;
        if (Cfg::aimWall && !p.isVisible) continue;
        Vec2 s;
        if (!W2S(BonePos(p, bone), s, G.vm, G.W, G.H)) continue;
        float d = Dist2D(s, center);
        if (d > fov || d >= best) continue;
        best = d; tgt = &p;
      }
    }
  }

  // Hedef
  static int s_StabSlot  = -1;
  static int s_StabCount = 0;
  if (!tgt) {
    s_StabSlot = -1; s_StabCount = 0;
    s_AimSlot  = -1; return;
  }
  if (tgt->slot != s_StabSlot) {
    s_StabSlot = tgt->slot; s_StabCount = 0;
  }
  s_StabCount++;
  if (s_StabCount < 3 && !isRage) { // rage
    // Hen
    return;
  }
  s_AimSlot = tgt->slot;

  Vec2 sc;
  if (!W2S(BonePos(*tgt, bone), sc, G.vm, G.W, G.H)) return;

  float dx = sc.x - center.x, dy = sc.y - center.y;
  float dist = sqrtf(dx * dx + dy * dy);

  // Akim
  if (dist > 400.f) return;

  if (dist < dz) {
    if (autoFire && isRage) {
      ULONGLONG n = GetTickCount64();
      if (n - s_LastShot >= 50) { ClickMouse(); s_LastShot = n; }
    }
    return;
  }
  float sm = smooth;
  if (!isRage) {
    float t = dist / fov;
    sm = smooth + (smooth * 2.f) * (1.f - t);
    if (sm < 1.f) sm = 1.f;
  }
  int ix = (int)(dx / sm), iy = (int)(dy / sm);
  if (!ix && fabsf(dx) > 0.3f) ix = (dx > 0) ? 1 : -1;
  if (!iy && fabsf(dy) > 0.3f) iy = (dy > 0) ? 1 : -1;
  if (ix || iy) MoveMouse(ix, iy);

  if (autoFire && isRage && dist < 25.f) {
    ULONGLONG n = GetTickCount64();
    if (n - s_LastShot >= 50) { ClickMouse(); s_LastShot = n; }
  }
}


//
// TRIGGERBOT
//
enum class TrigState { IDLE, WAITING, PRESSING, RELEASING };
static TrigState s_TrigState = TrigState::IDLE;
static ULONGLONG s_TrigTimer = 0;
static constexpr ULONGLONG TRIG_PRESS_MS = 35;

static bool TrigCheckTarget() {
  if (!G.localPawn || !G.client)
    return false;
  uintptr_t el = SafeRead<uintptr_t>(G.client + Off::dwEntityList);
  if (!el)
    return false;
  int idx = SafeRead<int>(G.localPawn + Off::m_iIDEntIndex);
  if (idx <= 0 || idx >= 0x7FFF)
    return false;
  uintptr_t ent = GetEntity(el, idx);
  if (!ent || ent <= 0x10000)
    ent = GetPawnByHandle(el, (uint32_t)idx);
  if (!ent || ent <= 0x10000)
    return false;
  int hp = SafeRead<int>(ent + Off::m_iHealth);
  if (hp <= 0 || hp > 200)
    hp = SafeRead<int>(ent + Off::m_iPawnHealth);
  if (hp <= 0 || hp > 200)
    return false;
  int tm = (int)SafeRead<uint8_t>(ent + Off::m_iTeamNum);
  if (tm == 0)
    return false;
  if (!Cfg::trigShootTeam && tm == G.localTeam)
    return false;
  if (!Cfg::trigWall) {
    bool vis = false;
    for (int j = 0; j < G.playerCount; j++)
      if (G.players[j].pawn == ent) {
        vis = G.players[j].isVisible;
        break;
      }
    if (!vis)
      return false;
  }
  return true;
}

inline void RunTrigger() {
  if (!Cfg::trigOn || Cfg::menuOpen ||
      (Cfg::trigNeedKey && Cfg::trigKey &&
       !(GetAsyncKeyState(Cfg::trigKey) & 0x8000))) {
    if (s_TrigState == TrigState::PRESSING)
      MouseUp();
    s_TrigState = TrigState::IDLE;
    s_TrigTimer = 0;
    return;
  }
  ULONGLONG now = GetTickCount64();
  ULONGLONG delay = (Cfg::trigDelay > 0.f && Cfg::trigDelay < 10000.f)
                        ? (ULONGLONG)Cfg::trigDelay
                        : 0ULL;
  switch (s_TrigState) {
  case TrigState::IDLE:
    if (now < s_TrigTimer)
      return;
    if (TrigCheckTarget()) {
      s_TrigState = TrigState::WAITING;
      s_TrigTimer = now + delay;
    }
    break;
  case TrigState::WAITING:
    if (!TrigCheckTarget()) {
      s_TrigState = TrigState::IDLE;
      return;
    }
    if (now >= s_TrigTimer) {
      MouseDown();
      s_TrigState = TrigState::PRESSING;
      s_TrigTimer = now + TRIG_PRESS_MS;
    }
    break;
  case TrigState::PRESSING:
    if (now >= s_TrigTimer) {
      MouseUp();
      s_TrigState = TrigState::RELEASING;
      s_TrigTimer = now + TRIG_PRESS_MS;
    }
    break;
  case TrigState::RELEASING:
    if (now >= s_TrigTimer) {
      s_TrigState = TrigState::IDLE;
      s_TrigTimer = now + delay;
    }
    break;
  }
}

//
// NO
//
inline void RunNoFlash() {
  // WriteSafe
  if (!Cfg::noFlash || !Cfg::WriteSafe() || G.localPawn <= 0x10000)
    return;
  SafeWrite<float>(G.localPawn + Off::m_flFlashDur, 0.f);
  SafeWrite<float>(G.localPawn + Off::m_flFlashDur + 4, 0.f);
  SafeWrite<float>(G.localPawn + Off::m_flFlashDur + 8, 0.f);
}



//
// ANTI-AIM
//
static float s_aaSpinAngle = 0.f;
inline void RunAntiAim() {
  if (!Cfg::aaOn || Cfg::menuOpen || !G.client)
    return;
  if (!Cfg::WriteSafe() || !G.localPawn)
    return;
  if (Cfg::aaKey && !(GetAsyncKeyState(Cfg::aaKey) & 0x8000))
    return;
  bool aimbotEngaged = (s_AimSlot >= 0) && (Cfg::aimOn || Cfg::rageOn);
  if (aimbotEngaged) {
    s_aaSpinAngle = 0.f;
    return;
  }
  if (SafeRead<int>(G.localPawn + Off::m_iHealth) <= 0) {
    s_aaSpinAngle = 0.f;
    return;
  }
  uintptr_t va = G.client + Off::dwViewAngles;
  float yaw = SafeRead<float>(va + 4);
  switch (Cfg::aaMode) {
  case 0:
    s_aaSpinAngle += 15.f;
    if (s_aaSpinAngle >= 360.f)
      s_aaSpinAngle -= 360.f;
    SafeWrite<float>(va + 4, yaw + s_aaSpinAngle);
    break;
  case 1:
    SafeWrite<float>(va + 4, yaw + Cfg::aaYawAdd);
    break;
  case 2: {
    static bool fl = false;
    fl = !fl;
    SafeWrite<float>(va + 4, yaw + (fl ? 35.f : -35.f));
    break;
  }
  case 3:
    SafeWrite<float>(va, 89.f);
    SafeWrite<float>(va + 4, yaw + Cfg::aaYawAdd);
    break;
  case 4:
    SafeWrite<float>(va, -89.f);
    SafeWrite<float>(va + 4, yaw + Cfg::aaYawAdd);
    break;
  }
}



//
// GLOW
//
inline void WriteGlow(uintptr_t pawn, float r, float g, float b, int type) {
  if (!pawn || pawn <= 0x10000)
    return;
  uintptr_t gb = pawn + Off::m_Glow;
  SafeWrite<float>(gb + Off::m_glowColorR, r);
  SafeWrite<float>(gb + Off::m_glowColorG, g);
  SafeWrite<float>(gb + Off::m_glowColorB, b);
  SafeWrite<float>(gb + Off::m_glowColorA, 1.f);
  uint32_t pk = 0xFF000000u | ((uint8_t)(b * 255) << 16) |
                ((uint8_t)(g * 255) << 8) | (uint8_t)(r * 255);
  SafeWrite<uint32_t>(gb + Off::m_glowColorPacked, pk);
  SafeWrite<int>(gb + Off::m_glowType, type);
  SafeWrite<uint8_t>(gb + Off::m_bUseCustomGlowColor, 1);
  SafeWrite<uint8_t>(gb + Off::m_bGlowing, 1);
  SafeWrite<uint8_t>(gb + Off::m_bGlowingAlt, 1);
}
inline void RunGlow() {
  if (!Cfg::glowOn || !Cfg::WriteSafe())
    return;
  // Chams
  if (Cfg::chamsOn)
    return;
  if (Cfg::glowKey && !(GetAsyncKeyState(Cfg::glowKey) & 0x8000))
    return;
  for (int i = 0; i < G.playerCount; i++) {
    auto &p = G.players[i];
    if (!p.pawn || p.health <= 0)
      continue;
    if (!Cfg::glowTeam && p.team == G.localTeam && G.localTeam)
      continue;
    float *c = p.isVisible ? Cfg::glowVis : Cfg::glowWall;
    WriteGlow(p.pawn, c[0], c[1], c[2], 3);
  }
}

// Chams
// WC
inline void GetChamsRainbow(float out[3]) {
  float spd = Cfg::chamsRainbowSpd < 0.01f ? 0.01f : Cfg::chamsRainbowSpd;
  double t = (double)GetTickCount64() / 1000.0;
  if(!Cfg::chamsRainbowCustom) {
    // Oto
    float hue = fmodf((float)(t * spd), 1.0f);
    // HSV-
    float h=hue; h=h-floorf(h);
    int i=(int)(h*6.f); float f=h*6.f-i;
    float q=1.f*(1.f-f), t2=1.f*(1.f-(1.f-f));
    switch(i%6){
      case 0:out[0]=1.f;out[1]=t2; out[2]=0.f;break;
      case 1:out[0]=q;  out[1]=1.f;out[2]=0.f;break;
      case 2:out[0]=0.f;out[1]=1.f;out[2]=t2; break;
      case 3:out[0]=0.f;out[1]=q;  out[2]=1.f;break;
      case 4:out[0]=t2; out[1]=0.f;out[2]=1.f;break;
      default:out[0]=1.f;out[1]=0.f;out[2]=q; break;
    }
  } else {
    // Ozel
    int n=Cfg::chamsRainbowCnt<2?2:(Cfg::chamsRainbowCnt>4?4:Cfg::chamsRainbowCnt);
    float cycleLen=1.0f/spd;
    float pos=fmodf((float)t/cycleLen,1.0f)*(float)n;
    int i0=(int)pos%n, i1=(i0+1)%n;
    float fr=pos-floorf(pos);
    fr=fr*fr*(3.f-2.f*fr); // smoothstep
    out[0]=Cfg::chamsRainbowCols[i0][0]+(Cfg::chamsRainbowCols[i1][0]-Cfg::chamsRainbowCols[i0][0])*fr;
    out[1]=Cfg::chamsRainbowCols[i0][1]+(Cfg::chamsRainbowCols[i1][1]-Cfg::chamsRainbowCols[i0][1])*fr;
    out[2]=Cfg::chamsRainbowCols[i0][2]+(Cfg::chamsRainbowCols[i1][2]-Cfg::chamsRainbowCols[i0][2])*fr;
  }
}


//
// CHAMS
//
//
// SPECTATOR
//
struct SpectatorSnap {
  char  name[64] = {};
  int   mode     = 0;   // ObsMode
  bool  valid    = false;
};
inline SpectatorSnap g_Specs[64];
inline int           g_SpecCount = 0;

inline void ScanSpectators() {
  g_SpecCount = 0;
  if (!G.client || !G.localPawn || G.localPawn <= 0x10000) return;
  uintptr_t el = SafeRead<uintptr_t>(G.client + Off::dwEntityList);
  if (!el || el <= 0x10000) return;

  for (int i = 1; i <= 64 && g_SpecCount < 64; i++) {
    uintptr_t ctrl = GetEntity(el, i);
    if (!ctrl || ctrl <= 0x10000) continue;

    // Controller
    uint32_t ph = SafeRead<uint32_t>(ctrl + Off::m_hPlayerPawn);
    if (!ph || ph == 0xFFFFFFFF) continue;
    uintptr_t pawn = GetPawnByHandle(el, ph);
    if (!pawn || pawn <= 0x10000 || pawn == G.localPawn) continue;

    // Sadece
    uint8_t ls = SafeRead<uint8_t>(pawn + Off::m_lifeState);
    if (ls == 0) continue; // Canl

    // ObserverServices
    uintptr_t obsSvc = SafeRead<uintptr_t>(pawn + Off::m_pObserverServices);
    if (!obsSvc || obsSvc <= 0x10000) continue;

    // Hedef
    uint32_t tgtHandle = SafeRead<uint32_t>(obsSvc + Off::m_hObserverTarget);
    if (!tgtHandle || tgtHandle == 0xFFFFFFFF) continue;
    uintptr_t tgtPawn = GetPawnByHandle(el, tgtHandle);
    if (tgtPawn != G.localPawn) continue; // Bizi

    // Observer
    uint8_t mode = SafeRead<uint8_t>(obsSvc + Off::m_iObserverMode);

    // sim
    SpectatorSnap& ss = g_Specs[g_SpecCount++];
    SafeReadStr(ctrl + Off::m_iszPlayerName, ss.name, 64);
    if (!ss.name[0]) { ss.name[0]='?'; ss.name[1]='\0'; }
    ss.mode  = (int)mode;
    ss.valid = true;
  }
}

inline void RunChams() {
  // Tum
}

//
// RUN
//
inline void RunFeatures() {
  if (G.localPawn > 0x10000) {
    if (!Cfg::firstPawnSeen)
      Cfg::firstPawnSeen = GetTickCount64();
  } else {
    Cfg::firstPawnSeen = 0;
  }

  RunBhop();
  RunNoFlash();
  RunAntiAim();
  RunGlow();
  RunChams();
  RunTrigger();
  RunRadarHack();
  RunAimbot(Cfg::aimOn, Cfg::aimKey, Cfg::aimNeedKey, Cfg::aimBone, Cfg::aimFov,
            Cfg::aimSmooth, Cfg::aimDz, Cfg::aimLock, false, false,
            Cfg::aimShootTeam);
  RunAimbot(Cfg::rageOn, Cfg::rageKey, Cfg::rageNeedKey, Cfg::rageBone,
            Cfg::rageFov, Cfg::rageSmooth, 0.f, false, true, Cfg::rageAuto,
            Cfg::rageShootTeam);
}
