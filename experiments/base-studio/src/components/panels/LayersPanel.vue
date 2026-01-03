<script setup lang="ts">
import { computed, ref } from "vue";
import type { AnyNode } from "@/types/scene";
import { useEditorStore } from "@/stores/editor";

const editor = useEditorStore();

type Row = {
  id: string;
  node: AnyNode;
  depth: number;
  isGroup: boolean;
};

const draggingId = ref<string | null>(null);

function childrenOf(parentId: string) {
  return editor.scene.nodes.filter((n) => n.parentId === parentId);
}

function buildRows(parentId: string | null, depth: number, out: Row[]) {
  const nodes = editor.scene.nodes.filter((n) => n.parentId === parentId);
  for (const n of nodes) {
    const isGroup = n.type === "group";
    out.push({ id: n.id, node: n, depth, isGroup });
    if (isGroup && !(n as any).collapsed) buildRows(n.id, depth + 1, out);
  }
}

const rows = computed(() => {
  const out: Row[] = [];
  buildRows(null, 0, out);
  return out;
});

function selectRow(e: MouseEvent, id: string) {
  if (e.shiftKey) editor.toggleSelection(id);
  else editor.setSelection([id]);
}

function toggleVisible(id: string) {
  const n = editor.scene.nodes.find((x) => x.id === id);
  if (!n) return;
  editor.pushHistory();
  editor.updateNode(id, { visible: !n.visible } as any);
}

function toggleLock(id: string) {
  const n = editor.scene.nodes.find((x) => x.id === id);
  if (!n) return;
  editor.pushHistory();
  editor.updateNode(id, { locked: !n.locked } as any);
}

function toggleCollapse(id: string) {
  const n = editor.scene.nodes.find((x) => x.id === id);
  if (!n || n.type !== "group") return;
  editor.pushHistory();
  editor.updateNode(id, { collapsed: !(n as any).collapsed } as any);
}

function rename(id: string, name: string) {
  editor.pushHistory();
  editor.updateNode(id, { name: name.trim() || "Layer" } as any);
}

function onDragStart(e: DragEvent, id: string) {
  draggingId.value = id;
  e.dataTransfer?.setData("text/plain", id);
  e.dataTransfer?.setDragImage(new Image(), 0, 0);
}

function onDropOnRow(e: DragEvent, beforeId: string) {
  e.preventDefault();
  const id = e.dataTransfer?.getData("text/plain") || draggingId.value;
  if (!id || id === beforeId) return;
  // only reorder within same parent (MVP)
  const a = editor.scene.nodes.find((n) => n.id === id);
  const b = editor.scene.nodes.find((n) => n.id === beforeId);
  if (!a || !b) return;
  if (a.parentId !== b.parentId) return;
  editor.reorderNode(id, beforeId);
  draggingId.value = null;
}

function onDropToEnd(e: DragEvent, parentId: string | null) {
  e.preventDefault();
  const id = e.dataTransfer?.getData("text/plain") || draggingId.value;
  if (!id) return;
  const a = editor.scene.nodes.find((n) => n.id === id);
  if (!a) return;
  if (a.parentId !== parentId) return;
  editor.reorderNode(id, null);
  draggingId.value = null;
}

function allowDrop(e: DragEvent) {
  e.preventDefault();
}

const selectedSet = computed(() => new Set(editor.selectedIds));
</script>

<template>
  <div class="p-3">
    <div class="flex items-center justify-between">
      <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Layers</div>
      <div class="text-xs text-zinc-500">{{ editor.scene.nodes.length }}</div>
    </div>

    <div class="mt-3 space-y-1">
      <div
        v-for="r in rows"
        :key="r.id"
        class="group flex items-center gap-2 rounded-lg border border-transparent px-2 py-1.5"
        :class="selectedSet.has(r.id) ? 'border-indigo-500 bg-zinc-900' : 'hover:bg-zinc-900/60'"
        draggable="true"
        @dragstart="onDragStart($event, r.id)"
        @dragover="allowDrop"
        @drop="onDropOnRow($event, r.id)"
        @click="selectRow($event as any, r.id)"
      >
        <div class="flex items-center gap-2" :style="{ paddingLeft: `${r.depth * 14}px` }">
          <button
            v-if="r.isGroup"
            class="h-6 w-6 rounded-md text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200"
            @click.stop="toggleCollapse(r.id)"
            :title="(r.node as any).collapsed ? 'Expand' : 'Collapse'"
          >
            {{ (r.node as any).collapsed ? "+" : "–" }}
          </button>
          <div v-else class="h-6 w-6"></div>
        </div>

        <div class="min-w-0 flex-1">
          <input
            class="w-full bg-transparent text-sm text-zinc-100 outline-none"
            :value="r.node.name"
            @click.stop
            @keydown.stop
            @change="rename(r.id, ($event.target as HTMLInputElement).value)"
          />
          <div class="text-[11px] text-zinc-500">{{ r.node.type }} • {{ r.id }}</div>
        </div>

        <button
          class="h-8 rounded-md px-2 text-xs text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200"
          @click.stop="toggleVisible(r.id)"
          :title="r.node.visible ? 'Hide' : 'Show'"
        >
          {{ r.node.visible ? "Visible" : "Hidden" }}
        </button>
        <button
          class="h-8 rounded-md px-2 text-xs text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200"
          @click.stop="toggleLock(r.id)"
          :title="r.node.locked ? 'Unlock' : 'Lock'"
        >
          {{ r.node.locked ? "Locked" : "Unlocked" }}
        </button>
      </div>

      <div
        class="mt-2 rounded-lg border border-dashed border-zinc-800 p-2 text-center text-xs text-zinc-500"
        @dragover="allowDrop"
        @drop="onDropToEnd($event, null)"
      >
        Drop here to send to top-level end
      </div>
    </div>

    <div class="mt-4 rounded-xl border border-zinc-800 bg-zinc-950 p-3 text-xs text-zinc-400">
      <div class="font-semibold text-zinc-300">Commands</div>
      <div class="mt-2 space-y-1">
        <div><span class="text-zinc-200">Cmd/Ctrl+G</span> group</div>
        <div><span class="text-zinc-200">Cmd/Ctrl+Shift+G</span> ungroup</div>
        <div><span class="text-zinc-200">Cmd/Ctrl+D</span> duplicate</div>
        <div><span class="text-zinc-200">Delete</span> delete</div>
        <div><span class="text-zinc-200">Cmd/Ctrl+Z</span> undo • <span class="text-zinc-200">Cmd/Ctrl+Shift+Z</span> redo</div>
      </div>
    </div>
  </div>
</template>

