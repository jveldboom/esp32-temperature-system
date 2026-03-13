#include "config.h"
#include <Preferences.h>

// Configuration storage
String cfgLocation;
String cfgWifiSsid;
String cfgWifiPass;
String cfgInfluxUrl;
String cfgInfluxToken;
String cfgInfluxOrg;
String cfgInfluxBucket;

Preferences prefs;

bool loadConfig() {
  // Try to open preferences namespace (read-only)
  if (!prefs.begin("config", true)) {
    // NVS not initialized yet - this is normal on first boot
    Serial.println("NVS not initialized - waiting for configuration");
    return false;
  }

  cfgLocation    = prefs.getString("location", "");
  cfgWifiSsid    = prefs.getString("wifi_ssid", "");
  cfgWifiPass    = prefs.getString("wifi_pass", "");
  cfgInfluxUrl   = prefs.getString("influx_url", "");
  cfgInfluxToken = prefs.getString("influx_token", "");
  cfgInfluxOrg   = prefs.getString("influx_org", "");
  cfgInfluxBucket = prefs.getString("influx_bucket", "");

  prefs.end();

  bool hasConfig = cfgWifiSsid.length() > 0 && cfgInfluxUrl.length() > 0;

  if (hasConfig) {
    Serial.println("Configuration loaded from NVS");
  }

  return hasConfig;
}

void saveConfig() {
  prefs.begin("config", false); // read-write

  prefs.putString("location", cfgLocation);
  prefs.putString("wifi_ssid", cfgWifiSsid);
  prefs.putString("wifi_pass", cfgWifiPass);
  prefs.putString("influx_url", cfgInfluxUrl);
  prefs.putString("influx_token", cfgInfluxToken);
  prefs.putString("influx_org", cfgInfluxOrg);
  prefs.putString("influx_bucket", cfgInfluxBucket);

  prefs.end();

  Serial.println("Configuration saved to NVS");
}

bool waitForConfig() {
  Serial.println("Waiting for configuration on Serial (10 seconds)...");
  Serial.println("Send config as key=value pairs, ending with CONFIG_END");

  unsigned long startTime = millis();
  String line = "";

  while (millis() - startTime < CONFIG_TIMEOUT_MS) {
    if (Serial.available()) {
      char c = Serial.read();

      if (c == '\n') {
        line.trim();

        if (line == "CONFIG_END") {
          Serial.println("Configuration received!");
          return true;
        }

        int eqPos = line.indexOf('=');
        if (eqPos > 0) {
          String key = line.substring(0, eqPos);
          String value = line.substring(eqPos + 1);

          key.trim();
          value.trim();

          if (key == "LOCATION") cfgLocation = value;
          else if (key == "WIFI_SSID") cfgWifiSsid = value;
          else if (key == "WIFI_PASS") cfgWifiPass = value;
          else if (key == "INFLUX_URL") cfgInfluxUrl = value;
          else if (key == "INFLUX_TOKEN") cfgInfluxToken = value;
          else if (key == "INFLUX_ORG") cfgInfluxOrg = value;
          else if (key == "INFLUX_BUCKET") cfgInfluxBucket = value;

          Serial.printf("Set %s\n", key.c_str());
        }

        line = "";
      } else if (c != '\r') {
        line += c;
      }
    }
    delay(10);
  }

  Serial.println("Configuration timeout - no data received");
  return false;
}
