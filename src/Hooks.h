#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <atomic>
#include <unordered_map>
#include <Psapi.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"psapi.lib")
#include "hook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include "SDK.h"
#include "VisualExtras.h"
#include "Features.h"
#include "SkinChanger.h"
#include "WeaponChams.h"
// FovChanger
#include "ConfigSystem.h"
#include "weaponFont_embed.h"  // TTF
#include "HitMarker.h"          // Hitmarker
#include "BombTimer.h"          // C4
#include "CrashLogger.h"        // Crash
#include <cstdarg>              // ILog
#include <shlobj.h>             // SHGetFolderPathA
#include <string>
#include <vector>
#include <utility>

// Pattern scanner
static std::vector<std::pair<uint8_t, bool>> PatternToBytes(const std::string& pattern) {
  std::vector<std::pair<uint8_t, bool>> pb;
  const char* s = pattern.c_str();
  const char* e = s + pattern.size();
  for (const char* c = s; c < e; ++c) {
    if (*c == ' ') continue;
    if (*c == '?') { pb.emplace_back(0, false); if (*(c+1) == '?') ++c; }
    else { pb.emplace_back((uint8_t)strtoul(c, nullptr, 16), true); if (*(c+1) != ' ') ++c; }
  }
  return pb;
}
static uintptr_t TargetPatternScan(const std::string& pattern, const std::string& modStr) {
  if (modStr.empty()) return 0;
  auto mod = GetModuleHandleA(modStr.c_str());
  if (!mod) return 0;
  MODULEINFO mi{};
  GetModuleInformation(GetCurrentProcess(), mod, &mi, sizeof(mi));
  size_t sz = mi.SizeOfImage;
  uintptr_t base = (uintptr_t)mi.lpBaseOfDll;
  auto pb = PatternToBytes(pattern);
  size_t pl = pb.size();
  for (size_t i = 0; i < sz - pl; ++i) {
    bool ok = true;
    for (size_t j = 0; j < pl; ++j) {
      if (pb[j].second && pb[j].first != *reinterpret_cast<uint8_t*>(base + i + j)) { ok = false; break; }
    }
    if (ok) return base + i;
  }
  return 0;
}

// Skybox hook
using DrawSkyboxArrayFn = void(__fastcall*)(void*,void*,void*,int,void*,void*,void*);
static DrawSkyboxArrayFn g_oDrawSkyboxArray = nullptr;
static bool  g_drawSkyHooked       = false;
static ULONGLONG g_lastSkyHookAttempt = 0;

static void __fastcall hkDrawSkyboxArray(void* a1,void* a2,void* draw_primitive,int count,void* a5,void* a6,void* a7) {
  if (Cfg::skyboxChangerOn && draw_primitive) {
    uintptr_t dp = (uintptr_t)draw_primitive;
    uintptr_t skyboxObj = SafeRead<uintptr_t>(dp + 0x18);
    if (skyboxObj >= 0x10000 && skyboxObj < 0x00007FFFFFFFFFFFull && (skyboxObj % 8 == 0)) {
      uintptr_t colorAddr = skyboxObj + 0xE8;
      SafeWrite<float>(colorAddr,     Cfg::skyboxColor[0]);
      SafeWrite<float>(colorAddr + 4, Cfg::skyboxColor[1]);
      SafeWrite<float>(colorAddr + 8, Cfg::skyboxColor[2]);
    }
  }
  if (g_oDrawSkyboxArray)
    g_oDrawSkyboxArray(a1, a2, draw_primitive, count, a5, a6, a7);
}

static void EnsureSkyboxHook() {
  if (!Cfg::skyboxChangerOn) return;
  if (g_drawSkyHooked) return;
  ULONGLONG now = GetTickCount64();
  if (now - g_lastSkyHookAttempt < 2000) return;
  g_lastSkyHookAttempt = now;
  HMODULE hScene = GetModuleHandleA("scenesystem.dll");
  if (!hScene) return;
  const char* pats[] = {
    "45 85 C9 0F 8E ? ? ? ? 4C 8B DC 55 41 56 49",
    "45 85 C9 0F 8E ? ? ? ? 4C 8B DC 55",
    "45 85 C9 0F 8E ? ? ? ? 4C 8B DC",
  };
  uintptr_t addr = 0;
  for (int i = 0; i < 3; i++) { addr = TargetPatternScan(pats[i], "scenesystem.dll"); if (addr) break; }
  if (addr) {
    if (MH_CreateHook((LPVOID)addr, (void*)hkDrawSkyboxArray, (void**)&g_oDrawSkyboxArray) == MH_OK) {
      MH_EnableHook((LPVOID)addr);
      g_drawSkyHooked = true;
    }
  }
}

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND,UINT,WPARAM,LPARAM);


typedef HRESULT(__stdcall* PFN_Present)(IDXGISwapChain*,UINT,UINT);
typedef HRESULT(__stdcall* PFN_Resize)(IDXGISwapChain*,UINT,UINT,UINT,DXGI_FORMAT,UINT);

static PFN_Present            oPresent     = nullptr;
static PFN_Resize             oResize      = nullptr;

static HWND                   g_Wnd        = nullptr;
static WNDPROC                g_WndProcOld = nullptr;
static ID3D11Device*          g_Dev        = nullptr;
static ID3D11DeviceContext*   g_Ctx        = nullptr;
static ID3D11RenderTargetView* g_RTV       = nullptr;
static bool                   g_Init       = false;
static std::atomic<bool>      g_NeedReinit{false};
static int                    g_ReinitDelay = 0; // Resize

static float s_FlashDurCached = 0.f;
static int g_Tab = 0;


static bool  g_introShown  = false;
static float g_introTimer  = 0.f;
static const float kIntroDur = 4.5f; // saniye

// Weapon
static ImFont* g_weaponFonts[4] = {}; // 0
inline ImFont* GetWeaponFont(){
  float s=Cfg::weaponIconSize;
  if(s<10.f) return g_weaponFonts[0];
  if(s<14.f) return g_weaponFonts[1];
  if(s<18.f) return g_weaponFonts[2];
  return g_weaponFonts[3];
}

// Menu
static float g_menuAnim        = 0.0f;  // 0
static bool  g_menuTarget      = false; // hedef
static bool  g_windowArrangeMode = false; // pencere


static ImVec4 cBG   = {0.043f,0.043f,0.059f,0.97f};
static ImVec4 cPane = {0.063f,0.063f,0.082f,1.f};
static ImVec4 cCard = {0.082f,0.082f,0.110f,1.f};
static ImVec4 cHdr  = {0.110f,0.110f,0.150f,1.f};
static ImVec4 cAcc  = {0.180f,0.800f,0.443f,1.f};  // 2ECC71
static ImVec4 cHov  = {0.298f,0.886f,0.557f,1.f};  // acik
static ImVec4 cActv = {0.100f,0.340f,0.200f,1.f};  // koyu
static ImVec4 cBdr  = {0.130f,0.130f,0.200f,1.f};
static ImVec4 cTxt  = {0.929f,0.937f,0.957f,1.f};
static ImVec4 cDim  = {0.450f,0.470f,0.540f,1.f};
static ImU32  uAcc  = IM_COL32(46,204,113,255);     // 2ECC71
static ImU32  uAcc2 = IM_COL32(76,226,143,150);     // acik
static ImU32  uBdr  = IM_COL32(33,33,51,255);

inline const char* KeyName(int vk){
  if(!vk)return"YOK";
  switch(vk){
    case VK_LBUTTON:return"M1";case VK_RBUTTON:return"M2";
    case VK_MBUTTON:return"M3";case VK_XBUTTON1:return"M4";
    case VK_XBUTTON2:return"M5";case VK_MENU:return"ALT";
    case VK_SHIFT:return"SHIFT";case VK_CONTROL:return"CTRL";
    case VK_SPACE:return"SPACE";case VK_ESCAPE:return"ESC";
    case VK_RETURN:return"ENTER";case VK_DELETE:return"DEL";
  }
  if(vk>='A'&&vk<='Z'){static char b[3];b[0]=(char)vk;b[1]=0;return b;}
  if(vk>='0'&&vk<='9'){static char b[3];b[0]=(char)vk;b[1]=0;return b;}
  if(vk>=VK_F1&&vk<=VK_F12){static char b[5];sprintf_s(b,"F%d",vk-VK_F1+1);return b;}
  static char b[8];sprintf_s(b,"0x%02X",vk);return b;
}

LRESULT __stdcall WndProc(HWND h,UINT m,WPARAM w,LPARAM l){
  if(Cfg::menuOpen || g_windowArrangeMode){
    if(m==WM_SETCURSOR){SetCursor(LoadCursor(NULL,IDC_ARROW));return TRUE;}
    ImGui_ImplWin32_WndProcHandler(h,m,w,l);
    switch(m){
      case WM_LBUTTONDOWN:case WM_LBUTTONUP:
      case WM_RBUTTONDOWN:case WM_RBUTTONUP:
      case WM_MOUSEWHEEL:case WM_MOUSEMOVE:return 0;
    }
  }
  return CallWindowProcW(g_WndProcOld,h,m,w,l);
}

HRESULT __stdcall hkResize(IDXGISwapChain* sc,UINT bc,UINT W,UINT H,DXGI_FORMAT f,UINT fl){
  if(g_RTV&&g_Ctx) g_Ctx->OMSetRenderTargets(0,nullptr,nullptr);
  if(g_RTV){g_RTV->Release();g_RTV=nullptr;}
  g_NeedReinit=true;
  HRESULT hr=oResize(sc,bc,W,H,f,fl);
  if(SUCCEEDED(hr)){if(W>0)G.W=(int)W;if(H>0)G.H=(int)H;}
  return hr;
}

static FILE* g_iLog = nullptr;
static void ILog(const char* fmt, ...) {
  // Konsola
  va_list a; va_start(a,fmt); vprintf(fmt,a); va_end(a);
  // Dosyaya
  if (!g_iLog) {
    char path[MAX_PATH]={};
    SHGetFolderPathA(nullptr,CSIDL_PERSONAL,nullptr,0,path);
    if (!path[0]) strcpy_s(path,"C:\\Users\\Public");
    strcat_s(path,"\\QuaFind_init.log");
    fopen_s(&g_iLog,path,"w"); // her
  }
  if (g_iLog) {
    va_list b; va_start(b,fmt); vfprintf(g_iLog,fmt,b); va_end(b);
    fflush(g_iLog);
  }
}

static void ImGuiShutdown(){
  if(!g_Init)return;
  ImGui_ImplDX11_Shutdown(); ImGui_ImplWin32_Shutdown(); ImGui::DestroyContext();
  if(g_RTV){g_RTV->Release();g_RTV=nullptr;}
  g_Init=false;
  if(g_iLog){fclose(g_iLog);g_iLog=nullptr;} // FILE
}

static bool ImGuiInit(IDXGISwapChain* sc){
  ILog("[INIT] ImGuiInit basladi sc=%p\n", sc);

    ID3D11Texture2D* bb=nullptr;
  HRESULT hr = sc->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&bb);
  if(FAILED(hr)){ILog("[INIT] GetBuffer FAIL 0x%08X\n",(unsigned)hr);return false;}
  ILog("[INIT] GetBuffer OK bb=%p\n",bb);

    g_Dev = nullptr;
  sc->GetDevice(__uuidof(ID3D11Device),(void**)&g_Dev);
  if(!g_Dev){
    // Fallback
    IUnknown* unk=nullptr;
    sc->GetDevice(__uuidof(IUnknown),(void**)&unk);
    if(unk){
      unk->QueryInterface(__uuidof(ID3D11Device),(void**)&g_Dev);
      unk->Release();
    }
  }
  if(!g_Dev){
    ILog("[INIT] GetDevice FAIL g_Dev null! D3D11 swapchain degil");
    bb->Release(); return false;
  }
  ILog("[INIT] GetDevice OK g_Dev=%p\n",g_Dev);

  g_Dev->GetImmediateContext(&g_Ctx);
  if(!g_Ctx){ILog("[INIT] GetImmediateContext FAIL\n");bb->Release();return false;}
  ILog("[INIT] g_Ctx=%p\n",g_Ctx);

    DXGI_SWAP_CHAIN_DESC scd{};
  hr = sc->GetDesc(&scd);
  if(SUCCEEDED(hr)){
    g_Wnd=scd.OutputWindow;
    if(scd.BufferDesc.Width)  G.W=(int)scd.BufferDesc.Width;
    if(scd.BufferDesc.Height) G.H=(int)scd.BufferDesc.Height;
    ILog("[INIT] SwapChain %dx%d fmt=%d wnd=%p\n",G.W,G.H,(int)scd.BufferDesc.Format,g_Wnd);
  } else {
    // GetDesc
    ILog("[INIT] GetDesc FAIL 0x%08X, pencere manuel aranacak\n",(unsigned)hr);
    if(!g_Wnd) g_Wnd=FindWindowA("SDL_app",nullptr);
    if(!g_Wnd) g_Wnd=FindWindowA(nullptr,"Counter-Strike 2");
    ILog("[INIT] Manuel wnd=%p\n",g_Wnd);
    if(g_Wnd){RECT rc;GetClientRect(g_Wnd,&rc);G.W=rc.right;G.H=rc.bottom;}
  }
  if(!g_Wnd){ILog("[INIT] g_Wnd NULL!\n");bb->Release();return false;}
  if(G.W<=0||G.H<=0){G.W=1920;G.H=1080;}

    if(g_RTV){g_RTV->Release();g_RTV=nullptr;}
  hr = g_Dev->CreateRenderTargetView(bb,nullptr,&g_RTV);
  bb->Release();
  if(FAILED(hr)||!g_RTV){ILog("[INIT] CreateRTV FAIL 0x%08X\n",(unsigned)hr);return false;}
  ILog("[INIT] RTV OK\n");

   WNDPROC cur=(WNDPROC)GetWindowLongPtrW(g_Wnd,GWLP_WNDPROC);
  if(cur!=WndProc){g_WndProcOld=cur;SetWindowLongPtrW(g_Wnd,GWLP_WNDPROC,(LONG_PTR)WndProc);}

   ImGui::CreateContext();
  ImGuiIO& io=ImGui::GetIO();
  io.ConfigFlags|=ImGuiConfigFlags_NoMouseCursorChange;
  io.MouseDrawCursor=false; io.IniFilename=nullptr;

  // Turkce
  static const ImWchar tr_ranges[]={
    0x0020,0x00FF, // Basic
    0x011E,0x011F, // G
    0x0130,0x0131, // I
    0x015E,0x015F, // S
    0x21BA,0x21BA, // Yenile
    0,
  };
  ImFont* font=io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf",16.f,nullptr,tr_ranges);
  if(!font) font=io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf",16.f,nullptr,tr_ranges);
  if(!font){ ILog("[INIT] Sistem fontu bulunamadi, varsayilan kullaniliyor\n"); io.Fonts->AddFontDefault(); }
  else ILog("[INIT] Font yuklendi OK\n");

  // weaponIcons
  {
    static const ImWchar wR[]={0x0020,0x007F,0};
    static const float sz[]={8.f,12.f,16.f,20.f};
    for(int si=0;si<4;si++){
      void* fc=malloc(g_weaponIconsFontSize);
      if(fc){ memcpy(fc,g_weaponIconsFontData,g_weaponIconsFontSize);
              g_weaponFonts[si]=io.Fonts->AddFontFromMemoryTTF(fc,g_weaponIconsFontSize,sz[si],nullptr,wR); }
    }
  }
  io.Fonts->Build();

  if(!ImGui_ImplWin32_Init(g_Wnd)){ILog("[INIT] ImplWin32_Init FAIL\n");ImGui::DestroyContext();return false;}
  if(!ImGui_ImplDX11_Init(g_Dev,g_Ctx)){ILog("[INIT] ImplDX11_Init FAIL\n");ImGui_ImplWin32_Shutdown();ImGui::DestroyContext();return false;}

  G.client=(uintptr_t)GetModuleHandleA("client.dll");
  ILog("[INIT] === BASARILI === client=0x%llX %dx%d wnd=%p\n",G.client,G.W,G.H,g_Wnd);
  return true;
}


static bool Toggle(const char* id,bool* v){
  ImVec2 p=ImGui::GetCursorScreenPos();
  ImDrawList* dl=ImGui::GetWindowDrawList();
  const float W=42.f,H=20.f,R=H*.5f;
  ImGui::InvisibleButton(id,{W,H});
  if(ImGui::IsItemClicked()) *v=!*v;
  ImU32 bg  = *v ? IM_COL32(46,204,113,235) : IM_COL32(26,26,38,220);
  ImU32 rim = *v ? IM_COL32(76,226,143,200) : IM_COL32(55,55,75,200);
  ImU32 dot = *v ? IM_COL32(255,255,255,245): IM_COL32(90,90,115,220);
  float cx  = p.x + (*v ? W-R : R);
  dl->AddRectFilled(p,{p.x+W,p.y+H},bg,R);
  dl->AddRect(p,{p.x+W,p.y+H},rim,R,0,1.2f);
  dl->AddCircleFilled({cx,p.y+R},R-2.8f,dot);
  return *v;
}

