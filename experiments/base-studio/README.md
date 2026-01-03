## BASE Studio (experimental)

Local-first visual editor (Vue 3 + Vite + TypeScript) with a Konva canvas, brush presets, layers, tokens, image import/paste, and AI-focused export.

### Run

```bash
cd experiments/base-studio
npm install
npm run dev
```

If you hit an `EPERM` npm cache error (common when the global npm cache has bad permissions), run:

```bash
npm install --cache ./_npm-cache
```

### Notes
- This sub-app is **self-contained** (its own `package.json` and `node_modules`).
- It **does not** modify the repo root config or lockfiles.

