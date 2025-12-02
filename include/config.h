#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// Configuration WiFi Access Point
// ============================================
#define WIFI_SSID "C'loc"
#define WIFI_PASSWORD "cloc12345"  // Min 8 caractères
#define WIFI_CHANNEL 1
#define WIFI_HIDDEN false
#define WIFI_MAX_CONNECTIONS 4

// ============================================
// Configuration Web Server
// ============================================
#define WEB_SERVER_PORT 80

// ============================================
// Configuration Moteurs - GPIO Pins
// ============================================

// Moteur 1 (Heures)
#define MOTOR1_STEP_PIN 26
#define MOTOR1_DIR_PIN  27

// Moteur 2 (Minutes)
#define MOTOR2_STEP_PIN 25
#define MOTOR2_DIR_PIN  33

// Microstepping (partagé entre les deux moteurs)
#define M0_PIN 5
#define M1_PIN 4
#define M2_PIN 15

// Enable (optionnel, partagé)
#define ENABLE_PIN 32

// ============================================
// Configuration Horloge
// ============================================
#define STEPS_PER_POSITION 53    // 3200 pas/tour ÷ 60 positions
#define TOTAL_POSITIONS 60       // 60 positions (0-59)
#define MICROSTEPS 16            // Microstepping 1/16

// Vitesse moteur (pas/seconde)
#define MOTOR_SPEED 200         // Vitesse par défaut
#define MOTOR_ACCELERATION 500  // Accélération (pas/s²)

// ============================================
// Constantes calculées
// ============================================
#define STEPS_PER_REVOLUTION (200 * MICROSTEPS)  // 3200 pas pour 1 tour complet

#endif // CONFIG_H