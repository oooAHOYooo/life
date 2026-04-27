'use strict'

const store = require('../store')

module.exports = function bookmarks(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'get':    return get(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    case 'delete': return del(flags)
    default: throw new Error(`Unknown bookmarks action: ${action}`)
  }
}

function list(flags) {
  let items = store.read('bookmarks')

  if (flags.search) {
    const query = flags.search.toLowerCase()
    items = items.filter(b =>
      b.title.toLowerCase().includes(query) ||
      b.description.toLowerCase().includes(query) ||
      b.url.toLowerCase().includes(query)
    )
  }

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function get(flags) {
  if (!flags.id && !flags.url) throw new Error('--id or --url is required')

  const items = store.read('bookmarks')
  let found = null

  if (flags.id) {
    found = store.findById(items, flags.id)
  } else if (flags.url) {
    for (let i = 0; i < items.length; i++) {
      if (items[i].url === flags.url) {
        found = { item: items[i], index: i }
        break
      }
    }
  }

  if (!found) throw new Error(`Bookmark not found: ${flags.id || flags.url}`)

  return { item: found.item }
}

function add(flags) {
  if (!flags.title) throw new Error('--title is required')
  if (!flags.url) throw new Error('--url is required')

  const items = store.read('bookmarks')

  const item = {
    title: flags.title,
    url: flags.url,
    description: flags.description || '',
  }

  items.push(item)
  store.write('bookmarks', items)

  return { created: item }
}

function update(flags) {
  if (!flags.id && !flags.url) throw new Error('--id or --url is required')

  const items = store.read('bookmarks')
  let found = null

  if (flags.id) {
    found = store.findById(items, flags.id)
  } else if (flags.url) {
    for (let i = 0; i < items.length; i++) {
      if (items[i].url === flags.url) {
        found = { item: items[i], index: i }
        break
      }
    }
  }

  if (!found) throw new Error(`Bookmark not found: ${flags.id || flags.url}`)

  const item = found.item

  if (flags.title !== undefined) item.title = flags.title
  if (flags.url !== undefined) item.url = flags.url
  if (flags.description !== undefined) item.description = flags.description

  store.write('bookmarks', items)

  return { updated: item }
}

function del(flags) {
  if (!flags.id && !flags.url) throw new Error('--id or --url is required')

  const items = store.read('bookmarks')
  let found = null

  if (flags.id) {
    found = store.findById(items, flags.id)
  } else if (flags.url) {
    for (let i = 0; i < items.length; i++) {
      if (items[i].url === flags.url) {
        found = { item: items[i], index: i }
        break
      }
    }
  }

  if (!found) throw new Error(`Bookmark not found: ${flags.id || flags.url}`)

  const deleted = items.splice(found.index, 1)[0]
  store.write('bookmarks', items)

  return { deleted }
}
