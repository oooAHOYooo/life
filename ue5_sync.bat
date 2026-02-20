@echo off
echo =======================================
echo Syncing P2G4W from Git...
echo =======================================

cd /d "%~dp0"
git pull

echo.
echo =======================================
echo Launching P2G4W (Unreal Engine 5)
echo =======================================
cd projects\CPPd1
start CPPd1.uproject

echo Done! If asked to rebuild modules, click "Yes".
pause
