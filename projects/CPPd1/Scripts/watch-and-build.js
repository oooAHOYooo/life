#!/usr/bin/env node
/**
 * Watch CPPd1 Source for .cpp/.h changes and run the build script.
 * So: edit in Cursor → save → build runs → UE5 (with Live Coding) hot-reloads.
 *
 * Usage (from repo root):
 *   UE5_ROOT=/path/to/UE5 node projects/CPPd1/Scripts/watch-and-build.js
 * Or on Windows (PowerShell):
 *   $env:UE5_ROOT = "C:\Program Files\Epic Games\UE_5.6"; node projects/CPPd1/Scripts/watch-and-build.js
 *
 * Requires Node (no extra deps; uses fs.watch).
 */

const fs = require('fs');
const path = require('path');
const { spawn } = require('child_process');

const SCRIPT_DIR = __dirname;
const PROJECT_DIR = path.resolve(SCRIPT_DIR, '..');
const SOURCE_DIR = path.join(PROJECT_DIR, 'Source');
const REPO_ROOT = path.resolve(PROJECT_DIR, '..', '..');

function runBuild() {
  const isWin = process.platform === 'win32';
  const cmd = isWin ? 'powershell.exe' : 'bash';
  const args = isWin
    ? ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', path.join(SCRIPT_DIR, 'build-cppd1.ps1')]
    : [path.join(SCRIPT_DIR, 'build-cppd1.sh')];

  console.log('[watch] Running build...');
  const child = spawn(cmd, args, {
    cwd: REPO_ROOT,
    stdio: 'inherit',
    env: process.env,
    shell: isWin,
  });

  child.on('close', (code) => {
    if (code === 0) {
      console.log('[watch] Build finished. Watching for changes.');
    } else {
      console.log('[watch] Build failed. Still watching for changes.');
    }
  });
}

function watchRecursive(dir, onEvent) {
  if (!fs.existsSync(dir)) {
    console.error('Source dir not found:', dir);
    process.exit(1);
  }

  const queue = [dir];
  const watched = new Set();

  function addDir(d) {
    const real = fs.realpathSync(d);
    if (watched.has(real)) return;
    watched.add(real);
    try {
      fs.watch(d, { recursive: false }, (ev, name) => {
        if (!name) return;
        const full = path.join(d, name);
        try {
          const stat = fs.statSync(full);
          if (stat.isDirectory()) addDir(full);
          else if (/\.(cpp|h)$/i.test(name)) onEvent(ev, full);
        } catch (_) {}
      });
      const entries = fs.readdirSync(d, { withFileTypes: true });
      for (const e of entries) {
        if (e.isDirectory() && e.name !== '.' && e.name !== '..') {
          addDir(path.join(d, e.name));
        }
      }
    } catch (err) {
      // ignore permission etc.
    }
  }

  addDir(dir);
  console.log('Watching', SOURCE_DIR, 'for .cpp / .h changes. Save a file to trigger build.');
}

let debounce = null;
watchRecursive(SOURCE_DIR, () => {
  if (debounce) clearTimeout(debounce);
  debounce = setTimeout(() => {
    debounce = null;
    runBuild();
  }, 500);
});
