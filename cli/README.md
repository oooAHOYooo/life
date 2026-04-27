# life CLI

An agent-friendly CLI for managing your personal life data. All commands output JSON when piped or called by an agent, human-readable tables when running interactively.

## Usage

```bash
node cli/life.js <resource> <action> [--flags]
# or
npm run life -- <resource> <action> [--flags]
```

### Goals

```bash
life goals list [--status <s>] [--category <c>] [--priority <p>]
life goals get --id <id>
life goals add --title <t> [--category <c>] [--priority <p>] [--status <s>] [--description <d>]
life goals update --id <id> [--status <s>] [--priority <p>] [--title <t>] [--description <d>]
life goals delete --id <id>
```

### Jobs

```bash
life jobs list [--category <c>] [--type <t>]
life jobs get --id <id>
life jobs add --title <t> --organization <o> [--category <c>] [--type <t>] [--url <u>] [--description <d>]
life jobs update --id <id> [--title <t>] [--organization <o>]
life jobs delete --id <id>
```

### Job Search (Singleton Config)

```bash
life job-search get
life job-search set-mission --value <text>
life job-search set-pitch --value <text>
life job-search checklist-add --item <text>
life job-search checklist-done --item <text>
```

### Bookmarks

```bash
life bookmarks list [--search <query>]
life bookmarks get --id <id>
life bookmarks add --title <t> --url <u> [--description <d>]
life bookmarks update --id <id> [--title <t>] [--url <u>] [--description <d>]
life bookmarks delete --id <id>
```

### Writing

```bash
life writing projects list [--status <s>] [--category <c>]
life writing projects get --id <id>
life writing projects add --title <t> --category <c> [--status <s>] [--description <d>] [--wordCount <n>]
life writing projects update --id <id> [--status <s>] [--wordCount <n>]
life writing projects delete --id <id>

life writing drafts list [--projectId <id>] [--status <s>]
life writing drafts get --id <id>
life writing drafts add --title <t> --projectId <id> [--fileType <ft>] [--notes <n>]
life writing drafts update --id <id> [--status <s>] [--wordCount <n>]
life writing drafts delete --id <id>
```

### Expenses

```bash
life expenses list
life expenses get --month <YYYY-MM>
life expenses add --month <YYYY-MM> --total <n>
life expenses update --month <YYYY-MM> --total <n>
```

### Bills

```bash
life bills list [--category <id>]
life bills get-category --id <id>
life bills add-item --category <id> --name <n> --amount <a> [--frequency <f>]
life bills update-item --category <id> --name <n> [--amount <a>] [--frequency <f>]
life bills delete-item --category <id> --name <n>
```

### Reminders

```bash
life reminders list [--category <c>]
life reminders get --id <id>
life reminders add --name <n> --vendor <v> --renewsOn <date> [--category <c>]
life reminders update --id <id> [--renewsOn <date>] [--notes <n>]
life reminders delete --id <id>
```

### Movies

```bash
life movies list [--month <YYYY-MM>] [--watched true|false]
life movies add --month <YYYY-MM> --title <t> [--watched false]
life movies update --month <YYYY-MM> --title <t> [--watched true] [--rating <n>]
life movies delete --month <YYYY-MM> --title <t>
```

### Decisions

```bash
life decisions list
life decisions get --id <id>
life decisions add --title <t> [--description <d>]
life decisions update --id <id> [--title <t>] [--description <d>]
life decisions delete --id <id>
```

### BEEP Game Data

```bash
life beep bots list [--specialty <s>]
life beep bots get --id <id>
life beep feed list [--limit <n>] [--type <t>]
life beep products list [--category <c>]
```

## Output Formats

### Human (TTY)

```bash
$ life goals list
3 result(s)
─────────────────────────────────────────────────────────────
[0] Ship Ahoy MVP           | High     | In Progress
    Category: Personal
```

### JSON (Piped / Agent)

```bash
$ life goals list | jq .
{
  "count": 3,
  "items": [
    {
      "id": "ship-ahoy-mvp",
      "title": "Ship Ahoy MVP",
      "status": "In Progress",
      ...
    }
  ]
}
```

### Auto-Detection

- If `stdout` is a TTY (interactive terminal): human output
- If `stdout` is piped or non-interactive: JSON
- Use `--json` to force JSON output always

## ID Matching

Most commands accept `--id` which can be:

1. **Exact slug ID**: `ship-ahoy-mvp` (matches `.id` field)
2. **Exact title**: `"Ship Ahoy MVP"` (matches `.title` field)
3. **Array index**: `0` (numeric; matches position in list output via `_index`)

Example:
```bash
life goals get --id ship-ahoy-mvp           # by slug
life goals get --id "Ship Ahoy MVP"         # by title
life goals get --id 0                       # by index
```

## Examples for Agents

```bash
# Check high-priority in-progress goals
node cli/life.js goals list --status "In Progress" --priority High | jq '.items | length'

# Update a goal status
node cli/life.js goals update --id ship-ahoy-mvp --status Done

# Add a job lead
node cli/life.js jobs add \
  --title "Engineer" \
  --organization "Anthropic" \
  --category "AI" \
  --type "Full-time" \
  --url "https://jobs.anthropic.com/..."

# Get upcoming renewals
node cli/life.js reminders list --category subscription

# Add a movie to watch
node cli/life.js movies add --month 2026-04 --title "Sinners"

# Check job search mission
node cli/life.js job-search get | jq '.jobSearch.mission'
```

## Technical Details

- **Storage**: All data in `/data/` as JSON files
- **Atomicity**: Writes use temp file + rename to prevent corruption
- **No dependencies**: Vanilla Node.js only
- **ID generation**: Auto-slugs from titles for records without IDs
- **Filtering**: Each resource supports relevant filters (status, category, etc.)
- **Wrapped stores**: `bills` and `expenses` have special handling for nested data
- **Singletons**: `job-search` and `movies` are whole-document reads/writes
