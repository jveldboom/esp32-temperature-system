# ESP32 Temperature System
Complete temperature monitoring system: ESP32 firmware, web-based flasher, and local backend infrastructure.

![Grafana Dashboard](./docs/grafana-dashboard.png)

**Before you begin:** see the [hardware guide](docs/hardware.md) for compatible ESP32 boards, DHT22 sensor, and wiring diagrams.

## Quick Start

### 1. Start Backend (InfluxDB + Grafana)
Requires Docker Compose.

```bash
make backend-start
```

Access services - works out of the box with default credentials. See [backend docs](./backend/README.md) to customize.
- InfluxDB: http://localhost:8086
- Grafana: http://localhost:3000 → "ESP32 Temperature & Humidity" dashboard

### 2. Flash ESP32 Device

**[Firmware Flash Web Tool](https://jveldboom.github.io/esp32-temperature-system/)** - requires Chrome or Edge (WebSerial is not supported in Firefox or Safari)

1. Select firmware version
2. Enter configuration:
   - WiFi credentials
   - InfluxDB URL: `http://<your-ip>:8086` (your machine's local network IP - run `ipconfig` on Windows or `ifconfig` on Mac/Linux)
   - Token, Org, Bucket: use the defaults from step 1 (or your custom values if configured)
3. Connect ESP32 via USB
4. Click "Connect & Flash Firmware"
5. View device output from the "Serial Monitor" tab

Once flashing is complete, your device will start sending data. It should appear in the Grafana dashboard within ~60 seconds.

## Troubleshooting

- **Flash tool not working / no "Connect" option** - WebSerial is only supported in Chrome and Edge. Firefox and Safari are not supported.

- **ESP32 not detected by the browser** - Some USB cables are charge-only and don't support data. The cable that came with my ESP32-C3 Supermini didn't work at all - try a different cable you know works. If it's still not detected, you may need to install a USB-to-serial driver for your operating system.

- **ESP32 keeps restarting or disconnecting** - Usually a wiring issue. Check the [hardware guide](docs/hardware.md) to verify your connections are correct.

- **No data appearing in Grafana**
  - Confirm the backend is running: `make backend-start`
  - Check the InfluxDB URL entered during flash — it must be your machine's local IP (e.g. `http://192.168.0.x:8086`), not `localhost`
  - Use the Serial Monitor tab in the [web interface](https://jveldboom.github.io/esp32-temperature-system/) to check for connection errors
  - Verify the token, org, and bucket match the values from [step 1](#1-start-backend-influxdb--grafana)

## Contributing

- Install the PlatformIO VS Code extension
- Run frontend locally with `make frontend-start`
