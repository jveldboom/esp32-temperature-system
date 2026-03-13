# Firmware Releases

This directory stores firmware binaries for local testing and GitHub Pages deployment.

## Structure

```
releases/
  0.0.1/
    esp32-c3-bootloader.bin
    esp32-c3-partitions.bin
    esp32-c3-firmware.bin
  0.0.2/
    ...
```

## Local Development

To test locally, download binaries from GitHub releases:

```bash
cd frontend/releases
mkdir -p 0.0.1
cd 0.0.1
curl -L -O https://github.com/jveldboom/esp32-temperature-system/releases/download/0.0.1/esp32-c3-bootloader.bin
curl -L -O https://github.com/jveldboom/esp32-temperature-system/releases/download/0.0.1/esp32-c3-partitions.bin
curl -L -O https://github.com/jveldboom/esp32-temperature-system/releases/download/0.0.1/esp32-c3-firmware.bin
```

## GitHub Pages Deployment

The GitHub Actions workflow automatically downloads binaries from releases during deployment, so this directory remains empty in the repository.

## Note

Binary files (*.bin) are gitignored and should never be committed to the repository.
