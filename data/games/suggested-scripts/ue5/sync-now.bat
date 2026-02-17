@echo off
REM Quick sync script - run this after pulling the life repo
REM This syncs C++ files from life repo to UE5 project

echo ========================================
echo Syncing P2G4W scripts to CoPlago project
echo ========================================
echo.

cd /d "%~dp0"
powershell -ExecutionPolicy Bypass -File "sync-to-ue5.ps1"

echo.
echo ========================================
echo Sync complete!
echo ========================================
echo.
echo Next: Open UE5 and build (Ctrl+Alt+B)
pause
