<script setup lang="ts">
import { computed } from "vue";
import { useEditorStore } from "@/stores/editor";

const editor = useEditorStore();

const tool = computed({
  get: () => editor.tool,
  set: (v) => editor.setTool(v as any),
});

function addFrame() {
  editor.addRect("frame", 140, 120);
}
function addRect() {
  editor.addRect("rect", 220, 180);
}
function addText() {
  editor.addText(260, 220);
}
</script>

<template>
  <div class="p-3">
    <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Tools</div>
    <div class="mt-2 grid grid-cols-2 gap-2">
      <button
        class="rounded-lg border border-zinc-800 px-3 py-2 text-sm"
        :class="tool === 'select' ? 'bg-zinc-900 text-zinc-100' : 'bg-zinc-950 text-zinc-300 hover:bg-zinc-900/60'"
        @click="tool = 'select'"
      >
        Select
      </button>
      <button
        class="rounded-lg border border-zinc-800 px-3 py-2 text-sm"
        :class="tool === 'brush' ? 'bg-zinc-900 text-zinc-100' : 'bg-zinc-950 text-zinc-300 hover:bg-zinc-900/60'"
        @click="tool = 'brush'"
      >
        Brush
      </button>
      <button
        class="rounded-lg border border-zinc-800 px-3 py-2 text-sm"
        :class="tool === 'rect' ? 'bg-zinc-900 text-zinc-100' : 'bg-zinc-950 text-zinc-300 hover:bg-zinc-900/60'"
        @click="tool = 'rect'"
      >
        Rect
      </button>
      <button
        class="rounded-lg border border-zinc-800 px-3 py-2 text-sm"
        :class="tool === 'text' ? 'bg-zinc-900 text-zinc-100' : 'bg-zinc-950 text-zinc-300 hover:bg-zinc-900/60'"
        @click="tool = 'text'"
      >
        Text
      </button>
      <button
        class="rounded-lg border border-zinc-800 px-3 py-2 text-sm"
        :class="tool === 'frame' ? 'bg-zinc-900 text-zinc-100' : 'bg-zinc-950 text-zinc-300 hover:bg-zinc-900/60'"
        @click="tool = 'frame'"
      >
        Frame
      </button>
    </div>

    <div class="mt-6 text-xs font-semibold uppercase tracking-wide text-zinc-400">Quick insert</div>
    <div class="mt-2 grid grid-cols-1 gap-2">
      <button class="rounded-lg bg-zinc-900 px-3 py-2 text-sm text-zinc-100 hover:bg-zinc-800" @click="addFrame">
        Add Frame
      </button>
      <button class="rounded-lg bg-zinc-900 px-3 py-2 text-sm text-zinc-100 hover:bg-zinc-800" @click="addRect">
        Add Rect
      </button>
      <button class="rounded-lg bg-zinc-900 px-3 py-2 text-sm text-zinc-100 hover:bg-zinc-800" @click="addText">
        Add Text
      </button>
    </div>

    <div class="mt-6 rounded-xl border border-zinc-800 bg-zinc-950 p-3 text-xs text-zinc-400">
      <div class="font-semibold text-zinc-300">Canvas gestures</div>
      <div class="mt-2 space-y-1">
        <div><span class="text-zinc-200">Space + drag</span> pans</div>
        <div><span class="text-zinc-200">Cmd/Ctrl + wheel</span> zooms</div>
        <div><span class="text-zinc-200">Shift + click</span> multi-select</div>
      </div>
    </div>
  </div>
</template>

