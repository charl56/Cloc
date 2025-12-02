#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "stepper_controller.h"

// Objets globaux
WiFiManager wifiManager;
StepperController stepperController;
WebServer *webServer;

void setup()
{
  // Initialisation du port série
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\n");
  Serial.println("================================");
  Serial.println("   ESP32 Stepper Clock Control  ");
  Serial.println("================================");

  // Initialisation du contrôleur de moteurs
  Serial.println("\n[1/3] Initialisation des moteurs...");
  stepperController.begin();

  // Démarrage du WiFi en mode Access Point
  Serial.println("\n[2/3] Démarrage du WiFi Access Point...");
  if (!wifiManager.startAccessPoint(WIFI_SSID, WIFI_PASSWORD))
  {
    Serial.println("ERREUR: Impossible de démarrer l'Access Point!");
    while (1)
    {
      delay(1000);
    }
  }

  // Démarrage du serveur web
  Serial.println("\n[3/3] Démarrage du serveur web...");
  webServer = new WebServer(&stepperController);
  webServer->begin();

  // Affichage des informations de connexion
  Serial.println("\n================================");
  Serial.println("✓ Système prêt!");
  Serial.println("================================");
  Serial.printf("→ Connectez-vous au WiFi: %s\n", WIFI_SSID);
  Serial.printf("→ Mot de passe: %s\n", WIFI_PASSWORD);
  Serial.printf("→ Ouvrez votre navigateur sur: http://%s\n", wifiManager.getIP().toString().c_str());
  Serial.println("================================\n");
}

void loop()
{
  // Mise à jour des moteurs (CRITIQUE pour le mouvement)
  stepperController.update();

  // Petit délai pour éviter de surcharger le CPU
  delayMicroseconds(10);
}