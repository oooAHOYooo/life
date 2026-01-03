<script setup lang="ts">
import Konva from "konva";
import { computed, onBeforeUnmount, onMounted, ref, watch } from "vue";
import type { AnyNode } from "@/types/scene";
import { useEditorStore } from "@/stores/editor";
import { useRuntimeStore } from "@/stores/runtime";
import { buildAnnotationLayer } from "@/utils/export";
import { bboxFromPoints, clamp } from "@/utils/geometry";

const editor = useEditorStore();
const runtime = useRuntimeStore();

const host = ref<HTMLDivElement | null>(null);
const stageW = ref(900);
const stageH = ref(600);

const stageRef = ref<any>(null);
const trRef = ref<any>(null);

const isSpaceDown = ref(false);
const isPanning = ref(false);
const panStart = ref({ x: 0, y: 0 });
const panStartPan = ref({ x: 0, y: 0 });

const selBox = ref<{ visible: boolean; x: number; y: number; w: number; h: number }>({
  visible: false,
  x: 0,
  y: 0,
  w: 0,
  h: 0,
});
const selStart = ref<{ x: number; y: number } | null>(null);

const stageConfig = computed(() => ({
  width: stageW.value,
  height: stageH.value,
  x: editor.pan.x,
  y: editor.pan.y,
  scaleX: editor.zoom,
  scaleY: editor.zoom,
}));

function getStage(): Konva.Stage | null {
  const s = stageRef.value?.getNode?.() as Konva.Stage | undefined;
  return s ?? null;
}

function pointerToWorld(stage: Konva.Stage) {
  const p = stage.getPointerPosition();
  if (!p) return { x: 0, y: 0 };
  return {
    x: (p.x - stage.x()) / stage.scaleX(),
    y: (p.y - stage.y()) / stage.scaleY(),
  };
}

function setTransformer() {
  const stage = getStage();
  const tr = trRef.value?.getNode?.() as Konva.Transformer | undefined;
  if (!stage || !tr) return;
  const nodes = editor.selectedIds
    .map((id) => stage.findOne(`#${CSS.escape(id)}`))
    .filter(Boolean) as Konva.Node[];
  tr.nodes(nodes);
  tr.getLayer()?.batchDraw();
}

function labelsForScene() {
  return editor.scene.nodes
    .filter((n) => n.visible)
    .map((n) => {
      const bb = editor.getNodeBBox(n.id);
      if (!bb) return null;
      return { id: n.id, name: n.name, type: n.type, x: bb.x, y: bb.y, width: bb.width, height: bb.height };
    })
    .filter(Boolean) as Array<{ id: string; name: string; type: string; x: number; y: number; width: number; height: number }>;
}

function syncAnnotLayer() {
  const stage = getStage();
  if (!stage) return;
  if (editor.aiOverlay) buildAnnotationLayer(stage, labelsForScene());
  else {
    const layer = stage.findOne(".annot") as Konva.Layer | null;
    layer?.destroy();
    stage.draw();
  }
}

watch(
  () => editor.selectedIds.join("|"),
  () => setTransformer()
);
watch(
  () => editor.aiOverlay,
  () => syncAnnotLayer()
);
watch(
  () => editor.scene.nodes.map((n) => `${n.id}:${n.x}:${n.y}:${(n as any).width ?? ""}:${(n as any).height ?? ""}:${n.visible}`).join("|"),
  () => {
    if (editor.aiOverlay) syncAnnotLayer();
  }
);

function onKeyDown(e: KeyboardEvent) {
  if (e.key === " ") {
    e.preventDefault();
    isSpaceDown.value = true;
  }
}
function onKeyUp(e: KeyboardEvent) {
  if (e.key === " ") {
    isSpaceDown.value = false;
    isPanning.value = false;
  }
}

function onWheel(e: any) {
  const stage = getStage();
  if (!stage) return;
  const ev = e.evt as WheelEvent;
  const isZoom = ev.ctrlKey || ev.metaKey;
  if (!isZoom) return;
  ev.preventDefault();

  const oldScale = editor.zoom;
  const pointer = stage.getPointerPosition();
  if (!pointer) return;

  const mousePointTo = {
    x: (pointer.x - stage.x()) / oldScale,
    y: (pointer.y - stage.y()) / oldScale,
  };

  const dir = ev.deltaY > 0 ? -1 : 1;
  const factor = 1.06;
  const newScale = clamp(dir > 0 ? oldScale * factor : oldScale / factor, 0.1, 6);

  editor.zoom = newScale;
  editor.pan = {
    x: pointer.x - mousePointTo.x * newScale,
    y: pointer.y - mousePointTo.y * newScale,
  };
}