static void TRow(const char* label,bool* v){
  float rw=ImGui::GetContentRegionAvail().x;
  ImGui::TextColored(*v?cTxt:cDim,"%s",label);
  ImGui::SameLine(rw-100.f);
  ImGui::TextColored(*v?ImVec4(.3f,.9f,.5f,.9f):ImVec4(.4f,.4f,.5f,.7f),
                     *v?u8"AKT\u0130F":"KAPALI");
  ImGui::SameLine();
  char id[64];snprintf(id,64,"##t_%s",label);
  Toggle(id,v);
}

static void ColBtn(const char* label,float col[3]){
  ImGui::TextColored(cDim,"%s",label);
  ImGui::SameLine(ImGui::GetCursorPosX()+ImGui::GetContentRegionAvail().x-28.f);
  char id[64];snprintf(id,64,"##c_%s",label);
  ImGui::SetNextItemWidth(24.f);
  ImGui::ColorEdit3(id,col,
    ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoLabel|
    ImGuiColorEditFlags_PickerHueWheel|ImGuiColorEditFlags_NoTooltip);
}

static void ColRow(const char* label,bool* v,float col[3]){
  float rw=ImGui::GetContentRegionAvail().x;
  ImGui::TextColored(*v?cTxt:cDim,"%s",label);
  ImGui::SameLine(rw-108.f);
  char cpId[64];snprintf(cpId,64,"##cp_%s",label);
  ImGui::SetNextItemWidth(24.f);
  ImGui::ColorEdit3(cpId,col,
    ImGuiColorEditFlags_NoInputs|
    ImGuiColorEditFlags_NoLabel|ImGuiColorEditFlags_NoTooltip|
    ImGuiColorEditFlags_PickerHueWheel);
  ImGui::SameLine();
  ImGui::TextColored(*v?ImVec4(.3f,.9f,.5f,.9f):ImVec4(.4f,.4f,.5f,.7f),
                     *v?u8"AKT\u0130F":"KAPALI");
  ImGui::SameLine();
  char tId[64];snprintf(tId,64,"##tv_%s",label);
  Toggle(tId,v);
}

static void Sli(const char* label,float* v,float mn,float mx,const char* fmt="%.1f"){
  ImGui::TextColored(cDim,"%s",label);
  ImGui::SetNextItemWidth(-1.f);
  char id[64];snprintf(id,64,"##s_%s",label);
  ImGui::PushStyleColor(ImGuiCol_SliderGrab,       cAcc);
  ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, cHov);
  ImGui::SliderFloat(id,v,mn,mx,fmt);
  ImGui::PopStyleColor(2);
}

static int* s_WaitForKey=nullptr;  // pointer-to-key
static bool s_KeyRel=false;
static void KeyBind(const char* label,int* key){
  bool waiting=(s_WaitForKey==key);
  char buf[80];
  if(waiting) snprintf(buf,80,"[ ... ]##k_%p",(void*)key);
  else         snprintf(buf,80,"[ %-7s ]##k_%p",KeyName(*key),(void*)key);
  ImGui::TextColored(cDim,"%s",label);ImGui::SameLine();
  float aw=ImGui::GetContentRegionAvail().x;
  if(aw>110) ImGui::SetCursorPosX(ImGui::GetCursorPosX()+aw-110.f);
  ImGui::PushStyleColor(ImGuiCol_Button, waiting?ImVec4(.1f,.5f,.25f,1.f):ImVec4(.1f,.1f,.18f,1.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(.12f,.34f,.2f,1.f));
  if(ImGui::Button(buf,{110,22})){
    if(!waiting){s_WaitForKey=key;s_KeyRel=false;} else s_WaitForKey=nullptr;
  }
  ImGui::PopStyleColor(2);
  if(waiting){
    // Once
    if(!s_KeyRel){
      bool any=false;
      for(int i=1;i<256;i++){
        if(i==VK_LBUTTON||i==VK_RBUTTON||i==VK_MBUTTON)continue;
        if(GetAsyncKeyState(i)&0x8000){any=true;break;}
      }
      if(!any)s_KeyRel=true;
    } else {
      for(int i=1;i<256;i++){
        if(i==VK_INSERT||i==VK_LBUTTON||i==VK_RBUTTON||i==VK_MBUTTON)continue;
        if(GetAsyncKeyState(i)&0x8000){
          *key=(i==VK_ESCAPE)?0:i;
          s_WaitForKey=nullptr;
          break;
        }
      }
    }
  }
}

static void Sep(){
  ImGui::PushStyleColor(ImGuiCol_Separator,cBdr);
  ImGui::Separator();ImGui::PopStyleColor();ImGui::Spacing();
}

template<typename Fn>
static void Card(const char* title,const char* id,float w,float /*h*/,Fn fn){
  static std::unordered_map<ImGuiID,float> s_cardH;
  ImGuiID hid=ImGui::GetCurrentWindow()->GetID(id);
  float ch=s_cardH.count(hid)?s_cardH[hid]:120.f;

  ImGui::PushStyleColor(ImGuiCol_ChildBg,cCard);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10.f);
  if(ImGui::BeginChild(id,{w,ch},false,
      ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse)){
    ImDrawList* dl=ImGui::GetWindowDrawList();
    ImVec2 p0=ImGui::GetWindowPos();
    dl->AddRectFilledMultiColor(p0,{p0.x+w,p0.y+3.f},uAcc,uAcc2,uAcc2,uAcc);
    ImGui::Dummy({0,7.f});
    ImGui::SetCursorPosX(10.f);
    ImGui::TextColored(cAcc,"%s",title);
    Sep();
    ImGui::SetCursorPosX(10.f);
    ImGui::BeginGroup();fn();ImGui::EndGroup();
    ImGui::Dummy({0,8.f});
    s_cardH[hid]=ImGui::GetCursorPosY();
  }
  ImGui::EndChild();
  ImGui::SetCursorPosX(20.f);
  ImGui::PopStyleVar();ImGui::PopStyleColor();
}

static void ApplyStyle(){
  ImGuiStyle& s=ImGui::GetStyle();
  s.WindowRounding=12.f;s.WindowPadding={0,0};
  s.ChildRounding=10.f; s.FrameRounding=7.f;
  s.ItemSpacing={8,6};  s.ScrollbarSize=5.f;
  s.GrabRounding=5.f;   s.PopupRounding=10.f;
  s.FramePadding={6,4};
  auto& c=s.Colors;
  c[ImGuiCol_WindowBg]      =cBG;  c[ImGuiCol_ChildBg]       ={0,0,0,0};
  c[ImGuiCol_PopupBg]       =cCard;c[ImGuiCol_Border]         =cBdr;
  c[ImGuiCol_Text]          =cTxt; c[ImGuiCol_TextDisabled]   =cDim;
  c[ImGuiCol_FrameBg]       =cPane;c[ImGuiCol_FrameBgHovered] =cHdr;
  c[ImGuiCol_FrameBgActive] =cActv;c[ImGuiCol_CheckMark]      =cAcc;
  c[ImGuiCol_SliderGrab]    =cAcc; c[ImGuiCol_SliderGrabActive]=cHov;
  c[ImGuiCol_Button]        =cHdr; c[ImGuiCol_ButtonHovered]  =cActv;
  c[ImGuiCol_ButtonActive]  =cAcc; c[ImGuiCol_Header]         =cActv;
  c[ImGuiCol_HeaderHovered] =cHdr; c[ImGuiCol_HeaderActive]   =cAcc;
  c[ImGuiCol_Separator]     =cBdr; c[ImGuiCol_ScrollbarBg]    =cBG;
  c[ImGuiCol_ScrollbarGrab] =cHdr; c[ImGuiCol_TitleBg]        =cBG;
  c[ImGuiCol_TitleBgActive] =cBG;
}


static void DrawESPPreview(ImVec2 pos,ImVec2 sz){
  ImDrawList* dl=ImGui::GetWindowDrawList();
  dl->AddRectFilled(pos,{pos.x+sz.x,pos.y+sz.y},IM_COL32(8,8,14,200),8.f);
  dl->AddRect(pos,{pos.x+sz.x,pos.y+sz.y},IM_COL32(46,204,113,120),8.f,0,1.f);

  float cx=pos.x+sz.x*.5f,cy=pos.y+sz.y*.5f;
  float bH=sz.y*.62f,bW=bH*.48f;
  float x1=cx-bW*.5f,y1=cy-bH*.5f,x2=cx+bW*.5f,y2=cy+bH*.5f;

  ImU32 bVis=IM_COL32((int)(Cfg::colBoxVis[0]*255),(int)(Cfg::colBoxVis[1]*255),(int)(Cfg::colBoxVis[2]*255),220);

  if(Cfg::espBox){
    if(Cfg::boxStyle==0){
      dl->AddRectFilled({x1,y1},{x2,y2},IM_COL32(0,0,0,20),2.f);
      dl->AddRect({x1-1,y1-1},{x2+1,y2+1},IM_COL32(0,0,0,140),2.f,0,1.5f);
      dl->AddRect({x1,y1},{x2,y2},bVis,2.f,0,1.2f);
    } else {
      float cs=bW*.28f;
      auto co=[&](float ax,float ay,float sx,float sy){
        dl->AddLine({ax,ay},{ax+sx*cs,ay},bVis,1.5f);
        dl->AddLine({ax,ay},{ax,ay+sy*cs},bVis,1.5f);
      };
      co(x1,y1,1,1);co(x2,y1,-1,1);co(x1,y2,1,-1);co(x2,y2,-1,-1);
    }
  }
  if(Cfg::espHp){
    float hpRat=0.72f;
    if(Cfg::hpBarSide==2){
      // Alt
      float barY=y2+5.f;
      float filledW=bW*hpRat;
      ImU32 hc=IM_COL32((int)((1.f-hpRat)*255),(int)(hpRat*200),50,220);
      dl->AddRectFilled({x1-1,barY-1},{x2+1,barY+4},IM_COL32(0,0,0,140),1.f);
      dl->AddRectFilled({x1,barY},{x1+filledW,barY+3},hc,1.f);
    } else {
      float bx=(Cfg::hpBarSide==0)?(x1-7.f):(x2+4.f);
      dl->AddRectFilled({bx-1,y1-1},{bx+4,y2+1},IM_COL32(0,0,0,140),1.f);
      float fy=y1+bH*(1.f-hpRat);
      ImU32 hc=IM_COL32((int)((1.f-hpRat)*255),(int)(hpRat*200),50,220);
      dl->AddRectFilled({bx,fy},{bx+3,y2},hc,1.f);
    }
  }
  if(Cfg::espBones){
    ImU32 bc=IM_COL32((int)(Cfg::colBone[0]*255),(int)(Cfg::colBone[1]*255),(int)(Cfg::colBone[2]*255),180);
    float nY=y1+bH*.10f,chY=y1+bH*.30f,pelY=y1+bH*.50f;
    dl->AddLine({cx,y1},{cx,nY},bc,1.f);
    dl->AddLine({cx,nY},{cx,chY},bc,1.f);
    dl->AddLine({cx,chY},{cx,pelY},bc,1.f);
    dl->AddLine({cx,nY},{cx-bW*.45f,chY},bc,1.f);
    dl->AddLine({cx,nY},{cx+bW*.45f,chY},bc,1.f);
    dl->AddLine({cx-bW*.45f,chY},{cx-bW*.35f,y1+bH*.5f},bc,1.f);
    dl->AddLine({cx+bW*.45f,chY},{cx+bW*.35f,y1+bH*.5f},bc,1.f);
    dl->AddLine({cx,pelY},{cx-bW*.28f,y1+bH*.70f},bc,1.f);
    dl->AddLine({cx,pelY},{cx+bW*.28f,y1+bH*.70f},bc,1.f);
    dl->AddLine({cx-bW*.28f,y1+bH*.70f},{cx-bW*.22f,y2},bc,1.f);
    dl->AddLine({cx+bW*.28f,y1+bH*.70f},{cx+bW*.22f,y2},bc,1.f);
  }
  // Kafa
  if(Cfg::espHeadCircle){
    float rr=bH*0.12f; if(rr<4.f)rr=4.f;
    dl->AddCircle({cx,y1+rr*0.3f},rr,IM_COL32(0,0,0,140),20,2.4f);
    dl->AddCircle({cx,y1+rr*0.3f},rr,IM_COL32(255,255,255,180),20,1.4f);
  }
  // Chinese
  if(Cfg::espHead){
    ImU32 hc=IM_COL32((int)(Cfg::colHead[0]*255),(int)(Cfg::colHead[1]*255),(int)(Cfg::colHead[2]*255),200);
    float hatW=bW*.70f,hatH=bH*0.14f;
    if(hatW<10.f)hatW=10.f; if(hatH<5.f)hatH=5.f;
    float headY=y1;
    dl->AddTriangleFilled({cx,headY-hatH-4.f},{cx-hatW,headY},{cx+hatW,headY},hc);
    dl->AddTriangle({cx,headY-hatH-4.f},{cx-hatW,headY},{cx+hatW,headY},IM_COL32(0,0,0,180),1.5f);
  }
  float nameOffY=(Cfg::espHead||Cfg::espHeadCircle)?14.f:0.f;
  if(Cfg::espName)
    dl->AddText({cx-14.f,y1-15.f-nameOffY},IM_COL32(235,235,245,200),"Enemy");
  if(Cfg::espWeapon){
    float wOffY=(Cfg::espHp&&Cfg::hpBarSide==2)?18.f:3.f;
    dl->AddRectFilled({cx-14.f,y2+wOffY-1},{cx+18.f,y2+wOffY+11},IM_COL32(0,0,0,100),2.f);
    dl->AddText({cx-13.f,y2+wOffY},IM_COL32(200,220,255,220),"AWP");
  }
}

