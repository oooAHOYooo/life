'use strict'

const store = require('../store')

module.exports = function jobs(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'get':    return get(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    case 'delete': return del(flags)
    default: throw new Error(`Unknown jobs action: ${action}`)
  }
}

function list(flags) {
  let items = store.read('jobs')

  if (flags.category) items = items.filter(j => j.category === flags.category)
  if (flags.type) items = items.filter(j => j.type === flags.type)

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function get(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('jobs')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Job not found: ${flags.id}`)

  return { item: found.item }
}

function add(flags) {
  if (!flags.title) throw new Error('--title is required')
  if (!flags.organization) throw new Error('--organization is required')

  const items = store.read('jobs')
  const id = store.generateId(items, flags.title)

  const item = {
    id,
    title: flags.title,
    organization: flags.organization,
    description: flags.description || '',
    category: flags.category || '',
    location: flags.location || '',
    type: flags.type || '',
    url: flags.url || '',
  }

  items.push(item)
  store.write('jobs', items)

  return { created: item }
}

function update(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('jobs')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Job not found: ${flags.id}`)

  const item = found.item

  if (flags.title !== undefined) item.title = flags.title
  if (flags.organization !== undefined) item.organization = flags.organization
  if (flags.description !== undefined) item.description = flags.description
  if (flags.category !== undefined) item.category = flags.category
  if (flags.location !== undefined) item.location = flags.location
  if (flags.type !== undefined) item.type = flags.type
  if (flags.url !== undefined) item.url = flags.url

  store.write('jobs', items)

  return { updated: item }
}

function del(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('jobs')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Job not found: ${flags.id}`)

  const deleted = items.splice(found.index, 1)[0]
  store.write('jobs', items)

  return { deleted }
}
