# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

---

## Repository Overview

This is **LifeOS** — a personal information management dashboard and project hub. It's a single-page application (SPA) that serves as a personal OS, with a main dashboard (`index.html`) and multiple subpages for managing different life areas: goals, jobs, writing, games, expenses, and more.

### Key Projects
- **BEEP Trainer**: Pokémon-style AI bot battle game (currently in MVP phase)
- **CPPd1**: C++ learning/prototyping project with UE5 integration
- **Personal Dashboard**: Aggregates data from JSON files into a unified interface
- **Games Directory**: Experimental game prototypes

---

## Repository Structure

```
life/
├── index.html                    # Main dashboard (entry point)
├── design-system.css             # Shared design tokens and styles
├── compile-all.js                # Build script: compiles markdown docs to HTML
├── package.json                  # Dependencies (marked for markdown → HTML)
│
├── subpages/                     # Individual HTML pages (dashboard + projects)
│   ├── beep-game.html           # BEEP game interface (fully playable)
│   ├── gdd.html                 # GDD documentation (compiled from data/games/gdd.md)
│   ├── 2026.html                # 2026 goals & planning
│   ├── cppd1.html               # C++ learning dashboard
│   ├── job-search.html          # Job hunting tracker
│   ├── expenses.html            # Expense tracking
│   ├── writing.html             # Writing projects
│   └── [other life management pages]
│
├── data/                         # JSON data files (read by subpages)
│   ├── beep-bots.json           # BEEP game bot definitions
│   ├── beep-products.json       # BEEP cosmetics/shop items
│   ├── beep-feed.json           # BEEP game events feed
│   ├── goals.json               # 2026 goals
│   ├── jobs.json                # Job search entries
│   ├── expenses.json            # Expense records
│   ├── games/                   # Game documentation
│   │   ├── gdd.md               # Game design document (source)
│   │   └── suggested-scripts/   # UE5 integration guides
│   ├── writing-projects.json    # Writing project metadata
│   └── [other data files]
│
├── assets/                       # Images, icons, media
│
├── archive/                      # Old versions and deprecated pages
│
├── game/                         # Game template (deprecated)
│
├── games/                        # Game prototypes directory
│
├── experiments/                  # Experimental features
│
└── projects/                     # CPPd1 and other large projects
    └── CPPd1/                    # C++ learning project
        ├── Scripts/
        │   ├── build-cppd1.js    # Single build
        │   └── watch-and-build.js # Continuous rebuild
        └── ...
```

---

## Development Commands

### Dashboard & Documentation

```bash
# Compile markdown docs (GDD, UE5 guide) → HTML pages
npm run compile

# This runs: node compile-all.js
# - data/games/gdd.md → subpages/gdd.html
# - data/games/suggested-scripts/GUIDE.md → subpages/p2g4w-ue5-guide.html
```

### CPPd1 Project (C++ Game)

```bash
# One-time build of CPPd1 documentation/assets
npm run build-cppd1

# Continuous watch-mode rebuild (recommended during development)
npm run watch-cppd1

# Sync UE5 scripts to Unreal Engine project
npm run sync-ue5
```

### Local Development

```bash
# Run a local HTTP server (required for AJAX to work)
python3 -m http.server 8000

# Or using Node.js:
npx http-server
```

---

## Architecture Patterns

### Data-Driven Pages

Most subpages follow a pattern:
1. **HTML Template**: Define layout and styling
2. **Script Block**: Fetch JSON from `/data/` and populate the page
3. **Event Handlers**: User interactions update local state

Example: `/subpages/beep-game.html`
- Loads bot data from `/data/beep-bots.json`
- Loads shop items from `/data/beep-products.json`
- Uses client-side state management (no backend yet)
- Saves game state to `localStorage` (Phase 1 limitation)

### Dashboard Navigation

The main `index.html`:
- Renders navigation sidebar (links to all subpages)
- Has a Three.js canvas background (animated on password screen)
- Password-protected entry (checks `localStorage`)
- Responsive layout with sidebar + main content area

### Styling

All pages import `design-system.css`:
- CSS custom properties (--color-*, --spacing-*, --font-size-*)
- Consistent card, button, and form component styles
- Sidebar, topbar, and content area layout patterns

---

## BEEP Trainer — Detailed Info

### Current State (as of Mar 2026)

- ✅ **Frontend Game UI**: Fully playable single-player game
- ✅ **Game Mechanics**: Dashboard, battles, training, leaderboard
- ❌ **Backend**: Not started (needed for persistence across sessions)
- ❌ **Silverback Integration**: Not started (bots are deterministic, not AI-driven)
- ❌ **3D Visualization**: Not started (no Three.js ecosystem view yet)
- ❌ **Real Market Data**: Not started (no live financial data feeds)

### Files

- **Game**: `/subpages/beep-game.html` (39 KB, fully self-contained)
- **Bot Data**: `/data/beep-bots.json`
- **Shop Items**: `/data/beep-products.json`
- **Game Events**: `/data/beep-feed.json`

