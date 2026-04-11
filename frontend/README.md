# Frontend

Web-based tool for flashing firmware, configuring devices, and monitoring serial output from ESP32 devices.

## Running Locally

Start the dev server from the repo root (WebSerial requires Chrome or Edge — Firefox is not supported):

```bash
make frontend-start
```

Then open http://localhost:8080/frontend in Chrome or Edge.

## Making Changes

- **UI / layout**: Edit `index.html` (HTML structure at the top, CSS in `<style>`, JavaScript in the `<script type="module">` block at the bottom)
- **Styles**: Edit `common.css` for shared styles; inline `<style>` in `index.html` for tab-specific styles
- **Firmware versions**: Edit `manifest.json` (see [Adding a New Release](#adding-a-new-release) below)
- **No build step is required** — save and reload the browser

## Firmware Releases

Firmware binaries are stored in `releases/<version>/` and are gitignored. They must be downloaded before use.

### Directory Structure

```
releases/
  0.0.1/
    esp32-c3-bootloader.bin
    esp32-c3-partitions.bin
    esp32-c3-firmware.bin
  0.1.0/
    ...
```

### Local Development

**Working on an existing release** — download all binaries referenced in `manifest.json`:

```bash
cd frontend
./download-releases.sh
```

**Working on a feature branch** — build firmware from your branch and use the workflow artifacts:

1. Push your changes to GitHub
2. Go to the [build-firmware workflow](https://github.com/jveldboom/esp32-temperature-system/actions/workflows/build-firmware.yml) and run it against your branch
3. Once the run completes, download the `firmware-binaries` artifact from the workflow run summary page
4. Extract the zip and move the `.bin` files into `releases/<version>/`
5. Add a new entry to `manifest.json` pointing to the new files (see [Adding a New Release](#adding-a-new-release) below)

Then open `index.html` in Chrome or Edge (WebSerial API required).

### Adding a New Release

1. Build firmware and publish a GitHub release with the binary assets
2. Add a new entry to `manifest.json`:
3. Run `./download-releases.sh` to fetch the new binaries locally

### GitHub Pages Deployment

The `deploy-pages` workflow automatically runs `download-releases.sh` on every push to `main` that modifies the `frontend/` directory. Binaries are fetched from GitHub releases at deploy time and are never committed to the repository.
