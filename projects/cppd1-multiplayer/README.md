# CPPd1 Multiplayer

A JavaScript port of the CPPd1 third-person character (move, look, jump) that runs in the browser with **shared-world multiplayer**: two players can play in the same world by sharing a link (Minecraft-style).

## Run locally

```bash
cd projects/cppd1-multiplayer
npm install
npm start
```

Open **http://localhost:3742**. You get a random room code. Share the link (e.g. `http://localhost:3742?room=ABCDE`) with a friend; when they open it, you both see each other in the same 3D world.

## Controls

- **WASD** – move (relative to camera)
- **Mouse** – look (click canvas to lock pointer)
- **Space** – jump

## Tech

- **Client**: Three.js, pointer lock, WebSocket.
- **Server**: Node (Express + `ws`). Serves the game and syncs player position/rotation in rooms.

Movement and tuning are aligned with the C++ `CPPd1Character` (capsule size, jump velocity, walk speed, rotation rate, air control, braking).
