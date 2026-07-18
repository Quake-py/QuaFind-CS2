#pragma once
// WeaponChams.h
// Ekipman
// Gecis-1
// Gecis-2
#include <d3d11.h>
#include <cmath>
#include <cstring>

namespace WC {

// Materyal
enum class EMat { Solid=0, Wire=1, Pulse=2, Bloom=3 };

// Rainbow
struct RainbowCfg {
    bool  on            = false;
    float speed         = 0.2f;  // hue
    bool  customPalette = false; // false
    int   colorCount    = 3;     // zel
    float colors[4][3]  = {      // zel
        {1.f, 0.f, 0.f},         // K
        {0.f, 0.5f, 1.f},        // Mavi
        {0.f, 1.f, 0.3f},        // Ye
        {1.f, 0.8f, 0.f}         // Sar
    };
};

// HSV
static void HSVtoRGB(float h, float out[3]) {
    h = h - floorf(h); // 0
    float s=1.f, v=1.f;
    int   i = (int)(h * 6.f);
    float f = h * 6.f - i;
    float p = v*(1.f-s), q=v*(1.f-f*s), t2=v*(1.f-(1.f-f)*s);
    switch(i%6){
        case 0:out[0]=v; out[1]=t2;out[2]=p; break;
        case 1:out[0]=q; out[1]=v; out[2]=p; break;
        case 2:out[0]=p; out[1]=v; out[2]=t2;break;
        case 3:out[0]=p; out[1]=q; out[2]=v; break;
        case 4:out[0]=t2;out[1]=p; out[2]=v; break;
        default:out[0]=v;out[1]=p; out[2]=q; break;
    }
}

// Rainbow
// Oto
// zel
inline void GetRainbowColor(const RainbowCfg& cfg, float out[3]) {
    float spd = cfg.speed < 0.01f ? 0.01f : cfg.speed;
    double t  = (double)GetTickCount64() / 1000.0;
    if(!cfg.customPalette) {
        // Oto
        float hue = fmodf((float)(t * spd), 1.0f);
        HSVtoRGB(hue, out);
    } else {
        // zel
        int n = cfg.colorCount < 2 ? 2 : (cfg.colorCount > 4 ? 4 : cfg.colorCount);
        // Her
        float cycleLen = 1.0f / spd;  // bir
        float pos = fmodf((float)t / cycleLen, 1.0f) * (float)n; // 0
        int   idx0 = (int)pos % n;
        int   idx1 = (idx0 + 1) % n;
        float frac = pos - floorf(pos);
        // Smoothstep
        frac = frac * frac * (3.f - 2.f * frac);
        out[0] = cfg.colors[idx0][0] + (cfg.colors[idx1][0]-cfg.colors[idx0][0])*frac;
        out[1] = cfg.colors[idx0][1] + (cfg.colors[idx1][1]-cfg.colors[idx0][1])*frac;
        out[2] = cfg.colors[idx0][2] + (cfg.colors[idx1][2]-cfg.colors[idx0][2])*frac;
    }
}

// Konfig
inline bool  g_On         = false;
// Silah
inline bool  g_WeapOn     = false;
inline float g_WeapCol[3] = {0.9f, 0.5f, 0.0f};
inline float g_WeapAlpha  = 0.85f;
inline EMat  g_WeapMat    = EMat::Solid;
inline RainbowCfg g_WeapRainbow;
// B
inline bool  g_KnifeOn    = false;
inline float g_KnifeCol[3]= {0.2f, 0.8f, 1.0f};
inline float g_KnifeAlpha = 0.85f;
inline EMat  g_KnifeMat   = EMat::Solid;
inline RainbowCfg g_KnifeRainbow;
// Eldiven
inline bool  g_GloveOn    = false;
inline float g_GloveCol[3]= {0.6f, 0.2f, 1.0f};
inline float g_GloveAlpha = 0.85f;
inline EMat  g_GloveMat   = EMat::Solid;
inline RainbowCfg g_GloveRainbow;

// Filtre
static constexpr int kWeapIdxMin  = 36635, kWeapIdxMax  = 500000;
static constexpr int kWeapStrMin  = 25,    kWeapStrMax  = 96;
static constexpr int kKnifeIdxMin = 3000,  kKnifeIdxMax = 30000;
static constexpr int kKnifeStrMin = 24,    kKnifeStrMax = 24;
static constexpr int kGloveIdxMin = 23908, kGloveIdxMax = 38715;
static constexpr int kGloveStrMin = 27,    kGloveStrMax = 28;

// D3D
inline ID3D11PixelShader*       g_PS        = nullptr;
inline ID3D11Buffer*            g_ColBuf    = nullptr;
inline ID3D11DepthStencilState* g_DSS_equal = nullptr;
inline ID3D11BlendState*        g_BS_factor = nullptr; // BLEND_FACTOR
inline ID3D11BlendState*        g_BS_add    = nullptr; // Additive
inline ID3D11RasterizerState*   g_RS_solid  = nullptr;
inline ID3D11RasterizerState*   g_RS_wire   = nullptr;
inline bool g_Ready = false;

static const char kPSSrc[] = R"(
cbuffer WCCol : register(b0) { float4 col; }
float4 main() : SV_Target { return col; }
)";

