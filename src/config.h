#pragma once

#include <Arduino.h>

//const String id = "esp8266-" + String(ESP.getChipId(), HEX);
//const String configUrl = "https://smarthome.coolex.info/config/" + id;
//const char* fingerprint = "67 DC 50 88 2C 9E 44 B8 BE 4F A3 24 4B 68 DA 64 CD 84 80 E1";

class Config
{
public:
    static Config& instance() {
        static Config _instance;
        return _instance;
    }
    
    Config (const Config&) = delete;
    Config& operator= (const Config&) = delete;

    const char* id() {
        return _id.c_str();
    }
    
    const char* mqttServer() {
        return _mqttServer;
    }
    
    int mqttPort() {
        return _mqttPort;
    }
    
    const char* mqttUser() {
        return _mqttUser;
    }
    
    const char* mqttPass() {
        return _mqttPass;
    }
    
    const char* mqttTopic() {
        return _mqttTopic;
    }
    
    const char* ssid() {
        return _ssid;
    }
    
    const char* wifiPassword() {
        return _wifiPassword;
    }
    
private:
    Config();
    
    String _id;
    const char* _mqttServer;
    int _mqttPort;
    const char* _mqttUser;
    const char* _mqttPass;
    const char* _mqttTopic;
    const char* _ssid;
    const char* _wifiPassword;
};
