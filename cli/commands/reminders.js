'use strict'

const store = require('../store')

module.exports = function reminders(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'get':    return get(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    case 'delete': return del(flags)
    default: throw new Error(`Unknown reminders action: ${action}`)
  }
}

function list(flags) {
  let items = store.read('reminders')

  if (flags.category) items = items.filter(r => r.category === flags.category)
  if (flags.action) items = items.filter(r => r.desiredAction === flags.action)

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function get(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('reminders')
  const found = items.find(r => r.id === flags.id)

  if (!found) throw new Error(`Reminder not found: ${flags.id}`)

  return { item: found }
}

function add(flags) {
  if (!flags.name) throw new Error('--name is required')
  if (!flags.vendor) throw new Error('--vendor is required')
  if (!flags.renewsOn) throw new Error('--renewsOn is required')

  const items = store.read('reminders')
  const id = 'reminder-' + Date.now()

  const item = {
    id,
    category: flags.category || '',
    name: flags.name,
    vendor: flags.vendor,
    planName: flags.planName || '',
    devicesAllowed: flags.devicesAllowed || '',
    devicesInUse: flags.devicesInUse || '',
    purchaseDate: flags.purchaseDate || '',
    renewsOn: flags.renewsOn,
    billingPeriod: flags.billingPeriod || 'Monthly',
    url: flags.url || '',
    notes: flags.notes || '',
    desiredAction: flags.desiredAction || '',
    actionPolicy: flags.actionPolicy || '',
    actionOnOrAfter: flags.actionOnOrAfter || '',
    notifyDaysBefore: flags.notifyDaysBefore || 7,
  }

  items.push(item)
  store.write('reminders', items)

  return { created: item }
}

function update(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('reminders')
  const found = items.find(r => r.id === flags.id)

  if (!found) throw new Error(`Reminder not found: ${flags.id}`)

  const item = found

  if (flags.name !== undefined) item.name = flags.name
  if (flags.vendor !== undefined) item.vendor = flags.vendor
  if (flags.category !== undefined) item.category = flags.category
  if (flags.renewsOn !== undefined) item.renewsOn = flags.renewsOn
  if (flags.billingPeriod !== undefined) item.billingPeriod = flags.billingPeriod
  if (flags.notes !== undefined) item.notes = flags.notes
  if (flags.desiredAction !== undefined) item.desiredAction = flags.desiredAction
  if (flags.url !== undefined) item.url = flags.url

  store.write('reminders', items)

  return { updated: item }
}

function del(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('reminders')
  const index = items.findIndex(r => r.id === flags.id)

  if (index === -1) throw new Error(`Reminder not found: ${flags.id}`)

  const deleted = items.splice(index, 1)[0]
  store.write('reminders', items)

  return { deleted }
}
