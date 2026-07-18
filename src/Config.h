#pragma once
#include "Features.h"
#include <string>
#include <fstream>
#include <sstream>

//
// CONFIG
//

static void WriteB(FILE* f, const char* k, bool v)   { fprintf(f, "%s=%d\n", k, (int)v); }
static void WriteI(FILE* f, const char* k, int v)    { fprintf(f, "%s=%d\n", k, v); }
static void WriteF(FILE* f, const char* k, float v)  { fprintf(f, "%s=%.4f\n", k, v); }
static void WriteC(FILE* f, const char* k, float* c) { fprintf(f, "%s=%.4f,%.4f,%.4f\n", k, c[0], c[1], c[2]); }

static void ReadLine(const std::string& line, const char* key, bool& out) {
    std::string k(key); k += '=';
    if (line.substr(0, k.size()) == k) out = (line[k.size()] == '1');
}
static void ReadLine(const std::string& line, const char* key, int& out) {
    std::string k(key); k += '=';
    if (line.substr(0, k.size()) == k) out = std::stoi(line.substr(k.size()));
}
static void ReadLine(const std::string& line, const char* key, float& out) {
    std::string k(key); k += '=';
    if (line.substr(0, k.size()) == k) out = std::stof(line.substr(k.size()));
}
static void ReadLineC(const std::string& line, const char* key, float* out) {
    std::string k(key); k += '=';
    if (line.substr(0, k.size()) == k) {
        std::string v = line.substr(k.size());
        float r,g,b;
        if (sscanf_s(v.c_str(), "%f,%f,%f", &r, &g, &b) == 3) { out[0]=r; out[1]=g; out[2]=b; }
    }
}

static const char* CFG_PATH = "QuaFind.cfg";

inline void SaveConfig() {
    FILE* f = nullptr;
    fopen_s(&f, CFG_PATH, "w");
    if (!f) return;

    fprintf(f, "[ESP]\n");
    WriteB(f,"espOn",Cfg::espOn); WriteB(f,"espBox",Cfg::espBox);
    WriteB(f,"espName",Cfg::espName); WriteB(f,"espHp",Cfg::espHp);
    WriteB(f,"espBones",Cfg::espBones); WriteB(f,"espHead",Cfg::espHead);
    WriteB(f,"espSnap",Cfg::espSnap); WriteB(f,"espWeapon",Cfg::espWeapon);
    WriteB(f,"espHint",Cfg::espHint); WriteB(f,"espTeam",Cfg::espTeam);
    WriteI(f,"boxStyle",Cfg::boxStyle);
    WriteC(f,"colBoxVis",Cfg::colBoxVis);  WriteC(f,"colBoxOcc",Cfg::colBoxOcc);
    WriteC(f,"colBone",Cfg::colBone); WriteC(f,"colHead",Cfg::colHead); WriteC(f,"colSnap",Cfg::colSnap);

    fprintf(f,"\n[Glow]\n");
    WriteB(f,"glowOn",Cfg::glowOn); WriteI(f,"glowKey",Cfg::glowKey);
    WriteC(f,"glowVis",Cfg::glowVis); WriteC(f,"glowWall",Cfg::glowWall);
    WriteF(f,"glowAlpha",Cfg::glowAlpha);

    fprintf(f,"\n[Chams]\n");
    WriteB(f,"chamsOn",Cfg::chamsOn);
    WriteI(f,"chamsKey",Cfg::chamsKey);
    WriteC(f,"chamsVis",Cfg::chamsVis); WriteC(f,"chamsOcc",Cfg::chamsOcc);
    WriteF(f,"chamsAlpha",Cfg::chamsAlpha);

    fprintf(f,"\n[Aimbot]\n");
    WriteB(f,"aimOn",Cfg::aimOn); WriteI(f,"aimKey",Cfg::aimKey);
    WriteB(f,"aimNeedKey",Cfg::aimNeedKey); WriteI(f,"aimBone",Cfg::aimBone);
    WriteF(f,"aimFov",Cfg::aimFov); WriteF(f,"aimSmooth",Cfg::aimSmooth);
    WriteB(f,"aimLock",Cfg::aimLock); WriteB(f,"aimWall",Cfg::aimWall);

    fprintf(f,"\n[Rage]\n");
    WriteB(f,"rageOn",Cfg::rageOn); WriteI(f,"rageKey",Cfg::rageKey);
    WriteB(f,"rageNeedKey",Cfg::rageNeedKey); WriteI(f,"rageBone",Cfg::rageBone);
    WriteF(f,"rageFov",Cfg::rageFov); WriteF(f,"rageSmooth",Cfg::rageSmooth);
    WriteB(f,"rageAuto",Cfg::rageAuto); WriteB(f,"rageWall",Cfg::rageWall);

    fprintf(f,"\n[Trigger]\n");
    WriteB(f,"trigOn",Cfg::trigOn); WriteI(f,"trigKey",Cfg::trigKey);
    WriteB(f,"trigNeedKey",Cfg::trigNeedKey); WriteB(f,"trigWall",Cfg::trigWall);
    WriteF(f,"trigDelay",Cfg::trigDelay); WriteI(f,"trigFireTime",Cfg::trigFireTime);

    fprintf(f,"\n[Misc]\n");
    WriteB(f,"noRecoil",Cfg::noRecoil); WriteB(f,"rcsOn",Cfg::rcsOn);
    WriteF(f,"rcsScale",Cfg::rcsScale); WriteB(f,"noSpread",Cfg::noSpread);
    WriteB(f,"bhopOn",Cfg::bhopOn); WriteB(f,"noFlash",Cfg::noFlash);
    WriteB(f,"thirdPerson",Cfg::thirdPerson); WriteB(f,"noScope",Cfg::noScope);
    WriteB(f,"skyboxChangerOn",Cfg::skyboxChangerOn); WriteC(f,"skyboxColor",Cfg::skyboxColor);

    fprintf(f,"\n[AntiAim]\n");
    WriteB(f,"aaOn",Cfg::aaOn); WriteI(f,"aaMode",Cfg::aaMode);
    WriteI(f,"aaKey",Cfg::aaKey); WriteF(f,"aaYawAdd",Cfg::aaYawAdd);
    WriteF(f,"aaSpinSpeed",Cfg::aaSpinSpeed);

    fprintf(f,"\n[UtilESP]\n");
    WriteB(f,"grenadeESP",Cfg::grenadeESP); WriteB(f,"molotovESP",Cfg::molotovESP);
    WriteB(f,"bombCam",Cfg::bombCam);

    fclose(f);
    printf("[CFG] Saved to %s\n", CFG_PATH);
}

