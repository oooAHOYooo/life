/**
 * Ninja Game - JavaScript port from Unreal Engine C++
 * Features: double jump, flips (backflip, frontflip, sideflip, 360), roll, cube ninja body
 */

import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js';

// —— Constants (aligned with C++ values) ——
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

// Flip constants
const FLIP_DURATION = 0.5;
const FLIP_UP_IMPULSE = 3.5;
const FLIP_HORIZONTAL_IMPULSE = 2.0;
const ROLL_DURATION = 0.4;
const ROLL_SPEED = 8.0;
const JUMP_COUNT_MAX = 2;

// —— Scene Setup ——
const canvas = document.getElementById('canvas');
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x1a1f3a);
scene.fog = new THREE.Fog(0x1a1f3a, 20, 80);

const renderer = new THREE.WebGLRenderer({ canvas, antialias: true });
renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;

const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(0, CAMERA_ARM_LENGTH, CAMERA_ARM_LENGTH);

// Lighting
const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
scene.add(ambientLight);
const dirLight = new THREE.DirectionalLight(0xffffff, 0.8);
dirLight.position.set(10, 20, 10);
dirLight.castShadow = true;
dirLight.shadow.camera.left = -20;
dirLight.shadow.camera.right = 20;
dirLight.shadow.camera.top = 20;
dirLight.shadow.camera.bottom = -20;
scene.add(dirLight);

// Ground
const groundGeometry = new THREE.PlaneGeometry(100, 100);
const groundMaterial = new THREE.MeshStandardMaterial({ color: 0x2a2a3a });
const ground = new THREE.Mesh(groundGeometry, groundMaterial);
ground.rotation.x = -Math.PI / 2;
ground.position.y = 0;
ground.receiveShadow = true;
scene.add(ground);

// —— Input State ——
const keys = {};
const mouse = { x: 0, y: 0, deltaX: 0, deltaY: 0 };
let pointerLocked = false;

document.addEventListener('keydown', (e) => { keys[e.code] = true; });
document.addEventListener('keyup', (e) => { keys[e.code] = false; });

canvas.addEventListener('click', () => {
  canvas.requestPointerLock();
});

document.addEventListener('pointerlockchange', () => {
  pointerLocked = document.pointerLockElement === canvas;
});

document.addEventListener('mousemove', (e) => {
  if (pointerLocked) {
    mouse.deltaX = e.movementX * 0.002;
    mouse.deltaY = e.movementY * 0.002;
  }
});

// —— Cube Ninja Body Component ——
class CubeNinjaBody {
  constructor(parent) {
    this.parent = parent;
    this.bodyGroup = new THREE.Group();
    this.bodyScale = 0.01; // Convert from cm to meters
    this.limbSwingAmount = 25 * Math.PI / 180; // degrees to radians
    this.limbSwingSpeed = 2.0;
    this.walkCycleTime = 0;
    
    this.parts = [];
    this.partMeshes = [];
    this.partPivots = [];
    this.swingPartIndices = [5, 10, 14, 18]; // L_UA, R_UA, L_UL, R_UL
    
    this.buildBody();
    parent.add(this.bodyGroup);
  }

