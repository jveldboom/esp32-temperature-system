#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Configuration timeout
#define CONFIG_TIMEOUT_MS 10000  // 10 seconds to receive config

// Configuration storage
extern String cfgLocation;
extern String cfgWifiSsid;
extern String cfgWifiPass;
extern String cfgInfluxUrl;
extern String cfgInfluxToken;
extern String cfgInfluxOrg;
extern String cfgInfluxBucket;

// Configuration functions
bool loadConfig();
void saveConfig();
bool waitForConfig();

#endif
