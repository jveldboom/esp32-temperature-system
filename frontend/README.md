# Frontend

Web-based tool for flashing firmware, configuring devices, and monitoring serial output from ESP32 devices.

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

```json
{
  "version": "0.1.0",
  "notes": "Description of changes",
  "chipFamily": "ESP32-C3",
  "parts": [
    { "path": "releases/0.1.0/bootloader.bin", "offset": 0 },
    { "path": "releases/0.1.0/partitions.bin", "offset": 32768 },
    { "path": "releases/0.1.0/firmware.bin", "offset": 65536 }
  ]
}
```

3. Run `./download-releases.sh` to fetch the new binaries locally

### GitHub Pages Deployment

The `deploy-pages` workflow automatically runs `download-releases.sh` on every push to `main` that modifies the `frontend/` directory. Binaries are fetched from GitHub releases at deploy time and are never committed to the repository.
