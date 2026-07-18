#pragma once
#include "Features.h"
#include "HitMarker.h"
#include "SkinChanger.h"
#include <windows.h>
#include <shlobj.h>
#include <cstdio>
#include <vector>
#include <string>
#include <mutex>

#pragma comment(lib, "shell32.lib")

namespace CfgSys {

static constexpr uint32_t MAGIC   = 0x51524537; // QRE7
static constexpr uint32_t VERSION = 8;           // v8

// Skin
struct SkinEntry {
  int   def;
  int   paintKit;
  float wear;
  int   seed;
  int   statTrak;
  int   knifeModel;
};

// Ana
#pragma pack(push, 1)
struct CfgData {
  uint32_t magic   = MAGIC;
  uint32_t version = VERSION;

  // ESP
  bool espOn,espBox,espName,espHp,espBones,espHead,espHeadCircle,espSnap,espWeapon,espHint,espFovCircle,espTeam;
  int  boxStyle,hpBarSide;
  float colBoxVis[3],colBoxOcc[3],colBone[3],colHead[3],colSnap[3];
  float colWeaponIcon[3]; float weaponIconSize;
  bool  espDistOn;

  // Glow
  bool glowOn; bool glowTeam; int glowKey;
  float glowVis[3],glowWall[3];

  // Chams
  bool chamsOn,chamsWall,chamsTeam; int chamsKey,chamsMat;
  float chamsVis[3],chamsOcc[3];

  // Legit
  bool aimOn,aimNeedKey,aimLock,aimWall,aimShootTeam;
  bool aimProxOn; float aimProxDist;
  int  aimKey,aimBone;
  float aimFov,aimSmooth,aimDz;

  // Ragebot
  bool rageOn,rageNeedKey,rageAuto,rageShootTeam;
  int  rageKey,rageBone;
  float rageFov,rageSmooth,rageAutoDelay;

  // Triggerbot
  bool trigOn,trigNeedKey,trigWall,trigShootTeam;
  int  trigKey; float trigDelay;

  // Misc
  bool bhopOn,noFlash,noScope,autoStrafe; int bhopKey;

  // Crosshair
  bool custXhair; float custXhairSize,custXhairThick; int custXhairGap;
  float colXhair[3];

  // Anti-Aim
  bool aaOn; int aaMode,aaKey; float aaYawAdd;

  // Radar
  bool radarHack;

  // Grenade
  bool grenadeESP,molotovWall;
  float colGrenadeT[3],colGrenadeCT[3];

  // Enemy
  bool  enemyIndicator;
  float enemyIndicatorCol[4];
  float enemyIndicatorSize;

  // Screen
  bool screenFogOn,screenFogTop,screenFogBottom,screenFogLeft,screenFogRight;
  float colFogTop[4],colFogBottom[4],colFogLeft[4],colFogRight[4];

  // Overlay
  bool spectatorListOn,shortcutListOn,bombTimerOn,infoOverlayOn;
  int  windowArrangeKey;

  // Overlay
  float specWinX,specWinY,scWinX,scWinY,bombWinX,bombWinY,infoWinX,infoWinY;

  // Gorsel
  bool  dx11WorldOn; float dx11WorldColor[4];
  bool  skyColorOn;  float skyColor[3]; float skyAlpha,skyHeight;
  bool  noSmoke;
  bool  snowOn; int snowDensity; bool particle3D;
  bool  sakuraOn; float sakuraColor[4];
  bool  skyboxChangerOn; float skyboxColor[3];

  // HitMarker
  bool  hmOn;
  float hmCol[3];   // X
  float hmSize;     // X
  float hmThick;    // cizgi
};
#pragma pack(pop)

// Yol
static bool GetFolderPath(char* out, int maxLen) {
  char docs[MAX_PATH] = {};
  if (FAILED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, docs))) return false;
  snprintf(out, maxLen, "%s\\QuaFindConfig", docs);
  CreateDirectoryA(out, NULL);
  return true;
}
static bool GetFilePath(const char* name, char* out, int maxLen) {
  char folder[MAX_PATH] = {};
  if (!GetFolderPath(folder, MAX_PATH)) return false;
  snprintf(out, maxLen, "%s\\%s.qcfg", folder, name);
  return true;
}

