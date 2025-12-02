#include "wifi_manager.h"
#include "config.h"

WiFiManager::WiFiManager() : localIP(192, 168, 4, 1) {}

bool WiFiManager::startAccessPoint(const char* ssid, const char* password) {
    Serial.println("\n=== Démarrage du WiFi Access Point ===");
    
    // Configuration de l'IP statique pour l'AP
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(localIP, localIP, IPAddress(255, 255, 255, 0));
    
    // Démarrage de l'Access Point
    bool success = WiFi.softAP(ssid, password, WIFI_CHANNEL, WIFI_HIDDEN, WIFI_MAX_CONNECTIONS);
    
    if (success) {
        Serial.println("✓ Access Point démarré avec succès");
        Serial.printf("  SSID: %s\n", ssid);
        Serial.printf("  Password: %s\n", password);
        Serial.printf("  IP: %s\n", localIP.toString().c_str());
        Serial.printf("  Canal: %d\n", WIFI_CHANNEL);
        Serial.printf("  Max connexions: %d\n", WIFI_MAX_CONNECTIONS);
        return true;
    } else {
        Serial.println("✗ Échec du démarrage de l'Access Point");
        return false;
    }
}

String WiFiManager::getSSID() const {
    return String(WIFI_SSID);
}

int WiFiManager::getConnectedClients() const {
    return WiFi.softAPgetStationNum();
}