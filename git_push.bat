@echo off

:: 设置 Git 执行路径
set GIT_PATH="C:\Program Files\Git\bin\git.exe"

:: 检查 Git 是否可用
if not exist %GIT_PATH% (
    echo Git 未找到，请确保已安装 Git 并更新脚本中的 GIT_PATH
    pause
    exit /b 1
)

:: 切换到脚本所在目录
cd /d "%~dp0"

echo ===================================
echo GTA5 DMA Tool - Git 上传脚本
echo ===================================
echo.

:: 添加所有修改的文件
echo 正在添加所有修改的文件...
%GIT_PATH% add .
if %errorlevel% neq 0 (
    echo 添加文件失败
    pause
    exit /b 1
)

:: 提示用户输入提交信息
echo.
echo 请输入提交信息（不输入则使用默认信息）：
set /p COMMIT_MSG=提交信息：

:: 如果用户未输入提交信息，则使用默认信息
if "%COMMIT_MSG%"=="" (
    set COMMIT_MSG="Update files"
)

:: 提交代码
echo.
echo 正在提交代码...
%GIT_PATH% commit -m %COMMIT_MSG%
if %errorlevel% neq 0 (
    echo 提交失败，请检查是否有未配置的用户信息
    echo 可以运行以下命令配置：
    echo %GIT_PATH% config --local user.name "你的用户名"
    echo %GIT_PATH% config --local user.email "你的邮箱"
    pause
    exit /b 1
)

:: 推送到 GitHub
echo.
echo 正在推送到 GitHub...
%GIT_PATH% push origin main
if %errorlevel% equ 0 (
    echo.
    echo 推送成功！
    echo 仓库地址：https://github.com/fmc999/GTA5-DMA-Tool
) else (
    echo 推送失败，请检查网络连接或 GitHub 权限
)

echo.
echo 按任意键退出...
pause >nul
