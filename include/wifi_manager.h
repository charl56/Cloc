#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
private:
    IPAddress localIP;
    
public:
    WiFiManager();
    
    bool startAccessPoint(const char* ssid, const char* password);
    IPAddress getIP() const { return localIP; }
    String getSSID() const;
    int getConnectedClients() const;
};

#endif // WIFI_MANAGER_H