<script setup lang="ts">
import { computed, ref } from "vue";
import { useProjectsStore } from "@/stores/projects";

const projects = useProjectsStore();
const creating = ref(false);
const newName = ref("");

const list = computed(() => projects.projects);

async function create() {
  creating.value = true;
  try {
    await projects.createProject(newName.value || undefined);
    newName.value = "";
  } finally {
    creating.value = false;
  }
}

async function del(id: string) {
  const ok = confirm("Delete this project? This cannot be undone.");
  if (!ok) return;
  await projects.deleteProject(id);
}
</script>

<template>
  <div class="mx-auto max-w-6xl px-6 py-10">
    <div class="flex items-start justify-between gap-6">
      <div>
        <div class="text-2xl font-semibold tracking-tight">BASE Studio</div>
        <div class="mt-1 text-sm text-zinc-400">Experimental local-first editor</div>
      </div>
      <div class="flex items-center gap-2">
        <input
          v-model="newName"
          class="h-10 w-64 rounded-lg border border-zinc-800 bg-zinc-900 px-3 text-sm text-zinc-100 placeholder:text-zinc-500"
          placeholder="New project name"
        />
        <button
          class="h-10 rounded-lg bg-indigo-500 px-4 text-sm font-medium text-white hover:bg-indigo-400 disabled:opacity-60"
          :disabled="creating"
          @click="create"
        >
          New
        </button>
      </div>
    </div>

    <div class="mt-8 grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3">
      <button
        v-for="p in list"
        :key="p.id"
        class="group flex flex-col overflow-hidden rounded-xl border border-zinc-800 bg-zinc-900 text-left hover:border-zinc-700"
        @click="projects.openProject(p.id)"
      >
        <div class="aspect-[16/10] w-full bg-zinc-950">
          <img v-if="p.thumbnail" :src="p.thumbnail" alt="" class="h-full w-full object-cover" />
          <div v-else class="flex h-full w-full items-center justify-center text-xs text-zinc-600">
            No thumbnail yet
          </div>
        </div>
        <div class="flex items-center justify-between gap-3 p-4">
          <div class="min-w-0">
            <div class="truncate text-sm font-medium text-zinc-100">{{ p.name }}</div>
            <div class="mt-1 text-xs text-zinc-500">
              Updated {{ new Date(p.updatedAt).toLocaleString() }}
            </div>
          </div>
          <button
            class="rounded-md px-2 py-1 text-xs text-zinc-400 hover:bg-zinc-800 hover:text-zinc-200"
            @click.stop="del(p.id)"
          >
            Delete
          </button>
        </div>
      </button>
    </div>

    <div v-if="projects.isLoading" class="mt-8 text-sm text-zinc-400">Loading…</div>
    <div v-if="!projects.isLoading && list.length === 0" class="mt-8 text-sm text-zinc-400">
      No projects yet. Create one to start.
    </div>
  </div>
</template>

