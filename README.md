# ESP32 Temperature System
Complete temperature monitoring system: ESP32 firmware, web-based flasher, and local backend infrastructure.

## Quick Start

### 1. Start Backend (InfluxDB + Grafana)
Requires Docker Compose.

```bash
make start-backend
```

Access services:
- InfluxDB: http://localhost:8086
- Grafana: http://localhost:3000 (admin/admin)

Query all sensor data in InfluxDB Data Explorer:

```flux
from(bucket: "sensors")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
```

### 2. Flash ESP32 Device

See [hardware requirements](docs/hardware.md) for compatible ESP32 boards, DHT22 sensor, and wiring guide.

**[Firmware Flash Web Tool](https://jveldboom.github.io/esp32-temperature-system/)**

1. Select firmware version
2. Enter configuration:
   - WiFi credentials
   - InfluxDB: `http://<your-ip>:8086` (`http://localhost:8086` if using Docker Compose above)
   - Token: `my-dev-token`
   - Org: `home`, Bucket: `sensors`
3. Connect ESP32 via USB
4. Click "Connect & Flash Firmware"
5. View device output from the "Serial Monitor" tab

## Contributing

- Install the PlatformIO VS Code extension
- Run frontend locally with `make start-frontend`
