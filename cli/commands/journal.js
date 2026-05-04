'use strict'

const writing = require('./writing')

module.exports = function journal(action, subResource, flags) {
  // Map journal commands to writing drafts for the 'personal-journal' project
  flags.projectId = 'personal-journal'
  
  return writing('drafts', action, flags)
}
