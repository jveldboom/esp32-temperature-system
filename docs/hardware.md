# ESP32 Hardware Guide

## Required Components

### ESP32 Development Board

**Board Type**: ESP32-C3

- Amazon search: https://www.amazon.com/s?k=ESP32-C3+development+board+USB-C
- **Specs (typical)**:
  - ESP32-C3 microcontroller (RISC-V, Wi‑Fi, Bluetooth LE)
  - USB‑C connector for power and programming
  - On‑board reset and boot buttons
  - 3.3 V operating voltage

### DHT22 Temperature/Humidity Sensor

- Amazon search: https://www.amazon.com/s?k=DHT22+temperature+humidity+sensor
- **Specs (typical)**:
  - Temperature range: -40°C to +80°C
  - Humidity range: 0% to 100% RH
  - Operating voltage: 3.3 V to 6.0 V
  - Digital single-wire interface

### Additional Parts

- USB-C cable (for programming ESP32)
- Jumper wires
- Breadboard (optional)

## Wiring Diagram

### DHT22 to ESP32-C3

| DHT22 Pin | ESP32-C3 Pin | Notes |
|-----------|--------------|-------|
| VCC       | 3.3V         |       |
| DATA      | GPIO 4       | Firmware expects DHT22 DATA on GPIO 4 (`DHTPIN 4`) |
| GND       | GND          |       |

### Wiring Notes

- The current firmware is hardcoded to read the DHT22 sensor on GPIO 4. Make sure the DHT22 DATA pin is connected to GPIO 4 on the ESP32-C3, or update and rebuild the firmware if you need a different pin.

## Setup Instructions

1.
2.
3.

## Troubleshooting

### Common Issues

-

### Tips

-
