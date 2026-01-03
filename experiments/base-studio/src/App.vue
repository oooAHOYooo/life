<script setup lang="ts">
import { computed, onMounted, watch } from "vue";
import { useProjectsStore } from "@/stores/projects";
import { useEditorStore } from "@/stores/editor";
import ProjectsView from "@/views/ProjectsView.vue";
import EditorView from "@/views/EditorView.vue";

const projects = useProjectsStore();
const editor = useEditorStore();

onMounted(async () => {
  await projects.boot();
});

const route = computed(() => projects.route);

watch(
  () => projects.route,
  async (r) => {
    if (r.name !== "editor") return;
    const meta = projects.projects.find((p) => p.id === r.projectId);
    const data = await projects.loadProjectData(r.projectId);
    const scene = (data?.scene ??
      ({
        version: 1,
        canvas: { width: 1200, height: 800, background: "#0b0b0c" },
        nodes: [],
      } as any)) as any;
    editor.hydrate(r.projectId, meta?.name ?? "Untitled Project", scene);
  },
  { immediate: true }
);
</script>

<template>
  <div class="min-h-screen">
    <ProjectsView v-if="route.name === 'projects'" />
    <EditorView v-else />
  </div>
</template>

