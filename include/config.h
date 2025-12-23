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
#define STEPS_PER_REVOLUTION 200  // 200 pas en full step (M0=M1=M2=LOW)
#define TOTAL_POSITIONS 60        // 60 positions (0-59 minutes)
#define MICROSTEPS 1              // Full step, pas de microstepping
#define STEP_DELAY_US 2000        // Délai entre deux pas en microsecondes
// Vitesse moteur (pas/seconde)
#define MOTOR_SPEED 200           // Vitesse par défaut
#define MOTOR_ACCELERATION 500    // Accélération (pas/s²)


// ============================================
// Configuration Lasers
// ============================================
#define LASER1_PIN 12           // GPIO pour laser 1
#define LASER2_PIN 14           // GPIO pour laser 2

// Canaux PWM ESP32 (0-15 disponibles)
#define LASER1_PWM_CHANNEL 0
#define LASER2_PWM_CHANNEL 1


#endif // CONFIG_H