static void DrawMenu(){
  // Animasyon
  if(!Cfg::menuOpen && g_menuAnim < 0.005f) return;
  if(g_menuAnim < 0.005f) return;
  ApplyStyle();

  ImGuiIO& io=ImGui::GetIO();

  // Ekran
  // 4
  static float s_dispW=0.f, s_dispH=0.f;
  static bool  s_prevOpen=false;
  // Men
  bool openedNow = (Cfg::menuOpen && !s_prevOpen);
  bool sizeJump  = (fabsf(io.DisplaySize.x-s_dispW)>100.f || fabsf(io.DisplaySize.y-s_dispH)>100.f);
  if(s_dispW<1.f || openedNow || sizeJump){
    s_dispW=io.DisplaySize.x; s_dispH=io.DisplaySize.y;
  }
  s_prevOpen=Cfg::menuOpen;

  // Resolution-adaptive
  float baseW=s_dispW*0.88f; if(baseW>1150)baseW=1150; if(baseW<620)baseW=620;
  float baseH=s_dispH*0.92f; if(baseH>920) baseH=920;  if(baseH<500)baseH=500;

  // Ease-out
  float t=g_menuAnim;
  float ease=1.0f-(1.0f-t)*(1.0f-t)*(1.0f-t); // ease-out
  float WW=baseW*ease, WH=baseH*ease;
  if(WW<4)WW=4; if(WH<4)WH=4;

  // Sabit
  float cx=s_dispW*.5f-WW*.5f;
  float cy=s_dispH*.5f-WH*.5f;
  ImGui::SetNextWindowPos({cx,cy},ImGuiCond_Always);
  ImGui::SetNextWindowSize({WW,WH},ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(ease*0.97f);

  ImGuiWindowFlags wf=ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
                      ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoBringToFrontOnFocus|
                      ImGuiWindowFlags_NoMove;
  if(!ImGui::Begin("##Q2",nullptr,wf)){ImGui::End();return;}
  // Animasyon
  if(ease<0.7f){ImGui::End();return;}
  ImDrawList* dl=ImGui::GetWindowDrawList();
  ImVec2 wp=ImGui::GetWindowPos();
  const float SB=180.f;

  dl->AddRectFilled(wp,{wp.x+SB,wp.y+WH},IM_COL32(9,12,11,255));
  dl->AddLine({wp.x+SB,wp.y},{wp.x+SB,wp.y+WH},uBdr,1.f);

  // Sidebar
  {
    float br=(sinf((float)ImGui::GetTime()*0.5f)+1.f)*0.5f; // 12.6s
    dl->AddRectFilled(wp,{wp.x+SB,wp.y+WH},IM_COL32(35,200,90,(int)(br*18.f+3.f)));
  }

  // Sol
  ImGui::SetCursorPos({0.f,0.f});
  ImGui::PushStyleColor(ImGuiCol_ChildBg,ImVec4(0,0,0,0));
  ImGui::BeginChild("##sidebar",{SB,WH},false,ImGuiWindowFlags_NoScrollbar);

  // QuaFind
  {
    ImDrawList* sdl=ImGui::GetWindowDrawList();
    ImVec2 sb=ImGui::GetWindowPos();
    const float fSz=22.f;
    // Glow
    sdl->AddText(nullptr,fSz,{sb.x+13.f,sb.y+15.f},IM_COL32(0,100,40,160),"QUAFIND");
    // Ana
    sdl->AddText(nullptr,fSz,{sb.x+12.f,sb.y+14.f},IM_COL32(55,225,105,255),"QUAFIND");
    // Ince
    sdl->AddLine({sb.x+8.f,sb.y+44.f},{sb.x+SB-8.f,sb.y+44.f},IM_COL32(40,180,70,70),1.f);
  }

  static char s_search[64]={};
  ImGui::SetCursorPos({8.f,52.f});
  ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(.08f,.10f,.09f,1.f));
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,ImVec4(.11f,.15f,.12f,1.f));
  ImGui::PushStyleColor(ImGuiCol_Text,cDim);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,8.f);
  ImGui::SetNextItemWidth(SB-16.f);
  ImGui::InputTextWithHint("##srch","  Ara...",s_search,sizeof(s_search));
  ImGui::PopStyleVar();ImGui::PopStyleColor(3);

  // Animasyonlu
  static float s_indY  = 88.f; // mevcut
  static float s_indTY = 88.f; // hedef

  auto NavBtn=[&](const char* label,int idx){
    if(s_search[0]){
      char lo[64]={},ls[64]={};
      for(int k=0;label[k]&&k<63;k++) lo[k]=(char)tolower((unsigned char)label[k]);
      for(int k=0;s_search[k]&&k<63;k++) ls[k]=(char)tolower((unsigned char)s_search[k]);
      if(!strstr(lo,ls)) return;
    }
    bool act=(g_Tab==idx);
    if(act){ s_indTY = ImGui::GetCursorScreenPos().y - wp.y; }
    // Aktif
    if(act){
      ImVec2 bMin=ImGui::GetCursorScreenPos();
      dl->AddRectFilled(bMin,{bMin.x+SB,bMin.y+32.f},IM_COL32(60,255,120,18),4.f);
    }
    char lbl[80];snprintf(lbl,80,"  %s##nb%d",label,idx);
    ImGui::PushStyleColor(ImGuiCol_Button,       act?ImVec4(.08f,.22f,.12f,.9f):ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(.06f,.15f,.09f,.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.08f,.22f,.12f,1.f));
    // Aktif
    ImGui::PushStyleColor(ImGuiCol_Text, act?ImVec4(.7f,1.f,.75f,1.f):cDim);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,7.f);
    ImGui::SetCursorPosX(0.f);
    if(ImGui::Button(lbl,{SB,32.f})){g_Tab=idx;s_search[0]=0;}
    // Aktif
    if(act&&ImGui::IsItemVisible()){
      char gt[80]; snprintf(gt,80,"  %s",label);
      ImVec2 rm=ImGui::GetItemRectMin();
      ImVec2 tsz=ImGui::CalcTextSize(gt);
      // ImGui
      ImVec2 tp={rm.x+(SB-tsz.x)*0.5f, rm.y+(32.f-tsz.y)*0.5f};
      ImU32 gc=IM_COL32(120,255,150,60);
      // Sadece
      int off[]={-1,0,1};
      for(int ox:off) for(int oy:off)
        if(ox||oy) dl->AddText({tp.x+(float)ox,tp.y+(float)oy},gc,gt);
    }
    ImGui::PopStyleVar();ImGui::PopStyleColor(4);
  };
  auto CatLbl=[&](const char* name){
    ImGui::Dummy({0,6.f});ImGui::SetCursorPosX(14.f);
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(.3f,.5f,.35f,1.f));
    ImGui::TextUnformatted(name);ImGui::PopStyleColor();ImGui::Dummy({0,2.f});
  };

  ImGui::SetCursorPosY(88.f);
  ImGui::SetCursorPosY(88.f);
  CatLbl("GORUNUM");
  NavBtn("ESP",0);
  NavBtn("Glow",1);
  NavBtn("Chams",2);
  NavBtn("Gorsel",3);
  CatLbl("NISAN");
  NavBtn("Legit Aimbot",4);
  NavBtn("Ragebot",5);
  NavBtn("Triggerbot",6);
  CatLbl("CESITLI");
  NavBtn("Anti-Aim",7);
  NavBtn("Nisangah",8);
  NavBtn("Radar",9);
  NavBtn("Hareket",10);
  NavBtn("Grenade",11);
  NavBtn("HitMarker",12);
  NavBtn(u8"Overlay",15);
  CatLbl("DIGER");
  NavBtn("Skinler",13);
  NavBtn("Ayarlar",14);
  // Ayirac
  ImGui::Dummy({0,8.f});
  ImGui::PushStyleColor(ImGuiCol_Separator,ImVec4(.2f,.2f,.2f,1.f));
  ImGui::Separator();
  ImGui::PopStyleColor();
  ImGui::Dummy({0,4.f});
  ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.06f,.14f,.09f,1.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(.09f,.22f,.13f,1.f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(.05f,.18f,.10f,1.f));
  ImGui::PushStyleColor(ImGuiCol_Text,cDim);
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,7.f);
  ImGui::SetCursorPosX(6.f);
  if(ImGui::Button(u8"Pencere D\u00fcz.",{SB-12.f,28.f})){
    g_windowArrangeMode=true;
    g_menuTarget=false;Cfg::menuOpen=false;g_menuAnim=0.f;
  }
  ImGui::PopStyleVar();ImGui::PopStyleColor(4);

  // Indikat
  {
    float idt=io.DeltaTime; if(idt>0.05f)idt=0.05f;
    float spd=1.f-expf(-idt*18.f); // hizli
    s_indY+=( s_indTY - s_indY )*spd;
    // Animasyonlu
    float iy=wp.y+s_indY;
    ImU32 indGlow=IM_COL32(60,255,120,40);
    dl->AddRectFilled({wp.x,iy},{wp.x+6.f,iy+32.f},indGlow,3.f);
    dl->AddRectFilled({wp.x,iy},{wp.x+3.f,iy+32.f},uAcc,2.f);
  }

  ImGui::EndChild(); // sidebar
  ImGui::PopStyleColor(); // ChildBg

  // Sag
  float CW=WW-SB-24.f;
  float CH=WH;
  // Tab
  static int   s_prevTab  = -1;
  static float s_tabFade  = 1.f;
  if(s_prevTab != g_Tab){
    ImGui::SetNextWindowScroll({0.f,0.f});
    s_prevTab = g_Tab;
    s_tabFade = 0.f;
  }
  float tabDt = io.DeltaTime; if(tabDt>0.1f)tabDt=0.1f;
  s_tabFade += tabDt * 7.f; if(s_tabFade>1.f)s_tabFade=1.f;
  ImGui::SetCursorPos({SB+1.f,0.f});
  ImGui::PushStyleColor(ImGuiCol_ChildBg,cBG);
  ImGui::BeginChild("##CT",{WW-SB-1.f,CH},false,0);
  ImGui::PushStyleVar(ImGuiStyleVar_Alpha,s_tabFade);
  ImGui::SetCursorPos({20.f,14.f});


 
  // PAGE
  if(g_Tab==0){
    ImGui::Columns(2,"##gc",false);
    ImGui::SetColumnWidth(0,236.f);
    ImGui::PushStyleColor(ImGuiCol_ChildBg,IM_COL32(8,8,14,200));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10.f);
    ImGui::BeginChild("##prev",{228.f,CH-6.f},false);
      ImGui::PushStyleColor(ImGuiCol_Text,cAcc);
      ImGui::SetCursorPosX(8.f);ImGui::Dummy({0,6.f});
      ImGui::SetCursorPosX(8.f);ImGui::TextUnformatted(u8"ESP \u00d6nizleme");
      ImGui::PopStyleColor();Sep();
      ImGui::TextColored(cDim,u8"Can Bar\u0131 Konumu");
      ImGui::SameLine(ImGui::GetCursorPosX()+ImGui::GetContentRegionAvail().x-72.f);
      const char* hpLabels[]={"|< Sol##hp","Sag >|##hp","vv Alt##hp"};
      if(ImGui::SmallButton(hpLabels[Cfg::hpBarSide%3]))Cfg::hpBarSide=(Cfg::hpBarSide+1)%3;
      ImGui::Spacing();
      ImVec2 pp=ImGui::GetCursorScreenPos();pp.x+=6.f;
      ImGui::Dummy({216.f,310.f});
      DrawESPPreview(pp,{216.f,310.f});
    ImGui::EndChild();
    ImGui::PopStyleVar();ImGui::PopStyleColor();
    ImGui::NextColumn();
    float rw=CW-244.f;
    Card("ESP","##ESP",rw,290,[&](){
      TRow("ESP Aktif",&Cfg::espOn);Sep();
      ImGui::TextColored(cDim,"Kutu Tipi");ImGui::SameLine();ImGui::SetNextItemWidth(100);
      const char* bs[]={u8"Tam Kutu",u8"K\u00f6\u015fe Kutu"};
      ImGui::Combo("##bst",&Cfg::boxStyle,bs,2);
      ColRow(u8"Kutu (G\u00f6r\u00fcn\u00fcr)",&Cfg::espBox,Cfg::colBoxVis);
      ColRow("Kutu (Duvar)",&Cfg::espBox,Cfg::colBoxOcc);
      TRow(u8"Can Bar\u0131",&Cfg::espHp);
      ColRow("Iskelet",&Cfg::espBones,Cfg::colBone);
      ColRow(u8"Chinese Hat",&Cfg::espHead,Cfg::colHead);
      TRow(u8"Kafa Dairesi",&Cfg::espHeadCircle);Sep();
      TRow("Isim",&Cfg::espName);
      TRow(u8"Mesafe (mt)",&Cfg::espDistOn);
      TRow(u8"Silah \u0130konu",&Cfg::espWeapon);
      if(Cfg::espWeapon){ImGui::Indent(8.f);ColBtn(u8"\u0130kon Rengi",Cfg::colWeaponIcon);
        ImGui::PushItemWidth(80);ImGui::SliderFloat(u8"Boyut##wiSz",&Cfg::weaponIconSize,1.f,24.f,"%.0f");ImGui::PopItemWidth();ImGui::Unindent(8.f);}
      TRow("FOV Dairesi",&Cfg::espFovCircle);
      ColRow("Snap Line",&Cfg::espSnap,Cfg::colSnap);
      TRow(u8"Tak\u0131m Arkada\u015flar\u0131",&Cfg::espTeam);
    });
    ImGui::Columns(1);
  }
  // PAGE
  else if(g_Tab==1){
    Card("Glow","##Glow",CW-8,160,[&](){
      TRow("Glow",&Cfg::glowOn);Sep();
      TRow(u8"Tak\u0131ma",&Cfg::glowTeam);
      KeyBind(u8"Tu\u015f",&Cfg::glowKey);ImGui::Spacing();
      ColBtn(u8"G\u00f6r\u00fcn\u00fcr",Cfg::glowVis);ImGui::Spacing();
      ColBtn("Duvar",Cfg::glowWall);
    });
  }
  // PAGE
  else if(g_Tab==2){
    // Yardimci
    // customPalette
    auto RainbowPanel=[&](const char* idPfx, bool* on, float* spd,
                           bool* customPalette, int* cnt, float cols[4][3]){
      Sep();
      // RGB
      ImGui::TextColored(*on?ImVec4(.2f,.95f,.55f,1.f):cDim,u8"RGB Rainbow");
      ImGui::SameLine();
      char tId[64]; snprintf(tId,64,"##rb_%s",idPfx);
      Toggle(tId,on);
      if(*on){
        // Hiz
        ImGui::SetNextItemWidth(-1);
        char sId[64]; snprintf(sId,64,"##rbspd_%s",idPfx);
        ImGui::SliderFloat(sId,spd,0.02f,2.f,u8"Hiz: %.2f");
        // Mod
        char rotoId[64],rcstId[64];
        snprintf(rotoId,64,u8"Oto (Tum Renkler)##rboto_%s",idPfx);
        snprintf(rcstId,64,u8"Ozel Palet##rbcst_%s",idPfx);
        if(ImGui::RadioButton(rotoId,!(*customPalette))) *customPalette=false;
        ImGui::SameLine();
        if(ImGui::RadioButton(rcstId,*customPalette))   *customPalette=true;
        // Ozel
        if(*customPalette){
          char cntId[64]; snprintf(cntId,64,"##rbcnt_%s",idPfx);
          ImGui::SetNextItemWidth(65.f);
          ImGui::SliderInt(cntId,cnt,2,4,u8"%d renk");
          ImGui::SameLine(0,8);
          for(int ri=0;ri<*cnt;ri++){
            if(ri>0) ImGui::SameLine(0,4);
            char cId[64]; snprintf(cId,64,"##rbc%d_%s",ri,idPfx);
            ImGui::ColorEdit3(cId,cols[ri],
              ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoLabel|
              ImGuiColorEditFlags_PickerHueWheel|ImGuiColorEditFlags_NoDragDrop);
          }
        }
      }
    };

    Card("Chams","##Chams",CW-8,0,[&](){
      TRow("Chams",&Cfg::chamsOn);TRow("Wallhack",&Cfg::chamsWall);Sep();
      KeyBind(u8"Tu\u015f",&Cfg::chamsKey);Sep();
      ImGui::TextColored(cDim,"Materyal");ImGui::SetNextItemWidth(-1);
      const char* mt[]={"Duz","Tel Kafes","Pulse"};
      ImGui::Combo("##cm",&Cfg::chamsMat,mt,3);
      if(!Cfg::chamsRainbowOn){
        ImGui::Spacing();
        ColBtn(u8"G\u00f6r\u00fcn\u00fcr",Cfg::chamsVis);ImGui::Spacing();
        ColBtn("Gizli",Cfg::chamsOcc);
      }
      RainbowPanel("ch",&Cfg::chamsRainbowOn,&Cfg::chamsRainbowSpd,
                   &Cfg::chamsRainbowCustom,&Cfg::chamsRainbowCnt,Cfg::chamsRainbowCols);
    });
    ImGui::Dummy({0,4.f});
    { const char* wmt[]={"Duz","Tel Kafes","Pulse","Bloom"};
      int wcMatI=(int)WC::g_WeapMat,knMatI=(int)WC::g_KnifeMat,glMatI=(int)WC::g_GloveMat;
      Card(u8"Ekipman Chams","##EqChCard",CW-8,0,[&](){
      TRow(u8"Ekipman Chams",&WC::g_On);
      if(WC::g_On){Sep();
        // Silah
        TRow(u8"Silah Chams",&WC::g_WeapOn);
        if(WC::g_WeapOn){
          if(!WC::g_WeapRainbow.on){
            ImGui::PushItemWidth(-1);ImGui::ColorEdit3("##wccol",WC::g_WeapCol,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoDragDrop);ImGui::PopItemWidth();
            ImGui::SetNextItemWidth(-1);ImGui::SliderFloat("##wcalp",&WC::g_WeapAlpha,0.1f,1.f,"%.2f");
          }
          ImGui::TextColored(cDim,"Mat");ImGui::SameLine();ImGui::SetNextItemWidth(-1);
          if(ImGui::Combo("##wcm",&wcMatI,wmt,4))WC::g_WeapMat=(WC::EMat)wcMatI;
          RainbowPanel("wc",&WC::g_WeapRainbow.on,&WC::g_WeapRainbow.speed,
                       &WC::g_WeapRainbow.customPalette,&WC::g_WeapRainbow.colorCount,WC::g_WeapRainbow.colors);
        }
        // Bicak
        TRow(u8"B\u0131\u00e7ak Chams",&WC::g_KnifeOn);
        if(WC::g_KnifeOn){
          if(!WC::g_KnifeRainbow.on){
            ImGui::PushItemWidth(-1);ImGui::ColorEdit3("##kncol",WC::g_KnifeCol,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoDragDrop);ImGui::PopItemWidth();
            ImGui::SetNextItemWidth(-1);ImGui::SliderFloat("##knalp",&WC::g_KnifeAlpha,0.1f,1.f,"%.2f");
          }
          ImGui::TextColored(cDim,"Mat");ImGui::SameLine();ImGui::SetNextItemWidth(-1);
          if(ImGui::Combo("##knm",&knMatI,wmt,4))WC::g_KnifeMat=(WC::EMat)knMatI;
          RainbowPanel("kn",&WC::g_KnifeRainbow.on,&WC::g_KnifeRainbow.speed,
                       &WC::g_KnifeRainbow.customPalette,&WC::g_KnifeRainbow.colorCount,WC::g_KnifeRainbow.colors);
        }
        // Eldiven
        TRow(u8"Eldiven Chams",&WC::g_GloveOn);
        if(WC::g_GloveOn){
          if(!WC::g_GloveRainbow.on){
            ImGui::PushItemWidth(-1);ImGui::ColorEdit3("##glcol",WC::g_GloveCol,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoDragDrop);ImGui::PopItemWidth();
            ImGui::SetNextItemWidth(-1);ImGui::SliderFloat("##glalp",&WC::g_GloveAlpha,0.1f,1.f,"%.2f");
          }
          ImGui::TextColored(cDim,"Mat");ImGui::SameLine();ImGui::SetNextItemWidth(-1);
          if(ImGui::Combo("##glm",&glMatI,wmt,4))WC::g_GloveMat=(WC::EMat)glMatI;
          RainbowPanel("gl",&WC::g_GloveRainbow.on,&WC::g_GloveRainbow.speed,
                       &WC::g_GloveRainbow.customPalette,&WC::g_GloveRainbow.colorCount,WC::g_GloveRainbow.colors);
        }
      }
    });}
  }
  // PAGE
  else if(g_Tab==3){
    Card(u8"Ekran Sis","##VigCard",CW-8,Cfg::screenFogOn?272:50,[&](){
      TRow(u8"Sis Efekti",&Cfg::screenFogOn);
      if(Cfg::screenFogOn){Sep();
        auto VR=[&](const char* lbl,const char* t,const char* c,const char* a,bool* on,float* col4){
          ImGui::TextColored(cDim,"%s",lbl);ImGui::SameLine(55);Toggle(t,on);
          if(*on){ImGui::SameLine(0,6);ImGui::ColorEdit3(c,col4,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoLabel|ImGuiColorEditFlags_NoDragDrop);
            ImGui::SameLine(0,4);ImGui::SetNextItemWidth(70);ImGui::SliderFloat(a,&col4[3],0.f,1.f,"Opa:%.2f");}};
        VR(u8"Yukari","##vot","##vct","##vat",&Cfg::screenFogTop,Cfg::colFogTop);
        VR(u8"Asagi","##vob","##vcb","##vab",&Cfg::screenFogBottom,Cfg::colFogBottom);
        VR(u8"Sag","##vor","##vcr","##var",&Cfg::screenFogRight,Cfg::colFogRight);
        VR(u8"Sol","##vol","##vcl","##val",&Cfg::screenFogLeft,Cfg::colFogLeft);}
    });ImGui::Dummy({0,4.f});
    Card(u8"DX11 D\u00fcnya Rengi","##DX11World",CW-8,Cfg::dx11WorldOn?150:50,[&](){
      TRow(u8"D\u00fcnya Tint",&Cfg::dx11WorldOn);
      if(Cfg::dx11WorldOn){Sep();ImGui::PushItemWidth(-1);
        ImGui::ColorEdit4(u8"##dxworldcol",Cfg::dx11WorldColor,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_AlphaBar|ImGuiColorEditFlags_AlphaPreview);
        ImGui::PopItemWidth();ImVec4 pv={Cfg::dx11WorldColor[0],Cfg::dx11WorldColor[1],Cfg::dx11WorldColor[2],1.f};
        ImGui::ColorButton("##dxp",pv,ImGuiColorEditFlags_NoTooltip,{CW-24,14});}
    });ImGui::Dummy({0,4.f});
    {int skyH=Cfg::skyColorOn?155:50;
    Card(u8"Gokyuzu (2D)","##SkyOvCard",CW-8,skyH,[&](){
      TRow(u8"Gokyuzu Efekti",&Cfg::skyColorOn);
      if(Cfg::skyColorOn){Sep();ImGui::PushItemWidth(-1);
        ImGui::ColorEdit3("##skycolpick",Cfg::skyColor,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoDragDrop);
        ImGui::PopItemWidth();ImGui::SetNextItemWidth(-1);ImGui::SliderFloat("##skyalpha",&Cfg::skyAlpha,0.05f,1.f,"%.2f");
        ImGui::SetNextItemWidth(-1);ImGui::SliderFloat("##skyheight",&Cfg::skyHeight,0.1f,0.8f,"%.2f");}
    });}ImGui::Dummy({0,4.f});
    {int snH=Cfg::snowOn?115:50;
    Card(u8"Kar Efekti","##SnowCard",CW-8,snH,[&](){
      TRow(u8"Kar Ya\u011f\u0131\u015f\u0131",&Cfg::snowOn);
      if(Cfg::snowOn){Sep();ImGui::SetNextItemWidth(CW-24);
        ImGui::SliderInt("##snowdens",&Cfg::snowDensity,0,2,Cfg::snowDensity==0?u8"Az":Cfg::snowDensity==1?u8"Orta":u8"Cok");
        Sep();TRow(u8"3D Takip",&Cfg::particle3D);}
    });}ImGui::Dummy({0,4.f});
    {int sakH=Cfg::sakuraOn?105:50;
    Card(u8"Sakura","##SakuraCard",CW-8,sakH,[&](){
      TRow(u8"Sakura",&Cfg::sakuraOn);
      if(Cfg::sakuraOn){Sep();ImGui::PushItemWidth(-1);
        ImGui::ColorEdit4("##sakuracol",Cfg::sakuraColor,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_AlphaBar|ImGuiColorEditFlags_AlphaPreview);
        ImGui::PopItemWidth();Sep();TRow(u8"3D Takip",&Cfg::particle3D);}
    });}
    ImGui::Dummy({0,4.f});
    {int sbxH=Cfg::skyboxChangerOn?105:50;
    Card(u8"3D Skybox","##SkyboxCard",CW-8,sbxH,[&](){
      TRow(u8"3D Skybox Rengi",&Cfg::skyboxChangerOn);
      if(Cfg::skyboxChangerOn){Sep();ImGui::PushItemWidth(-1);
        ImGui::ColorEdit3("##skyboxcolpick",Cfg::skyboxColor,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_NoDragDrop);
        ImGui::PopItemWidth();}
    });}
  }
  // PAGE
  else if(g_Tab==4){
    Card("Legit Aimbot","##Legit",CW-8,360,[&](){
      TRow("Aktif",&Cfg::aimOn);Sep();
      TRow(u8"Tu\u015f Gerekli",&Cfg::aimNeedKey);
      if(Cfg::aimNeedKey)KeyBind(u8"Tu\u015f",&Cfg::aimKey);
      TRow(u8"Tak\u0131ma Ni\u015fan",&Cfg::aimShootTeam);Sep();
      ImGui::TextColored(cDim,"Hedef");ImGui::SameLine();ImGui::SetNextItemWidth(100);
      const char* bn[]={"Kafa","Boyun",u8"G\u00f6\u011f\u00fcs","Bel"};
      ImGui::Combo("##ab",&Cfg::aimBone,bn,4);
      Sli("FOV",&Cfg::aimFov,1.f,360.f,"%.0f");
      Sli(u8"D\u00fczle\u015ftir",&Cfg::aimSmooth,1.f,20.f);
      Sli(u8"\u00d6l\u00fc Alan",&Cfg::aimDz,0.f,20.f);Sep();
      TRow("Hedef Kilitle",&Cfg::aimLock);
      TRow(u8"Duvar Kontrol\u00fc",&Cfg::aimWall);Sep();
      TRow(u8"Yak\u0131nl\u0131k Kilidi",&Cfg::aimProxOn);
      if(Cfg::aimProxOn)Sli(u8"Mesafe##prx",&Cfg::aimProxDist,1.f,50.f,"%.0f");
    });
  }
  // PAGE
  else if(g_Tab==5){
    Card("Rage Bot","##Rage",CW-8,280,[&](){
      TRow("Aktif",&Cfg::rageOn);Sep();
      TRow(u8"Tu\u015f Gerekli",&Cfg::rageNeedKey);
      if(Cfg::rageNeedKey)KeyBind(u8"Tu\u015f",&Cfg::rageKey);
      TRow(u8"Tak\u0131ma",&Cfg::rageShootTeam);Sep();
      ImGui::TextColored(cDim,"Hedef");ImGui::SameLine();ImGui::SetNextItemWidth(100);
      const char* rb[]={"Kafa","Boyun",u8"G\u00f6\u011f\u00fcs"};
      ImGui::Combo("##rb",&Cfg::rageBone,rb,3);
      Sli("FOV",&Cfg::rageFov,1.f,999.f,"%.0f");
      Sli(u8"D\u00fczle\u015ftir",&Cfg::rageSmooth,1.f,5.f);Sep();
      TRow(u8"Otomatik Ate\u015f",&Cfg::rageAuto);
      if(Cfg::rageAuto)Sli("Gecik.(ms)",&Cfg::rageAutoDelay,0.f,300.f,"%.0f");
    });
  }
  // PAGE
  else if(g_Tab==6){
    Card("Triggerbot","##Trig",CW-8,210,[&](){
      TRow("Triggerbot",&Cfg::trigOn);Sep();
      TRow(u8"Tu\u015f Gerekli",&Cfg::trigNeedKey);
      if(Cfg::trigNeedKey)KeyBind(u8"Tu\u015f",&Cfg::trigKey);
      TRow(u8"Tak\u0131ma S\u0131k",&Cfg::trigShootTeam);
      Sli("Gecikme(ms)",&Cfg::trigDelay,0.f,500.f,"%.0f");
      TRow(u8"Duvar Kontrol\u00fc",&Cfg::trigWall);
    });
  }
  // PAGE
  else if(g_Tab==7){
    Card("Anti-Aim","##AA",CW-8,200,[&](){
      TRow("Anti-Aim",&Cfg::aaOn);Sep();
      KeyBind(u8"Tu\u015f",&Cfg::aaKey);Sep();
      ImGui::TextColored(cDim,"Mod");ImGui::SetNextItemWidth(-1);
      const char* m[]={"Spin","Sabit",u8"Sars\u0131nt\u0131",u8"A\u015fa\u011f\u0131",u8"Yukar\u0131"};
      ImGui::Combo("##aaM",&Cfg::aaMode,m,5);
      Sli("Yaw",&Cfg::aaYawAdd,-180.f,180.f,"%.0f");
    });
  }
  // PAGE
  else if(g_Tab==8){
    Card(u8"Ni\u015fangah","##Nis",CW-8,170,[&](){
      TRow(u8"\u00d6zel Ni\u015fangah",&Cfg::custXhair);Sep();
      if(Cfg::custXhair){
        Sli("Boyut",&Cfg::custXhairSize,3.f,30.f,"%.0f");
        Sli(u8"Kal\u0131nl\u0131k",&Cfg::custXhairThick,.5f,4.f);
        ImGui::TextColored(cDim,u8"Bo\u015fluk");ImGui::SetNextItemWidth(-1);
        ImGui::SliderInt("##xg",&Cfg::custXhairGap,0,15);
        ColBtn("Renk",Cfg::colXhair);}
    });
  }
  // PAGE
  else if(g_Tab==9){
    Card("Radar","##Rad",CW-8,90,[&](){
      TRow("Radar Hilesi",&Cfg::radarHack);
      ImGui::TextColored(cDim,u8"(d\u00fc\u015fmanlar g\u00f6r\u00fcn\u00fcr)");
    });
  }
  // PAGE
  else if(g_Tab==10){
    Card("Hareket","##Har",CW-8,100,[&](){
      TRow("Bunny Hop",&Cfg::bhopOn);Sep();
      KeyBind(u8"Tu\u015f",&Cfg::bhopKey);
    });ImGui::Dummy({0,4.f});
    Card("Silah","##Sil",CW-8,105,[&](){
      TRow("Flash Yok",&Cfg::noFlash);
      TRow("Scope Yok",&Cfg::noScope);
      TRow("Duman Yok",&Cfg::noSmoke);
    });
  }
  // PAGE
  else if(g_Tab==11){
    Card("Grenade ESP","##Gren",CW-8,152,[&](){
      TRow("Grenade ESP",&Cfg::grenadeESP);
      TRow("Molotov Wall",&Cfg::molotovWall);Sep();
      ColBtn("CT Trail",Cfg::colGrenadeCT);ImGui::Spacing();
      ColBtn("T Trail",Cfg::colGrenadeT);
    });
  }
  // PAGE
  else if(g_Tab==12){
    Card("Hit Marker","##HMCard",CW-8,0,[&](){
      TRow("Hitmarker",&HM::hmOn);
      if(HM::hmOn){
        Sep();
        // Boyut
        ImGui::TextColored(cDim,u8"Boyut");
        ImGui::SetNextItemWidth(-1);
        ImGui::SliderFloat("##hmsize",&HM::hmSize,4.f,24.f,"%.0f px");
        // Kalinlik
        ImGui::TextColored(cDim,u8"Kal\u0131nl\u0131k");
        ImGui::SetNextItemWidth(-1);
        ImGui::SliderFloat("##hmthick",&HM::hmThick,1.f,5.f,"%.1f");
        // Renk
        ColBtn(u8"Renk",HM::hmCol);
      }
    });
    ImGui::Dummy({0,4.f});
    {int indH=Cfg::enemyIndicator?120:50;
    Card(u8"D\u00fc\u015fman \u0130ndikat\u00f6r\u00fc","##EIndCard2",CW-8,indH,[&](){
      TRow(u8"Offscreen Ok",&Cfg::enemyIndicator);
      if(Cfg::enemyIndicator){Sep();ImGui::PushItemWidth(-1);
        ImGui::ColorEdit4("##indcol2",Cfg::enemyIndicatorCol,ImGuiColorEditFlags_NoInputs|ImGuiColorEditFlags_AlphaBar|ImGuiColorEditFlags_AlphaPreview);
        ImGui::PopItemWidth();ImGui::SetNextItemWidth(-1);
        ImGui::SliderFloat("##indsize2",&Cfg::enemyIndicatorSize,6.f,20.f,"%.0f");}
    });}
  }
  // PAGE
  else if(g_Tab==15){
    Card("Izleyici Listesi","##OvSpec",CW-8,48,[&](){
      TRow("Izleyici Listesi",&Cfg::spectatorListOn);
    });ImGui::Dummy({0,4.f});
    Card("Kisayol Listesi","##OvShort",CW-8,48,[&](){
      TRow("Kisayol Listesi",&Cfg::shortcutListOn);
    });ImGui::Dummy({0,4.f});
    Card("C4 Zamanlayici","##OvBomb",CW-8,48,[&](){
      TRow("C4 Zamanlayici",&Cfg::bombTimerOn);
    });ImGui::Dummy({0,4.f});
    Card("Info Overlay","##OvInfo",CW-8,48,[&](){
      TRow("QUAFIND Info",&Cfg::infoOverlayOn);
    });ImGui::Dummy({0,4.f});
    ImGui::TextColored(cDim,"  Konumlandirma icin 'Pencere Duz.' butonunu kullanin.");
  }
  // PAGE
  else if(g_Tab==13){
    Card("Skin Changer","##Skin",CW-8,600,[&](){
      static int sel=0,pk=0,seed=1;static float wear=0.008f;
      ImGui::TextColored(cDim,"Silah");ImGui::SetNextItemWidth(-1);
      if(ImGui::BeginCombo("##ws",Skin::kWeapons[sel].name)){
        for(int i=0;Skin::kWeapons[i].name;i++){bool s=(sel==i);
          if(ImGui::Selectable(Skin::kWeapons[i].name,s))sel=i;
          if(s)ImGui::SetItemDefaultFocus();}
        ImGui::EndCombo();}
      ImGui::Spacing();ImGui::TextColored(cDim,"Paint Kit ID");
      ImGui::SetNextItemWidth(-1);ImGui::InputInt("##pk",&pk,1,10);if(pk<0)pk=0;
      Sli("Wear",&wear,0.f,1.f,"%.4f");
      ImGui::TextColored(cDim,"Seed");ImGui::SetNextItemWidth(-1);ImGui::InputInt("##sk",&seed,1,1);
      Sep();ImGui::Spacing();ImGui::PushStyleColor(ImGuiCol_Button,cAcc);
      if(ImGui::Button("Skin Kaydet##sk",{-1,28})){
        int def=Skin::kWeapons[sel].def;Skin::SkinConfig sc;
        sc.paintKit=pk;sc.wear=wear;sc.seed=seed;sc.statTrak=-1;sc.knifeModel=0;sc.enabled=true;
        std::lock_guard<std::mutex> lk(Skin::configMutex);
        Skin::weaponSkins[def]=sc;Skin::forceUpdate.store(true);}
      ImGui::PopStyleColor();ImGui::Spacing();Sep();
      ImGui::TextColored(cAcc,"Aktif Skinler:");
      std::lock_guard<std::mutex> lk2(Skin::configMutex);
      for(auto& [d,cfg]:Skin::weaponSkins){if(!cfg.enabled)continue;
        char lb[96];
        if(cfg.knifeModel>0)snprintf(lb,96,"%s: PK=%d -> %s",Skin::GetWeaponName(d),cfg.paintKit,Skin::GetWeaponName(cfg.knifeModel));
        else snprintf(lb,96,"%s: PK=%d",Skin::GetWeaponName(d),cfg.paintKit);
        ImGui::TextColored(cTxt,"%s",lb);ImGui::SameLine();
        char rb[32];snprintf(rb,32,"Sil##%d",d);
        if(ImGui::SmallButton(rb))cfg.enabled=false;}
    });
  }
  // PAGE
  else if(g_Tab==14){
    Card("Sistem","##Sys",CW-8,200,[&](){
      char b[72];
      snprintf(b,72,"Ekran : %dx%d",G.W,G.H);ImGui::TextColored(cDim,"%s",b);
      snprintf(b,72,"FPS   : %.0f",io.Framerate);ImGui::TextColored(cAcc,"%s",b);
      snprintf(b,72,"Oyuncu: %d",G.playerCount);ImGui::TextColored(cDim,"%s",b);
      snprintf(b,72,"Pawn  : 0x%llX",G.localPawn);ImGui::TextColored(cDim,"%s",b);
      snprintf(b,72,"Gren  : %d  Inf: %d",G.grenadeCount,G.infernoCount);ImGui::TextColored(cDim,"%s",b);
      Sep();ULONGLONG ws=Cfg::firstPawnSeen;
      if(!ws)ImGui::TextColored({1.f,.4f,.4f,1.f},"Yazma: BEKLIYOR");
      else if(!Cfg::WriteSafe()){ULONGLONG r2=Cfg::WRITE_DELAY-(GetTickCount64()-ws);
        snprintf(b,72,"Yazma: %.1fs",r2/1000.f);ImGui::TextColored({1.f,.8f,.2f,1.f},"%s",b);}
      else ImGui::TextColored({.2f,1.f,.4f,1.f},u8"Yazma: AKT\u0130F");
    });ImGui::Dummy({0,4.f});
    Card(u8"Config Y\u00f6neticisi","##CfgMgr",CW-8,400,[&](){
      static char   cfgName[64]  = "default";
      static int    selIdx       = -1;
      static char   msg[128]     = {};
      static ULONGLONG msgTime   = 0;
      static std::vector<std::string> cfgList = CfgSys::ListConfigs();
      ImGui::TextColored(cDim,u8"Config Ad\u0131:");
      ImGui::SetNextItemWidth(-1);
      ImGui::InputText("##cn",cfgName,64);
      ImGui::Spacing();
      float bw=(CW-30)*.47f;
      ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.2f,.6f,.3f,1.f));
      if(ImGui::Button(u8"Kaydet / \u00dcze Yaz",{bw,28})){
        if(cfgName[0]){
          if(CfgSys::Save(cfgName)){
            snprintf(msg,128,u8"\u2713 Kaydedildi: %s",cfgName);
            cfgList=CfgSys::ListConfigs();
          } else snprintf(msg,128,u8"Kaydetme hatas\u0131!");
          msgTime=GetTickCount64();
        }
      }
      ImGui::PopStyleColor();
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.55f,.15f,.15f,1.f));
      if(ImGui::Button(u8"Se\u00e7ileni Sil",{bw,28})){
        if(selIdx>=0&&selIdx<(int)cfgList.size()){
          if(CfgSys::Delete(cfgList[selIdx].c_str())){
            snprintf(msg,128,u8"\u2713 Silindi: %s",cfgList[selIdx].c_str());
            cfgList=CfgSys::ListConfigs();
            selIdx=-1; cfgName[0]=0;
          } else snprintf(msg,128,u8"Silme hatas\u0131!");
          msgTime=GetTickCount64();
      }
      }
      ImGui::PopStyleColor();
      Sep();
      ImGui::TextColored(cAcc,u8"Kaydedilen Configler");
      ImGui::SameLine();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX()+4.f);
      if(ImGui::SmallButton(u8"\u21ba Yenile##ref")) cfgList=CfgSys::ListConfigs();
      ImGui::Spacing();
      ImGui::PushStyleColor(ImGuiCol_ChildBg,IM_COL32(10,14,10,200));
      ImGui::BeginChild("##cfglist",{-1,180},false);
      if(cfgList.empty()){
        ImGui::TextColored(cDim,u8"  Hic config bulunamadi.");
        ImGui::TextColored(cDim,u8"  Documents\\QuaFindConfig\\ klasoru");
      }
      for(int i=0;i<(int)cfgList.size();i++){
        bool sel=(selIdx==i);
        ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(.08f,.28f,.12f,1.f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(.10f,.35f,.15f,1.f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive,  ImVec4(.08f,.28f,.12f,1.f));
        if(ImGui::Selectable(cfgList[i].c_str(),sel,0,{ImGui::GetContentRegionAvail().x-55.f,0})){
          selIdx=i;
          snprintf(cfgName,64,"%s",cfgList[i].c_str());
        }
        ImGui::PopStyleColor(3);
        ImGui::SameLine();
        char loadLbl[32]; snprintf(loadLbl,32,u8"\u25b6 Yukle##L%d",i);
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.18f,.55f,.28f,1.f));
        if(ImGui::SmallButton(loadLbl)){
          if(CfgSys::Load(cfgList[i].c_str())){
            snprintf(msg,128,u8"\u2713 Yuklendi: %s",cfgList[i].c_str());
            selIdx=i; snprintf(cfgName,64,"%s",cfgList[i].c_str());
          } else snprintf(msg,128,u8"Yukleme hatas\u0131!");
          msgTime=GetTickCount64();
        }
        ImGui::PopStyleColor();
      }
      ImGui::EndChild();
      ImGui::PopStyleColor();
      if(msg[0]&&(GetTickCount64()-msgTime<3000))
        ImGui::TextColored({.3f,1.f,.5f,1.f},"%s",msg);
      else msg[0]=0;
    });
    ImGui::Dummy({0,4.f});
    Card(u8"Tu\u015flar","##Keys",CW-8,85,[&](){
      ImGui::TextColored(cDim,u8"INSERT  = Men\u00fcy\u00fc A\u00e7/Kapat");
      ImGui::TextColored(cDim,"END     = Sil");
    });
    ImGui::Dummy({0,24.f}); // alt
  }

  ImGui::PopStyleVar();   // Alpha
  ImGui::EndChild(); // CT
  ImGui::PopStyleColor(); // cBG

  // Menunun
  {
    ImDrawList* fdl=ImGui::GetForegroundDrawList();
    float bspd=220.f; // piksel
    float bLen=2.f*(WW+WH);
    float bPos=fmodf((float)ImGui::GetTime()*bspd, bLen);
    float tailPx=180.f;
    // Perimetre
    auto bPt=[&](float p)->ImVec2{
      p=fmodf(p+bLen*2.f,bLen);
      if(p<WW)  return {wp.x+p,      wp.y};
      p-=WW; if(p<WH) return {wp.x+WW,    wp.y+p};
      p-=WH; if(p<WW) return {wp.x+WW-p,  wp.y+WH};
      p-=WW;          return {wp.x,        wp.y+WH-p};
    };
    // Gradient
    const int segs=22;
    for(int i=0;i<segs;i++){
      float f0=(float)i/segs, f1=(float)(i+1)/segs;
      float a=(1.f-f0)*(1.f-f0);
      ImU32 c=IM_COL32(70,255,130,(int)(a*210.f));
      fdl->AddLine(bPt(bPos-tailPx*f0),bPt(bPos-tailPx*f1),c,2.f);
    }
  }

  ImGui::End();
}

