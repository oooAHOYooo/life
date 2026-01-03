import JSZip from "jszip";
import Konva from "konva";
import type { Scene } from "@/types/scene";
import { downloadBlob, downloadDataUrl } from "@/utils/download";

function toBlob(dataUrl: string): Blob {
  const [head, b64] = dataUrl.split(",");
  const mime = (head?.match(/data:(.*?);base64/)?.[1] ?? "application/octet-stream") as string;
  const bin = atob(b64 ?? "");
  const u8 = new Uint8Array(bin.length);
  for (let i = 0; i < bin.length; i++) u8[i] = bin.charCodeAt(i);
  return new Blob([u8], { type: mime });
}

function safeName(name: string) {
  return name.replace(/[^a-z0-9\-_]+/gi, "-").replace(/-+/g, "-").replace(/^-|-$/g, "").toLowerCase() || "project";
}

function getLayerByName(stage: Konva.Stage, name: string) {
  return stage.findOne(`.${name}`) as Konva.Layer | null;
}

export function renderCleanPngDataUrl(stage: Konva.Stage) {
  const ui = getLayerByName(stage, "ui");
  const annot = getLayerByName(stage, "annot");
  const prevUiVis = ui?.visible();
  const prevAnnotVis = annot?.visible();
  if (ui) ui.visible(false);
  if (annot) annot.visible(false);
  stage.draw();
  const url = stage.toDataURL({ pixelRatio: 2 });
  if (ui && prevUiVis != null) ui.visible(prevUiVis);
  if (annot && prevAnnotVis != null) annot.visible(prevAnnotVis);
  stage.draw();
  return url;
}

export function renderAnnotatedPngDataUrl(stage: Konva.Stage) {
  const ui = getLayerByName(stage, "ui");
  const prevUiVis = ui?.visible();
  if (ui) ui.visible(false);
  stage.draw();
  const url = stage.toDataURL({ pixelRatio: 2 });
  if (ui && prevUiVis != null) ui.visible(prevUiVis);
  stage.draw();
  return url;
}

export function buildAnnotationLayer(stage: Konva.Stage, labels: Array<{ id: string; name: string; type: string; x: number; y: number; width: number; height: number }>) {
  const existing = getLayerByName(stage, "annot");
  existing?.destroy();

  const layer = new Konva.Layer();
  layer.name("annot");

  for (const l of labels) {
    const rect = new Konva.Rect({
      x: l.x,
      y: l.y,
      width: Math.max(1, l.width),
      height: Math.max(1, l.height),
      stroke: "#22c55e",
      strokeWidth: 2,
      dash: [6, 4],
      listening: false,
    });
    const text = new Konva.Text({
      x: l.x,
      y: Math.max(0, l.y - 18),
      text: `${l.name} • ${l.type} • ${l.id}`,
      fontSize: 12,
      fontFamily: "Inter, ui-sans-serif, system-ui",
      fill: "#22c55e",
      listening: false,
    });
    const bg = new Konva.Rect({
      x: text.x() - 4,
      y: text.y() - 2,
      width: text.width() + 8,
      height: text.height() + 4,
      fill: "rgba(0,0,0,0.55)",
      cornerRadius: 6,
      listening: false,
    });
    layer.add(rect);
    layer.add(bg);
    layer.add(text);
  }

  stage.add(layer);
  layer.moveToTop();
  stage.draw();
  return layer;
}

export function exportSceneJson(scene: Scene, filenameBase: string) {
  const json = JSON.stringify(scene, null, 2);
  const blob = new Blob([json], { type: "application/json" });
  downloadBlob(`${filenameBase}_scene.json`, blob);
  return json;
}

export function makePromptMd(opts: {
  projectName: string;
  scene: Scene;
  tokensJson: unknown;
}) {
  const lines: string[] = [];
  lines.push(`# BASE Studio AI Bundle`);
  lines.push(``);
  lines.push(`Project: **${opts.projectName}**`);
  lines.push(``);
  lines.push(`## Files`);
  lines.push(`- design.png: clean render`);
  lines.push(`- design_annotated.png: bounding boxes + labels`);
  lines.push(`- scene.json: editable scene graph (layers/groups/props)`);
  lines.push(`- tokens.json: design tokens used for quick styling`);
  lines.push(``);
  lines.push(`## Layer inventory`);
  for (const n of opts.scene.nodes) {
    lines.push(`- ${n.id} • ${n.type} • ${n.name}`);
  }
  lines.push(``);
  lines.push(`## Instructions (template)`);
  lines.push(`You are given a UI design PNG and an annotated version with bounding boxes + IDs.`);
  lines.push(`Recreate the screen as a web UI, preserving layout, typography, spacing, and colors.`);
  lines.push(`Use tokens.json as the source of truth for palette/sizing where possible.`);
  lines.push(``);
  lines.push(`### Output requirements`);
  lines.push(`- Provide semantic HTML structure`);
  lines.push(`- Provide CSS (or Tailwind classes) matching the design`);
  lines.push(`- Map each annotated layer ID to a corresponding element`);
  lines.push(``);
  return lines.join("\n");
}

export async function exportAiBundleZip(opts: {
  stage: Konva.Stage;
  filenameBase: string;
  projectName: string;
  scene: Scene;
  tokensJson: unknown;
}) {
  const base = safeName(opts.filenameBase);
  const designUrl = renderCleanPngDataUrl(opts.stage);
  const annotatedUrl = renderAnnotatedPngDataUrl(opts.stage);
  const sceneJson = JSON.stringify(opts.scene, null, 2);
  const tokensJson = JSON.stringify(opts.tokensJson, null, 2);
  const promptMd = makePromptMd({ projectName: opts.projectName, scene: opts.scene, tokensJson: opts.tokensJson });

  const zip = new JSZip();
  zip.file("design.png", toBlob(designUrl));
  zip.file("design_annotated.png", toBlob(annotatedUrl));
  zip.file("scene.json", sceneJson);
  zip.file("tokens.json", tokensJson);
  zip.file("prompt.md", promptMd);

  const blob = await zip.generateAsync({ type: "blob" });
  downloadBlob(`${base}_ai-bundle.zip`, blob);
}

export function downloadCleanPng(stage: Konva.Stage, filenameBase: string) {
  const url = renderCleanPngDataUrl(stage);
  downloadDataUrl(`${filenameBase}.png`, url);
}

export function downloadAnnotatedPng(stage: Konva.Stage, filenameBase: string) {
  const url = renderAnnotatedPngDataUrl(stage);
  downloadDataUrl(`${filenameBase}_annotated.png`, url);
}

