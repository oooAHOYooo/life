'use strict'

const store = require('../store')

module.exports = function jobSearch(action, _sub, flags) {
  switch (action) {
    case 'get':           return get()
    case 'set-mission':   return setMission(flags)
    case 'set-pitch':     return setPitch(flags)
    case 'checklist-add': return checklistAdd(flags)
    case 'checklist-done': return checklistDone(flags)
    default: throw new Error(`Unknown job-search action: ${action}`)
  }
}

function get() {
  const data = store.readAll('job-search')
  return { jobSearch: data }
}

function setMission(flags) {
  if (!flags.value) throw new Error('--value is required')

  const data = store.readAll('job-search')
  data.mission = flags.value
  store.write('job-search', data)

  return { updated: { field: 'mission', value: data.mission } }
}

function setPitch(flags) {
  if (!flags.value) throw new Error('--value is required')

  const data = store.readAll('job-search')
  data.pitch = flags.value
  store.write('job-search', data)

  return { updated: { field: 'pitch', value: data.pitch } }
}

function checklistAdd(flags) {
  if (!flags.item) throw new Error('--item is required')

  const data = store.readAll('job-search')
  if (!data.dailyChecklist) data.dailyChecklist = []

  const checklistItem = {
    id: 'item-' + Date.now(),
    text: flags.item,
    done: false,
    addedDate: new Date().toISOString(),
  }

  data.dailyChecklist.push(checklistItem)
  store.write('job-search', data)

  return { created: checklistItem }
}

function checklistDone(flags) {
  if (!flags.item) throw new Error('--item is required')

  const data = store.readAll('job-search')
  if (!data.dailyChecklist) data.dailyChecklist = []

  const item = data.dailyChecklist.find(i =>
    i.text === flags.item || i.id === flags.item
  )

  if (!item) throw new Error(`Checklist item not found: ${flags.item}`)

  item.done = true
  item.doneDate = new Date().toISOString()
  store.write('job-search', data)

  return { updated: item }
}
