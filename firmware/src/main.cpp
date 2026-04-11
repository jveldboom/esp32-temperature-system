#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <InfluxDbClient.h>
#include "config.h"

#define DHTPIN 4
#define DHTTYPE DHT22

#define READING_INTERVAL_MS 60000
#define WIFI_MAX_RECONNECT_ATTEMPTS 3

DHT dht(DHTPIN, DHTTYPE);
InfluxDBClient* client = nullptr;
Point sensor("dht22");
int wifiReconnectFailures = 0;

// Attempts to connect/reconnect to WiFi. Reboots after WIFI_MAX_RECONNECT_ATTEMPTS
// consecutive failures to recover from states where the WiFi stack gets stuck.
bool connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return true;

  if (wifiReconnectFailures == 0) {
    Serial.printf("Connecting to WiFi %s...\n", cfgWifiSsid.c_str());
  } else {
    Serial.printf("WiFi lost, reconnecting to %s...\n", cfgWifiSsid.c_str());
  }
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(cfgWifiSsid.c_str(), cfgWifiPass.c_str());

  // Poll for connection every 500ms, up to 20 seconds total
  for (int i = 0; i < 40 && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("Connected! IP: %s\n", WiFi.localIP().toString().c_str());
    wifiReconnectFailures = 0;
    return true;
  }

  wifiReconnectFailures++;
  Serial.printf("Reconnect failed (%d/%d)\n", wifiReconnectFailures, WIFI_MAX_RECONNECT_ATTEMPTS);
  if (wifiReconnectFailures >= WIFI_MAX_RECONNECT_ATTEMPTS) {
    Serial.println("Max reconnect attempts reached, rebooting...");
    delay(1000);
    ESP.restart();
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  delay(100);

  if (!loadConfig()) {
    if (waitForConfig()) {
      saveConfig();
    } else {
      Serial.println("ERROR: No configuration available. Device cannot operate.");
      while (true) { delay(1000); }
    }
  }

  dht.begin();

  while (!connectWifi()) {
    Serial.println("Retrying WiFi connection...");
    delay(5000);
  }

  client = new InfluxDBClient(
    cfgInfluxUrl.c_str(),
    cfgInfluxOrg.c_str(),
    cfgInfluxBucket.c_str(),
    cfgInfluxToken.c_str()
  );

  String localIp = WiFi.localIP().toString();
  sensor.addTag("mac",              WiFi.macAddress());
  sensor.addTag("chip_model",       ESP.getChipModel());
  sensor.addTag("firmware_version", FIRMWARE_VERSION);
  sensor.addTag("location",         cfgLocation.c_str());
  sensor.addTag("ip",               localIp);

  if (client->validateConnection()) {
    Serial.printf("Connected to InfluxDB: %s\n", client->getServerUrl().c_str());
  } else {
    Serial.printf("InfluxDB connection failed: %s\n", client->getLastErrorMessage().c_str());
  }

  Serial.println("\nSetup complete! Starting sensor loop...");
}

void loop() {
  if (!connectWifi()) {
    delay(READING_INTERVAL_MS);
    return;
  }

  float humidity = dht.readHumidity();
  float tempC    = dht.readTemperature();
  float tempF    = dht.readTemperature(true);

  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("ERROR: Failed to read from sensor!");
    delay(10000);
    return;
  }

  int      rssi         = WiFi.RSSI();
  uint32_t heapFree     = ESP.getFreeHeap();
  uint32_t heapMinFree  = ESP.getMinFreeHeap();
  uint32_t heapMaxAlloc = ESP.getMaxAllocHeap();
  // Percentage of free heap that is fragmented (not available as a single block)
  float    heapFrag     = (heapFree > 0) ? (1.0f - (float)heapMaxAlloc / (float)heapFree) * 100.0f : 0.0f;

  sensor.clearFields();
  sensor.addField("heap_fragmentation",  heapFrag);
  sensor.addField("heap_free",           heapFree);
  sensor.addField("heap_min_free",       heapMinFree);
  sensor.addField("heat_index_c",        dht.computeHeatIndex(tempC, humidity, false));
  sensor.addField("heat_index_f",        dht.computeHeatIndex(tempF, humidity, true));
  sensor.addField("humidity",            humidity);
  sensor.addField("temperature_c",       tempC);
  sensor.addField("temperature_f",       tempF);
  sensor.addField("uptime_s",            millis() / 1000);
  sensor.addField("wifi_channel",        WiFi.channel());
  sensor.addField("wifi_reconnect_count", wifiReconnectFailures);
  sensor.addField("wifi_rssi",           rssi);

  Serial.printf("Humidity: %.2f%% Temp: %.2f°C RSSI: %d dBm Heap: %u bytes Frag: %.1f%% → ",
                humidity, tempC, rssi, heapFree, heapFrag);

  if (!client->writePoint(sensor)) {
    Serial.printf("Write failed: %s\n", client->getLastErrorMessage().c_str());
  } else {
    Serial.println("Written to InfluxDB ✓");
  }

  delay(READING_INTERVAL_MS);
}
