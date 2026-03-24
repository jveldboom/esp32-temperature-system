# ESP32 Temperature System - Backend

This directory contains a Docker Compose setup for **local development and testing** of InfluxDB v2 and Grafana. This is designed to help you quickly get started and test your ESP32 temperature sensors.

This setup uses default configurations and simple networking suitable for local testing only. For production deployments, consider proper security hardening, SSL/TLS, authentication, backups, and monitoring.

## Prerequisites

- Docker
- Docker Compose

## Quick Start

1. **Start the services:**
   ```bash
   # from the project root
   make backend-start
   ```

2. **Access the services:**
   - **InfluxDB UI**: http://localhost:8086
   - **Grafana**: http://localhost:3000

The setup works out of the box with default credentials - no configuration required. If you'd like to use your own credentials, you can override them with a `.env` file (see [Custom Configuration](#custom-configuration) below).

## Configuration

### Default Credentials

The following defaults are built into [docker-compose.yaml](./docker-compose.yaml) so the setup works without any changes:

| Service  | Setting           | Default         | Required for ESP32 Setup |
|----------|-------------------|-----------------|--------------------------|
| InfluxDB | Organization      | `home`          | ✓                        |
| InfluxDB | Bucket            | `sensors`       | ✓                        |
| InfluxDB | Username          | `admin`         |                          |
| InfluxDB | Password          | `password123`   |                          |
| InfluxDB | Token             | `my-dev-token`  | ✓                        |
| Grafana  | Username          | `admin`         |                          |
| Grafana  | Password          | `password123`   |                          |

> Values marked ✓ are required when flashing firmware to your ESP32 via the [web interface](https://jveldboom.github.io/esp32-temperature-system/).

### Custom Configuration (Optional)

To override any of the defaults, copy `.env.example` to `.env` and update the values:

```bash
cp .env.example .env
```

Then edit `.env` with your preferred credentials. Docker Compose will automatically pick up the file on next start.

### Grafana

Grafana is automatically provisioned with a default dashboard and InfluxDB datasource.

1. Log in at http://localhost:3000
2. Click "Dashboard" → "ESP32 Temperature & Humidity" dashboard


## Managing Docker Services

```bash
# stop all services
make backend-stop

# stop and delete all containers and volumes
make backend-delete
```
