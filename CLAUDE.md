# ESP32 Temperature System - Claude Code Instructions

## Project Overview

This project contains two distinct codebases that work together:

1. **Firmware** (`/firmware`) - ESP32-C3 embedded code for temperature monitoring
2. **Frontend** (`/frontend`) - Web-based tool for flashing firmware to ESP32 devices

While they work together, they are developed and built independently.

---

## 1. Firmware Codebase (`/firmware`)

### Purpose
Embedded C++ code that runs on ESP32-C3 microcontrollers to monitor temperature and humidity using a DHT22 sensor and send data to InfluxDB.

### Technology Stack
- **Platform**: ESP32-C3 (RISC-V architecture)
- **Build System**: PlatformIO
- **Framework**: Arduino
- **Key Libraries**:
  - DHT sensor library (temperature/humidity)
  - InfluxDB client (time-series database)
  - WiFi (ESP32 built-in)

### Key Files
- `platformio.ini` - Build configuration and dependencies
- `src/main.cpp` - Main firmware code
- Build artifacts: `bootloader.bin`, `partitions.bin`, `firmware.bin`

### Build Process
Use PlatformIO CLI or IDE to build:
```bash
cd firmware
pio run
```

### Configuration
The firmware expects these values (normally in `secrets.ini`):
- WiFi SSID and password
- InfluxDB URL, token, org, and bucket
- Location identifier
- DHT sensor pin

### Important Notes
- Reads sensor data every 60 seconds
- Includes WiFi reconnection logic
- Reports device health metrics (RSSI, uptime, free heap)
- Uses static tags for device identification

---

## 2. Frontend Codebase (`/frontend`)

### Purpose
Web-based ESP32 flasher that allows users to upload firmware to their ESP32 devices directly from their browser using WebSerial API.

### Technology Stack
- **HTML/JavaScript** - Single-page application
- **esptool-js** - JavaScript port of esptool for flashing ESP32 devices
- **WebSerial API** - Browser-based serial communication

### Key Files
- `esp32-flasher.html` - Main UI for flashing
- `esptool-js.bundle.js` - Flashing library
- `manifest.json` - Firmware version metadata
- `*.bin` files - Precompiled firmware binaries copied from firmware build
- `Makefile` - Automation for copying binaries from firmware build

### Workflow
1. User opens `esp32-flasher.html` in Chrome/Edge (WebSerial support required)
2. Connects ESP32 device via USB
3. Web app flashes firmware using the bundled `.bin` files

### Build Process
After building firmware, copy binaries to frontend:
```bash
cd frontend
make
```

This copies the three required binary files from the firmware build directory.

---

## Working with This Project

### When Modifying Firmware
1. Edit code in `/firmware/src/`
2. Build with PlatformIO: `pio run`
3. Test on device: `pio run --target upload`
4. Copy binaries to frontend: `cd ../frontend && make`
5. Update `manifest.json` version if releasing

### When Modifying Frontend
1. Edit `/frontend/esp32-flasher.html` or configuration files
2. Update `manifest.json` for new releases
3. Test in browser with an ESP32 device connected

### Dependencies Between Codebases
- Frontend requires binary artifacts from firmware builds
- Frontend's `manifest.json` should reference current firmware version
- They are separate but the frontend distributes the firmware

### Testing
- **Firmware**: Connect ESP32 via USB, monitor serial output at 115200 baud
- **Frontend**: Open HTML file in Chrome/Edge, test with actual ESP32 device

---

## Development Guidelines

### Code Style
- Firmware follows Arduino C++ conventions
- Keep configuration values at the top of files as `#define` constants
- Use meaningful variable names and comments for hardware-specific code

### Security Considerations
- Never commit real WiFi credentials or InfluxDB tokens to version control
- Use `secrets.ini` (gitignored) for sensitive values
- Frontend runs client-side only, no server-side secrets

### Version Management
- Firmware version is tracked in `frontend/manifest.json`
- Update version number and release notes when releasing new firmware
- Binary files in frontend should match the version in manifest

---

## Common Tasks

### Add a new sensor reading to firmware
1. Read sensor value in `loop()`
2. Add field to `sensor.addField()` calls
3. Rebuild and test
4. Update InfluxDB queries/dashboards if needed

### Release new firmware version
1. Build firmware with production settings
2. Copy binaries to frontend: `cd frontend && make`
3. Update `manifest.json` with new version and release notes
4. Commit both firmware code and frontend binaries
5. Tag release in git

### Configure for a new deployment
1. Create/update `firmware/secrets.ini` with WiFi and InfluxDB settings
2. Modify `LOCATION` constant in `main.cpp` for the deployment site
3. Build and flash to device
