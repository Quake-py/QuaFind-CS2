// dear
// headers

// Help
// -
// -
// -
// Read

// Resources
// -
// -
// -
// -
// -
// -
// -
// -
// -

// Getting
// -
// -
// please

// Library
// Integer
#define IMGUI_VERSION       "1.89.9"
#define IMGUI_VERSION_NUM   18990
#define IMGUI_HAS_TABLE

/*

Index of this file:
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION
// SECTION

*/

#pragma once

// Configuration
// edit
#ifdef IMGUI_USER_CONFIG
#include IMGUI_USER_CONFIG
#endif
#include "imconfig.h"

#ifndef IMGUI_DISABLE

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Includes
#include <float.h>                  // FLT_MIN
#include <stdarg.h>                 // va_list
#include <stddef.h>                 // ptrdiff_t
#include <string.h>                 // memset

// Define
// IMGUI_API
// Using
#ifndef IMGUI_API
#define IMGUI_API
#endif
#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API              IMGUI_API
#endif

// Helper
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)            assert(_EXPR)                               // You
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size
#define IM_UNUSED(_VAR)             ((void)(_VAR))                              // Used
#define IM_OFFSETOF(_TYPE,_MEMBER)  offsetof(_TYPE, _MEMBER)                    // Offset
#define IMGUI_CHECKVERSION()        ImGui::DebugCheckVersionAndDataLayout(IMGUI_VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), sizeof(ImDrawVert), sizeof(ImDrawIdx))

// Helper
#if !defined(IMGUI_USE_STB_SPRINTF) && defined(__MINGW32__) && !defined(__clang__)
#define IM_FMTARGS(FMT)             __attribute__((format(gnu_printf, FMT, FMT+1)))
#define IM_FMTLIST(FMT)             __attribute__((format(gnu_printf, FMT, 0)))
#elif !defined(IMGUI_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__))
#define IM_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1)))
#define IM_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif

// Disable
#if defined(_MSC_VER) && !defined(__clang__)  && !defined(__INTEL_COMPILER) && !defined(IMGUI_DEBUG_PARANOID)
#define IM_MSVC_RUNTIME_CHECKS_OFF      __pragma(runtime_checks("",off))     __pragma(check_stack(off)) __pragma(strict_gs_check(push,off))
#define IM_MSVC_RUNTIME_CHECKS_RESTORE  __pragma(runtime_checks("",restore)) __pragma(check_stack())    __pragma(strict_gs_check(pop))
#else
#define IM_MSVC_RUNTIME_CHECKS_OFF
#define IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif

// Warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 26495)    // Static
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wreserved-identifier"            // warning
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"          // warning
#pragma GCC diagnostic ignored "-Wclass-memaccess"  // __GNUC__
#endif

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Forward
struct ImDrawChannel;               // Temporary
struct ImDrawCmd;                   // A
struct ImDrawData;                  // All
struct ImDrawList;                  // A
struct ImDrawListSharedData;        // Data
struct ImDrawListSplitter;          // Helper
struct ImDrawVert;                  // A
struct ImFont;                      // Runtime
struct ImFontAtlas;                 // Runtime
struct ImFontBuilderIO;             // Opaque
struct ImFontConfig;                // Configuration
struct ImFontGlyph;                 // A
struct ImFontGlyphRangesBuilder;    // Helper
struct ImColor;                     // Helper
struct ImGuiContext;                // Dear
struct ImGuiIO;                     // Main
struct ImGuiInputTextCallbackData;  // Shared
struct ImGuiKeyData;                // Storage
struct ImGuiListClipper;            // Helper
struct ImGuiOnceUponAFrame;         // Helper
struct ImGuiPayload;                // User
struct ImGuiPlatformImeData;        // Platform
struct ImGuiSizeCallbackData;       // Callback
struct ImGuiStorage;                // Helper
struct ImGuiStyle;                  // Runtime
struct ImGuiTableSortSpecs;         // Sorting
struct ImGuiTableColumnSortSpecs;   // Sorting
struct ImGuiTextBuffer;             // Helper
struct ImGuiTextFilter;             // Helper
struct ImGuiViewport;               // A

// Enumerations
// -
// -
// In
// With
enum ImGuiKey : int;                // -
enum ImGuiMouseSource : int;        // -
typedef int ImGuiCol;               // -
typedef int ImGuiCond;              // -
typedef int ImGuiDataType;          // -
typedef int ImGuiDir;               // -
typedef int ImGuiMouseButton;       // -
typedef int ImGuiMouseCursor;       // -
typedef int ImGuiSortDirection;     // -
typedef int ImGuiStyleVar;          // -
typedef int ImGuiTableBgTarget;     // -

// Flags
// -
// In
// With
typedef int ImDrawFlags;            // -
typedef int ImDrawListFlags;        // -
typedef int ImFontAtlasFlags;       // -
typedef int ImGuiBackendFlags;      // -
typedef int ImGuiButtonFlags;       // -
typedef int ImGuiColorEditFlags;    // -
typedef int ImGuiConfigFlags;       // -
typedef int ImGuiComboFlags;        // -
typedef int ImGuiDragDropFlags;     // -
typedef int ImGuiFocusedFlags;      // -
typedef int ImGuiHoveredFlags;      // -
typedef int ImGuiInputTextFlags;    // -
typedef int ImGuiKeyChord;          // -
typedef int ImGuiPopupFlags;        // -
typedef int ImGuiSelectableFlags;   // -
typedef int ImGuiSliderFlags;       // -
typedef int ImGuiTabBarFlags;       // -
typedef int ImGuiTabItemFlags;      // -
typedef int ImGuiTableFlags;        // -
typedef int ImGuiTableColumnFlags;  // -
typedef int ImGuiTableRowFlags;     // -
typedef int ImGuiTreeNodeFlags;     // -
typedef int ImGuiViewportFlags;     // -
typedef int ImGuiWindowFlags;       // -

// ImTexture
// -
// -
#ifndef ImTextureID
typedef void* ImTextureID;          // Default
#endif

// ImDrawIdx
// -
// -
#ifndef ImDrawIdx
typedef unsigned short ImDrawIdx;   // Default
#endif

// Scalar
typedef unsigned int        ImGuiID;// A
typedef signed char         ImS8;   // 8-bit
typedef unsigned char       ImU8;   // 8-bit
typedef signed short        ImS16;  // 16-bit
typedef unsigned short      ImU16;  // 16-bit
typedef signed int          ImS32;  // 32-bit
typedef unsigned int        ImU32;  // 32-bit
typedef signed   long long  ImS64;  // 64-bit
typedef unsigned long long  ImU64;  // 64-bit

// Character
// we
typedef unsigned short ImWchar16;   // A
typedef unsigned int ImWchar32;     // A
#ifdef IMGUI_USE_WCHAR32            // ImWchar
typedef ImWchar32 ImWchar;
#else
typedef ImWchar16 ImWchar;
#endif

// Callback
typedef int     (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData* data);    // Callback
typedef void    (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);              // Callback
typedef void*   (*ImGuiMemAllocFunc)(size_t sz, void* user_data);               // Function
typedef void    (*ImGuiMemFreeFunc)(void* ptr, void* user_data);                // Function

// ImVec2
// This
IM_MSVC_RUNTIME_CHECKS_OFF
struct ImVec2
{
    float                                   x, y;
    constexpr ImVec2()                      : x(0.0f), y(0.0f) { }
    constexpr ImVec2(float _x, float _y)    : x(_x), y(_y) { }
    float& operator[] (size_t idx)          { IM_ASSERT(idx == 0 || idx == 1); return ((float*)(void*)(char*)this)[idx]; } // We
    float  operator[] (size_t idx) const    { IM_ASSERT(idx == 0 || idx == 1); return ((const float*)(const void*)(const char*)this)[idx]; }
#ifdef IM_VEC2_CLASS_EXTRA
    IM_VEC2_CLASS_EXTRA     // Define
#endif
};

// ImVec4
struct ImVec4
{
    float                                                     x, y, z, w;
    constexpr ImVec4()                                        : x(0.0f), y(0.0f), z(0.0f), w(0.0f) { }
    constexpr ImVec4(float _x, float _y, float _z, float _w)  : x(_x), y(_y), z(_z), w(_w) { }
#ifdef IM_VEC4_CLASS_EXTRA
    IM_VEC4_CLASS_EXTRA     // Define
#endif
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

// -----------------------------------------------------------------------------
// SECTION
// Note
// -----------------------------------------------------------------------------

namespace ImGui
{
    // Context
    // -
    // -
    // for
    IMGUI_API ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL);
    IMGUI_API void          DestroyContext(ImGuiContext* ctx = NULL);   // NULL
    IMGUI_API ImGuiContext* GetCurrentContext();
    IMGUI_API void          SetCurrentContext(ImGuiContext* ctx);

    // Main
    IMGUI_API ImGuiIO&      GetIO();                                    // access
    IMGUI_API ImGuiStyle&   GetStyle();                                 // access
    IMGUI_API void          NewFrame();                                 // start
    IMGUI_API void          EndFrame();                                 // ends
    IMGUI_API void          Render();                                   // ends
    IMGUI_API ImDrawData*   GetDrawData();                              // valid

    // Demo
    IMGUI_API void          ShowDemoWindow(bool* p_open = NULL);        // create
    IMGUI_API void          ShowMetricsWindow(bool* p_open = NULL);     // create
    IMGUI_API void          ShowDebugLogWindow(bool* p_open = NULL);    // create
    IMGUI_API void          ShowStackToolWindow(bool* p_open = NULL);   // create
    IMGUI_API void          ShowAboutWindow(bool* p_open = NULL);       // create
    IMGUI_API void          ShowStyleEditor(ImGuiStyle* ref = NULL);    // add
    IMGUI_API bool          ShowStyleSelector(const char* label);       // add
    IMGUI_API void          ShowFontSelector(const char* label);        // add
    IMGUI_API void          ShowUserGuide();                            // add
    IMGUI_API const char*   GetVersion();                               // get

    // Styles
    IMGUI_API void          StyleColorsDark(ImGuiStyle* dst = NULL);    // new
    IMGUI_API void          StyleColorsLight(ImGuiStyle* dst = NULL);   // best
    IMGUI_API void          StyleColorsClassic(ImGuiStyle* dst = NULL); // classic

    // Windows
    // -
    // -
    // which
    // -
    // Some
    // -
    // anything
    // Important
    // BeginPopup
    // returned
    // -
    IMGUI_API bool          Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
    IMGUI_API void          End();

    // Child
    // -
    // -
    // -
    // Always
    // Important
    // BeginPopup
    // returned
    IMGUI_API bool          BeginChild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API bool          BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    IMGUI_API void          EndChild();

    // Windows
    // -
    IMGUI_API bool          IsWindowAppearing();
    IMGUI_API bool          IsWindowCollapsed();
    IMGUI_API bool          IsWindowFocused(ImGuiFocusedFlags flags=0); // is
    IMGUI_API bool          IsWindowHovered(ImGuiHoveredFlags flags=0); // is
    IMGUI_API ImDrawList*   GetWindowDrawList();                        // get
    IMGUI_API ImVec2        GetWindowPos();                             // get
    IMGUI_API ImVec2        GetWindowSize();                            // get
    IMGUI_API float         GetWindowWidth();                           // get
    IMGUI_API float         GetWindowHeight();                          // get

