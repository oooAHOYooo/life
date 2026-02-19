#!/usr/bin/env bash
# Build CPPd1 from the life repo so UE5 picks up C++ changes.
# Run from repo root or from projects/CPPd1. Uses UE5 install path from env.
#
# Usage:
#   export UE5_ROOT=~/UnrealEngine   # or /Applications/UnrealEngine/UE_5.6 etc.
#   ./projects/CPPd1/Scripts/build-cppd1.sh
#
# Or:
#   UE5_ROOT=~/UnrealEngine ./projects/CPPd1/Scripts/build-cppd1.sh

set -e

CONFIGURATION="${1:-Development}"
PLATFORM="Mac"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
REPO_ROOT="$(cd "$PROJECT_DIR/../.." && pwd)"
UPROJECT="$PROJECT_DIR/CPPd1.uproject"

if [ ! -f "$UPROJECT" ]; then
  echo "UProject not found: $UPROJECT"
  exit 1
fi

UE="${UE5_ROOT:-$UE_ROOT}"
if [ -z "$UE" ]; then
  # Common defaults on Mac
  for candidate in "$HOME/UnrealEngine" "/Users/Shared/UnrealEngine/UE_5.6" "/Applications/Unreal Engine/UE_5.6"; do
    if [ -d "$candidate/Engine/Build/BatchFiles/Mac" ]; then
      UE="$candidate"
      break
    fi
  done
fi

BUILD_SH="$UE/Engine/Build/BatchFiles/Mac/Build.sh"
if [ ! -f "$BUILD_SH" ]; then
  echo "Unreal Build Tool not found at: $BUILD_SH"
  echo "Set UE5_ROOT to your UE5 Engine folder, e.g.:"
  echo '  export UE5_ROOT=~/UnrealEngine'
  exit 1
fi

echo "Building CPPd1Editor $PLATFORM $CONFIGURATION"
echo "  Project: $UPROJECT"
echo "  UE:      $UE"
echo ""

"$BUILD_SH" CPPd1Editor "$PLATFORM" "$CONFIGURATION" "$UPROJECT"

echo ""
echo "Build succeeded. If UE5 is open with this project, Live Coding will hot-reload."
