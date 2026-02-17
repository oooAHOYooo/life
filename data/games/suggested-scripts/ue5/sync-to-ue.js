#!/usr/bin/env node
/**
 * Sync P2G4W scripts from suggested-scripts to actual-scripts (with CoPlago transformation)
 * Then sync to UE5 project
 * 
 * Usage: node sync-to-ue.js
 */

const fs = require('fs');
const path = require('path');

// Get the repo root (4 levels up from this file: ue5/ -> suggested-scripts/ -> games/ -> data/ -> root)
const REPO_ROOT = path.resolve(__dirname, '..', '..', '..', '..');

// Configuration
const CONFIG = {
  sourceModuleName: 'P2G4W',
  targetModuleName: 'CoPlago',
  suggestedScriptsPath: __dirname, // Current folder (ue5/)
  actualScriptsPath: path.join(__dirname, '..', 'actual-scripts'),
  ue5ProjectPath: 'C:\\Users\\agonzalez7\\Documents\\Unreal Projects\\CoPlago',
  ue5SourcePath: null, // Will be set to: ue5ProjectPath/Source/CoPlago
  ue5PublicPath: null,  // Will be set to: ue5SourcePath/Public
  ue5PrivatePath: null  // Will be set to: ue5SourcePath/Private
};

// File mappings: source filename -> destination folder (Public or Private)
const FILE_MAPPINGS = {
  'P2G4W.h': 'Public',
  'P2G4W.cpp': 'Private',
  'P2G4WGameModeBase.h': 'Public',
  'P2G4WGameModeBase.cpp': 'Private',
  'P2G4WGameStateBase.h': 'Public',
  'P2G4WGameStateBase.cpp': 'Private',
  'P2G4WPlayerState.h': 'Public',
  'P2G4WPlayerState.cpp': 'Private',
  'P2G4WPlayerController.h': 'Public',
  'P2G4WPlayerController.cpp': 'Private',
  'P2G4WCharacter.h': 'Public',
  'P2G4WCharacter.cpp': 'Private',
  'P2G4WGoalZone.h': 'Public',
  'P2G4WGoalZone.cpp': 'Private',
  'P2G4WLockOnTargetComponent.h': 'Public',
  'P2G4WLockOnTargetComponent.cpp': 'Private',
  'P2G4WEnemy.h': 'Public',
  'P2G4WEnemy.cpp': 'Private',
  'P2G4WEnemyWaveSpawner.h': 'Public',
  'P2G4WEnemyWaveSpawner.cpp': 'Private'
};

// Initialize paths
CONFIG.ue5SourcePath = path.join(CONFIG.ue5ProjectPath, 'Source', CONFIG.targetModuleName);
CONFIG.ue5PublicPath = path.join(CONFIG.ue5SourcePath, 'Public');
CONFIG.ue5PrivatePath = path.join(CONFIG.ue5SourcePath, 'Private');

/**
 * Replace module name in content
 */
function replaceModuleName(content, sourceName, targetName) {
  // Replace in class names, includes, etc.
  return content
    .replace(new RegExp(sourceName, 'g'), targetName)
    .replace(new RegExp(sourceName.toLowerCase(), 'g'), targetName.toLowerCase());
}

/**
 * Replace module name in filename
 */
function replaceModuleNameInFilename(filename, sourceName, targetName) {
  return filename.replace(new RegExp(sourceName, 'g'), targetName);
}

/**
 * Ensure directory exists
 * @param {boolean} quiet - If true, suppress verbose output
 */
function ensureDir(dirPath, quiet = false) {
  if (!fs.existsSync(dirPath)) {
    fs.mkdirSync(dirPath, { recursive: true });
    if (!quiet) console.log(`✓ Created directory: ${dirPath}`);
  }
}

/**
 * Copy and transform file
 */
function copyAndTransformFile(sourceFile, destFile, sourceName, targetName) {
  // Read source file
  const content = fs.readFileSync(sourceFile, 'utf8');
  
  // Replace module name in content
  const transformedContent = replaceModuleName(content, sourceName, targetName);
  
  // Write to destination
  ensureDir(path.dirname(destFile));
  fs.writeFileSync(destFile, transformedContent, 'utf8');
  
  return transformedContent;
}

/**
 * Step 1: Transform suggested-scripts to actual-scripts
 * @param {boolean} quiet - If true, suppress verbose output
 */
