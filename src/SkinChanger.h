#pragma once
// SkinChanger.h
// Offsetler
#include "SDK.h"
#include <windows.h>
#include <Psapi.h>
#include <cstdio>
#include <cstring>
#include <map>
#include <mutex>
#include <atomic>
#include <vector>

#pragma comment(lib, "psapi.lib")

namespace Skin {

  // Skin
  struct SkinConfig {
    int   paintKit   = 0;
    float wear       = 0.001f;
    int   seed       = 1;    // 0
    int   statTrak   = -1;
    int   knifeModel = 0;  // 0
    bool  enabled    = false;
  };

  inline std::map<int, SkinConfig> weaponSkins;
  inline std::atomic<bool>         forceUpdate{false};
  inline std::mutex                configMutex;

  // Weapon
  struct WeaponInfo { int def; const char* name; };
  static const WeaponInfo kWeapons[] = {
    {1,"Desert Eagle"},{2,"Dual Berettas"},{3,"Five-SeveN"},
    {4,"Glock-18"},{7,"AK-47"},{8,"AUG"},{9,"AWP"},{10,"FAMAS"},
    {11,"G3SG1"},{13,"Galil AR"},{14,"M249"},{16,"M4A4"},
    {17,"MAC-10"},{19,"P90"},{23,"MP5-SD"},{24,"UMP-45"},
    {25,"XM1014"},{26,"PP-Bizon"},{27,"MAG-7"},{28,"Negev"},
    {29,"Sawed-Off"},{30,"Tec-9"},{32,"P2000"},{33,"MP7"},
    {34,"MP9"},{35,"Nova"},{36,"P250"},{38,"SCAR-20"},
    {39,"SG 553"},{40,"SSG 08"},{60,"M4A1-S"},{61,"USP-S"},
    {63,"CZ75-Auto"},{64,"R8 Revolver"},
    // Standart
    {42,u8"B\u0131\u00e7ak (T-Side)"},{59,u8"B\u0131\u00e7ak (CT-Side)"},
    // Koleksiyon
    {500,"Bayonet"},{503,"Classic Knife"},{505,"Flip Knife"},
    {506,"Gut Knife"},{507,"Karambit"},{508,"M9 Bayonet"},
    {509,"Huntsman"},{512,"Falchion"},{514,"Bowie Knife"},
    {515,"Butterfly"},{516,"Shadow Daggers"},{519,"Ursus Knife"},
    {520,"Navaja"},{521,"Nomad Knife"},{522,"Stiletto"},
    {523,"Talon Knife"},{524,"Paracord"},{525,"Skeleton Knife"},
    {0,nullptr}
  };

  inline const char* GetWeaponName(int d) {
    for (int i = 0; kWeapons[i].name; i++)
      if (kWeapons[i].def == d) return kWeapons[i].name;
    return "Weapon";
  }

  // m_nSubclassID
  // m_nSubclassID
  static uint32_t MurmurHash2Token(const char* key) {
    int len = (int)strlen(key);
    const uint32_t m = 0x5bd1e995u;
    uint32_t h = 0u ^ (uint32_t)len;
    const auto* data = (const uint8_t*)key;
    while (len >= 4) {
      uint32_t k  = (uint32_t)data[0]        | ((uint32_t)data[1] << 8)
                  | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
      // tolower
      k |= ((((k + 0x05050505u) ^ (k + 0x1b1b1b1bu)) & 0x80808080u) >> 2);
      k *= m; k ^= k >> 24; k *= m;
      h *= m; h ^= k;
      data += 4; len -= 4;
    }
    switch (len) {
      case 3: h ^= (uint32_t)((data[2]|0x20)) << 16; [[fallthrough]];
      case 2: h ^= (uint32_t)((data[1]|0x20)) << 8;  [[fallthrough]];
      case 1: h ^= (uint32_t)((data[0]|0x20)); h *= m;
    }
    h ^= h >> 13; h *= m; h ^= h >> 15;
    return h;
  }

