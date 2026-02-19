@echo off
REM Quick sync script - syncs UE5 project to life repo
REM Run this after making changes in UE5

echo ========================================
echo Syncing CPPd1 from UE5 to Life Repo
echo ========================================
echo.

cd /d "%~dp0"
powershell -ExecutionPolicy Bypass -File "sync-ue5-to-life.ps1" -Commit

echo.
echo ========================================
echo Sync complete!
echo ========================================
echo.
pause
