/**
 * CPPd1-style third-person character in JS (ported from C++).
 * Move in camera space, mouse look, jump. Multiplayer: shared world via WebSocket.
 */

import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js';

// —— Tuning (aligned with CPPd1Character C++) ——
const CAPSULE_RADIUS = 0.42;
const CAPSULE_HEIGHT = 0.96;
const JUMP_VELOCITY = 8.5;
const MAX_WALK_SPEED = 8;
const ROTATION_RATE = 8;
const AIR_CONTROL = 0.35;
const BRAKING_WALKING = 80;
const BRAKING_FALLING = 50;
const GRAVITY = -28;
const CAMERA_ARM_LENGTH = 6;
const CAMERA_PITCH_LIMIT = 0.45 * Math.PI;

// —— Scene ——
const canvas = document.getElementById('canvas');
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x87ceeb);
scene.fog = new THREE.Fog(0x87ceeb, 20, 80);

const renderer = new THREE.WebGLRenderer({ canvas, antialias: true });
renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;

function resize() {
  const w = window.innerWidth;
  const h = window.innerHeight;
  renderer.setSize(w, h);
  if (camera) camera.aspect = w / h;
  if (camera) camera.updateProjectionMatrix();
}
window.addEventListener('resize', resize);

// Lighting
const ambient = new THREE.AmbientLight(0x404060, 0.6);
scene.add(ambient);
const sun = new THREE.DirectionalLight(0xfff5e6, 0.9);
sun.position.set(12, 20, 10);
sun.castShadow = true;
sun.shadow.mapSize.set(1024, 1024);
sun.shadow.camera.near = 0.5;
sun.shadow.camera.far = 60;
sun.shadow.camera.left = sun.shadow.camera.bottom = -15;
sun.shadow.camera.right = sun.shadow.camera.top = 15;
scene.add(sun);

// Ground
const groundGeo = new THREE.PlaneGeometry(120, 120);
const groundMat = new THREE.MeshStandardMaterial({ color: 0x2d5a27, roughness: 0.9, metalness: 0.1 });
const ground = new THREE.Mesh(groundGeo, groundMat);
ground.rotation.x = -Math.PI / 2;
ground.receiveShadow = true;
scene.add(ground);

// Simple platforms (shared world props)
const boxGeo = new THREE.BoxGeometry(4, 1, 4);
const boxMat = new THREE.MeshStandardMaterial({ color: 0x6b5344, roughness: 0.8 });
[-8, 0, 8].forEach((z, i) => {
  const block = new THREE.Mesh(boxGeo, boxMat.clone());
  block.position.set(i * 6 - 6, 0.5, z);
  block.castShadow = true;
  block.receiveShadow = true;
  scene.add(block);
});

// —— Local player (capsule + camera) ——
const capsuleGeo = new THREE.CapsuleGeometry(CAPSULE_RADIUS, CAPSULE_HEIGHT - 2 * CAPSULE_RADIUS, 8, 12);
const playerMat = new THREE.MeshStandardMaterial({ color: 0x3b82f6 });
const playerMesh = new THREE.Mesh(capsuleGeo, playerMat);
playerMesh.castShadow = true;
const player = new THREE.Group();
player.add(playerMesh);
player.position.set(0, CAPSULE_HEIGHT / 2, 0);
scene.add(player);

let camera = new THREE.PerspectiveCamera(70, 1, 0.1, 200);
camera.position.set(0, 3, CAMERA_ARM_LENGTH);
camera.lookAt(0, 2, 0);
scene.add(camera);

// Controller state (like UE character + controller)
const state = {
  x: 0, y: CAPSULE_HEIGHT / 2, z: 0,
  vx: 0, vy: 0, vz: 0,
  yaw: 0,
  pitch: 0,
  onGround: true,
  moveInput: { forward: 0, right: 0 },
  jumpHeld: false,
};

function getForwardRight() {
  const cos = Math.cos(state.yaw);
  const sin = Math.sin(state.yaw);
  return {
    forward: new THREE.Vector3(-sin, 0, -cos),
    right: new THREE.Vector3(cos, 0, -sin),
  };
}

function tickCharacter(dt) {
  const { forward, right } = getForwardRight();
  const move = new THREE.Vector3(0, 0, 0);
  if (state.moveInput.forward) move.addScaledVector(forward, state.moveInput.forward);
  if (state.moveInput.right) move.addScaledVector(right, state.moveInput.right);
  move.clampLength(0, 1);

  const effectiveSpeed = state.onGround ? MAX_WALK_SPEED : MAX_WALK_SPEED * AIR_CONTROL;
  const wishVel = move.multiplyScalar(effectiveSpeed);
  const brake = state.onGround ? BRAKING_WALKING : BRAKING_FALLING;

  state.vx += (wishVel.x - state.vx) * Math.min(1, brake * dt);
  state.vz += (wishVel.z - state.vz) * Math.min(1, brake * dt);
  state.vy += GRAVITY * dt;

  if (state.onGround && state.jumpHeld) {
    state.vy = JUMP_VELOCITY;
    state.onGround = false;
  }

  state.x += state.vx * dt;
  state.y += state.vy * dt;
  state.z += state.vz * dt;

  const groundY = CAPSULE_HEIGHT / 2;
  if (state.y <= groundY) {
    state.y = groundY;
    state.vy = 0;
    state.onGround = true;
  } else {
    state.onGround = false;
  }

  // Orient character toward movement (like bOrientRotationToMovement)
  const flatVel = new THREE.Vector2(state.vx, state.vz);
  if (flatVel.lengthSq() > 0.01) {
    const targetYaw = Math.atan2(-state.vx, -state.vz);
    let dy = targetYaw - state.yaw;
    while (dy > Math.PI) dy -= 2 * Math.PI;
    while (dy < -Math.PI) dy += 2 * Math.PI;
    state.yaw += dy * Math.min(1, ROTATION_RATE * dt);
  }

  player.position.set(state.x, state.y, state.z);
  player.rotation.y = state.yaw;
}

