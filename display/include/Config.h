#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions
#define IR_RECEIVE_PIN 15
#define LED_PIN 2

// WiFi Configuration (loaded from .env)
#ifndef WIFI_SSID
#define WIFI_SSID "DefaultSSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "DefaultPassword"
#endif

// Hue Configuration (loaded from .env)
#ifndef HUE_BRIDGE_IP
#define HUE_BRIDGE_IP "192.168.1.100"
#endif

#ifndef HUE_USERNAME
#define HUE_USERNAME "default-username"
#endif


// Network Configuration
#define HTTP_TIMEOUT 5000
#define MAX_RETRY_ATTEMPTS 3

// Light IDs (auto-discovered, but can be manually set)
extern int cellingId;
extern int deskId;
extern int lamp1Id;
extern int lamp2Id;

#endif