static void DrawESP(){
  if(!Cfg::espOn)return;
  ImDrawList* dl=ImGui::GetBackgroundDrawList();
  int W=G.W,H=G.H;if(W<=0||H<=0)return;

  if(Cfg::espFovCircle&&Cfg::rageOn){
    float fp=Cfg::rageFov<999.f?Cfg::rageFov:0.f;
    if(fp>0.f&&fp<(float)W)dl->AddCircle({W*.5f,H*.5f},fp,IM_COL32(46,204,113,110),64,1.2f);
  }
  if(Cfg::espFovCircle&&Cfg::aimOn)
    dl->AddCircle({W*.5f,H*.5f},Cfg::aimFov,IM_COL32(80,180,255,70),64,1.f);

  for(int i=0;i<G.playerCount;i++){
    const PlayerSnap& p=G.players[i];
    if(!p.valid||p.health<=0||!p.pawn)continue;
    bool isTeammate = (p.team==G.localTeam && G.localTeam);
    if(isTeammate && !Cfg::espTeam && !Cfg::chamsOn) continue;
    if(!isTeammate) HM::UpdateEnemy(p.pawn, p.health);

    float* cArr=p.isVisible?Cfg::colBoxVis:Cfg::colBoxOcc;
    ImU32 bCol=IM_COL32((int)(cArr[0]*255),(int)(cArr[1]*255),(int)(cArr[2]*255),p.isVisible?220:150);

    Vec2 fsc,hsc;
    if(!W2S(p.origin,fsc,G.vm,W,H))continue;
    if(!W2S(p.headPos,hsc,G.vm,W,H))continue;
    float bH=fsc.y-hsc.y;if(bH<4.f)continue;
    float bW=bH*.55f;
    float cx=(fsc.x+hsc.x)*.5f;
    float x1=cx-bW*.5f,y1=hsc.y-3.f,x2=cx+bW*.5f,y2=fsc.y+4.f;

    if(isTeammate){
      const char* teamLabel="TAKIM";
      ImVec2 tls=ImGui::CalcTextSize(teamLabel);
      float ty=y1-18.f;
      dl->AddText({cx-tls.x*.5f+1,ty+1},IM_COL32(0,0,0,200),teamLabel);
      dl->AddText({cx-tls.x*.5f,ty},IM_COL32(255,220,50,255),teamLabel);
      if(!Cfg::espTeam) continue;
    }

    if(Cfg::espBox){
      if(Cfg::boxStyle==0){
        dl->AddRectFilled({x1,y1},{x2,y2},IM_COL32(0,0,0,20),1.f);
        dl->AddRect({x1-1,y1-1},{x2+1,y2+1},IM_COL32(0,0,0,150),1.f,0,1.5f);
        dl->AddRect({x1,y1},{x2,y2},bCol,1.f,0,1.2f);
      } else {
        float cs=bW*.25f;
        auto co=[&](float ax,float ay,float sx,float sy){
          dl->AddLine({ax,ay},{ax+sx*cs,ay},bCol,1.5f);
          dl->AddLine({ax,ay},{ax,ay+sy*cs},bCol,1.5f);
        };
        co(x1,y1,1,1);co(x2,y1,-1,1);co(x1,y2,1,-1);co(x2,y2,-1,-1);
      }
    }
    if(Cfg::espHp){
      float hr=p.health/100.f;
      if(Cfg::hpBarSide==2){
        float barW=bW;
        float barY=y2+5.f;
        float filledW=barW*hr;
        ImU32 hc2=IM_COL32((int)((1.f-hr)*255),(int)(hr*200),50,220);
        dl->AddRectFilled({x1-1,barY-1},{x2+1,barY+4},IM_COL32(0,0,0,150),1.f);
        dl->AddRectFilled({x1,barY},{x1+filledW,barY+3},hc2,1.f);
        char hb[8];snprintf(hb,8,"%d",p.health);
        ImVec2 hts=ImGui::CalcTextSize(hb);
        dl->AddText({cx-hts.x*.5f,barY+4.f},IM_COL32(220,220,220,200),hb);
      } else {
        float bx=(Cfg::hpBarSide==0)?(x1-7.f):(x2+4.f);
        dl->AddRectFilled({bx-1,y1-1},{bx+4,y2+1},IM_COL32(0,0,0,150),1.f);
        float fy=y1+bH*(1.f-hr);
        ImU32 hc=IM_COL32((int)((1.f-hr)*255),(int)(hr*200),50,220);
        dl->AddRectFilled({bx,fy},{bx+3,y2},hc,1.f);
        char hb[8];snprintf(hb,8,"%d",p.health);
        dl->AddText({bx-1,y2+2},IM_COL32(220,220,220,200),hb);
      }
    }
    if(Cfg::espName){
      ImVec2 ts=ImGui::CalcTextSize(p.name);
      float nameY=y1-16.f;
      if(Cfg::espHead) nameY-=bH*0.08f;
      dl->AddText({cx-ts.x*.5f,nameY},IM_COL32(235,235,245,220),p.name);
    }
    if(Cfg::espDistOn){
      float mt = p.dist / 64.f;
      char db[16]; snprintf(db,16,"%.0fmt",mt);
      ImVec2 ds=ImGui::CalcTextSize(db);
      float dx2=x2+4.f, dy2=y1;
      dl->AddText({dx2+1,dy2+1},IM_COL32(0,0,0,180),db);
      dl->AddText({dx2,dy2},IM_COL32(180,255,180,220),db);
    }
    if(Cfg::espWeapon){
      int wd=GetWeaponDefForPawn(p.pawn);
      if(wd>0 && GetWeaponFont()){
        const char* iconStr=nullptr;
        float offX=0.f;
        switch(wd){
          case 1:  iconStr="A"; offX=-8.f; break; case 2:  iconStr="B"; offX= 0.f; break; case 3:  iconStr="C"; offX= 0.f; break; case 4:  iconStr="D"; offX= 0.f; break;
          case 64: iconStr="J"; offX=-5.f; break; case 21: iconStr="E"; offX= 0.f; break; case 32: iconStr="F"; offX= 0.f; break; case 36: iconStr="F"; offX= 0.f; break;
          case 61: iconStr="G"; offX= 0.f; break; case 30: iconStr="H"; offX= 0.f; break; case 63: iconStr="I"; offX= 0.f; break; case 17: iconStr="K"; offX= 0.f; break;
          case 24: iconStr="L"; offX=-10.f;break; case 26: iconStr="M"; offX=-10.f;break; case 33: iconStr="N"; offX=-5.f; break; case 34: iconStr="R"; offX=-10.f;break;
          case 19: iconStr="O"; offX=-10.f;break; case 13: iconStr="Q"; offX=-15.f;break; case 10: iconStr="R"; offX=-15.f;break; case 60: iconStr="T"; offX=-15.f;break;
          case 16: iconStr="S"; offX=-15.f;break; case 8:  iconStr="U"; offX=-10.f;break; case 39: iconStr="V"; offX=-15.f;break; case 7:  iconStr="W"; offX=-15.f;break;
          case 11: iconStr="X"; offX=-15.f;break; case 38: iconStr="Y"; offX=-15.f;break; case 9:  iconStr="Z"; offX=-15.f;break; case 40: iconStr="a"; offX=-15.f;break;
          case 25: iconStr="b"; offX=-15.f;break; case 29: iconStr="c"; offX=-15.f;break; case 27: iconStr="d"; offX=-15.f;break; case 20: iconStr="e"; offX=-15.f;break;
          case 28: iconStr="f"; offX=-15.f;break; case 14: iconStr="g"; offX=-15.f;break; case 31: iconStr="h"; offX= 0.f; break; case 42: iconStr="i"; offX= 5.f; break;
          case 41: iconStr="j"; offX= 5.f; break; case 43: iconStr="k"; offX= 5.f; break; case 44: iconStr="l"; offX= 5.f; break; case 45: iconStr="m"; offX= 5.f; break;
          case 46: iconStr="n"; offX= 5.f; break; case 47: iconStr="o"; offX= 0.f; break; default: if(wd>=500){iconStr="]"; offX=-8.f;} break;
        }
        if(iconStr){
          float wOffY=(Cfg::espHp&&Cfg::hpBarSide==2)?18.f:3.f;
          float wy=y2+wOffY;
          ImU32 iconCol=IM_COL32((int)(Cfg::colWeaponIcon[0]*255),(int)(Cfg::colWeaponIcon[1]*255),(int)(Cfg::colWeaponIcon[2]*255),235);
          ImGui::PushFont(GetWeaponFont());
          ImVec2 is=ImGui::CalcTextSize(iconStr);
          float ix=cx-is.x*.5f+offX;
          dl->AddText({ix+1,wy+1},IM_COL32(0,0,0,160),iconStr);
          dl->AddText({ix,wy},iconCol,iconStr);
          ImGui::PopFont();
        }
      }
    }
    if(Cfg::espBones&&p.hasBones){
      ImU32 bc=IM_COL32((int)(Cfg::colBone[0]*255),(int)(Cfg::colBone[1]*255),(int)(Cfg::colBone[2]*255),170);
      for(int j=0;j<p.boneCount;j++){
        Vec2 a,b2;
        if(W2S(p.boneA[j],a,G.vm,W,H)&&W2S(p.boneB[j],b2,G.vm,W,H))
          dl->AddLine({a.x,a.y},{b2.x,b2.y},bc,1.2f);
      }
    }
    if(Cfg::espHeadCircle&&p.hasBones){
      Vec2 headSc;
      if(W2S(p.boneHead,headSc,G.vm,W,H)){
        float rr=bH*0.06f; if(rr<3.f)rr=3.f; if(rr>12.f)rr=12.f;
        ImU32 hcCol=IM_COL32((int)(Cfg::colBone[0]*255),(int)(Cfg::colBone[1]*255),(int)(Cfg::colBone[2]*255),200);
        dl->AddCircle({headSc.x,headSc.y},rr,hcCol,16,1.2f);
      }
    }
    if(Cfg::espHead){
      ImU32 hc=IM_COL32((int)(Cfg::colHead[0]*255),(int)(Cfg::colHead[1]*255),(int)(Cfg::colHead[2]*255),200);
      float hatW=bW*0.70f;
      float hatH=bH*0.14f;
      if(hatW<10.f)hatW=10.f; if(hatH<5.f)hatH=5.f;
      float headY=y1;
      dl->AddTriangleFilled({cx, headY-hatH-4.f},{cx-hatW, headY},{cx+hatW, headY}, hc);
      dl->AddTriangle({cx, headY-hatH-4.f},{cx-hatW, headY},{cx+hatW, headY}, IM_COL32(0,0,0,180),1.5f);
    }
    if(Cfg::espSnap){
      ImU32 sc2=IM_COL32((int)(Cfg::colSnap[0]*255),(int)(Cfg::colSnap[1]*255),(int)(Cfg::colSnap[2]*255),150);
      dl->AddLine({(float)W/2,(float)H},{cx,y2},sc2,1.f);
    }
  }

  if(Cfg::molotovWall){
    ULONGLONG now=GetTickCount64();
    for(int i=0;i<8;i++){
      MolotovZone& z=g_MolotovZones[i];
      if(!z.active) continue;
      float elapsed=(float)(now-z.startTime)*0.001f;
      if(elapsed>=7.f){ z.active=false; continue; }
      float t=elapsed/7.f;
      float radius=30.f+100.f*t;
      float alpha=(elapsed>5.5f)?(1.f-(elapsed-5.5f)/1.5f):1.f;
      if(alpha<0.f)alpha=0.f;
      ImU32 cLine=IM_COL32(255,45,20,(int)(220*alpha));
      ImU32 cFill=IM_COL32(255,45,20,(int)(45*alpha));
      ImU32 cInner=IM_COL32(255,140,20,(int)(25*alpha));
      const int SEGS=32;
      ImVec2 pts[SEGS]; bool ok[SEGS];
      for(int s=0;s<SEGS;s++){
        float ang=(float)s/SEGS*6.28318f;
        Vec3 wp={z.center.x+cosf(ang)*radius,z.center.y+sinf(ang)*radius,z.center.z};
        Vec2 sp; ok[s]=W2S(wp,sp,G.vm,W,H);
        if(ok[s]) pts[s]={sp.x,sp.y};
      }
      ImVec2 poly[SEGS]; int pc=0;
      for(int s=0;s<SEGS;s++) if(ok[s]) poly[pc++]=pts[s];
      if(pc>=3){
        dl->AddConvexPolyFilled(poly,pc,cFill);
        dl->AddConvexPolyFilled(poly,pc,cInner);
      }
      for(int s=0;s<SEGS;s++){
        int nx=(s+1)%SEGS;
        if(ok[s]&&ok[nx]) dl->AddLine(pts[s],pts[nx],cLine,2.f);
      }
      Vec2 cen; if(W2S(z.center,cen,G.vm,W,H)) dl->AddCircleFilled({cen.x,cen.y},4.f,cLine);
    }
  }
}