    // Window
    // -
    IMGUI_API void          SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0)); // set
    IMGUI_API void          SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);                  // set
    IMGUI_API void          SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL); // set
    IMGUI_API void          SetNextWindowContentSize(const ImVec2& size);                               // set
    IMGUI_API void          SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                 // set
    IMGUI_API void          SetNextWindowFocus();                                                       // set
    IMGUI_API void          SetNextWindowScroll(const ImVec2& scroll);                                  // set
    IMGUI_API void          SetNextWindowBgAlpha(float alpha);                                          // set
    IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);                        // not
    IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiCond cond = 0);                      // not
    IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0);                     // not
    IMGUI_API void          SetWindowFocus();                                                           // not
    IMGUI_API void          SetWindowFontScale(float scale);                                            // OBSOLETE
    IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond = 0);      // set
    IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond = 0);    // set
    IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0);   // set
    IMGUI_API void          SetWindowFocus(const char* name);                                           // set

    // Content
    // -
    // -
    IMGUI_API ImVec2        GetContentRegionAvail();                                        // GetContentRegionMax
    IMGUI_API ImVec2        GetContentRegionMax();                                          // current
    IMGUI_API ImVec2        GetWindowContentRegionMin();                                    // content
    IMGUI_API ImVec2        GetWindowContentRegionMax();                                    // content

    // Windows
    // -
    // -
    IMGUI_API float         GetScrollX();                                                   // get
    IMGUI_API float         GetScrollY();                                                   // get
    IMGUI_API void          SetScrollX(float scroll_x);                                     // set
    IMGUI_API void          SetScrollY(float scroll_y);                                     // set
    IMGUI_API float         GetScrollMaxX();                                                // get
    IMGUI_API float         GetScrollMaxY();                                                // get
    IMGUI_API void          SetScrollHereX(float center_x_ratio = 0.5f);                    // adjust
    IMGUI_API void          SetScrollHereY(float center_y_ratio = 0.5f);                    // adjust
    IMGUI_API void          SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f);  // adjust
    IMGUI_API void          SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);  // adjust

    // Parameters
    IMGUI_API void          PushFont(ImFont* font);                                         // use
    IMGUI_API void          PopFont();
    IMGUI_API void          PushStyleColor(ImGuiCol idx, ImU32 col);                        // modify
    IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);
    IMGUI_API void          PopStyleColor(int count = 1);
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, float val);                     // modify
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);             // modify
    IMGUI_API void          PopStyleVar(int count = 1);
    IMGUI_API void          PushTabStop(bool tab_stop);                                     // tab
    IMGUI_API void          PopTabStop();
    IMGUI_API void          PushButtonRepeat(bool repeat);                                  // in
    IMGUI_API void          PopButtonRepeat();

    // Parameters
    IMGUI_API void          PushItemWidth(float item_width);                                // push
    IMGUI_API void          PopItemWidth();
    IMGUI_API void          SetNextItemWidth(float item_width);                             // set
    IMGUI_API float         CalcItemWidth();                                                // width
    IMGUI_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // push
    IMGUI_API void          PopTextWrapPos();

    // Style
    // -
    IMGUI_API ImFont*       GetFont();                                                      // get
    IMGUI_API float         GetFontSize();                                                  // get
    IMGUI_API ImVec2        GetFontTexUvWhitePixel();                                       // get
    IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);              // retrieve
    IMGUI_API ImU32         GetColorU32(const ImVec4& col);                                 // retrieve
    IMGUI_API ImU32         GetColorU32(ImU32 col);                                         // retrieve
    IMGUI_API const ImVec4& GetStyleColorVec4(ImGuiCol idx);                                // retrieve

    // Cursor
    // -
    // -
    // -
    // -
    // Window-local
    // Absolute
    IMGUI_API void          Separator();                                                    // separator
    IMGUI_API void          SameLine(float offset_from_start_x=0.0f, float spacing=-1.0f);  // call
    IMGUI_API void          NewLine();                                                      // undo
    IMGUI_API void          Spacing();                                                      // add
    IMGUI_API void          Dummy(const ImVec2& size);                                      // add
    IMGUI_API void          Indent(float indent_w = 0.0f);                                  // move
    IMGUI_API void          Unindent(float indent_w = 0.0f);                                // move
    IMGUI_API void          BeginGroup();                                                   // lock
    IMGUI_API void          EndGroup();                                                     // unlock
    IMGUI_API ImVec2        GetCursorPos();                                                 // cursor
    IMGUI_API float         GetCursorPosX();                                                // some
    IMGUI_API float         GetCursorPosY();                                                // other
    IMGUI_API void          SetCursorPos(const ImVec2& local_pos);                          // are
    IMGUI_API void          SetCursorPosX(float local_x);                                   // GetWindowPos
    IMGUI_API void          SetCursorPosY(float local_y);                                   //
    IMGUI_API ImVec2        GetCursorStartPos();                                            // initial
    IMGUI_API ImVec2        GetCursorScreenPos();                                           // cursor
    IMGUI_API void          SetCursorScreenPos(const ImVec2& pos);                          // cursor
    IMGUI_API void          AlignTextToFramePadding();                                      // vertically
    IMGUI_API float         GetTextLineHeight();                                            // FontSize
    IMGUI_API float         GetTextLineHeightWithSpacing();                                 // FontSize
    IMGUI_API float         GetFrameHeight();                                               // FontSize
    IMGUI_API float         GetFrameHeightWithSpacing();                                    // FontSize

    // ID
    // Read
    // -
    // -
    // -
    // -
    // -
    // want
    // -
    // -
    // whereas
    IMGUI_API void          PushID(const char* str_id);                                     // push
    IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push
    IMGUI_API void          PushID(const void* ptr_id);                                     // push
    IMGUI_API void          PushID(int int_id);                                             // push
    IMGUI_API void          PopID();                                                        // pop
    IMGUI_API ImGuiID       GetID(const char* str_id);                                      // calculate
    IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_end);
    IMGUI_API ImGuiID       GetID(const void* ptr_id);

    // Widgets
    IMGUI_API void          TextUnformatted(const char* text, const char* text_end = NULL); // raw
    IMGUI_API void          Text(const char* fmt, ...)                                      IM_FMTARGS(1); // formatted
    IMGUI_API void          TextV(const char* fmt, va_list args)                            IM_FMTLIST(1);
    IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...)            IM_FMTARGS(2); // shortcut
    IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_list args)  IM_FMTLIST(2);
    IMGUI_API void          TextDisabled(const char* fmt, ...)                              IM_FMTARGS(1); // shortcut
    IMGUI_API void          TextDisabledV(const char* fmt, va_list args)                    IM_FMTLIST(1);
    IMGUI_API void          TextWrapped(const char* fmt, ...)                               IM_FMTARGS(1); // shortcut
    IMGUI_API void          TextWrappedV(const char* fmt, va_list args)                     IM_FMTLIST(1);
    IMGUI_API void          LabelText(const char* label, const char* fmt, ...)              IM_FMTARGS(2); // display
    IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_list args)    IM_FMTLIST(2);
    IMGUI_API void          BulletText(const char* fmt, ...)                                IM_FMTARGS(1); // shortcut
    IMGUI_API void          BulletTextV(const char* fmt, va_list args)                      IM_FMTLIST(1);
    IMGUI_API void          SeparatorText(const char* label);                               // currently

    // Widgets
    // -
    // -
    IMGUI_API bool          Button(const char* label, const ImVec2& size = ImVec2(0, 0));   // button
    IMGUI_API bool          SmallButton(const char* label);                                 // button
    IMGUI_API bool          InvisibleButton(const char* str_id, const ImVec2& size, ImGuiButtonFlags flags = 0); // flexible
    IMGUI_API bool          ArrowButton(const char* str_id, ImGuiDir dir);                  // square
    IMGUI_API bool          Checkbox(const char* label, bool* v);
    IMGUI_API bool          CheckboxFlags(const char* label, int* flags, int flags_value);
    IMGUI_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
    IMGUI_API bool          RadioButton(const char* label, bool active);                    // use
    IMGUI_API bool          RadioButton(const char* label, int* v, int v_button);           // shortcut
    IMGUI_API void          ProgressBar(float fraction, const ImVec2& size_arg = ImVec2(-FLT_MIN, 0), const char* overlay = NULL);
    IMGUI_API void          Bullet();                                                       // draw

    // Widgets
    // -
    IMGUI_API void          Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
    IMGUI_API bool          ImageButton(const char* str_id, ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

    // Widgets
    // -
    // -
    IMGUI_API bool          BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    IMGUI_API void          EndCombo(); // only
    IMGUI_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate
    IMGUI_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

    // Widgets
    // -
    // -
    // the
    // -
    // -
    // -
    // -
    // -
    // -
    // -
    // If
    IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);     // If
    IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);  // If
    IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);

    // Widgets
    // -
    // -
    // -
    // -
    // If
    IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);     // adjust
    IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          VSliderScalar(const char* label, const ImVec2& size, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format = NULL, ImGuiSliderFlags flags = 0);

    // Widgets
    // -
    // -
    IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputScalarN(const char* label, ImGuiDataType data_type, void* p_data, int components, const void* p_step = NULL, const void* p_step_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);

    // Widgets
    // -
    // -
    IMGUI_API bool          ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
    IMGUI_API bool          ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // display
    IMGUI_API void          SetColorEditOptions(ImGuiColorEditFlags flags);                     // initialize

    // Widgets
    // -
    IMGUI_API bool          TreeNode(const char* label);
    IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   // helper
    IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   //
    IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
    IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API void          TreePush(const char* str_id);                                       // Indent
    IMGUI_API void          TreePush(const void* ptr_id);                                       //
    IMGUI_API void          TreePop();                                                          // Unindent
    IMGUI_API float         GetTreeNodeToLabelSpacing();                                        // horizontal
    IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);  // if
    IMGUI_API bool          CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0); // when
    IMGUI_API void          SetNextItemOpen(bool is_open, ImGuiCond cond = 0);                  // set

    // Widgets
    // -
    // -
    IMGUI_API bool          Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // bool
    IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));      // bool

    // Widgets
    // -
    // -
    // -
    // -
    // -
    IMGUI_API bool          BeginListBox(const char* label, const ImVec2& size = ImVec2(0, 0)); // open
    IMGUI_API void          EndListBox();                                                       // only
    IMGUI_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
    IMGUI_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);

    // Widgets
    // -
    IMGUI_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
    IMGUI_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
    IMGUI_API void          PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

    // Widgets
    // -
    IMGUI_API void          Value(const char* prefix, bool b);
    IMGUI_API void          Value(const char* prefix, int v);
    IMGUI_API void          Value(const char* prefix, unsigned int v);
    IMGUI_API void          Value(const char* prefix, float v, const char* float_format = NULL);

    // Widgets
    // -
    // -
    // -
    // -
    IMGUI_API bool          BeginMenuBar();                                                     // append
    IMGUI_API void          EndMenuBar();                                                       // only
    IMGUI_API bool          BeginMainMenuBar();                                                 // create
    IMGUI_API void          EndMainMenuBar();                                                   // only
    IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);                  // create
    IMGUI_API void          EndMenu();                                                          // only
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return

    // Tooltips
    // -
    // -
    IMGUI_API bool          BeginTooltip();                                                     // begin
    IMGUI_API void          EndTooltip();                                                       // only
    IMGUI_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);                     // set
    IMGUI_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Tooltips
    // -
    // -
    // -
    IMGUI_API bool          BeginItemTooltip();                                                 // begin
    IMGUI_API void          SetItemTooltip(const char* fmt, ...) IM_FMTARGS(1);                 // set
    IMGUI_API void          SetItemTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Popups
    // -
    // -
    // -
    // -
    // -
    // -
    // This

    // Popups
    // -
    // -
    IMGUI_API bool          BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);                         // return
    IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0); // return
    IMGUI_API void          EndPopup();                                                                         // only

    // Popups
    // -
    // -
    // -
    // -
    // -
    // -
    // -
    IMGUI_API void          OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);                     // call
    IMGUI_API void          OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags = 0);                             // id
    IMGUI_API void          OpenPopupOnItemClick(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);   // helper
    IMGUI_API void          CloseCurrentPopup();                                                                // manually

    // Popups
    // -
    // -
    // -
    // -
    IMGUI_API bool          BeginPopupContextItem(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);  // open
    IMGUI_API bool          BeginPopupContextWindow(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);// open
    IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, ImGuiPopupFlags popup_flags = 1);  // open

    // Popups
    // -
    // -
    // -
    IMGUI_API bool          IsPopupOpen(const char* str_id, ImGuiPopupFlags flags = 0);                         // return

    // Tables
    // -
    // -
    // -
    // The
    // -
    // -
    // -
    // -
    // -
    // -
    // -
    // you
    // TableNextColumn
    // -
    // -
    // --------------------------------------------------------------------------------------------------------
    // TableNextRow
    // TableNextRow
    // TableNextColumn
    // TableNextRow
    // --------------------------------------------------------------------------------------------------------
    // -
    IMGUI_API bool          BeginTable(const char* str_id, int column, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f);
    IMGUI_API void          EndTable();                                         // only
    IMGUI_API void          TableNextRow(ImGuiTableRowFlags row_flags = 0, float min_row_height = 0.0f); // append
    IMGUI_API bool          TableNextColumn();                                  // append
    IMGUI_API bool          TableSetColumnIndex(int column_n);                  // append

    // Tables
    // -
    // -
    // Headers
    // The
    // -
    // some
    // -
    IMGUI_API void          TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
    IMGUI_API void          TableSetupScrollFreeze(int cols, int rows);         // lock
    IMGUI_API void          TableHeadersRow();                                  // submit
    IMGUI_API void          TableHeader(const char* label);                     // submit

    // Tables
    // -
    // When
    // changed
    // else
    // -
    IMGUI_API ImGuiTableSortSpecs*  TableGetSortSpecs();                        // get
    IMGUI_API int                   TableGetColumnCount();                      // return
    IMGUI_API int                   TableGetColumnIndex();                      // return
    IMGUI_API int                   TableGetRowIndex();                         // return
    IMGUI_API const char*           TableGetColumnName(int column_n = -1);      // return
    IMGUI_API ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1);     // return
    IMGUI_API void                  TableSetColumnEnabled(int column_n, bool v);// change
    IMGUI_API void                  TableSetBgColor(ImGuiTableBgTarget target, ImU32 color, int column_n = -1);  // change

    // Legacy
    // -
    IMGUI_API void          Columns(int count = 1, const char* id = NULL, bool border = true);
    IMGUI_API void          NextColumn();                                                       // next
    IMGUI_API int           GetColumnIndex();                                                   // get
    IMGUI_API float         GetColumnWidth(int column_index = -1);                              // get
    IMGUI_API void          SetColumnWidth(int column_index, float width);                      // set
    IMGUI_API float         GetColumnOffset(int column_index = -1);                             // get
    IMGUI_API void          SetColumnOffset(int column_index, float offset_x);                  // set
    IMGUI_API int           GetColumnsCount();

    // Tab
    // -
    IMGUI_API bool          BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);        // create
    IMGUI_API void          EndTabBar();                                                        // only
    IMGUI_API bool          BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0); // create
    IMGUI_API void          EndTabItem();                                                       // only
    IMGUI_API bool          TabItemButton(const char* label, ImGuiTabItemFlags flags = 0);      // create
    IMGUI_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify

    // Logging
    // -
    IMGUI_API void          LogToTTY(int auto_open_depth = -1);                                 // start
    IMGUI_API void          LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // start
    IMGUI_API void          LogToClipboard(int auto_open_depth = -1);                           // start
    IMGUI_API void          LogFinish();                                                        // stop
    IMGUI_API void          LogButtons();                                                       // helper
    IMGUI_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);                        // pass
    IMGUI_API void          LogTextV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Drag
    // -
    // -
    // -
    // -
    IMGUI_API bool          BeginDragDropSource(ImGuiDragDropFlags flags = 0);                                      // call
    IMGUI_API bool          SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0);  // type
    IMGUI_API void          EndDragDropSource();                                                                    // only
    IMGUI_API bool                  BeginDragDropTarget();                                                          // call
    IMGUI_API const ImGuiPayload*   AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);          // accept
    IMGUI_API void                  EndDragDropTarget();                                                            // only
    IMGUI_API const ImGuiPayload*   GetDragDropPayload();                                                           // peek

    // Disabling
    // -
    // -
    // -
    IMGUI_API void          BeginDisabled(bool disabled = true);
    IMGUI_API void          EndDisabled();

    // Clipping
    // -
    IMGUI_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
    IMGUI_API void          PopClipRect();

    // Focus
    // -
    IMGUI_API void          SetItemDefaultFocus();                                              // make
    IMGUI_API void          SetKeyboardFocusHere(int offset = 0);                               // focus

    // Overlapping
    IMGUI_API void          SetNextItemAllowOverlap();                                          // allow

    // Item
    // -
    // -
    IMGUI_API bool          IsItemHovered(ImGuiHoveredFlags flags = 0);                         // is
    IMGUI_API bool          IsItemActive();                                                     // is
    IMGUI_API bool          IsItemFocused();                                                    // is
    IMGUI_API bool          IsItemClicked(ImGuiMouseButton mouse_button = 0);                   // is
    IMGUI_API bool          IsItemVisible();                                                    // is
    IMGUI_API bool          IsItemEdited();                                                     // did
    IMGUI_API bool          IsItemActivated();                                                  // was
    IMGUI_API bool          IsItemDeactivated();                                                // was
    IMGUI_API bool          IsItemDeactivatedAfterEdit();                                       // was
    IMGUI_API bool          IsItemToggledOpen();                                                // was
    IMGUI_API bool          IsAnyItemHovered();                                                 // is
    IMGUI_API bool          IsAnyItemActive();                                                  // is
    IMGUI_API bool          IsAnyItemFocused();                                                 // is
    IMGUI_API ImGuiID       GetItemID();                                                        // get
    IMGUI_API ImVec2        GetItemRectMin();                                                   // get
    IMGUI_API ImVec2        GetItemRectMax();                                                   // get
    IMGUI_API ImVec2        GetItemRectSize();                                                  // get

    // Viewports
    // -
    // -
    // -
    IMGUI_API ImGuiViewport* GetMainViewport();                                                 // return

    // Background
    IMGUI_API ImDrawList*   GetBackgroundDrawList();                                            // this
    IMGUI_API ImDrawList*   GetForegroundDrawList();                                            // this

    // Miscellaneous
    IMGUI_API bool          IsRectVisible(const ImVec2& size);                                  // test
    IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);      // test
    IMGUI_API double        GetTime();                                                          // get
    IMGUI_API int           GetFrameCount();                                                    // get
    IMGUI_API ImDrawListSharedData* GetDrawListSharedData();                                    // you
    IMGUI_API const char*   GetStyleColorName(ImGuiCol idx);                                    // get
    IMGUI_API void          SetStateStorage(ImGuiStorage* storage);                             // replace
    IMGUI_API ImGuiStorage* GetStateStorage();
    IMGUI_API bool          BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags flags = 0); // helper
    IMGUI_API void          EndChildFrame();                                                    // always

    // Text
    IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    // Color
    IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
    IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
    IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    // Inputs
    // -
    // -
    // -
    // -
    IMGUI_API bool          IsKeyDown(ImGuiKey key);                                            // is
    IMGUI_API bool          IsKeyPressed(ImGuiKey key, bool repeat = true);                     // was
    IMGUI_API bool          IsKeyReleased(ImGuiKey key);                                        // was
    IMGUI_API int           GetKeyPressedAmount(ImGuiKey key, float repeat_delay, float rate);  // uses
    IMGUI_API const char*   GetKeyName(ImGuiKey key);                                           // DEBUG
    IMGUI_API void          SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard);        // Override

    // Inputs
    // -
    // -
    // -
    IMGUI_API bool          IsMouseDown(ImGuiMouseButton button);                               // is
    IMGUI_API bool          IsMouseClicked(ImGuiMouseButton button, bool repeat = false);       // did
    IMGUI_API bool          IsMouseReleased(ImGuiMouseButton button);                           // did
    IMGUI_API bool          IsMouseDoubleClicked(ImGuiMouseButton button);                      // did
    IMGUI_API int           GetMouseClickedCount(ImGuiMouseButton button);                      // return
    IMGUI_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);// is
    IMGUI_API bool          IsMousePosValid(const ImVec2* mouse_pos = NULL);                    // by
    IMGUI_API bool          IsAnyMouseDown();                                                   // WILL
    IMGUI_API ImVec2        GetMousePos();                                                      // shortcut
    IMGUI_API ImVec2        GetMousePosOnOpeningCurrentPopup();                                 // retrieve
    IMGUI_API bool          IsMouseDragging(ImGuiMouseButton button, float lock_threshold = -1.0f);         // is
    IMGUI_API ImVec2        GetMouseDragDelta(ImGuiMouseButton button = 0, float lock_threshold = -1.0f);   // return
    IMGUI_API void          ResetMouseDragDelta(ImGuiMouseButton button = 0);                   //
    IMGUI_API ImGuiMouseCursor GetMouseCursor();                                                // get
    IMGUI_API void          SetMouseCursor(ImGuiMouseCursor cursor_type);                       // set
    IMGUI_API void          SetNextFrameWantCaptureMouse(bool want_capture_mouse);              // Override

    // Clipboard
    // -
    IMGUI_API const char*   GetClipboardText();
    IMGUI_API void          SetClipboardText(const char* text);

    // Settings
    // -
    // -
    // -
    IMGUI_API void          LoadIniSettingsFromDisk(const char* ini_filename);                  // call
    IMGUI_API void          LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size=0); // call
    IMGUI_API void          SaveIniSettingsToDisk(const char* ini_filename);                    // this
    IMGUI_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL);               // return

    // Debug
    IMGUI_API void          DebugTextEncoding(const char* text);
    IMGUI_API bool          DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx); // This

    // Memory
    // -
    // -
    // for
    IMGUI_API void          SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func, ImGuiMemFreeFunc free_func, void* user_data = NULL);
    IMGUI_API void          GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data);
    IMGUI_API void*         MemAlloc(size_t size);
    IMGUI_API void          MemFree(void* ptr);

} // namespace

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Flags
// Those
enum ImGuiWindowFlags_
{
    ImGuiWindowFlags_None                   = 0,
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable
    ImGuiWindowFlags_NoResize               = 1 << 1,   // Disable
    ImGuiWindowFlags_NoMove                 = 1 << 2,   // Disable
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable
    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable
    ImGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize
    ImGuiWindowFlags_NoBackground           = 1 << 7,   // Disable
    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never
    ImGuiWindowFlags_NoMouseInputs          = 1 << 9,   // Disable
    ImGuiWindowFlags_MenuBar                = 1 << 10,  // Has
    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow
    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable
    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always
    ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure
    ImGuiWindowFlags_NoNavInputs            = 1 << 18,  // No
    ImGuiWindowFlags_NoNavFocus             = 1 << 19,  // No
    ImGuiWindowFlags_UnsavedDocument        = 1 << 20,  // Display
    ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
    ImGuiWindowFlags_NoInputs               = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    // Internal
    ImGuiWindowFlags_NavFlattened           = 1 << 23,  // BETA
    ImGuiWindowFlags_ChildWindow            = 1 << 24,  // Don
    ImGuiWindowFlags_Tooltip                = 1 << 25,  // Don
    ImGuiWindowFlags_Popup                  = 1 << 26,  // Don
    ImGuiWindowFlags_Modal                  = 1 << 27,  // Don
    ImGuiWindowFlags_ChildMenu              = 1 << 28,  // Don
};