inline void Init(ID3D11Device* dev) {
    if (g_Ready || !dev) return;
    typedef HRESULT(__stdcall*PFN)(LPCVOID,SIZE_T,LPCSTR,void*,void*,LPCSTR,LPCSTR,UINT,UINT,ID3DBlob**,ID3DBlob**);
    HMODULE m = LoadLibraryA("d3dcompiler_47.dll");
    if(!m) m = LoadLibraryA("d3dcompiler_43.dll"); if(!m) return;
    auto D3DC = (PFN)GetProcAddress(m,"D3DCompile"); if(!D3DC) return;

    ID3DBlob *pb=nullptr, *e=nullptr;
    if(FAILED(D3DC(kPSSrc,strlen(kPSSrc),nullptr,nullptr,nullptr,"main","ps_4_0",0,0,&pb,&e))||!pb){
        if(e)e->Release(); return; }
    if(e)e->Release();
    dev->CreatePixelShader(pb->GetBufferPointer(),pb->GetBufferSize(),nullptr,&g_PS);
    pb->Release();

    D3D11_BUFFER_DESC bd={16,D3D11_USAGE_DEFAULT,D3D11_BIND_CONSTANT_BUFFER,0,0,0};
    dev->CreateBuffer(&bd,nullptr,&g_ColBuf);

    // EQUAL
    // El
    D3D11_DEPTH_STENCIL_DESC dsd={};
    dsd.DepthEnable    = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsd.DepthFunc      = D3D11_COMPARISON_LESS_EQUAL; // LEQUAL
    dsd.StencilEnable  = FALSE;
    dev->CreateDepthStencilState(&dsd,&g_DSS_equal);

    D3D11_BLEND_DESC bld={}; auto& r=bld.RenderTarget[0];
    // BLEND_FACTOR
    // Kullanici
    r.BlendEnable=TRUE; r.SrcBlend=D3D11_BLEND_BLEND_FACTOR;
    r.DestBlend=D3D11_BLEND_INV_BLEND_FACTOR; r.BlendOp=D3D11_BLEND_OP_ADD;
    r.SrcBlendAlpha=D3D11_BLEND_ONE; r.DestBlendAlpha=D3D11_BLEND_ZERO;
    r.BlendOpAlpha=D3D11_BLEND_OP_ADD; r.RenderTargetWriteMask=0xF;
    dev->CreateBlendState(&bld,&g_BS_factor);

    // Additive
    D3D11_BLEND_DESC abd={}; auto& ar=abd.RenderTarget[0];
    ar.BlendEnable=TRUE; ar.SrcBlend=D3D11_BLEND_ONE;
    ar.DestBlend=D3D11_BLEND_ONE; ar.BlendOp=D3D11_BLEND_OP_ADD;
    ar.SrcBlendAlpha=D3D11_BLEND_ONE; ar.DestBlendAlpha=D3D11_BLEND_ZERO;
    ar.BlendOpAlpha=D3D11_BLEND_OP_ADD; ar.RenderTargetWriteMask=0xF;
    dev->CreateBlendState(&abd,&g_BS_add);

    // Solid
    D3D11_RASTERIZER_DESC rd={};
    rd.FillMode=D3D11_FILL_SOLID; rd.CullMode=D3D11_CULL_NONE;
    rd.DepthClipEnable=TRUE;
    dev->CreateRasterizerState(&rd,&g_RS_solid);

    // Wireframe
    rd.FillMode=D3D11_FILL_WIREFRAME;
    dev->CreateRasterizerState(&rd,&g_RS_wire);

    g_Ready = (g_PS && g_ColBuf && g_DSS_equal && g_BS_factor && g_BS_add && g_RS_solid && g_RS_wire);
}

inline void Release() {
    if(g_PS)       {g_PS->Release();       g_PS=nullptr;}
    if(g_ColBuf)   {g_ColBuf->Release();   g_ColBuf=nullptr;}
    if(g_DSS_equal){g_DSS_equal->Release();g_DSS_equal=nullptr;}
    if(g_BS_factor){g_BS_factor->Release();g_BS_factor=nullptr;}
    if(g_BS_add)   {g_BS_add->Release();   g_BS_add=nullptr;}
    if(g_RS_solid) {g_RS_solid->Release(); g_RS_solid=nullptr;}
    if(g_RS_wire)  {g_RS_wire->Release();  g_RS_wire=nullptr;}
    g_Ready=false;
}

enum class ECat { None, Weapon, Knife, Glove };

static ECat Categorize(UINT idx, UINT stride) {
    if(!g_On) return ECat::None;
    if(g_WeapOn  && (int)idx>=kWeapIdxMin  && (int)idx<=kWeapIdxMax  &&
       (int)stride>=kWeapStrMin  && (int)stride<=kWeapStrMax) return ECat::Weapon;
    if(g_KnifeOn && (int)idx>=kKnifeIdxMin && (int)idx<=kKnifeIdxMax &&
       (int)stride>=kKnifeStrMin && (int)stride<=kKnifeStrMax) return ECat::Knife;
    if(g_GloveOn && (int)idx>=kGloveIdxMin && (int)idx<=kGloveIdxMax &&
       (int)stride>=kGloveStrMin && (int)stride<=kGloveStrMax) return ECat::Glove;
    return ECat::None;
}