static void DrawEdgeIndicator(Vec3 worldPos,ImU32 col,const char* lbl){
  ImDrawList* dl=ImGui::GetBackgroundDrawList();
  int W=G.W,H=G.H;if(W<=0||H<=0)return;
  float x=G.vm.m[0][0]*worldPos.x+G.vm.m[0][1]*worldPos.y+G.vm.m[0][2]*worldPos.z+G.vm.m[0][3];
  float y=G.vm.m[1][0]*worldPos.x+G.vm.m[1][1]*worldPos.y+G.vm.m[1][2]*worldPos.z+G.vm.m[1][3];
  float w=G.vm.m[3][0]*worldPos.x+G.vm.m[3][1]*worldPos.y+G.vm.m[3][2]*worldPos.z+G.vm.m[3][3];
  if(w<0){x=-x;y=-y;}
  float len=sqrtf(x*x+y*y);if(len<0.001f)return;
  float nx=x/len,ny=-y/len;
  float cx2=W*.5f,cy2=H*.5f,margin=36.f;
  float ex=cx2+nx*(cx2-margin*1.5f);
  float ey=cy2+ny*(cy2-margin*1.5f);
  if(ex<margin)ex=margin;if(ex>W-margin)ex=(float)W-margin;
  if(ey<margin)ey=margin;if(ey>H-margin)ey=(float)H-margin;
  dl->AddCircleFilled({ex,ey},11.f,col);
  dl->AddCircle({ex,ey},11.f,IM_COL32(0,0,0,160),16,1.5f);
  if(lbl){
    ImVec2 ts=ImGui::CalcTextSize(lbl);
    dl->AddText({ex-ts.x*.5f,ey-ts.y*.5f},IM_COL32(255,255,255,220),lbl);
  }
}