// Config
inline std::vector<std::string> ListConfigs() {
  std::vector<std::string> res;
  char folder[MAX_PATH] = {};
  if (!GetFolderPath(folder, MAX_PATH)) return res;
  char pattern[MAX_PATH] = {};
  snprintf(pattern, MAX_PATH, "%s\\*.qcfg", folder);
  WIN32_FIND_DATAA fd{};
  HANDLE h = FindFirstFileA(pattern, &fd);
  if (h == INVALID_HANDLE_VALUE) return res;
  do {
    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
    std::string nm = fd.cFileName;
    if (nm.size() > 5) nm = nm.substr(0, nm.size() - 5);
    res.push_back(nm);
  } while (FindNextFileA(h, &fd));
  FindClose(h);
  return res;
}

// Pack
#define MCP(f) memcpy(d.f, Cfg::f, sizeof(d.f))
#define CP(f)  d.f = Cfg::f

static void Pack(CfgData& d) {
  d.magic=MAGIC; d.version=VERSION;

  // ESP
  CP(espOn);CP(espBox);CP(espName);CP(espHp);CP(espBones);CP(espHead);CP(espHeadCircle);
  CP(espSnap);CP(espWeapon);CP(espHint);CP(espFovCircle);CP(espTeam);
  CP(boxStyle);CP(hpBarSide);
  MCP(colBoxVis);MCP(colBoxOcc);MCP(colBone);MCP(colHead);MCP(colSnap);
  MCP(colWeaponIcon); CP(weaponIconSize); CP(espDistOn);

  // Glow
  CP(glowOn);CP(glowTeam);CP(glowKey);
  MCP(glowVis);MCP(glowWall);

  // Chams
  CP(chamsOn);CP(chamsWall);CP(chamsTeam);CP(chamsKey);CP(chamsMat);
  MCP(chamsVis);MCP(chamsOcc);

  // Legit
  CP(aimOn);CP(aimNeedKey);CP(aimLock);CP(aimWall);CP(aimShootTeam);
  CP(aimProxOn);CP(aimProxDist);
  CP(aimKey);CP(aimBone);CP(aimFov);CP(aimSmooth);CP(aimDz);

  // Rage
  CP(rageOn);CP(rageNeedKey);CP(rageAuto);CP(rageShootTeam);
  CP(rageKey);CP(rageBone);CP(rageFov);CP(rageSmooth);CP(rageAutoDelay);

  // Trigger
  CP(trigOn);CP(trigNeedKey);CP(trigWall);CP(trigShootTeam);
  CP(trigKey);CP(trigDelay);

  // Misc
  CP(bhopOn);CP(noFlash);CP(noScope);CP(autoStrafe);CP(bhopKey);

  // Crosshair
  CP(custXhair);CP(custXhairSize);CP(custXhairThick);CP(custXhairGap);
  MCP(colXhair);

  // Anti-Aim
  CP(aaOn);CP(aaMode);CP(aaKey);CP(aaYawAdd);

  // Radar
  CP(radarHack);

  // Grenade
  CP(grenadeESP);CP(molotovWall);
  MCP(colGrenadeT);MCP(colGrenadeCT);

  // Enemy
  CP(enemyIndicator);
  MCP(enemyIndicatorCol);
  CP(enemyIndicatorSize);

  // Screen
  CP(screenFogOn);CP(screenFogTop);CP(screenFogBottom);CP(screenFogLeft);CP(screenFogRight);
  MCP(colFogTop);MCP(colFogBottom);MCP(colFogLeft);MCP(colFogRight);

  // Overlay
  CP(spectatorListOn);CP(shortcutListOn);CP(bombTimerOn);CP(infoOverlayOn);
  CP(windowArrangeKey);
  CP(specWinX);CP(specWinY);CP(scWinX);CP(scWinY);
  CP(bombWinX);CP(bombWinY);CP(infoWinX);CP(infoWinY);

  // Gorsel
  CP(dx11WorldOn); MCP(dx11WorldColor);
  CP(skyColorOn);  MCP(skyColor); CP(skyAlpha);CP(skyHeight);
  CP(noSmoke);
  CP(snowOn);CP(snowDensity);CP(particle3D);
  CP(sakuraOn); MCP(sakuraColor);
  CP(skyboxChangerOn); MCP(skyboxColor);

  // HitMarker
  d.hmOn = HM::hmOn;
  memcpy(d.hmCol,   HM::hmCol,   12);
  d.hmSize  = HM::hmSize;
  d.hmThick = HM::hmThick;
}

