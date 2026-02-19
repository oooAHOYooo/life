#!/usr/bin/env node
// Generates a self-contained CPPd1 C++ code viewer HTML file
import { readdirSync, readFileSync, writeFileSync } from 'fs';
import { join, relative, extname } from 'path';

const SRC = '/Users/ag/life/projects/CPPd1/Source/CPPd1';
const OUT = '/Users/ag/life/subpages/cppd1-source-viewer.html';

// Collect all .cpp and .h files recursively
function walk(dir) {
  const entries = readdirSync(dir, { withFileTypes: true });
  const files = [];
  for (const e of entries) {
    const full = join(dir, e.name);
    if (e.isDirectory()) files.push(...walk(full));
    else if (['.cpp', '.h', '.hpp'].includes(extname(e.name))) files.push(full);
  }
  return files.sort();
}

const allFiles = walk(SRC);

// Build tree structure for sidebar
function buildTree(files, root) {
  const tree = { name: 'CPPd1', path: '', children: {}, files: [] };
  for (const f of files) {
    const rel = relative(root, f);
    const parts = rel.split('/');
    let node = tree;
    for (let i = 0; i < parts.length - 1; i++) {
      const seg = parts[i];
      if (!node.children[seg]) node.children[seg] = { name: seg, path: parts.slice(0, i + 1).join('/'), children: {}, files: [] };
      node = node.children[seg];
    }
    node.files.push({ name: parts[parts.length - 1], path: rel });
  }
  return tree;
}

const tree = buildTree(allFiles, SRC);

// Embed file contents as JSON
const fileData = {};
for (const f of allFiles) {
  const rel = relative(SRC, f);
  fileData[rel] = readFileSync(f, 'utf8');
}

const fileDataJson = JSON.stringify(fileData);
const treeJson = JSON.stringify(tree);

const totalLines = Object.values(fileData).reduce((s, v) => s + v.split('\n').length, 0);
const totalFiles = allFiles.length;

