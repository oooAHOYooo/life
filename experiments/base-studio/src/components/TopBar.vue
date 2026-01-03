<script setup lang="ts">
import { computed, ref } from "vue";
import tokens from "@/tokens/tokens.json";
import { useEditorStore } from "@/stores/editor";
import { useProjectsStore } from "@/stores/projects";
import { useRuntimeStore } from "@/stores/runtime";
import { buildAnnotationLayer, downloadAnnotatedPng, downloadCleanPng, exportAiBundleZip, exportSceneJson } from "@/utils/export";

const emit = defineEmits<{ (e: "back"): void }>();

const editor = useEditorStore();
const projects = useProjectsStore();
const runtime = useRuntimeStore();

const zoomPct = computed(() => Math.round(editor.zoom * 100));
const exporting = ref(false);

function labelsForScene() {
  return editor.scene.nodes
    .filter((n) => n.visible)
    .map((n) => {
      const bb = editor.getNodeBBox(n.id);
      if (!bb) return null;
      return {
        id: n.id,
        name: n.name,
        type: n.type,
        x: bb.x,
        y: bb.y,
        width: bb.width,
        height: bb.height,
      };
    })
    .filter(Boolean) as Array<{ id: string; name: string; type: string; x: number; y: number; width: number; height: number }>;
}

function ensureAnnotLayer() {
  const stage = runtime.stage;
  if (!stage) return;
  buildAnnotationLayer(stage, labelsForScene());
}

function exportPngClean() {
  const stage = runtime.stage;
  if (!stage) return;
  downloadCleanPng(stage, "design");
}

function exportPngAnnotated() {
  const stage = runtime.stage;
  if (!stage) return;
  ensureAnnotLayer();
  downloadAnnotatedPng(stage, "design");
}

function exportScene() {
  exportSceneJson(editor.scene, "design");
}

async function exportBundle() {
  const stage = runtime.stage;
  if (!stage) return;
  exporting.value = true;
  try {
    ensureAnnotLayer();
    await exportAiBundleZip({
      stage,
      filenameBase: editor.projectName,
      projectName: editor.projectName,
      scene: editor.scene,
      tokensJson: tokens,
    });
  } finally {
    exporting.value = false;
  }
}

function toggleOverlay() {
  editor.aiOverlay = !editor.aiOverlay;
  if (editor.aiOverlay) ensureAnnotLayer();
}

async function captureThumb() {
  const stage = runtime.stage;
  if (!stage || !editor.projectId) return;
  // quick thumb: low-res, no UI layers
  const ui = stage.findOne(".ui") as any;
  const prev = ui?.visible?.();
  if (ui?.visible) ui.visible(false);
  stage.draw();
  const url = stage.toDataURL({ pixelRatio: 0.35 });
  if (ui?.visible && prev != null) ui.visible(prev);
  stage.draw();
  await projects.updateThumbnail(editor.projectId, url);
}

const showMenu = ref(false);
function toggleMenu() {
  showMenu.value = !showMenu.value;
}
function closeMenu() {
  showMenu.value = false;
}

const fileInput = ref<HTMLInputElement | null>(null);
async function fileToDataUrl(file: File): Promise<string> {
  return new Promise((resolve, reject) => {
    const r = new FileReader();
    r.onload = () => resolve(String(r.result));
    r.onerror = () => reject(new Error("Failed to read file"));
    r.readAsDataURL(file);
  });
}
async function onPickImage(e: Event) {
  const input = e.target as HTMLInputElement;
  const f = input.files?.[0];
  input.value = "";
  if (!f) return;
  const url = await fileToDataUrl(f);
  editor.addImage(url, 180, 140, 360, 240);
}
</script>

<template>
  <div class="flex h-14 items-center justify-between border-b border-zinc-800 bg-zinc-950 px-3">
    <div class="flex items-center gap-2">
      <button class="rounded-md px-2 py-1 text-sm text-zinc-300 hover:bg-zinc-900" @click="emit('back')">
        Projects
      </button>
      <div class="h-6 w-px bg-zinc-800"></div>
      <input
        v-model="editor.projectName"
        class="h-9 w-64 rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100"
        spellcheck="false"
      />
      <button class="rounded-md px-2 py-1 text-xs text-zinc-400 hover:bg-zinc-900 hover:text-zinc-200" @click="captureThumb">
        Save thumb
      </button>
    </div>

    <div class="flex items-center gap-2">
      <button
        class="rounded-md bg-zinc-900 px-3 py-1.5 text-sm text-zinc-200 hover:bg-zinc-800 disabled:opacity-60"
        :disabled="!editor.canUndo"
        @click="editor.undo"
      >
        Undo
      </button>
      <button
        class="rounded-md bg-zinc-900 px-3 py-1.5 text-sm text-zinc-200 hover:bg-zinc-800 disabled:opacity-60"
        :disabled="!editor.canRedo"
        @click="editor.redo"
      >
        Redo
      </button>

      <div class="h-6 w-px bg-zinc-800"></div>

      <div class="text-xs text-zinc-400">{{ zoomPct }}%</div>
      <button
        class="rounded-md bg-zinc-900 px-3 py-1.5 text-sm text-zinc-200 hover:bg-zinc-800"
        @click="toggleOverlay"
        :style="editor.aiOverlay ? { border: `1px solid ${tokens.palette.accent}` } : undefined"
      >
        AI Overlay
      </button>

      <div class="relative">
        <button
          class="rounded-md bg-indigo-500 px-3 py-1.5 text-sm font-medium text-white hover:bg-indigo-400 disabled:opacity-60"
          :disabled="exporting"
          @click="toggleMenu"
          @blur="closeMenu"
        >
          Export
        </button>
        <div
          v-if="showMenu"
          class="absolute right-0 z-50 mt-2 w-64 overflow-hidden rounded-xl border border-zinc-800 bg-zinc-950 shadow-xl"
        >
          <button class="w-full px-4 py-2 text-left text-sm hover:bg-zinc-900" @mousedown.prevent @click="exportPngClean">
            Export PNG (clean)
          </button>
          <button class="w-full px-4 py-2 text-left text-sm hover:bg-zinc-900" @mousedown.prevent @click="exportPngAnnotated">
            Export PNG (annotated)
          </button>
          <button class="w-full px-4 py-2 text-left text-sm hover:bg-zinc-900" @mousedown.prevent @click="exportScene">
            Export scene JSON
          </button>
          <button class="w-full px-4 py-2 text-left text-sm hover:bg-zinc-900" @mousedown.prevent @click="exportBundle">
            Export AI Bundle (.zip)
          </button>
          <div class="h-px bg-zinc-800"></div>
          <label class="block w-full cursor-pointer px-4 py-2 text-left text-sm hover:bg-zinc-900">
            Import Image…
            <input ref="fileInput" class="hidden" type="file" accept="image/*" @change="onPickImage" />
          </label>
        </div>
      </div>
    </div>
  </div>
</template>