#undef CP
#undef MCP

#define MUP(f) memcpy(Cfg::f, d.f, sizeof(d.f))
#define UP(f)  Cfg::f = d.f

static void Unpack(const CfgData& d) {
  // ESP
  UP(espOn);UP(espBox);UP(espName);UP(espHp);UP(espBones);UP(espHead);UP(espHeadCircle);
  UP(espSnap);UP(espWeapon);UP(espHint);UP(espFovCircle);UP(espTeam);
  UP(boxStyle);UP(hpBarSide);
  MUP(colBoxVis);MUP(colBoxOcc);MUP(colBone);MUP(colHead);MUP(colSnap);
  MUP(colWeaponIcon); UP(weaponIconSize); UP(espDistOn);

  // Glow
  UP(glowOn);UP(glowTeam);UP(glowKey);
  MUP(glowVis);MUP(glowWall);

  // Chams
  UP(chamsOn);UP(chamsWall);UP(chamsTeam);UP(chamsKey);UP(chamsMat);
  MUP(chamsVis);MUP(chamsOcc);

  // Legit
  UP(aimOn);UP(aimNeedKey);UP(aimLock);UP(aimWall);UP(aimShootTeam);
  UP(aimProxOn);UP(aimProxDist);
  UP(aimKey);UP(aimBone);UP(aimFov);UP(aimSmooth);UP(aimDz);

  // Rage
  UP(rageOn);UP(rageNeedKey);UP(rageAuto);UP(rageShootTeam);
  UP(rageKey);UP(rageBone);UP(rageFov);UP(rageSmooth);UP(rageAutoDelay);

  // Trigger
  UP(trigOn);UP(trigNeedKey);UP(trigWall);UP(trigShootTeam);
  UP(trigKey);UP(trigDelay);

  // Misc
  UP(bhopOn);UP(noFlash);UP(noScope);UP(autoStrafe);UP(bhopKey);

  // Crosshair
  UP(custXhair);UP(custXhairSize);UP(custXhairThick);UP(custXhairGap);
  MUP(colXhair);

  // Anti-Aim
  UP(aaOn);UP(aaMode);UP(aaKey);UP(aaYawAdd);

  // Radar
  UP(radarHack);

  // Grenade
  UP(grenadeESP);UP(molotovWall);
  MUP(colGrenadeT);MUP(colGrenadeCT);

  // Enemy
  UP(enemyIndicator);
  MUP(enemyIndicatorCol);
  UP(enemyIndicatorSize);

  // Screen
  UP(screenFogOn);UP(screenFogTop);UP(screenFogBottom);UP(screenFogLeft);UP(screenFogRight);
  MUP(colFogTop);MUP(colFogBottom);MUP(colFogLeft);MUP(colFogRight);

  // Overlay
  UP(spectatorListOn);UP(shortcutListOn);UP(bombTimerOn);UP(infoOverlayOn);
  UP(windowArrangeKey);
  UP(specWinX);UP(specWinY);UP(scWinX);UP(scWinY);
  UP(bombWinX);UP(bombWinY);UP(infoWinX);UP(infoWinY);

  // Gorsel
  UP(dx11WorldOn); MUP(dx11WorldColor);
  UP(skyColorOn);  MUP(skyColor); UP(skyAlpha);UP(skyHeight);
  UP(noSmoke);
  UP(snowOn);UP(snowDensity);UP(particle3D);
  UP(sakuraOn); MUP(sakuraColor);
  UP(skyboxChangerOn); MUP(skyboxColor);

  // HitMarker
  HM::hmOn   = d.hmOn;
  memcpy(HM::hmCol, d.hmCol, 12);
  HM::hmSize  = d.hmSize;
  HM::hmThick = d.hmThick;
}