const html = `<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
<title>CPPd1 C++ Source Viewer — LifeOS</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/atom-one-dark.min.css">
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"><\/script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/languages/cpp.min.js"><\/script>
<style>
*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

:root {
  --bg: #0d1117;
  --sidebar-bg: #161b22;
  --border: #30363d;
  --header-bg: #010409;
  --text: #e6edf3;
  --muted: #7d8590;
  --dim: #484f58;
  --accent: #58a6ff;
  --accent2: #79c0ff;
  --folder: #e3b341;
  --ext-cpp: #f78166;
  --ext-h: #79c0ff;
  --active-bg: #1f2937;
  --active-border: #58a6ff;
  --scrollbar: #30363d;
  --tag-bg: #21262d;
}

html, body {
  height: 100%;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', system-ui, sans-serif;
  background: var(--bg);
  color: var(--text);
  overflow: hidden;
}

/* ── LAYOUT ──────────────────────────────────────────────────────────────── */
#app { display: flex; flex-direction: column; height: 100vh; }

/* HEADER */
#header {
  background: var(--header-bg);
  border-bottom: 1px solid var(--border);
  padding: 10px 16px;
  display: flex;
  align-items: center;
  gap: 12px;
  flex-shrink: 0;
  z-index: 20;
  flex-wrap: wrap;
  row-gap: 6px;
}

#hamburger {
  display: none;
  background: none;
  border: none;
  color: var(--muted);
  font-size: 18px;
  cursor: pointer;
  padding: 4px 6px;
  border-radius: 4px;
  line-height: 1;
  flex-shrink: 0;
}
#hamburger:hover { color: var(--text); background: rgba(255,255,255,0.06); }

#header-title { font-size: 14px; font-weight: 600; white-space: nowrap; }
#header-title span { color: var(--accent); }

.badge {
  background: var(--tag-bg);
  border: 1px solid var(--border);
  border-radius: 20px;
  padding: 2px 9px;
  font-size: 11px;
  color: var(--muted);
  white-space: nowrap;
}

#back-link {
  color: var(--muted);
  text-decoration: none;
  font-size: 12px;
  padding: 3px 8px;
  border: 1px solid var(--border);
  border-radius: 5px;
  transition: color .15s, border-color .15s;
}
#back-link:hover { color: var(--accent); border-color: var(--accent); }

#search-wrap {
  margin-left: auto;
  position: relative;
  display: flex;
  align-items: center;
}
#search {
  background: #0d1117;
  border: 1px solid var(--border);
  border-radius: 6px;
  padding: 5px 10px 5px 28px;
  color: var(--text);
  font-size: 13px;
  width: 200px;
  outline: none;
  transition: border-color .15s, width .2s;
}
#search:focus { border-color: var(--accent); width: 250px; }
#search::placeholder { color: var(--dim); }
#search-icon { position: absolute; left: 8px; color: var(--dim); font-size: 13px; pointer-events: none; }

/* BODY */
#body { display: flex; flex: 1; overflow: hidden; position: relative; }

/* SIDEBAR */
#sidebar {
  width: 270px;
  flex-shrink: 0;
  background: var(--sidebar-bg);
  border-right: 1px solid var(--border);
  overflow-y: auto;
  overflow-x: hidden;
  padding: 8px 0 20px;
  transition: transform .2s ease;
}
#sidebar::-webkit-scrollbar { width: 4px; }
#sidebar::-webkit-scrollbar-thumb { background: var(--scrollbar); border-radius: 2px; }

/* MAIN */
#main { flex: 1; display: flex; flex-direction: column; overflow: hidden; min-width: 0; }

#file-header {
  background: var(--sidebar-bg);
  border-bottom: 1px solid var(--border);
  padding: 8px 16px;
  display: flex;
  align-items: center;
  gap: 8px;
  flex-shrink: 0;
  min-height: 40px;
  overflow: hidden;
}
#breadcrumb { font-size: 12px; color: var(--muted); overflow: hidden; text-overflow: ellipsis; white-space: nowrap; flex: 1; }
#breadcrumb span { color: var(--text); font-weight: 500; }
#breadcrumb .sep { margin: 0 3px; color: var(--dim); }
#line-count { font-size: 11px; color: var(--dim); white-space: nowrap; flex-shrink: 0; }

#code-wrap { flex: 1; overflow: auto; }
#code-wrap::-webkit-scrollbar { width: 8px; height: 8px; }
#code-wrap::-webkit-scrollbar-track { background: var(--bg); }
#code-wrap::-webkit-scrollbar-thumb { background: var(--scrollbar); border-radius: 4px; }

/* ── TREE ──────────────────────────────────────────────────────────────────── */
.tree-folder-label {
  display: flex;
  align-items: center;
  gap: 5px;
  padding: 4px 10px;
  font-size: 11px;
  font-weight: 600;
  color: var(--muted);
  text-transform: uppercase;
  letter-spacing: 0.05em;
  cursor: pointer;
  user-select: none;
}
.tree-folder-label:hover { background: rgba(255,255,255,0.04); color: var(--folder); }
.tree-folder-label .icon { font-size: 9px; transition: transform .15s; }
.tree-folder-label.open .icon { transform: rotate(90deg); }

.tree-children { display: none; }
.tree-children.open { display: block; }

.tree-file {
  display: flex;
  align-items: center;
  gap: 5px;
  padding: 3px 10px;
  font-size: 12px;
  color: var(--muted);
  cursor: pointer;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  border-left: 2px solid transparent;
  transition: background .1s, color .1s;
}
.tree-file:hover { background: rgba(255,255,255,0.04); color: var(--text); }
.tree-file.active { background: var(--active-bg); color: var(--accent); border-left-color: var(--active-border); }
.ext-cpp { color: var(--ext-cpp); font-size: 10px; font-weight: 700; flex-shrink: 0; }
.ext-h   { color: var(--ext-h);   font-size: 10px; font-weight: 700; flex-shrink: 0; }

/* ── WELCOME ──────────────────────────────────────────────────────────────── */
#welcome { padding: 40px 32px; max-width: 680px; }
#welcome h2 { font-size: 20px; font-weight: 700; margin-bottom: 10px; }
#welcome p { font-size: 13px; color: var(--muted); line-height: 1.7; margin-bottom: 18px; }
.section-row {
  display: flex; align-items: center; gap: 10px;
  padding: 9px 14px;
  background: var(--sidebar-bg);
  border: 1px solid var(--border);
  border-radius: 7px;
  font-size: 13px;
  cursor: pointer;
  margin-bottom: 6px;
  transition: border-color .15s, color .15s;
}
.section-row:hover { border-color: var(--accent); color: var(--accent); }
.section-row .count { margin-left: auto; color: var(--dim); font-size: 12px; }

/* ── CODE ──────────────────────────────────────────────────────────────────── */
pre {
  margin: 0;
  padding: 16px 20px;
  font-size: 13px;
  line-height: 1.6;
  font-family: 'JetBrains Mono', 'Fira Code', 'Cascadia Code', 'SF Mono', Menlo, Monaco, Consolas, monospace;
}
pre code.hljs { background: transparent !important; padding: 0 !important; font-size: inherit; font-family: inherit; }
.hljs { background: var(--bg) !important; }

/* ── SEARCH RESULTS ──────────────────────────────────────────────────────── */
#search-results { display: none; flex-direction: column; overflow-y: auto; padding: 10px 16px; gap: 3px; }
#search-results.visible { display: flex; }
.sr-item {
  display: flex; gap: 8px; align-items: center;
  padding: 5px 8px; border-radius: 5px; cursor: pointer; font-size: 12px;
}
.sr-item:hover { background: rgba(255,255,255,0.05); }
.sr-file { color: var(--accent); font-weight: 600; white-space: nowrap; flex-shrink: 0; }
.sr-line { color: var(--dim); font-size: 11px; white-space: nowrap; flex-shrink: 0; }
.sr-text { color: var(--muted); overflow: hidden; text-overflow: ellipsis; white-space: nowrap; font-family: monospace; font-size: 11px; }
.sr-match { color: var(--accent2); font-weight: bold; }

/* ── OVERLAY (mobile sidebar backdrop) ──────────────────────────────────── */
#overlay {
  display: none;
  position: fixed;
  inset: 0;
  background: rgba(0,0,0,0.6);
  z-index: 15;
}

/* ── MOBILE ──────────────────────────────────────────────────────────────── */
@media (max-width: 700px) {
  #hamburger { display: block; }

  #sidebar {
    position: absolute;
    top: 0; left: 0; bottom: 0;
    z-index: 16;
    transform: translateX(-100%);
    border-right: 1px solid var(--border);
    box-shadow: 4px 0 20px rgba(0,0,0,0.5);
  }
  #sidebar.open { transform: translateX(0); }
  #overlay.open { display: block; }

  #search { width: 150px; }
  #search:focus { width: 180px; }

  pre { font-size: 11.5px; padding: 12px 14px; }

  .badge:nth-child(3) { display: none; }
}
</style>
</head>
<body>
<div id="app">
  <header id="header">
    <button id="hamburger" aria-label="Toggle file tree">☰</button>
    <div id="header-title"><span>CPPd1</span> C++ Source</div>
    <span class="badge">${totalFiles} files</span>
    <span class="badge">${totalLines.toLocaleString()} lines</span>
    <a href="cppd1.html" id="back-link">← CPPd1 Hub</a>
    <div id="search-wrap">
      <span id="search-icon">⌕</span>
      <input id="search" type="text" placeholder="Search… (⌘K)" autocomplete="off" spellcheck="false">
    </div>
  </header>

  <div id="body">
    <div id="overlay"></div>
    <nav id="sidebar" aria-label="File tree">
      <div id="tree-root"></div>
    </nav>
    <main id="main">
      <div id="file-header">
        <div id="breadcrumb">← select a file</div>
        <div id="line-count"></div>
      </div>
      <div id="code-wrap">
        <div id="welcome">
          <h2>CPPd1 — Unreal Engine 5 C++</h2>
          <p>Browse all ${totalFiles} source files from the sidebar (or tap ☰ on mobile). This project is an Unreal Engine 5 C++ game framework with three gameplay variants: Combat (ninja/marauder), Platforming, and SideScrolling.</p>
          <div id="quick-nav"></div>
        </div>
        <pre id="code-pre" style="display:none"><code id="code-display" class="language-cpp"></code></pre>
        <div id="search-results"></div>
      </div>
    </main>
  </div>
</div>

<script>
const FILES = ${fileDataJson};
const TREE  = ${treeJson};

let currentFile = null;

// ── escape helpers ────────────────────────────────────────────────────────
function esc(s) {
  return String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;');
}
function escRegex(s) {
  var out = '';
  var sp = '\\\\.[]{}()*+?^|';
  for (var i = 0; i < s.length; i++) { if (sp.indexOf(s[i]) !== -1) out += '\\\\'; out += s[i]; }
  return out;
}

// ── sidebar hamburger ─────────────────────────────────────────────────────
const sidebar  = document.getElementById('sidebar');
const overlay  = document.getElementById('overlay');
const hamburger = document.getElementById('hamburger');

function openSidebar()  { sidebar.classList.add('open');  overlay.classList.add('open'); }
function closeSidebar() { sidebar.classList.remove('open'); overlay.classList.remove('open'); }
hamburger.addEventListener('click', () => sidebar.classList.contains('open') ? closeSidebar() : openSidebar());
overlay.addEventListener('click', closeSidebar);

// ── build tree HTML ───────────────────────────────────────────────────────
function extTag(name) {
  if (name.endsWith('.cpp')) return '<span class="ext-cpp">CPP</span>';
  if (name.endsWith('.h'))   return '<span class="ext-h">H</span>';
  return '';
}

function renderNode(node, depth) {
  let html = '';
  for (const f of node.files) {
    const indent = (depth * 14) + 22;
    html += \`<div class="tree-file" data-path="\${esc(f.path)}" style="padding-left:\${indent}px" title="\${esc(f.path)}">\${extTag(f.name)}<span>\${esc(f.name)}</span></div>\`;
  }
  for (const key of Object.keys(node.children).sort()) {
    const child = node.children[key];
    const id    = 'f_' + child.path.replace(/[^a-z0-9]/gi,'_');
    const indent = (depth * 14) + 8;
    html += \`<div class="tree-folder">
      <div class="tree-folder-label open" data-folder="\${id}" style="padding-left:\${indent}px">
        <span class="icon">▶</span><span>\${esc(child.name)}</span>
      </div>
      <div class="tree-children open" id="\${id}">\${renderNode(child, depth + 1)}</div>
    </div>\`;
  }
  return html;
}

document.getElementById('tree-root').innerHTML = renderNode(TREE, 0);

// ── folder toggle ─────────────────────────────────────────────────────────
document.getElementById('tree-root').addEventListener('click', e => {
  const lbl = e.target.closest('[data-folder]');
  if (lbl) {
    const ch = document.getElementById(lbl.dataset.folder);
    const open = ch.classList.toggle('open');
    lbl.classList.toggle('open', open);
    return;
  }
  const fi = e.target.closest('[data-path]');
  if (fi) { openFile(fi.dataset.path); closeSidebar(); }
});

// ── open file ─────────────────────────────────────────────────────────────
function openFile(path) {
  const content = FILES[path];
  if (content === undefined) return;
  currentFile = path;

  document.querySelectorAll('.tree-file.active').forEach(el => el.classList.remove('active'));
  const el = document.querySelector(\`.tree-file[data-path="\${CSS.escape(path)}"]\`);
  if (el) { el.classList.add('active'); el.scrollIntoView({ block: 'nearest' }); }

  const parts = path.split('/');
  const crumb = parts.slice(0,-1).map(p => \`<span>\${esc(p)}</span><span class="sep">/</span>\`).join('') + \`<span>\${esc(parts.at(-1))}</span>\`;
  document.getElementById('breadcrumb').innerHTML = crumb;
  document.getElementById('line-count').textContent = content.split('\\n').length + ' lines';

  document.getElementById('welcome').style.display = 'none';
  document.getElementById('search-results').classList.remove('visible');
  const pre    = document.getElementById('code-pre');
  const codeEl = document.getElementById('code-display');
  pre.style.display = 'block';
  codeEl.textContent = content;
  hljs.highlightElement(codeEl);
  document.getElementById('code-wrap').scrollTop = 0;
}

// ── quick nav ─────────────────────────────────────────────────────────────
const SECTIONS = [
  { label: 'Core Framework',      prefix: '',                        hint: ['CPPd1Character.cpp','CPPd1Character.h','CPPd1InputManager.cpp','CPPd1InputManager.h','CPPd1SplitScreenManager.cpp','CPPd1SplitScreenManager.h','CPPd1LockOnTargetComponent.cpp','CPPd1LockOnTargetComponent.h','CPPd1PlayerController.cpp','CPPd1PlayerController.h','CPPd1GameMode.cpp','CPPd1GameMode.h','CPPd1.cpp','CPPd1.h'] },
  { label: 'Combat — Characters', prefix: 'Variant_Combat/',         hint: ['CombatCharacter.cpp','CombatCharacter.h','NinjaCharacter.cpp','NinjaCharacter.h','CubeNinjaCharacter.cpp','CubeNinjaCharacter.h'] },
  { label: 'Combat — AI',         prefix: 'Variant_Combat/AI/',      hint: ['CombatEnemy.cpp','CombatEnemy.h','NinjaEnemy.cpp','NinjaEnemy.h','BruteMarauder.cpp','BruteMarauder.h','TricksterMarauder.cpp','TricksterMarauder.h','CombatWaveSpawner.cpp','CombatWaveSpawner.h','SpinningRadiusWaveSpawner.cpp','SpinningRadiusWaveSpawner.h'] },
  { label: 'Combat — Systems',    prefix: 'Variant_Combat/',         hint: ['CombatSkillSystem.cpp','CombatSkillSystem.h','CombatStaminaSystem.cpp','CombatStaminaSystem.h','CombatFlowSystem.cpp','CombatFlowSystem.h','CombatTrickSystem.cpp','CombatTrickSystem.h','CombatAdvancedMechanics.cpp','CombatAdvancedMechanics.h'] },
  { label: 'Platforming',         prefix: 'Variant_Platforming/',    hint: ['PlatformingCharacter.cpp','PlatformingCharacter.h','PlatformingGameMode.cpp','PlatformingGameMode.h'] },
  { label: 'SideScrolling',       prefix: 'Variant_SideScrolling/',  hint: ['SideScrollingCharacter.cpp','SideScrollingCharacter.h','SideScrollingCameraManager.cpp','SideScrollingCameraManager.h'] },
  { label: 'Procedural',          prefix: 'Procedural/',             hint: ['CPPd1ProceduralCube.cpp','CPPd1ProceduralCube.h','CubeNinjaBodyComponent.cpp','CubeNinjaBodyComponent.h'] },
];

const qnav = document.getElementById('quick-nav');
for (const sec of SECTIONS) {
  const valid = sec.hint.filter(f => FILES[sec.prefix + f]);
  if (!valid.length) continue;
  const first = sec.prefix + valid[0];
  const row = document.createElement('div');
  row.className = 'section-row';
  row.innerHTML = \`<span>\${esc(sec.label)}</span><span class="count">\${valid.length} files</span>\`;
  row.addEventListener('click', () => openFile(first));
  qnav.appendChild(row);
}

// ── search ─────────────────────────────────────────────────────────────────
const searchEl  = document.getElementById('search');
const resultsEl = document.getElementById('search-results');
let searchTimer = null;

searchEl.addEventListener('input', () => { clearTimeout(searchTimer); searchTimer = setTimeout(doSearch, 150); });

function doSearch() {
  const q = searchEl.value.trim().toLowerCase();
  if (!q) {
    resultsEl.classList.remove('visible');
    resultsEl.innerHTML = '';
    if (currentFile) document.getElementById('code-pre').style.display = 'block';
    else document.getElementById('welcome').style.display = '';
    return;
  }
  document.getElementById('welcome').style.display = 'none';
  document.getElementById('code-pre').style.display = 'none';
  resultsEl.innerHTML = '';
  resultsEl.classList.add('visible');

  const matches = [];
  for (const [path, content] of Object.entries(FILES)) {
    const fname = path.split('/').pop().toLowerCase();
    if (fname.includes(q)) matches.push({ path, lineNum: 0, lineText: path, type: 'file' });
    const lines = content.split('\\n');
    let count = 0;
    for (let i = 0; i < lines.length && count < 15; i++) {
      if (lines[i].toLowerCase().includes(q)) { matches.push({ path, lineNum: i+1, lineText: lines[i].trim(), type: 'content' }); count++; }
    }
  }

  if (!matches.length) {
    resultsEl.innerHTML = '<div style="padding:20px;color:var(--dim);font-size:13px">No results.</div>';
    return;
  }

  const re = new RegExp(escRegex(q), 'gi');
  for (const m of matches.slice(0, 200)) {
    const fname = m.path.split('/').pop();
    const highlighted = m.lineText.replace(re, match => \`<span class="sr-match">\${esc(match)}</span>\`);
    const div = document.createElement('div');
    div.className = 'sr-item';
    if (m.type === 'file') {
      div.innerHTML = \`<span class="sr-file">\${esc(fname)}</span><span class="sr-line">file</span>\`;
    } else {
      div.innerHTML = \`<span class="sr-file">\${esc(fname)}</span><span class="sr-line">:\${m.lineNum}</span><span class="sr-text">\${highlighted}</span>\`;
    }
    div.addEventListener('click', () => { searchEl.value=''; doSearch(); openFile(m.path); });
    resultsEl.appendChild(div);
  }
  if (matches.length > 200) {
    const more = document.createElement('div');
    more.style.cssText = 'padding:10px 16px;color:var(--dim);font-size:12px';
    more.textContent = \`… \${matches.length - 200} more. Refine search.\`;
    resultsEl.appendChild(more);
  }
}

// ── keyboard shortcuts ────────────────────────────────────────────────────
document.addEventListener('keydown', e => {
  if ((e.metaKey || e.ctrlKey) && e.key === 'k') { e.preventDefault(); searchEl.focus(); searchEl.select(); }
  if (e.key === 'Escape') { searchEl.value = ''; doSearch(); searchEl.blur(); }
});
<\/script>
</body>
</html>`;

writeFileSync(OUT, html, 'utf8');
console.log(`✓ Generated: ${OUT}`);
console.log(`  Files: ${totalFiles}, Lines: ${totalLines.toLocaleString()}`);
