<script setup lang="ts">
import PresetsPanel from "@/components/panels/PresetsPanel.vue";
import ComponentsPanel from "@/components/panels/ComponentsPanel.vue";
import TokensPanel from "@/components/panels/TokensPanel.vue";

const tab = defineModel<"Presets" | "Components" | "Tokens">("tab", { required: true });
</script>

<template>
  <div class="flex h-full flex-col border-r border-zinc-800 bg-zinc-950">
    <div class="flex gap-1 border-b border-zinc-800 p-2">
      <button
        v-for="t in ['Presets', 'Components', 'Tokens'] as const"
        :key="t"
        class="flex-1 rounded-lg px-3 py-2 text-sm"
        :class="tab === t ? 'bg-zinc-900 text-zinc-100' : 'text-zinc-400 hover:bg-zinc-900/60 hover:text-zinc-200'"
        @click="tab = t"
      >
        {{ t }}
      </button>
    </div>
    <div class="min-h-0 flex-1 overflow-auto">
      <PresetsPanel v-if="tab === 'Presets'" />
      <ComponentsPanel v-else-if="tab === 'Components'" />
      <TokensPanel v-else />
    </div>
  </div>
</template>

