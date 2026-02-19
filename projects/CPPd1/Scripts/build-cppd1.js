#!/usr/bin/env node
/**
 * Run the platform-specific CPPd1 build from the life repo.
 * Usage: node projects/CPPd1/Scripts/build-cppd1.js
 * Or: npm run build-cppd1 (from repo root)
 */
const path = require('path');
const { spawnSync } = require('child_process');

const SCRIPT_DIR = __dirname;
const REPO_ROOT = path.resolve(SCRIPT_DIR, '..', '..');

const isWin = process.platform === 'win32';
const cmd = isWin ? 'powershell.exe' : 'bash';
const args = isWin
  ? ['-NoProfile', '-ExecutionPolicy', 'Bypass', '-File', path.join(SCRIPT_DIR, 'build-cppd1.ps1')]
  : [path.join(SCRIPT_DIR, 'build-cppd1.sh')];

const r = spawnSync(cmd, args, { cwd: REPO_ROOT, stdio: 'inherit', env: process.env, shell: isWin });
process.exit(r.status ?? 1);
