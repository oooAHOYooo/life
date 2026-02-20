#!/bin/bash
echo "======================================="
echo "Syncing P2G4W from Git..."
echo "======================================="

cd "$(dirname "$0")"
git pull

echo ""
echo "======================================="
echo "Launching P2G4W (Unreal Engine 5)"
echo "======================================="
open projects/CPPd1/CPPd1.uproject

echo "Done! If asked to rebuild modules, click 'Yes'."
