<script setup lang="ts">
import { computed, ref } from "vue";
import type { BrushPreset } from "@/types/scene";
import { DEFAULT_PRESETS, PRESET_CATEGORIES } from "@/data/presets";
import { useEditorStore } from "@/stores/editor";

const editor = useEditorStore();
const filter = ref<BrushPreset["category"] | "All">("All");

const manyPresets = computed(() => {
  // Expand defaults into a "megapack-ish" list by generating small variations.
  const out: BrushPreset[] = [];
  for (const p of DEFAULT_PRESETS) {
    out.push(p);
    for (let i = 1; i <= 6; i++) {
      out.push({
        ...p,
        id: `${p.id}_${i}`,
        name: `${p.name} ${i}`,
        width: Math.max(1, Math.round(p.width * (1 + i * 0.06))),
        opacity: Math.max(0.08, Math.min(1, p.opacity * (1 - i * 0.05))),
        jitter: Math.min(1, p.jitter + i * 0.03),
      });
    }
  }
  return out;
});

const visiblePresets = computed(() => {
  if (filter.value === "All") return manyPresets.value;
  return manyPresets.value.filter((p) => p.category === filter.value);
});

function pick(p: BrushPreset) {
  editor.setBrushPreset(p);
}
</script>

<template>
  <div class="p-3">
    <div class="flex items-center justify-between">
      <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Presets</div>
      <select
        v-model="filter"
        class="h-8 rounded-lg border border-zinc-800 bg-zinc-900 px-2 text-xs text-zinc-100"
      >
        <option value="All">All</option>
        <option v-for="c in PRESET_CATEGORIES" :key="c" :value="c">{{ c }}</option>
      </select>
    </div>

    <div class="mt-3 space-y-2">
      <button
        v-for="p in visiblePresets"
        :key="p.id"
        class="flex w-full items-center gap-3 rounded-xl border border-zinc-800 bg-zinc-950 px-3 py-2 text-left hover:bg-zinc-900/60"
        :style="editor.brushPreset.id === p.id ? { borderColor: '#6366f1' } : undefined"
        @click="pick(p)"
      >
        <div class="shrink-0">
          <v-stage :config="{ width: 64, height: 22 }">
            <v-layer>
              <v-line
                :config="{
                  points: [6, 14, 18, 8, 30, 15, 42, 7, 58, 13],
                  stroke: p.color,
                  strokeWidth: p.width,
                  opacity: p.opacity,
                  lineCap: 'round',
                  lineJoin: 'round',
                  tension: 0.3
                }"
              />
            </v-layer>
          </v-stage>
        </div>
        <div class="min-w-0 flex-1">
          <div class="truncate text-sm font-medium text-zinc-100">{{ p.name }}</div>
          <div class="mt-0.5 text-xs text-zinc-500">
            {{ p.category }} • w {{ p.width }} • o {{ Math.round(p.opacity * 100) }}% • j {{ Math.round(p.jitter * 100) }}%
          </div>
        </div>
      </button>
    </div>
  </div>
</template>

