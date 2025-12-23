#include <Arduino.h>
#include "config.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "stepper_controller.h"

// Nombre de pas par tour (full step)
const int STEPS_PER_REV = 200;

// Durée entre deux fronts de STEP (en microsecondes)
// Commence large pour être sûr que le moteur suive

// void setup()
// {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("Test minimal DRV8825 + NEMA17");

//   // Config pins
//   pinMode(MOTOR1_STEP_PIN, OUTPUT);
//   pinMode(MOTOR1_DIR_PIN, OUTPUT);
//   pinMode(ENABLE_PIN, OUTPUT);
//   pinMode(M0_PIN, OUTPUT);
//   pinMode(M1_PIN, OUTPUT);
//   pinMode(M2_PIN, OUTPUT);

//   // Full step (M0=M1=M2=LOW, à vérifier avec ta carte)
//   digitalWrite(M0_PIN, LOW);
//   digitalWrite(M1_PIN, LOW);
//   digitalWrite(M2_PIN, LOW);

//   // Active le driver
//   digitalWrite(ENABLE_PIN, LOW); // LOW = activé sur DRV8825

//   // Défaut
//   digitalWrite(MOTOR1_STEP_PIN, LOW);
//   digitalWrite(MOTOR1_DIR_PIN, LOW);

//   // Petite pause avant de démarrer
//   delay(2000);
// }

// void loop()
// {
//   Serial.println("Tour complet sens 1");
//   digitalWrite(MOTOR1_DIR_PIN, HIGH); // sens 1

//   for (int i = 0; i < STEPS_PER_REV; i++)
//   {
//     digitalWrite(MOTOR1_STEP_PIN, HIGH);
//     delayMicroseconds(STEP_DELAY_US);
//     digitalWrite(MOTOR1_STEP_PIN, LOW);
//     delayMicroseconds(STEP_DELAY_US);
//   }

//   delay(1000);

//   Serial.println("Tour complet sens 2");
//   digitalWrite(MOTOR1_DIR_PIN, LOW); // sens 2

//   for (int i = 0; i < STEPS_PER_REV; i++)
//   {
//     digitalWrite(MOTOR1_STEP_PIN, HIGH);
//     delayMicroseconds(STEP_DELAY_US);
//     digitalWrite(MOTOR1_STEP_PIN, LOW);
//     delayMicroseconds(STEP_DELAY_US);
//   }

//   delay(2000);
// }

// Objets globaux
WiFiManager wifiManager;
StepperController stepperController;
LaserController laserController;
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

  // Initialisation du contrôleur de moteurs
  Serial.println("\n[1/3] Initialisation des lasers...");
  laserController.begin();

  // WiFi sur Core 1
  Serial.println("\n[3/3] Démarrage du WiFi Access Point...");
  if (!wifiManager.startAccessPoint(WIFI_SSID, WIFI_PASSWORD))
  {
    Serial.println("ERREUR: Impossible de démarrer l'Access Point!");
    while (1)
      delay(1000);
  }

  // Démarrage du serveur web
  Serial.println("\n[3/3] Démarrage du serveur web...");
  Serial.printf("Begin stepperMotor, position : %d \n", stepperController.getMotor1Minutes());

  webServer = new WebServer(&stepperController, &laserController);
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
  // Le loop() tourne sur Core 1 avec le WiFi
  delay(100); // Juste pour ne pas surcharger
}