#undef UP
#undef MUP

// Save
inline bool Save(const char* name) {
  if (!name || !name[0]) return false;
  char path[MAX_PATH] = {};
  if (!GetFilePath(name, path, MAX_PATH)) return false;

  CfgData d{}; Pack(d);

  std::vector<SkinEntry> skins;
  {
    std::lock_guard<std::mutex> lk(Skin::configMutex);
    for (auto& [def, sc] : Skin::weaponSkins) {
      if (!sc.enabled) continue;
      SkinEntry e;
      e.def=def; e.paintKit=sc.paintKit; e.wear=sc.wear;
      e.seed=sc.seed; e.statTrak=sc.statTrak; e.knifeModel=sc.knifeModel;
      skins.push_back(e);
    }
  }

  HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL, NULL);
  if (h == INVALID_HANDLE_VALUE) return false;

  DWORD w = 0;
  WriteFile(h, &d, sizeof(d), &w, NULL);
  int cnt = (int)skins.size();
  WriteFile(h, &cnt, sizeof(int), &w, NULL);
  if (cnt > 0)
    WriteFile(h, skins.data(), cnt * (int)sizeof(SkinEntry), &w, NULL);

  CloseHandle(h);
  printf("[CFG] Kaydedildi: %s (%d skin)\n", name, cnt);
  return true;
}

// Load
inline bool Load(const char* name) {
  if (!name || !name[0]) return false;
  char path[MAX_PATH] = {};
  if (!GetFilePath(name, path, MAX_PATH)) return false;

  HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (h == INVALID_HANDLE_VALUE) return false;

  CfgData d{};
  DWORD r = 0;
  ReadFile(h, &d, sizeof(d), &r, NULL);
  if (r != sizeof(d) || d.magic != MAGIC || d.version != VERSION) {
    CloseHandle(h);
    printf("[CFG] Gecersiz veya eski versiyon (beklenen v%u): %s\n", VERSION, name);
    return false;
  }
  Unpack(d);

  int cnt = 0;
  ReadFile(h, &cnt, sizeof(int), &r, NULL);
  if (cnt > 0 && cnt < 256 && r == sizeof(int)) {
    std::vector<SkinEntry> skins((size_t)cnt);
    ReadFile(h, skins.data(), cnt * (int)sizeof(SkinEntry), &r, NULL);
    if (r == (DWORD)(cnt * (int)sizeof(SkinEntry))) {
      std::lock_guard<std::mutex> lk(Skin::configMutex);
      Skin::weaponSkins.clear();
      for (auto& e : skins) {
        Skin::SkinConfig sc;
        sc.paintKit=e.paintKit; sc.wear=e.wear; sc.seed=e.seed;
        sc.statTrak=e.statTrak; sc.knifeModel=e.knifeModel; sc.enabled=true;
        Skin::weaponSkins[e.def] = sc;
      }
      Skin::forceUpdate.store(true);
    }
  }

  CloseHandle(h);
  printf("[CFG] Yuklendi: %s (%d skin)\n", name, cnt);
  return true;
}

// Delete
inline bool Delete(const char* name) {
  if (!name || !name[0]) return false;
  char path[MAX_PATH] = {};
  if (!GetFilePath(name, path, MAX_PATH)) return false;
  return DeleteFileA(path) != 0;
}

} // namespace
