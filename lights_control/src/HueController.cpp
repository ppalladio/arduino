#include "HueController.h"
#include "config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

HueController::HueController() {}

void HueController::begin() {
    Serial.println("[HUE] Controller initialized");
}

String HueController::buildUrl(const char* endpoint) {
    return "http://" + String(HUE_BRIDGE_IP) + "/api/" + String(HUE_USERNAME) + endpoint;
}

bool HueController::sendRequest(const String& url, const String& method, const String& body) {
    HTTPClient http;
    http.begin(url);
    
    int httpCode = -1;
    
    if (method == "GET") {
        httpCode = http.GET();
    } else if (method == "PUT") {
        http.addHeader("Content-Type", "application/json");
        httpCode = http.PUT(body);
    } else if (method == "POST") {
        http.addHeader("Content-Type", "application/json");
        httpCode = http.POST(body);
    }
    
    bool success = (httpCode == 200);
    http.end();
    return success;
}

void HueController::setLight(int lightId, bool on) {
    if (lightId == 0) return;
    
    String url = buildUrl(("/lights/" + String(lightId) + "/state").c_str());
    String body = on ? "{\"on\":true}" : "{\"on\":false}";
    
    if (sendRequest(url, "PUT", body)) {
        Serial.printf("[HUE] Light %d → %s\n", lightId, on ? "ON" : "OFF");
    }
}

bool HueController::getLightState(int lightId) {
    if (lightId == 0) return false;
    
    HTTPClient http;
    String url = buildUrl(("/lights/" + String(lightId)).c_str());
    
    http.begin(url);
    int httpCode = http.GET();
    
    bool state = false;
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        state = doc["state"]["on"];
    }
    
    http.end();
    return state;
}

void HueController::toggleLight(int lightId) {
    bool currentState = getLightState(lightId);
    setLight(lightId, !currentState);
}

void HueController::setAllLights(bool on) {
    String url = buildUrl("/groups/0/action");
    String body = on ? "{\"on\":true}" : "{\"on\":false}";
    
    if (sendRequest(url, "PUT", body)) {
        Serial.printf("[HUE] All lights → %s\n", on ? "ON" : "OFF");
    }
}

void HueController::toggleAllLights() {
    HTTPClient http;
    String url = buildUrl("/groups/0");
    
    http.begin(url);
    int httpCode = http.GET();
    
    bool anyOn = false;
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        anyOn = doc["state"]["any_on"];
    }
    http.end();
    
    setAllLights(!anyOn);
}

void HueController::setGroup(int groupId, bool on) {
    String url = buildUrl(("/groups/" + String(groupId) + "/action").c_str());
    String body = on ? "{\"on\":true}" : "{\"on\":false}";
    sendRequest(url, "PUT", body);
}

void HueController::discoverLights() {
    HTTPClient http;
    String url = buildUrl("/lights");
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(4096);
        deserializeJson(doc, payload);
        
        Serial.println("[HUE] Discovered lights:");
        
        JsonObject lights = doc.as<JsonObject>();
        for (JsonPair kv : lights) {
            int id = atoi(kv.key().c_str());
            String name = kv.value()["name"].as<String>();
            Serial.printf("  %d: %s\n", id, name.c_str());
            
            // Auto-assign based on name
            String nameLower = name;
            nameLower.toLowerCase();
            
            if (nameLower.indexOf("celling") >= 0) {
                cellingId = id;
            } else if (nameLower == "desk") {
                deskId = id;
            } else if (nameLower.indexOf("lamp") >= 0 && nameLower.indexOf("2") >= 0) {
                lamp2Id = id;
            } else if (nameLower.indexOf("lamp") >= 0) {
                lamp1Id = id;
            }
        }
        
        Serial.printf("\nAssigned: Celling=%d, Desk=%d, Lamp1=%d, Lamp2=%d\n", 
                      cellingId, deskId, lamp1Id, lamp2Id);
    }
    
    http.end();
}

String HueController::createUser() {
    HTTPClient http;
    String url = "http://" + String(HUE_BRIDGE_IP) + "/api";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    String body = "{\"devicetype\":\"esp32_remote#controller\"}";
    int httpCode = http.POST(body);
    
    String username = "";
    if (httpCode == 200) {
        String response = http.getString();
        DynamicJsonDocument doc(512);
        deserializeJson(doc, response);
        
        if (doc[0]["success"]) {
            username = doc[0]["success"]["username"].as<String>();
            Serial.println("[HUE] User created: " + username);
            Serial.println("Update HUE_USERNAME in config.h");
        } else {
            Serial.println("[HUE] Failed - Press bridge button first!");
        }
    }
    
    http.end();
    return username;
}