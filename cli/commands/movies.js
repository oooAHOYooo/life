'use strict'

const store = require('../store')

module.exports = function movies(action, _sub, flags) {
  switch (action) {
    case 'list':   return list(flags)
    case 'add':    return add(flags)
    case 'update': return update(flags)
    case 'delete': return del(flags)
    default: throw new Error(`Unknown movies action: ${action}`)
  }
}

function list(flags) {
  const data = store.readAll('movies')
  let months = Object.keys(data).sort()

  if (flags.month) {
    months = months.filter(m => m === flags.month)
  }

  const result = []
  months.forEach(month => {
    let movies = data[month] || []

    if (flags.watched !== undefined) {
      const watched = flags.watched === 'true' || flags.watched === true
      movies = movies.filter(m => m.watched === watched)
    }

    movies = movies.map((movie, idx) => ({
      ...movie,
      month,
      _index: idx,
    }))

    result.push(...movies)
  })

  return { count: result.length, items: result }
}

function add(flags) {
  if (!flags.month) throw new Error('--month is required (format: YYYY-MM)')
  if (!flags.title) throw new Error('--title is required')

  const data = store.readAll('movies')
  if (!data[flags.month]) data[flags.month] = []

  if (data[flags.month].some(m => m.title === flags.title)) {
    throw new Error(`Movie already exists in ${flags.month}: ${flags.title}`)
  }

  const movie = {
    title: flags.title,
    watched: flags.watched === 'true' ? true : false,
    addedDate: new Date().toISOString(),
    watchedDate: flags.watchedDate || '',
    rating: flags.rating ? parseInt(flags.rating, 10) : null,
    poster: flags.poster || '',
  }

  data[flags.month].push(movie)
  store.write('movies', data)

  return { created: movie }
}

function update(flags) {
  if (!flags.month) throw new Error('--month is required (format: YYYY-MM)')
  if (!flags.title) throw new Error('--title is required')

  const data = store.readAll('movies')
  if (!data[flags.month]) throw new Error(`Month not found: ${flags.month}`)

  const movie = data[flags.month].find(m => m.title === flags.title)
  if (!movie) throw new Error(`Movie not found in ${flags.month}: ${flags.title}`)

  if (flags.watched !== undefined) {
    movie.watched = flags.watched === 'true' ? true : false
    if (movie.watched && !movie.watchedDate) {
      movie.watchedDate = new Date().toISOString()
    }
  }
  if (flags.rating !== undefined) movie.rating = parseInt(flags.rating, 10)
  if (flags.poster !== undefined) movie.poster = flags.poster
  if (flags.watchedDate !== undefined) movie.watchedDate = flags.watchedDate

  store.write('movies', data)

  return { updated: movie }
}

function del(flags) {
  if (!flags.month) throw new Error('--month is required (format: YYYY-MM)')
  if (!flags.title) throw new Error('--title is required')

  const data = store.readAll('movies')
  if (!data[flags.month]) throw new Error(`Month not found: ${flags.month}`)

  const index = data[flags.month].findIndex(m => m.title === flags.title)
  if (index === -1) throw new Error(`Movie not found in ${flags.month}: ${flags.title}`)

  const deleted = data[flags.month].splice(index, 1)[0]
  store.write('movies', data)

  return { deleted }
}
