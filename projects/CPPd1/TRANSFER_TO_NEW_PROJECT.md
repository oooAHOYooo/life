# Transferring CPPd1 Characters to a New UE5 Project

## ✅ What's Already in Git

All C++ source files are committed and pushed to GitHub:
- ✅ All `.cpp` and `.h` files
- ✅ `CPPd1.Build.cs` (module dependencies)
- ✅ All documentation
- ✅ Sync scripts

**Location in repo:** `projects/CPPd1/Source/`

---

## 🚀 Setup on New Computer

### Step 1: Clone/Pull the Life Repo

```powershell
# Clone if first time
git clone https://github.com/oooAHOYooo/life.git
cd life

# Or pull if already cloned
git pull
```

### Step 2: Create New Blank C++ UE5 Project

1. Open Unreal Engine 5.6
2. **New Project** → **Games** → **Blank**
3. **C++** (not Blueprint)
4. **No Starter Content** (or Starter Content, your choice)
5. Choose project location (e.g., `C:\Users\YourName\Documents\Unreal Projects\NewProjectName`)
6. Click **Create Project**

### Step 3: Copy C++ Source Files

**Option A: Copy Entire Source Folder (Easiest)**

```powershell
# From your life repo
$SourcePath = "C:\path\to\life\projects\CPPd1\Source"
$DestPath = "C:\Users\YourName\Documents\Unreal Projects\NewProjectName\Source"

# Copy entire Source folder
Copy-Item -Path "$SourcePath\*" -Destination "$DestPath\" -Recurse -Force
```

**Option B: Copy Specific Characters/Systems**

If you only want the ninja characters:

```powershell
# Copy ninja-related files
$Source = "C:\path\to\life\projects\CPPd1\Source\CPPd1"
$Dest = "C:\Users\YourName\Documents\Unreal Projects\NewProjectName\Source\NewProjectName"

# Copy ninja character files
Copy-Item "$Source\Variant_Combat\NinjaCharacter.*" -Destination "$Dest\Variant_Combat\" -Force
Copy-Item "$Source\Variant_Combat\CubeNinjaCharacter.*" -Destination "$Dest\Variant_Combat\" -Force
Copy-Item "$Source\Variant_Combat\NinjaGameMode.*" -Destination "$Dest\Variant_Combat\" -Force
Copy-Item "$Source\Variant_Combat\NinjaPlayerController.*" -Destination "$Dest\Variant_Combat\" -Force
Copy-Item "$Source\Variant_Combat\NinjaEnemy.*" -Destination "$Dest\Variant_Combat\AI\" -Force

# Copy procedural cube body
Copy-Item "$Source\Procedural\CubeNinjaBodyComponent.*" -Destination "$Dest\Procedural\" -Force
Copy-Item "$Source\Procedural\CPPd1ProceduralCube.*" -Destination "$Dest\Procedural\" -Force

# Copy dependencies (CombatCharacter, etc.)
Copy-Item "$Source\Variant_Combat\CombatCharacter.*" -Destination "$Dest\Variant_Combat\" -Force
Copy-Item "$Source\Variant_Combat\CombatGameMode.*" -Destination "$Dest\Variant_Combat\" -Force
# ... copy all dependencies
```

### Step 4: Update Module Name

**Important:** You need to replace `CPPd1` with your new project name!

1. **Update `.Build.cs` file:**
   - Rename `CPPd1.Build.cs` → `YourProjectName.Build.cs`
   - Change class name: `CPPd1` → `YourProjectName`

2. **Replace in all files:**
   ```powershell
   # PowerShell script to replace module name
   $ProjectName = "YourProjectName"
   $OldName = "CPPd1"
   
   Get-ChildItem -Path "Source" -Recurse -Include *.h,*.cpp,*.cs | ForEach-Object {
       (Get-Content $_.FullName) -replace $OldName, $ProjectName | Set-Content $_.FullName
   }
   ```

3. **Rename files:**
   - `CPPd1.h` → `YourProjectName.h`
   - `CPPd1.cpp` → `YourProjectName.cpp`
   - `CPPd1.Build.cs` → `YourProjectName.Build.cs`

### Step 5: Update Includes

Update all `#include "CPPd1.h"` to `#include "YourProjectName.h"`

### Step 6: Update .uproject File

1. Open your `.uproject` file in a text editor
2. Make sure the module name matches:
   ```json
   {
     "FileVersion": 3,
     "EngineAssociation": "5.6",
     "Category": "",
     "Description": "",
     "Modules": [
       {
         "Name": "YourProjectName",
         "Type": "Runtime",
         "LoadingPhase": "Default"
       }
     ]
   }
   ```

### Step 7: Generate Project Files

1. Right-click your `.uproject` file
2. Select **Generate Visual Studio project files**
3. Or in UE5: **Tools** → **Refresh Visual Studio Project**

### Step 8: Compile

1. Open project in UE5
2. Press **Ctrl+Alt+F11** to compile
3. Fix any remaining errors (mostly module name replacements)

---

## 📋 Quick Transfer Checklist

- [ ] Clone/pull life repo on new computer
- [ ] Create new blank C++ UE5 project
- [ ] Copy Source files from `projects/CPPd1/Source/`
- [ ] Replace `CPPd1` with new project name in all files
- [ ] Rename module files (`.h`, `.cpp`, `.Build.cs`)
- [ ] Update `.uproject` file
- [ ] Generate Visual Studio project files
- [ ] Compile in UE5
- [ ] Test!

---

## 🎯 What Gets Transferred

### ✅ Transferred (in Git):
- All C++ source files (`.cpp`, `.h`)
- Module build file (`.Build.cs`)
- Documentation (`.md` files)
- Sync scripts

### ❌ NOT Transferred (need to recreate):
- Blueprints (`.uasset` files)
- Materials
- Input Actions/Mappings
- Level files
- Content folder assets

**Solution:** Create Blueprints on new computer (they're just thin wrappers around C++ classes)

---

## 💡 Pro Tips

1. **Keep C++ in Git:** All your C++ code is version controlled
2. **Recreate Blueprints:** They're just data - recreate them quickly
3. **Use Sync Script:** The `sync-ue5-to-life.ps1` script works on any computer
4. **Document Dependencies:** Keep track of what other systems your characters need

---

## 🔄 Future Sync Workflow

On new computer:
```powershell
# Pull latest C++ changes
cd "C:\path\to\life"
git pull

# Sync to UE5 project
cd "projects\CPPd1"
.\sync-ue5-to-life.ps1 -Commit
```

---

## 📝 Module Name Replacement Script

Save this as `replace-module-name.ps1`:

```powershell
param(
    [string]$OldName = "CPPd1",
    [string]$NewName = "YourProjectName",
    [string]$SourcePath = "Source"
)

Write-Host "Replacing $OldName with $NewName in $SourcePath..."

Get-ChildItem -Path $SourcePath -Recurse -Include *.h,*.cpp,*.cs | ForEach-Object {
    $content = Get-Content $_.FullName -Raw
    $newContent = $content -replace $OldName, $NewName
    if ($content -ne $newContent) {
        Set-Content $_.FullName -Value $newContent -NoNewline
        Write-Host "Updated: $($_.FullName)"
    }
}

Write-Host "Done! Now rename files manually:"
Write-Host "  $OldName.h -> $NewName.h"
Write-Host "  $OldName.cpp -> $NewName.cpp"
Write-Host "  $OldName.Build.cs -> $NewName.Build.cs"
```

---

**Everything is committed and pushed! You're ready to switch computers! 🚀**
