#!/usr/bin/env node
'use strict'

const { parseArgs, dispatch } = require('./router')
const output = require('./output')

const VERSION = '1.0.0'

function showHelp(resource) {
  if (!resource) {
    console.log(`life CLI v${VERSION}

Usage: life <resource> <action> [--flag value]

Resources:
  goals         Manage goals
  jobs          Job tracking
  job-search    Job search strategy (singleton)
  bookmarks     Bookmarks
  writing       Writing projects and drafts
  expenses      Expense tracking
  bills         Bills and subscriptions
  reminders     Renewal reminders
  movies        Movies to watch
  decisions     Decisions log
  beep          BEEP game data (read-only)

Options:
  --json        Force JSON output
  --help, -h    Show this help

Examples:
  life goals list --status "In Progress"
  life goals add --title "Ship X" --priority High
  life bookmarks list --search "ai"
  life writing projects list
  life job-search get

See 'life <resource> --help' for resource-specific commands.`)
    return
  }

  const helpText = {
    goals: `goals <action>

Actions:
  list           List goals [--status <s>] [--category <c>] [--priority <p>]
  get            Get goal [--id <id>]
  add            Add goal --title <t> [--category <c>] [--priority <p>] [--status <s>] [--description <d>]
  update         Update goal --id <id> [--status <s>] [--title <t>] [--priority <p>] [--description <d>]
  delete         Delete goal --id <id>`,

    jobs: `jobs <action>

Actions:
  list           List jobs [--category <c>] [--type <t>]
  get            Get job [--id <id>]
  add            Add job --title <t> --organization <o> [--category <c>] [--type <t>] [--url <u>] [--description <d>]
  update         Update job --id <id> [--title <t>] [--organization <o>]
  delete         Delete job --id <id>`,

    'job-search': `job-search <action>

Actions:
  get            Show job search strategy
  set-mission    Update mission --value <text>
  set-pitch      Update pitch --value <text>
  checklist-add  Add checklist item --item <text>
  checklist-done Mark checklist item done --item <text>`,

    bookmarks: `bookmarks <action>

Actions:
  list           List bookmarks [--search <query>]
  get            Get bookmark [--id <id>]
  add            Add bookmark --title <t> --url <u> [--description <d>]
  update         Update bookmark --id <id> [--title <t>] [--url <u>] [--description <d>]
  delete         Delete bookmark --id <id>`,

    writing: `writing <resource> <action>

Resources:
  projects       Writing projects
  drafts         Writing drafts

Examples:
  writing projects list [--status <s>] [--category <c>]
  writing projects add --title <t> --category <c> [--status <s>] [--description <d>] [--wordCount <n>]
  writing projects update --id <id> [--status <s>] [--wordCount <n>]
  writing drafts list [--projectId <id>] [--status <s>]
  writing drafts add --title <t> --projectId <id> [--fileType <ft>] [--notes <n>]
  writing drafts update --id <id> [--status <s>] [--wordCount <n>]`,

    expenses: `expenses <action>

Actions:
  list           List all months
  get            Get month [--month <YYYY-MM>]
  add            Add month --month <YYYY-MM> --total <n>
  update         Update month --month <YYYY-MM> --total <n>`,

    bills: `bills <action>

Actions:
  list           List bill categories
  get-category   Get category [--id <id>]
  add-item       Add bill item --category <id> --name <n> --amount <a> [--frequency <f>]
  update-item    Update bill item --category <id> --name <n> [--amount <a>] [--frequency <f>]
  delete-item    Delete bill item --category <id> --name <n>`,

    reminders: `reminders <action>

Actions:
  list           List reminders [--category <c>]
  get            Get reminder [--id <id>]
  add            Add reminder --name <n> --vendor <v> --renewsOn <date> [--category <c>]
  update         Update reminder --id <id> [--renewsOn <date>] [--notes <n>]
  delete         Delete reminder --id <id>`,

    movies: `movies <action>

Actions:
  list           List movies [--month <YYYY-MM>] [--watched true|false]
  add            Add movie --month <YYYY-MM> --title <t> [--watched false]
  update         Update movie --month <YYYY-MM> --title <t> [--watched true] [--rating <n>]
  delete         Delete movie --month <YYYY-MM> --title <t>`,

    decisions: `decisions <action>

Actions:
  list           List decisions
  get            Get decision [--id <id>]
  add            Add decision --title <t> [--description <d>]
  update         Update decision --id <id> [--title <t>] [--description <d>]
  delete         Delete decision --id <id>`,

    beep: `beep <resource> <action>

Resources:
  bots           AI bots
  feed           Social feed
  products       Products

Examples:
  beep bots list
  beep bots get --id <id>
  beep feed list [--limit <n>]
  beep products list`,
  }

  const text = helpText[resource]
  console.log(text || `No help available for '${resource}'`)
}

try {
  const argv = process.argv.slice(2)

  if (argv.length === 0 || argv[0] === '--help' || argv[0] === '-h' || argv[0] === '--version') {
    if (argv[0] === '--version') {
      console.log(`life CLI v${VERSION}`)
    } else {
      showHelp()
    }
    process.exit(0)
  }

  const { resource, subResource, action, flags } = parseArgs(argv)

  if (!resource || flags.help) {
    showHelp(resource)
    process.exit(0)
  }

  const result = dispatch(resource, subResource, action, flags)
  output.out(result, flags)
} catch (err) {
  output.err(err, {})
  process.exit(1)
}
