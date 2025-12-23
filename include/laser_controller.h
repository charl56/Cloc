#ifndef LASER_CONTROLLER_H
#define LASER_CONTROLLER_H

#include <Arduino.h>

// ============================================
// Classe Laser - Représente un laser individuel
// ============================================
class Laser {
private:
    uint8_t pin;              // Pin de contrôle du transistor
    bool state;               // État actuel (ON/OFF)
    uint8_t brightness;       // Luminosité PWM (0-255)
    uint8_t pwmChannel;       // Canal PWM de l'ESP32
    String name;              // Nom du laser (pour debug)
    
    // Configuration PWM ESP32
    static const uint32_t PWM_FREQ = 5000;      // 5 kHz
    static const uint8_t PWM_RESOLUTION = 8;    // 8 bits (0-255)

public:
    Laser(uint8_t pin, uint8_t pwmChannel, const String& name);
    
    void begin();
    
    // Contrôle ON/OFF simple
    void on();
    void off();
    void toggle();
    
    // Contrôle PWM (luminosité)
    void setBrightness(uint8_t brightness);  // 0-255
    void setBrightnessPercent(uint8_t percent);  // 0-100
    
    // Getters
    bool isOn() const { return state; }
    uint8_t getBrightness() const { return brightness; }
    uint8_t getBrightnessPercent() const { return map(brightness, 0, 255, 0, 100); }
    String getName() const { return name; }
    
    // Effets
    void pulse(uint16_t durationMs);  // Pulse simple
    void blink(uint16_t onTimeMs, uint16_t offTimeMs, uint8_t times);
};

// ============================================
// Classe LaserController - Gère les 2 lasers
// ============================================
class LaserController {
private:
    Laser* laser1;
    Laser* laser2;

public:
    LaserController();
    ~LaserController();
    
    void begin();
    
    // Accès aux lasers individuels
    Laser* getLaser1() { return laser1; }
    Laser* getLaser2() { return laser2; }
    
    // Contrôle groupé
    void allOn();
    void allOff();
    void setAllBrightness(uint8_t brightness);
    
    // Patterns synchronisés
    void alternatingBlink(uint16_t intervalMs, uint8_t times);
    void breathingEffect(uint16_t cycleDurationMs);
    
    // Status
    void printStatus();
};

#endif // LASER_CONTROLLER_H