// Flags
// Those
enum ImGuiInputTextFlags_
{
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,   // Allow
    ImGuiInputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow
    ImGuiInputTextFlags_CharsUppercase      = 1 << 2,   // Turn
    ImGuiInputTextFlags_CharsNoBlank        = 1 << 3,   // Filter
    ImGuiInputTextFlags_AutoSelectAll       = 1 << 4,   // Select
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 5,   // Return
    ImGuiInputTextFlags_CallbackCompletion  = 1 << 6,   // Callback
    ImGuiInputTextFlags_CallbackHistory     = 1 << 7,   // Callback
    ImGuiInputTextFlags_CallbackAlways      = 1 << 8,   // Callback
    ImGuiInputTextFlags_CallbackCharFilter  = 1 << 9,   // Callback
    ImGuiInputTextFlags_AllowTabInput       = 1 << 10,  // Pressing
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In
    ImGuiInputTextFlags_NoHorizontalScroll  = 1 << 12,  // Disable
    ImGuiInputTextFlags_AlwaysOverwrite     = 1 << 13,  // Overwrite
    ImGuiInputTextFlags_ReadOnly            = 1 << 14,  // Read-only
    ImGuiInputTextFlags_Password            = 1 << 15,  // Password
    ImGuiInputTextFlags_NoUndoRedo          = 1 << 16,  // Disable
    ImGuiInputTextFlags_CharsScientific     = 1 << 17,  // Allow
    ImGuiInputTextFlags_CallbackResize      = 1 << 18,  // Callback
    ImGuiInputTextFlags_CallbackEdit        = 1 << 19,  // Callback
    ImGuiInputTextFlags_EscapeClearsAll     = 1 << 20,  // Escape

    // Obsolete
    // ImGuiInputTextFlags_AlwaysInsertMode
};

// Flags
enum ImGuiTreeNodeFlags_
{
    ImGuiTreeNodeFlags_None                 = 0,
    ImGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw
    ImGuiTreeNodeFlags_Framed               = 1 << 1,   // Draw
    ImGuiTreeNodeFlags_AllowOverlap         = 1 << 2,   // Hit
    ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don
    ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don
    ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default
    ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Need
    ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Only
    ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No
    ImGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display
    ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use
    ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend
    ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // WIP
    // ImGuiTreeNodeFlags_NoScrollOnOpen
    ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    ImGuiTreeNodeFlags_AllowItemOverlap     = ImGuiTreeNodeFlags_AllowOverlap,  // Renamed
#endif
};

// Flags
// -
// small
// It
// -
// IMPORTANT
// and
// -
enum ImGuiPopupFlags_
{
    ImGuiPopupFlags_None                    = 0,
    ImGuiPopupFlags_MouseButtonLeft         = 0,        // For
    ImGuiPopupFlags_MouseButtonRight        = 1,        // For
    ImGuiPopupFlags_MouseButtonMiddle       = 2,        // For
    ImGuiPopupFlags_MouseButtonMask_        = 0x1F,
    ImGuiPopupFlags_MouseButtonDefault_     = 1,
    ImGuiPopupFlags_NoOpenOverExistingPopup = 1 << 5,   // For
    ImGuiPopupFlags_NoOpenOverItems         = 1 << 6,   // For
    ImGuiPopupFlags_AnyPopupId              = 1 << 7,   // For
    ImGuiPopupFlags_AnyPopupLevel           = 1 << 8,   // For
    ImGuiPopupFlags_AnyPopup                = ImGuiPopupFlags_AnyPopupId | ImGuiPopupFlags_AnyPopupLevel,
};

// Flags
enum ImGuiSelectableFlags_
{
    ImGuiSelectableFlags_None               = 0,
    ImGuiSelectableFlags_DontClosePopups    = 1 << 0,   // Clicking
    ImGuiSelectableFlags_SpanAllColumns     = 1 << 1,   // Selectable
    ImGuiSelectableFlags_AllowDoubleClick   = 1 << 2,   // Generate
    ImGuiSelectableFlags_Disabled           = 1 << 3,   // Cannot
    ImGuiSelectableFlags_AllowOverlap       = 1 << 4,   // WIP

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    ImGuiSelectableFlags_AllowItemOverlap   = ImGuiSelectableFlags_AllowOverlap,  // Renamed
#endif
};

// Flags
enum ImGuiComboFlags_
{
    ImGuiComboFlags_None                    = 0,
    ImGuiComboFlags_PopupAlignLeft          = 1 << 0,   // Align
    ImGuiComboFlags_HeightSmall             = 1 << 1,   // Max
    ImGuiComboFlags_HeightRegular           = 1 << 2,   // Max
    ImGuiComboFlags_HeightLarge             = 1 << 3,   // Max
    ImGuiComboFlags_HeightLargest           = 1 << 4,   // As
    ImGuiComboFlags_NoArrowButton           = 1 << 5,   // Display
    ImGuiComboFlags_NoPreview               = 1 << 6,   // Display
    ImGuiComboFlags_HeightMask_             = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest,
};

// Flags
enum ImGuiTabBarFlags_
{
    ImGuiTabBarFlags_None                           = 0,
    ImGuiTabBarFlags_Reorderable                    = 1 << 0,   // Allow
    ImGuiTabBarFlags_AutoSelectNewTabs              = 1 << 1,   // Automatically
    ImGuiTabBarFlags_TabListPopupButton             = 1 << 2,   // Disable
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,   // Disable
    ImGuiTabBarFlags_NoTabListScrollingButtons      = 1 << 4,   // Disable
    ImGuiTabBarFlags_NoTooltip                      = 1 << 5,   // Disable
    ImGuiTabBarFlags_FittingPolicyResizeDown        = 1 << 6,   // Resize
    ImGuiTabBarFlags_FittingPolicyScroll            = 1 << 7,   // Add
    ImGuiTabBarFlags_FittingPolicyMask_             = ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
    ImGuiTabBarFlags_FittingPolicyDefault_          = ImGuiTabBarFlags_FittingPolicyResizeDown,
};

// Flags
enum ImGuiTabItemFlags_
{
    ImGuiTabItemFlags_None                          = 0,
    ImGuiTabItemFlags_UnsavedDocument               = 1 << 0,   // Display
    ImGuiTabItemFlags_SetSelected                   = 1 << 1,   // Trigger
    ImGuiTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,   // Disable
    ImGuiTabItemFlags_NoPushId                      = 1 << 3,   // Don
    ImGuiTabItemFlags_NoTooltip                     = 1 << 4,   // Disable
    ImGuiTabItemFlags_NoReorder                     = 1 << 5,   // Disable
    ImGuiTabItemFlags_Leading                       = 1 << 6,   // Enforce
    ImGuiTabItemFlags_Trailing                      = 1 << 7,   // Enforce
};

// Flags
// -
// Read
// -
// -
// -
// -
// -
// -
// -
// -
// -
// The
// this
// -
// -
// -
// -
// -
// -
// If
// -
enum ImGuiTableFlags_
{
    // Features
    ImGuiTableFlags_None                       = 0,
    ImGuiTableFlags_Resizable                  = 1 << 0,   // Enable
    ImGuiTableFlags_Reorderable                = 1 << 1,   // Enable
    ImGuiTableFlags_Hideable                   = 1 << 2,   // Enable
    ImGuiTableFlags_Sortable                   = 1 << 3,   // Enable
    ImGuiTableFlags_NoSavedSettings            = 1 << 4,   // Disable
    ImGuiTableFlags_ContextMenuInBody          = 1 << 5,   // Right-click
    // Decorations
    ImGuiTableFlags_RowBg                      = 1 << 6,   // Set
    ImGuiTableFlags_BordersInnerH              = 1 << 7,   // Draw
    ImGuiTableFlags_BordersOuterH              = 1 << 8,   // Draw
    ImGuiTableFlags_BordersInnerV              = 1 << 9,   // Draw
    ImGuiTableFlags_BordersOuterV              = 1 << 10,  // Draw
    ImGuiTableFlags_BordersH                   = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersOuterH, // Draw
    ImGuiTableFlags_BordersV                   = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersOuterV, // Draw
    ImGuiTableFlags_BordersInner               = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH, // Draw
    ImGuiTableFlags_BordersOuter               = ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH, // Draw
    ImGuiTableFlags_Borders                    = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersOuter,   // Draw
    ImGuiTableFlags_NoBordersInBody            = 1 << 11,  // ALPHA
    ImGuiTableFlags_NoBordersInBodyUntilResize = 1 << 12,  // ALPHA
    // Sizing
    ImGuiTableFlags_SizingFixedFit             = 1 << 13,  // Columns
    ImGuiTableFlags_SizingFixedSame            = 2 << 13,  // Columns
    ImGuiTableFlags_SizingStretchProp          = 3 << 13,  // Columns
    ImGuiTableFlags_SizingStretchSame          = 4 << 13,  // Columns
    // Sizing
    ImGuiTableFlags_NoHostExtendX              = 1 << 16,  // Make
    ImGuiTableFlags_NoHostExtendY              = 1 << 17,  // Make
    ImGuiTableFlags_NoKeepColumnsVisible       = 1 << 18,  // Disable
    ImGuiTableFlags_PreciseWidths              = 1 << 19,  // Disable
    // Clipping
    ImGuiTableFlags_NoClip                     = 1 << 20,  // Disable
    // Padding
    ImGuiTableFlags_PadOuterX                  = 1 << 21,  // Default
    ImGuiTableFlags_NoPadOuterX                = 1 << 22,  // Default
    ImGuiTableFlags_NoPadInnerX                = 1 << 23,  // Disable
    // Scrolling
    ImGuiTableFlags_ScrollX                    = 1 << 24,  // Enable
    ImGuiTableFlags_ScrollY                    = 1 << 25,  // Enable
    // Sorting
    ImGuiTableFlags_SortMulti                  = 1 << 26,  // Hold
    ImGuiTableFlags_SortTristate               = 1 << 27,  // Allow

    // Internal
    ImGuiTableFlags_SizingMask_                = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_SizingStretchSame,
};

// Flags
enum ImGuiTableColumnFlags_
{
    // Input
    ImGuiTableColumnFlags_None                  = 0,
    ImGuiTableColumnFlags_Disabled              = 1 << 0,   // Overriding
    ImGuiTableColumnFlags_DefaultHide           = 1 << 1,   // Default
    ImGuiTableColumnFlags_DefaultSort           = 1 << 2,   // Default
    ImGuiTableColumnFlags_WidthStretch          = 1 << 3,   // Column
    ImGuiTableColumnFlags_WidthFixed            = 1 << 4,   // Column
    ImGuiTableColumnFlags_NoResize              = 1 << 5,   // Disable
    ImGuiTableColumnFlags_NoReorder             = 1 << 6,   // Disable
    ImGuiTableColumnFlags_NoHide                = 1 << 7,   // Disable
    ImGuiTableColumnFlags_NoClip                = 1 << 8,   // Disable
    ImGuiTableColumnFlags_NoSort                = 1 << 9,   // Disable
    ImGuiTableColumnFlags_NoSortAscending       = 1 << 10,  // Disable
    ImGuiTableColumnFlags_NoSortDescending      = 1 << 11,  // Disable
    ImGuiTableColumnFlags_NoHeaderLabel         = 1 << 12,  // TableHeadersRow
    ImGuiTableColumnFlags_NoHeaderWidth         = 1 << 13,  // Disable
    ImGuiTableColumnFlags_PreferSortAscending   = 1 << 14,  // Make
    ImGuiTableColumnFlags_PreferSortDescending  = 1 << 15,  // Make
    ImGuiTableColumnFlags_IndentEnable          = 1 << 16,  // Use
    ImGuiTableColumnFlags_IndentDisable         = 1 << 17,  // Ignore

    // Output
    ImGuiTableColumnFlags_IsEnabled             = 1 << 24,  // Status
    ImGuiTableColumnFlags_IsVisible             = 1 << 25,  // Status
    ImGuiTableColumnFlags_IsSorted              = 1 << 26,  // Status
    ImGuiTableColumnFlags_IsHovered             = 1 << 27,  // Status

