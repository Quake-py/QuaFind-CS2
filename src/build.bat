@echo off
title QUAFIND  - Build System
cd /d "%~dp0"

echo.
echo  ================================================
echo   QUAFIND   -  CS2 Internal
echo  ================================================
echo.

call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" > nul 2>&1
if errorlevel 1 (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" > nul 2>&1
)
if errorlevel 1 (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" > nul 2>&1
)
if errorlevel 1 (
    echo  [-] Visual Studio not found!
    pause & exit /b 1
)

echo  [+] VS Environment loaded
echo  [+] Compiling QuaFind.dll
echo.

cl.exe /nologo /EHa /MD /O2 /std:c++17 /W0 /LD ^
    dllmain.cpp ^
    imgui\imgui.cpp ^
    imgui\imgui_draw.cpp ^
    imgui\imgui_tables.cpp ^
    imgui\imgui_widgets.cpp ^
    imgui\imgui_impl_win32.cpp ^
    imgui\imgui_impl_dx11.cpp ^
    hook\src\buffer.c ^
    hook\src\hook.c ^
    hook\src\trampoline.c ^
    hook\src\hde\hde32.c ^
    hook\src\hde\hde64.c ^
    /Fe:QuaFind.dll ^
    /link /PDB:QuaFind.pdb ^
    user32.lib d3d11.lib dxgi.lib dwmapi.lib shell32.lib gdi32.lib ole32.lib windowscodecs.lib d3dcompiler.lib psapi.lib

if errorlevel 1 (
    echo.
    echo  ============================================
    echo   [-] BUILD FAILED
    echo  ============================================
    del *.obj
) else (
    echo.
    echo  ============================================
    echo   [+] SUCCESS: QuaFind.dll Built!
    echo  ============================================
    del *.obj
)

echo.
pause
