'use strict'

const store = require('../store')

module.exports = function beep(action, subResource, flags) {
  if (!subResource) throw new Error('Usage: beep <bots|feed|products> <action>')

  if (subResource === 'bots') {
    return handleBots(action, flags)
  } else if (subResource === 'feed') {
    return handleFeed(action, flags)
  } else if (subResource === 'products') {
    return handleProducts(action, flags)
  } else {
    throw new Error(`Unknown beep resource: ${subResource}`)
  }
}

function handleBots(action, flags) {
  switch (action) {
    case 'list': return botsList(flags)
    case 'get':  return botsGet(flags)
    default: throw new Error(`Unknown action: ${action}`)
  }
}

function handleFeed(action, flags) {
  switch (action) {
    case 'list': return feedList(flags)
    default: throw new Error(`Unknown action: ${action}`)
  }
}

function handleProducts(action, flags) {
  switch (action) {
    case 'list': return productsList(flags)
    default: throw new Error(`Unknown action: ${action}`)
  }
}

function botsList(flags) {
  let items = store.read('beep-bots')

  if (flags.specialty) items = items.filter(b => b.specialty === flags.specialty)
  if (flags.verified !== undefined) {
    const verified = flags.verified === 'true' || flags.verified === true
    items = items.filter(b => b.verified === verified)
  }

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function botsGet(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('beep-bots')
  const found = items.find(b => b.id === flags.id)

  if (!found) throw new Error(`Bot not found: ${flags.id}`)

  return { item: found }
}

function feedList(flags) {
  let items = store.read('beep-feed')
  const limit = flags.limit ? parseInt(flags.limit, 10) : 20

  if (flags.type) items = items.filter(f => f.type === flags.type)
  if (flags.botId) items = items.filter(f => f.botId === flags.botId)

  items = items
    .sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp))
    .slice(0, limit)
    .map((item, idx) => ({
      ...item,
      _index: idx,
    }))

  return { count: items.length, items }
}

function productsList(flags) {
  let items = store.read('beep-products')

  if (flags.category) items = items.filter(p => p.category === flags.category)

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}
