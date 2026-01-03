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

### Netlify: same-site deploy (serve at `/experiments/base-studio/`)
If your main site is deployed from the repo root, you can keep it as one Netlify site by building BASE Studio during the Netlify build and copying the built output into the publish folder.

Recommended Netlify settings:
- **Base directory**: _(blank)_
- **Build command**:

```bash
npm --prefix experiments/base-studio ci --cache ./experiments/base-studio/_npm-cache
npm --prefix experiments/base-studio run build
rm -rf _site
mkdir -p _site/experiments/base-studio
cp -R index.html styles.css design-system.css subpages data archive _site/
cp -R experiments/base-studio/dist/* _site/experiments/base-studio/
```

- **Publish directory**: `_site`

This avoids publishing any `node_modules` while still serving the built app at `/experiments/base-studio/`.

