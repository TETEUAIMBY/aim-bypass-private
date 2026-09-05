@echo off
REM Aim Bypass Private - FiveM Injector
REM Windows 10 Pro

color 05
title [AIM BYPASS PRIVATE] Injector v2.0
cls

echo.
echo ===============================================
echo   [AIM BYPASS PRIVATE] Injector v2.0
echo ===============================================
echo.

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERRO] Requer privilégios de Administrador!
    timeout /t 2 >nul
    powershell -Command "Start-Process cmd -Verb RunAs -ArgumentList '/c %0'"
    exit /b 1
)

setlocal enabledelayedexpansion
cd /d "%~dp0"
set "DLL_PATH=%CD%\..\bin\AimBypassPrivate.dll"
set "INJECTOR_PATH=%CD%\..\bin\injector.exe"

if not exist "!DLL_PATH!" (
    echo [ERRO] DLL nao encontrada
    pause
    exit /b 1
)

echo [*] Aguardando FiveM.exe...
echo [INFO] Abra o FiveM normalmente
echo.

:wait_fivem
tasklist /FI "IMAGENAME eq FiveM.exe" 2>NUL | find /I /N "FiveM.exe">NUL
if errorlevel 1 (
    echo [.] Aguardando...
    timeout /t 2 /nobreak >nul
    goto wait_fivem
)

echo.
echo [+] FiveM detectado!
echo [+] Injetando DLL...
echo.

"!INJECTOR_PATH!" "!DLL_PATH!"

if errorlevel 1 (
    echo.
    echo [ERRO] Falha na injecao
    pause
    exit /b 1
)

echo.
echo ===============================================
echo [SUCCESS] DLL injetada com sucesso!
echo ===============================================
echo.
echo [CONTROLES]
echo   INSERT = Abrir/Fechar Menu
echo   ESC    = Fechar Menu
echo.

:monitor
tasklist /FI "IMAGENAME eq FiveM.exe" 2>NUL | find /I /N "FiveM.exe">NUL
if errorlevel 1 (
    echo [!] FiveM foi fechado
    exit /b 0
)

timeout /t 1 /nobreak >nul
goto monitor
