export type NodeType = "frame" | "rect" | "text" | "stroke" | "image" | "group";

export type BaseNode = {
  id: string;
  type: NodeType;
  name: string;
  x: number;
  y: number;
  rotation: number;
  scaleX: number;
  scaleY: number;
  width?: number;
  height?: number;
  opacity: number;
  visible: boolean;
  locked: boolean;
  parentId: string | null;
};

export type RectNode = BaseNode & {
  type: "rect" | "frame";
  width: number;
  height: number;
  fill: string;
  stroke: string;
  strokeWidth: number;
  radius: number;
};

export type TextNode = BaseNode & {
  type: "text";
  text: string;
  fontSize: number;
  fontFamily: string;
  fill: string;
  align: "left" | "center" | "right";
  width: number;
};

export type StrokeNode = BaseNode & {
  type: "stroke";
  points: number[];
  stroke: string;
  strokeWidth: number;
  tension: number;
  lineCap: "round" | "butt" | "square";
  lineJoin: "round" | "bevel" | "miter";
};

export type ImageNode = BaseNode & {
  type: "image";
  src: string; // data URL
  width: number;
  height: number;
};

export type GroupNode = BaseNode & {
  type: "group";
  // group bounds are computed; width/height optional
  collapsed: boolean;
};

export type AnyNode = RectNode | TextNode | StrokeNode | ImageNode | GroupNode;

export type Scene = {
  version: 1;
  canvas: {
    width: number;
    height: number;
    background: string;
  };
  nodes: AnyNode[];
};

export type BrushPreset = {
  id: string;
  category: "Oils" | "Pencil" | "Ink" | "Pastel" | "Marker" | "Custom";
  name: string;
  color: string;
  width: number;
  opacity: number;
  jitter: number; // 0..1 (simulated roughness)
};