function onMouseDown(e: any) {
  const stage = getStage();
  if (!stage) return;
  const ev = e.evt as MouseEvent;

  // panning
  if (isSpaceDown.value) {
    isPanning.value = true;
    panStart.value = { x: ev.clientX, y: ev.clientY };
    panStartPan.value = { ...editor.pan };
    return;
  }

  const target = e.target as Konva.Node;
  const clickedOnEmpty = target === stage;
  const w = pointerToWorld(stage);

  // tool actions
  if (editor.tool === "rect") {
    editor.addRect("rect", w.x, w.y);
    editor.tool = "select";
    return;
  }
  if (editor.tool === "frame") {
    editor.addRect("frame", w.x, w.y);
    editor.tool = "select";
    return;
  }
  if (editor.tool === "text") {
    editor.addText(w.x, w.y);
    editor.tool = "select";
    return;
  }
  if (editor.tool === "brush") {
    editor.beginStroke(w.x, w.y);
    return;
  }

  // selection
  if (clickedOnEmpty) {
    if (ev.shiftKey) {
      // start marquee
      selStart.value = { x: w.x, y: w.y };
      selBox.value = { visible: true, x: w.x, y: w.y, w: 0, h: 0 };
    } else {
      editor.setSelection([]);
    }
    return;
  }

  const id = target.id();
  if (!id) return;
  if (ev.shiftKey) editor.toggleSelection(id);
  else editor.setSelection([id]);
}

function onMouseMove(e: any) {
  const stage = getStage();
  if (!stage) return;
  const ev = e.evt as MouseEvent;

  if (isPanning.value) {
    const dx = ev.clientX - panStart.value.x;
    const dy = ev.clientY - panStart.value.y;
    editor.pan = { x: panStartPan.value.x + dx, y: panStartPan.value.y + dy };
    return;
  }

  if (editor.isDrawing) {
    const w = pointerToWorld(stage);
    editor.appendStroke(w.x, w.y);
    return;
  }

  if (selBox.value.visible && selStart.value) {
    const w = pointerToWorld(stage);
    const x1 = selStart.value.x;
    const y1 = selStart.value.y;
    const x2 = w.x;
    const y2 = w.y;
    selBox.value = {
      visible: true,
      x: Math.min(x1, x2),
      y: Math.min(y1, y2),
      w: Math.abs(x2 - x1),
      h: Math.abs(y2 - y1),
    };
  }
}

function onMouseUp() {
  const stage = getStage();
  if (!stage) return;
  isPanning.value = false;

  if (editor.isDrawing) {
    editor.endStroke();
    return;
  }

  if (selBox.value.visible) {
    const box = selBox.value;
    const hits: string[] = [];
    for (const n of editor.scene.nodes) {
      if (!n.visible) continue;
      if (n.type === "group") continue;
      const bb =
        n.type === "stroke"
          ? bboxFromPoints(n.points)
          : { x: n.x, y: n.y, width: (n as any).width ?? 0, height: (n as any).height ?? 0 };
      const inside =
        bb.x >= box.x &&
        bb.y >= box.y &&
        bb.x + bb.width <= box.x + box.w &&
        bb.y + bb.height <= box.y + box.h;
      if (inside) hits.push(n.id);
    }
    editor.setSelection(hits);
    selBox.value = { visible: false, x: 0, y: 0, w: 0, h: 0 };
    selStart.value = null;
  }
}

// ---- node event handlers ----
function canInteract(n: AnyNode) {
  return n.visible && !n.locked && editor.tool === "select";
}

function onDragStartNode() {
  editor.pushHistory();
}

function onDragEndNode(e: any, n: AnyNode) {
  const node = e.target as Konva.Node;
  const pos = node.position();
  if (n.type === "stroke") {
    // move by delta (since points are absolute-ish)
    const dx = pos.x;
    const dy = pos.y;
    editor.updateNode(n.id, {
      points: n.points.map((v, i) => (i % 2 === 0 ? v + dx : v + dy)),
    } as any);
    node.position({ x: 0, y: 0 });
    return;
  }
  editor.updateNode(n.id, { x: pos.x, y: pos.y } as any);
}

function onTransformStart() {
  editor.pushHistory();
}

function onTransformEnd(e: any, n: AnyNode) {
  const node = e.target as Konva.Node;
  const scaleX = node.scaleX();
  const scaleY = node.scaleY();
  const rotation = node.rotation();

  if (n.type === "rect" || n.type === "frame" || n.type === "image" || n.type === "text") {
    const w = (n as any).width ?? 0;
    const h = (n as any).height ?? 0;
    const nextW = Math.max(2, Math.round(w * scaleX));
    const nextH = Math.max(2, Math.round(h * scaleY));
    node.scaleX(1);
    node.scaleY(1);
    editor.updateNode(n.id, {
      x: node.x(),
      y: node.y(),
      rotation,
      width: nextW,
      height: nextH,
    } as any);
  } else {
    editor.updateNode(n.id, { x: node.x(), y: node.y(), rotation } as any);
  }
}

// ---- images ----
const imgCache = new Map<string, HTMLImageElement>();
function getImage(src: string) {
  if (imgCache.has(src)) return imgCache.get(src)!;
  const img = new Image();
  img.src = src;
  imgCache.set(src, img);
  return img;
}

