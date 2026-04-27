'use strict'

const fs = require('fs')
const path = require('path')

const DATA_DIR = path.join(__dirname, '../data')

const STORES = {
  goals:              { file: 'goals.json',                  type: 'array' },
  jobs:               { file: 'jobs.json',                   type: 'array' },
  'job-search':       { file: 'job-search.json',             type: 'singleton' },
  bookmarks:          { file: 'bookmarks.json',              type: 'array' },
  'writing-projects': { file: 'writing-projects.json',       type: 'array' },
  'writing-drafts':   { file: 'writing-drafts.json',         type: 'array' },
  expenses:           { file: 'expenses.json',               type: 'wrapped', key: 'months' },
  bills:              { file: 'bills.json',                  type: 'wrapped', key: 'categories' },
  reminders:          { file: 'reminders.json',              type: 'array' },
  movies:             { file: 'movies-to-see.json',          type: 'singleton' },
  decisions:          { file: '2026-decisions-vision.json',  type: 'array' },
  'beep-bots':        { file: 'beep-bots.json',              type: 'array' },
  'beep-feed':        { file: 'beep-feed.json',              type: 'array' },
  'beep-products':    { file: 'beep-products.json',          type: 'array' },
  barnacle:           { file: 'barnacle-projects.json',      type: 'array' },
}

function read(name) {
  const store = STORES[name]
  if (!store) throw new Error(`Unknown store: ${name}`)

  const filePath = path.join(DATA_DIR, store.file)
  if (!fs.existsSync(filePath)) throw new Error(`File not found: ${store.file}`)

  const content = fs.readFileSync(filePath, 'utf8')
  const data = JSON.parse(content)

  if (store.type === 'array') return data
  if (store.type === 'singleton') return data
  if (store.type === 'wrapped') return data[store.key] || []

  throw new Error(`Unknown store type: ${store.type}`)
}

function readAll(name) {
  const store = STORES[name]
  if (!store) throw new Error(`Unknown store: ${name}`)

  const filePath = path.join(DATA_DIR, store.file)
  if (!fs.existsSync(filePath)) throw new Error(`File not found: ${store.file}`)

  const content = fs.readFileSync(filePath, 'utf8')
  return JSON.parse(content)
}

function write(name, data) {
  const store = STORES[name]
  if (!store) throw new Error(`Unknown store: ${name}`)

  const filePath = path.join(DATA_DIR, store.file)
  const tmpPath = filePath + '.tmp'

  const dataToWrite = (store.type === 'wrapped' || store.type === 'singleton')
    ? data
    : data

  fs.writeFileSync(tmpPath, JSON.stringify(dataToWrite, null, 2) + '\n')
  fs.renameSync(tmpPath, filePath)
}

function generateId(items, title) {
  if (!title) return 'item-' + Math.random().toString(36).substr(2, 9)

  const slug = title
    .toLowerCase()
    .trim()
    .replace(/\s+/g, '-')
    .replace(/[^a-z0-9\-]/g, '')
    .replace(/^-+|-+$/g, '')

  let id = slug
  let count = 1
  while (items && items.some(item => item.id === id)) {
    id = slug + '-' + count
    count++
  }

  return id || 'item-' + Math.random().toString(36).substr(2, 9)
}

function findById(items, idOrTitle) {
  if (!items) return null

  for (let i = 0; i < items.length; i++) {
    const item = items[i]
    if (item.id === idOrTitle) return { item, index: i }
    if (item.title === idOrTitle) return { item, index: i }
  }

  const idx = parseInt(idOrTitle, 10)
  if (!isNaN(idx) && idx >= 0 && idx < items.length) {
    return { item: items[idx], index: idx }
  }

  return null
}

module.exports = {
  read,
  readAll,
  write,
  generateId,
  findById,
  STORES,
}
