'use strict'

const store = require('../store')

module.exports = function decisions(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'get':    return get(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    case 'delete': return del(flags)
    default: throw new Error(`Unknown decisions action: ${action}`)
  }
}

function list(flags) {
  let items = store.read('decisions')

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function get(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('decisions')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Decision not found: ${flags.id}`)

  return { item: found.item }
}

function add(flags) {
  if (!flags.title) throw new Error('--title is required')

  const items = store.read('decisions')
  const id = store.generateId(items, flags.title)

  const item = {
    id,
    type: flags.type || 'decision',
    title: flags.title,
    description: flags.description || '',
    category: flags.category || 'General',
  }

  items.push(item)
  store.write('decisions', items)

  return { created: item }
}

function update(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('decisions')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Decision not found: ${flags.id}`)

  const item = found.item

  if (flags.title !== undefined) item.title = flags.title
  if (flags.description !== undefined) item.description = flags.description
  if (flags.category !== undefined) item.category = flags.category
  if (flags.type !== undefined) item.type = flags.type

  store.write('decisions', items)

  return { updated: item }
}

function del(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('decisions')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Decision not found: ${flags.id}`)

  const deleted = items.splice(found.index, 1)[0]
  store.write('decisions', items)

  return { deleted }
}
