import { openDB, type DBSchema } from "idb";

type ProjectMeta = {
  id: string;
  name: string;
  updatedAt: number;
  createdAt: number;
  thumbnail?: string; // data URL
};

type ProjectData = {
  id: string;
  scene: unknown;
  tokens: unknown;
};

interface BaseStudioDB extends DBSchema {
  projects: {
    key: string;
    value: ProjectMeta;
    indexes: { "by-updatedAt": number };
  };
  projectData: {
    key: string;
    value: ProjectData;
  };
}

const DB_NAME = "base-studio";
const DB_VERSION = 1;

export async function getDb() {
  return openDB<BaseStudioDB>(DB_NAME, DB_VERSION, {
    upgrade(db) {
      const projects = db.createObjectStore("projects", { keyPath: "id" });
      projects.createIndex("by-updatedAt", "updatedAt");
      db.createObjectStore("projectData", { keyPath: "id" });
    },
  });
}

export async function dbListProjects(): Promise<ProjectMeta[]> {
  try {
    const db = await getDb();
    const all = await db.getAll("projects");
    return all.sort((a, b) => b.updatedAt - a.updatedAt);
  } catch {
    return lsListProjectsFallback();
  }
}

export async function dbPutProjectMeta(meta: ProjectMeta) {
  try {
    const db = await getDb();
    await db.put("projects", meta);
  } catch {
    lsPutProjectMetaFallback(meta);
  }
}

export async function dbDeleteProject(projectId: string) {
  try {
    const db = await getDb();
    await db.delete("projects", projectId);
    await db.delete("projectData", projectId);
  } catch {
    lsDeleteProjectFallback(projectId);
  }
}

export async function dbGetProjectData(projectId: string): Promise<ProjectData | undefined> {
  try {
    const db = await getDb();
    return await db.get("projectData", projectId);
  } catch {
    return lsGetProjectDataFallback(projectId);
  }
}

export async function dbPutProjectData(data: ProjectData) {
  try {
    const db = await getDb();
    await db.put("projectData", data);
  } catch {
    lsPutProjectDataFallback(data);
  }
}

// ---- localStorage fallback ----
const LS_META_KEY = "base-studio:projects";
const LS_DATA_PREFIX = "base-studio:projectData:";

function lsReadMeta(): ProjectMeta[] {
  try {
    const raw = localStorage.getItem(LS_META_KEY);
    if (!raw) return [];
    const parsed = JSON.parse(raw) as ProjectMeta[];
    if (!Array.isArray(parsed)) return [];
    return parsed;
  } catch {
    return [];
  }
}

function lsWriteMeta(all: ProjectMeta[]) {
  try {
    localStorage.setItem(LS_META_KEY, JSON.stringify(all));
  } catch {
    // ignore
  }
}

function lsListProjectsFallback(): ProjectMeta[] {
  return lsReadMeta().sort((a, b) => b.updatedAt - a.updatedAt);
}

function lsPutProjectMetaFallback(meta: ProjectMeta) {
  const all = lsReadMeta();
  const idx = all.findIndex((p) => p.id === meta.id);
  if (idx >= 0) all[idx] = meta;
  else all.push(meta);
  lsWriteMeta(all);
}

function lsDeleteProjectFallback(projectId: string) {
  const all = lsReadMeta().filter((p) => p.id !== projectId);
  lsWriteMeta(all);
  try {
    localStorage.removeItem(`${LS_DATA_PREFIX}${projectId}`);
  } catch {
    // ignore
  }
}

function lsGetProjectDataFallback(projectId: string): ProjectData | undefined {
  try {
    const raw = localStorage.getItem(`${LS_DATA_PREFIX}${projectId}`);
    if (!raw) return undefined;
    return JSON.parse(raw) as ProjectData;
  } catch {
    return undefined;
  }
}

function lsPutProjectDataFallback(data: ProjectData) {
  try {
    localStorage.setItem(`${LS_DATA_PREFIX}${data.id}`, JSON.stringify(data));
  } catch {
    // ignore
  }
}

export type { ProjectMeta, ProjectData };

