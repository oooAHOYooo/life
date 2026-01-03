<script setup lang="ts">
import tokens from "@/tokens/tokens.json";
import { useEditorStore } from "@/stores/editor";

const editor = useEditorStore();

function applyFill(color: string) {
  editor.updateSelected({ fill: color } as any);
}
function applyStroke(color: string) {
  editor.updateSelected({ stroke: color } as any);
}
function applyFontSize(size: number) {
  editor.updateSelected({ fontSize: size } as any);
}
function applyRadius(radius: number) {
  editor.updateSelected({ radius } as any);
}
</script>

<template>
  <div class="p-3">
    <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Palette</div>
    <div class="mt-2 grid grid-cols-2 gap-2">
      <button
        v-for="(c, name) in tokens.palette"
        :key="name"
        class="flex items-center justify-between rounded-lg border border-zinc-800 bg-zinc-950 px-3 py-2 text-left text-xs hover:bg-zinc-900/60"
        @click="applyFill(String(c))"
        title="Click to apply as fill to selection"
      >
        <span class="truncate text-zinc-300">{{ name }}</span>
        <span class="h-4 w-8 rounded border border-zinc-800" :style="{ background: String(c) }"></span>
      </button>
    </div>

    <div class="mt-6 text-xs font-semibold uppercase tracking-wide text-zinc-400">Stroke quick</div>
    <div class="mt-2 flex flex-wrap gap-2">
      <button
        v-for="(c, name) in tokens.palette"
        :key="`s-${name}`"
        class="h-8 rounded-lg border border-zinc-800 px-3 text-xs text-zinc-300 hover:bg-zinc-900/60"
        @click="applyStroke(String(c))"
      >
        {{ name }}
      </button>
    </div>

    <div class="mt-6 text-xs font-semibold uppercase tracking-wide text-zinc-400">Font sizes</div>
    <div class="mt-2 flex flex-wrap gap-2">
      <button
        v-for="(v, k) in tokens.fontSizes"
        :key="k"
        class="rounded-lg border border-zinc-800 bg-zinc-950 px-3 py-2 text-xs text-zinc-300 hover:bg-zinc-900/60"
        @click="applyFontSize(Number(v))"
      >
        {{ k }} ({{ v }})
      </button>
    </div>

    <div class="mt-6 text-xs font-semibold uppercase tracking-wide text-zinc-400">Radii</div>
    <div class="mt-2 flex flex-wrap gap-2">
      <button
        v-for="(v, k) in tokens.radii"
        :key="k"
        class="rounded-lg border border-zinc-800 bg-zinc-950 px-3 py-2 text-xs text-zinc-300 hover:bg-zinc-900/60"
        @click="applyRadius(Number(v))"
      >
        {{ k }} ({{ v }})
      </button>
    </div>

    <div class="mt-6 rounded-xl border border-zinc-800 bg-zinc-950 p-3 text-xs text-zinc-400">
      <div class="font-semibold text-zinc-300">Tip</div>
      <div class="mt-1">Tokens apply to the current selection (fill/stroke/fontSize/radius where supported).</div>
    </div>
  </div>
</template>

