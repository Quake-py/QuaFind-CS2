#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <windows.h>

//
// SAFE
//
template <typename T> inline T SafeRead(uintptr_t addr) {
  T val{};
  if (!addr || addr < 0x10000) return val;
  __try { val = *(T*)addr; } __except(EXCEPTION_EXECUTE_HANDLER) {}
  return val;
}
template <typename T> inline void SafeWrite(uintptr_t addr, T val) {
  if (!addr || addr < 0x10000) return;
  __try { *(T*)addr = val; } __except(EXCEPTION_EXECUTE_HANDLER) {}
}
inline bool SafeReadBuf(uintptr_t addr, void* buf, size_t size) {
  if (!addr || addr < 0x10000 || !buf || !size) return false;
  SIZE_T rd = 0;
  return ReadProcessMemory(GetCurrentProcess(),(LPCVOID)addr,buf,size,&rd) && rd==size;
}
inline void SafeReadStr(uintptr_t addr, char* out, int maxLen) {
  if (!out || maxLen <= 1) return;
  memset(out, 0, maxLen);
  if (!addr || addr < 0x10000) return;
  // SEH
  __try {
    const char* src = (const char*)addr;
    for (int i = 0; i < maxLen-1; i++) {
      char c = src[i];
      out[i] = c;
      if (!c) break;
    }
    out[maxLen-1] = '\0';
  } __except(EXCEPTION_EXECUTE_HANDLER) {}
}

//
// MATH
//
struct Vec2 { float x=0,y=0; };
struct Vec3 {
  float x=0,y=0,z=0;
  float Len() const { return sqrtf(x*x+y*y+z*z); }
  Vec3 operator-(const Vec3& o) const { return {x-o.x,y-o.y,z-o.z}; }
  Vec3 operator+(const Vec3& o) const { return {x+o.x,y+o.y,z+o.z}; }
  Vec3 operator*(float f)       const { return {x*f,y*f,z*f}; }
};
struct Mat4 { float m[4][4]{}; };

inline float Dist2D(Vec2 a, Vec2 b) {
  float dx=a.x-b.x, dy=a.y-b.y;
  return sqrtf(dx*dx+dy*dy);
}
inline float Dist3D(Vec3 a, Vec3 b) {
  float dx=a.x-b.x, dy=a.y-b.y, dz=a.z-b.z;
  return sqrtf(dx*dx+dy*dy+dz*dz);
}
inline bool W2S(Vec3 world, Vec2& scr, const Mat4& vm, int W, int H) {
  float x = vm.m[0][0]*world.x + vm.m[0][1]*world.y + vm.m[0][2]*world.z + vm.m[0][3];
  float y = vm.m[1][0]*world.x + vm.m[1][1]*world.y + vm.m[1][2]*world.z + vm.m[1][3];
  float w = vm.m[3][0]*world.x + vm.m[3][1]*world.y + vm.m[3][2]*world.z + vm.m[3][3];
  if (w < 0.001f) return false;
  scr.x = (W*0.5f) + (x/w)*(W*0.5f);
  scr.y = (H*0.5f) - (y/w)*(H*0.5f);
  return true;
}