    // Internal
    ImGuiTableColumnFlags_WidthMask_            = ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_WidthFixed,
    ImGuiTableColumnFlags_IndentMask_           = ImGuiTableColumnFlags_IndentEnable | ImGuiTableColumnFlags_IndentDisable,
    ImGuiTableColumnFlags_StatusMask_           = ImGuiTableColumnFlags_IsEnabled | ImGuiTableColumnFlags_IsVisible | ImGuiTableColumnFlags_IsSorted | ImGuiTableColumnFlags_IsHovered,
    ImGuiTableColumnFlags_NoDirectResize_       = 1 << 30,  // Internal
};

// Flags
enum ImGuiTableRowFlags_
{
    ImGuiTableRowFlags_None                     = 0,
    ImGuiTableRowFlags_Headers                  = 1 << 0,   // Identify
};

// Enum
// Background
// -
// -
// -
// The
// When
// If
// If
enum ImGuiTableBgTarget_
{
    ImGuiTableBgTarget_None                     = 0,
    ImGuiTableBgTarget_RowBg0                   = 1,        // Set
    ImGuiTableBgTarget_RowBg1                   = 2,        // Set
    ImGuiTableBgTarget_CellBg                   = 3,        // Set
};

// Flags
enum ImGuiFocusedFlags_
{
    ImGuiFocusedFlags_None                          = 0,
    ImGuiFocusedFlags_ChildWindows                  = 1 << 0,   // Return
    ImGuiFocusedFlags_RootWindow                    = 1 << 1,   // Test
    ImGuiFocusedFlags_AnyWindow                     = 1 << 2,   // Return
    ImGuiFocusedFlags_NoPopupHierarchy              = 1 << 3,   // Do
    // ImGuiFocusedFlags_DockHierarchy
    ImGuiFocusedFlags_RootAndChildWindows           = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows,
};

// Flags
// Note
// Note
enum ImGuiHoveredFlags_
{
    ImGuiHoveredFlags_None                          = 0,        // Return
    ImGuiHoveredFlags_ChildWindows                  = 1 << 0,   // IsWindowHovered
    ImGuiHoveredFlags_RootWindow                    = 1 << 1,   // IsWindowHovered
    ImGuiHoveredFlags_AnyWindow                     = 1 << 2,   // IsWindowHovered
    ImGuiHoveredFlags_NoPopupHierarchy              = 1 << 3,   // IsWindowHovered
    // ImGuiHoveredFlags_DockHierarchy
    ImGuiHoveredFlags_AllowWhenBlockedByPopup       = 1 << 5,   // Return
    // ImGuiHoveredFlags_AllowWhenBlockedByModal
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 7,   // Return
    ImGuiHoveredFlags_AllowWhenOverlappedByItem     = 1 << 8,   // IsItemHovered
    ImGuiHoveredFlags_AllowWhenOverlappedByWindow   = 1 << 9,   // IsItemHovered
    ImGuiHoveredFlags_AllowWhenDisabled             = 1 << 10,  // IsItemHovered
    ImGuiHoveredFlags_NoNavOverride                 = 1 << 11,  // IsItemHovered
    ImGuiHoveredFlags_AllowWhenOverlapped           = ImGuiHoveredFlags_AllowWhenOverlappedByItem | ImGuiHoveredFlags_AllowWhenOverlappedByWindow,
    ImGuiHoveredFlags_RectOnly                      = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows           = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows,

    // Tooltips
    // -
    // -
    // e.g.
    // -
    // -
    ImGuiHoveredFlags_ForTooltip                    = 1 << 12,  // Shortcut

    // Advanced
    // -
    // -
    ImGuiHoveredFlags_Stationary                    = 1 << 13,  // Require
    ImGuiHoveredFlags_DelayNone                     = 1 << 14,  // IsItemHovered
    ImGuiHoveredFlags_DelayShort                    = 1 << 15,  // IsItemHovered
    ImGuiHoveredFlags_DelayNormal                   = 1 << 16,  // IsItemHovered
    ImGuiHoveredFlags_NoSharedDelay                 = 1 << 17,  // IsItemHovered
};

// Flags
enum ImGuiDragDropFlags_
{
    ImGuiDragDropFlags_None                         = 0,
    // BeginDragDropSource
    ImGuiDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,   // Disable
    ImGuiDragDropFlags_SourceNoDisableHover         = 1 << 1,   // By
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,   // Disable
    ImGuiDragDropFlags_SourceAllowNullID            = 1 << 3,   // Allow
    ImGuiDragDropFlags_SourceExtern                 = 1 << 4,   // External
    ImGuiDragDropFlags_SourceAutoExpirePayload      = 1 << 5,   // Automatically
    // AcceptDragDropPayload
    ImGuiDragDropFlags_AcceptBeforeDelivery         = 1 << 10,  // AcceptDragDropPayload
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,  // Do
    ImGuiDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,  // Request
    ImGuiDragDropFlags_AcceptPeekOnly               = ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect, // For
};

// Standard
#define IMGUI_PAYLOAD_TYPE_COLOR_3F     "_COL3F"    // float
#define IMGUI_PAYLOAD_TYPE_COLOR_4F     "_COL4F"    // float

// A
enum ImGuiDataType_
{
    ImGuiDataType_S8,       // signed
    ImGuiDataType_U8,       // unsigned
    ImGuiDataType_S16,      // short
    ImGuiDataType_U16,      // unsigned
    ImGuiDataType_S32,      // int
    ImGuiDataType_U32,      // unsigned
    ImGuiDataType_S64,      // long
    ImGuiDataType_U64,      // unsigned
    ImGuiDataType_Float,    // float
    ImGuiDataType_Double,   // double
    ImGuiDataType_COUNT
};

// A
enum ImGuiDir_
{
    ImGuiDir_None    = -1,
    ImGuiDir_Left    = 0,
    ImGuiDir_Right   = 1,
    ImGuiDir_Up      = 2,
    ImGuiDir_Down    = 3,
    ImGuiDir_COUNT
};

// A
enum ImGuiSortDirection_
{
    ImGuiSortDirection_None         = 0,
    ImGuiSortDirection_Ascending    = 1,    // Ascending
    ImGuiSortDirection_Descending   = 2     // Descending
};

// A
// All
// Since
// Read
// Note
enum ImGuiKey : int
{
    // Keyboard
    ImGuiKey_None = 0,
    ImGuiKey_Tab = 512,             // ImGuiKey_NamedKey_BEGIN
    ImGuiKey_LeftArrow,
    ImGuiKey_RightArrow,
    ImGuiKey_UpArrow,
    ImGuiKey_DownArrow,
    ImGuiKey_PageUp,
    ImGuiKey_PageDown,
    ImGuiKey_Home,
    ImGuiKey_End,
    ImGuiKey_Insert,
    ImGuiKey_Delete,
    ImGuiKey_Backspace,
    ImGuiKey_Space,
    ImGuiKey_Enter,
    ImGuiKey_Escape,
    ImGuiKey_LeftCtrl, ImGuiKey_LeftShift, ImGuiKey_LeftAlt, ImGuiKey_LeftSuper,
    ImGuiKey_RightCtrl, ImGuiKey_RightShift, ImGuiKey_RightAlt, ImGuiKey_RightSuper,
    ImGuiKey_Menu,
    ImGuiKey_0, ImGuiKey_1, ImGuiKey_2, ImGuiKey_3, ImGuiKey_4, ImGuiKey_5, ImGuiKey_6, ImGuiKey_7, ImGuiKey_8, ImGuiKey_9,
    ImGuiKey_A, ImGuiKey_B, ImGuiKey_C, ImGuiKey_D, ImGuiKey_E, ImGuiKey_F, ImGuiKey_G, ImGuiKey_H, ImGuiKey_I, ImGuiKey_J,
    ImGuiKey_K, ImGuiKey_L, ImGuiKey_M, ImGuiKey_N, ImGuiKey_O, ImGuiKey_P, ImGuiKey_Q, ImGuiKey_R, ImGuiKey_S, ImGuiKey_T,
    ImGuiKey_U, ImGuiKey_V, ImGuiKey_W, ImGuiKey_X, ImGuiKey_Y, ImGuiKey_Z,
    ImGuiKey_F1, ImGuiKey_F2, ImGuiKey_F3, ImGuiKey_F4, ImGuiKey_F5, ImGuiKey_F6,
    ImGuiKey_F7, ImGuiKey_F8, ImGuiKey_F9, ImGuiKey_F10, ImGuiKey_F11, ImGuiKey_F12,
    ImGuiKey_Apostrophe,        //
    ImGuiKey_Comma,             //
    ImGuiKey_Minus,             // -
    ImGuiKey_Period,            // .
    ImGuiKey_Slash,             //
    ImGuiKey_Semicolon,         //
    ImGuiKey_Equal,             //
    ImGuiKey_LeftBracket,       //
    ImGuiKey_Backslash,         // this
    ImGuiKey_RightBracket,      //
    ImGuiKey_GraveAccent,       //
    ImGuiKey_CapsLock,
    ImGuiKey_ScrollLock,
    ImGuiKey_NumLock,
    ImGuiKey_PrintScreen,
    ImGuiKey_Pause,
    ImGuiKey_Keypad0, ImGuiKey_Keypad1, ImGuiKey_Keypad2, ImGuiKey_Keypad3, ImGuiKey_Keypad4,
    ImGuiKey_Keypad5, ImGuiKey_Keypad6, ImGuiKey_Keypad7, ImGuiKey_Keypad8, ImGuiKey_Keypad9,
    ImGuiKey_KeypadDecimal,
    ImGuiKey_KeypadDivide,
    ImGuiKey_KeypadMultiply,
    ImGuiKey_KeypadSubtract,
    ImGuiKey_KeypadAdd,
    ImGuiKey_KeypadEnter,
    ImGuiKey_KeypadEqual,

    // Gamepad
    // download
    ImGuiKey_GamepadStart,          // Menu
    ImGuiKey_GamepadBack,           // View
    ImGuiKey_GamepadFaceLeft,       // X
    ImGuiKey_GamepadFaceRight,      // B
    ImGuiKey_GamepadFaceUp,         // Y
    ImGuiKey_GamepadFaceDown,       // A
    ImGuiKey_GamepadDpadLeft,       // D-pad
    ImGuiKey_GamepadDpadRight,      // D-pad
    ImGuiKey_GamepadDpadUp,         // D-pad
    ImGuiKey_GamepadDpadDown,       // D-pad
    ImGuiKey_GamepadL1,             // L
    ImGuiKey_GamepadR1,             // R
    ImGuiKey_GamepadL2,             // L
    ImGuiKey_GamepadR2,             // R
    ImGuiKey_GamepadL3,             // L
    ImGuiKey_GamepadR3,             // R
    ImGuiKey_GamepadLStickLeft,     // Analog
    ImGuiKey_GamepadLStickRight,    // Analog
    ImGuiKey_GamepadLStickUp,       // Analog
    ImGuiKey_GamepadLStickDown,     // Analog
    ImGuiKey_GamepadRStickLeft,     // Analog
    ImGuiKey_GamepadRStickRight,    // Analog
    ImGuiKey_GamepadRStickUp,       // Analog
    ImGuiKey_GamepadRStickDown,     // Analog

    // Aliases
    // -
    ImGuiKey_MouseLeft, ImGuiKey_MouseRight, ImGuiKey_MouseMiddle, ImGuiKey_MouseX1, ImGuiKey_MouseX2, ImGuiKey_MouseWheelX, ImGuiKey_MouseWheelY,

    // Internal
    ImGuiKey_ReservedForModCtrl, ImGuiKey_ReservedForModShift, ImGuiKey_ReservedForModAlt, ImGuiKey_ReservedForModSuper,
    ImGuiKey_COUNT,

    // Keyboard
    // -
    // them
    // -
    // and
    // -
    // In
    // backends
    ImGuiMod_None                   = 0,
    ImGuiMod_Ctrl                   = 1 << 12, // Ctrl
    ImGuiMod_Shift                  = 1 << 13, // Shift
    ImGuiMod_Alt                    = 1 << 14, // Option
    ImGuiMod_Super                  = 1 << 15, // Cmd
    ImGuiMod_Shortcut               = 1 << 11, // Alias
    ImGuiMod_Mask_                  = 0xF800,  // 5-bits

    // Internal
    // We
    // If
    ImGuiKey_NamedKey_BEGIN         = 512,
    ImGuiKey_NamedKey_END           = ImGuiKey_COUNT,
    ImGuiKey_NamedKey_COUNT         = ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,
#ifdef IMGUI_DISABLE_OBSOLETE_KEYIO
    ImGuiKey_KeysData_SIZE          = ImGuiKey_NamedKey_COUNT,  // Size
    ImGuiKey_KeysData_OFFSET        = ImGuiKey_NamedKey_BEGIN,  // Accesses
#else
    ImGuiKey_KeysData_SIZE          = ImGuiKey_COUNT,           // Size
    ImGuiKey_KeysData_OFFSET        = 0,                        // Accesses
#endif

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    ImGuiKey_ModCtrl = ImGuiMod_Ctrl, ImGuiKey_ModShift = ImGuiMod_Shift, ImGuiKey_ModAlt = ImGuiMod_Alt, ImGuiKey_ModSuper = ImGuiMod_Super, // Renamed
    ImGuiKey_KeyPadEnter = ImGuiKey_KeypadEnter,    // Renamed
#endif
};

#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
// OBSOLETED
// Official
// Custom
enum ImGuiNavInput
{
    ImGuiNavInput_Activate, ImGuiNavInput_Cancel, ImGuiNavInput_Input, ImGuiNavInput_Menu, ImGuiNavInput_DpadLeft, ImGuiNavInput_DpadRight, ImGuiNavInput_DpadUp, ImGuiNavInput_DpadDown,
    ImGuiNavInput_LStickLeft, ImGuiNavInput_LStickRight, ImGuiNavInput_LStickUp, ImGuiNavInput_LStickDown, ImGuiNavInput_FocusPrev, ImGuiNavInput_FocusNext, ImGuiNavInput_TweakSlow, ImGuiNavInput_TweakFast,
    ImGuiNavInput_COUNT,
};
#endif

// Configuration
enum ImGuiConfigFlags_
{
    ImGuiConfigFlags_None                   = 0,
    ImGuiConfigFlags_NavEnableKeyboard      = 1 << 0,   // Master
    ImGuiConfigFlags_NavEnableGamepad       = 1 << 1,   // Master
    ImGuiConfigFlags_NavEnableSetMousePos   = 1 << 2,   // Instruct
    ImGuiConfigFlags_NavNoCaptureKeyboard   = 1 << 3,   // Instruct
    ImGuiConfigFlags_NoMouse                = 1 << 4,   // Instruct
    ImGuiConfigFlags_NoMouseCursorChange    = 1 << 5,   // Instruct

    // User
    ImGuiConfigFlags_IsSRGB                 = 1 << 20,  // Application
    ImGuiConfigFlags_IsTouchScreen          = 1 << 21,  // Application
};

// Backend
enum ImGuiBackendFlags_
{
    ImGuiBackendFlags_None                  = 0,
    ImGuiBackendFlags_HasGamepad            = 1 << 0,   // Backend
    ImGuiBackendFlags_HasMouseCursors       = 1 << 1,   // Backend
    ImGuiBackendFlags_HasSetMousePos        = 1 << 2,   // Backend
    ImGuiBackendFlags_RendererHasVtxOffset  = 1 << 3,   // Backend
};

// Enumeration
enum ImGuiCol_
{
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,              // Background
    ImGuiCol_ChildBg,               // Background
    ImGuiCol_PopupBg,               // Background
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,               // Background
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,                // Header
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,            // Resize
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,                   // TabItem
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,         // Table
    ImGuiCol_TableBorderStrong,     // Table
    ImGuiCol_TableBorderLight,      // Table
    ImGuiCol_TableRowBg,            // Table
    ImGuiCol_TableRowBgAlt,         // Table
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,        // Rectangle
    ImGuiCol_NavHighlight,          // Gamepad
    ImGuiCol_NavWindowingHighlight, // Highlight
    ImGuiCol_NavWindowingDimBg,     // Darken
    ImGuiCol_ModalWindowDimBg,      // Darken
    ImGuiCol_COUNT
};

