#pragma once
// HitMarker.h
// Tek
#include "SDK.h"

namespace HM {

// Konfig
inline bool  hmOn      = false;
inline float hmSize    = 10.f;   // X
inline float hmThick   = 2.0f;   // cizgi
inline float hmGap     = 3.f;    // ortadaki
inline float hmCol[3]  = {1.f, 0.22f, 0.22f}; // varsayilan

// Shot
static int   s_lastFired  = 0;
static float s_shotWindow = 0.f;
static const float kWin   = 0.55f; // 550ms

// Animasyon
struct HitAnim { float timer; };
static constexpr int MAX_HIT = 8;
static HitAnim g_hits[MAX_HIT] = {};
static int     g_hitCount = 0;
static const float kDur   = 0.32f;

// Vurus
static void TriggerHit() {
  if (!hmOn) return;
  if (g_hitCount < MAX_HIT)
    g_hits[g_hitCount++] = {kDur};
}

// HP
static constexpr int HC = 64;
static struct { uintptr_t pawn; int hp; } g_hp[HC] = {};
static int g_hpN = 0;

// ESP
inline void UpdateEnemy(uintptr_t pawn, int curHp) {
  for(int i = 0; i < g_hpN; i++){
    if(g_hp[i].pawn == pawn){
      int prev = g_hp[i].hp;
      g_hp[i].hp = curHp;
      if(curHp < prev && s_shotWindow > 0.f)
        TriggerHit();
      return;
    }
  }
  if(g_hpN < HC) g_hp[g_hpN++] = {pawn, curHp};
}

// Her
inline void Tick(float dt) {
  if(s_shotWindow > 0.f) s_shotWindow -= dt;
  if(G.localPawn > 0x10000){
    int fired = SafeRead<int>(G.localPawn + Off::m_iShotsFired);
    if(fired > s_lastFired) s_shotWindow = kWin;
    s_lastFired = fired;
  }
}

inline void ClearCache(){ g_hpN = 0; }

// Render
// Animasyon
inline void Render(float W, float H, float dt) {
  if (!hmOn) return;
  float cx = W * .5f, cy = H * .5f;
  ImDrawList* fg = ImGui::GetForegroundDrawList();
  int alive = 0;
  for(int i = 0; i < g_hitCount; i++){
    g_hits[i].timer -= dt;
    if(g_hits[i].timer <= 0.f) continue;

    float t   = 1.f - (g_hits[i].timer / kDur); // 0
    float a   = (1.f - t) * 255.f;               // alfa
    float sz  = hmSize + t * 4.f;                // hafif
    float gap = hmGap;
    float th  = hmThick;

    ImU32 c = IM_COL32(
      (int)(hmCol[0]*255),
      (int)(hmCol[1]*255),
      (int)(hmCol[2]*255),
      (int)a
    );
    // Golgeli
    ImU32 shadow = IM_COL32(0, 0, 0, (int)(a * 0.45f));
    float so = 1.2f;

    // Sol-ust
    fg->AddLine({cx-sz-gap+so, cy-sz-gap+so}, {cx-gap+so, cy-gap+so}, shadow, th+1.5f);
    fg->AddLine({cx-sz-gap,    cy-sz-gap   }, {cx-gap,    cy-gap   }, c, th);

    fg->AddLine({cx+gap+so, cy+gap+so}, {cx+sz+gap+so, cy+sz+gap+so}, shadow, th+1.5f);
    fg->AddLine({cx+gap,    cy+gap   }, {cx+sz+gap,    cy+sz+gap   }, c, th);

    // Sag-ust
    fg->AddLine({cx+sz+gap+so, cy-sz-gap+so}, {cx+gap+so, cy-gap+so}, shadow, th+1.5f);
    fg->AddLine({cx+sz+gap,    cy-sz-gap   }, {cx+gap,    cy-gap   }, c, th);

    fg->AddLine({cx-gap+so, cy+gap+so}, {cx-sz-gap+so, cy+sz+gap+so}, shadow, th+1.5f);
    fg->AddLine({cx-gap,    cy+gap   }, {cx-sz-gap,    cy+sz+gap   }, c, th);

    g_hits[alive++] = g_hits[i];
  }
  g_hitCount = alive;
}

} // namespace