//
// OFFSETS
//
namespace Off {
// client.dll
inline uintptr_t dwEntityList = 0x254EE60;  // offsets.hpp
inline uintptr_t dwLocalPawn = 0x23A4238;  // offsets.hpp
inline uintptr_t dwLocalPawnCtrl = 0x237EBA0;  // offsets.hpp
inline uintptr_t dwViewMatrix = 0x23A9340;  // offsets.hpp
inline uintptr_t dwViewAngles = 0x23B9C78;  // offsets.hpp
inline uintptr_t dwCSGOInput = 0x23B95F0;  // offsets.hpp
inline uintptr_t dwForceJump = 0x2094490;  // buttons.hpp
inline uintptr_t dwForceAttack = 0x2093F80;  // buttons.hpp
inline uintptr_t dwGlowManager = 0x23A0708;  // offsets.hpp
inline uintptr_t dwPlantedC4 = 0x236E678;  // offsets.hpp

// Button
inline uintptr_t dwForceRight = 0x2094370;  // buttons.hpp
inline uintptr_t dwForceLeft = 0x20942E0;  // buttons.hpp
inline uintptr_t dwForceForward = 0x20941C0;  // buttons.hpp
inline uintptr_t dwForceBack = 0x2094250;  // buttons.hpp
inline uintptr_t dwForceAttack2 = 0x2094010;  // buttons.hpp

// CCSPlayerController
constexpr uintptr_t m_hPlayerPawn = 0x914;  // client_dll
constexpr uintptr_t m_bPawnIsAlive = 0x91C;
constexpr uintptr_t m_iPawnHealth = 0x920;
constexpr uintptr_t m_iszPlayerName = 0x6F4;  // client_dll

// C_BaseEntity
constexpr uintptr_t m_iHealth    = 0x34C;  // L2319
constexpr uintptr_t m_lifeState  = 0x354;  // L2321
constexpr uintptr_t m_iTeamNum = 0x3E7;  // L2353
constexpr uintptr_t m_fFlags = 0x3F4;  // L2356
constexpr uintptr_t m_vecVelocity= 0x430;  // L2359
constexpr uintptr_t m_bIsScoped = 0x1C70; // client_dll
constexpr uintptr_t m_iShotsFired = 0x1C84; // client_dll

// Flash
constexpr uintptr_t m_flFlashDur = 0x1428;
constexpr uintptr_t m_flFlashDuration = 0x1428;

// Crosshair
constexpr uintptr_t m_iIDEntIndex = 0x341C; // client_dll

// Spotted
constexpr uintptr_t m_entitySpottedState = 0x1CF8;  // client_dll
constexpr uintptr_t m_bSpotted = 0x8;
constexpr uintptr_t m_bSpottedByMask = 0xC;

// Punch
// AimPunchServices
constexpr uintptr_t m_pAimPunchSvc = 0x14B8; // C_CSPlayerPawn
constexpr uintptr_t m_aimPunchAngle    = 0x68;  // within
constexpr uintptr_t m_aimPunchAngleVel = 0x74;  // within
constexpr uintptr_t m_aimPunchCache    = 0x128; // within

// Spread
constexpr uintptr_t m_flAccPenalty = 0x17F0; // TODO

// Eye
constexpr uintptr_t m_angEyeAngles = 0x3340; // client_dll

// CGameSceneNode
constexpr uintptr_t m_pGameSceneNode = 0x330;  // C_BaseEntity
constexpr uintptr_t m_vecAbsOrigin   = 0xC8;   // CGameSceneNode
constexpr uintptr_t m_modelState = 0x140;  // client_dll
constexpr uintptr_t m_boneArray = 0x80;   // modelState

// CGlowProperty
constexpr uintptr_t m_Glow = 0xDE0; // client_dll
constexpr uintptr_t m_glowColorR        = 0x08;  // CGlowProperty
constexpr uintptr_t m_glowColorG        = 0x0C;  // CGlowProperty
constexpr uintptr_t m_glowColorB        = 0x10;  // CGlowProperty
constexpr uintptr_t m_glowColorA        = 0x14;
constexpr uintptr_t m_glowType          = 0x30;  // CGlowProperty
constexpr uintptr_t m_iGlowType         = 0x30;
constexpr uintptr_t m_glowColorPacked   = 0x40;  // CGlowProperty
constexpr uintptr_t m_glowColorOverride = 0x40;
constexpr uintptr_t m_bUseCustomGlowColor = 0x44; // m_bFlashing
constexpr uintptr_t m_bGlowing            = 0x51; // CGlowProperty
constexpr uintptr_t m_bGlowingAlt         = 0x51;

// Model
constexpr uintptr_t m_nRenderMode           = 0xC78; // client_dll
constexpr uintptr_t m_clrRender             = 0xC98; // client_dll
constexpr uintptr_t m_ClientOverrideTint = 0xF60; // TODO
constexpr uintptr_t m_bUseClientOverrideTint = 0xF64; // TODO

// Weapon
constexpr uintptr_t m_pWeaponSvc = 0x1208; // C_BasePlayerPawn
constexpr uintptr_t m_hMyWeapons     = 0x48;   // CPlayer_WeaponServices
constexpr uintptr_t m_hActiveWeapon  = 0x60;   // CPlayer_WeaponServices
constexpr uintptr_t m_pClippingWeapon= 0x3DC0; // TODO

// Weapon
constexpr uintptr_t m_iItemDefIdx  = 0x1BA;  // m_iItemDefinitionIndex
constexpr uintptr_t m_nFbPaintKit = 0x1680; // m_nFallbackPaintKit
constexpr uintptr_t m_nFbSeed = 0x1684; // m_nFallbackSeed
constexpr uintptr_t m_flFbWear = 0x1688; // m_flFallbackWear
constexpr uintptr_t m_AttrMgr = 0x11A8; // C_EconEntity
constexpr uintptr_t m_Item         = 0x50;   // C_AttributeContainer
constexpr uintptr_t m_iItemIDHigh  = 0x1D0;  // C_EconItemView
constexpr uintptr_t m_iItemIDLow   = 0x1D4;  // C_EconItemView
constexpr uintptr_t m_iItemID      = 0x1C8;  // C_EconItemView
constexpr uintptr_t m_iAccountID   = 0x1D8;  // C_EconItemView
constexpr uintptr_t m_iEntityQuality = 0x1BC; // C_EconItemView
constexpr uintptr_t m_AttributeList= 0x208;  // C_EconItemView
constexpr uintptr_t m_Attributes   = 0x8;
constexpr uintptr_t m_nFbStatTrak = 0x168C; // m_nFallbackStatTrak

// GlowServices
constexpr uintptr_t m_pGlowServices = 0x14A8;

// Grenade
constexpr uintptr_t m_bExplodeEffectBegan = 0x1214; // client_dll
constexpr uintptr_t m_arrTrajectoryTrailPoints = 0x1228; // client_dll

// Observer
// m_pObserverServices
// m_iObserverMode
// m_hObserverTarget
// m_bForcedObserverMode
constexpr uintptr_t m_pObserverServices = 0x1220;
constexpr uintptr_t m_iObserverMode       = 0x48;  // client_dll.hpp
constexpr uintptr_t m_hObserverTarget     = 0x4C;  // client_dll.hpp
constexpr uintptr_t m_bForcedObserverMode = 0x54;  // client_dll.hpp

// Camera
// Dogrulanmis
// m_iFOV
// m_iFOV
constexpr uintptr_t m_pCameraServices = 0x1240; // C_BasePlayerPawn
constexpr uintptr_t m_iFOV_Cam        = 0x290;  // CCSPlayerBase_CameraServices
constexpr uintptr_t m_iFOVStart_Cam   = 0x294;  // L1632
constexpr uintptr_t m_flFOVTime_Cam   = 0x298;  // GameTime_t
constexpr uintptr_t m_flFOVRate_Cam   = 0x29C;  // float32

} // namespace