  // b
  struct KnifeClass { int def; const char* cls; };
  static const KnifeClass knifeClasses[] = {
    {500, "C_Knife_Bayonet"},
    {503, "C_KnifeClassic"},
    {505, "C_KnifeFlip"},
    {506, "C_KnifeGut"},
    {507, "C_KnifeKarambit"},
    {508, "C_KnifeM9Bayonet"},
    {509, "C_KnifeHuntsmanKnife"},
    {512, "C_KnifeFalchionKnife"},
    {514, "C_KnifeSurvivalBowie"},
    {515, "C_KnifeButterfly"},
    {516, "C_KnifeShadowDagger"},
    {519, "C_KnifeUrsus"},
    {520, "C_KnifeGypsyJackKnife"},
    {521, "C_KnifeNomad"},
    {522, "C_KnifeStiletto"},
    {523, "C_KnifeTalon"},
    {524, "C_KnifeParacord"},
    {525, "C_KnifeSkeleton"},
    {0, nullptr}
  };

  // B
  // CS2
  // viewmodel

  struct KnifeModelData {
    uint64_t hModel;    // CModelState
    uint64_t modelName; // CModelState
    uint32_t subclassID;
    bool     valid = false;
  };
  inline std::map<int,KnifeModelData> g_knifeLearned;

  // entity
  // 8192
  inline void LearnKnifeModels(uintptr_t skipWeapon) {
    uintptr_t el = SafeRead<uintptr_t>(G.client + Off::dwEntityList);
    if (!el || el <= 0x10000) return;
    for (int i = 1; i < 8192; i++) {
      // SDK.h
      uintptr_t chunk = SafeRead<uintptr_t>(el + 8*(i>>9) + 16);
      if (!chunk) continue;
      uintptr_t ent = SafeRead<uintptr_t>(chunk + 112*(i & 0x1FF));
      if (!ent || ent <= 0x10000 || ent == skipWeapon) continue;
      uint16_t def = SafeRead<uint16_t>(ent + Off::m_AttrMgr + Off::m_Item + Off::m_iItemDefIdx);
      bool isKnife = (def >= 500 && def < 600) || def == 42 || def == 59;
      if (!isKnife) continue;
      if (g_knifeLearned.count(def) && g_knifeLearned[def].valid) continue;
      uintptr_t sn = SafeRead<uintptr_t>(ent + Off::m_pGameSceneNode);
      if (!sn || sn <= 0x10000) continue;
      uint64_t hm  = SafeRead<uint64_t>(sn + 0x150 + 0xA0);
      uint64_t mn  = SafeRead<uint64_t>(sn + 0x150 + 0xA8);
      uint32_t sub = SafeRead<uint32_t>(ent + 0x380);
      if (!hm) continue;
      g_knifeLearned[def] = {hm, mn, sub, true};
      printf("[KNIFE-LEARN] def=%d hModel=0x%llX sub=0x%X ent=0x%llX\n",def,hm,sub,ent);
    }
  }