static void DrawDX11Overlay(){
  if(!Cfg::dx11WorldOn) return;
  ImDrawList* dl = ImGui::GetBackgroundDrawList();
  float W=(float)G.W, H=(float)G.H;
  if(W<=0||H<=0) return;
  float* c = Cfg::dx11WorldColor;
  ImU32 wc = IM_COL32((int)(c[0]*255),(int)(c[1]*255),(int)(c[2]*255),(int)(c[3]*255));
  dl->AddRectFilled({0,0},{W,H}, wc);
}

static void DrawEnemyIndicators(){
  if(!Cfg::enemyIndicator) return;
  ImDrawList* dl=ImGui::GetBackgroundDrawList();
  float sw=(float)G.W, sh=(float)G.H;
  if(sw<=0||sh<=0) return;
  float pad=40.f;
  float cx=sw*.5f, cy=sh*.5f;
  float ts=Cfg::enemyIndicatorSize;
  ImU32 col=IM_COL32((int)(Cfg::enemyIndicatorCol[0]*255),(int)(Cfg::enemyIndicatorCol[1]*255),(int)(Cfg::enemyIndicatorCol[2]*255),(int)(Cfg::enemyIndicatorCol[3]*255));
  ImU32 outline=IM_COL32(0,0,0,200);
  for(int i=0;i<G.playerCount;i++){
    const PlayerSnap& p=G.players[i];
    if(!p.valid||p.health<=0) continue;
    if(p.team==G.localTeam&&G.localTeam!=0) continue;
    Vec3 target={p.origin.x, p.origin.y, p.origin.z+60.f};
    Vec2 sc;
    bool onScreen=W2S(target,sc,G.vm,G.W,G.H);
    bool visible=onScreen&&sc.x>=pad&&sc.x<=sw-pad&&sc.y>=pad&&sc.y<=sh-pad;
    if(visible) continue;
    float ex=G.vm.m[0][0]*target.x+G.vm.m[0][1]*target.y+G.vm.m[0][2]*target.z+G.vm.m[0][3];
    float ey=G.vm.m[1][0]*target.x+G.vm.m[1][1]*target.y+G.vm.m[1][2]*target.z+G.vm.m[1][3];
    float ew=G.vm.m[3][0]*target.x+G.vm.m[3][1]*target.y+G.vm.m[3][2]*target.z+G.vm.m[3][3];
    if(ew<0){ex=-ex;ey=-ey;}
    float len=sqrtf(ex*ex+ey*ey);
    if(len<0.001f) continue;
    float nx= ex/len;
    float ny=-ey/len;
    float tx=(nx>0)?(sw-pad-cx)/nx:(-cx+pad)/nx;
    float ty=(ny>0)?(sh-pad-cy)/ny:(-cy+pad)/ny;
    float t=((nx!=0&&fabsf(tx)<fabsf(ty))||ny==0)?fabsf(tx):fabsf(ty);
    float ax=cx+nx*t, ay=cy+ny*t;
    if(ax<pad)ax=pad; if(ax>sw-pad)ax=sw-pad;
    if(ay<pad)ay=pad; if(ay>sh-pad)ay=sh-pad;
    float ang=atan2f(ny,nx);
    ImVec2 tip ={ax+cosf(ang)*ts,      ay+sinf(ang)*ts};
    ImVec2 lft ={ax-cosf(ang)*ts*.6f+sinf(ang)*ts, ay-sinf(ang)*ts*.6f-cosf(ang)*ts};
    ImVec2 rgt ={ax-cosf(ang)*ts*.6f-sinf(ang)*ts, ay-sinf(ang)*ts*.6f+cosf(ang)*ts};
    dl->AddTriangleFilled(tip,lft,rgt,col);
    dl->AddTriangle(tip,lft,rgt,outline,1.5f);
    float dist=Dist3D(G.localPos,target);
    char dbuf[16]; snprintf(dbuf,16,"%.0fm",dist/100.f);
    ImVec2 ts2=ImGui::CalcTextSize(dbuf);
    dl->AddText({ax-ts2.x*.5f,ay+ts+2.f},IM_COL32(255,255,255,180),dbuf);
  }
}

static void DrawGrenadeESP(){
  if(!Cfg::grenadeESP)return;
  ImDrawList* dl=ImGui::GetBackgroundDrawList();
  int W=G.W,H=G.H;if(W<=0||H<=0)return;
  for(int i=0;i<G.grenadeCount;i++){
    const GrenadeSnap& g=G.grenades[i];
    if(!g.valid)continue;
    bool isMine=(g.team==G.localTeam&&G.localTeam!=0);
    float* tRC=(g.team==3)?Cfg::colGrenadeCT:Cfg::colGrenadeT;
    ImU32 dotCol=isMine?IM_COL32(50,200,50,220):IM_COL32(255,80,80,220);
    for(int t=1;t<g.trailCount;t++){
      Vec2 ta,tb;
      if(W2S(g.trail[t-1],ta,G.vm,W,H)&&W2S(g.trail[t],tb,G.vm,W,H)){
        float alpha=(float)t/g.trailCount;
        ImU32 tc=IM_COL32((int)(tRC[0]*255),(int)(tRC[1]*255),(int)(tRC[2]*255),(int)(alpha*(isMine?230.f:160.f)));
        dl->AddLine({ta.x,ta.y},{tb.x,tb.y},tc,isMine?2.5f:1.5f);
      }
    }
    Vec2 sc;
    bool vis=W2S(g.pos,sc,G.vm,W,H);
    if(isMine&&!vis) DrawEdgeIndicator(g.pos,dotCol,"G");
    else if(vis){
      if(g.detonated) dl->AddCircle({sc.x,sc.y},10.f,IM_COL32(255,100,50,160),16,1.5f);
      else {
        dl->AddCircleFilled({sc.x,sc.y},5.f,dotCol);
        dl->AddCircle({sc.x,sc.y},5.f,IM_COL32(0,0,0,150),12,1.f);
        if(g.vel.Len()>50.f){
          Vec3 ahead=g.pos+g.vel*0.15f; Vec2 sca;
          if(W2S(ahead,sca,G.vm,W,H)) dl->AddLine({sc.x,sc.y},{sca.x,sca.y},dotCol,1.5f);
        }
      }
    }
  }
  for(int i=0;i<G.infernoCount;i++){
    const InfernoSnap& inf=G.infernos[i];
    if(!inf.valid)continue;
    Vec2 sc; bool vis=W2S(inf.pos,sc,G.vm,W,H);
    bool friendly=(inf.team==G.localTeam&&G.localTeam);
    ImU32 fc =friendly?IM_COL32(255,165,0,140):IM_COL32(255,50,0,140);
    ImU32 fc2=friendly?IM_COL32(255,200,80,220):IM_COL32(255,100,0,220);
    if(vis){
      dl->AddCircleFilled({sc.x,sc.y},16.f,fc);
      dl->AddText({sc.x-12.f,sc.y-8.f},IM_COL32(255,255,200,220),"ATES");
    } else if(Cfg::molotovWall) DrawEdgeIndicator(inf.pos,fc2,"M");
  }
}

static void DrawHUD(){
  ImDrawList* dl=ImGui::GetForegroundDrawList();
  int W=G.W,H=G.H;if(W<=0||H<=0)return;
  // FPS
  if(Cfg::noFlash&&s_FlashDurCached>0.05f){
    float ratio=s_FlashDurCached/3.5f;if(ratio>1.f)ratio=1.f;
    float overlayA=ratio*0.30f;
    dl->AddRectFilled({0,0},{(float)W,(float)H},IM_COL32(255,255,255,(int)(overlayA*255)));
    float ex=(float)W*.5f,ey=(float)H-70.f;
    dl->AddCircleFilled({ex,ey},8.f,IM_COL32(40,40,50,200));
    dl->AddCircleFilled({ex,ey},4.f,IM_COL32(255,255,255,200));
    dl->AddLine({ex-24.f,ey-24.f},{ex+24.f,ey+24.f},IM_COL32(200,50,50,220),2.5f);
    char fb[32];snprintf(fb,32,"FLASH: %.1fs",s_FlashDurCached);
    ImVec2 ts=ImGui::CalcTextSize(fb);
    dl->AddText({ex-ts.x*.5f,ey+26.f},IM_COL32(40,40,50,220),fb);
  }
  if(Cfg::custXhair&&!Cfg::menuOpen){
    float cx3=(float)W*.5f,cy3=(float)H*.5f,szz=Cfg::custXhairSize,gap=(float)Cfg::custXhairGap,th=Cfg::custXhairThick;
    ImU32 xc=IM_COL32((int)(Cfg::colXhair[0]*255),(int)(Cfg::colXhair[1]*255),(int)(Cfg::colXhair[2]*255),230);
    ImU32 xs=IM_COL32(0,0,0,150);
    dl->AddLine({cx3-szz-1,cy3+1},{cx3-gap-1,cy3+1},xs,th+1.5f);
    dl->AddLine({cx3+gap+1,cy3+1},{cx3+szz+1,cy3+1},xs,th+1.5f);
    dl->AddLine({cx3+1,cy3-szz-1},{cx3+1,cy3-gap-1},xs,th+1.5f);
    dl->AddLine({cx3+1,cy3+gap+1},{cx3+1,cy3+szz+1},xs,th+1.5f);
    dl->AddLine({cx3-szz,cy3},{cx3-gap,cy3},xc,th);
    dl->AddLine({cx3+gap,cy3},{cx3+szz,cy3},xc,th);
    dl->AddLine({cx3,cy3-szz},{cx3,cy3-gap},xc,th);
    dl->AddLine({cx3,cy3+gap},{cx3,cy3+szz},xc,th);
  }
  if(Cfg::noScope&&!Cfg::menuOpen&&G.localPawn>0x10000&&G.scoped){
    float cx3=(float)W*.5f,cy3=(float)H*.5f,ll=(float)H*.42f;
    dl->AddRectFilled({0,0},{cx3-ll,(float)H},IM_COL32(0,0,0,210));
    dl->AddRectFilled({cx3+ll,0},{(float)W,(float)H},IM_COL32(0,0,0,210));
    ImU32 lc=IM_COL32(200,200,200,180);
    dl->AddLine({cx3-ll,cy3},{cx3+ll,cy3},lc,1.f);
    dl->AddLine({cx3,cy3-ll},{cx3,cy3+ll},lc,1.f);
    dl->AddCircleFilled({cx3,cy3},2.f,IM_COL32(255,255,255,200));
    dl->AddCircle({cx3,cy3},ll,IM_COL32(160,160,160,110),128,1.5f);
  }
}

