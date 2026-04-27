'use strict'

function out(data, flags) {
  const isJson = flags.json || !process.stdout.isTTY
  if (isJson) {
    process.stdout.write(JSON.stringify(data) + '\n')
  } else {
    prettyPrint(data)
  }
}

function err(error, flags) {
  const isJson = flags.json || !process.stdout.isTTY
  const errorObj = error instanceof Error
    ? { error: error.message, command: process.argv.slice(2).join(' ') }
    : { error: String(error), command: process.argv.slice(2).join(' ') }

  const output = isJson
    ? JSON.stringify(errorObj) + '\n'
    : `Error: ${errorObj.error}\n`

  process.stderr.write(output)
}

function prettyPrint(data) {
  if (Array.isArray(data)) {
    if (data.length === 0) {
      console.log('(empty)')
      return
    }

    const firstItem = data[0]
    if (typeof firstItem === 'object' && firstItem !== null && !Array.isArray(firstItem)) {
      console.table(data)
    } else {
      console.log(JSON.stringify(data, null, 2))
    }
  } else if (typeof data === 'object' && data !== null) {
    if (data.count !== undefined && data.items !== undefined) {
      console.log(`${data.count} result(s)`)
      if (data.items && data.items.length > 0) {
        console.table(data.items)
      }
    } else if (data.created !== undefined || data.updated !== undefined || data.deleted !== undefined) {
      const action = data.created ? 'created' : data.updated ? 'updated' : 'deleted'
      const item = data.created || data.updated || data.deleted
      console.log(`✓ ${action}`)
      console.log(JSON.stringify(item, null, 2))
    } else {
      console.log(JSON.stringify(data, null, 2))
    }
  } else {
    console.log(data)
  }
}

module.exports = {
  out,
  err,
}
