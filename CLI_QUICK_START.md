# life CLI — Quick Start

Your personal OS now has a **command-line interface** that an AI agent (or you) can call anytime to read and update all your life data.

## What It Does

The CLI lets you manage all your personal data from the command line:

- **Goals** — track progress, status, priority
- **Jobs** — maintain job search leads
- **Job Search** — update mission, pitch, daily checklist
- **Bookmarks** — organize 190+ bookmarks with search
- **Writing** — manage projects and drafts
- **Expenses** — track monthly spending
- **Bills** — manage subscriptions and renewals
- **Reminders** — renewal/subscription dates
- **Movies** — movies to watch by month
- **Decisions** — decision log
- **BEEP** — game data (read-only)

## Get Started

```bash
# Show help
node cli/life.js --help
node cli/life.js goals --help

# List your in-progress goals
node cli/life.js goals list --status "In Progress"

# Add a new goal
node cli/life.js goals add --title "Ship X" --priority High --category "Work"

# Update a goal status
node cli/life.js goals update --id ship-x --status "Done"

# Get your job search mission
node cli/life.js job-search get

# List movies to watch this month
node cli/life.js movies list --month 2026-04

# Use npm shortcut
npm run life -- goals list
```

## Key Design: JSON-First for Agents

**It auto-detects your context:**

- **In a terminal?** Pretty-printed human-readable output ✓
- **Piped to `jq` or another command?** JSON automatically ✓
- **Called by an AI agent?** JSON by default ✓

You never need to add `--json` flags manually—the CLI figures it out.

```bash
# Human: see pretty table
$ node cli/life.js goals list

# Agent/pipe: gets JSON automatically
$ node cli/life.js goals list | jq '.count'
```

## Agent Use Cases

An AI agent can now:

```bash
# Check your workload
node cli/life.js goals list --status "In Progress" | jq '.count'

# See if a goal exists
node cli/life.js goals list | jq '.items[] | select(.title | contains("Ahoy"))'

# Update goal status after a meeting
node cli/life.js goals update --id ship-ahoy-mvp --status "In Progress"

# Log a new job lead
node cli/life.js jobs add --title "Staff Engineer" --organization "Anthropic" \
  --category AI --type "Full-time" --url "https://jobs.anthropic.com/..."

# Add a movie to watch later
node cli/life.js movies add --month 2026-05 --title "Dune Part Three"

# Check upcoming renewals
node cli/life.js reminders list --category subscription

# Query complex data
node cli/life.js job-search get | jq '.jobSearch | {mission, pitch, deadline}'
```

## File Structure

```
cli/
├── life.js              # Entry point
├── router.js            # Command routing
├── store.js             # Data layer (read/write JSON)
├── output.js            # Output formatting
├── README.md            # Full documentation
└── commands/
    ├── goals.js         # CRUD for goals
    ├── jobs.js
    ├── job-search.js    # Singleton config
    ├── bookmarks.js
    ├── writing.js       # Projects + drafts
    ├── expenses.js      # Wrapped store
    ├── bills.js         # Wrapped store
    ├── reminders.js
    ├── movies.js        # Date-keyed singleton
    ├── decisions.js
    └── beep.js          # Game data
```

## Architecture

- **No external dependencies** — vanilla Node.js only
- **Atomic writes** — uses temp file + rename to prevent JSON corruption
- **ID generation** — auto-slugs titles for records without IDs
- **Smart matching** — finds items by `.id`, `.title`, or array index
- **Wrapped stores** — special handling for `bills` (nested under `categories`) and `expenses` (nested under `months`)
- **Singletons** — `job-search` and `movies-to-see` are whole-document reads/writes

## Command Syntax

```
life <resource> <action> [--flag value] [--json]
```

Most resources follow: `list`, `get`, `add`, `update`, `delete`

Special cases:
- `writing <projects|drafts>` — 3-level routing
- `beep <bots|feed|products>` — 3-level routing
- `job-search` — singleton operations (get, set-mission, set-pitch, checklist-add/done)

## Examples

### Goals
```bash
life goals list --status "In Progress" --priority High
life goals add --title "Launch feature" --category Work --priority High
life goals update --id launch-feature --status Done
```

### Writing
```bash
life writing projects list --status "In Progress"
life writing projects add --title "Novel Draft" --category fiction
life writing drafts add --title "Chapter 1" --projectId novel-draft
```

### Expenses
```bash
life expenses list
life expenses add --month 2026-04 --total 3500
life expenses update --month 2026-04 --total 3650
```

### Movies
```bash
life movies list --month 2026-04
life movies add --month 2026-04 --title "Sinners"
life movies update --month 2026-04 --title "Sinners" --watched true --rating 8
```

## Next Steps

1. **Try it out**: `node cli/life.js goals list --status "In Progress"`
2. **Read the docs**: `cli/README.md` has all commands and options
3. **Pipe to jq**: `node cli/life.js goals list | jq '.items | map(.priority)'`
4. **Let agents use it**: Just give them the command path, they'll handle the JSON

---

**Future:** This CLI is the foundation for:
- Real-time sync across devices
- Slack/email notifications on goal changes
- Integration with calendar/project management
- Mobile app consuming the same endpoints
- Multi-user collaboration