HRESULT __stdcall hkPresent(IDXGISwapChain* sc,UINT sync,UINT flags){
  if(g_NeedReinit.load()){
    g_NeedReinit=false;
    ImGuiShutdown();
    g_ReinitDelay=3;
  }
  if(g_ReinitDelay>0){ g_ReinitDelay--; return oPresent(sc,sync,flags); }
  if(!g_Init){if(!ImGuiInit(sc))return oPresent(sc,sync,flags);g_Init=true;}
  if(!g_RTV&&g_Dev){
    ID3D11Texture2D* bb=nullptr;
    if(SUCCEEDED(sc->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&bb))){
      g_Dev->CreateRenderTargetView(bb,nullptr,&g_RTV);bb->Release();
    }
    if(!g_RTV)return oPresent(sc,sync,flags);
  }
  {static bool iw=false;bool in2=(GetAsyncKeyState(VK_INSERT)&0x8000)!=0;
   if(in2&&!iw){
     if(g_windowArrangeMode){
       g_windowArrangeMode=false; g_menuTarget=true; Cfg::menuOpen=true; g_menuAnim=1.f;
     } else { g_menuTarget=!g_menuTarget; Cfg::menuOpen=g_menuTarget; }
   }iw=in2;}
  {
    float dt=ImGui::GetIO().DeltaTime;
    float target=g_menuTarget?1.0f:0.0f;
    float speed=22.f;
    g_menuAnim+=(target-g_menuAnim)*fminf(1.0f,dt*speed);
    if(!g_menuTarget&&g_menuAnim<0.005f)g_menuAnim=0.0f;
  }
  if(G.localPawn>0x10000)
    s_FlashDurCached=SafeRead<float>(G.localPawn+Off::m_flFlashDur);
  else s_FlashDurCached=0.f;
  G.Update();
  EnsureSkyboxHook();
  RunFeatures();
  BT::Tick();
  if(G.localPawn > 0x10000) Skin::Tick();
  VisExt::RunNoSmoke(G.client, G.localPawn);
  CL::FrameTick();
  ID3D11RenderTargetView* oldRTV = nullptr;
  ID3D11DepthStencilView* oldDSV = nullptr;
  ID3D11DepthStencilState* oldDSS = nullptr; UINT oldStencilRef = 0;
  ID3D11BlendState* oldBlend = nullptr; float oldBlendFactor[4]{}; UINT oldBlendMask = 0;
  ID3D11RasterizerState* oldRS = nullptr;
  D3D11_VIEWPORT oldVP{}; UINT vpCount = 1;
  ID3D11PixelShader* oldPS = nullptr;
  ID3D11VertexShader* oldVS = nullptr;
  if (g_Ctx) {
    g_Ctx->OMGetRenderTargets(1, &oldRTV, &oldDSV);
    g_Ctx->OMGetDepthStencilState(&oldDSS, &oldStencilRef);
    g_Ctx->OMGetBlendState(&oldBlend, oldBlendFactor, &oldBlendMask);
    g_Ctx->RSGetState(&oldRS);
    g_Ctx->RSGetViewports(&vpCount, &oldVP);
    g_Ctx->PSGetShader(&oldPS, nullptr, nullptr);
    g_Ctx->VSGetShader(&oldVS, nullptr, nullptr);
  }
  if(g_RTV && g_Ctx) {
    g_Ctx->OMSetRenderTargets(1, &g_RTV, nullptr);
    D3D11_VIEWPORT vp{};
    vp.Width    = (float)G.W;
    vp.Height   = (float)G.H;
    vp.MinDepth = 0.f;
    vp.MaxDepth = 1.f;
    g_Ctx->RSSetViewports(1, &vp);
  }
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  if((Cfg::menuOpen||g_windowArrangeMode)&&g_Wnd){
    ImGuiIO& io=ImGui::GetIO();
    io.DisplaySize={(float)G.W,(float)G.H};
    ClipCursor(nullptr);SetCursor(LoadCursor(NULL,IDC_ARROW));
    RECT cr;GetClientRect(g_Wnd,&cr);
    float wW=(float)(cr.right-cr.left),wH=(float)(cr.bottom-cr.top);
    POINT pt;GetCursorPos(&pt);ScreenToClient(g_Wnd,&pt);
    if(wW>0&&wH>0)io.MousePos={(float)pt.x*(float)G.W/wW,(float)pt.y*(float)G.H/wH};
    io.MouseDown[0]=(GetAsyncKeyState(VK_LBUTTON)&0x8000)!=0;
    io.MouseDown[1]=(GetAsyncKeyState(VK_RBUTTON)&0x8000)!=0;
    io.MouseDown[2]=(GetAsyncKeyState(VK_MBUTTON)&0x8000)!=0;
  } else {ImGui::GetIO().MousePos={-1,-1};}

  ImGui::NewFrame();
  if(!g_introShown){
    float dt2=ImGui::GetIO().DeltaTime;
    if(dt2>0.1f)dt2=0.1f;
    g_introTimer+=dt2;
    float prog=g_introTimer/kIntroDur;
    if(prog>=1.0f){
      g_introShown=true; prog=1.0f;
      g_menuTarget=true;
      Cfg::menuOpen=true;
      g_menuAnim=1.0f;
    }
    ImDrawList* fg=ImGui::GetForegroundDrawList();
    float W=(float)G.W, H=(float)G.H;
    float bgA = prog<0.6f ? 1.0f : 1.0f-(prog-0.6f)/0.4f;
    fg->AddRectFilled({0,0},{W,H},IM_COL32(0,0,0,(int)(bgA*255)));
    float txtA = prog<0.1f ? 0.f :
                 prog<0.55f ? (prog-0.1f)/0.45f :
                 prog<0.65f ? 1.0f :
                 (1.0f-(prog-0.65f)/0.35f);
    if(txtA>0.01f){
      float tSlide = prog<0.3f ? (1.0f-prog/0.3f)*28.f : 0.f;
      ImVec2 tsz0=ImGui::CalcTextSize("QUAFIND");
      float scl=2.2f;
      ImVec2 tsz={tsz0.x*scl, tsz0.y*scl};
      ImVec2 tp={W*.5f-tsz.x*.5f, H*.5f-tsz.y*.5f - 22.f - tSlide};
      fg->AddText(nullptr,tsz.y,{tp.x+2,tp.y+2},IM_COL32(0,180,60,(int)(txtA*80)),"QUAFIND");
      fg->AddText(nullptr,tsz.y,tp,IM_COL32(40,220,100,(int)(txtA*180)),"QUAFIND");
      fg->AddText(nullptr,tsz.y,tp,IM_COL32(200,255,220,(int)(txtA*255)),"QUAFIND");
      const char* sub="open source cheat";
      ImVec2 ssz=ImGui::CalcTextSize(sub);
      ImVec2 sp2={W*.5f-ssz.x*.5f, tp.y+tsz.y+8.f};
      fg->AddText(sp2,IM_COL32(120,200,150,(int)(txtA*200)),sub);
    }
  }
    if(Cfg::screenFogOn){
    ImDrawList* bg=ImGui::GetBackgroundDrawList();
    float W=(float)G.W, H=(float)G.H;
    float thH=H*0.22f, thW=W*0.18f; // kenar
    auto C4=[](float* c)->ImU32{return IM_COL32((int)(c[0]*255),(int)(c[1]*255),(int)(c[2]*255),(int)(c[3]*255));};
    auto C0=IM_COL32(0,0,0,0);
    if(Cfg::screenFogTop){
      ImU32 c=C4(Cfg::colFogTop);
      bg->AddRectFilledMultiColor({0,0},{W,thH},c,c,C0,C0);
    }
    if(Cfg::screenFogBottom){
      ImU32 c=C4(Cfg::colFogBottom);
      bg->AddRectFilledMultiColor({0,H-thH},{W,H},C0,C0,c,c);
    }
    if(Cfg::screenFogLeft){
      ImU32 c=C4(Cfg::colFogLeft);
      bg->AddRectFilledMultiColor({0,0},{thW,H},c,C0,C0,c);
    }
    if(Cfg::screenFogRight){
      ImU32 c=C4(Cfg::colFogRight);
      bg->AddRectFilledMultiColor({W-thW,0},{W,H},C0,c,c,C0);
    }
  }
  VisExt::DrawSkyOverlay((float)G.W, (float)G.H);
  DrawDX11Overlay();  // Dunya
  VisExt::DrawParticles(ImGui::GetIO().DeltaTime, G.vm, G.localPos, G.W, G.H);
  DrawESP();
  DrawGrenadeESP();
  DrawEnemyIndicators();
  DrawHUD();
  // Hitmarker
  { float dt=ImGui::GetIO().DeltaTime;
    HM::Tick(dt);
    HM::Render((float)G.W,(float)G.H,dt);
  }
  bool canDrag = Cfg::menuOpen || g_windowArrangeMode;
  auto WFlags=[&](){ return ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
    ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoScrollbar|
    (canDrag?0:ImGuiWindowFlags_NoMove); };

    if(Cfg::spectatorListOn){
    ImGui::SetNextWindowBgAlpha(0.78f);
    auto& _io=ImGui::GetIO();
    float _sw=_io.DisplaySize.x, _sh=_io.DisplaySize.y;
    if(Cfg::specWinX<0.f)Cfg::specWinX=0.f; if(Cfg::specWinX>_sw-80.f)Cfg::specWinX=_sw-80.f;
    if(Cfg::specWinY<0.f)Cfg::specWinY=0.f; if(Cfg::specWinY>_sh-40.f)Cfg::specWinY=_sh-40.f;
    ImGui::SetNextWindowPos({Cfg::specWinX,Cfg::specWinY},ImGuiCond_Always);
    if(ImGui::Begin("##SpecL",nullptr,WFlags())){
      if(canDrag && ImGui::IsWindowHovered()){
        if(ImGui::IsMouseDragging(0,0.f)){
          ImVec2 d=ImGui::GetMouseDragDelta(0,0.f);
          ImGui::ResetMouseDragDelta(0);
          Cfg::specWinX+=d.x; Cfg::specWinY+=d.y;
        }
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
      }
      ImGui::TextColored(ImVec4(.4f,.9f,.5f,1.f),u8"\u0130zleyenler");
      ImGui::Separator();
      int cnt=0;
      if(G.client&&G.localPawn>0x10000){
        uintptr_t el=SafeRead<uintptr_t>(G.client+Off::dwEntityList);
        if(el>0x10000){
          for(int i=1;i<64;i++){
            uintptr_t ctrl=GetEntity(el,i); if(!ctrl||ctrl<=0x10000) continue;
            uint32_t ph=SafeRead<uint32_t>(ctrl+Off::m_hPlayerPawn);
            if(!ph||ph==0xFFFFFFFF) continue;
            uintptr_t pawn=GetPawnByHandle(el,ph); if(!pawn||pawn<=0x10000) continue;
            if(pawn==G.localPawn) continue;
            // Sadece spectate edebilirler (olu olmali)
            uint8_t ls=SafeRead<uint8_t>(pawn+Off::m_lifeState);
            if(ls==0) continue;
            uintptr_t obsSvc=SafeRead<uintptr_t>(pawn+Off::m_pObserverServices);
            if(!obsSvc||obsSvc<=0x10000) continue;
            uint8_t mode=SafeRead<uint8_t>(obsSvc+Off::m_iObserverMode); if(!mode) continue;
            uint32_t tgtH=SafeRead<uint32_t>(obsSvc+Off::m_hObserverTarget);
            if(!tgtH||tgtH==0xFFFFFFFF) continue;
            uintptr_t tgt=GetPawnByHandle(el,tgtH);
            if(tgt!=G.localPawn) continue;
            char nm[64]={};
            SafeReadStr(ctrl+Off::m_iszPlayerName,nm,64);
            if(!nm[0]) snprintf(nm,64,"Player %d",i);
            ImGui::TextColored(ImVec4(.9f,.9f,.5f,1.f),"%s",nm);
            cnt++;
          }
        }
      }
      if(!cnt) ImGui::TextColored(ImVec4(.5f,.5f,.5f,1.f),"-");
    }
    ImGui::End();
  }

   if(Cfg::shortcutListOn){
    ImGui::SetNextWindowBgAlpha(0.78f);
    auto& _io2=ImGui::GetIO();
    float _sw2=_io2.DisplaySize.x, _sh2=_io2.DisplaySize.y;
    if(Cfg::scWinX<0.f)Cfg::scWinX=0.f; if(Cfg::scWinX>_sw2-80.f)Cfg::scWinX=_sw2-80.f;
    if(Cfg::scWinY<0.f)Cfg::scWinY=0.f; if(Cfg::scWinY>_sh2-40.f)Cfg::scWinY=_sh2-40.f;
    ImGui::SetNextWindowPos({Cfg::scWinX,Cfg::scWinY},ImGuiCond_Always);
    if(ImGui::Begin("##KL",nullptr,WFlags())){
      if(canDrag && ImGui::IsWindowHovered()){
        if(ImGui::IsMouseDragging(0,0.f)){
          ImVec2 d=ImGui::GetMouseDragDelta(0,0.f);
          ImGui::ResetMouseDragDelta(0);
          Cfg::scWinX+=d.x; Cfg::scWinY+=d.y;
        }
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
      }
      ImGui::TextColored(ImVec4(.4f,.9f,.5f,1.f),u8"K\u0131sayollar");
      ImGui::Separator();
      auto VKName=[](int k)->const char*{
        static char buf[16];
        switch(k){
          case VK_LBUTTON:return"M1";case VK_RBUTTON:return"M2";
          case VK_MBUTTON:return"M3";case VK_XBUTTON1:return"M4";
          case VK_XBUTTON2:return"M5";case VK_BACK:return"Backspace";
          case VK_TAB:return"Tab";case VK_RETURN:return"Enter";
          case VK_SHIFT:return"Shift";case VK_CONTROL:return"Ctrl";
          case VK_MENU:return"Alt";case VK_CAPITAL:return"CapsLock";
          case VK_ESCAPE:return"Esc";case VK_SPACE:return"Space";
          case VK_PRIOR:return"PgUp";case VK_NEXT:return"PgDn";
          case VK_HOME:return"Home";case VK_END:return"End";
          case VK_INSERT:return"Insert";case VK_DELETE:return"Delete";
          case VK_LEFT:return"Left";case VK_RIGHT:return"Right";
          case VK_UP:return"Up";case VK_DOWN:return"Down";
          case VK_LSHIFT:return"LShift";case VK_RSHIFT:return"RShift";
          case VK_LCONTROL:return"LCtrl";case VK_RCONTROL:return"RCtrl";
          case VK_LMENU:return"LAlt";case VK_RMENU:return"RAlt";
          default:
            if(k>=VK_F1&&k<=VK_F24){snprintf(buf,16,"F%d",k-VK_F1+1);return buf;}
            if(k>='0'&&k<='9'){snprintf(buf,16,"%c",(char)k);return buf;}
            if(k>='A'&&k<='Z'){snprintf(buf,16,"%c",(char)k);return buf;}
            snprintf(buf,16,"[%d]",k);return buf;
        }
      };
      auto KRow=[&](const char* f,int k){
        if(!k)return;
        ImGui::TextColored(ImVec4(.9f,.9f,.5f,1.f),"%s",f);
        ImGui::SameLine(130);ImGui::TextColored(ImVec4(.4f,.9f,.5f,1.f),"%s",VKName(k));
      };
      if(Cfg::glowOn) KRow("Glow",Cfg::glowKey);
      if(Cfg::chamsOn)KRow("Chams",Cfg::chamsKey);
      if(Cfg::aimOn)  KRow("Legit Aim",Cfg::aimKey);
      if(Cfg::rageOn) KRow("Rage Aim",Cfg::rageKey);
      if(Cfg::trigOn) KRow("TriggerBot",Cfg::trigKey);
      if(Cfg::bhopOn) KRow("BHop",Cfg::bhopKey);
      if(Cfg::aaOn)   KRow(u8"Anti-Aim",Cfg::aaKey);
    }
    ImGui::End();
  }

   // ACTIVE
  bool bombEndVisible = (BT::state==BT::State::DEFUSED || BT::state==BT::State::EXPLODED)
                        && BT::s_endTick && (GetTickCount64()-BT::s_endTick) < 10000;
  bool showBomb = Cfg::bombTimerOn &&
    (BT::state==BT::State::ACTIVE || bombEndVisible || g_windowArrangeMode || Cfg::menuOpen);
  if(showBomb){
    ImGui::SetNextWindowBgAlpha(0.82f);
    auto& _io3=ImGui::GetIO();
    float _sw3=_io3.DisplaySize.x, _sh3=_io3.DisplaySize.y;
    if(Cfg::bombWinX<0.f)Cfg::bombWinX=0.f; if(Cfg::bombWinX>_sw3-80.f)Cfg::bombWinX=_sw3-80.f;
    if(Cfg::bombWinY<0.f)Cfg::bombWinY=0.f; if(Cfg::bombWinY>_sh3-40.f)Cfg::bombWinY=_sh3-40.f;
    ImGui::SetNextWindowPos({Cfg::bombWinX,Cfg::bombWinY},ImGuiCond_Always);
    if(ImGui::Begin("##BombWin",nullptr,WFlags())){
      if(canDrag && ImGui::IsWindowHovered()){
        if(ImGui::IsMouseDragging(0,0.f)){
          ImVec2 d=ImGui::GetMouseDragDelta(0,0.f);
          ImGui::ResetMouseDragDelta(0);
          Cfg::bombWinX+=d.x; Cfg::bombWinY+=d.y;
        }
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
      }
      if(BT::state==BT::State::ACTIVE){
        const char* st=BT::site==0?"A":"B";
        ImGui::TextColored(ImVec4(1.f,.4f,.2f,1.f),u8"BOMBA \u2014 %s B\u00d6LGES\u0130",st);
        ImGui::Separator();
        float tl=BT::timeLeft;
        bool danger=tl<=10.f;
        ImU32 tc=danger?IM_COL32(255,80,50,255):IM_COL32(50,220,80,255);
        ImGui::TextColored(danger?ImVec4(1.f,.3f,.2f,1.f):ImVec4(.2f,.9f,.3f,1.f),
          u8"Patlama: %.1f sn",tl);
        float frac=BT::s_timerLen>0.f?tl/BT::s_timerLen:0.f;
        if(frac>1.f)frac=1.f; if(frac<0.f)frac=0.f;
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram,tc);
        ImGui::ProgressBar(frac,{170.f,7.f},"");
        ImGui::PopStyleColor();
      } else if(BT::state==BT::State::DEFUSED){
        ImGui::TextColored(ImVec4(.2f,1.f,.4f,1.f),u8"BOMBA \u00c7\u00d6Z\u00dcLD\u00dc!");
      } else if(BT::state==BT::State::EXPLODED){
        ImGui::TextColored(ImVec4(1.f,.3f,.1f,1.f),u8"BOMBA PATLADI!");
      } else {
        ImGui::TextColored(ImVec4(.5f,.5f,.5f,1.f),u8"C4 Bomba Zamanlayici");
        ImGui::TextColored(ImVec4(.4f,.4f,.4f,1.f),u8"(Onizleme)");
      }
    }
    ImGui::End();
  }

  // QuaFind
  if(Cfg::infoOverlayOn){
    ImGuiIO& _iio=ImGui::GetIO();
    float _sw=_iio.DisplaySize.x,_sh=_iio.DisplaySize.y;
    if(Cfg::infoWinX<0.f) Cfg::infoWinX=_sw-155.f;
    if(Cfg::infoWinY<0.f) Cfg::infoWinY=8.f;
    if(Cfg::infoWinX<0.f)Cfg::infoWinX=0.f;
    if(Cfg::infoWinX>_sw-60.f)Cfg::infoWinX=_sw-60.f;
    if(Cfg::infoWinY<0.f)Cfg::infoWinY=0.f;
    if(Cfg::infoWinY>_sh-30.f)Cfg::infoWinY=_sh-30.f;
    ImGui::SetNextWindowPos({Cfg::infoWinX,Cfg::infoWinY},ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.88f);
    ImGuiWindowFlags _inf=ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_AlwaysAutoResize|
                          ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove;
    if(ImGui::Begin("##InfoOv",nullptr,_inf)){
      // Surukleme
      if(canDrag && ImGui::IsWindowHovered()){
        if(ImGui::IsMouseDragging(0,0.f)){
          ImVec2 d=ImGui::GetMouseDragDelta(0,0.f);
          ImGui::ResetMouseDragDelta(0);
          Cfg::infoWinX+=d.x; Cfg::infoWinY+=d.y;
        }
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
      }
      // Baslik
      ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(.35f,.95f,.5f,1.f));
      ImGui::Text("QUAFIND");
      ImGui::PopStyleColor();
      ImGui::SameLine(0,3);
      ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(.6f,.6f,.6f,1.f));
      ImGui::Text("QuaFind");
      ImGui::PopStyleColor();
      ImGui::Separator();
      // FPS
      char _fb[32]; snprintf(_fb,32,"FPS  %.0f",_iio.Framerate);
      ImGui::TextColored(ImVec4(.35f,.9f,.5f,1.f),"%s",_fb);
      // Saat
      SYSTEMTIME _st; GetLocalTime(&_st);
      char _tb[32]; snprintf(_tb,32,"%02d:%02d:%02d",_st.wHour,_st.wMinute,_st.wSecond);
      ImGui::TextColored(ImVec4(.55f,.55f,.55f,1.f),"%s",_tb);
      // Alt
      {
        ImDrawList* _wdl=ImGui::GetWindowDrawList();
        ImVec2 wp=ImGui::GetWindowPos();
        ImVec2 ws=ImGui::GetWindowSize();
        float t=(float)ImGui::GetTime()*0.35f;
        const int segs=24;
        float barH=3.5f;
        float barY=wp.y+ws.y-barH;
        for(int i=0;i<segs;i++){
          float f0=(float)i/segs, f1=(float)(i+1)/segs;
          float x0=wp.x+f0*ws.x, x1=wp.x+f1*ws.x;
          float r0,g0,b0,r1,g1,b1;
          ImGui::ColorConvertHSVtoRGB(fmodf(f0+t,1.f),1.f,1.f,r0,g0,b0);
          ImGui::ColorConvertHSVtoRGB(fmodf(f1+t,1.f),1.f,1.f,r1,g1,b1);
          ImU32 c0=IM_COL32((int)(r0*255),(int)(g0*255),(int)(b0*255),230);
          ImU32 c1=IM_COL32((int)(r1*255),(int)(g1*255),(int)(b1*255),230);
          _wdl->AddRectFilledMultiColor({x0,barY},{x1,barY+barH},c0,c1,c1,c0);
        }
      }
    }
    ImGui::End();
  }

    if(g_windowArrangeMode){
    float W=(float)G.W;
    ImGui::SetNextWindowPos({W*.5f-128.f,6.f},ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.92f);
    ImGuiWindowFlags af=ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_AlwaysAutoResize|
                        ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar;
    if(ImGui::Begin("##ArrMode",nullptr,af)){
      ImGui::TextColored(ImVec4(.3f,.9f,.4f,1.f),u8"PENCERE DUZENLEME MODU");
      ImGui::TextColored(ImVec4(.5f,.7f,.5f,1.f),u8"Cikis: [INSERT] veya asagidaki tusa tikla");
      ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(.1f,.5f,.2f,1.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(.15f,.7f,.25f,1.f));
      if(ImGui::Button(u8"\u2713 Kaydet ve Cik",{255.f,28.f})){
        g_windowArrangeMode=false;
        g_menuTarget=true; Cfg::menuOpen=true; g_menuAnim=1.f;
      }
      ImGui::PopStyleColor(2);
    }
    ImGui::End();
  }

  if(!g_windowArrangeMode) DrawMenu();

  ImGui::EndFrame();
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

   if (g_Ctx) {
    g_Ctx->OMSetRenderTargets(1, &oldRTV, oldDSV);
    g_Ctx->OMSetDepthStencilState(oldDSS, oldStencilRef);
    g_Ctx->OMSetBlendState(oldBlend, oldBlendFactor, oldBlendMask);
    g_Ctx->RSSetState(oldRS);
    if (vpCount > 0) g_Ctx->RSSetViewports(vpCount, &oldVP);
    g_Ctx->PSSetShader(oldPS, nullptr, 0);
    g_Ctx->VSSetShader(oldVS, nullptr, 0);
  }
  // COM
  if (oldRTV)   oldRTV->Release();
  if (oldDSV)   oldDSV->Release();
  if (oldDSS)   oldDSS->Release();
  if (oldBlend) oldBlend->Release();
  if (oldRS)    oldRS->Release();
  if (oldPS)    oldPS->Release();
  if (oldVS)    oldVS->Release();

  RestoreSpotted();
  return oPresent(sc,sync,flags);
}


