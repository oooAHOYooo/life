<script setup lang="ts">
import { computed, onBeforeUnmount, onMounted, ref, watch } from "vue";
import TopBar from "@/components/TopBar.vue";
import LeftSidebar from "@/components/LeftSidebar.vue";
import RightSidebar from "@/components/RightSidebar.vue";
import CanvasStage from "@/components/CanvasStage.vue";
import { useProjectsStore } from "@/stores/projects";
import { useEditorStore } from "@/stores/editor";
import { useRuntimeStore } from "@/stores/runtime";
import tokens from "@/tokens/tokens.json";

const projects = useProjectsStore();
const editor = useEditorStore();
const runtime = useRuntimeStore();

const leftTab = ref<"Presets" | "Components" | "Tokens">("Presets");
const rightTab = ref<"Layers" | "Properties">("Layers");

const projectMeta = computed(() => projects.projects.find((p) => p.id === editor.projectId));

// ---- autosave (scene + tokens.json snapshot) ----
let saveTimer: number | null = null;
let thumbTimer: number | null = null;
function scheduleSave() {
  if (!editor.projectId) return;
  if (saveTimer) window.clearTimeout(saveTimer);
  saveTimer = window.setTimeout(async () => {
    await projects.saveProjectData(editor.projectId, editor.scene, tokens);
  }, 600);
}

function scheduleThumb() {
  if (!editor.projectId) return;
  if (thumbTimer) window.clearTimeout(thumbTimer);
  thumbTimer = window.setTimeout(async () => {
    const stage = runtime.stage;
    if (!stage) return;
    const ui = stage.findOne(".ui") as any;
    const annot = stage.findOne(".annot") as any;
    const prevUi = ui?.visible?.();
    const prevAnnot = annot?.visible?.();
    if (ui?.visible) ui.visible(false);
    if (annot?.visible) annot.visible(false);
    stage.draw();
    const url = stage.toDataURL({ pixelRatio: 0.25 });
    if (ui?.visible && prevUi != null) ui.visible(prevUi);
    if (annot?.visible && prevAnnot != null) annot.visible(prevAnnot);
    stage.draw();
    await projects.updateThumbnail(editor.projectId, url);
  }, 1500);
}

watch(
  () => editor.scene,
  () => {
    scheduleSave();
    scheduleThumb();
  },
  { deep: true }
);

watch(
  () => editor.projectName,
  async (name) => {
    if (!editor.projectId) return;
    await projects.renameProject(editor.projectId, name);
  }
);

// ---- keyboard shortcuts ----
function onKeyDown(e: KeyboardEvent) {
  const mod = e.metaKey || e.ctrlKey;
  if (!mod && e.key === "Delete") {
    e.preventDefault();
    editor.deleteSelected();
    return;
  }
  if (!mod) return;

  if (e.key.toLowerCase() === "z") {
    e.preventDefault();
    if (e.shiftKey) editor.redo();
    else editor.undo();
    return;
  }
  if (e.key.toLowerCase() === "d") {
    e.preventDefault();
    editor.duplicateSelected();
    return;
  }
  if (e.key.toLowerCase() === "g") {
    e.preventDefault();
    if (e.shiftKey) editor.ungroupSelected();
    else editor.groupSelected();
    return;
  }
  if (e.key === "Backspace") {
    e.preventDefault();
    editor.deleteSelected();
  }
}

// ---- image import/paste/dragdrop ----
async function fileToDataUrl(file: File): Promise<string> {
  return new Promise((resolve, reject) => {
    const r = new FileReader();
    r.onload = () => resolve(String(r.result));
    r.onerror = () => reject(new Error("Failed to read file"));
    r.readAsDataURL(file);
  });
}

async function addImageFile(file: File) {
  const url = await fileToDataUrl(file);
  // basic default size; CanvasStage can refine on load
  editor.addImage(url, 180, 140, 360, 240);
}

async function onPaste(e: ClipboardEvent) {
  const items = e.clipboardData?.items;
  if (!items) return;
  for (const it of items) {
    if (it.type.startsWith("image/")) {
      e.preventDefault();
      const f = it.getAsFile();
      if (f) await addImageFile(f);
      return;
    }
  }
}

async function onDrop(e: DragEvent) {
  const files = e.dataTransfer?.files;
  if (!files || files.length === 0) return;
  const imgs = [...files].filter((f) => f.type.startsWith("image/"));
  if (imgs.length === 0) return;
  e.preventDefault();
  for (const f of imgs) await addImageFile(f);
}

function onDragOver(e: DragEvent) {
  if ([...(e.dataTransfer?.items ?? [])].some((x) => x.type.startsWith("image/"))) {
    e.preventDefault();
  }
}

function goBack() {
  projects.goProjects();
}

onMounted(() => {
  window.addEventListener("keydown", onKeyDown);
  window.addEventListener("paste", onPaste as any);
  window.addEventListener("drop", onDrop);
  window.addEventListener("dragover", onDragOver);
});

onBeforeUnmount(() => {
  window.removeEventListener("keydown", onKeyDown);
  window.removeEventListener("paste", onPaste as any);
  window.removeEventListener("drop", onDrop);
  window.removeEventListener("dragover", onDragOver);
});

watch(
  () => projectMeta.value?.name,
  (name) => {
    if (name) editor.projectName = name;
  },
  { immediate: true }
);
</script>

<template>
  <div class="flex h-screen flex-col">
    <TopBar @back="goBack" />
    <div class="flex min-h-0 flex-1">
      <LeftSidebar v-model:tab="leftTab" class="w-80 shrink-0" />
      <div class="relative min-w-0 flex-1">
        <CanvasStage class="absolute inset-0" />
      </div>
      <RightSidebar v-model:tab="rightTab" class="w-80 shrink-0" />
    </div>
  </div>
</template>

