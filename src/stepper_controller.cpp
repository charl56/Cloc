#include "stepper_controller.h"
#include "config.h"

// ============================================
// StepperMotor Implementation
// ============================================

StepperMotor::StepperMotor(uint8_t stepPin, uint8_t dirPin)
    : stepPin(stepPin), dirPin(dirPin), currentPosition(0), 
      targetPosition(0), stepDelay(500), lastStepTime(0), isMoving(false) {}

void StepperMotor::begin() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    digitalWrite(stepPin, LOW);
    digitalWrite(dirPin, LOW);
    setSpeed(MOTOR_SPEED);
}

void StepperMotor::setSpeed(uint16_t stepsPerSecond) {
    if (stepsPerSecond > 0) {
        stepDelay = 1000000UL / stepsPerSecond;  // Conversion en microsecondes
    }
}

void StepperMotor::moveTo(int32_t position) {
    targetPosition = position;
    isMoving = (currentPosition != targetPosition);
}

void StepperMotor::moveToPosition(uint8_t position) {
    if (position >= TOTAL_POSITIONS) {
        position = TOTAL_POSITIONS - 1;
    }
    int32_t targetSteps = position * STEPS_PER_POSITION;
    moveTo(targetSteps);
}

void StepperMotor::update() {
    if (!isMoving) return;
    
    uint32_t currentTime = micros();
    
    if (currentTime - lastStepTime >= stepDelay) {
        if (currentPosition < targetPosition) {
            // Avancer
            digitalWrite(dirPin, HIGH);
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(5);  // Pulse minimum 1.9µs pour DRV8825
            digitalWrite(stepPin, LOW);
            currentPosition++;
        } 
        else if (currentPosition > targetPosition) {
            // Reculer
            digitalWrite(dirPin, LOW);
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(5);
            digitalWrite(stepPin, LOW);
            currentPosition--;
        }
        
        lastStepTime = currentTime;
        
        // Vérifier si on a atteint la cible
        if (currentPosition == targetPosition) {
            isMoving = false;
        }
    }
}

uint8_t StepperMotor::getCurrentClockPosition() const {
    int32_t pos = currentPosition / STEPS_PER_POSITION;
    return (uint8_t)(pos % TOTAL_POSITIONS);
}

void StepperMotor::stop() {
    targetPosition = currentPosition;
    isMoving = false;
}

void StepperMotor::setCurrentPosition(int32_t position) {
    currentPosition = position;
    targetPosition = position;
    isMoving = false;
}

// ============================================
// StepperController Implementation
// ============================================

StepperController::StepperController() {
    motor1 = new StepperMotor(MOTOR1_STEP_PIN, MOTOR1_DIR_PIN);
    motor2 = new StepperMotor(MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);
}

StepperController::~StepperController() {
    delete motor1;
    delete motor2;
}

void StepperController::begin() {
    // Configuration des pins de microstepping
    pinMode(M0_PIN, OUTPUT);
    pinMode(M1_PIN, OUTPUT);
    pinMode(M2_PIN, OUTPUT);
    
    // Configuration pour 1/16 microstepping
    // M0=HIGH, M1=HIGH, M2=LOW = 1/16
    digitalWrite(M0_PIN, HIGH);
    digitalWrite(M1_PIN, HIGH);
    digitalWrite(M2_PIN, LOW);
    
    // Configuration du pin ENABLE (LOW = activé)
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);
    
    // Initialisation des moteurs
    motor1->begin();
    motor2->begin();
    
    Serial.println("Stepper Controller initialisé");
    Serial.printf("Microstepping: 1/%d\n", MICROSTEPS);
    Serial.printf("Pas par position: %d\n", STEPS_PER_POSITION);
}

void StepperController::update() {
    motor1->update();
    motor2->update();
}

void StepperController::setMotor1Position(uint8_t position) {
    motor1->moveToPosition(position);
}

void StepperController::setMotor2Position(uint8_t position) {
    motor2->moveToPosition(position);
}

uint8_t StepperController::getMotor1Position() const {
    return motor1->getCurrentClockPosition();
}

uint8_t StepperController::getMotor2Position() const {
    return motor2->getCurrentClockPosition();
}

bool StepperController::isMotor1Moving() const {
    return motor1->getIsMoving();
}

bool StepperController::isMotor2Moving() const {
    return motor2->getIsMoving();
}

void StepperController::stopAll() {
    motor1->stop();
    motor2->stop();
}

void StepperController::enableMotors(bool enable) {
    digitalWrite(ENABLE_PIN, enable ? LOW : HIGH);
}