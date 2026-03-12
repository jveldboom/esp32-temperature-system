#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <InfluxDbClient.h>
#include "config.h"

// ----- DHT22 config -----
#define DHTPIN  4
#define DHTTYPE DHT22

// ----- Timing config -----
#define READING_INTERVAL_MS 60000  // 60 seconds between readings

DHT dht(DHTPIN, DHTTYPE);
InfluxDBClient* client = nullptr;
Point sensor("dht22");

void connectWifi() {
  Serial.printf("Connecting to WiFi SSID: '%s'\n", cfgWifiSsid.c_str());
  Serial.printf("SSID length: %d\n", cfgWifiSsid.length());
  Serial.printf("Password length: %d\n", cfgWifiPass.length());

  // Set WiFi mode to station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(cfgWifiSsid.c_str(), cfgWifiPass.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("Connected! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.printf("Failed to connect to WiFi! Status code: %d\n", WiFi.status());
    Serial.println("Status codes: 0=IDLE, 1=NO_SSID_AVAIL, 3=CONNECTED, 4=CONNECT_FAILED, 6=DISCONNECTED");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Load or receive configuration
  if (!loadConfig()) {
    if (waitForConfig()) {
      Serial.println("Configuration received from serial:");
      Serial.printf("  Location: '%s'\n", cfgLocation.c_str());
      Serial.printf("  WiFi SSID: '%s'\n", cfgWifiSsid.c_str());
      Serial.printf("  WiFi Pass: '%s'\n", cfgWifiPass.c_str());
      Serial.printf("  InfluxDB URL: '%s'\n", cfgInfluxUrl.c_str());
      saveConfig();
    } else {
      Serial.println("ERROR: No configuration available!");
      Serial.println("Device cannot operate without configuration.");
      while (true) { delay(1000); } // Halt
    }
  } else {
    Serial.println("Using stored configuration:");
    Serial.printf("  Location: '%s'\n", cfgLocation.c_str());
    Serial.printf("  WiFi SSID: '%s'\n", cfgWifiSsid.c_str());
    Serial.printf("  WiFi Pass: '%s'\n", cfgWifiPass.c_str());
    Serial.printf("  InfluxDB URL: '%s'\n", cfgInfluxUrl.c_str());
  }

  // Initialize sensor
  dht.begin();

  // Connect to WiFi
  connectWifi();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ERROR: WiFi connection required!");
    while (true) { delay(1000); } // Halt
  }

  // Initialize InfluxDB client
  client = new InfluxDBClient(
    cfgInfluxUrl.c_str(),
    cfgInfluxOrg.c_str(),
    cfgInfluxBucket.c_str(),
    cfgInfluxToken.c_str()
  );

  // Static tags applied to every data point
  sensor.addTag("device",   ESP.getChipModel());
  sensor.addTag("location", cfgLocation.c_str());
  sensor.addTag("ip",       WiFi.localIP().toString());

  if (client->validateConnection()) {
    Serial.printf("Connected to InfluxDB: %s\n", client->getServerUrl().c_str());
  } else {
    Serial.printf("InfluxDB connection failed: %s\n", client->getLastErrorMessage().c_str());
  }
}

void loop() {
  float humidity = dht.readHumidity();
  float tempC    = dht.readTemperature();
  float tempF    = dht.readTemperature(true);

  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("ERROR: Failed to read from sensor!");
    delay(READING_INTERVAL_MS);
    return;
  }

  float heatIndexC = dht.computeHeatIndex(tempC, humidity, false);
  float heatIndexF = dht.computeHeatIndex(tempF, humidity, true);

  sensor.clearFields();

  // Sensor readings
  sensor.addField("humidity",      humidity);
  sensor.addField("temperature_c", tempC);
  sensor.addField("temperature_f", tempF);
  sensor.addField("heat_index_c",  heatIndexC);
  sensor.addField("heat_index_f",  heatIndexF);

  // WiFi stats
  sensor.addField("rssi",    WiFi.RSSI());
  sensor.addField("channel", WiFi.channel());

  // Device health
  sensor.addField("uptime_s",  millis() / 1000);
  sensor.addField("free_heap", ESP.getFreeHeap());

  Serial.printf("Humidity: %.2f%%  Temp: %.2f°C  RSSI: %d dBm  Uptime: %lus  Heap: %u bytes  → ",
              humidity, tempC, WiFi.RSSI(), millis() / 1000, ESP.getFreeHeap());

  if (!client->writePoint(sensor)) {
    Serial.printf("Write failed: %s\n", client->getLastErrorMessage().c_str());
  } else {
    Serial.println("Written to InfluxDB ✓");
  }

  // Reconnect WiFi if dropped
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting...");
    connectWifi();
  }

  delay(READING_INTERVAL_MS);
}