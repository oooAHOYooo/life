import type Konva from "konva";
import { inject, provide, shallowRef, type ShallowRef } from "vue";

const StageKey: unique symbol = Symbol("BASE_STAGE");

export function provideStageBus() {
  const stage = shallowRef<Konva.Stage | null>(null);
  provide(StageKey, stage);
  return stage;
}

export function useStageBus(): ShallowRef<Konva.Stage | null> {
  const v = inject<ShallowRef<Konva.Stage | null>>(StageKey);
  if (!v) throw new Error("Stage bus not provided");
  return v;
}

