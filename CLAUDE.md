# ESP32 Temperature System - Claude Code Instructions

## Project Overview

This is a complete ESP32-based temperature monitoring system with three main components:

1. **Firmware** (`/firmware`) - ESP32-C3 embedded code for temperature monitoring
2. **Frontend** (`/frontend`) - Web-based tool for flashing firmware, configuring devices, and serial monitoring
3. **Backend** (`/backend`) - Docker Compose setup for InfluxDB v2 and Grafana (local development/testing)

These components work together to create an end-to-end IoT temperature monitoring solution.

## System Architecture

```
┌─────────────────┐
│   ESP32 Device  │  ← Runs firmware, reads sensors
│   (Firmware)    │
└────────┬────────┘
         │
         │ WiFi: Sends sensor data every 60s
         ↓
┌─────────────────┐
│    InfluxDB     │  ← Stores time-series sensor data
│   (Backend)     │
└────────┬────────┘
         │
         │ Queries data
         ↓
┌─────────────────┐
│     Grafana     │  ← Visualizes data in dashboards
│   (Backend)     │
└─────────────────┘

┌─────────────────┐
│  Web Interface  │  ← Flashes firmware, configures devices,
│   (Frontend)    │     monitors serial output
└────────┬────────┘
         │
         │ USB/Serial: Flash & configure
         ↓
┌─────────────────┐
│   ESP32 Device  │
└─────────────────┘
```

### Data Flow
1. **Setup**: User flashes firmware to ESP32 via web interface (USB connection)
2. **Configuration**: Device receives WiFi and InfluxDB credentials during flash
3. **Operation**: ESP32 reads DHT22 sensor every 60 seconds
4. **Transmission**: Device sends temperature/humidity data to InfluxDB over WiFi
5. **Visualization**: Grafana queries InfluxDB and displays real-time dashboards
6. **Monitoring**: User can view serial output via web interface for troubleshooting

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
The firmware receives configuration via serial during the flashing process from the web interface:
- WiFi SSID and password
- InfluxDB URL, token, org, and bucket
- Location identifier
- DHT sensor pin (hardcoded in firmware)

### Important Notes
- Reads sensor data every 60 seconds
- Includes WiFi reconnection logic
- Reports device health metrics (wifi_rssi, wifi_channel, wifi_reconnect_count, uptime_s, heap_free, heap_min_free, heap_fragmentation)
- Uses static tags for device identification

---

## 2. Frontend Codebase (`/frontend`)

### Purpose
Web-based single-page application that provides:
- **Firmware Flashing**: Upload firmware to ESP32 devices via WebSerial API
- **Device Configuration**: Configure WiFi and InfluxDB settings during flash
- **Serial Monitor**: View real-time serial output from connected devices
- **Version Management**: Select firmware version from manifest

### Technology Stack
- **HTML/JavaScript** - Single-page application with tab-based UI
- **esptool-js** - JavaScript port of esptool for flashing ESP32 devices
- **WebSerial API** - Browser-based serial communication (Chrome/Edge only)

### Key Files
- `index.html` - Main application with Flash Firmware and Serial Monitor tabs
- `common.css` - Shared styles for light/dark themes
- `manifest.json` - Firmware version metadata and binary paths
- `*.bin` files - Precompiled firmware binaries copied from firmware build
- `Makefile` - Automation for copying binaries from firmware build

### Features

#### Flash Firmware Tab
- Select firmware version from dropdown (populated from `manifest.json`)
- Optional device configuration (WiFi credentials, InfluxDB settings, location)
- Can update firmware without reconfiguring (preserves existing settings)
- Real-time flashing progress with log output

#### Serial Monitor Tab
- Connect to ESP32 and view serial output
- Send commands to device
- Device restart/reset control
- Configurable line endings (LF, CR+LF, CR, None)

### Workflow
1. User opens `index.html` in Chrome/Edge (WebSerial support required)
2. Connects ESP32 device via USB
3. Selects firmware version and enters configuration
4. Flashes firmware and optionally configures device
5. Can switch to Serial Monitor tab to view device output (no page reload)

### Build Process
After building firmware, copy binaries to frontend:
```bash
cd frontend
make
```

This copies the three required binary files from the firmware build directory.

### Important Notes
- All tabs operate without page reloads - input fields are preserved when switching tabs
- Firmware versions are defined in `manifest.json` with paths to binary files
- Configuration is sent to device via serial after flashing completes
- Serial monitor shares USB connection context across tabs

---

## 3. Backend Infrastructure (`/backend`)

### Purpose
Docker Compose setup for **local development and testing** that provides:
- **InfluxDB v2**: Time-series database for storing sensor data
- **Grafana**: Visualization and dashboarding for sensor metrics

**Note**: This is designed for quick setup and local testing, not production deployment.

### Technology Stack
- **Docker & Docker Compose**: Container orchestration
- **InfluxDB 2.7**: Time-series database with automatic initialization
- **Grafana 10.4.1**: Visualization platform with pre-configured datasource

### Key Files
- `docker-compose.yaml` - Service definitions with hardcoded dev credentials
- `.env.example` - Example environment file (not used by default setup)
- `.gitignore` - Protects `.env` from being committed
- `grafana/provisioning/datasources/influxdb.yaml` - Auto-configures InfluxDB connection
- `README.md` - Setup instructions and configuration details

### Default Configuration
The Docker setup includes these **hardcoded defaults** for local testing:
- **InfluxDB**:
  - Organization: `home`
  - Bucket: `sensors`
  - Admin username: `admin`
  - Admin password: `password123`
  - Admin token: `my-dev-token`
  - Port: `8086`
