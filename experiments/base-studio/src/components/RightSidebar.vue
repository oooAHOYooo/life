<script setup lang="ts">
import LayersPanel from "@/components/panels/LayersPanel.vue";
import PropertiesPanel from "@/components/panels/PropertiesPanel.vue";

const tab = defineModel<"Layers" | "Properties">("tab", { required: true });
</script>

<template>
  <div class="flex h-full flex-col border-l border-zinc-800 bg-zinc-950">
    <div class="flex gap-1 border-b border-zinc-800 p-2">
      <button
        v-for="t in ['Layers', 'Properties'] as const"
        :key="t"
        class="flex-1 rounded-lg px-3 py-2 text-sm"
        :class="tab === t ? 'bg-zinc-900 text-zinc-100' : 'text-zinc-400 hover:bg-zinc-900/60 hover:text-zinc-200'"
        @click="tab = t"
      >
        {{ t }}
      </button>
    </div>
    <div class="min-h-0 flex-1 overflow-auto">
      <LayersPanel v-if="tab === 'Layers'" />
      <PropertiesPanel v-else />
    </div>
  </div>
</template>