// Observer
namespace ObsMode {
  constexpr int NONE      = 0; // Oyunda
  constexpr int DEATHCAM  = 1;
  constexpr int FREEZECAM = 2;
  constexpr int FIXED     = 3;
  constexpr int IN_EYE    = 4; // Birinci
  constexpr int CHASE     = 5; // nc
  constexpr int ROAMING   = 6; // Serbest
} // namespace

//
// ENTITY
//
inline uintptr_t GetEntity(uintptr_t el, int idx) {
  if (idx < 0 || idx > 0x7FFF || !el) return 0;
  uintptr_t chunk = SafeRead<uintptr_t>(el + 8*(idx>>9) + 16);
  if (!chunk) return 0;
  return SafeRead<uintptr_t>(chunk + 112*(idx&0x1FF));
}
inline uintptr_t GetPawnByHandle(uintptr_t el, uint32_t handle) {
  if (!handle || handle==0xFFFFFFFF) return 0;
  int idx = (int)(handle & 0x7FFF);
  uintptr_t chunk = SafeRead<uintptr_t>(el + 8*(idx>>9) + 16);
  if (!chunk) return 0;
  uintptr_t p = SafeRead<uintptr_t>(chunk + 112*(idx&0x1FF));
  return (p > 0x10000) ? p : 0;
}

//
// SNAPSHOTS
//
struct PlayerSnap {
  bool valid=false, isVisible=false, hasBones=false;
  int health=0, team=0, slot=0;
  float dist=0.f;
  Vec3 origin, headPos;
  Vec3 boneHead, boneNeck, boneChest, bonePelvis;
  Vec3 boneLeftKnee, boneRightKnee;
  Vec3 boneA[20], boneB[20];
  int boneCount=0;
  char name[64]={};
  uintptr_t pawn=0;
};
struct GrenadeSnap {
  bool valid=false;
  Vec3 pos={}, vel={};
  bool detonated=false;
  int team=0;
  int entIdx=0;
  Vec3 trail[100]; int trailCount=0;
};
struct InfernoSnap {
  bool valid=false;
  Vec3 pos={};
  int team=0;
};