// Enumeration
// -
// During
// -
// In
// With
// -
enum ImGuiStyleVar_
{
    // Enum
    ImGuiStyleVar_Alpha,               // float
    ImGuiStyleVar_DisabledAlpha,       // float
    ImGuiStyleVar_WindowPadding,       // ImVec2
    ImGuiStyleVar_WindowRounding,      // float
    ImGuiStyleVar_WindowBorderSize,    // float
    ImGuiStyleVar_WindowMinSize,       // ImVec2
    ImGuiStyleVar_WindowTitleAlign,    // ImVec2
    ImGuiStyleVar_ChildRounding,       // float
    ImGuiStyleVar_ChildBorderSize,     // float
    ImGuiStyleVar_PopupRounding,       // float
    ImGuiStyleVar_PopupBorderSize,     // float
    ImGuiStyleVar_FramePadding,        // ImVec2
    ImGuiStyleVar_FrameRounding,       // float
    ImGuiStyleVar_FrameBorderSize,     // float
    ImGuiStyleVar_ItemSpacing,         // ImVec2
    ImGuiStyleVar_ItemInnerSpacing,    // ImVec2
    ImGuiStyleVar_IndentSpacing,       // float
    ImGuiStyleVar_CellPadding,         // ImVec2
    ImGuiStyleVar_ScrollbarSize,       // float
    ImGuiStyleVar_ScrollbarRounding,   // float
    ImGuiStyleVar_GrabMinSize,         // float
    ImGuiStyleVar_GrabRounding,        // float
    ImGuiStyleVar_TabRounding,         // float
    ImGuiStyleVar_ButtonTextAlign,     // ImVec2
    ImGuiStyleVar_SelectableTextAlign, // ImVec2
    ImGuiStyleVar_SeparatorTextBorderSize,// float
    ImGuiStyleVar_SeparatorTextAlign,  // ImVec2
    ImGuiStyleVar_SeparatorTextPadding,// ImVec2
    ImGuiStyleVar_COUNT
};

// Flags
enum ImGuiButtonFlags_
{
    ImGuiButtonFlags_None                   = 0,
    ImGuiButtonFlags_MouseButtonLeft        = 1 << 0,   // React
    ImGuiButtonFlags_MouseButtonRight       = 1 << 1,   // React
    ImGuiButtonFlags_MouseButtonMiddle      = 1 << 2,   // React

    // Internal
    ImGuiButtonFlags_MouseButtonMask_       = ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonMiddle,
    ImGuiButtonFlags_MouseButtonDefault_    = ImGuiButtonFlags_MouseButtonLeft,
};

// Flags
enum ImGuiColorEditFlags_
{
    ImGuiColorEditFlags_None            = 0,
    ImGuiColorEditFlags_NoAlpha         = 1 << 1,   // ColorEdit
    ImGuiColorEditFlags_NoPicker        = 1 << 2,   // ColorEdit
    ImGuiColorEditFlags_NoOptions       = 1 << 3,   // ColorEdit
    ImGuiColorEditFlags_NoSmallPreview  = 1 << 4,   // ColorEdit
    ImGuiColorEditFlags_NoInputs        = 1 << 5,   // ColorEdit
    ImGuiColorEditFlags_NoTooltip       = 1 << 6,   // ColorEdit
    ImGuiColorEditFlags_NoLabel         = 1 << 7,   // ColorEdit
    ImGuiColorEditFlags_NoSidePreview   = 1 << 8,   // ColorPicker
    ImGuiColorEditFlags_NoDragDrop      = 1 << 9,   // ColorEdit
    ImGuiColorEditFlags_NoBorder        = 1 << 10,  // ColorButton

    // User
    ImGuiColorEditFlags_AlphaBar        = 1 << 16,  // ColorEdit
    ImGuiColorEditFlags_AlphaPreview    = 1 << 17,  // ColorEdit
    ImGuiColorEditFlags_AlphaPreviewHalf= 1 << 18,  // ColorEdit
    ImGuiColorEditFlags_HDR             = 1 << 19,  // WIP
    ImGuiColorEditFlags_DisplayRGB      = 1 << 20,  // Display
    ImGuiColorEditFlags_DisplayHSV      = 1 << 21,  // Display
    ImGuiColorEditFlags_DisplayHex      = 1 << 22,  // Display
    ImGuiColorEditFlags_Uint8           = 1 << 23,  // DataType
    ImGuiColorEditFlags_Float           = 1 << 24,  // DataType
    ImGuiColorEditFlags_PickerHueBar    = 1 << 25,  // Picker
    ImGuiColorEditFlags_PickerHueWheel  = 1 << 26,  // Picker
    ImGuiColorEditFlags_InputRGB        = 1 << 27,  // Input
    ImGuiColorEditFlags_InputHSV        = 1 << 28,  // Input

    // Defaults
    // override
    ImGuiColorEditFlags_DefaultOptions_ = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar,

    // Internal
    ImGuiColorEditFlags_DisplayMask_    = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex,
    ImGuiColorEditFlags_DataTypeMask_   = ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_Float,
    ImGuiColorEditFlags_PickerMask_     = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_PickerHueBar,
    ImGuiColorEditFlags_InputMask_      = ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_InputHSV,

    // Obsolete
    // ImGuiColorEditFlags_RGB
};

// Flags
// We
// Those
enum ImGuiSliderFlags_
{
    ImGuiSliderFlags_None                   = 0,
    ImGuiSliderFlags_AlwaysClamp            = 1 << 4,       // Clamp
    ImGuiSliderFlags_Logarithmic            = 1 << 5,       // Make
    ImGuiSliderFlags_NoRoundToFormat        = 1 << 6,       // Disable
    ImGuiSliderFlags_NoInput                = 1 << 7,       // Disable
    ImGuiSliderFlags_InvalidMask_           = 0x7000000F,   // Internal

    // Obsolete
    // ImGuiSliderFlags_ClampOnInput
};

// Identify
// Those
enum ImGuiMouseButton_
{
    ImGuiMouseButton_Left = 0,
    ImGuiMouseButton_Right = 1,
    ImGuiMouseButton_Middle = 2,
    ImGuiMouseButton_COUNT = 5
};

// Enumeration
// User
enum ImGuiMouseCursor_
{
    ImGuiMouseCursor_None = -1,
    ImGuiMouseCursor_Arrow = 0,
    ImGuiMouseCursor_TextInput,         // When
    ImGuiMouseCursor_ResizeAll,         // Unused
    ImGuiMouseCursor_ResizeNS,          // When
    ImGuiMouseCursor_ResizeEW,          // When
    ImGuiMouseCursor_ResizeNESW,        // When
    ImGuiMouseCursor_ResizeNWSE,        // When
    ImGuiMouseCursor_Hand,              // Unused
    ImGuiMouseCursor_NotAllowed,        // When
    ImGuiMouseCursor_COUNT
};

// Enumeration
// Historically
// But
// You
enum ImGuiMouseSource : int
{
    ImGuiMouseSource_Mouse = 0,         // Input
    ImGuiMouseSource_TouchScreen,       // Input
    ImGuiMouseSource_Pen,               // Input
    ImGuiMouseSource_COUNT
};

// Enumeration
// Represent
// Important
enum ImGuiCond_
{
    ImGuiCond_None          = 0,        // No
    ImGuiCond_Always        = 1 << 0,   // No
    ImGuiCond_Once          = 1 << 1,   // Set
    ImGuiCond_FirstUseEver  = 1 << 2,   // Set
    ImGuiCond_Appearing     = 1 << 3,   // Set
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// IM_MALLOC
// We
// Defining
// -----------------------------------------------------------------------------

struct ImNewWrapper {};
inline void* operator new(size_t, ImNewWrapper, void* ptr) { return ptr; }
inline void  operator delete(void*, ImNewWrapper, void*)   {} // This
#define IM_ALLOC(_SIZE)                     ImGui::MemAlloc(_SIZE)
#define IM_FREE(_PTR)                       ImGui::MemFree(_PTR)
#define IM_PLACEMENT_NEW(_PTR)              new(ImNewWrapper(), _PTR)
#define IM_NEW(_TYPE)                       new(ImNewWrapper(), ImGui::MemAlloc(sizeof(_TYPE))) _TYPE
template<typename T> void IM_DELETE(T* p)   { if (p) { p->~T(); ImGui::MemFree(p); } }

// -----------------------------------------------------------------------------
// ImVector
// Lightweight
// -----------------------------------------------------------------------------
// -
// -
// -
// -
// Do
// -----------------------------------------------------------------------------

IM_MSVC_RUNTIME_CHECKS_OFF
template<typename T>
struct ImVector
{
    int                 Size;
    int                 Capacity;
    T*                  Data;

    // Provide
    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    // Constructors
    inline ImVector()                                       { Size = Capacity = 0; Data = NULL; }
    inline ImVector(const ImVector<T>& src)                 { Size = Capacity = 0; Data = NULL; operator=(src); }
    inline ImVector<T>& operator=(const ImVector<T>& src)   { clear(); resize(src.Size); if (src.Data) memcpy(Data, src.Data, (size_t)Size * sizeof(T)); return *this; }
    inline ~ImVector()                                      { if (Data) IM_FREE(Data); } // Important

    inline void         clear()                             { if (Data) { Size = Capacity = 0; IM_FREE(Data); Data = NULL; } }  // Important
    inline void         clear_delete()                      { for (int n = 0; n < Size; n++) IM_DELETE(Data[n]); clear(); }     // Important
    inline void         clear_destruct()                    { for (int n = 0; n < Size; n++) Data[n].~T(); clear(); }           // Important

    inline bool         empty() const                       { return Size == 0; }
    inline int          size() const                        { return Size; }
    inline int          size_in_bytes() const               { return Size * (int)sizeof(T); }
    inline int          max_size() const                    { return 0x7FFFFFFF / (int)sizeof(T); }
    inline int          capacity() const                    { return Capacity; }
    inline T&           operator[](int i)                   { IM_ASSERT(i >= 0 && i < Size); return Data[i]; }
    inline const T&     operator[](int i) const             { IM_ASSERT(i >= 0 && i < Size); return Data[i]; }