function updateCamera(dt) {
  const cos = Math.cos(state.yaw);
  const sin = Math.sin(state.yaw);
  const cp = Math.cos(state.pitch);
  const sp = Math.sin(state.pitch);
  const arm = new THREE.Vector3(sin * cp, -sp, cos * cp).multiplyScalar(-CAMERA_ARM_LENGTH);
  const target = new THREE.Vector3(state.x, state.y + 1.2, state.z);
  camera.position.copy(target).add(arm);
  camera.lookAt(target);
}

// —— Input (DoMove, DoLook, DoJump like C++) ——
const keys = {};
document.addEventListener('keydown', (e) => {
  keys[e.code] = true;
  if (e.code === 'Space') {
    e.preventDefault();
    state.jumpHeld = true;
  }
});
document.addEventListener('keyup', (e) => {
  keys[e.code] = false;
  if (e.code === 'Space') state.jumpHeld = false;
});

canvas.addEventListener('click', () => canvas.requestPointerLock());
document.addEventListener('pointerlockchange', () => {
  if (document.pointerLockElement === canvas) state.pointerLock = true;
  else state.pointerLock = false;
});

document.addEventListener('mousemove', (e) => {
  if (document.pointerLockElement !== canvas) return;
  const sens = 0.002;
  state.yaw -= e.movementX * sens;
  state.pitch -= e.movementY * sens;
  state.pitch = Math.max(-CAMERA_PITCH_LIMIT, Math.min(CAMERA_PITCH_LIMIT, state.pitch));
});

function updateMoveInput() {
  let forward = 0, right = 0;
  if (keys['KeyW']) forward += 1;
  if (keys['KeyS']) forward -= 1;
  if (keys['KeyD']) right += 1;
  if (keys['KeyA']) right -= 1;
  state.moveInput = { forward, right };
}

// —— Multiplayer ——
const otherPlayers = new Map(); // playerId -> { mesh, state }

function createOtherPlayerMesh() {
  const geo = new THREE.CapsuleGeometry(CAPSULE_RADIUS, CAPSULE_HEIGHT - 2 * CAPSULE_RADIUS, 8, 12);
  const mat = new THREE.MeshStandardMaterial({ color: 0xf97316 });
  const m = new THREE.Mesh(geo, mat);
  m.castShadow = true;
  const g = new THREE.Group();
  g.add(m);
  scene.add(g);
  return g;
}

let ws = null;
let myPlayerId = null;
let roomId = null;
const WS_URL = `${location.protocol === 'https:' ? 'wss' : 'ws'}://${location.host}`;

function connect() {
  const params = new URLSearchParams(location.search);
  const room = params.get('room') || '';
  const url = room ? `${WS_URL}?room=${encodeURIComponent(room)}` : WS_URL;
  ws = new WebSocket(url);

  ws.onopen = () => {};

  ws.onmessage = (ev) => {
    try {
      const msg = JSON.parse(ev.data);
      if (msg.type === 'welcome') {
        myPlayerId = msg.playerId;
        roomId = msg.roomId;
        document.getElementById('room').textContent = `Room: ${roomId}`;
        const link = `${location.origin}${location.pathname}?room=${roomId}`;
        document.getElementById('shareLink').href = link;
        document.getElementById('copy').onclick = () => {
          navigator.clipboard.writeText(link);
          document.getElementById('copy').textContent = 'Copied!';
          setTimeout(() => { document.getElementById('copy').textContent = 'Copy link'; }, 1500);
        };
      } else if (msg.type === 'join') {
        const mesh = createOtherPlayerMesh();
        otherPlayers.set(msg.playerId, { mesh, state: { x:0, y:1, z:0, yaw:0, pitch:0 } });
      } else if (msg.type === 'leave') {
        const o = otherPlayers.get(msg.playerId);
        if (o) {
          scene.remove(o.mesh);
          otherPlayers.delete(msg.playerId);
        }
      } else if (msg.type === 'state') {
        if (msg.playerId === myPlayerId) return;
        let o = otherPlayers.get(msg.playerId);
        if (!o) {
          o = { mesh: createOtherPlayerMesh(), state: {} };
          otherPlayers.set(msg.playerId, o);
        }
        o.state = { x: msg.x, y: msg.y, z: msg.z, yaw: msg.yaw, pitch: msg.pitch };
      }
    } catch (_) {}
  };

  ws.onclose = () => {
    setTimeout(connect, 2000);
  };
}

function sendState() {
  if (ws && ws.readyState === 1 && myPlayerId) {
    ws.send(JSON.stringify({
      type: 'state',
      x: state.x, y: state.y, z: state.z,
      yaw: state.yaw, pitch: state.pitch,
      jumping: state.jumpHeld || !state.onGround,
    }));
  }
}

let lastSend = 0;
const SEND_INTERVAL = 1 / 20;

// —— Loop ——
let lastTime = performance.now() / 1000;
function loop() {
  requestAnimationFrame(loop);
  const now = performance.now() / 1000;
  const dt = Math.min(now - lastTime, 0.1);
  lastTime = now;

  updateMoveInput();
  tickCharacter(dt);
  updateCamera(dt);

  otherPlayers.forEach(({ mesh, state: s }) => {
    mesh.position.set(s.x, s.y, s.z);
    mesh.rotation.y = s.yaw;
  });

  if (now - lastSend >= SEND_INTERVAL) {
    sendState();
    lastSend = now;
  }

  renderer.render(scene, camera);
}
resize();
connect();
loop();