// Grenade
struct GrenadeTracker {
  int entIdx=0;
  Vec3 positions[100];
  int count=0;
  ULONGLONG lastSeen=0;
  bool active=false;
};
inline GrenadeTracker g_GrenTrackers[16];

inline void TrackGrenade(int entIdx, Vec3 pos) {
  ULONGLONG now=GetTickCount64();
  for(int i=0;i<16;i++){
    if(g_GrenTrackers[i].active && g_GrenTrackers[i].entIdx==entIdx){
      auto& t=g_GrenTrackers[i];
      if(t.count==0 || Dist3D(t.positions[t.count-1],pos)>5.f){
        if(t.count<100) t.positions[t.count++]=pos;
        else { memmove(t.positions,t.positions+1,99*sizeof(Vec3)); t.positions[99]=pos; }
      }
      t.lastSeen=now;
      return;
    }
  }
  for(int i=0;i<16;i++){
    if(!g_GrenTrackers[i].active || (now-g_GrenTrackers[i].lastSeen>3000)){
      g_GrenTrackers[i]={entIdx,{},0,now,true};
      g_GrenTrackers[i].positions[0]=pos;
      g_GrenTrackers[i].count=1;
      return;
    }
  }
}

inline void CopyTrackerTrail(GrenadeSnap& g, int entIdx){
  for(int i=0;i<16;i++){
    if(g_GrenTrackers[i].active && g_GrenTrackers[i].entIdx==entIdx){
      int c=g_GrenTrackers[i].count; if(c>100) c=100;
      memcpy(g.trail, g_GrenTrackers[i].positions, c*sizeof(Vec3));
      g.trailCount=c;
      return;
    }
  }
}

// Molotov
struct MolotovZone {
  bool      active    = false;
  Vec3      center;
  ULONGLONG startTime = 0;
  int       team      = 0;   // 2
  int       entIdx    = 0;
};
inline MolotovZone g_MolotovZones[8] = {};

inline void TrackMolotovZone(int entIdx, Vec3 pos, int team) {
  for (int i=0; i<8; i++)
    if (g_MolotovZones[i].active && g_MolotovZones[i].entIdx==entIdx) return;
  ULONGLONG now=GetTickCount64();
  for (int i=0; i<8; i++) {
    if (!g_MolotovZones[i].active) {
      g_MolotovZones[i] = {true, pos, now, team, entIdx};
      return;
    }
  }
}

// Ucu
// Entity
struct FireProj { bool active=false; bool seen=false; int entIdx=0; Vec3 lastPos; int team=0; };
inline FireProj g_FireProjs[8] = {};

// Her
inline void FlushFireProjs() {
  static int s_key=1; // Her
  for (int j=0;j<8;j++) {
    if (!g_FireProjs[j].active) continue;
    if (!g_FireProjs[j].seen) {                   // Bu
      TrackMolotovZone(s_key++, g_FireProjs[j].lastPos, g_FireProjs[j].team);

      g_FireProjs[j].active=false;
    }
  }
}

// Frame
inline void ResetFireProjSeen() {
  for (int j=0;j<8;j++) if(g_FireProjs[j].active) g_FireProjs[j].seen=false;
}

// Entity
inline void UpdateFireProj(int entIdx, Vec3 pos, int team) {
  // Mevcut
  for (int j=0;j<8;j++) {
    if (g_FireProjs[j].active && g_FireProjs[j].entIdx==entIdx) {
      g_FireProjs[j].lastPos=pos; g_FireProjs[j].seen=true; return;
    }
  }
  // Yeni
  for (int j=0;j<8;j++) {
    if (!g_FireProjs[j].active) {
      g_FireProjs[j]={true,true,entIdx,pos,team}; return;
    }
  }
}

//
// GAME
//
struct GameCtx {
  uintptr_t client=0;
  uintptr_t localPawn=0;
  int localTeam=0;
  int localSlot=0;
  bool scoped=false;
  Vec3 localPos;
  Mat4 vm;
  int W=1920, H=1080;

  PlayerSnap  players[64]; int playerCount=0;
  GrenadeSnap grenades[16]; int grenadeCount=0;
  InfernoSnap infernos[8];  int infernoCount=0;

  void Init() { client=(uintptr_t)GetModuleHandleA("client.dll"); }

