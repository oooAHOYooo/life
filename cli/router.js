'use strict'

function parseArgs(argv) {
  if (argv.length === 0) {
    return { resource: null, subResource: null, action: null, flags: { help: true } }
  }

  let resource = argv[0]
  let subResource = null
  let action = null
  let flagsStart = 1

  if (resource === 'writing' && argv.length > 1 && (argv[1] === 'projects' || argv[1] === 'drafts')) {
    subResource = argv[1]
    action = argv[2]
    flagsStart = 3
  } else if (resource === 'beep' && argv.length > 1 && (argv[1] === 'bots' || argv[1] === 'feed' || argv[1] === 'products')) {
    subResource = argv[1]
    action = argv[2]
    flagsStart = 3
  } else {
    action = argv[1]
    flagsStart = 2
  }

  const flags = parseFlags(argv.slice(flagsStart))

  return { resource, subResource, action, flags }
}

function parseFlags(argv) {
  const flags = {
    help: false,
    json: false,
  }

  for (let i = 0; i < argv.length; i++) {
    const arg = argv[i]

    if (arg === '--help' || arg === '-h') {
      flags.help = true
    } else if (arg === '--json') {
      flags.json = true
    } else if (arg.startsWith('--')) {
      const [key, val] = arg.substring(2).split('=')

      if (val !== undefined) {
        flags[key] = val
      } else if (i + 1 < argv.length && !argv[i + 1].startsWith('--')) {
        flags[key] = argv[i + 1]
        i++
      } else {
        flags[key] = true
      }
    }
  }

  return flags
}

const handlers = {
  goals:       require('./commands/goals'),
  jobs:        require('./commands/jobs'),
  'job-search': require('./commands/job-search'),
  bookmarks:   require('./commands/bookmarks'),
  writing:     require('./commands/writing'),
  expenses:    require('./commands/expenses'),
  bills:       require('./commands/bills'),
  reminders:   require('./commands/reminders'),
  movies:      require('./commands/movies'),
  decisions:   require('./commands/decisions'),
  beep:        require('./commands/beep'),
}

function dispatch(resource, subResource, action, flags) {
  const handler = handlers[resource]
  if (!handler) throw new Error(`Unknown resource: ${resource}`)

  return handler(action, subResource, flags)
}

module.exports = {
  parseArgs,
  dispatch,
}
