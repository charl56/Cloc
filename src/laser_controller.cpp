#include "laser_controller.h"
#include "config.h"

// ============================================
// Implémentation Laser
// ============================================

Laser::Laser(uint8_t pin, uint8_t pwmChannel, const String& name)
    : pin(pin), pwmChannel(pwmChannel), name(name), 
      state(false), brightness(255) {}

void Laser::begin() {
    // Configuration du pin en sortie
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    
    // Configuration du canal PWM
    ledcSetup(pwmChannel, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(pin, pwmChannel);
    ledcWrite(pwmChannel, 0);
    
    Serial.printf("Laser '%s' initialisé (Pin: %d, PWM Channel: %d)\n", 
                  name.c_str(), pin, pwmChannel);
}

void Laser::on() {
    state = true;
    ledcWrite(pwmChannel, brightness);
    Serial.printf("Laser '%s' ON (Brightness: %d)\n", name.c_str(), brightness);
}

void Laser::off() {
    state = false;
    ledcWrite(pwmChannel, 0);
    Serial.printf("Laser '%s' OFF\n", name.c_str());
}

void Laser::toggle() {
    if (state) {
        off();
    } else {
        on();
    }
}

void Laser::setBrightness(uint8_t newBrightness) {
    brightness = newBrightness;
    if (state) {
        ledcWrite(pwmChannel, brightness);
    }
    Serial.printf("Laser '%s' brightness: %d/255\n", name.c_str(), brightness);
}

void Laser::setBrightnessPercent(uint8_t percent) {
    if (percent > 100) percent = 100;
    brightness = map(percent, 0, 100, 0, 255);
    if (state) {
        ledcWrite(pwmChannel, brightness);
    }
    Serial.printf("Laser '%s' brightness: %d%%\n", name.c_str(), percent);
}

void Laser::pulse(uint16_t durationMs) {
    on();
    delay(durationMs);
    off();
}

void Laser::blink(uint16_t onTimeMs, uint16_t offTimeMs, uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        on();
        delay(onTimeMs);
        off();
        if (i < times - 1) {  // Pas de délai après le dernier blink
            delay(offTimeMs);
        }
    }
}

// ============================================
// Implémentation LaserController
// ============================================

LaserController::LaserController() {
    laser1 = new Laser(LASER1_PIN, LASER1_PWM_CHANNEL, "Laser 1");
    laser2 = new Laser(LASER2_PIN, LASER2_PWM_CHANNEL, "Laser 2");
}

LaserController::~LaserController() {
    delete laser1;
    delete laser2;
}

void LaserController::begin() {
    Serial.println("\n=== Initialisation Laser Controller ===");
    laser1->begin();
    laser2->begin();
    Serial.println("=== Laser Controller prêt ===\n");
}

void LaserController::allOn() {
    laser1->on();
    laser2->on();
}

void LaserController::allOff() {
    laser1->off();
    laser2->off();
}

void LaserController::setAllBrightness(uint8_t brightness) {
    laser1->setBrightness(brightness);
    laser2->setBrightness(brightness);
}

void LaserController::alternatingBlink(uint16_t intervalMs, uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        laser1->on();
        laser2->off();
        delay(intervalMs);
        
        laser1->off();
        laser2->on();
        delay(intervalMs);
    }
    allOff();
}

void LaserController::breathingEffect(uint16_t cycleDurationMs) {
    uint16_t steps = 50;
    uint16_t delayTime = cycleDurationMs / (steps * 2);
    
    // Fade in
    for (uint16_t i = 0; i <= steps; i++) {
        uint8_t brightness = map(i, 0, steps, 0, 255);
        laser1->setBrightness(brightness);
        laser2->setBrightness(brightness);
        laser1->on();
        laser2->on();
        delay(delayTime);
    }
    
    // Fade out
    for (uint16_t i = steps; i > 0; i--) {
        uint8_t brightness = map(i, 0, steps, 0, 255);
        laser1->setBrightness(brightness);
        laser2->setBrightness(brightness);
        delay(delayTime);
    }
    
    allOff();
}

void LaserController::printStatus() {
    Serial.println("\n=== Status Lasers ===");
    Serial.printf("%s: %s | Brightness: %d%% (%d/255)\n",
                  laser1->getName().c_str(),
                  laser1->isOn() ? "ON" : "OFF",
                  laser1->getBrightnessPercent(),
                  laser1->getBrightness());
    Serial.printf("%s: %s | Brightness: %d%% (%d/255)\n",
                  laser2->getName().c_str(),
                  laser2->isOn() ? "ON" : "OFF",
                  laser2->getBrightnessPercent(),
                  laser2->getBrightness());
    Serial.println("====================\n");
}