  void Update() {
    if (!client) Init();
    if (!client) return;

    vm       = SafeRead<Mat4>(client + Off::dwViewMatrix);
    localPawn= SafeRead<uintptr_t>(client + Off::dwLocalPawn);

    if (localPawn > 0x10000) {
      localTeam = SafeRead<uint8_t>(localPawn + Off::m_iTeamNum);
      scoped    = SafeRead<bool>(localPawn + Off::m_bIsScoped);
      uintptr_t ln = SafeRead<uintptr_t>(localPawn + Off::m_pGameSceneNode);
      if (ln > 0x10000) localPos = SafeRead<Vec3>(ln + Off::m_vecAbsOrigin);
    } else { localTeam=0; scoped=false; }

    uintptr_t el = SafeRead<uintptr_t>(client + Off::dwEntityList);
    if (!el || el < 0x10000) { playerCount=0; return; }

    // PLAYER
    int cnt=0;
    for (int i=1; i<=64 && cnt<64; i++) {
      uintptr_t ctrl = GetEntity(el, i);
      if (!ctrl || ctrl<=0x10000) continue;

      uint32_t ph = SafeRead<uint32_t>(ctrl + Off::m_hPlayerPawn);
      if (!ph || ph==0xFFFFFFFF) continue;

      uintptr_t pawn = GetPawnByHandle(el, ph);
      if (!pawn) continue;
      if (pawn==localPawn) { localSlot=i; continue; }

      if (SafeRead<uint8_t>(pawn + Off::m_lifeState) != 0) continue;

      int hp = SafeRead<int>(pawn + Off::m_iHealth);
      if (hp<=0 || hp>200) hp = SafeRead<int>(ctrl + Off::m_iPawnHealth);
      if (hp<=0 || hp>200) continue;

      uintptr_t node = SafeRead<uintptr_t>(pawn + Off::m_pGameSceneNode);
      if (!node || node<=0x10000) continue;

      Vec3 org = SafeRead<Vec3>(node + Off::m_vecAbsOrigin);
      if (org.x==0 && org.y==0 && org.z==0) continue;

      PlayerSnap& p = players[cnt];
      memset(&p, 0, sizeof(p));
      p.valid=true; p.slot=i;
      p.team   = SafeRead<uint8_t>(pawn + Off::m_iTeamNum);
      p.health = hp;
      p.pawn   = pawn;
      p.origin = org;
      p.dist   = Dist3D(localPos, org);

      // Visibility
      {
        uint32_t mask = SafeRead<uint32_t>(pawn + Off::m_entitySpottedState + Off::m_bSpottedByMask);
        if (localSlot > 0) {
          // localSlot
          p.isVisible = (bool)((mask >> (localSlot - 1)) & 1u);
        } else {
          // localSlot
          p.isVisible = SafeRead<bool>(pawn + Off::m_entitySpottedState + Off::m_bSpotted);
        }
        // ok
        if (!p.isVisible && p.dist < 400.f) p.isVisible = true;
      }

      SafeReadStr(ctrl + Off::m_iszPlayerName, p.name, 64);
      if (!p.name[0]) { p.name[0]='P'; p.name[1]='l'; p.name[2]='y'; p.name[3]='\0'; }

      // Bone
      p.headPos = p.boneHead = {org.x, org.y, org.z+75.f};
      p.boneNeck   = {org.x, org.y, org.z+67.f};
      p.boneChest  = {org.x, org.y, org.z+48.f};
      p.bonePelvis = org;
      p.boneLeftKnee = p.boneRightKnee = {org.x, org.y, org.z+20.f};

      // Real
      uintptr_t ba = SafeRead<uintptr_t>(node + Off::m_modelState + Off::m_boneArray);
      if (ba > 0x10000) {
        char buf[64*32]{};  // 64
        if (SafeReadBuf(ba, buf, sizeof(buf))) {
          auto B = [&](int id) -> Vec3 {
            if (id<0||id>=64) return {};
            Vec3 v; memcpy(&v, buf+id*32, 12); return v;
          };
          // B07
          // BONE-SCAN
          Vec3 th = B(7);
          if (!th.x && !th.y && !th.z) th = B(6);

          if (th.x || th.y || th.z) {
            p.boneHead      = th;      // B07
            p.boneNeck      = B(6);    // B06
            p.boneChest     = B(4);    // B04
            p.bonePelvis    = B(0);    // B00
            p.boneLeftKnee  = B(18);   // B18
            p.boneRightKnee = B(21);   // B21
            p.hasBones = true;
            if (p.boneHead.z > org.z) p.headPos = p.boneHead;

            // BONE-DUMP
            // B00
            // Omurga
            // Sol
            // Sag
            // Sol
            // Sag
            static const int ids[40] = {
              7,6,  6,5,  5,4,  4,3,  3,1,   // omurga
              5,8,  8,9,  9,10, 10,11,        // sol
              5,12, 12,13, 13,14, 14,15,      // sag
              1,17, 17,18, 18,19,              // sol
              1,20, 20,21, 21,22               // sag
            };
            p.boneCount=0;
            for (int j=0; j<40 && p.boneCount<20; j+=2) {
              Vec3 ba2=B(ids[j]), bb2=B(ids[j+1]);
              if (!ba2.x&&!ba2.y&&!ba2.z) continue;
              if (!bb2.x&&!bb2.y&&!bb2.z) continue;
              p.boneA[p.boneCount]=ba2;
              p.boneB[p.boneCount]=bb2;
              p.boneCount++;
            }




          }
        }
      }
      cnt++;
    }
    for (int i=cnt;i<64;i++) memset(&players[i],0,sizeof(PlayerSnap));
    playerCount=cnt;

    // GRENADE
    grenadeCount=0; infernoCount=0;


    // Asama
    // Grenade
    // Inferno
    ULONGLONG now1 = GetTickCount64();

    for (int t=0; t<16; t++) {
      if (!g_GrenTrackers[t].active) continue;
      int eidx = g_GrenTrackers[t].entIdx;
      uintptr_t ent2 = GetEntity(el, eidx);

      // Entity
      if (!ent2 || ent2<=0x10000 || (now1 - g_GrenTrackers[t].lastSeen > 2500)) {
        g_GrenTrackers[t].active = false; // trail
        continue;
      }

      uintptr_t nd2 = SafeRead<uintptr_t>(ent2 + Off::m_pGameSceneNode);
      if (!nd2 || nd2<=0x10000) continue;
      Vec3 ePos2 = SafeRead<Vec3>(nd2 + Off::m_vecAbsOrigin);
      if (ePos2.x==0 && ePos2.y==0 && ePos2.z==0) continue;
      Vec3 eVel2 = SafeRead<Vec3>(ent2 + Off::m_vecVelocity);
      bool det2  = SafeRead<bool>(ent2 + Off::m_bExplodeEffectBegan);

      // Patladi
      if (det2) {
        g_GrenTrackers[t].active = false;
        continue;
      }

      int eteam2 = (int)SafeRead<uint8_t>(ent2 + Off::m_iTeamNum);
      if (grenadeCount < 16) {
        auto& g2 = grenades[grenadeCount];
        memset(&g2, 0, sizeof(g2));
        g2.valid = true; g2.pos = ePos2; g2.vel = eVel2;
        g2.detonated = false; g2.team = eteam2; g2.entIdx = eidx;
        CopyTrackerTrail(g2, eidx);
        grenadeCount++;
      }
      if (eVel2.Len()>50.f)
        TrackGrenade(eidx, ePos2);
    }

    // Inferno
    for (int mz=0; mz<8; mz++) {
      if (!g_MolotovZones[mz].active) continue;
      // 30
      if (now1 - g_MolotovZones[mz].startTime > 30000) {
        g_MolotovZones[mz].active = false; continue;
      }
      if (infernoCount < 8) {
        infernos[infernoCount++] = {true, g_MolotovZones[mz].center, g_MolotovZones[mz].team};
      }
    }

    // Asama
    static int s_grenFrame = 0; s_grenFrame++;
    if (s_grenFrame % 4 != 0) goto skip_gren;

    // SADECE
    ResetFireProjSeen();

    for (int i=1; i<2048 && (grenadeCount<16||infernoCount<8); i++) {
      uintptr_t ent=GetEntity(el,i);
      if (!ent||ent<=0x10000) continue;
      uintptr_t identity=SafeRead<uintptr_t>(ent+0x10);
      if (!identity||identity<=0x10000) continue;
      uintptr_t namePtr=SafeRead<uintptr_t>(identity+0x20);
      if (!namePtr||namePtr<=0x10000) continue;
      char cls[64]={}; SafeReadStr(namePtr,cls,64);
      for(int c=0;cls[c];c++) if(cls[c]>='A'&&cls[c]<='Z') cls[c]+='a'-'A';

      bool isProj=(strstr(cls,"projectile")!=nullptr)||(strstr(cls,"grenade")!=nullptr);
      bool isInf =(strstr(cls,"inferno")!=nullptr);

      if (!isProj && !isInf) continue;

      uintptr_t nd2=SafeRead<uintptr_t>(ent+Off::m_pGameSceneNode);
      if (!nd2||nd2<=0x10000) continue;
      Vec3 ePos=SafeRead<Vec3>(nd2+Off::m_vecAbsOrigin);
      if (ePos.x==0&&ePos.y==0&&ePos.z==0) continue;
      int eTeam=(int)SafeRead<uint8_t>(ent+Off::m_iTeamNum);
      float dd=Dist3D(localPos,ePos);

      if (isProj && dd<5000.f) {
        bool alreadyTracked = false;
        for (int t2=0; t2<16; t2++)
          if (g_GrenTrackers[t2].active && g_GrenTrackers[t2].entIdx==i) { alreadyTracked=true; break; }
        if (!alreadyTracked) TrackGrenade(i, ePos);

        bool isMolotovProj=(strstr(cls,"projectile")!=nullptr)&&
                           ((strstr(cls,"molotov")!=nullptr)||(strstr(cls,"incendiary")!=nullptr));
        if (isMolotovProj) UpdateFireProj(i, ePos, eTeam);
      }
      if (isInf && infernoCount<8 && dd<10000.f) {
        infernos[infernoCount++]={true,ePos,eTeam};
        TrackMolotovZone(i, ePos, eTeam);
      }
    }
    // FlushFireProjs
    FlushFireProjs();
    skip_gren:;
  }

};

