'use strict'

const store = require('../store')

module.exports = function bills(action, _sub, flags) {
  switch (action) {
    case 'list':        return list(flags)
    case 'get-category': return getCategory(flags)
    case 'add-item':    return addItem(flags)
    case 'update-item': return updateItem(flags)
    case 'delete-item': return deleteItem(flags)
    default: throw new Error(`Unknown bills action: ${action}`)
  }
}

function list(flags) {
  const data = store.readAll('bills')
  const categories = data.categories || []

  const items = categories.map((cat, idx) => ({
    ...cat,
    _index: idx,
    itemCount: cat.items ? cat.items.length : 0,
  }))

  return { count: items.length, generatedAt: data.generatedAt, items }
}

function getCategory(flags) {
  if (!flags.id) throw new Error('--id is required')

  const data = store.readAll('bills')
  const cat = (data.categories || []).find(c => c.id === flags.id)

  if (!cat) throw new Error(`Category not found: ${flags.id}`)

  return { category: cat }
}

function addItem(flags) {
  if (!flags.category) throw new Error('--category is required')
  if (!flags.name) throw new Error('--name is required')
  if (!flags.amount) throw new Error('--amount is required')

  const data = store.readAll('bills')
  const cat = (data.categories || []).find(c => c.id === flags.category)

  if (!cat) throw new Error(`Category not found: ${flags.category}`)

  if (!cat.items) cat.items = []

  const item = {
    name: flags.name,
    frequency: flags.frequency || 'Monthly',
    account: flags.account || '',
    assumedUsername: flags.assumedUsername || '',
    dueIn: flags.dueIn || '',
    dueInDays: flags.dueInDays || '',
    amount: parseFloat(flags.amount),
    notes: flags.notes || '',
  }

  cat.items.push(item)
  store.write('bills', data)

  return { created: item }
}

function updateItem(flags) {
  if (!flags.category) throw new Error('--category is required')
  if (!flags.name) throw new Error('--name is required')

  const data = store.readAll('bills')
  const cat = (data.categories || []).find(c => c.id === flags.category)

  if (!cat) throw new Error(`Category not found: ${flags.category}`)

  const item = (cat.items || []).find(i => i.name === flags.name)

  if (!item) throw new Error(`Item not found: ${flags.name}`)

  if (flags.frequency !== undefined) item.frequency = flags.frequency
  if (flags.amount !== undefined) item.amount = parseFloat(flags.amount)
  if (flags.account !== undefined) item.account = flags.account
  if (flags.dueIn !== undefined) item.dueIn = flags.dueIn
  if (flags.dueInDays !== undefined) item.dueInDays = flags.dueInDays
  if (flags.notes !== undefined) item.notes = flags.notes

  store.write('bills', data)

  return { updated: item }
}

function deleteItem(flags) {
  if (!flags.category) throw new Error('--category is required')
  if (!flags.name) throw new Error('--name is required')

  const data = store.readAll('bills')
  const cat = (data.categories || []).find(c => c.id === flags.category)

  if (!cat) throw new Error(`Category not found: ${flags.category}`)

  const index = (cat.items || []).findIndex(i => i.name === flags.name)

  if (index === -1) throw new Error(`Item not found: ${flags.name}`)

  const deleted = cat.items.splice(index, 1)[0]
  store.write('bills', data)

  return { deleted }
}
