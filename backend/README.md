# ESP32 Temperature System - Backend

This directory contains a Docker Compose setup for **local development and testing** of InfluxDB v2 and Grafana. This is designed to help you quickly get started and test your ESP32 temperature sensors.

This setup uses default configurations and simple networking suitable for local testing only. For production deployments, consider proper security hardening, SSL/TLS, authentication, backups, and monitoring.

## Prerequisites

- Docker
- Docker Compose

## Quick Start

1. **Start the services:**
   ```bash
   docker-compose up -d
   ```

2. **Access the services:**
   - **InfluxDB UI**: http://localhost:8086 (username: `admin`, password: `password123`)
   - **Grafana**: http://localhost:3000 (username: `admin`, password: `password123`)

That's it! InfluxDB will be automatically configured with default credentials suitable for local testing.

## Configuration

### InfluxDB

InfluxDB is automatically initialized with these defaults:
- Organization: `home`
- Bucket: `sensors`
- Admin username: `admin`
- Admin password: `password123`
- Admin token: `my-dev-token`

You'll use these values in your ESP32 firmware configuration.

### Grafana

Grafana automatically connects to InfluxDB on startup. To create your first dashboard:

1. Log in at http://localhost:3000 (username: `admin`, password: `admin`)
2. Click "+" → "Dashboard" → "Add visualization"
3. Select "InfluxDB" as the data source
4. Write a Flux query to fetch your sensor data

Example Flux query to get temperature data:
```flux
from(bucket: "sensors")
  |> range(start: -1h)
  |> filter(fn: (r) => r["_measurement"] == "environment")
  |> filter(fn: (r) => r["_field"] == "temperature")
```

## Managing Docker Services

```bash
# view logs
docker-compose logs -f

# stop services
docker-compose down

# stop and delete all containers and volumes
make backend-delete
```
