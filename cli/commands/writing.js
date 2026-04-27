'use strict'

const store = require('../store')

module.exports = function writing(action, subResource, flags) {
  if (!subResource) throw new Error('Usage: writing <projects|drafts> <action>')

  if (subResource === 'projects') {
    return handleProjects(action, flags)
  } else if (subResource === 'drafts') {
    return handleDrafts(action, flags)
  } else {
    throw new Error(`Unknown writing resource: ${subResource}`)
  }
}

function handleProjects(action, flags) {
  switch (action) {
    case 'list':   return projectsList(flags)
    case 'get':    return projectsGet(flags)
    case 'add':    return projectsAdd(flags)
    case 'update': return projectsUpdate(flags)
    case 'delete': return projectsDelete(flags)
    default: throw new Error(`Unknown action: ${action}`)
  }
}

function handleDrafts(action, flags) {
  switch (action) {
    case 'list':   return draftsList(flags)
    case 'get':    return draftsGet(flags)
    case 'add':    return draftsAdd(flags)
    case 'update': return draftsUpdate(flags)
    case 'delete': return draftsDelete(flags)
    default: throw new Error(`Unknown action: ${action}`)
  }
}

function projectsList(flags) {
  let items = store.read('writing-projects')

  if (flags.status) items = items.filter(p => p.status === flags.status)
  if (flags.category) items = items.filter(p => p.category === flags.category)

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function projectsGet(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('writing-projects')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Project not found: ${flags.id}`)

  return { item: found.item }
}

function projectsAdd(flags) {
  if (!flags.title) throw new Error('--title is required')
  if (!flags.category) throw new Error('--category is required')

  const items = store.read('writing-projects')
  const id = store.generateId(items, flags.title)

  const item = {
    id,
    title: flags.title,
    description: flags.description || '',
    category: flags.category,
    status: flags.status || 'In Progress',
    wordCount: parseInt(flags.wordCount || '0', 10),
    lastUpdated: new Date().toISOString(),
    notes: flags.notes || '',
    filePath: flags.filePath || '',
  }

  items.push(item)
  store.write('writing-projects', items)

  return { created: item }
}

function projectsUpdate(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('writing-projects')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Project not found: ${flags.id}`)

  const item = found.item

  if (flags.title !== undefined) item.title = flags.title
  if (flags.description !== undefined) item.description = flags.description
  if (flags.category !== undefined) item.category = flags.category
  if (flags.status !== undefined) item.status = flags.status
  if (flags.wordCount !== undefined) item.wordCount = parseInt(flags.wordCount, 10)
  if (flags.notes !== undefined) item.notes = flags.notes
  if (flags.filePath !== undefined) item.filePath = flags.filePath

  item.lastUpdated = new Date().toISOString()
  store.write('writing-projects', items)

  return { updated: item }
}

function projectsDelete(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('writing-projects')
  const found = store.findById(items, flags.id)

  if (!found) throw new Error(`Project not found: ${flags.id}`)

  const deleted = items.splice(found.index, 1)[0]
  store.write('writing-projects', items)

  return { deleted }
}

function draftsList(flags) {
  let items = store.read('writing-drafts')

  if (flags.projectId) items = items.filter(d => d.projectId === flags.projectId)
  if (flags.status) items = items.filter(d => d.status === flags.status)

  items = items.map((item, idx) => ({
    ...item,
    _index: idx,
  }))

  return { count: items.length, items }
}

function draftsGet(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('writing-drafts')
  const found = items.find(d => d.id === flags.id)

  if (!found) throw new Error(`Draft not found: ${flags.id}`)

  return { item: found }
}

function draftsAdd(flags) {
  if (!flags.title) throw new Error('--title is required')
  if (!flags.projectId) throw new Error('--projectId is required')

  const items = store.read('writing-drafts')
  const id = 'draft-' + Date.now()

  const item = {
    id,
    title: flags.title,
    projectId: flags.projectId,
    version: flags.version || '1.0',
    fileType: flags.fileType || 'doc',
    filePath: flags.filePath || '',
    wordCount: parseInt(flags.wordCount || '0', 10),
    lastModified: new Date().toISOString(),
    status: flags.status || 'In Progress',
    notes: flags.notes || '',
    tags: flags.tags ? flags.tags.split(',').map(t => t.trim()) : [],
  }

  items.push(item)
  store.write('writing-drafts', items)

  return { created: item }
}

function draftsUpdate(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('writing-drafts')
  const found = items.find(d => d.id === flags.id)

  if (!found) throw new Error(`Draft not found: ${flags.id}`)

  const item = found

  if (flags.title !== undefined) item.title = flags.title
  if (flags.version !== undefined) item.version = flags.version
  if (flags.fileType !== undefined) item.fileType = flags.fileType
  if (flags.filePath !== undefined) item.filePath = flags.filePath
  if (flags.wordCount !== undefined) item.wordCount = parseInt(flags.wordCount, 10)
  if (flags.status !== undefined) item.status = flags.status
  if (flags.notes !== undefined) item.notes = flags.notes
  if (flags.tags !== undefined) item.tags = flags.tags.split(',').map(t => t.trim())

  item.lastModified = new Date().toISOString()
  store.write('writing-drafts', items)

  return { updated: item }
}

function draftsDelete(flags) {
  if (!flags.id) throw new Error('--id is required')

  const items = store.read('writing-drafts')
  const index = items.findIndex(d => d.id === flags.id)

  if (index === -1) throw new Error(`Draft not found: ${flags.id}`)

  const deleted = items.splice(index, 1)[0]
  store.write('writing-drafts', items)

  return { deleted }
}