  buildBody() {
    // Part definitions (from C++ code)
    const parts = [
      { name: 'Pelvis', isSphere: true, radius: 14, location: [0, 0, 14] },
      { name: 'Spine', isSphere: false, halfExtents: [6, 5, 8], location: [0, 0, 8] },
      { name: 'Chest', isSphere: false, halfExtents: [8, 6, 7], location: [0, 0, 7] },
      { name: 'Head', isSphere: true, radius: 11, location: [0, 0, 11] },
      { name: 'L_Shoulder', isSphere: true, radius: 6, location: [-14, 1, 0] },
      { name: 'L_UpperArm', isSphere: false, halfExtents: [2.5, 2.5, 10], location: [0, 0, -10], defaultRot: [0, 0, 12] },
      { name: 'L_Elbow', isSphere: true, radius: 4, location: [0, 0, -4] },
      { name: 'L_LowerArm', isSphere: false, halfExtents: [2, 2, 9], location: [0, 0, -9] },
      { name: 'L_Hand', isSphere: true, radius: 3.5, location: [0, 0, 0] },
      { name: 'R_Shoulder', isSphere: true, radius: 6, location: [14, 1, 0] },
      { name: 'R_UpperArm', isSphere: false, halfExtents: [2.5, 2.5, 10], location: [0, 0, -10], defaultRot: [0, 0, -12] },
      { name: 'R_Elbow', isSphere: true, radius: 4, location: [0, 0, -4] },
      { name: 'R_LowerArm', isSphere: false, halfExtents: [2, 2, 9], location: [0, 0, -9] },
      { name: 'R_Hand', isSphere: true, radius: 3.5, location: [0, 0, 0] },
      { name: 'L_UpperLeg', isSphere: false, halfExtents: [3, 3, 11], location: [-9.8, 0, -11] },
      { name: 'L_Knee', isSphere: true, radius: 5, location: [0, 0, -5] },
      { name: 'L_LowerLeg', isSphere: false, halfExtents: [2.5, 2.5, 10], location: [0, 0, -10] },
      { name: 'L_Foot', isSphere: true, radius: 4, location: [0, 0, -4] },
      { name: 'R_UpperLeg', isSphere: false, halfExtents: [3, 3, 11], location: [9.8, 0, -11] },
      { name: 'R_Knee', isSphere: true, radius: 5, location: [0, 0, -5] },
      { name: 'R_LowerLeg', isSphere: false, halfExtents: [2.5, 2.5, 10], location: [0, 0, -10] },
      { name: 'R_Foot', isSphere: true, radius: 4, location: [0, 0, -4] },
    ];

    // Build hierarchy
    const hierarchy = [
      { part: 0, parent: null }, // Pelvis
      { part: 1, parent: 0 }, // Spine
      { part: 2, parent: 1 }, // Chest
      { part: 3, parent: 2 }, // Head
      { part: 4, parent: 2 }, // L_Shoulder
      { part: 5, parent: 4 }, // L_UpperArm
      { part: 6, parent: 5 }, // L_Elbow
      { part: 7, parent: 6 }, // L_LowerArm
      { part: 8, parent: 7 }, // L_Hand
      { part: 9, parent: 2 }, // R_Shoulder
      { part: 10, parent: 9 }, // R_UpperArm
      { part: 11, parent: 10 }, // R_Elbow
      { part: 12, parent: 11 }, // R_LowerArm
      { part: 13, parent: 12 }, // R_Hand
      { part: 14, parent: 0 }, // L_UpperLeg
      { part: 15, parent: 14 }, // L_Knee
      { part: 16, parent: 15 }, // L_LowerLeg
      { part: 17, parent: 16 }, // L_Foot
      { part: 18, parent: 0 }, // R_UpperLeg
      { part: 19, parent: 18 }, // R_Knee
      { part: 20, parent: 19 }, // R_LowerLeg
      { part: 21, parent: 20 }, // R_Foot
    ];

    const material = new THREE.MeshStandardMaterial({ 
      color: 0x4a5568, 
      metalness: 0.3, 
      roughness: 0.7 
    });

    this.partPivots = [];
    this.partMeshes = [];

    hierarchy.forEach(({ part, parent }) => {
      const partDef = parts[part];
      const pivot = new THREE.Group();
      pivot.position.set(
        partDef.location[0] * this.bodyScale,
        partDef.location[1] * this.bodyScale,
        partDef.location[2] * this.bodyScale
      );
      
      if (partDef.defaultRot) {
        pivot.rotation.set(
          partDef.defaultRot[0] * Math.PI / 180,
          partDef.defaultRot[1] * Math.PI / 180,
          partDef.defaultRot[2] * Math.PI / 180
        );
      }

      let mesh;
      if (partDef.isSphere) {
        const geometry = new THREE.SphereGeometry(partDef.radius * this.bodyScale, 12, 12);
        mesh = new THREE.Mesh(geometry, material);
      } else {
        const geometry = new THREE.BoxGeometry(
          partDef.halfExtents[0] * 2 * this.bodyScale,
          partDef.halfExtents[1] * 2 * this.bodyScale,
          partDef.halfExtents[2] * 2 * this.bodyScale
        );
        mesh = new THREE.Mesh(geometry, material);
      }
      
      mesh.castShadow = true;
      mesh.receiveShadow = true;
      pivot.add(mesh);

      if (parent === null) {
        this.bodyGroup.add(pivot);
      } else {
        this.partPivots[parent].add(pivot);
      }

      this.partPivots.push(pivot);
      this.partMeshes.push(mesh);
    });
  }