### Next Phase (Task #14-15)

1. **Polish UI**: Add animations, tutorials, visual feedback
2. **Build Backend**: Node.js/Express + PostgreSQL for persistence
3. **User Auth**: Auth0 integration for account management
4. **Save Progress**: Move from localStorage to database

See the "6-Month Development Roadmap" section at the end of this file for full Phase 1-6 plan.

---

## Adding a New Subpage

To add a new subpage to the dashboard:

1. **Create HTML file** in `/subpages/` (e.g., `new-feature.html`)
   - Copy layout from similar existing page
   - Import `design-system.css`
   - Use same nav bar pattern for consistency

2. **Add data file** in `/data/` if needed (e.g., `new-feature.json`)

3. **Add navigation link** in `index.html` sidebar

4. **If markdown source**, update `compile-all.js` to compile it:
   ```javascript
   function compileNewFeature() {
     makeDocPage(
       path.join(ROOT, "subpages", "2026.html"), // template
       path.join(ROOT, "data", "new-feature.md"),  // source
       path.join(ROOT, "subpages", "new-feature.html"), // output
       "New Feature",     // page title
       "My Feature",      // header h1
       "Description",     // header subtitle
       "New Feature"      // nav label
     );
   }
   ```

---

## Key Insights for Future Work

### BEEP is Phase 1 of Bigger Vision

The dashboard isn't just a personal tool—it's a prototype for BEEP's player hub. Same architecture:
- Data-driven frontend (JSON → UI)
- Real-time game state updates
- Persistence to backend
- Multiple views (dashboard, game, leaderboard, profile)

### Design System is Intentional

The CSS variables and component patterns exist because we're building toward:
- Themable UI (dark/light mode, cosmetics)
- Scalable to 100K+ users
- Consistent across all pages and subpages

### Avoid Over-Engineering Early Phases

- ❌ Don't rebuild with React until Phase 4
- ❌ Don't integrate Three.js until Phase 2 is approved
- ✅ Keep using vanilla HTML/CSS/JS for MVP speed
- ✅ Focus on gameplay and progression feel first

---

## BEEP 6-Month Roadmap (Reference)

### Phase 1: MVP Polish & Backend (Weeks 1-3)
- Fix UI bugs, add animations, create tutorial
- Node.js/Express backend + PostgreSQL + Auth0
- **Start here**

### Phase 2: Real Intelligence & Visualization (Weeks 4-8)
- Silverback SDK integration (bots autonomous)
- Three.js 3D force-directed graph (bots debating)
- **Depends on**: Phase 1 complete

### Phase 3: Market Data & Multiplayer (Weeks 9-13)
- Polygon.io integration (real S&P 500 data)
- Player matchmaking, 1v1 battles, ranking
- **Depends on**: Phase 2 complete

### Phase 4: Monetization (Weeks 14-17)
- Cosmetics shop (skins, titles, themes)
- Premium battle pass ($9.99/mo)
- **Depends on**: Phase 3 complete

### Phase 5: Enterprise & Esports (Weeks 18-22)
- Tournament bracket, regional leagues
- Enterprise API for fintech ($5K-100K/mo)
- **Depends on**: Phase 4 complete

### Phase 6: Launch (Weeks 23-26)
- AWS/Vercel deployment, monitoring, backups
- ProductHunt + Twitter + Reddit marketing
- **Depends on**: Phase 5 complete

**Timeline**: 6 months → acquisition-ready ($15-50M valuation)

---

## Technical Decisions

1. **Vanilla JS (not React)**: Faster MVP, deploy immediately. React in Phase 4+ if needed.
2. **localStorage → PostgreSQL**: Start simple, migrate to database in Phase 1b.
3. **Three.js for 3D**: Optional but essential for viral moment (Phase 2).
4. **Silverback Agents**: Real LLM autonomy (not scripted AI) — hedges against commoditization.
5. **Multiple Revenue Streams**: Never >40% from one source. Cosmetics + Pass + Enterprise API.

---

## Metrics to Track

- **Engagement**: DAU, session length, bot adoptions, battles per user
- **Monetization**: Conversion rate (target 10%), ARPU (target $2-5), cosmetic adoption
- **Growth**: User acquisition cost, viral coefficient, churn rate, retention D7/D30
- **Business**: MRR, gross margin (target 75%), CAC payback (target <3mo), NDR

---

## References

- **BEEP Concept**: Pokémon meets stock market simulator. Bots debate financial strategies using real LLM reasoning.
- **Acquisition Target**: OpenAI/Anthropic (validation platform), GitHub (code reviews), LinkedIn (B2B data), AWS/GCP, Stripe.
- **Pitch**: "BEEP is the trust layer for the AI economy. Same network effects as Yelp/Rotten Tomatoes, but for the $100B+ DevTools market."