  // Hem
  inline void ApplyKnifeModel(uintptr_t weapon, int knifeModel, uintptr_t el) {
    if (!knifeModel) return;
    LearnKnifeModels(weapon);

    auto it = g_knifeLearned.find(knifeModel);
    if (it == g_knifeLearned.end() || !it->second.valid) {
      // Fallback
      static const struct{int d;const char*c;}kc[]={{500,"c_knife_bayonet"},{503,"c_knifeclassic"},
        {505,"c_knifeflip"},{506,"c_knifegut"},{507,"c_knifekarambit"},{508,"c_knifem9bayonet"},
        {509,"c_knifehuntsmanknife"},{512,"c_knifefalchionknife"},{514,"c_knifesurvivalbowie"},
        {515,"c_knifebutterfly"},{516,"c_knifeshadowdagger"},{519,"c_knifeursus"},
        {520,"c_knifegypsyjackknife"},{521,"c_knifenomad"},{522,"c_knifestiletto"},
        {523,"c_knifetalon"},{524,"c_knifeparacord"},{525,"c_knifeskeleton"},{0,nullptr}};
      auto mm2=[](const char*s)->uint32_t{
        int n=(int)strlen(s);const uint32_t M=0x5bd1e995u;uint32_t h=0^(uint32_t)n;
        const auto*d=(const uint8_t*)s;
        while(n>=4){uint32_t k=(uint32_t)d[0]|((uint32_t)d[1]<<8)|((uint32_t)d[2]<<16)|((uint32_t)d[3]<<24);
          k*=M;k^=k>>24;k*=M;h*=M;h^=k;d+=4;n-=4;}
        switch(n){case 3:h^=(uint32_t)d[2]<<16;[[fallthrough]];
          case 2:h^=(uint32_t)d[1]<<8;[[fallthrough]];
          case 1:h^=(uint32_t)d[0];h*=M;}h^=h>>13;h*=M;h^=h>>15;return h;};
      uint32_t tok=0;
      for(int i=0;kc[i].c;i++) if(kc[i].d==knifeModel){tok=mm2(kc[i].c);break;}
      if(tok) SafeWrite<uint32_t>(weapon+0x380,tok);
      return;
    }

    const auto& kmd = it->second;

    // 1
    SafeWrite<uint32_t>(weapon + 0x380, kmd.subclassID);
    uintptr_t wsn = SafeRead<uintptr_t>(weapon + Off::m_pGameSceneNode);
    if (wsn && wsn > 0x10000) {
      SafeWrite<uint64_t>(wsn + 0x150 + 0xA0, kmd.hModel);
      SafeWrite<uint64_t>(wsn + 0x150 + 0xA8, kmd.modelName);
    }

    // 2
    if (el && el > 0x10000) {
      uint32_t vmH = SafeRead<uint32_t>(weapon + 0x1688);
      if (vmH && vmH != 0xFFFFFFFF) {
        // GetPawnByHandle
        uintptr_t vm = GetPawnByHandle(el, vmH);
        if (vm && vm > 0x10000) {
          uintptr_t vsn = SafeRead<uintptr_t>(vm + Off::m_pGameSceneNode);
          if (vsn && vsn > 0x10000) {
            SafeWrite<uint64_t>(vsn + 0x150 + 0xA0, kmd.hModel);
            SafeWrite<uint64_t>(vsn + 0x150 + 0xA8, kmd.modelName);
          }
        }
      }
    }
  }


  // Attribute
  // CEconItemAttribute
  #pragma pack(push,1)
  struct CEconItemAttribute {
    uintptr_t vtable;
    uintptr_t owner;
    char      pad[32];
    uint16_t  defIndex;
    char      pad2[2];
    float     value;
    float     initValue;
    int32_t   refundableCurrency;
    bool      setBonus;
    char      pad3[7];
  };
  #pragma pack(pop)

  struct CPtrGameVector {
    uint64_t  size;
    uintptr_t ptr;
  };

  inline CEconItemAttribute* g_attrBuffer  = nullptr;
  inline bool                g_attrWritten = false; // Biz

  inline CEconItemAttribute MakeAttr(uint16_t def, float val) {
    CEconItemAttribute a{};
    a.defIndex  = def;
    a.value     = val;
    a.initValue = val;
    return a;
  }

