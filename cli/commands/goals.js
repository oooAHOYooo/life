'use strict'

const store = require('../store')

const VALID_STATUSES = ['In Progress', 'Planning', 'Done', 'Blocked', 'On Hold']
const VALID_PRIORITIES = ['High', 'Medium', 'Low']

module.exports = function goals(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'get':    return get(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    case 'delete': return del(flags)
    default: throw new Error(`Unknown goals action: ${action}`)
  }
}

function list(flags) {
  let items = store.read('goals')

  if (flags.status)   items = items.filter(g => g.status === flags.status)
  if (flags.category) items = items.filter(g => g.category === flags.category)
  if (flags.priority) items = items.filter(g => g.priority === flags.priority)

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function get(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('goals')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Goal not found: ${flags.id}`)

  return { item: found.item }
}

function add(flags) {
  if (!flags.title) throw new Error('--title is required')

  const items = store.read('goals')
  const id = store.generateId(items, flags.title)

  const item = {
    id,
    title: flags.title,
    description: flags.description || '',
    category: flags.category || 'General',
    priority: flags.priority || 'Medium',
    status: flags.status || 'Planning',
  }

  if (VALID_PRIORITIES.indexOf(item.priority) === -1) {
    throw new Error(`Invalid priority. Must be one of: ${VALID_PRIORITIES.join(', ')}`)
  }

  if (VALID_STATUSES.indexOf(item.status) === -1) {
    throw new Error(`Invalid status. Must be one of: ${VALID_STATUSES.join(', ')}`)
  }

  items.push(item)
  store.write('goals', items)

  return { created: item }
}

function update(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('goals')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Goal not found: ${flags.id}`)

  const item = found.item

  if (flags.title !== undefined) item.title = flags.title
  if (flags.description !== undefined) item.description = flags.description
  if (flags.category !== undefined) item.category = flags.category
  if (flags.priority !== undefined) {
    if (VALID_PRIORITIES.indexOf(flags.priority) === -1) {
      throw new Error(`Invalid priority. Must be one of: ${VALID_PRIORITIES.join(', ')}`)
    }
    item.priority = flags.priority
  }
  if (flags.status !== undefined) {
    if (VALID_STATUSES.indexOf(flags.status) === -1) {
      throw new Error(`Invalid status. Must be one of: ${VALID_STATUSES.join(', ')}`)
    }
    item.status = flags.status
  }

  store.write('goals', items)

  return { updated: item }
}

function del(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('goals')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Goal not found: ${flags.id}`)

  const deleted = items.splice(found.index, 1)[0]
  store.write('goals', items)

  return { deleted }
}
