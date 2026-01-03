import { defineStore } from "pinia";
import type { AnyNode, BrushPreset, Scene } from "@/types/scene";
import { uid } from "@/utils/ids";
import { bboxFromPoints, clamp } from "@/utils/geometry";
import { DEFAULT_PRESETS } from "@/data/presets";

type Tool = "select" | "rect" | "text" | "frame" | "brush" | "image";

type HistoryEntry = {
  scene: Scene;
  selectedIds: string[];
};

function deepClone<T>(v: T): T {
  return structuredClone(v);
}

function normalizeNodeBase(n: AnyNode): AnyNode {
  return {
    ...n,
    rotation: n.rotation ?? 0,
    scaleX: n.scaleX ?? 1,
    scaleY: n.scaleY ?? 1,
    opacity: n.opacity ?? 1,
    visible: n.visible ?? true,
    locked: n.locked ?? false,
    parentId: n.parentId ?? null,
  } as AnyNode;
}

function defaultScene(): Scene {
  return {
    version: 1,
    canvas: { width: 1200, height: 800, background: "#0b0b0c" },
    nodes: [],
  };
}

export const useEditorStore = defineStore("editor", {
  state: () => ({
    projectId: "" as string,
    projectName: "Untitled Project" as string,
    scene: defaultScene() as Scene,
    selectedIds: [] as string[],
    tool: "select" as Tool,
    brushPreset: DEFAULT_PRESETS[0] as BrushPreset,
    aiOverlay: false,
    zoom: 1,
    pan: { x: 0, y: 0 },

    // transient drawing
    isDrawing: false,
    currentStrokeId: "" as string,

    // history
    undoStack: [] as HistoryEntry[],
    redoStack: [] as HistoryEntry[],
  }),
  getters: {
    nodesById: (s) => {
      const m = new Map<string, AnyNode>();
      for (const n of s.scene.nodes) m.set(n.id, n);
      return m;
    },
    selectedNodes(): AnyNode[] {
      const m = this.nodesById;
      return this.selectedIds.map((id) => m.get(id)).filter(Boolean) as AnyNode[];
    },
    canUndo: (s) => s.undoStack.length > 0,
    canRedo: (s) => s.redoStack.length > 0,
  },
  actions: {
    hydrate(projectId: string, projectName: string, scene: Scene) {
      this.projectId = projectId;
      this.projectName = projectName;
      this.scene = {
        ...scene,
        nodes: (scene.nodes ?? []).map(normalizeNodeBase),
      };
      this.selectedIds = [];
      this.undoStack = [];
      this.redoStack = [];
      this.zoom = 1;
      this.pan = { x: 0, y: 0 };
      this.aiOverlay = false;
      this.tool = "select";
    },

    setTool(t: Tool) {
      this.tool = t;
    },

    setBrushPreset(p: BrushPreset) {
      this.brushPreset = p;
      this.tool = "brush";
    },

    setSelection(ids: string[]) {
      this.selectedIds = [...new Set(ids)];
    },
    toggleSelection(id: string) {
      const set = new Set(this.selectedIds);
      if (set.has(id)) set.delete(id);
      else set.add(id);
      this.selectedIds = [...set];
    },

    pushHistory() {
      this.undoStack.push({
        scene: deepClone(this.scene),
        selectedIds: [...this.selectedIds],
      });
      this.redoStack = [];
      // keep reasonable
      if (this.undoStack.length > 150) this.undoStack.shift();
    },

    undo() {
      const prev = this.undoStack.pop();
      if (!prev) return;
      this.redoStack.push({ scene: deepClone(this.scene), selectedIds: [...this.selectedIds] });
      this.scene = deepClone(prev.scene);
      this.selectedIds = [...prev.selectedIds];
    },

    redo() {
      const next = this.redoStack.pop();
      if (!next) return;
      this.undoStack.push({ scene: deepClone(this.scene), selectedIds: [...this.selectedIds] });
      this.scene = deepClone(next.scene);
      this.selectedIds = [...next.selectedIds];
    },

    addRect(kind: "rect" | "frame", x: number, y: number) {
      this.pushHistory();
      const id = uid(kind);
      const node: AnyNode =
        kind === "frame"
          ? ({
              id,
              type: "frame",
              name: "Frame",
              x,
              y,
              rotation: 0,
              scaleX: 1,
              scaleY: 1,
              width: 520,
              height: 340,
              opacity: 1,
              visible: true,
              locked: false,
              parentId: null,
              fill: "#0f0f12",
              stroke: "#3f3f46",
              strokeWidth: 2,
              radius: 16,
            } as const)
          : ({
              id,
              type: "rect",
              name: "Rect",
              x,
              y,
              rotation: 0,
              scaleX: 1,
              scaleY: 1,
              width: 260,
              height: 160,
              opacity: 1,
              visible: true,
              locked: false,
              parentId: null,
              fill: "#18181b",
              stroke: "#52525b",
              strokeWidth: 2,
              radius: 10,
            } as const);
      this.scene.nodes.push(node);
      this.selectedIds = [id];
    },

    addText(x: number, y: number) {
      this.pushHistory();
      const id = uid("text");
      this.scene.nodes.push({
        id,
        type: "text",
        name: "Text",
        x,
        y,
        rotation: 0,
        scaleX: 1,
        scaleY: 1,
        width: 320,
        height: 0,
        opacity: 1,
        visible: true,
        locked: false,
        parentId: null,
        text: "Edit me",
        fontSize: 24,
        fontFamily: "Inter, ui-sans-serif, system-ui",
        fill: "#f4f4f5",
        align: "left",
      });
      this.selectedIds = [id];
    },

    beginStroke(x: number, y: number) {
      this.pushHistory();
      const id = uid("stroke");
      const p = this.brushPreset;
      const node: AnyNode = {
        id,
        type: "stroke",
        name: `Stroke (${p.name})`,
        x: 0,
        y: 0,
        rotation: 0,
        scaleX: 1,
        scaleY: 1,
        opacity: clamp(p.opacity, 0, 1),
        visible: true,
        locked: false,
        parentId: null,
        points: [x, y],
        stroke: p.color,
        strokeWidth: p.width,
        tension: 0.25,
        lineCap: "round",
        lineJoin: "round",
      };
      this.scene.nodes.push(node);
      this.currentStrokeId = id;
      this.isDrawing = true;
      this.selectedIds = [id];
    },

    appendStroke(x: number, y: number) {
      if (!this.isDrawing || !this.currentStrokeId) return;
      const n = this.scene.nodes.find((s) => s.id === this.currentStrokeId);
      if (!n || n.type !== "stroke") return;
      const p = this.brushPreset;
      const j = clamp(p.jitter, 0, 1);
      const jx = (Math.random() - 0.5) * j * 6;
      const jy = (Math.random() - 0.5) * j * 6;
      n.points.push(x + jx, y + jy);
    },

    endStroke() {
      this.isDrawing = false;
      this.currentStrokeId = "";
    },

    addImage(dataUrl: string, x: number, y: number, w: number, h: number) {
      this.pushHistory();
      const id = uid("img");
      this.scene.nodes.push({
        id,
        type: "image",
        name: "Image",
        x,
        y,
        rotation: 0,
        scaleX: 1,
        scaleY: 1,
        width: w,
        height: h,
        opacity: 1,
        visible: true,
        locked: false,
        parentId: null,
        src: dataUrl,
      });
      this.selectedIds = [id];
    },

    updateNode(id: string, patch: Partial<AnyNode>) {
      const idx = this.scene.nodes.findIndex((n) => n.id === id);
      if (idx < 0) return;
      this.scene.nodes[idx] = { ...this.scene.nodes[idx], ...patch } as AnyNode;
    },

    updateSelected(patch: Partial<AnyNode>) {
      if (this.selectedIds.length === 0) return;
      this.pushHistory();
      for (const id of this.selectedIds) this.updateNode(id, patch);
    },

    deleteSelected() {
      if (this.selectedIds.length === 0) return;
      this.pushHistory();
      const toDelete = new Set(this.selectedIds);
      // also delete children of groups being deleted
      const childrenOf = (parentId: string) => this.scene.nodes.filter((n) => n.parentId === parentId).map((n) => n.id);
      for (const id of [...toDelete]) {
        for (const childId of childrenOf(id)) toDelete.add(childId);
      }
      this.scene.nodes = this.scene.nodes.filter((n) => !toDelete.has(n.id));
      this.selectedIds = [];
    },

    duplicateSelected() {
      if (this.selectedIds.length === 0) return;
      this.pushHistory();
      const clones: AnyNode[] = [];
      const map = new Map<string, string>(); // old -> new

      const cloneNode = (n: AnyNode): AnyNode => {
        const newId = uid(n.type);
        map.set(n.id, newId);
        const base = { ...deepClone(n), id: newId, x: n.x + 24, y: n.y + 24, name: `${n.name} copy` } as AnyNode;
        return base;
      };

      for (const id of this.selectedIds) {
        const n = this.scene.nodes.find((x) => x.id === id);
        if (!n) continue;
        clones.push(cloneNode(n));
        // shallow: if it's a group, also clone its direct children
        if (n.type === "group") {
          for (const child of this.scene.nodes.filter((c) => c.parentId === n.id)) {
            const c2 = cloneNode(child);
            c2.parentId = map.get(n.id)!;
            clones.push(c2);
          }
        }
      }
      this.scene.nodes.push(...clones);
      this.selectedIds = clones.map((c) => c.id);
    },

    groupSelected() {
      if (this.selectedIds.length < 2) return;
      this.pushHistory();
      const selected = this.selectedNodes.filter((n) => n.visible && !n.locked);
      if (selected.length < 2) return;

      const groupId = uid("group");
      this.scene.nodes.push({
        id: groupId,
        type: "group",
        name: "Group",
        x: 0,
        y: 0,
        rotation: 0,
        scaleX: 1,
        scaleY: 1,
        opacity: 1,
        visible: true,
        locked: false,
        parentId: null,
        collapsed: false,
      });
      for (const n of selected) this.updateNode(n.id, { parentId: groupId });
      this.selectedIds = [groupId];
    },

    ungroupSelected() {
      if (this.selectedIds.length !== 1) return;
      const id = this.selectedIds[0]!;
      const n = this.scene.nodes.find((x) => x.id === id);
      if (!n || n.type !== "group") return;
      this.pushHistory();
      for (const child of this.scene.nodes.filter((c) => c.parentId === id)) {
        this.updateNode(child.id, { parentId: null });
      }
      this.scene.nodes = this.scene.nodes.filter((x) => x.id !== id);
      this.selectedIds = [];
    },

    reorderNode(id: string, beforeId: string | null) {
      const idx = this.scene.nodes.findIndex((n) => n.id === id);
      if (idx < 0) return;
      this.pushHistory();
      const [n] = this.scene.nodes.splice(idx, 1);
      if (!n) return;
      if (beforeId == null) {
        this.scene.nodes.push(n);
        return;
      }
      const j = this.scene.nodes.findIndex((x) => x.id === beforeId);
      if (j < 0) this.scene.nodes.push(n);
      else this.scene.nodes.splice(j, 0, n);
    },

    // For overlay export/labels
    getNodeBBox(id: string) {
      const n = this.scene.nodes.find((x) => x.id === id);
      if (!n) return null;
      if (n.type === "stroke") {
        const bb = bboxFromPoints(n.points);
        return { x: bb.x, y: bb.y, width: bb.width, height: bb.height };
      }
      const w = (n as any).width ?? 0;
      const h = (n as any).height ?? 0;
      return { x: n.x, y: n.y, width: w * (n.scaleX ?? 1), height: h * (n.scaleY ?? 1) };
    },
  },
});

