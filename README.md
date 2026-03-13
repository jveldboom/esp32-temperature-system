# ESP32 Temperature System
Complete temperature monitoring system: ESP32 firmware, web-based flasher, and local backend infrastructure.

## Quick Start

### 1. Flash ESP32 Device

See [hardware requirements](docs/hardware.md) for list of compatible ESP32 boards, DHT22 sensor, and wiring guide.

**[Firmware Flash Web Tool](https://jveldboom.github.io/esp32-temperature-system/)**

1. Select firmware version
2. Enter configuration:
   - WiFi credentials
   - InfluxDB: `http://<your-ip>:8086` (`http://localhost:8086` if using Docker Compose below)
   - Token: `my-dev-token`
   - Org: `home`, Bucket: `sensors`
3. Connect ESP32 via USB
4. Click "Connect & Flash Firmware"
5. View device output from the "Serial Monitor" tab

You should see something like this every 60 seconds if everything is working. If still having issues, view the [Device Hardware](docs/hardware.md) document for device specific guidance.
```
Connecting to WiFi: <my-ssid>
Connected! IP: 192.168.0.200
Humidity: 50% Temp: 23°C RSSI: 35 dBm Heap: 210000 bytes
Humidity: 50% Temp: 23°C RSSI: 35 dBm Heap: 210000 bytes
```

### 2. Start Backend (InfluxDB + Grafana)
This will spin up a local copy of InfluxDB & Grafana. Note this requires having Docker Compose installed locally.

```bash
make start-backend
```

Access services:
- InfluxDB: http://localhost:8086
- Grafana: http://localhost:3000 (admin/admin)

The InfluxDB Data Explorer is a great way to view all the available data. Below is a quick example InfluxDB query to show all data in the `sensors` bucket:

```flux
from(bucket: "sensors")
  |> range(start: v.timeRangeStart, stop: v.timeRangeStop)
```

## Contributing
Full details coming soon -- quick reference bullet points

- Install Platform.io VS Studio Extension
- Run frontend locally with `make start-frontend`