inline GameCtx G;

//
// WEAPON
//
inline int GetWeaponDefForPawn(uintptr_t pawn) {
  if (!pawn||pawn<=0x10000||!G.client) return -1;
  uintptr_t wSvc=SafeRead<uintptr_t>(pawn+Off::m_pWeaponSvc);
  if (!wSvc||wSvc<=0x10000) return -1;
  uint32_t wHandle=SafeRead<uint32_t>(wSvc+Off::m_hActiveWeapon);
  if (!wHandle||wHandle==0xFFFFFFFF) return -1;
  uintptr_t el=SafeRead<uintptr_t>(G.client+Off::dwEntityList);
  if (!el) return -1;
  uintptr_t weapon=GetPawnByHandle(el,wHandle);
  if (!weapon||weapon<=0x10000) return -1;
  // Dogru
  uintptr_t item = weapon + Off::m_AttrMgr + Off::m_Item;
  return (int)SafeRead<uint16_t>(item + Off::m_iItemDefIdx);
}
inline bool IsSniperDef(int def) { return def==9||def==40||def==11||def==38; }
inline const char* WeaponNameFromDefIdx(int def) {
  switch(def) {
    case 1:  return "Deagle";    case 2:  return "Duals";
    case 3:  return "Five-7";    case 4:  return "Glock";
    case 7:  return "AK-47";     case 8:  return "AUG";
    case 9:  return "AWP";       case 10: return "FAMAS";
    case 11: return "G3SG1";     case 13: return "Galil AR";
    case 14: return "M249";      case 16: return "M4A4";
    case 17: return "MAC-10";    case 19: return "P90";
    case 20: return "Nova";      case 21: return "P2000";
    case 23: return "MP5-SD";    case 24: return "UMP-45";
    case 25: return "XM1014";    case 26: return "PP-Bizon";
    case 27: return "MAG-7";     case 28: return "Negev";
    case 29: return "Sawed-Off"; case 30: return "Tec-9";
    case 31: return "Zeus";      case 32: return "P250";
    case 33: return "MP7";       case 34: return "MP9";
    case 36: return "P250";      case 38: return "SCAR-20";
    case 39: return "SG 553";    case 40: return "SSG 08";
    case 41: return "HE";        case 42: return "Flash";
    case 43: return "Smoke";     case 44: return "Molotov";
    case 45: return "Decoy";     case 46: return "Incendiary";
    case 47: return "C4";
    case 57: case 60: return "M4A1-S";
    case 61: return "USP-S";     case 63: return "CZ75";
    case 64: return "R8";
    case 500: return "Karambit"; case 503: return "Huntsman";
    case 505: return "Shadow Dag";
    default: return nullptr;
  }
}
