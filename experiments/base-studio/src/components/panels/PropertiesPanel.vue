<script setup lang="ts">
import { computed, ref, watch } from "vue";
import type { AnyNode } from "@/types/scene";
import { useEditorStore } from "@/stores/editor";

const editor = useEditorStore();

const single = computed<AnyNode | null>(() => (editor.selectedNodes.length === 1 ? editor.selectedNodes[0]! : null));

const local = ref({
  x: 0,
  y: 0,
  width: 0,
  height: 0,
  rotation: 0,
  opacity: 1,
  fill: "",
  stroke: "",
  strokeWidth: 2,
  radius: 0,
  text: "",
  fontSize: 16,
});

watch(
  () => single.value,
  (n) => {
    if (!n) return;
    local.value = {
      x: Math.round(n.x),
      y: Math.round(n.y),
      width: Math.round((n as any).width ?? 0),
      height: Math.round((n as any).height ?? 0),
      rotation: Math.round(n.rotation ?? 0),
      opacity: Number(n.opacity ?? 1),
      fill: String((n as any).fill ?? ""),
      stroke: String((n as any).stroke ?? ""),
      strokeWidth: Number((n as any).strokeWidth ?? 2),
      radius: Number((n as any).radius ?? 0),
      text: String((n as any).text ?? ""),
      fontSize: Number((n as any).fontSize ?? 16),
    };
  },
  { immediate: true }
);

function commit(patch: Partial<AnyNode>) {
  editor.updateSelected(patch);
}
</script>

<template>
  <div class="p-3">
    <div class="flex items-center justify-between">
      <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Properties</div>
      <div class="text-xs text-zinc-500">
        <template v-if="editor.selectedIds.length === 0">none</template>
        <template v-else>{{ editor.selectedIds.length }} selected</template>
      </div>
    </div>

    <div v-if="editor.selectedIds.length === 0" class="mt-4 text-sm text-zinc-400">
      Select an object to edit its properties.
    </div>

    <div v-else-if="!single" class="mt-4 rounded-xl border border-zinc-800 bg-zinc-950 p-3 text-sm text-zinc-300">
      Multiple selection. Use Tokens panel for quick styling, or use the canvas transformer to resize/rotate.
      <div class="mt-3 grid grid-cols-2 gap-2">
        <label class="text-xs text-zinc-400">
          Opacity
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            step="0.05"
            min="0"
            max="1"
            :value="1"
            @change="commit({ opacity: Number(($event.target as HTMLInputElement).value) } as any)"
          />
        </label>
      </div>
    </div>

    <div v-else class="mt-4 space-y-4">
      <div class="rounded-xl border border-zinc-800 bg-zinc-950 p-3">
        <div class="text-sm font-medium text-zinc-100">{{ single.name }}</div>
        <div class="mt-1 text-xs text-zinc-500">{{ single.type }} • {{ single.id }}</div>
      </div>

      <div class="grid grid-cols-2 gap-2">
        <label class="text-xs text-zinc-400">
          X
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            v-model.number="local.x"
            @change="commit({ x: Number(local.x) } as any)"
          />
        </label>
        <label class="text-xs text-zinc-400">
          Y
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            v-model.number="local.y"
            @change="commit({ y: Number(local.y) } as any)"
          />
        </label>
        <label v-if="single.type !== 'stroke' && single.type !== 'group'" class="text-xs text-zinc-400">
          W
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            v-model.number="local.width"
            @change="commit({ width: Number(local.width) } as any)"
          />
        </label>
        <label v-if="single.type !== 'stroke' && single.type !== 'group'" class="text-xs text-zinc-400">
          H
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            v-model.number="local.height"
            @change="commit({ height: Number(local.height) } as any)"
          />
        </label>
        <label class="text-xs text-zinc-400">
          Rotation
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            v-model.number="local.rotation"
            @change="commit({ rotation: Number(local.rotation) } as any)"
          />
        </label>
        <label class="text-xs text-zinc-400">
          Opacity
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            step="0.05"
            min="0"
            max="1"
            v-model.number="local.opacity"
            @change="commit({ opacity: Number(local.opacity) } as any)"
          />
        </label>
      </div>

      <div v-if="single.type === 'text'" class="rounded-xl border border-zinc-800 bg-zinc-950 p-3">
        <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Text</div>
        <label class="mt-2 block text-xs text-zinc-400">
          Content
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            v-model="local.text"
            @change="commit({ text: local.text } as any)"
          />
        </label>
        <label class="mt-2 block text-xs text-zinc-400">
          Font size
          <input
            class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
            type="number"
            v-model.number="local.fontSize"
            @change="commit({ fontSize: Number(local.fontSize) } as any)"
          />
        </label>
      </div>

      <div v-if="single.type === 'rect' || single.type === 'frame'" class="rounded-xl border border-zinc-800 bg-zinc-950 p-3">
        <div class="text-xs font-semibold uppercase tracking-wide text-zinc-400">Shape</div>
        <div class="mt-2 grid grid-cols-2 gap-2">
          <label class="text-xs text-zinc-400">
            Fill
            <input
              class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
              v-model="local.fill"
              @change="commit({ fill: local.fill } as any)"
            />
          </label>
          <label class="text-xs text-zinc-400">
            Stroke
            <input
              class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
              v-model="local.stroke"
              @change="commit({ stroke: local.stroke } as any)"
            />
          </label>
          <label class="text-xs text-zinc-400">
            Stroke width
            <input
              class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
              type="number"
              v-model.number="local.strokeWidth"
              @change="commit({ strokeWidth: Number(local.strokeWidth) } as any)"
            />
          </label>
          <label class="text-xs text-zinc-400">
            Radius
            <input
              class="mt-1 h-9 w-full rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
              type="number"
              v-model.number="local.radius"
              @change="commit({ radius: Number(local.radius) } as any)"
            />
          </label>
        </div>
      </div>
    </div>
  </div>
</template>

