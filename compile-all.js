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

function main() {
  compileGdd();
  compileP2G4WGuide();
  console.log("compile-all: done.");
}

main();
