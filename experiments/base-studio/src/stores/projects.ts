import { defineStore } from "pinia";
import tokens from "@/tokens/tokens.json";
import type { Scene } from "@/types/scene";
import { uid } from "@/utils/ids";
import {
  dbDeleteProject,
  dbGetProjectData,
  dbListProjects,
  dbPutProjectData,
  dbPutProjectMeta,
  type ProjectMeta,
} from "@/persistence/db";

const DEFAULT_SCENE: Scene = {
  version: 1,
  canvas: {
    width: 1200,
    height: 800,
    background: "#0b0b0c",
  },
  nodes: [
    {
      id: "frame_1",
      type: "frame",
      name: "Frame",
      x: 120,
      y: 90,
      rotation: 0,
      scaleX: 1,
      scaleY: 1,
      width: 960,
      height: 620,
      opacity: 1,
      visible: true,
      locked: false,
      parentId: null,
      fill: "#0f0f12",
      stroke: "#27272a",
      strokeWidth: 2,
      radius: 16
    }
  ],
};

export type AppRoute =
  | { name: "projects" }
  | { name: "editor"; projectId: string };

export const useProjectsStore = defineStore("projects", {
  state: () => ({
    route: { name: "projects" } as AppRoute,
    projects: [] as ProjectMeta[],
    isLoading: false,
    booted: false,
  }),
  actions: {
    async boot() {
      if (this.booted) return;
      this.isLoading = true;
      try {
        this.projects = await dbListProjects();
      } finally {
        this.isLoading = false;
        this.booted = true;
      }
    },

    async createProject(name?: string) {
      const id = uid("proj");
      const now = Date.now();
      const meta: ProjectMeta = {
        id,
        name: name?.trim() || "Untitled Project",
        createdAt: now,
        updatedAt: now,
      };
      await dbPutProjectMeta(meta);
      await dbPutProjectData({ id, scene: DEFAULT_SCENE, tokens });
      this.projects = await dbListProjects();
      this.route = { name: "editor", projectId: id };
      return id;
    },

    async deleteProject(id: string) {
      await dbDeleteProject(id);
      this.projects = await dbListProjects();
      if (this.route.name === "editor" && this.route.projectId === id) {
        this.route = { name: "projects" };
      }
    },

    async openProject(id: string) {
      this.route = { name: "editor", projectId: id };
    },

    goProjects() {
      this.route = { name: "projects" };
    },

    async renameProject(id: string, name: string) {
      const p = this.projects.find((x) => x.id === id);
      if (!p) return;
      const next: ProjectMeta = { ...p, name: name.trim() || "Untitled Project", updatedAt: Date.now() };
      await dbPutProjectMeta(next);
      this.projects = await dbListProjects();
    },

    async updateThumbnail(id: string, thumbnail: string) {
      const p = this.projects.find((x) => x.id === id);
      if (!p) return;
      const next: ProjectMeta = { ...p, thumbnail, updatedAt: Date.now() };
      await dbPutProjectMeta(next);
      this.projects = await dbListProjects();
    },

    async loadProjectData(projectId: string) {
      return await dbGetProjectData(projectId);
    },

    async saveProjectData(projectId: string, scene: unknown, tokensPayload: unknown) {
      const p = this.projects.find((x) => x.id === projectId);
      const now = Date.now();
      if (p) {
        await dbPutProjectMeta({ ...p, updatedAt: now });
      } else {
        await dbPutProjectMeta({
          id: projectId,
          name: "Untitled Project",
          createdAt: now,
          updatedAt: now,
        });
      }
      await dbPutProjectData({ id: projectId, scene, tokens: tokensPayload });
      this.projects = await dbListProjects();
    },
  },
});