  updateLimbSwing(deltaTime, speed) {
    const isMoving = speed > 0.1;
    if (isMoving) {
      this.walkCycleTime += deltaTime * this.limbSwingSpeed * 2 * Math.PI;
    } else {
      this.walkCycleTime = THREE.MathUtils.lerp(this.walkCycleTime, 0, deltaTime * 5);
    }

    const swing = Math.sin(this.walkCycleTime) * this.limbSwingAmount;
    const swingLeg = Math.sin(this.walkCycleTime + Math.PI) * this.limbSwingAmount;

    this.swingPartIndices.forEach((idx) => {
      if (this.partPivots[idx]) {
        const baseRot = idx === 5 || idx === 10 ? 
          (idx === 5 ? [0, 0, 12] : [0, 0, -12]) : [0, 0, 0];
        const baseRotRad = baseRot.map(r => r * Math.PI / 180);
        
        if (idx === 5 || idx === 10) {
          // Arms
          this.partPivots[idx].rotation.x = baseRotRad[0] + (idx === 5 ? swing : -swing);
        } else {
          // Legs
          this.partPivots[idx].rotation.x = baseRotRad[0] + (idx === 14 ? swingLeg : -swingLeg);
        }
      }
    });
  }
}

// —— Ninja Character ——
class NinjaCharacter {
  constructor() {
    this.position = new THREE.Vector3(0, 2, 0);
    this.velocity = new THREE.Vector3();
    this.yaw = 0;
    this.pitch = 0;
    this.isGrounded = false;
    this.jumpCount = 0;
    this.isRolling = false;
    this.rollTimeRemaining = 0;
    this.rollDirection = new THREE.Vector3();
    this.isFlipping = false;
    this.flipType = 'None';
    this.flipElapsed = 0;
    this.flipTotalDegrees = 0;
    this.meshFlipBaseRotation = new THREE.Euler();

    // Create mesh group
    this.mesh = new THREE.Group();
    this.mesh.position.copy(this.position);
    
    // Create cube ninja body
    this.cubeBody = new CubeNinjaBody(this.mesh);
    
    scene.add(this.mesh);
  }

