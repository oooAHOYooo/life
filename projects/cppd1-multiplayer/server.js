/**
 * CPPd1 Multiplayer – shared-world server
 * Serves the game and syncs player positions in rooms (Minecraft-style share link).
 */

import express from 'express';
import { WebSocketServer } from 'ws';
import { createServer } from 'http';
import { fileURLToPath } from 'url';
import { dirname, join } from 'path';

const __dirname = dirname(fileURLToPath(import.meta.url));
const PORT = process.env.PORT || 3742;

const app = express();
app.use(express.static(join(__dirname, 'public')));

app.get('/', (req, res) => {
  res.sendFile(join(__dirname, 'public', 'index.html'));
});

const server = createServer(app);
const wss = new WebSocketServer({ server });

// roomId -> Set of WebSockets
const rooms = new Map();
// socket -> { roomId, playerId, lastState }
const socketInfo = new Map();

function randomRoomId() {
  const chars = 'ABCDEFGHJKLMNPQRSTUVWXYZ23456789';
  let id = '';
  for (let i = 0; i < 5; i++) id += chars[Math.floor(Math.random() * chars.length)];
  return id;
}

function ensureRoom(roomId) {
  if (!rooms.has(roomId)) rooms.set(roomId, new Set());
  return rooms.get(roomId);
}

function broadcastToRoom(roomId, message, excludeSocket = null) {
  const room = rooms.get(roomId);
  if (!room) return;
  const payload = typeof message === 'string' ? message : JSON.stringify(message);
  room.forEach((ws) => {
    if (ws.readyState === 1 && ws !== excludeSocket) ws.send(payload);
  });
}

wss.on('connection', (ws, req) => {
  const url = new URL(req.url || '', `http://${req.headers.host}`);
  const roomId = url.searchParams.get('room') || randomRoomId();
  const room = ensureRoom(roomId);
  const playerId = `p${Date.now()}_${Math.random().toString(36).slice(2, 9)}`;
  room.add(ws);
  socketInfo.set(ws, { roomId, playerId, lastState: null });

  ws.send(JSON.stringify({ type: 'welcome', roomId, playerId }));

  // Tell others in room that this player joined
  broadcastToRoom(roomId, { type: 'join', playerId }, ws);

  // Send current occupants to the new player
  room.forEach((other) => {
    const info = socketInfo.get(other);
    if (other !== ws && info && info.lastState) {
      ws.send(JSON.stringify({ type: 'state', playerId: info.playerId, ...info.lastState }));
    }
  });

  ws.on('message', (data) => {
    try {
      const msg = JSON.parse(data.toString());
      if (msg.type === 'state') {
        const info = socketInfo.get(ws);
        if (info) {
          info.lastState = { x: msg.x, y: msg.y, z: msg.z, yaw: msg.yaw, pitch: msg.pitch, jumping: msg.jumping };
          broadcastToRoom(info.roomId, { type: 'state', playerId: info.playerId, ...info.lastState }, ws);
        }
      }
    } catch (_) {}
  });

  ws.on('close', () => {
    const info = socketInfo.get(ws);
    if (info) {
      broadcastToRoom(info.roomId, { type: 'leave', playerId: info.playerId });
      rooms.get(info.roomId)?.delete(ws);
      if (rooms.get(info.roomId)?.size === 0) rooms.delete(info.roomId);
      socketInfo.delete(ws);
    }
  });
});

server.listen(PORT, () => {
  console.log(`CPPd1 Multiplayer server: http://localhost:${PORT}`);
});
