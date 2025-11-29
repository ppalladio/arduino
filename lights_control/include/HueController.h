#ifndef HUE_CONTROLLER_H
#define HUE_CONTROLLER_H

#include <Arduino.h>

class HueController {
public:
    HueController();
    
    // Setup and discovery
    void begin();
    void discoverLights();
    String createUser();
    
    // Light control
    void setLight(int lightId, bool on);
    bool getLightState(int lightId);
    void toggleLight(int lightId);
    void setAllLights(bool on);
    void toggleAllLights();
    
    // Group control
    void setGroup(int groupId, bool on);
    
private:
    String buildUrl(const char* endpoint);
    bool sendRequest(const String& url, const String& method, const String& body = "");
};

#endif