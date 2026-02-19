/**
 * compile-all.js – build step for LifeOS
 * Run from repo root: node compile-all.js
 *
 * - Compiles data/games/gdd.md → subpages/gdd.html (GDD page)
 * - Compiles data/games/suggested-scripts/GUIDE.md → subpages/p2g4w-ue5-guide.html (UE5 scripts guide)
 */

const fs = require("fs");
const path = require("path");
const { marked } = require("marked");

const ROOT = __dirname;

function makeDocPage(templatePath, mdPath, htmlPath, title, headerTitle, headerSubtitle, navCurrentLabel) {
  if (!fs.existsSync(mdPath)) {
    console.warn("compile-all: " + path.relative(ROOT, mdPath) + " not found, skipping.");
    return;
  }
  const md = fs.readFileSync(mdPath, "utf8");
  marked.setOptions({ breaks: true, gfm: true });
  const bodyHtml = marked.parse(md);

  const navBlock = '<a href="2026.html" class="nav-link">2026</a>\n                <span style="color: #8b7355;">|</span>\n                <a href="xmas-break.html" class="nav-link">Christmas Break</a>\n                <span style="color: #8b7355;">|</span>\n                <a href="return-first.html" class="nav-link">Return First</a>\n                <span style="color: #8b7355;">|</span>\n                ' + (navCurrentLabel === "GDD" ? '<span class="nav-link" style="font-style: normal;">GDD</span>\n                <span style="color: #8b7355;">|</span>\n                <a href="p2g4w-ue5-guide.html" class="nav-link">UE5 Guide</a>' : '<a href="gdd.html" class="nav-link">GDD</a>\n                <span style="color: #8b7355;">|</span>\n                <span class="nav-link" style="font-style: normal;">UE5 Guide</span>');

  let template = fs.readFileSync(templatePath, "utf8");
  template = template.replace("<title>2026 Goals - LifeOS</title>", "<title>" + title + " - LifeOS</title>");
  template = template.replace(
    /<h1>2026<\/h1>\s*<p>GOALS & PLAN<\/p>/,
    "<h1>" + headerTitle + "</h1>\n                <p>" + headerSubtitle + "</p>"
  );
  template = template.replace(
    /<a href="xmas-break\.html" class="nav-link">Christmas Break<\/a>\s*<span[^>]*>\|<\/span>\s*<a href="return-first\.html" class="nav-link">Return First<\/a>/,
    navBlock
  );
  template = template.replace(
    '<div id="contentContainer" class="content"></div>',
    '<div id="contentContainer" class="content">' + bodyHtml + "</div>"
  );
  template = template.replace(
    /<script>[\s\S]*?Goals2026Page[\s\S]*?<\/script>/,
    "<!-- Content compiled by compile-all.js -->"
  );
  fs.writeFileSync(htmlPath, template, "utf8");
}

function compileGdd() {
  makeDocPage(
    path.join(ROOT, "subpages", "2026.html"),
    path.join(ROOT, "data", "games", "gdd.md"),
    path.join(ROOT, "subpages", "gdd.html"),
    "P2G4W – Game Design Document",
    "P2G4W",
    "GAME DESIGN DOCUMENT",
    "GDD"
  );
  console.log("compile-all: data/games/gdd.md → subpages/gdd.html");
}

function compileP2G4WGuide() {
  makeDocPage(
    path.join(ROOT, "subpages", "2026.html"),
    path.join(ROOT, "data", "games", "suggested-scripts", "GUIDE.md"),
    path.join(ROOT, "subpages", "p2g4w-ue5-guide.html"),
    "P2G4W – UE5 Scripts Guide",
    "P2G4W",
    "UE5 SCRIPTS GUIDE",
    "UE5 Guide"
  );
  console.log("compile-all: data/games/suggested-scripts/GUIDE.md → subpages/p2g4w-ue5-guide.html");
}

