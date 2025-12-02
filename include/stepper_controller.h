#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <Arduino.h>

class StepperMotor {
private:
    uint8_t stepPin;
    uint8_t dirPin;
    
    int32_t currentPosition;      // Position actuelle en pas
    int32_t targetPosition;       // Position cible en pas
    
    uint32_t stepDelay;           // Délai entre chaque pas (microsecondes)
    uint32_t lastStepTime;        // Timestamp du dernier pas
    
    bool isMoving;

public:
    StepperMotor(uint8_t stepPin, uint8_t dirPin);
    
    void begin();
    void setSpeed(uint16_t stepsPerSecond);
    void moveTo(int32_t position);
    void moveToPosition(uint8_t position);  // Position 0-59
    void update();                          // À appeler dans loop()
    
    int32_t getCurrentPosition() const { return currentPosition; }
    uint8_t getCurrentClockPosition() const;  // Retourne 0-59
    bool getIsMoving() const { return isMoving; }
    
    void stop();
    void setCurrentPosition(int32_t position);
};

class StepperController {
private:
    StepperMotor* motor1;
    StepperMotor* motor2;
    
public:
    StepperController();
    ~StepperController();
    
    void begin();
    void update();
    
    void setMotor1Position(uint8_t position);  // 0-59
    void setMotor2Position(uint8_t position);  // 0-59
    
    uint8_t getMotor1Position() const;
    uint8_t getMotor2Position() const;
    
    bool isMotor1Moving() const;
    bool isMotor2Moving() const;
    
    void stopAll();
    void enableMotors(bool enable);
};

#endif // STEPPER_CONTROLLER_H