function transformToActualScripts(quiet = false) {
  if (!quiet) {
    console.log('\n=== Step 1: Transforming to actual-scripts ===');
    console.log(`Source: ${CONFIG.suggestedScriptsPath}`);
    console.log(`Destination: ${CONFIG.actualScriptsPath}`);
    console.log(`Replacing: ${CONFIG.sourceModuleName} -> ${CONFIG.targetModuleName}\n`);
  }
  
  ensureDir(CONFIG.actualScriptsPath);
  
  let filesTransformed = 0;
  
  for (const [sourceFileName, destFolder] of Object.entries(FILE_MAPPINGS)) {
    const sourceFile = path.join(CONFIG.suggestedScriptsPath, sourceFileName);
    
    if (!fs.existsSync(sourceFile)) {
      if (!quiet) console.log(`⚠️  Source file not found: ${sourceFileName}`);
      continue;
    }
    
    // Determine destination filename
    const destFileName = replaceModuleNameInFilename(
      sourceFileName,
      CONFIG.sourceModuleName,
      CONFIG.targetModuleName
    );
    
    const destFile = path.join(CONFIG.actualScriptsPath, destFolder, destFileName);
    
    // Copy and transform
    copyAndTransformFile(sourceFile, destFile, CONFIG.sourceModuleName, CONFIG.targetModuleName);
    
    if (!quiet) console.log(`✓ Transformed: ${sourceFileName} -> ${destFolder}/${destFileName}`);
    filesTransformed++;
  }
  
  if (!quiet) console.log(`\n✓ Step 1 complete: ${filesTransformed} files transformed`);
  return filesTransformed;
}

/**
 * Step 2: Sync actual-scripts to UE5 project
 * @param {boolean} quiet - If true, suppress verbose output
 */
function syncToUE5(quiet = false) {
  if (!quiet) {
    console.log('\n=== Step 2: Syncing to UE5 project ===');
    console.log(`Source: ${CONFIG.actualScriptsPath}`);
    console.log(`Destination: ${CONFIG.ue5SourcePath}\n`);
  }
  
  // Ensure UE5 directories exist
  ensureDir(CONFIG.ue5PublicPath);
  ensureDir(CONFIG.ue5PrivatePath);
  
  let filesSynced = 0;
  
  for (const [sourceFileName, destFolder] of Object.entries(FILE_MAPPINGS)) {
    // Get the transformed filename
    const transformedFileName = replaceModuleNameInFilename(
      sourceFileName,
      CONFIG.sourceModuleName,
      CONFIG.targetModuleName
    );
    
    const sourceFile = path.join(CONFIG.actualScriptsPath, destFolder, transformedFileName);
    
    if (!fs.existsSync(sourceFile)) {
      if (!quiet) console.log(`⚠️  Actual script file not found: ${destFolder}/${transformedFileName}`);
      continue;
    }
    
    // Destination in UE5 project
    const destFile = path.join(
      destFolder === 'Public' ? CONFIG.ue5PublicPath : CONFIG.ue5PrivatePath,
      transformedFileName
    );
    
    // Copy file (content already transformed)
    ensureDir(path.dirname(destFile));
    fs.copyFileSync(sourceFile, destFile);
    
    if (!quiet) console.log(`✓ Synced: ${transformedFileName} -> UE5/${destFolder}`);
    filesSynced++;
  }
  
  if (!quiet) console.log(`\n✓ Step 2 complete: ${filesSynced} files synced to UE5`);
  return filesSynced;
}

/**
 * Main execution
 * @param {boolean} quiet - If true, suppress verbose output (for use in compile-all.js)
 */
function main(quiet = false) {
  if (!quiet) {
    console.log('========================================');
    console.log('  Sync P2G4W Scripts to UE5 Project');
    console.log('========================================');
  }
  
  // Validate paths
  if (!fs.existsSync(CONFIG.suggestedScriptsPath)) {
    const error = `❌ Error: Suggested scripts path not found: ${CONFIG.suggestedScriptsPath}`;
    if (quiet) throw new Error(error);
    console.error(error);
    process.exit(1);
  }
  
  if (!fs.existsSync(CONFIG.ue5ProjectPath)) {
    const error = `❌ Error: UE5 project path not found: ${CONFIG.ue5ProjectPath}`;
    if (quiet) throw new Error(error);
    console.error(error);
    console.error('   Please update CONFIG.ue5ProjectPath in sync-to-ue.js');
    process.exit(1);
  }
  
  try {
    // Step 1: Transform to actual-scripts
    const transformed = transformToActualScripts(quiet);
    
    // Step 2: Sync to UE5
    const synced = syncToUE5(quiet);
    
    // Summary
    if (!quiet) {
      console.log('\n========================================');
      console.log('  Sync Complete!');
      console.log('========================================');
      console.log(`Files transformed: ${transformed}`);
      console.log(`Files synced to UE5: ${synced}`);
      console.log('\nNext steps:');
      console.log('1. If UE5 editor is OPEN:');
      console.log('   - UE5 will detect file changes automatically');
      console.log('   - Click "Compile" button in UE5 (or press Ctrl+Alt+B)');
      console.log('   - Or UE5 may prompt to hot-reload');
      console.log('2. If UE5 editor is CLOSED:');
      console.log('   - Open your UE5 project');
      console.log('   - Build the project (Ctrl+Alt+B)');
      console.log('   - Then hit Play');
      console.log('\n💡 TIP: Keep UE5 editor open while developing for faster iteration!');
      console.log('\nActual scripts location:');
      console.log(`  ${CONFIG.actualScriptsPath}`);
      console.log('\n');
    }
    
    return { transformed, synced };
    
  } catch (error) {
    if (quiet) throw error;
    console.error('\n❌ Error during sync:', error.message);
    console.error(error.stack);
    process.exit(1);
  }
}

// Run if called directly
if (require.main === module) {
  main();
}

module.exports = { main, CONFIG };