inline void LoadConfig() {
    std::ifstream fs(CFG_PATH);
    if (!fs.is_open()) return;
    std::string line;
    while (std::getline(fs, line)) {
        if (line.empty() || line[0]=='[' || line[0]==';') continue;
        try {
            ReadLine(line,"espOn",Cfg::espOn); ReadLine(line,"espBox",Cfg::espBox);
            ReadLine(line,"espName",Cfg::espName); ReadLine(line,"espHp",Cfg::espHp);
            ReadLine(line,"espBones",Cfg::espBones); ReadLine(line,"espHead",Cfg::espHead);
            ReadLine(line,"espSnap",Cfg::espSnap); ReadLine(line,"espWeapon",Cfg::espWeapon);
            ReadLine(line,"espHint",Cfg::espHint); ReadLine(line,"espTeam",Cfg::espTeam);
            ReadLine(line,"boxStyle",Cfg::boxStyle);
            ReadLineC(line,"colBoxVis",Cfg::colBoxVis); ReadLineC(line,"colBoxOcc",Cfg::colBoxOcc);
            ReadLineC(line,"colBone",Cfg::colBone); ReadLineC(line,"colHead",Cfg::colHead); ReadLineC(line,"colSnap",Cfg::colSnap);
            ReadLine(line,"glowOn",Cfg::glowOn); ReadLine(line,"glowKey",Cfg::glowKey);
            ReadLineC(line,"glowVis",Cfg::glowVis); ReadLineC(line,"glowWall",Cfg::glowWall);
            ReadLine(line,"glowAlpha",Cfg::glowAlpha);
            ReadLine(line,"chamsOn",Cfg::chamsOn);
            ReadLine(line,"chamsKey",Cfg::chamsKey);
            ReadLineC(line,"chamsVis",Cfg::chamsVis); ReadLineC(line,"chamsOcc",Cfg::chamsOcc);
            ReadLine(line,"chamsAlpha",Cfg::chamsAlpha);
            ReadLine(line,"aimOn",Cfg::aimOn); ReadLine(line,"aimKey",Cfg::aimKey);
            ReadLine(line,"aimNeedKey",Cfg::aimNeedKey); ReadLine(line,"aimBone",Cfg::aimBone);
            ReadLine(line,"aimFov",Cfg::aimFov); ReadLine(line,"aimSmooth",Cfg::aimSmooth);
            ReadLine(line,"aimLock",Cfg::aimLock); ReadLine(line,"aimWall",Cfg::aimWall);
            ReadLine(line,"rageOn",Cfg::rageOn); ReadLine(line,"rageKey",Cfg::rageKey);
            ReadLine(line,"rageNeedKey",Cfg::rageNeedKey); ReadLine(line,"rageBone",Cfg::rageBone);
            ReadLine(line,"rageFov",Cfg::rageFov); ReadLine(line,"rageSmooth",Cfg::rageSmooth);
            ReadLine(line,"rageAuto",Cfg::rageAuto); ReadLine(line,"rageWall",Cfg::rageWall);
            ReadLine(line,"trigOn",Cfg::trigOn); ReadLine(line,"trigKey",Cfg::trigKey);
            ReadLine(line,"trigNeedKey",Cfg::trigNeedKey); ReadLine(line,"trigWall",Cfg::trigWall);
            ReadLine(line,"trigDelay",Cfg::trigDelay); ReadLine(line,"trigFireTime",Cfg::trigFireTime);
            ReadLine(line,"noRecoil",Cfg::noRecoil); ReadLine(line,"rcsOn",Cfg::rcsOn);
            ReadLine(line,"rcsScale",Cfg::rcsScale); ReadLine(line,"noSpread",Cfg::noSpread);
            ReadLine(line,"bhopOn",Cfg::bhopOn); ReadLine(line,"noFlash",Cfg::noFlash);
            ReadLine(line,"thirdPerson",Cfg::thirdPerson); ReadLine(line,"noScope",Cfg::noScope);
            ReadLine(line,"skyboxChangerOn",Cfg::skyboxChangerOn); ReadLineC(line,"skyboxColor",Cfg::skyboxColor);
            ReadLine(line,"aaOn",Cfg::aaOn); ReadLine(line,"aaMode",Cfg::aaMode);
            ReadLine(line,"aaKey",Cfg::aaKey); ReadLine(line,"aaYawAdd",Cfg::aaYawAdd);
            ReadLine(line,"aaSpinSpeed",Cfg::aaSpinSpeed);
            ReadLine(line,"grenadeESP",Cfg::grenadeESP); ReadLine(line,"molotovESP",Cfg::molotovESP);
            ReadLine(line,"bombCam",Cfg::bombCam);
        } catch(...) {}
    }
    printf("[CFG] Loaded from %s\n", CFG_PATH);
}