    inline T*           begin()                             { return Data; }
    inline const T*     begin() const                       { return Data; }
    inline T*           end()                               { return Data + Size; }
    inline const T*     end() const                         { return Data + Size; }
    inline T&           front()                             { IM_ASSERT(Size > 0); return Data[0]; }
    inline const T&     front() const                       { IM_ASSERT(Size > 0); return Data[0]; }
    inline T&           back()                              { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline const T&     back() const                        { IM_ASSERT(Size > 0); return Data[Size - 1]; }
    inline void         swap(ImVector<T>& rhs)              { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int          _grow_capacity(int sz) const        { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
    inline void         resize(int new_size)                { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
    inline void         shrink(int new_size)                { IM_ASSERT(new_size <= Size); Size = new_size; } // Resize
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
    inline void         reserve_discard(int new_capacity)   { if (new_capacity <= Capacity) return; if (Data) IM_FREE(Data); Data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); Capacity = new_capacity; }

    // NB
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
    inline void         pop_back()                          { IM_ASSERT(Size > 0); Size--; }
    inline void         push_front(const T& v)              { if (Size == 0) push_back(v); else insert(Data, v); }
    inline T*           erase(const T* it)                  { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
    inline T*           erase(const T* it, const T* it_last){ IM_ASSERT(it >= Data && it < Data + Size && it_last >= it && it_last <= Data + Size); const ptrdiff_t count = it_last - it; const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + count, ((size_t)Size - (size_t)off - (size_t)count) * sizeof(T)); Size -= (int)count; return Data + off; }
    inline T*           erase_unsorted(const T* it)         { IM_ASSERT(it >= Data && it < Data + Size);  const ptrdiff_t off = it - Data; if (it < Data + Size - 1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
    inline T*           insert(const T* it, const T& v)     { IM_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
    inline bool         contains(const T& v) const          { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
    inline T*           find(const T& v)                    { T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline const T*     find(const T& v) const              { const T* data = Data;  const T* data_end = Data + Size; while (data < data_end) if (*data == v) break; else ++data; return data; }
    inline bool         find_erase(const T& v)              { const T* it = find(v); if (it < Data + Size) { erase(it); return true; } return false; }
    inline bool         find_erase_unsorted(const T& v)     { const T* it = find(v); if (it < Data + Size) { erase_unsorted(it); return true; } return false; }
    inline int          index_from_ptr(const T* it) const   { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; return (int)off; }
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------
// You
// During
// and
// -----------------------------------------------------------------------------

struct ImGuiStyle
{
    float       Alpha;                      // Global
    float       DisabledAlpha;              // Additional
    ImVec2      WindowPadding;              // Padding
    float       WindowRounding;             // Radius
    float       WindowBorderSize;           // Thickness
    ImVec2      WindowMinSize;              // Minimum
    ImVec2      WindowTitleAlign;           // Alignment
    ImGuiDir    WindowMenuButtonPosition;   // Side
    float       ChildRounding;              // Radius
    float       ChildBorderSize;            // Thickness
    float       PopupRounding;              // Radius
    float       PopupBorderSize;            // Thickness
    ImVec2      FramePadding;               // Padding
    float       FrameRounding;              // Radius
    float       FrameBorderSize;            // Thickness
    ImVec2      ItemSpacing;                // Horizontal
    ImVec2      ItemInnerSpacing;           // Horizontal
    ImVec2      CellPadding;                // Padding
    ImVec2      TouchExtraPadding;          // Expand
    float       IndentSpacing;              // Horizontal
    float       ColumnsMinSpacing;          // Minimum
    float       ScrollbarSize;              // Width
    float       ScrollbarRounding;          // Radius
    float       GrabMinSize;                // Minimum
    float       GrabRounding;               // Radius
    float       LogSliderDeadzone;          // The
    float       TabRounding;                // Radius
    float       TabBorderSize;              // Thickness
    float       TabMinWidthForCloseButton;  // Minimum
    ImGuiDir    ColorButtonPosition;        // Side
    ImVec2      ButtonTextAlign;            // Alignment
    ImVec2      SelectableTextAlign;        // Alignment
    float       SeparatorTextBorderSize;    // Thickkness
    ImVec2      SeparatorTextAlign;         // Alignment
    ImVec2      SeparatorTextPadding;       // Horizontal
    ImVec2      DisplayWindowPadding;       // Window
    ImVec2      DisplaySafeAreaPadding;     // If
    float       MouseCursorScale;           // Scale
    bool        AntiAliasedLines;           // Enable
    bool        AntiAliasedLinesUseTex;     // Enable
    bool        AntiAliasedFill;            // Enable
    float       CurveTessellationTol;       // Tessellation
    float       CircleTessellationMaxError; // Maximum
    ImVec4      Colors[ImGuiCol_COUNT];

    // Behaviors
    // It
    float             HoverStationaryDelay;     // Delay
    float             HoverDelayShort;          // Delay
    float             HoverDelayNormal;         // Delay
    ImGuiHoveredFlags HoverFlagsForTooltipMouse;// Default
    ImGuiHoveredFlags HoverFlagsForTooltipNav;  // Default

    IMGUI_API ImGuiStyle();
    IMGUI_API void ScaleAllSizes(float scale_factor);
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------
// Communicate
// Access
// -----------------------------------------------------------------------------

// Internal
// If
struct ImGuiKeyData
{
    bool        Down;               // True
    float       DownDuration;       // Duration
    float       DownDurationPrev;   // Last
    float       AnalogValue;        // 0.0f..1.0f
};

struct ImGuiIO
{
    // ------------------------------------------------------------------
    // Configuration
    // ------------------------------------------------------------------

    ImGuiConfigFlags   ConfigFlags;             // 0
    ImGuiBackendFlags  BackendFlags;            // 0
    ImVec2      DisplaySize;                    // unset
    float       DeltaTime;                      // 1.0f
    float       IniSavingRate;                  // 5.0f
    const char* IniFilename;                    // imgui.ini
    const char* LogFilename;                    // imgui_log.txt
    void*       UserData;                       // NULL

    ImFontAtlas*Fonts;                          // auto
    float       FontGlobalScale;                // 1.0f
    bool        FontAllowUserScaling;           // false
    ImFont*     FontDefault;                    // NULL
    ImVec2      DisplayFramebufferScale;        // 1

    // Miscellaneous
    bool        MouseDrawCursor;                // false
    bool        ConfigMacOSXBehaviors;          // defined
    bool        ConfigInputTrickleEventQueue;   // true
    bool        ConfigInputTextCursorBlink;     // true
    bool        ConfigInputTextEnterKeepActive; // false
    bool        ConfigDragClickToInputText;     // false
    bool        ConfigWindowsResizeFromEdges;   // true
    bool        ConfigWindowsMoveFromTitleBarOnly; // false
    float       ConfigMemoryCompactTimer;       // 60.0f

    // Inputs
    // other
    float       MouseDoubleClickTime;           // 0.30f
    float       MouseDoubleClickMaxDist;        // 6.0f
    float       MouseDragThreshold;             // 6.0f
    float       KeyRepeatDelay;                 // 0.275f
    float       KeyRepeatRate;                  // 0.050f

    // ------------------------------------------------------------------
    // Debug
    // ------------------------------------------------------------------

    // Tools
    // Presently
    // This
    // We
    bool        ConfigDebugBeginReturnValueOnce;// false
    bool        ConfigDebugBeginReturnValueLoop;// false

    // Option
    // Backends
    // Consider
    bool        ConfigDebugIgnoreFocusLoss;     // false

    // Options
    bool        ConfigDebugIniSettings;         // false

    // ------------------------------------------------------------------
    // Platform
    // the
    // ------------------------------------------------------------------

    // Optional
    const char* BackendPlatformName;            // NULL
    const char* BackendRendererName;            // NULL
    void*       BackendPlatformUserData;        // NULL
    void*       BackendRendererUserData;        // NULL
    void*       BackendLanguageUserData;        // NULL

    // Optional
    // default
    const char* (*GetClipboardTextFn)(void* user_data);
    void        (*SetClipboardTextFn)(void* user_data, const char* text);
    void*       ClipboardUserData;

    // Optional
    // default
    void        (*SetPlatformImeDataFn)(ImGuiViewport* viewport, ImGuiPlatformImeData* data);
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    void*       ImeWindowHandle;                // NULL
#else
    void*       _UnusedPadding;                                     // Unused
#endif

    // Optional
    ImWchar     PlatformLocaleDecimalPoint;     // .

    // ------------------------------------------------------------------
    // Input
    // ------------------------------------------------------------------

    // Input
    IMGUI_API void  AddKeyEvent(ImGuiKey key, bool down);                   // Queue
    IMGUI_API void  AddKeyAnalogEvent(ImGuiKey key, bool down, float v);    // Queue
    IMGUI_API void  AddMousePosEvent(float x, float y);                     // Queue
    IMGUI_API void  AddMouseButtonEvent(int button, bool down);             // Queue
    IMGUI_API void  AddMouseWheelEvent(float wheel_x, float wheel_y);       // Queue
    IMGUI_API void  AddMouseSourceEvent(ImGuiMouseSource source);           // Queue
    IMGUI_API void  AddFocusEvent(bool focused);                            // Queue
    IMGUI_API void  AddInputCharacter(unsigned int c);                      // Queue
    IMGUI_API void  AddInputCharacterUTF16(ImWchar16 c);                    // Queue
    IMGUI_API void  AddInputCharactersUTF8(const char* str);                // Queue

    IMGUI_API void  SetKeyEventNativeData(ImGuiKey key, int native_keycode, int native_scancode, int native_legacy_index = -1); // Optional
    IMGUI_API void  SetAppAcceptingEvents(bool accepting_events);           // Set
    IMGUI_API void  ClearEventsQueue();                                     // Clear
    IMGUI_API void  ClearInputKeys();                                       // Clear
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    IMGUI_API void  ClearInputCharacters();                                 // Obsolete
#endif

    // ------------------------------------------------------------------
    // Output
    // when
    // generally
    // ------------------------------------------------------------------

    bool        WantCaptureMouse;                   // Set
    bool        WantCaptureKeyboard;                // Set
    bool        WantTextInput;                      // Mobile
    bool        WantSetMousePos;                    // MousePos
    bool        WantSaveIniSettings;                // When
    bool        NavActive;                          // Keyboard
    bool        NavVisible;                         // Keyboard
    float       Framerate;                          // Estimate
    int         MetricsRenderVertices;              // Vertices
    int         MetricsRenderIndices;               // Indices
    int         MetricsRenderWindows;               // Number
    int         MetricsActiveWindows;               // Number
    int         MetricsActiveAllocations;           // Number
    ImVec2      MouseDelta;                         // Mouse

    // Legacy
    // This
    // Old
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    int         KeyMap[ImGuiKey_COUNT];             // LEGACY
    bool        KeysDown[ImGuiKey_COUNT];           // LEGACY
    float       NavInputs[ImGuiNavInput_COUNT];     // LEGACY
#endif

    // ------------------------------------------------------------------
    // Internal
    // ------------------------------------------------------------------

    ImGuiContext* Ctx;                              // Parent

    // Main
    // this
    // reading
    ImVec2      MousePos;                           // Mouse
    bool        MouseDown[5];                       // Mouse
    float       MouseWheel;                         // Mouse
    float       MouseWheelH;                        // Mouse
    ImGuiMouseSource MouseSource;                   // Mouse
    bool        KeyCtrl;                            // Keyboard
    bool        KeyShift;                           // Keyboard
    bool        KeyAlt;                             // Keyboard
    bool        KeySuper;                           // Keyboard

    // Other
    ImGuiKeyChord KeyMods;                          // Key
    ImGuiKeyData  KeysData[ImGuiKey_KeysData_SIZE]; // Key
    bool        WantCaptureMouseUnlessPopupClose;   // Alternative
    ImVec2      MousePosPrev;                       // Previous
    ImVec2      MouseClickedPos[5];                 // Position
    double      MouseClickedTime[5];                // Time
    bool        MouseClicked[5];                    // Mouse
    bool        MouseDoubleClicked[5];              // Has
    ImU16       MouseClickedCount[5];               // 0
    ImU16       MouseClickedLastCount[5];           // Count
    bool        MouseReleased[5];                   // Mouse
    bool        MouseDownOwned[5];                  // Track
    bool        MouseDownOwnedUnlessPopupClose[5];  // Track
    bool        MouseWheelRequestAxisSwap;          // On
    float       MouseDownDuration[5];               // Duration
    float       MouseDownDurationPrev[5];           // Previous
    float       MouseDragMaxDistanceSqr[5];         // Squared
    float       PenPressure;                        // Touch
    bool        AppFocusLost;                       // Only
    bool        AppAcceptingEvents;                 // Only
    ImS8        BackendUsingLegacyKeyArrays;        // -1
    bool        BackendUsingLegacyNavInputArray;    // 0
    ImWchar16   InputQueueSurrogate;                // For
    ImVector<ImWchar> InputQueueCharacters;         // Queue

    IMGUI_API   ImGuiIO();
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Shared
// The
// Callbacks
// -
// -
// -
// -
// -
// -
struct ImGuiInputTextCallbackData
{
    ImGuiContext*       Ctx;            // Parent
    ImGuiInputTextFlags EventFlag;      // One
    ImGuiInputTextFlags Flags;          // What
    void*               UserData;       // What

    // Arguments
    // -
    // -
    ImWchar             EventChar;      // Character
    ImGuiKey            EventKey;       // Key
    char*               Buf;            // Text
    int                 BufTextLen;     // Text
    int                 BufSize;        // Buffer
    bool                BufDirty;       // Set
    int                 CursorPos;      // Read-write
    int                 SelectionStart; // Read-write
    int                 SelectionEnd;   // Read-write

    // Helper
    // Use
    IMGUI_API ImGuiInputTextCallbackData();
    IMGUI_API void      DeleteChars(int pos, int bytes_count);
    IMGUI_API void      InsertChars(int pos, const char* text, const char* text_end = NULL);
    void                SelectAll()             { SelectionStart = 0; SelectionEnd = BufTextLen; }
    void                ClearSelection()        { SelectionStart = SelectionEnd = BufTextLen; }
    bool                HasSelection() const    { return SelectionStart != SelectionEnd; }
};

// Resizing
// NB
struct ImGuiSizeCallbackData
{
    void*   UserData;       // Read-only.
    ImVec2  Pos;            // Read-only.
    ImVec2  CurrentSize;    // Read-only.
    ImVec2  DesiredSize;    // Read-write.
};

// Data
struct ImGuiPayload
{
    // Members
    void*           Data;               // Data
    int             DataSize;           // Data

    // Internal
    ImGuiID         SourceId;           // Source
    ImGuiID         SourceParentId;     // Source
    int             DataFrameCount;     // Data
    char            DataType[32 + 1];   // Data
    bool            Preview;            // Set
    bool            Delivery;           // Set

    ImGuiPayload()  { Clear(); }
    void Clear()    { SourceId = SourceParentId = 0; Data = NULL; DataSize = 0; memset(DataType, 0, sizeof(DataType)); DataFrameCount = -1; Preview = Delivery = false; }
    bool IsDataType(const char* type) const { return DataFrameCount != -1 && strcmp(type, DataType) == 0; }
    bool IsPreview() const                  { return Preview; }
    bool IsDelivery() const                 { return Delivery; }
};

// Sorting
struct ImGuiTableColumnSortSpecs
{
    ImGuiID                     ColumnUserID;       // User
    ImS16                       ColumnIndex;        // Index
    ImS16                       SortOrder;          // Index
    ImGuiSortDirection          SortDirection : 8;  // ImGuiSortDirection_Ascending

    ImGuiTableColumnSortSpecs() { memset(this, 0, sizeof(*this)); }
};

// Sorting
// Obtained
// When
// Make
struct ImGuiTableSortSpecs
{
    const ImGuiTableColumnSortSpecs* Specs;     // Pointer
    int                         SpecsCount;     // Sort
    bool                        SpecsDirty;     // Set

    ImGuiTableSortSpecs()       { memset(this, 0, sizeof(*this)); }
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Helper
#define IM_UNICODE_CODEPOINT_INVALID 0xFFFD     // Invalid
#ifdef IMGUI_USE_WCHAR32
#define IM_UNICODE_CODEPOINT_MAX     0x10FFFF   // Maximum
#else
#define IM_UNICODE_CODEPOINT_MAX     0xFFFF     // Maximum
#endif

// Helper
// Usage
struct ImGuiOnceUponAFrame
{
    ImGuiOnceUponAFrame() { RefFrame = -1; }
    mutable int RefFrame;
    operator bool() const { int current_frame = ImGui::GetFrameCount(); if (RefFrame == current_frame) return false; RefFrame = current_frame; return true; }
};

// Helper
struct ImGuiTextFilter
{
    IMGUI_API           ImGuiTextFilter(const char* default_filter = "");
    IMGUI_API bool      Draw(const char* label = "Filter (inc,-exc)", float width = 0.0f);  // Helper
    IMGUI_API bool      PassFilter(const char* text, const char* text_end = NULL) const;
    IMGUI_API void      Build();
    void                Clear()          { InputBuf[0] = 0; Build(); }
    bool                IsActive() const { return !Filters.empty(); }

    // Internal
    struct ImGuiTextRange
    {
        const char*     b;
        const char*     e;

        ImGuiTextRange()                                { b = e = NULL; }
        ImGuiTextRange(const char* _b, const char* _e)  { b = _b; e = _e; }
        bool            empty() const                   { return b == e; }
        IMGUI_API void  split(char separator, ImVector<ImGuiTextRange>* out) const;
    };
    char                    InputBuf[256];
    ImVector<ImGuiTextRange>Filters;
    int                     CountGrep;
};

// Helper
// this
struct ImGuiTextBuffer
{
    ImVector<char>      Buf;
    IMGUI_API static char EmptyString[1];

    ImGuiTextBuffer()   { }
    inline char         operator[](int i) const { IM_ASSERT(Buf.Data != NULL); return Buf.Data[i]; }
    const char*         begin() const           { return Buf.Data ? &Buf.front() : EmptyString; }
    const char*         end() const             { return Buf.Data ? &Buf.back() : EmptyString; }   // Buf
    int                 size() const            { return Buf.Size ? Buf.Size - 1 : 0; }
    bool                empty() const           { return Buf.Size <= 1; }
    void                clear()                 { Buf.clear(); }
    void                reserve(int capacity)   { Buf.reserve(capacity); }
    const char*         c_str() const           { return Buf.Data ? Buf.Data : EmptyString; }
    IMGUI_API void      append(const char* str, const char* str_end = NULL);
    IMGUI_API void      appendf(const char* fmt, ...) IM_FMTARGS(2);
    IMGUI_API void      appendfv(const char* fmt, va_list args) IM_FMTLIST(2);
};

// Helper
// Typically
// We
// This
// You
// -
// -
// Types
struct ImGuiStorage
{
    // Internal
    struct ImGuiStoragePair
    {
        ImGuiID key;
        union { int val_i; float val_f; void* val_p; };
        ImGuiStoragePair(ImGuiID _key, int _val_i)      { key = _key; val_i = _val_i; }
        ImGuiStoragePair(ImGuiID _key, float _val_f)    { key = _key; val_f = _val_f; }
        ImGuiStoragePair(ImGuiID _key, void* _val_p)    { key = _key; val_p = _val_p; }
    };

    ImVector<ImGuiStoragePair>      Data;

    // -
    // -
    // -
    void                Clear() { Data.clear(); }
    IMGUI_API int       GetInt(ImGuiID key, int default_val = 0) const;
    IMGUI_API void      SetInt(ImGuiID key, int val);
    IMGUI_API bool      GetBool(ImGuiID key, bool default_val = false) const;
    IMGUI_API void      SetBool(ImGuiID key, bool val);
    IMGUI_API float     GetFloat(ImGuiID key, float default_val = 0.0f) const;
    IMGUI_API void      SetFloat(ImGuiID key, float val);
    IMGUI_API void*     GetVoidPtr(ImGuiID key) const; // default_val
    IMGUI_API void      SetVoidPtr(ImGuiID key, void* val);

    // -
    // -
    // -
    // float
    IMGUI_API int*      GetIntRef(ImGuiID key, int default_val = 0);
    IMGUI_API bool*     GetBoolRef(ImGuiID key, bool default_val = false);
    IMGUI_API float*    GetFloatRef(ImGuiID key, float default_val = 0.0f);
    IMGUI_API void**    GetVoidPtrRef(ImGuiID key, void* default_val = NULL);

    // Use
    IMGUI_API void      SetAllInt(int val);

    // For
    IMGUI_API void      BuildSortByKey();
};

// Helper
// If
// clipping
// The
// Dear
// fetching
// scale
// Usage
// ImGuiListClipper
// clipper.Begin
// while
// for
// ImGui
// Generally
// -
// -
// -
// -
// -
// -
struct ImGuiListClipper
{
    ImGuiContext*   Ctx;                // Parent
    int             DisplayStart;       // First
    int             DisplayEnd;         // End
    int             ItemsCount;         // Internal
    float           ItemsHeight;        // Internal
    float           StartPosY;          // Internal
    void*           TempData;           // Internal

    // items_count
    // items_height
    IMGUI_API ImGuiListClipper();
    IMGUI_API ~ImGuiListClipper();
    IMGUI_API void  Begin(int items_count, float items_height = -1.0f);
    IMGUI_API void  End();             // Automatically
    IMGUI_API bool  Step();            // Call

    // Call
    // Due
    inline void     IncludeItemByIndex(int item_index)                  { IncludeItemsByIndex(item_index, item_index + 1); }
    IMGUI_API void  IncludeItemsByIndex(int item_begin, int item_end);  // item_end

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    inline void IncludeRangeByIndices(int item_begin, int item_end)      { IncludeItemsByIndex(item_begin, item_end); } // renamed
    inline void ForceDisplayRangeByIndices(int item_begin, int item_end) { IncludeItemsByIndex(item_begin, item_end); } // renamed
    // inline
#endif
};

// Helpers
// -
// -
// -
#ifdef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS_IMPLEMENTED
IM_MSVC_RUNTIME_CHECKS_OFF
static inline ImVec2  operator*(const ImVec2& lhs, const float rhs)     { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
static inline ImVec2  operator/(const ImVec2& lhs, const float rhs)     { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2  operator*(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2  operator/(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2  operator-(const ImVec2& lhs)                      { return ImVec2(-lhs.x, -lhs.y); }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs)          { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs)          { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
static inline ImVec4  operator+(const ImVec4& lhs, const ImVec4& rhs)   { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
static inline ImVec4  operator-(const ImVec4& lhs, const ImVec4& rhs)   { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
static inline ImVec4  operator*(const ImVec4& lhs, const ImVec4& rhs)   { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif

// Helpers
// User
#ifndef IM_COL32_R_SHIFT
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define IM_COL32_R_SHIFT    16
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    0
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#else
#define IM_COL32_R_SHIFT    0
#define IM_COL32_G_SHIFT    8
#define IM_COL32_B_SHIFT    16
#define IM_COL32_A_SHIFT    24
#define IM_COL32_A_MASK     0xFF000000
#endif
#endif
#define IM_COL32(R,G,B,A)    (((ImU32)(A)<<IM_COL32_A_SHIFT) | ((ImU32)(B)<<IM_COL32_B_SHIFT) | ((ImU32)(G)<<IM_COL32_G_SHIFT) | ((ImU32)(R)<<IM_COL32_R_SHIFT))
#define IM_COL32_WHITE       IM_COL32(255,255,255,255)  // Opaque
#define IM_COL32_BLACK       IM_COL32(0,0,0,255)        // Opaque
#define IM_COL32_BLACK_TRANS IM_COL32(0,0,0,0)          // Transparent

// Helper
// Prefer
// Avoid
// None
struct ImColor
{
    ImVec4          Value;

    constexpr ImColor()                                             { }
    constexpr ImColor(float r, float g, float b, float a = 1.0f)    : Value(r, g, b, a) { }
    constexpr ImColor(const ImVec4& col)                            : Value(col) {}
    constexpr ImColor(int r, int g, int b, int a = 255)             : Value((float)r * (1.0f / 255.0f), (float)g * (1.0f / 255.0f), (float)b * (1.0f / 255.0f), (float)a* (1.0f / 255.0f)) {}
    constexpr ImColor(ImU32 rgba)                                   : Value((float)((rgba >> IM_COL32_R_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> IM_COL32_G_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> IM_COL32_B_SHIFT) & 0xFF) * (1.0f / 255.0f), (float)((rgba >> IM_COL32_A_SHIFT) & 0xFF) * (1.0f / 255.0f)) {}
    inline operator ImU32() const                                   { return ImGui::ColorConvertFloat4ToU32(Value); }
    inline operator ImVec4() const                                  { return Value; }

    // FIXME-OBSOLETE
    inline void    SetHSV(float h, float s, float v, float a = 1.0f){ ImGui::ColorConvertHSVtoRGB(h, s, v, Value.x, Value.y, Value.z); Value.w = a; }
    static ImColor HSV(float h, float s, float v, float a = 1.0f)   { float r, g, b; ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b); return ImColor(r, g, b, a); }
};

// -----------------------------------------------------------------------------
// SECTION
// Hold
// -----------------------------------------------------------------------------

// The
#ifndef IM_DRAWLIST_TEX_LINES_WIDTH_MAX
#define IM_DRAWLIST_TEX_LINES_WIDTH_MAX     (63)
#endif

// ImDrawCallback
// NB
// you
// A
// B
// The
// If
#ifndef ImDrawCallback
typedef void (*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);
#endif

// Special
// The
// This
// It
#define ImDrawCallback_ResetRenderState     (ImDrawCallback)(-1)

// Typically
// -
// this
// Backends
// -
struct ImDrawCmd
{
    ImVec4          ClipRect;           // 4
    ImTextureID     TextureId;          // 4-8
    unsigned int    VtxOffset;          // 4
    unsigned int    IdxOffset;          // 4
    unsigned int    ElemCount;          // 4
    ImDrawCallback  UserCallback;       // 4-8
    void*           UserCallbackData;   // 4-8

    ImDrawCmd() { memset(this, 0, sizeof(*this)); } // Also

    // Since
    inline ImTextureID GetTexID() const { return TextureId; }
};

// Vertex
#ifndef IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT
struct ImDrawVert
{
    ImVec2  pos;
    ImVec2  uv;
    ImU32   col;
};
#else
// You
// The
// The
// NOTE
IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT;
#endif

// Internal
struct ImDrawCmdHeader
{
    ImVec4          ClipRect;
    ImTextureID     TextureId;
    unsigned int    VtxOffset;
};

// Internal
struct ImDrawChannel
{
    ImVector<ImDrawCmd>         _CmdBuffer;
    ImVector<ImDrawIdx>         _IdxBuffer;
};


// Split
// This
struct ImDrawListSplitter
{
    int                         _Current;    // Current
    int                         _Count;      // Number
    ImVector<ImDrawChannel>     _Channels;   // Draw

    inline ImDrawListSplitter()  { memset(this, 0, sizeof(*this)); }
    inline ~ImDrawListSplitter() { ClearFreeMemory(); }
    inline void                 Clear() { _Current = 0; _Count = 1; } // Do
    IMGUI_API void              ClearFreeMemory();
    IMGUI_API void              Split(ImDrawList* draw_list, int count);
    IMGUI_API void              Merge(ImDrawList* draw_list);
    IMGUI_API void              SetCurrentChannel(ImDrawList* draw_list, int channel_idx);
};

// Flags
// Legacy
enum ImDrawFlags_
{
    ImDrawFlags_None                        = 0,
    ImDrawFlags_Closed                      = 1 << 0, // PathStroke
    ImDrawFlags_RoundCornersTopLeft         = 1 << 4, // AddRect
    ImDrawFlags_RoundCornersTopRight        = 1 << 5, // AddRect
    ImDrawFlags_RoundCornersBottomLeft      = 1 << 6, // AddRect
    ImDrawFlags_RoundCornersBottomRight     = 1 << 7, // AddRect
    ImDrawFlags_RoundCornersNone            = 1 << 8, // AddRect
    ImDrawFlags_RoundCornersTop             = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersBottom          = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersLeft            = ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersTopLeft,
    ImDrawFlags_RoundCornersRight           = ImDrawFlags_RoundCornersBottomRight | ImDrawFlags_RoundCornersTopRight,
    ImDrawFlags_RoundCornersAll             = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight,
    ImDrawFlags_RoundCornersDefault_        = ImDrawFlags_RoundCornersAll, // Default
    ImDrawFlags_RoundCornersMask_           = ImDrawFlags_RoundCornersAll | ImDrawFlags_RoundCornersNone,
};

// Flags
// It
enum ImDrawListFlags_
{
    ImDrawListFlags_None                    = 0,
    ImDrawListFlags_AntiAliasedLines        = 1 << 0,  // Enable
    ImDrawListFlags_AntiAliasedLinesUseTex  = 1 << 1,  // Enable
    ImDrawListFlags_AntiAliasedFill         = 1 << 2,  // Enable
    ImDrawListFlags_AllowVtxOffset          = 1 << 3,  // Can
};

// Draw
// This
// all
// Each
// access
// You
// In
// You
// Important
struct ImDrawList
{
    // This
    ImVector<ImDrawCmd>     CmdBuffer;          // Draw
    ImVector<ImDrawIdx>     IdxBuffer;          // Index
    ImVector<ImDrawVert>    VtxBuffer;          // Vertex
    ImDrawListFlags         Flags;              // Flags

    // Internal
    unsigned int            _VtxCurrentIdx;     // Internal
    ImDrawListSharedData*   _Data;              // Pointer
    const char*             _OwnerName;         // Pointer
    ImDrawVert*             _VtxWritePtr;       // Internal
    ImDrawIdx*              _IdxWritePtr;       // Internal
    ImVector<ImVec4>        _ClipRectStack;     // Internal
    ImVector<ImTextureID>   _TextureIdStack;    // Internal
    ImVector<ImVec2>        _Path;              // Internal
    ImDrawCmdHeader         _CmdHeader;         // Internal
    ImDrawListSplitter      _Splitter;          // Internal
    float                   _FringeScale;       // Internal

    // If
    ImDrawList(ImDrawListSharedData* shared_data) { memset(this, 0, sizeof(*this)); _Data = shared_data; }

    ~ImDrawList() { _ClearFreeMemory(); }
    IMGUI_API void  PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect = false);  // Render-level
    IMGUI_API void  PushClipRectFullScreen();
    IMGUI_API void  PopClipRect();
    IMGUI_API void  PushTextureID(ImTextureID texture_id);
    IMGUI_API void  PopTextureID();
    inline ImVec2   GetClipRectMin() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.x, cr.y); }
    inline ImVec2   GetClipRectMax() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.z, cr.w); }

    // Primitives
    // -
    // -
    // -
    // In
    // In
    // Use
    IMGUI_API void  AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);   // a
    IMGUI_API void  AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0);                     // a
    IMGUI_API void  AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
    IMGUI_API void  AddQuad(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddQuadFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col);
    IMGUI_API void  AddTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.0f);
    IMGUI_API void  AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);
    IMGUI_API void  AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.0f);
    IMGUI_API void  AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments = 0);
    IMGUI_API void  AddNgon(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness = 1.0f);
    IMGUI_API void  AddNgonFilled(const ImVec2& center, float radius, ImU32 col, int num_segments);
    IMGUI_API void  AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
    IMGUI_API void  AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
    IMGUI_API void  AddPolyline(const ImVec2* points, int num_points, ImU32 col, ImDrawFlags flags, float thickness);
    IMGUI_API void  AddConvexPolyFilled(const ImVec2* points, int num_points, ImU32 col);
    IMGUI_API void  AddBezierCubic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments = 0); // Cubic
    IMGUI_API void  AddBezierQuadratic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness, int num_segments = 0);               // Quadratic

    // Image
    // -
    // -
    // -
    IMGUI_API void  AddImage(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE);
    IMGUI_API void  AddImageQuad(ImTextureID user_texture_id, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& uv1 = ImVec2(0, 0), const ImVec2& uv2 = ImVec2(1, 0), const ImVec2& uv3 = ImVec2(1, 1), const ImVec2& uv4 = ImVec2(0, 1), ImU32 col = IM_COL32_WHITE);
    IMGUI_API void  AddImageRounded(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col, float rounding, ImDrawFlags flags = 0);

    // Stateful
    // -
    inline    void  PathClear()                                                 { _Path.Size = 0; }
    inline    void  PathLineTo(const ImVec2& pos)                               { _Path.push_back(pos); }
    inline    void  PathLineToMergeDuplicate(const ImVec2& pos)                 { if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0) _Path.push_back(pos); }
    inline    void  PathFillConvex(ImU32 col)                                   { AddConvexPolyFilled(_Path.Data, _Path.Size, col); _Path.Size = 0; }
    inline    void  PathStroke(ImU32 col, ImDrawFlags flags = 0, float thickness = 1.0f) { AddPolyline(_Path.Data, _Path.Size, col, flags, thickness); _Path.Size = 0; }
    IMGUI_API void  PathArcTo(const ImVec2& center, float radius, float a_min, float a_max, int num_segments = 0);
    IMGUI_API void  PathArcToFast(const ImVec2& center, float radius, int a_min_of_12, int a_max_of_12);                // Use
    IMGUI_API void  PathBezierCubicCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments = 0); // Cubic
    IMGUI_API void  PathBezierQuadraticCurveTo(const ImVec2& p2, const ImVec2& p3, int num_segments = 0);               // Quadratic
    IMGUI_API void  PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, ImDrawFlags flags = 0);

    // Advanced
    IMGUI_API void  AddCallback(ImDrawCallback callback, void* callback_data);  // Your
    IMGUI_API void  AddDrawCmd();                                               // This
    IMGUI_API ImDrawList* CloneOutput() const;                                  // Create

    // Advanced
    // -
    // -
    // -
    // Prefer
    // Using
    inline void     ChannelsSplit(int count)    { _Splitter.Split(this, count); }
    inline void     ChannelsMerge()             { _Splitter.Merge(this); }
    inline void     ChannelsSetCurrent(int n)   { _Splitter.SetCurrentChannel(this, n); }

    // Advanced
    // -
    // -
    IMGUI_API void  PrimReserve(int idx_count, int vtx_count);
    IMGUI_API void  PrimUnreserve(int idx_count, int vtx_count);
    IMGUI_API void  PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);      // Axis
    IMGUI_API void  PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
    IMGUI_API void  PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
    inline    void  PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)    { _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
    inline    void  PrimWriteIdx(ImDrawIdx idx)                                     { *_IdxWritePtr = idx; _IdxWritePtr++; }
    inline    void  PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col)         { PrimWriteIdx((ImDrawIdx)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); } // Write

