@echo off

:: 设置项目路径
set PROJECT_DIR=%~dp0
set SOLUTION_PATH=%PROJECT_DIR%GTA5_DMA\GTA5_DMA.sln
set BUILD_CONFIGURATION=Release

:: 检查解决方案文件是否存在
if not exist "%SOLUTION_PATH%" (
    echo 错误：解决方案文件不存在：%SOLUTION_PATH%
    echo 请检查项目目录结构是否正确
    pause
    exit /b 1
)

echo ===================================
echo GTA5 DMA Tool - 自动编译脚本
echo ===================================
echo.
echo 项目目录：%PROJECT_DIR%
echo 解决方案：%SOLUTION_PATH%
echo 编译配置：%BUILD_CONFIGURATION%
echo.

:: 查找 MSBuild 路径
echo 正在查找 MSBuild...

:: 尝试不同的 MSBuild 路径
set MSBUILD_PATHS=
set MSBUILD_PATHS=%MSBUILD_PATHS%;"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set MSBUILD_PATHS=%MSBUILD_PATHS%;"C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
set MSBUILD_PATHS=%MSBUILD_PATHS%;"C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
set MSBUILD_PATHS=%MSBUILD_PATHS%;"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
set MSBUILD_PATHS=%MSBUILD_PATHS%;"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe"

set MSBUILD_EXE=
for %%p in (%MSBUILD_PATHS%) do (
    if exist %%p (
        set MSBUILD_EXE=%%p
        goto :msbuild_found
    )
)

:msbuild_found
if "%MSBUILD_EXE%"=="" (
    echo 错误：未找到 MSBuild.exe
    echo 请确保已安装 Visual Studio 2019 或 2022
    pause
    exit /b 1
)

echo 找到 MSBuild：%MSBUILD_EXE%
echo.

:: 查找 NuGet 路径
set NUGET_EXE=
if exist "%PROJECT_DIR%.nuget\nuget.exe" (
    set NUGET_EXE="%PROJECT_DIR%.nuget\nuget.exe"
) else if exist "%PROGRAMFILES%\NuGet\nuget.exe" (
    set NUGET_EXE="%PROGRAMFILES%\NuGet\nuget.exe"
) else (
    echo 警告：未找到 NuGet.exe，将尝试使用 MSBuild 自动恢复
)

echo.
echo ===================================
echo 开始编译过程...
echo ===================================
echo.

:: 切换到解决方案目录
cd /d "%PROJECT_DIR%GTA5_DMA"

:: 恢复 NuGet 包
if not "%NUGET_EXE%"=="" (
    echo 正在恢复 NuGet 包...
    %NUGET_EXE% restore "%SOLUTION_PATH%"
    if %errorlevel% neq 0 (
        echo 错误：NuGet 包恢复失败
        pause
        exit /b 1
    )
    echo NuGet 包恢复完成
    echo.
) else (
    echo 将使用 MSBuild 自动恢复 NuGet 包
    echo.
)

:: 编译解决方案
echo 正在编译解决方案...
echo 命令：%MSBUILD_EXE% /m /p:Configuration=%BUILD_CONFIGURATION% "%SOLUTION_PATH%"
echo.

%MSBUILD_EXE% /m /p:Configuration=%BUILD_CONFIGURATION% "%SOLUTION_PATH%"
if %errorlevel% equ 0 (
    echo.
    echo ===================================
    echo 编译成功！
    echo ===================================
    echo 编译输出目录：%PROJECT_DIR%GTA5_DMA\x64\%BUILD_CONFIGURATION%\
    echo.
    echo 可执行文件：%PROJECT_DIR%GTA5_DMA\x64\%BUILD_CONFIGURATION%\GTA5_DMA.exe
    echo.
) else (
    echo.
    echo ===================================
    echo 编译失败！
    echo ===================================
    echo 请检查编译错误信息
    echo.
)

:: 切换回原始目录
cd /d "%PROJECT_DIR%"

echo 按任意键退出...
pause >nul