  update(deltaTime) {
    // Camera rotation
    if (pointerLocked) {
      this.pitch -= mouse.deltaY;
      this.pitch = Math.max(-CAMERA_PITCH_LIMIT, Math.min(CAMERA_PITCH_LIMIT, this.pitch));
      this.yaw -= mouse.deltaX;
      mouse.deltaX = 0;
      mouse.deltaY = 0;
    }

    // Movement input
    const moveInput = new THREE.Vector3();
    if (keys['KeyW']) moveInput.z -= 1;
    if (keys['KeyS']) moveInput.z += 1;
    if (keys['KeyA']) moveInput.x -= 1;
    if (keys['KeyD']) moveInput.x += 1;
    moveInput.normalize();

    // Camera space movement
    const cameraForward = new THREE.Vector3(0, 0, -1);
    const cameraRight = new THREE.Vector3(1, 0, 0);
    cameraForward.applyAxisAngle(new THREE.Vector3(0, 1, 0), this.yaw);
    cameraRight.applyAxisAngle(new THREE.Vector3(0, 1, 0), this.yaw);
    
    const moveDir = new THREE.Vector3();
    moveDir.addScaledVector(cameraForward, moveInput.z);
    moveDir.addScaledVector(cameraRight, moveInput.x);
    moveDir.y = 0;
    moveDir.normalize();

    // Ground check
    this.isGrounded = this.position.y <= 0.1;

    // Jump
    if (keys['Space'] && !keys['_spacePressed']) {
      keys['_spacePressed'] = true;
      if (this.isGrounded || this.jumpCount < JUMP_COUNT_MAX - 1) {
        this.velocity.y = JUMP_VELOCITY;
        this.jumpCount++;
        this.isGrounded = false;
      }
    }
    if (!keys['Space']) {
      keys['_spacePressed'] = false;
    }

    // Roll
    if (keys['KeyQ'] && !keys['_qPressed'] && !this.isRolling && !this.isFlipping) {
      keys['_qPressed'] = true;
      this.doRoll(moveDir);
    }
    if (!keys['KeyQ']) {
      keys['_qPressed'] = false;
    }

    // Flips
    if (keys['Digit1'] && !keys['_1Pressed'] && !this.isFlipping && !this.isRolling) {
      keys['_1Pressed'] = true;
      this.doBackflip();
    }
    if (!keys['Digit1']) keys['_1Pressed'] = false;

    if (keys['Digit2'] && !keys['_2Pressed'] && !this.isFlipping && !this.isRolling) {
      keys['_2Pressed'] = true;
      this.doFrontflip();
    }
    if (!keys['Digit2']) keys['_2Pressed'] = false;

    if (keys['Digit3'] && !keys['_3Pressed'] && !this.isFlipping && !this.isRolling) {
      keys['_3Pressed'] = true;
      this.doSideflipLeft();
    }
    if (!keys['Digit3']) keys['_3Pressed'] = false;

    if (keys['Digit4'] && !keys['_4Pressed'] && !this.isFlipping && !this.isRolling) {
      keys['_4Pressed'] = true;
      this.doSideflipRight();
    }
    if (!keys['Digit4']) keys['_4Pressed'] = false;

    if (keys['Digit5'] && !keys['_5Pressed'] && !this.isFlipping && !this.isRolling) {
      keys['_5Pressed'] = true;
      this.doFlip360();
    }
    if (!keys['Digit5']) keys['_5Pressed'] = false;

    // Update roll
    if (this.isRolling) {
      this.rollTimeRemaining -= deltaTime;
      this.velocity.x = this.rollDirection.x * ROLL_SPEED;
      this.velocity.z = this.rollDirection.z * ROLL_SPEED;
      if (this.rollTimeRemaining <= 0) {
        this.isRolling = false;
      }
    } else if (!this.isFlipping) {
      // Normal movement
      const control = this.isGrounded ? 1.0 : AIR_CONTROL;
      const targetVel = moveDir.clone().multiplyScalar(MAX_WALK_SPEED);
      const braking = this.isGrounded ? BRAKING_WALKING : BRAKING_FALLING;
      
      this.velocity.x = THREE.MathUtils.lerp(this.velocity.x, targetVel.x * control, deltaTime * braking);
      this.velocity.z = THREE.MathUtils.lerp(this.velocity.z, targetVel.z * control, deltaTime * braking);
    }

    // Update flip
    if (this.isFlipping) {
      this.updateFlip(deltaTime);
    }

    // Gravity
    if (!this.isGrounded) {
      this.velocity.y += GRAVITY * deltaTime;
    } else {
      if (this.velocity.y < 0) {
        this.velocity.y = 0;
        this.position.y = 0;
        this.jumpCount = 0;
      }
    }

    // Update position
    this.position.addScaledVector(this.velocity, deltaTime);

    // Rotate character to face movement direction
    if (moveDir.length() > 0.1 && !this.isRolling && !this.isFlipping) {
      const targetYaw = Math.atan2(moveDir.x, moveDir.z);
      this.mesh.rotation.y = THREE.MathUtils.lerp(this.mesh.rotation.y, targetYaw, deltaTime * ROTATION_RATE);
    }

    this.mesh.position.copy(this.position);

    // Update cube body limb swing
    const speed = new THREE.Vector3(this.velocity.x, 0, this.velocity.z).length();
    this.cubeBody.updateLimbSwing(deltaTime, speed);

    // Update camera
    const cameraOffset = new THREE.Vector3(0, CAMERA_ARM_LENGTH * 0.5, CAMERA_ARM_LENGTH);
    cameraOffset.applyAxisAngle(new THREE.Vector3(0, 1, 0), this.yaw);
    cameraOffset.applyAxisAngle(new THREE.Vector3(1, 0, 0), this.pitch);
    camera.position.copy(this.position).add(cameraOffset);
    camera.lookAt(this.position);
  }

