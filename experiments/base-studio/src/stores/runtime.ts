import { defineStore } from "pinia";
import { markRaw, shallowRef } from "vue";
import type { Stage } from "konva/lib/Stage";

// Setup-style store so we can keep Konva instances in a shallowRef
// (avoids Vue/Pinia unwrapping making Stage not assignable to Stage).
export const useRuntimeStore = defineStore("runtime", () => {
  const stage = shallowRef<Stage | null>(null);

  function setStage(s: Stage | null) {
    stage.value = s ? markRaw(s) : null;
  }

  return { stage, setStage };
});