typedef void(__stdcall* PFN_DrawIndexedInstanced)(
  ID3D11DeviceContext*,UINT,UINT,UINT,INT,UINT);

static PFN_DrawIndexedInstanced oDrawIndexedInstanced = nullptr;

static ID3D11PixelShader*       pChamsVisPS    = nullptr;
static ID3D11PixelShader*       pChamsOccPS    = nullptr;
static ID3D11DepthStencilState* pChamsOccState = nullptr;
static ID3D11DepthStencilState* pChamsVisState = nullptr;
static ID3D11BlendState*        pChamsBlend    = nullptr;
static ID3D11Buffer*            pChamsColorBuf = nullptr;
static ID3D11RasterizerState*   pChamsRS_solid = nullptr;
static ID3D11RasterizerState*   pChamsRS_wire  = nullptr;
static bool                     g_ChamsReady   = false;


static const char* g_psVisCode =
  "struct PS_INPUT { float4 pos : SV_POSITION; };"
  "cbuffer cbColor { float4 color; };"
  "float hash(float2 p) {"
  "    p = frac(p * float2(443.897, 441.423));"
  "    p += dot(p, p.yx + 19.19);"
  "    return frac(p.x * p.y);"
  "}"
  "float noise(float2 uv) {"
  "    float2 i = floor(uv);"
  "    float2 f = frac(uv);"
  "    f = f * f * (3.0 - 2.0 * f);"
  "    float a = hash(i);"
  "    float b = hash(i + float2(1.0, 0.0));"
  "    float c = hash(i + float2(0.0, 1.0));"
  "    float d = hash(i + float2(1.0, 1.0));"
  "    return lerp(lerp(a, b, f.x), lerp(c, d, f.x), f.y);"
  "}"
  "float4 main(PS_INPUT input) : SV_Target {"
  "    float2 uv = input.pos.xy * 0.001;"
  "    float light = 0.5 + 0.5 * (1.0 - uv.y);"
  "    float n = noise(uv * 15.0);"
  "    float glow = n * n;"
  "    float3 final = color.rgb * light + glow * color.rgb * 0.8;"
  "    return float4(final, color.a);"
  "}";

static const char* g_psOccCode =
  "struct PS_INPUT { float4 pos : SV_POSITION; };"
  "cbuffer cbColor { float4 color; };"
  "float4 main(PS_INPUT input) : SV_Target {"
  "    float2 uv = input.pos.xy * 0.001;"
  "    float light = 0.5 + 0.5 * (1.0 - uv.y);"
  "    return float4(color.rgb * light, color.a);"
  "}";

static void InitChamsResources(ID3D11Device* pDev) {
  if (g_ChamsReady || !pDev) return;

  ID3DBlob* blob = nullptr;
  // Visible
  if (SUCCEEDED(D3DCompile(g_psVisCode, strlen(g_psVisCode), nullptr, nullptr, nullptr,
      "main", "ps_4_0", 0, 0, &blob, nullptr))) {
    pDev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pChamsVisPS);
    blob->Release(); blob = nullptr;
  }
  // Occluded
  if (SUCCEEDED(D3DCompile(g_psOccCode, strlen(g_psOccCode), nullptr, nullptr, nullptr,
      "main", "ps_4_0", 0, 0, &blob, nullptr))) {
    pDev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pChamsOccPS);
    blob->Release(); blob = nullptr;
  }

  // Color
  D3D11_BUFFER_DESC cbd{};
  cbd.Usage = D3D11_USAGE_DEFAULT;
  cbd.ByteWidth = 16;
  cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  pDev->CreateBuffer(&cbd, nullptr, &pChamsColorBuf);

  // Blend
  D3D11_BLEND_DESC bd{};
  bd.RenderTarget[0].BlendEnable = TRUE;
  bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  pDev->CreateBlendState(&bd, &pChamsBlend);

  // Occluded
  D3D11_DEPTH_STENCIL_DESC occD{};
  occD.DepthEnable = TRUE;
  occD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  occD.DepthFunc = D3D11_COMPARISON_GREATER;
  pDev->CreateDepthStencilState(&occD, &pChamsOccState);

  // Visible
  D3D11_DEPTH_STENCIL_DESC visD{};
  visD.DepthEnable = TRUE;
  visD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  visD.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  pDev->CreateDepthStencilState(&visD, &pChamsVisState);

  // Solid
  D3D11_RASTERIZER_DESC rsD{};
  rsD.FillMode = D3D11_FILL_SOLID; rsD.CullMode = D3D11_CULL_NONE;
  rsD.DepthClipEnable = TRUE;
  pDev->CreateRasterizerState(&rsD, &pChamsRS_solid);
  rsD.FillMode = D3D11_FILL_WIREFRAME;
  pDev->CreateRasterizerState(&rsD, &pChamsRS_wire);

  g_ChamsReady = (pChamsVisPS && pChamsOccPS && pChamsColorBuf);

}

void __stdcall hkDrawIndexedInstanced(
  ID3D11DeviceContext* ctx,
  UINT IndexCountPerInstance,
  UINT InstanceCount,
  UINT StartIndexLocation,
  INT  BaseVertexLocation,
  UINT StartInstanceLocation)
{
  bool localPawnOK = (G.localPawn && G.localPawn > 0x10000);

  // Lazy
  if (localPawnOK && (!g_ChamsReady || !WC::g_Ready)) {
    ID3D11Device* realDev = nullptr;
    ctx->GetDevice(&realDev);
    if (realDev) {
      InitChamsResources(realDev);
      WC::Init(realDev);
      realDev->Release();
    }
  }

  // Stride
  UINT stride=0;
  { UINT off2=0; ID3D11Buffer* vb2=nullptr;
    ctx->IAGetVertexBuffers(0,1,&vb2,&stride,&off2);
    if(vb2) vb2->Release(); }

    if (WC::HandleDrawCall(ctx,IndexCountPerInstance,InstanceCount,
        StartIndexLocation,BaseVertexLocation,StartInstanceLocation,
        stride,(void*)oDrawIndexedInstanced))
    return;

  // Chams
  if (!Cfg::chamsOn || !localPawnOK) {
    oDrawIndexedInstanced(ctx,IndexCountPerInstance,InstanceCount,
                          StartIndexLocation,BaseVertexLocation,StartInstanceLocation);
    return;
  }

  if (!g_ChamsReady) {
    oDrawIndexedInstanced(ctx,IndexCountPerInstance,InstanceCount,
                          StartIndexLocation,BaseVertexLocation,StartInstanceLocation);
    return;
  }

  // Player
  bool isPlayer =
    Cfg::chamsOn && localPawnOK && g_ChamsReady &&
    InstanceCount == 1 &&
    IndexCountPerInstance > 3000 && IndexCountPerInstance < 50000 &&
    stride >= 32 && stride <= 48;

  if (isPlayer) {
    // State
    ID3D11PixelShader* oldPS=nullptr;
    ctx->PSGetShader(&oldPS,nullptr,nullptr);
    ID3D11DepthStencilState* oldDS=nullptr; UINT oldRef=0;
    ctx->OMGetDepthStencilState(&oldDS,&oldRef);
    ID3D11BlendState* oldBlend=nullptr; float oldFactor[4]; UINT oldMask;
    ctx->OMGetBlendState(&oldBlend,oldFactor,&oldMask);
    // SRV
    ID3D11ShaderResourceView* oldSRVs[16]={};
    ctx->PSGetShaderResources(0,16,oldSRVs);

    float bf[4]={0,0,0,0};
    ctx->OMSetBlendState(pChamsBlend,bf,0xffffffff);
    ID3D11ShaderResourceView* nullSRV[16]={};
    ctx->PSSetShaderResources(0,16,nullSRV);

    // Materyal
    ID3D11RasterizerState* oldRS=nullptr; ctx->RSGetState(&oldRS);
    bool isWire = (Cfg::chamsMat == 1);
    ctx->RSSetState(isWire ? pChamsRS_wire : pChamsRS_solid);

    // Pulse
    float pulseA = 1.0f;
    if(Cfg::chamsMat == 2) {
      float t = (float)(GetTickCount64() % 2000) / 2000.f;
      pulseA = 0.35f + 0.65f * fabsf(sinf(t * 3.14159f * 2.f));
    }

    // Rainbow
    float rbCol[3]={Cfg::chamsVis[0],Cfg::chamsVis[1],Cfg::chamsVis[2]};
    if(Cfg::chamsRainbowOn) GetChamsRainbow(rbCol);
    float rbR=rbCol[0], rbG=rbCol[1], rbB=rbCol[2];

    // 1
    if (Cfg::chamsWall) {
      float r=Cfg::chamsRainbowOn?rbR:Cfg::chamsOcc[0];
      float g=Cfg::chamsRainbowOn?rbG:Cfg::chamsOcc[1];
      float b=Cfg::chamsRainbowOn?rbB:Cfg::chamsOcc[2];
      float colOcc[4]={r,g,b,0.65f*pulseA};
      ctx->UpdateSubresource(pChamsColorBuf,0,nullptr,colOcc,0,0);
      ctx->PSSetConstantBuffers(0,1,&pChamsColorBuf);
      ctx->OMSetDepthStencilState(pChamsOccState,0);
      ctx->PSSetShader(pChamsOccPS,nullptr,0);
      oDrawIndexedInstanced(ctx,IndexCountPerInstance,InstanceCount,
                            StartIndexLocation,BaseVertexLocation,StartInstanceLocation);
    }

    // 2
    float colVis[4]={rbR,rbG,rbB,pulseA};
    ctx->UpdateSubresource(pChamsColorBuf,0,nullptr,colVis,0,0);
    ctx->PSSetConstantBuffers(0,1,&pChamsColorBuf);
    ctx->OMSetDepthStencilState(pChamsVisState,0);
    ctx->PSSetShader(pChamsVisPS,nullptr,0);
    oDrawIndexedInstanced(ctx,IndexCountPerInstance,InstanceCount,
                          StartIndexLocation,BaseVertexLocation,StartInstanceLocation);

    // State
    ctx->RSSetState(oldRS);
    ctx->PSSetShader(oldPS,nullptr,0);
    ctx->OMSetDepthStencilState(oldDS,oldRef);
    ctx->OMSetBlendState(oldBlend,oldFactor,oldMask);
    ctx->PSSetShaderResources(0,16,oldSRVs);
    if(oldPS)    oldPS->Release();
    if(oldDS)    oldDS->Release();
    if(oldBlend) oldBlend->Release();
    if(oldRS)    oldRS->Release();
    for(int si=0;si<16;si++) if(oldSRVs[si]) oldSRVs[si]->Release();
    return;
  }

  // Original
  oDrawIndexedInstanced(ctx,IndexCountPerInstance,InstanceCount,
                        StartIndexLocation,BaseVertexLocation,StartInstanceLocation);
}


inline bool SetupHooks(){
  // Window
  WNDCLASSEXA wc{sizeof(wc)};
  wc.lpfnWndProc=DefWindowProcA;
  wc.hInstance=GetModuleHandleA(0);
  wc.lpszClassName="QDummy";
  RegisterClassExA(&wc);
  HWND tmp=CreateWindowExA(0,"QDummy","",WS_POPUP,0,0,8,8,0,0,wc.hInstance,0);
  if(!tmp){
    UnregisterClassA("QDummy",wc.hInstance);
    return false;
  }

  DXGI_SWAP_CHAIN_DESC scd{};
  scd.BufferCount=1;
  scd.BufferDesc.Width=8; scd.BufferDesc.Height=8;
  scd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
  scd.BufferDesc.RefreshRate.Numerator=60;
  scd.BufferDesc.RefreshRate.Denominator=1;
  scd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.OutputWindow=tmp;
  scd.SampleDesc.Count=1;
  scd.Windowed=TRUE;
  scd.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
  scd.Flags=DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  IDXGISwapChain* sc2=nullptr; ID3D11Device* dev=nullptr;
  ID3D11DeviceContext* devCtx=nullptr; D3D_FEATURE_LEVEL fl;

  // Hardware
  HRESULT hr=D3D11CreateDeviceAndSwapChain(
    nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
    nullptr, 0, D3D11_SDK_VERSION,
    &scd, &sc2, &dev, &fl, &devCtx);
  if(FAILED(hr)){
    hr=D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0,
      nullptr, 0, D3D11_SDK_VERSION,
      &scd, &sc2, &dev, &fl, &devCtx);
  }

  DestroyWindow(tmp);
  UnregisterClassA("QDummy", wc.hInstance);

  if(FAILED(hr) || !sc2 || !dev || !devCtx){
    if(sc2)   sc2->Release();
    if(dev)   dev->Release();
    if(devCtx) devCtx->Release();
    return false;
  }

  // vtable
  // IDXGISwapChain
  void** scVtbl  = *(void***)sc2;
  void** ctxVtbl = *(void***)devCtx;
  void* fnP  = scVtbl[8];   // Present
  void* fnR  = scVtbl[13];  // ResizeBuffers
  void* fnDII= ctxVtbl[20]; // DrawIndexedInstanced


  sc2->Release(); dev->Release(); devCtx->Release();

  // Sanity
  HMODULE hDXGI=GetModuleHandleA("dxgi.dll");
  HMODULE hD3D =GetModuleHandleA("d3d11.dll");
  if(!hDXGI || !hD3D){
    return false;
  }

  MH_Initialize();
  MH_STATUS stP = MH_CreateHook(fnP,  (void*)hkPresent,(void**)&oPresent);
  MH_STATUS stR = MH_CreateHook(fnR,  (void*)hkResize, (void**)&oResize);
  MH_STATUS stD = MH_CreateHook(fnDII,(void*)hkDrawIndexedInstanced,(void**)&oDrawIndexedInstanced);


  if(stP != MH_OK){
    MH_Uninitialize();
    return false;
  }

  MH_EnableHook(MH_ALL_HOOKS);
  return true;
}


inline void RemoveHooks(){
  MH_DisableHook(MH_ALL_HOOKS);MH_Uninitialize();
  ImGuiShutdown();
  if(g_WndProcOld&&g_Wnd)SetWindowLongPtrW(g_Wnd,GWLP_WNDPROC,(LONG_PTR)g_WndProcOld);
}