  doRoll(moveDir) {
    if (moveDir.length() < 0.1) {
      moveDir.set(-Math.sin(this.mesh.rotation.y), 0, -Math.cos(this.mesh.rotation.y));
    }
    this.rollDirection.copy(moveDir);
    this.rollTimeRemaining = ROLL_DURATION;
    this.isRolling = true;
  }

  doBackflip() {
    this.startFlip('Backflip', new THREE.Vector3(
      -Math.sin(this.mesh.rotation.y),
      0,
      -Math.cos(this.mesh.rotation.y)
    ));
  }

  doFrontflip() {
    this.startFlip('Frontflip', new THREE.Vector3(
      Math.sin(this.mesh.rotation.y),
      0,
      Math.cos(this.mesh.rotation.y)
    ));
  }

  doSideflipLeft() {
    this.startFlip('SideflipLeft', new THREE.Vector3(
      -Math.cos(this.mesh.rotation.y),
      0,
      Math.sin(this.mesh.rotation.y)
    ));
  }

  doSideflipRight() {
    this.startFlip('SideflipRight', new THREE.Vector3(
      Math.cos(this.mesh.rotation.y),
      0,
      -Math.sin(this.mesh.rotation.y)
    ));
  }

  doFlip360() {
    this.startFlip('Flip360', new THREE.Vector3(0, 0, 0));
  }

  startFlip(flipType, horizontalDir) {
    this.isFlipping = true;
    this.flipType = flipType;
    this.flipElapsed = 0;
    this.meshFlipBaseRotation.copy(this.cubeBody.bodyGroup.rotation);

    switch (flipType) {
      case 'Backflip':
      case 'Frontflip':
      case 'Flip360':
        this.flipTotalDegrees = flipType === 'Flip360' ? 360 : 180;
        break;
      case 'SideflipLeft':
      case 'SideflipRight':
        this.flipTotalDegrees = 180;
        break;
    }

    const impulse = new THREE.Vector3(0, FLIP_UP_IMPULSE, 0);
    if (horizontalDir.length() > 0.1) {
      impulse.addScaledVector(horizontalDir.normalize(), FLIP_HORIZONTAL_IMPULSE);
    }
    this.velocity.add(impulse);
  }

  updateFlip(deltaTime) {
    this.flipElapsed += deltaTime;
    const t = Math.min(this.flipElapsed / FLIP_DURATION, 1);
    const angleDeg = this.flipTotalDegrees * t;

    let pitchDelta = 0;
    let rollDelta = 0;

    switch (this.flipType) {
      case 'Backflip':
        pitchDelta = angleDeg * Math.PI / 180;
        break;
      case 'Frontflip':
        pitchDelta = -angleDeg * Math.PI / 180;
        break;
      case 'SideflipLeft':
        rollDelta = angleDeg * Math.PI / 180;
        break;
      case 'SideflipRight':
        rollDelta = -angleDeg * Math.PI / 180;
        break;
      case 'Flip360':
        pitchDelta = angleDeg * Math.PI / 180;
        break;
    }

    this.cubeBody.bodyGroup.rotation.x = this.meshFlipBaseRotation.x + pitchDelta;
    this.cubeBody.bodyGroup.rotation.z = this.meshFlipBaseRotation.z + rollDelta;

    if (this.flipElapsed >= FLIP_DURATION) {
      this.endFlip();
    }
  }

  endFlip() {
    this.isFlipping = false;
    this.flipType = 'None';
    this.cubeBody.bodyGroup.rotation.copy(this.meshFlipBaseRotation);
  }
}

// —— Game Loop ——
const ninja = new NinjaCharacter();

function resize() {
  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();
  renderer.setSize(window.innerWidth, window.innerHeight);
}
window.addEventListener('resize', resize);
resize();

let lastTime = performance.now();
function animate() {
  requestAnimationFrame(animate);
  const now = performance.now();
  const deltaTime = Math.min((now - lastTime) / 1000, 0.1);
  lastTime = now;

  ninja.update(deltaTime);
  renderer.render(scene, camera);
}
animate();