// Nav block for CPPd1 / P2G4W project pages (GDD, hub, and guides)
function cppd1NavBlock(currentLabel) {
  const sep = '\n                <span style="color: #8b7355;">|</span>\n                ';
  const cppd1Part = currentLabel === "CPPd1"
    ? '<span class="nav-link" style="font-style: normal;">CPPd1</span>'
    : '<a href="cppd1.html" class="nav-link">CPPd1</a>' + sep + '<span class="nav-link" style="font-style: normal;">' + currentLabel + "</span>";
  return '<a href="2026.html" class="nav-link">2026</a>' + sep + '<a href="gdd.html" class="nav-link">GDD</a>' + sep + cppd1Part + sep + '<a href="p2g4w-ue5-guide.html" class="nav-link">UE5 Guide</a>';
}

function makeCppd1Page(templatePath, htmlPath, title, headerTitle, headerSubtitle, navCurrentLabel, bodyHtml) {
  const navBlock = cppd1NavBlock(navCurrentLabel);
  let template = fs.readFileSync(templatePath, "utf8");
  template = template.replace("<title>2026 Goals - LifeOS</title>", "<title>" + title + " - LifeOS</title>");
  template = template.replace(
    /<h1>2026<\/h1>\s*<p>GOALS & PLAN<\/p>/,
    "<h1>" + headerTitle + "</h1>\n                <p>" + headerSubtitle + "</p>"
  );
  template = template.replace(
    /<a href="xmas-break\.html" class="nav-link">Christmas Break<\/a>\s*<span[^>]*>\|<\/span>\s*<a href="return-first\.html" class="nav-link">Return First<\/a>/,
    navBlock
  );
  template = template.replace(
    '<div id="contentContainer" class="content"></div>',
    '<div id="contentContainer" class="content">' + bodyHtml + "</div>"
  );
  template = template.replace(
    /<script>[\s\S]*?Goals2026Page[\s\S]*?<\/script>/,
    "<!-- Content compiled by compile-all.js -->"
  );
  fs.writeFileSync(htmlPath, template, "utf8");
}

const CPPD1_GUIDES = [
  { md: "NINJA_SETUP.md", html: "cppd1-ninja-setup.html", title: "CPPd1 – Ninja Setup", subtitle: "NINJA SETUP", nav: "Ninja Setup" },
  { md: "CUBE_NINJA_MODEL.md", html: "cppd1-cube-ninja.html", title: "CPPd1 – Cube Ninja Model", subtitle: "CUBE NINJA MODEL", nav: "Cube Ninja" },
  { md: "MARAUDERS_SETUP.md", html: "cppd1-marauders.html", title: "CPPd1 – Marauders Setup", subtitle: "MARAUDERS SETUP", nav: "Marauders" },
  { md: "WORKFLOW_CURSOR_TO_UE5.md", html: "cppd1-workflow.html", title: "CPPd1 – Workflow Cursor to UE5", subtitle: "WORKFLOW CURSOR TO UE5", nav: "Workflow" },
  { md: "CPP_IN_UE5_REFERENCE.md", html: "cppd1-cpp-reference.html", title: "CPPd1 – C++ in UE5 Reference", subtitle: "C++ IN UE5 REFERENCE", nav: "C++ Reference" },
  { md: "QUICK_START_GUIDE.md", html: "cppd1-quick-start.html", title: "CPPd1 – Quick Start", subtitle: "QUICK START", nav: "Quick Start" },
  { md: "WAVE_SYSTEM_GUIDE.md", html: "cppd1-wave-system.html", title: "CPPd1 – Wave System", subtitle: "WAVE SYSTEM", nav: "Wave System" },
  { md: "SETUP_GUIDE.md", html: "cppd1-setup-guide.html", title: "CPPd1 – Setup Guide", subtitle: "SETUP GUIDE", nav: "Setup" },
  { md: "GAMEMODE_SETUP.md", html: "cppd1-gamemode.html", title: "CPPd1 – Gamemode Setup", subtitle: "GAMEMODE SETUP", nav: "Gamemode" },
  { md: "SYNC_WITH_LIFE_REPO.md", html: "cppd1-sync-life.html", title: "CPPd1 – Sync with Life Repo", subtitle: "SYNC WITH LIFE REPO", nav: "Sync" },
  { md: "README.md", html: "cppd1-readme.html", title: "CPPd1 – Readme", subtitle: "README", nav: "Readme" },
];