// GetColor

// DII
inline bool HandleDrawCall(
    ID3D11DeviceContext* ctx,
    UINT idxCnt, UINT instCnt, UINT startIdx, INT baseVtx, UINT startInst,
    UINT stride, void* oDraw)
{
    typedef void(__stdcall*PFNDII)(ID3D11DeviceContext*,UINT,UINT,UINT,INT,UINT);
    if(!g_Ready) return false;
    if(instCnt != 1) return false;

    ECat cat = Categorize(idxCnt, stride);
    if(cat == ECat::None) return false;

    EMat mat = (cat==ECat::Weapon)?g_WeapMat:(cat==ECat::Knife)?g_KnifeMat:g_GloveMat;

    // Renk
    const float* c    = (cat==ECat::Weapon)?g_WeapCol:(cat==ECat::Knife)?g_KnifeCol:g_GloveCol;
    float alpha       = (cat==ECat::Weapon)?g_WeapAlpha:(cat==ECat::Knife)?g_KnifeAlpha:g_GloveAlpha;
    const RainbowCfg& rb = (cat==ECat::Weapon)?g_WeapRainbow:(cat==ECat::Knife)?g_KnifeRainbow:g_GloveRainbow;
    float rbCol[3];
    if(rb.on) { GetRainbowColor(rb, rbCol); c = rbCol; }
    if(mat==EMat::Pulse){
        float pt=(float)GetTickCount64()*0.002f;
        alpha*=(0.5f+0.5f*sinf(pt));
    }
    float col[4]={c[0],c[1],c[2],1.0f};
    ctx->UpdateSubresource(g_ColBuf,0,nullptr,col,0,0);
    float bf[4]={alpha,alpha,alpha,alpha};

    // State
    ID3D11PixelShader*       oldPS=nullptr; ctx->PSGetShader(&oldPS,nullptr,nullptr);
    ID3D11ShaderResourceView* oldSRV[16]={}; ctx->PSGetShaderResources(0,16,oldSRV);
    ID3D11Buffer*             oldCB=nullptr; ctx->PSGetConstantBuffers(0,1,&oldCB);
    ID3D11DepthStencilState*  oldDS=nullptr; UINT oldRef=0;
    ctx->OMGetDepthStencilState(&oldDS,&oldRef);
    ID3D11BlendState*         oldBS=nullptr; float oldBF[4]; UINT oldBM;
    ctx->OMGetBlendState(&oldBS,oldBF,&oldBM);
    ID3D11RasterizerState*    oldRS=nullptr; ctx->RSGetState(&oldRS);

    // TEK
    // Orijinal
    ctx->OMSetBlendState(g_BS_factor,bf,0xFFFFFFFF);
    ctx->OMSetDepthStencilState(g_DSS_equal,0); // LESS_EQUAL
    ctx->PSSetShader(g_PS,nullptr,0);
    ctx->PSSetConstantBuffers(0,1,&g_ColBuf);
    ID3D11ShaderResourceView* nullSRV[16]={};
    ctx->PSSetShaderResources(0,16,nullSRV);
    ctx->RSSetState(mat==EMat::Wire ? g_RS_wire : g_RS_solid);
    ((PFNDII)oDraw)(ctx,idxCnt,instCnt,startIdx,baseVtx,startInst);

    // BLOOM
    if(mat == EMat::Bloom) {
        ctx->OMSetBlendState(g_BS_add,bf,0xFFFFFFFF);
        static const float kBrightness[3]={0.45f, 0.25f, 0.10f};
        for(int gi=0; gi<3; gi++){
            float glow[4]={c[0]*kBrightness[gi], c[1]*kBrightness[gi],
                           c[2]*kBrightness[gi], 1.0f};
            ctx->UpdateSubresource(g_ColBuf,0,nullptr,glow,0,0);
            ((PFNDII)oDraw)(ctx,idxCnt,instCnt,startIdx,baseVtx,startInst);
        }
        ctx->UpdateSubresource(g_ColBuf,0,nullptr,col,0,0);
    }

    // State
    ctx->PSSetShader(oldPS,nullptr,0);
    ctx->PSSetShaderResources(0,16,oldSRV);
    ctx->PSSetConstantBuffers(0,1,&oldCB);
    ctx->OMSetDepthStencilState(oldDS,oldRef);
    ctx->OMSetBlendState(oldBS,oldBF,oldBM);
    ctx->RSSetState(oldRS);
    if(oldPS) oldPS->Release();
    if(oldCB) oldCB->Release();
    if(oldDS) oldDS->Release();
    if(oldBS) oldBS->Release();
    if(oldRS) oldRS->Release();
    for(int i=0;i<16;i++) if(oldSRV[i]) oldSRV[i]->Release();
    return true;
}

} // namespace