- **Grafana**:
  - Admin username: `admin`
  - Admin password: `admin`
  - Port: `3000`
  - InfluxDB datasource: pre-configured automatically

### Quick Start
```bash
cd backend
docker-compose up -d
```

Access services:
- InfluxDB UI: http://localhost:8086
- Grafana: http://localhost:3000

### Data Persistence
Docker volumes are used for data persistence:
- `influxdb-data`: Time-series data
- `grafana-data`: Dashboards and settings

Data persists across container restarts but can be removed with `docker-compose down -v`.

### Important Notes
- **Not for production**: Uses simple credentials and no SSL/TLS
- Grafana automatically connects to InfluxDB on startup
- ESP32 devices send data directly to InfluxDB (port 8086)
- For production, implement proper security hardening, backups, and monitoring

---

## Working with This Project

### When Modifying Firmware
1. Edit code in `/firmware/src/`
2. Build with PlatformIO: `pio run`
3. Test on device: `pio run --target upload`
4. Copy binaries to frontend: `cd ../frontend && make`
5. Update `manifest.json` version if releasing

### When Modifying Frontend
1. Edit `/frontend/index.html`, `common.css`, or configuration files
2. Update `manifest.json` for new releases (versions, release notes, binary paths)
3. Test in Chrome/Edge with an ESP32 device connected
4. Verify both tabs work correctly (Flash Firmware and Serial Monitor)
5. Ensure input fields persist when switching tabs

### When Working with Backend
1. Start services: `cd backend && docker-compose up -d`
2. View logs: `docker-compose logs -f`
3. Stop services: `docker-compose down`
4. Reset all data: `docker-compose down -v` (removes volumes)
5. Access InfluxDB at http://localhost:8086, Grafana at http://localhost:3000

### Dependencies Between Components
- **Frontend ← Firmware**: Frontend requires binary artifacts from firmware builds
- **Frontend**: `manifest.json` must reference correct firmware versions and binary paths
- **Firmware → Backend**: Firmware sends sensor data to InfluxDB
- **Backend**: Provides infrastructure for data storage (InfluxDB) and visualization (Grafana)
- Components are developed independently but work as an integrated system

### Testing
- **Firmware**: Connect ESP32 via USB, monitor serial output at 115200 baud
- **Frontend**: Open `index.html` in Chrome/Edge, test with actual ESP32 device
  - Test both flashing and serial monitor tabs
  - Verify configuration fields persist when switching tabs
- **Backend**:
  - Verify InfluxDB is receiving data: Check UI at http://localhost:8086
  - Create Grafana dashboards: http://localhost:3000
  - Query data using Flux queries in InfluxDB Data Explorer

---

## Development Guidelines

### Code Style
- Firmware follows Arduino C++ conventions
- Keep configuration values at the top of files as `#define` constants
- Use meaningful variable names and comments for hardware-specific code

### Security Considerations
- **Never commit secrets**: WiFi credentials, InfluxDB tokens should never be in version control
- **Firmware**: Configuration is provided at flash time via web interface, not stored in code
- **Frontend**: Runs entirely client-side, no server-side secrets
  - Configuration data is sent directly to device via serial connection
  - No credentials are stored in browser or transmitted over network
- **Backend**: Default Docker setup uses hardcoded credentials for **local testing only**
  - Change all passwords/tokens before any production deployment
  - Implement SSL/TLS for remote access
  - Use proper authentication and network segmentation
  - Regular backups and security updates

### Version Management
- Firmware version is tracked in `frontend/manifest.json`
- Update version number and release notes when releasing new firmware
- Binary files in frontend should match the version in manifest

---

## Common Tasks

### Set up a complete local environment
1. Start backend services: `cd backend && docker-compose up -d`
2. Build firmware: `cd firmware && pio run`
3. Copy binaries to frontend: `cd frontend && make`
4. Open `frontend/index.html` in Chrome/Edge
5. Configure and flash ESP32 device using web interface
6. Monitor data in InfluxDB UI and create Grafana dashboards

### Add a new sensor reading to firmware
1. Read sensor value in `loop()`
2. Add field to `sensor.addField()` calls
3. Rebuild and test
4. Update InfluxDB queries/dashboards if needed

### Release new firmware version
1. Build firmware with production settings: `cd firmware && pio run`
2. Copy binaries to frontend: `cd frontend && make`
3. Update `frontend/manifest.json`:
   - Add new release entry with version, notes, chipFamily
   - Specify binary paths and offsets in `parts` array
4. Test flashing with web interface
5. Commit firmware code, binaries, and updated manifest
6. Tag release in git

### Configure a new ESP32 device
1. Open `frontend/index.html` in Chrome/Edge
2. Select desired firmware version from dropdown
3. Check "Configure device settings"
4. Enter WiFi credentials and InfluxDB settings (from backend)
5. Connect ESP32 via USB and click "Connect & Flash Firmware"
6. Switch to Serial Monitor tab to verify device is working
7. Check InfluxDB UI to confirm data is being received

### Update firmware without changing configuration
1. Open `frontend/index.html`
2. Select new firmware version
3. **Uncheck** "Configure device settings"
4. Connect and flash - existing configuration will be preserved

### Troubleshoot a device
1. Open `frontend/index.html` and switch to Serial Monitor tab
2. Connect to ESP32 device
3. View real-time serial output
4. Send commands if needed
5. Use "Restart Device" button to reboot without disconnecting
