'use strict'

const store = require('../store')

module.exports = function expenses(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'get':    return get(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    default: throw new Error(`Unknown expenses action: ${action}`)
  }
}

function list(flags) {
  const data = store.readAll('expenses')
  const months = data.months || []

  const items = months.map((month, idx) => ({
    ...month,
    _index: idx,
  }))

  return { count: items.length, items }
}

function get(flags) {
  if (!flags.month) throw new Error('--month is required (format: YYYY-MM)')

  const data = store.readAll('expenses')
  const month = (data.months || []).find(m => m.month === flags.month)

  if (!month) throw new Error(`Month not found: ${flags.month}`)

  return { month }
}

function add(flags) {
  if (!flags.month) throw new Error('--month is required (format: YYYY-MM)')
  if (!flags.total) throw new Error('--total is required')

  const data = store.readAll('expenses')
  if (!data.months) data.months = []

  if (data.months.some(m => m.month === flags.month)) {
    throw new Error(`Month already exists: ${flags.month}`)
  }

  const month = {
    month: flags.month,
    total: parseFloat(flags.total),
  }

  data.months.push(month)
  data.months.sort((a, b) => a.month.localeCompare(b.month))
  store.write('expenses', data)

  return { created: month }
}

function update(flags) {
  if (!flags.month) throw new Error('--month is required (format: YYYY-MM)')
  if (!flags.total) throw new Error('--total is required')

  const data = store.readAll('expenses')
  const month = (data.months || []).find(m => m.month === flags.month)

  if (!month) throw new Error(`Month not found: ${flags.month}`)

  month.total = parseFloat(flags.total)

  store.write('expenses', data)

  return { updated: month }
}