function compileCppd1Guides() {
  const templatePath = path.join(ROOT, "subpages", "2026.html");
  const cppd1Root = path.join(ROOT, "projects", "CPPd1");
  marked.setOptions({ breaks: true, gfm: true });
  for (const g of CPPD1_GUIDES) {
    const mdPath = path.join(cppd1Root, g.md);
    if (!fs.existsSync(mdPath)) {
      console.warn("compile-all: " + path.relative(ROOT, mdPath) + " not found, skipping.");
      continue;
    }
    const md = fs.readFileSync(mdPath, "utf8");
    const bodyHtml = marked.parse(md);
    makeCppd1Page(
      templatePath,
      path.join(ROOT, "subpages", g.html),
      g.title,
      "CPPd1 / P2G4W",
      g.subtitle,
      g.nav,
      bodyHtml
    );
    console.log("compile-all: projects/CPPd1/" + g.md + " → subpages/" + g.html);
  }
}

function compileCppd1Hub() {
  const templatePath = path.join(ROOT, "subpages", "2026.html");
  const htmlPath = path.join(ROOT, "subpages", "cppd1.html");
  const links = [
    { href: "gdd.html", label: "Game Design Document (GDD)" },
    { href: "p2g4w-ue5-guide.html", label: "P2G4W UE5 Scripts Guide" },
    { href: "cppd1-ninja-setup.html", label: "Ninja Setup" },
    { href: "cppd1-cube-ninja.html", label: "Cube Ninja Model" },
    { href: "cppd1-marauders.html", label: "Marauders Setup" },
    { href: "cppd1-workflow.html", label: "Workflow: Cursor to UE5" },
    { href: "cppd1-cpp-reference.html", label: "C++ in UE5 Reference" },
    { href: "cppd1-quick-start.html", label: "Quick Start Guide" },
    { href: "cppd1-wave-system.html", label: "Wave System Guide" },
    { href: "cppd1-setup-guide.html", label: "Setup Guide" },
    { href: "cppd1-gamemode.html", label: "Gamemode Setup" },
    { href: "cppd1-sync-life.html", label: "Sync with Life Repo" },
    { href: "cppd1-readme.html", label: "CPPd1 Readme" },
  ];
  const listHtml = links.map((l) => '<li><a href="' + l.href + '">' + l.label + "</a></li>").join("\n");
  const bodyHtml = "<p>P2G4W (Two Gods) UE5 project — CPPd1. Docs and guides:</p>\n<ul>\n" + listHtml + "\n</ul>";
  makeCppd1Page(
    templatePath,
    htmlPath,
    "CPPd1 / P2G4W – Project Hub",
    "CPPd1 / P2G4W",
    "PROJECT HUB & GUIDES",
    "CPPd1",
    bodyHtml
  );
  console.log("compile-all: subpages/cppd1.html (hub)");
}

function syncUE5Scripts() {
  // Import and run the sync script
  const syncScriptPath = path.join(ROOT, "data", "games", "suggested-scripts", "ue5", "sync-to-ue.js");
  if (!fs.existsSync(syncScriptPath)) {
    console.warn("compile-all: sync-to-ue.js not found, skipping UE5 sync.");
    return;
  }
  
  // Run the sync script as a module (quiet mode for compile-all)
  try {
    const syncModule = require(syncScriptPath);
    const result = syncModule.main(true); // quiet = true
    console.log(`compile-all: Synced ${result.synced} UE5 scripts to CoPlago project`);
  } catch (error) {
    console.warn("compile-all: UE5 sync failed:", error.message);
  }
}

function main() {
  compileGdd();
  compileP2G4WGuide();
  compileCppd1Guides();
  compileCppd1Hub();
  syncUE5Scripts();
  console.log("compile-all: done.");
}

main();
