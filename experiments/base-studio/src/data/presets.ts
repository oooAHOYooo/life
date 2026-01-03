import type { BrushPreset } from "@/types/scene";

const mk = (p: Omit<BrushPreset, "id"> & { id?: string }): BrushPreset => ({
  id: p.id ?? crypto.randomUUID(),
  ...p,
});

export const PRESET_CATEGORIES: BrushPreset["category"][] = [
  "Oils",
  "Pencil",
  "Ink",
  "Pastel",
  "Marker",
  "Custom",
];

export const DEFAULT_PRESETS: BrushPreset[] = [
  // Oils
  mk({ category: "Oils", name: "Oil Thick", color: "#f97316", width: 18, opacity: 0.8, jitter: 0.2 }),
  mk({ category: "Oils", name: "Oil Smooth", color: "#f59e0b", width: 12, opacity: 0.75, jitter: 0.08 }),
  mk({ category: "Oils", name: "Oil Glaze", color: "#fb7185", width: 20, opacity: 0.25, jitter: 0.05 }),

  // Pencil
  mk({ category: "Pencil", name: "HB Sketch", color: "#e5e7eb", width: 3, opacity: 0.45, jitter: 0.35 }),
  mk({ category: "Pencil", name: "2B Soft", color: "#f4f4f5", width: 5, opacity: 0.4, jitter: 0.45 }),
  mk({ category: "Pencil", name: "Draft Light", color: "#d4d4d8", width: 2, opacity: 0.3, jitter: 0.2 }),

  // Ink
  mk({ category: "Ink", name: "Ink Pen", color: "#111827", width: 4, opacity: 0.9, jitter: 0.05 }),
  mk({ category: "Ink", name: "Brush Ink", color: "#0f172a", width: 10, opacity: 0.85, jitter: 0.18 }),

  // Pastel
  mk({ category: "Pastel", name: "Soft Pastel", color: "#a78bfa", width: 22, opacity: 0.35, jitter: 0.35 }),
  mk({ category: "Pastel", name: "Chalk Dust", color: "#60a5fa", width: 26, opacity: 0.25, jitter: 0.55 }),

  // Marker
  mk({ category: "Marker", name: "Marker Bold", color: "#22c55e", width: 16, opacity: 0.6, jitter: 0.06 }),
  mk({ category: "Marker", name: "Marker Fine", color: "#06b6d4", width: 8, opacity: 0.55, jitter: 0.04 })
];