// ---- mount sizing + stage bus ----
let ro: ResizeObserver | null = null;
onMounted(() => {
  window.addEventListener("keydown", onKeyDown);
  window.addEventListener("keyup", onKeyUp);
  ro = new ResizeObserver(() => {
    if (!host.value) return;
    const r = host.value.getBoundingClientRect();
    stageW.value = Math.max(300, Math.floor(r.width));
    stageH.value = Math.max(300, Math.floor(r.height));
  });
  if (host.value) ro.observe(host.value);

  const stage = getStage();
  if (stage) runtime.setStage(stage);
});

onBeforeUnmount(() => {
  window.removeEventListener("keydown", onKeyDown);
  window.removeEventListener("keyup", onKeyUp);
  ro?.disconnect();
  ro = null;
});

watch(
  () => stageRef.value,
  () => {
    const stage = getStage();
    if (stage) runtime.setStage(stage);
    setTransformer();
    syncAnnotLayer();
  }
);
</script>

<template>
  <div ref="host" class="h-full w-full canvas-grid">
    <v-stage ref="stageRef" :config="stageConfig" @wheel="onWheel" @mousedown="onMouseDown" @mousemove="onMouseMove" @mouseup="onMouseUp">
      <v-layer :config="{ name: 'content' }">
        <!-- Canvas background (in world coords) -->
        <v-rect
          :config="{
            x: 0,
            y: 0,
            width: editor.scene.canvas.width,
            height: editor.scene.canvas.height,
            fill: editor.scene.canvas.background,
            stroke: '#27272a',
            strokeWidth: 2,
            listening: false
          }"
        />

        <template v-for="n in editor.scene.nodes" :key="n.id">
          <v-rect
            v-if="n.type === 'rect' || n.type === 'frame'"
            :config="{
              id: n.id,
              x: n.x,
              y: n.y,
              width: (n as any).width,
              height: (n as any).height,
              fill: (n as any).fill,
              stroke: (n as any).stroke,
              strokeWidth: (n as any).strokeWidth,
              cornerRadius: (n as any).radius ?? 0,
              rotation: n.rotation,
              opacity: n.opacity,
              visible: n.visible,
              draggable: canInteract(n)
            }"
            @dragstart="onDragStartNode"
            @dragend="(e: any) => onDragEndNode(e, n)"
            @transformstart="onTransformStart"
            @transformend="(e: any) => onTransformEnd(e, n)"
          />

          <v-text
            v-else-if="n.type === 'text'"
            :config="{
              id: n.id,
              x: n.x,
              y: n.y,
              text: (n as any).text,
              fontSize: (n as any).fontSize,
              fontFamily: (n as any).fontFamily,
              fill: (n as any).fill,
              width: (n as any).width,
              align: (n as any).align,
              rotation: n.rotation,
              opacity: n.opacity,
              visible: n.visible,
              draggable: canInteract(n)
            }"
            @dragstart="onDragStartNode"
            @dragend="(e: any) => onDragEndNode(e, n)"
            @transformstart="onTransformStart"
            @transformend="(e: any) => onTransformEnd(e, n)"
          />

          <v-line
            v-else-if="n.type === 'stroke'"
            :config="{
              id: n.id,
              points: (n as any).points,
              stroke: (n as any).stroke,
              strokeWidth: (n as any).strokeWidth,
              opacity: n.opacity,
              lineCap: (n as any).lineCap,
              lineJoin: (n as any).lineJoin,
              tension: (n as any).tension,
              visible: n.visible,
              draggable: canInteract(n)
            }"
            @dragstart="onDragStartNode"
            @dragend="(e: any) => onDragEndNode(e, n)"
          />

          <v-image
            v-else-if="n.type === 'image'"
            :config="{
              id: n.id,
              x: n.x,
              y: n.y,
              image: getImage((n as any).src),
              width: (n as any).width,
              height: (n as any).height,
              rotation: n.rotation,
              opacity: n.opacity,
              visible: n.visible,
              draggable: canInteract(n)
            }"
            @dragstart="onDragStartNode"
            @dragend="(e: any) => onDragEndNode(e, n)"
            @transformstart="onTransformStart"
            @transformend="(e: any) => onTransformEnd(e, n)"
          />
        </template>
      </v-layer>

      <v-layer :config="{ name: 'ui' }">
        <v-rect
          v-if="selBox.visible"
          :config="{
            x: selBox.x,
            y: selBox.y,
            width: selBox.w,
            height: selBox.h,
            stroke: '#6366f1',
            strokeWidth: 2,
            dash: [6, 4],
            fill: 'rgba(99,102,241,0.10)',
            listening: false
          }"
        />
        <v-transformer
          ref="trRef"
          :config="{
            rotateEnabled: true,
            ignoreStroke: true,
            boundBoxFunc: (_oldBox: any, newBox: any) => {
              if (newBox.width < 8 || newBox.height < 8) return _oldBox;
              return newBox;
            }
          }"
        />
      </v-layer>
    </v-stage>
  </div>
</template>