    // Obsolete
    // inline
    // inline

    // Internal
    IMGUI_API void  _ResetForNewFrame();
    IMGUI_API void  _ClearFreeMemory();
    IMGUI_API void  _PopUnusedDrawCmd();
    IMGUI_API void  _TryMergeDrawCmds();
    IMGUI_API void  _OnChangedClipRect();
    IMGUI_API void  _OnChangedTextureID();
    IMGUI_API void  _OnChangedVtxOffset();
    IMGUI_API int   _CalcCircleAutoSegmentCount(float radius) const;
    IMGUI_API void  _PathArcToFastEx(const ImVec2& center, float radius, int a_min_sample, int a_max_sample, int a_step);
    IMGUI_API void  _PathArcToN(const ImVec2& center, float radius, float a_min, float a_max, int num_segments);
};

// All
// NB
// as
struct ImDrawData
{
    bool                Valid;              // Only
    int                 CmdListsCount;      // Number
    int                 TotalIdxCount;      // For
    int                 TotalVtxCount;      // For
    ImVector<ImDrawList*> CmdLists;         // Array
    ImVec2              DisplayPos;         // Top-left
    ImVec2              DisplaySize;        // Size
    ImVec2              FramebufferScale;   // Amount
    ImGuiViewport*      OwnerViewport;      // Viewport

    // Functions
    ImDrawData()    { Clear(); }
    IMGUI_API void  Clear();
    IMGUI_API void  AddDrawList(ImDrawList* draw_list);     // Helper
    IMGUI_API void  DeIndexAllBuffers();                    // Helper
    IMGUI_API void  ScaleClipRects(const ImVec2& fb_scale); // Helper
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

struct ImFontConfig
{
    void*           FontData;               // TTF
    int             FontDataSize;           // TTF
    bool            FontDataOwnedByAtlas;   // true
    int             FontNo;                 // 0
    float           SizePixels;             // Size
    int             OversampleH;            // 2
    int             OversampleV;            // 1
    bool            PixelSnapH;             // false
    ImVec2          GlyphExtraSpacing;      // 0
    ImVec2          GlyphOffset;            // 0
    const ImWchar*  GlyphRanges;            // NULL
    float           GlyphMinAdvanceX;       // 0
    float           GlyphMaxAdvanceX;       // FLT_MAX
    bool            MergeMode;              // false
    unsigned int    FontBuilderFlags;       // 0
    float           RasterizerMultiply;     // 1.0f
    ImWchar         EllipsisChar;           // -1

