#include <WiFi.h>
#include "config.h"
#include "HueController.h"
#include "RemoteHandler.h" 
// Global instances
HueController hue;
RemoteHandler remote(IR_RECEIVE_PIN);

// Light ID storage
int cellingId = 0;
int deskId = 0;
int lamp1Id = 0;
int lamp2Id = 0;

// ============================================================================
// Setup Functions
// ============================================================================

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    Serial.print("[WiFi] Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\n[WiFi] Connected: %s\n", WiFi.localIP().toString().c_str());
}

void setupHue() {
    hue.begin();
    
    if (String(HUE_USERNAME) == "YOUR_HUE_USERNAME_HERE") {
        Serial.println("\n[SETUP] First-time setup required:");
        Serial.println("  1. Press button on Hue Bridge");
        Serial.println("  2. Press Button 0 on remote");
    } else {
        hue.discoverLights();
        Serial.println("[HUE] Ready");
    }
}

void setupRemote() {
    remote.begin();
    
    // Register button handlers
    
    // Button 0: Create Hue user (setup only)
    remote.onButton0([]() {
        Serial.println("\n[BTN 0] Creating Hue user...");
        hue.createUser();
    });
    
    // Button 1: Toggle all lights
    remote.onButton1([]() {
        Serial.println("[BTN 1] All lights toggle");
        hue.toggleAllLights();
    });
    
    // Button 2: Toggle Celling light
    remote.onButton2([]() {
        Serial.println("[BTN 2] Celling toggle");
        hue.toggleLight(cellingId);
    });
    
    // Button 3: Toggle Desk light
    remote.onButton3([]() {
        Serial.println("[BTN 3] Desk toggle");
        hue.toggleLight(deskId);
    });
    
    // Button 4: Toggle both Lamps
    remote.onButton4([]() {
        Serial.println("[BTN 4] Lamps toggle");
        bool state = hue.getLightState(lamp1Id);
        hue.setLight(lamp1Id, !state);
        hue.setLight(lamp2Id, !state);
    });
    
    // Button UP: Example - future use
    remote.onButtonUp([]() {
        Serial.println("[BTN UP] Available for future feature");
    });
    
    // Button DOWN: Example - future use
    remote.onButtonDown([]() {
        Serial.println("[BTN DOWN] Available for future feature");
    });
    
    // Button OK: Example - future use
    remote.onButtonOk([]() {
        Serial.println("[BTN OK] Available for future feature");
    });
    
    Serial.println("[IR] Button handlers registered");
}

// ============================================================================
// Main
// ============================================================================

void setup() {
    Serial.begin(115200);
    delay(500);
    
    Serial.println("\n=== ESP32 Smart Remote ===\n");
    
    setupWiFi();
    setupHue();
    setupRemote();
    
    Serial.println("\n=== Ready ===\n");
}

void loop() {
    remote.handle();
    delay(10);
}
