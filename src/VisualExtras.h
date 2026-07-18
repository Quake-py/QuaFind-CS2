#pragma once
// VisualExtras.h
#include "SDK.h"
#include "Features.h"
#include <cmath>
#include <vector>
#include <algorithm>

namespace VisOff {
  constexpr uintptr_t m_flLastSmokeOverlayAlpha = 0x1420;
  constexpr uintptr_t m_bSmokeEffectSpawned     = 0x1299;
  constexpr uintptr_t m_pClientAlphaProperty    = 0xF50;
  constexpr uintptr_t m_nAlpha                  = 0x17;
}

namespace VisExt {

// Sky
inline void DrawSkyOverlay(float sw, float sh) {
  if (!Cfg::skyColorOn) return;
  ImDrawList* dl = ImGui::GetBackgroundDrawList(); if (!dl) return;
  float* c = Cfg::skyColor; float a = Cfg::skyAlpha;
  ImU32 colT = IM_COL32((int)(c[0]*255),(int)(c[1]*255),(int)(c[2]*255),(int)(a*255));
  ImU32 colB = IM_COL32((int)(c[0]*255),(int)(c[1]*255),(int)(c[2]*255),0);
  dl->AddRectFilledMultiColor({0.f,0.f},{sw,sh*Cfg::skyHeight},colT,colT,colB,colB);
}

// No
static UINT64 g_lastSmokeTick = 0;
inline void RunNoSmoke(uintptr_t client, uintptr_t lp) {
  if (!Cfg::noSmoke || !client) return;
  __try {
    if (lp > 0x10000)
      *reinterpret_cast<float*>(lp + VisOff::m_flLastSmokeOverlayAlpha) = 0.f;
    UINT64 now = GetTickCount64();
    if (now - g_lastSmokeTick < 100) return; g_lastSmokeTick = now;
    uintptr_t el = SafeRead<uintptr_t>(client + Off::dwEntityList); if (!el) return;
    for (int i = 0; i < 2048; i++) {
      uintptr_t ck = SafeRead<uintptr_t>(el + 8*((i&0x7FFF)>>9)+16); if (!ck) continue;
      uintptr_t ent = SafeRead<uintptr_t>(ck+112*(i&0x1FF)); if(!ent||ent<0x10000) continue;
      __try {
        if (!*reinterpret_cast<uint8_t*>(ent+VisOff::m_bSmokeEffectSpawned)) continue;
        uintptr_t ap = SafeRead<uintptr_t>(ent+VisOff::m_pClientAlphaProperty);
        if (ap>0x10000) *reinterpret_cast<uint8_t*>(ap+VisOff::m_nAlpha)=0;
      } __except(EXCEPTION_EXECUTE_HANDLER) {}
    }
  } __except(EXCEPTION_EXECUTE_HANDLER) {}
}

// Particles
struct Particle {
  float x, y, vx, vy, size, lifetime, maxlife, phase;
  float rot, spin;
  ImU32 color; int type; // 0=Snow 1=Sakura
  Vec3 worldPos; bool is3D;
  Vec3 worldVel;
};

static std::vector<Particle> g_parts;
static bool g_prevWas3D = false;

// Yardimcilar
static float VR01() { return (float)rand() / (float)RAND_MAX; }
static float VR(float lo, float hi) { return lo + (hi-lo)*VR01(); }
static ImU32 VA(ImU32 col, float a) {
  int ai=(int)(a*255.f); if(ai<0)ai=0; if(ai>255)ai=255;
  return (col&0x00FFFFFFu)|((ImU32)ai<<24);
}
static void VQ(ImDrawList* dl, ImVec2 center, float w, float h, float angle, ImU32 col) {
  float c=cosf(angle), s=sinf(angle);
  ImVec2 pts[4]={
    {center.x+(-w*c-h*s), center.y+(-w*s+h*c)},
    {center.x+( w*c-h*s), center.y+( w*s+h*c)},
    {center.x+( w*c+h*s), center.y+( w*s-h*c)},
    {center.x+(-w*c+h*s), center.y+(-w*s-h*c)}
  };
  dl->AddConvexPolyFilled(pts,4,col);
}

static constexpr int MAX_PARTICLES = 1500;

// 3D dunya icin yeniden dogum
static void SpawnWorld(Particle& p, const Vec3& origin, float worldFloor, float worldHeight, float worldRadius) {
  float ang=VR(0.f,6.2831853f);
  float rad=sqrtf(VR01())*worldRadius;
  p.worldPos={origin.x+cosf(ang)*rad, origin.y+sinf(ang)*rad, origin.z+worldFloor+VR(0.f,worldHeight)};
  p.rot=VR(0.f,6.2831853f); p.phase=VR(0.f,6.2831853f);
  if(p.type==1) p.worldVel={VR(-8.f,8.f),VR(-8.f,8.f),VR(-45.f,-15.f)};
  else          p.worldVel={VR(-5.f,5.f),VR(-5.f,5.f),VR(-70.f,-30.f)};
}

// Genel spawn
static void SpawnParticle(Particle& p, int type, float sw, float sh, bool fill, bool is3D, const Vec3& origin, float worldFloor, float worldHeight, float worldRadius) {
  p.size=VR(1.5f,3.5f); p.maxlife=VR(4.f,9.f);
  p.lifetime=fill?VR(0.f,p.maxlife):p.maxlife;
  p.phase=VR(0.f,6.28f); p.rot=VR(0.f,6.28f); p.spin=VR(-1.5f,1.5f);
  p.type=type; p.is3D=is3D;
  if(type==0) p.color=IM_COL32(255,255,255,200);
  else{ float*c=Cfg::sakuraColor; p.color=IM_COL32((int)(c[0]*255),(int)(c[1]*255),(int)(c[2]*255),200); }
  if(is3D){ SpawnWorld(p,origin,worldFloor,worldHeight,worldRadius); }
  else {
    p.x=VR(0.f,sw); p.y=fill?VR(0.f,sh):VR(-20.f,0.f);
    p.vx=VR(-10.f,10.f); p.vy=(type==0)?VR(60.f,120.f):VR(40.f,85.f);
    p.worldPos={0.f,0.f,0.f}; p.worldVel={0.f,0.f,0.f};
  }
}

inline void DrawParticles(float dt, const Mat4& vm, const Vec3& origin, int SW, int SH) {
  if (!Cfg::snowOn && !Cfg::sakuraOn) { g_parts.clear(); g_prevWas3D=false; return; }

  ImGuiIO& io=ImGui::GetIO();
  float sw=(io.DisplaySize.x>100.f)?io.DisplaySize.x:(float)SW;
  float sh=(io.DisplaySize.y>100.f)?io.DisplaySize.y:(float)SH;
  if(sw<100.f||sh<100.f) return;

  static bool seeded=false;
  if(!seeded){srand((unsigned)(GetTickCount64()^(UINT64)(uintptr_t)&seeded));seeded=true;}

  // 3D mod
  bool is3D=Cfg::particle3D&&(SW>0)&&(origin.x!=0.f||origin.y!=0.f||origin.z!=0.f);
  if(is3D!=g_prevWas3D){g_parts.clear();g_prevWas3D=is3D;}

  if(g_parts.capacity()<MAX_PARTICLES) g_parts.reserve(MAX_PARTICLES);
  ImDrawList* dl=ImGui::GetBackgroundDrawList(); if(!dl) return;

  // 3D parametreler (referans ile ayni)
  float worldRadius=1200.f, worldHeight=600.f, worldFloor=-80.f;
  float wind=15.f, depthFade=0.0015f;
  Vec3 windVec{wind, wind*0.35f, 0.f};

  // Hedef sayilar
  int snowRate=(Cfg::snowDensity==0?25:(Cfg::snowDensity==1?60:120));
  int sakuraRate=40;
  static float snowAcc=0.f, sakuraAcc=0.f;
  if(Cfg::snowOn)   snowAcc+=dt*(float)snowRate;
  if(Cfg::sakuraOn) sakuraAcc+=dt*(float)sakuraRate;

  auto spawn=[&](int count,int type){
    for(int i=0;i<count;i++){
      if((int)g_parts.size()>=MAX_PARTICLES) break;
      g_parts.emplace_back();
      SpawnParticle(g_parts.back(),type,sw,sh,false,is3D,origin,worldFloor,worldHeight,worldRadius);
    }
  };

  // Ilk dolum
  if(g_parts.empty()){
    int initSnow=Cfg::snowOn?(Cfg::snowDensity==0?100:(Cfg::snowDensity==1?250:500)):0;
    int initSak=Cfg::sakuraOn?150:0;
    for(int i=0;i<initSnow;i++){
      if((int)g_parts.size()>=MAX_PARTICLES) break;
      g_parts.emplace_back();
      SpawnParticle(g_parts.back(),0,sw,sh,true,is3D,origin,worldFloor,worldHeight,worldRadius);
    }
    for(int i=0;i<initSak;i++){
      if((int)g_parts.size()>=MAX_PARTICLES) break;
      g_parts.emplace_back();
      SpawnParticle(g_parts.back(),1,sw,sh,true,is3D,origin,worldFloor,worldHeight,worldRadius);
    }
  }

  while(snowAcc>=1.f)  {spawn(1,0);snowAcc-=1.f;}
  while(sakuraAcc>=1.f){spawn(1,1);sakuraAcc-=1.f;}

  // Guncelle
  for(auto& p:g_parts){
    p.lifetime-=dt;
    if(p.is3D){
      p.phase+=dt; p.rot+=p.spin*dt;
      float sway=(p.type==1?12.f:6.f)*sinf(p.phase+p.rot);
      p.worldPos.x+=(p.worldVel.x+windVec.x+sway)*dt;
      p.worldPos.y+=(p.worldVel.y+windVec.y+(p.type==1?-sway*0.4f:0.f))*dt;
      p.worldPos.z+=p.worldVel.z*dt;
      if(p.type==0) p.worldVel.z-=8.f*dt;
      else          p.worldVel.z-=5.f*dt;
      float baseZ=origin.z+worldFloor;
      if(p.worldPos.z<baseZ) SpawnWorld(p,origin,worldFloor,worldHeight,worldRadius);
      float dx=p.worldPos.x-origin.x, dy=p.worldPos.y-origin.y;
      float maxR=worldRadius*1.35f;
      if(dx*dx+dy*dy>maxR*maxR) SpawnWorld(p,origin,worldFloor,worldHeight,worldRadius);
    } else {
      p.x+=p.vx*dt; p.y+=p.vy*dt;
      if(p.type==1) p.vx+=sinf(p.phase+p.lifetime)*5.f*dt;
    }
  }

  // Oluleri temizle
  g_parts.erase(std::remove_if(g_parts.begin(),g_parts.end(),[&](const Particle& p){
    if(p.lifetime<=0.f) return true;
    if(p.type==0&&!Cfg::snowOn) return true;
    if(p.type==1&&!Cfg::sakuraOn) return true;
    if(!p.is3D&&(p.x<-20.f||p.x>sw+20.f||p.y>sh+20.f)) return true;
    return false;
  }),g_parts.end());

  // Ciz
  for(const auto& p:g_parts){
    float x=p.x, y=p.y;
    if(p.is3D){
      Vec2 sc;
      if(!W2S({p.worldPos.x,p.worldPos.y,p.worldPos.z},sc,vm,SW,SH)) continue;
      x=sc.x; y=sc.y;
    }
    float life=fmaxf(0.f,fminf(p.lifetime/p.maxlife,1.f));
    float depth=1.f;
    if(p.is3D){
      float dx2=p.worldPos.x-origin.x,dy2=p.worldPos.y-origin.y,dz2=p.worldPos.z-origin.z;
      float dist=sqrtf(dx2*dx2+dy2*dy2+dz2*dz2);
      depth=1.f/(1.f+dist*depthFade);
      depth=fmaxf(0.25f,fminf(depth,1.2f));
    }
    float alpha=fmaxf(0.f,fminf(life*depth,1.f));
    float t3d=p.is3D?fmaxf(0.f,fminf(depth,1.f)):0.f;
    float sz=p.size*(p.is3D?(0.65f+(1.4f-0.65f)*t3d):1.f);
    if(p.type==0){
      dl->AddCircleFilled({x,y},sz*1.6f,VA(p.color,alpha*0.35f),12);
      dl->AddCircleFilled({x,y},sz,      VA(p.color,alpha),       12);
    } else {
      VQ(dl,{x,y},sz*2.2f,sz*1.2f, p.rot,      VA(p.color,alpha*0.9f));
      VQ(dl,{x,y},sz*1.4f,sz*0.8f, p.rot+0.8f, VA(p.color,alpha*0.55f));
    }
  }
}

} // namespace