    // Internal
    char            Name[40];               // Name
    ImFont*         DstFont;

    IMGUI_API ImFontConfig();
};

// Hold
// Note
struct ImFontGlyph
{
    unsigned int    Colored : 1;        // Flag
    unsigned int    Visible : 1;        // Flag
    unsigned int    Codepoint : 30;     // 0x0000..0x10FFFF
    float           AdvanceX;           // Distance
    float           X0, Y0, X1, Y1;     // Glyph
    float           U0, V0, U1, V1;     // Texture
};

// Helper
// This
struct ImFontGlyphRangesBuilder
{
    ImVector<ImU32> UsedChars;            // Store

    ImFontGlyphRangesBuilder()              { Clear(); }
    inline void     Clear()                 { int size_in_bytes = (IM_UNICODE_CODEPOINT_MAX + 1) / 8; UsedChars.resize(size_in_bytes / (int)sizeof(ImU32)); memset(UsedChars.Data, 0, (size_t)size_in_bytes); }
    inline bool     GetBit(size_t n) const  { int off = (int)(n >> 5); ImU32 mask = 1u << (n & 31); return (UsedChars[off] & mask) != 0; }  // Get
    inline void     SetBit(size_t n)        { int off = (int)(n >> 5); ImU32 mask = 1u << (n & 31); UsedChars[off] |= mask; }               // Set
    inline void     AddChar(ImWchar c)      { SetBit(c); }                      // Add
    IMGUI_API void  AddText(const char* text, const char* text_end = NULL);     // Add
    IMGUI_API void  AddRanges(const ImWchar* ranges);                           // Add
    IMGUI_API void  BuildRanges(ImVector<ImWchar>* out_ranges);                 // Output
};

// See
struct ImFontAtlasCustomRect
{
    unsigned short  Width, Height;  // Input
    unsigned short  X, Y;           // Output
    unsigned int    GlyphID;        // Input
    float           GlyphAdvanceX;  // Input
    ImVec2          GlyphOffset;    // Input
    ImFont*         Font;           // Input
    ImFontAtlasCustomRect()         { Width = Height = 0; X = Y = 0xFFFF; GlyphID = 0; GlyphAdvanceX = 0.0f; GlyphOffset = ImVec2(0, 0); Font = NULL; }
    bool IsPacked() const           { return X != 0xFFFF; }
};

// Flags
enum ImFontAtlasFlags_
{
    ImFontAtlasFlags_None               = 0,
    ImFontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don
    ImFontAtlasFlags_NoMouseCursors     = 1 << 1,   // Don
    ImFontAtlasFlags_NoBakedLines       = 1 << 2,   // Don
};

// Load
// -
// -
// -
// It
// -
// -
// -
// -
// This
// Common
// -
// atlas
// -
// You
// -
// -
struct ImFontAtlas
{
    IMGUI_API ImFontAtlas();
    IMGUI_API ~ImFontAtlas();
    IMGUI_API ImFont*           AddFont(const ImFontConfig* font_cfg);
    IMGUI_API ImFont*           AddFontDefault(const ImFontConfig* font_cfg = NULL);
    IMGUI_API ImFont*           AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);
    IMGUI_API ImFont*           AddFontFromMemoryTTF(void* font_data, int font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // Note
    IMGUI_API ImFont*           AddFontFromMemoryCompressedTTF(const void* compressed_font_data, int compressed_font_size, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL); // compressed_font_data
    IMGUI_API ImFont*           AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85, float size_pixels, const ImFontConfig* font_cfg = NULL, const ImWchar* glyph_ranges = NULL);              // compressed_font_data_base85
    IMGUI_API void              ClearInputData();           // Clear
    IMGUI_API void              ClearTexData();             // Clear
    IMGUI_API void              ClearFonts();               // Clear
    IMGUI_API void              Clear();                    // Clear

    // Build
    // User
    // The
    // Building
    // the
    IMGUI_API bool              Build();                    // Build
    IMGUI_API void              GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 1
    IMGUI_API void              GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel = NULL);  // 4
    bool                        IsBuilt() const             { return Fonts.Size > 0 && TexReady; } // Bit
    void                        SetTexID(ImTextureID id)    { TexID = id; }

    // -------------------------------------------
    // Glyph
    // -------------------------------------------

    // Helpers
    // NB
    // Read
    // NB
    IMGUI_API const ImWchar*    GetGlyphRangesDefault();                // Basic
    IMGUI_API const ImWchar*    GetGlyphRangesGreek();                  // Default
    IMGUI_API const ImWchar*    GetGlyphRangesKorean();                 // Default
    IMGUI_API const ImWchar*    GetGlyphRangesJapanese();               // Default
    IMGUI_API const ImWchar*    GetGlyphRangesChineseFull();            // Default
    IMGUI_API const ImWchar*    GetGlyphRangesChineseSimplifiedCommon();// Default
    IMGUI_API const ImWchar*    GetGlyphRangesCyrillic();               // Default
    IMGUI_API const ImWchar*    GetGlyphRangesThai();                   // Default
    IMGUI_API const ImWchar*    GetGlyphRangesVietnamese();             // Default

    // -------------------------------------------
    // BETA
    // -------------------------------------------

    // You
    // -
    // -
    // -
    // so
    // -
    // -
    IMGUI_API int               AddCustomRectRegular(int width, int height);
    IMGUI_API int               AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset = ImVec2(0, 0));
    ImFontAtlasCustomRect*      GetCustomRectByIndex(int index) { IM_ASSERT(index >= 0); return &CustomRects[index]; }

    // Internal
    IMGUI_API void              CalcCustomRectUV(const ImFontAtlasCustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max) const;
    IMGUI_API bool              GetMouseCursorTexData(ImGuiMouseCursor cursor, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2]);

    // -------------------------------------------
    // Members
    // -------------------------------------------

    ImFontAtlasFlags            Flags;              // Build
    ImTextureID                 TexID;              // User
    int                         TexDesiredWidth;    // Texture
    int                         TexGlyphPadding;    // Padding
    bool                        Locked;             // Marked
    void*                       UserData;           // Store

    // Internal
    // NB
    bool                        TexReady;           // Set
    bool                        TexPixelsUseColors; // Tell
    unsigned char*              TexPixelsAlpha8;    // 1
    unsigned int*               TexPixelsRGBA32;    // 4
    int                         TexWidth;           // Texture
    int                         TexHeight;          // Texture
    ImVec2                      TexUvScale;         // 1.0f
    ImVec2                      TexUvWhitePixel;    // Texture
    ImVector<ImFont*>           Fonts;              // Hold
    ImVector<ImFontAtlasCustomRect> CustomRects;    // Rectangles
    ImVector<ImFontConfig>      ConfigData;         // Configuration
    ImVec4                      TexUvLines[IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1];  // UVs

    // Internal
    const ImFontBuilderIO*      FontBuilderIO;      // Opaque
    unsigned int                FontBuilderFlags;   // Shared

    // Internal
    int                         PackIdMouseCursors; // Custom
    int                         PackIdLines;        // Custom

    // Obsolete
    // typedef
    // typedef
};

// Font
// ImFontAtlas
struct ImFont
{
    // Members
    ImVector<float>             IndexAdvanceX;      // 12-16
    float                       FallbackAdvanceX;   // 4
    float                       FontSize;           // 4

    // Members
    ImVector<ImWchar>           IndexLookup;        // 12-16
    ImVector<ImFontGlyph>       Glyphs;             // 12-16
    const ImFontGlyph*          FallbackGlyph;      // 4-8

    // Members
    ImFontAtlas*                ContainerAtlas;     // 4-8
    const ImFontConfig*         ConfigData;         // 4-8
    short                       ConfigDataCount;    // 2
    ImWchar                     FallbackChar;       // 2
    ImWchar                     EllipsisChar;       // 2
    short                       EllipsisCharCount;  // 1
    float                       EllipsisWidth;      // 4
    float                       EllipsisCharStep;   // 4
    bool                        DirtyLookupTables;  // 1
    float                       Scale;              // 4
    float                       Ascent, Descent;    // 4
    int                         MetricsTotalSurface;// 4
    ImU8                        Used4kPagesMap[(IM_UNICODE_CODEPOINT_MAX+1)/4096/8]; // 2

    // Methods
    IMGUI_API ImFont();
    IMGUI_API ~ImFont();
    IMGUI_API const ImFontGlyph*FindGlyph(ImWchar c) const;
    IMGUI_API const ImFontGlyph*FindGlyphNoFallback(ImWchar c) const;
    float                       GetCharAdvance(ImWchar c) const     { return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; }
    bool                        IsLoaded() const                    { return ContainerAtlas != NULL; }
    const char*                 GetDebugName() const                { return ConfigData ? ConfigData->Name : "<unknown>"; }

    // max_width
    // wrap_width
    IMGUI_API ImVec2            CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8
    IMGUI_API const char*       CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
    IMGUI_API void              RenderChar(ImDrawList* draw_list, float size, const ImVec2& pos, ImU32 col, ImWchar c) const;
    IMGUI_API void              RenderText(ImDrawList* draw_list, float size, const ImVec2& pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;

    // Internal
    IMGUI_API void              BuildLookupTable();
    IMGUI_API void              ClearOutputData();
    IMGUI_API void              GrowIndex(int new_size);
    IMGUI_API void              AddGlyph(const ImFontConfig* src_cfg, ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
    IMGUI_API void              AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst = true); // Makes
    IMGUI_API void              SetGlyphVisible(ImWchar c, bool visible);
    IMGUI_API bool              IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Flags
enum ImGuiViewportFlags_
{
    ImGuiViewportFlags_None                     = 0,
    ImGuiViewportFlags_IsPlatformWindow         = 1 << 0,   // Represent
    ImGuiViewportFlags_IsPlatformMonitor        = 1 << 1,   // Represent
    ImGuiViewportFlags_OwnedByApp               = 1 << 2,   // Platform
};

// -
// -
// -
// -
// -
// -
// -
struct ImGuiViewport
{
    ImGuiViewportFlags  Flags;                  // See
    ImVec2              Pos;                    // Main
    ImVec2              Size;                   // Main
    ImVec2              WorkPos;                // Work
    ImVec2              WorkSize;               // Work

    // Platform
    void*               PlatformHandleRaw;      // void

    ImGuiViewport()     { memset(this, 0, sizeof(*this)); }

    // Helpers
    ImVec2              GetCenter() const       { return ImVec2(Pos.x + Size.x * 0.5f, Pos.y + Size.y * 0.5f); }
    ImVec2              GetWorkCenter() const   { return ImVec2(WorkPos.x + WorkSize.x * 0.5f, WorkPos.y + WorkSize.y * 0.5f); }
};

// -----------------------------------------------------------------------------
// SECTION
// -----------------------------------------------------------------------------

// Optional
struct ImGuiPlatformImeData
{
    bool    WantVisible;        // A
    ImVec2  InputPos;           // Position
    float   InputLineHeight;    // Line

    ImGuiPlatformImeData() { memset(this, 0, sizeof(*this)); }
};

// -----------------------------------------------------------------------------
// SECTION
// Will
// Please
// -----------------------------------------------------------------------------

namespace ImGui
{
#ifndef IMGUI_DISABLE_OBSOLETE_KEYIO
    IMGUI_API ImGuiKey     GetKeyIndex(ImGuiKey key);  // map
#else
    static inline ImGuiKey GetKeyIndex(ImGuiKey key)   { IM_ASSERT(key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END && "ImGuiKey and native_index was merged together and native_index is disabled by IMGUI_DISABLE_OBSOLETE_KEYIO. Please switch to ImGuiKey."); return key; }
#endif
}

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
namespace ImGui
{
    // OBSOLETED
    IMGUI_API void      SetItemAllowOverlap();                                              // Use
    // OBSOLETED
    static inline void  PushAllowKeyboardFocus(bool tab_stop)                               { PushTabStop(tab_stop); }
    static inline void  PopAllowKeyboardFocus()                                             { PopTabStop(); }
    // OBSOLETED
    IMGUI_API bool      ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1)); // Use
    // OBSOLETED
    static inline void  CaptureKeyboardFromApp(bool want_capture_keyboard = true)           { SetNextFrameWantCaptureKeyboard(want_capture_keyboard); } // Renamed
    static inline void  CaptureMouseFromApp(bool want_capture_mouse = true)                 { SetNextFrameWantCaptureMouse(want_capture_mouse); }       // Renamed
    // OBSOLETED
    IMGUI_API void      CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end); // Calculate
    // OBSOLETED
    static inline float GetWindowContentRegionWidth()                                       { return GetWindowContentRegionMax().x - GetWindowContentRegionMin().x; }

    // Some
    // --
    // static
    // static
    // static
    // --
    // static
    // --
    // IMGUI_API
    // IMGUI_API
    // IMGUI_API
    // IMGUI_API
    // static
    // static
    // static
    // static
    // static
    // static
    // static
    // static
    // --
    // static
    // static
    // static
    // static
    // static
    // static
    // static
    // --
    // static
    // static
    // static
    // static
    // static
    // static
    // static
    // IMGUI_API
    // static
    // static
    // static
    // static
    // static
    // static
    // static
    // --
    // static
    // static
    // static
    // static
}

// OBSOLETED
typedef ImDrawFlags ImDrawCornerFlags;
enum ImDrawCornerFlags_
{
    ImDrawCornerFlags_None      = ImDrawFlags_RoundCornersNone,         // Was
    ImDrawCornerFlags_TopLeft   = ImDrawFlags_RoundCornersTopLeft,      // Was
    ImDrawCornerFlags_TopRight  = ImDrawFlags_RoundCornersTopRight,     // Was
    ImDrawCornerFlags_BotLeft   = ImDrawFlags_RoundCornersBottomLeft,   // Was
    ImDrawCornerFlags_BotRight  = ImDrawFlags_RoundCornersBottomRight,  // Was
    ImDrawCornerFlags_All       = ImDrawFlags_RoundCornersAll,          // Was
    ImDrawCornerFlags_Top       = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight,
    ImDrawCornerFlags_Bot       = ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight,
    ImDrawCornerFlags_Left      = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft,
    ImDrawCornerFlags_Right     = ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight,
};

// RENAMED
// RENAMED
typedef ImGuiKeyChord ImGuiModFlags;      // int.
enum ImGuiModFlags_ { ImGuiModFlags_None = 0, ImGuiModFlags_Ctrl = ImGuiMod_Ctrl, ImGuiModFlags_Shift = ImGuiMod_Shift, ImGuiModFlags_Alt = ImGuiMod_Alt, ImGuiModFlags_Super = ImGuiMod_Super };
// typedef
// enum

#endif // ifndef

// RENAMED
#if defined(IMGUI_DISABLE_METRICS_WINDOW) && !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && !defined(IMGUI_DISABLE_DEBUG_TOOLS)
#define IMGUI_DISABLE_DEBUG_TOOLS
#endif
#if defined(IMGUI_DISABLE_METRICS_WINDOW) && defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
#error IMGUI_DISABLE_METRICS_WINDOW was renamed to IMGUI_DISABLE_DEBUG_TOOLS, please use new name.
#endif

// -----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

// Include
#ifdef IMGUI_INCLUDE_IMGUI_USER_H
#include "imgui_user.h"
#endif

#endif // ifndef
