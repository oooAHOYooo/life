import { defineStore } from "pinia";
import type Konva from "konva";

export const useRuntimeStore = defineStore("runtime", {
  state: () => ({
    stage: null as Konva.Stage | null,
  }),
  actions: {
    setStage(stage: Konva.Stage | null) {
      this.stage = stage;
    },
  },
});