  // CreateAttributes
  // Onceki
  // calisip
  inline void CreateAttributes(uintptr_t item, int kit, int seed, float wear) {
    g_attrWritten = false;
    if (kit <= 0) return;

    if (!g_attrBuffer) {
      g_attrBuffer = reinterpret_cast<CEconItemAttribute*>(
        VirtualAlloc(nullptr, sizeof(CEconItemAttribute) * 3,
                     MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
      if (!g_attrBuffer) return;
    }
    // CS2
    // Seed
    g_attrBuffer[0] = MakeAttr(6, (float)kit);
    g_attrBuffer[1] = MakeAttr(8, wear);
    g_attrBuffer[2] = MakeAttr(7, (float)(seed > 0 ? seed : 1));

    uintptr_t listAddr = item + Off::m_AttributeList + Off::m_Attributes;
    CPtrGameVector nv;
    nv.size = 3;
    nv.ptr  = reinterpret_cast<uintptr_t>(g_attrBuffer);
    SafeWrite<CPtrGameVector>(listAddr, nv);
    g_attrWritten = true;
  }

  // RemoveAttributes
  // Onceki
  inline void RemoveAttributes(uintptr_t item) {
    if (!g_attrWritten) return;
    uintptr_t listAddr = item + Off::m_AttributeList + Off::m_Attributes;
    SafeWrite<CPtrGameVector>(listAddr, CPtrGameVector{});
    g_attrWritten = false;
  }

  // RegenerateWeaponSkins
  inline uintptr_t regenAddr    = 0;
  inline bool      regenPatched = false;

  inline uintptr_t SigScan(uintptr_t base, size_t sz, const char* pat) {
    struct BE { uint8_t byte; bool wildcard; };
    std::vector<BE> sig;
    const char* p = pat;
    while (*p) {
      if (*p == ' ') { p++; continue; }
      if (*p == '?') {
        sig.push_back({0, true});
        while (*p == '?') p++;
      } else {
        sig.push_back({(uint8_t)strtoul(p, const_cast<char**>(&p), 16), false});
      }
    }
    uint8_t* bytes = reinterpret_cast<uint8_t*>(base);
    for (size_t i = 0; i < sz - sig.size(); i++) {
      bool found = true;
      for (size_t j = 0; j < sig.size(); j++) {
        if (!sig[j].wildcard && bytes[i+j] != sig[j].byte) { found = false; break; }
      }
      if (found) return base + i;
    }
    return 0;
  }

  inline void InitRegen() {
    if (regenAddr) return;
    HMODULE hClient = GetModuleHandleW(L"client.dll");
    if (!hClient) return;
    MODULEINFO mi{};
    if (!GetModuleInformation(GetCurrentProcess(), hClient, &mi, sizeof(mi))) return;

    regenAddr = SigScan(reinterpret_cast<uintptr_t>(hClient), mi.SizeOfImage,
      "48 83 EC ? E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 10");

    if (regenAddr) {
      // Patch
      uint16_t combined = (uint16_t)(Off::m_AttrMgr + Off::m_Item +
                                     Off::m_AttributeList + Off::m_Attributes);
      DWORD old;
      if (VirtualProtect(reinterpret_cast<void*>(regenAddr + 0x52), 2, PAGE_EXECUTE_READWRITE, &old)) {
        *reinterpret_cast<uint16_t*>(regenAddr + 0x52) = combined;
        VirtualProtect(reinterpret_cast<void*>(regenAddr + 0x52), 2, old, &old);
        regenPatched = true;
        printf("[SkinChanger] Regen found+patched at 0x%llX\n", regenAddr);
      }
    } else {
      printf("[SkinChanger] RegenerateWeaponSkins NOT found\n");
    }
  }

  inline void CallRegen() {
    if (!regenAddr || !regenPatched) return;
    __try {
      typedef void(__fastcall* RegenFn)();
      reinterpret_cast<RegenFn>(regenAddr)();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
      printf("[SkinChanger] Regen SEH exception\n");
    }
  }

  // Apply
  inline uintptr_t lastWeapon = 0;
  inline int       lastKit    = 0;

  inline void ApplyAndRegen(uintptr_t weapon, const SkinConfig& skin, uint16_t /*def*/) {
    uintptr_t item = weapon + Off::m_AttrMgr + Off::m_Item;

    // Custom
    SafeWrite<uint32_t>(item + Off::m_iItemIDHigh, 0xFFFFFFFF);
    SafeWrite<uint32_t>(item + Off::m_iItemIDLow,  1);
    SafeWrite<uint64_t>(item + Off::m_iItemID,     0xFFFFFFFFFFFFFFFFull);

    // Fallback
    SafeWrite<int32_t>(weapon + Off::m_nFbPaintKit, skin.paintKit);
    SafeWrite<float>  (weapon + Off::m_flFbWear,    skin.wear);
    SafeWrite<int32_t>(weapon + Off::m_nFbSeed,     skin.seed);
    if (skin.statTrak >= 0)
      SafeWrite<int32_t>(weapon + Off::m_nFbStatTrak, skin.statTrak);

    // Bicak
    if (skin.knifeModel > 0) {
      SafeWrite<uint16_t>(item + Off::m_iItemDefIdx,    (uint16_t)skin.knifeModel);
      SafeWrite<int32_t> (item + Off::m_iEntityQuality, 3);
      // el
    }

    CreateAttributes(item, skin.paintKit, skin.seed, skin.wear);
    CallRegen();
    RemoveAttributes(item);
  }

  // Main
  inline void Tick() {
    if (!G.client || !G.localPawn || G.localPawn <= 0x10000) return;

    uint8_t life = SafeRead<uint8_t>(G.localPawn + Off::m_lifeState);
    int32_t hp   = SafeRead<int32_t>(G.localPawn + Off::m_iHealth);
    if (life != 0 || hp <= 0) { lastWeapon = 0; lastKit = 0; return; }

    InitRegen();

    static int tickCounter = 0;
    if (++tickCounter >= 2000) {
      forceUpdate.store(true);
      tickCounter = 0;
    }

    bool force = forceUpdate.load();
    std::lock_guard<std::mutex> lock(configMutex);

    // Aktif
    // m_pClippingWeapon
    uintptr_t wSvc = SafeRead<uintptr_t>(G.localPawn + Off::m_pWeaponSvc);
    if (!wSvc || wSvc <= 0x10000) return;
    uint32_t wHandle = SafeRead<uint32_t>(wSvc + Off::m_hActiveWeapon);
    if (!wHandle || wHandle == 0xFFFFFFFF) return;
    uintptr_t el = SafeRead<uintptr_t>(G.client + Off::dwEntityList);
    if (!el || el <= 0x10000) return;
    uintptr_t activeWeapon = GetPawnByHandle(el, wHandle);
    if (!activeWeapon || activeWeapon <= 0x10000) return;

    uintptr_t item = activeWeapon + Off::m_AttrMgr + Off::m_Item;
    uint16_t  def  = SafeRead<uint16_t>(item + Off::m_iItemDefIdx);
    bool isWep = (def > 0 && def < 70) || (def >= 500 && def < 600);
    if (!isWep || def == 31) return; // Zeus

    // Knife
    // def
    // Koleksiyon
    int lookupIndex = def;
    if (def >= 500 && def < 600) {
      // Fancy
      if (!weaponSkins.count(def) || !weaponSkins[def].enabled)
        lookupIndex = 42;
    }
    // def

    auto it = weaponSkins.find(lookupIndex);
    if (it == weaponSkins.end() || !it->second.enabled) return;
    // paintKit
    if (it->second.paintKit <= 0 && it->second.knifeModel <= 0) return;

    const SkinConfig& skin = it->second;
    static int   lastKnife = 0;
    static int   lastSeed  = -999;
    static float lastWear  = -1.f;  // Wear
    bool needApply = force
        || (activeWeapon != lastWeapon)
        || (skin.paintKit != lastKit)
        || (skin.knifeModel != lastKnife)
        || (skin.seed != lastSeed)
        || (skin.wear != lastWear);

    // Bicak
    if (skin.knifeModel > 0) {
      uintptr_t kitem = activeWeapon + Off::m_AttrMgr + Off::m_Item;
      SafeWrite<uint16_t>(kitem + Off::m_iItemDefIdx,    (uint16_t)skin.knifeModel);
      SafeWrite<int32_t> (kitem + Off::m_iEntityQuality, 3);
      ApplyKnifeModel(activeWeapon, skin.knifeModel, el);
    }

    if (!needApply) return;

    // Canli
    life = SafeRead<uint8_t>(G.localPawn + Off::m_lifeState);
    hp   = SafeRead<int32_t>(G.localPawn + Off::m_iHealth);
    if (life != 0 || hp <= 0) return;

    ApplyAndRegen(activeWeapon, skin, def);

    lastWeapon = activeWeapon;
    lastKit    = skin.paintKit;
    lastKnife  = skin.knifeModel;
    lastSeed   = skin.seed;
    lastWear   = skin.wear;

    if (force) forceUpdate.store(false);
  }
}
