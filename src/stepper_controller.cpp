#include "stepper_controller.h"
#include "config.h"

// ============================================
// StepperMotor Implementation
// ============================================

StepperMotor::StepperMotor(uint8_t stepPin, uint8_t dirPin)
    : stepPin(stepPin), dirPin(dirPin), currentPosition(0),
      targetPosition(0), lastStepTime(0), isMoving(false) {}

void StepperMotor::begin()
{
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    digitalWrite(stepPin, LOW);
    digitalWrite(dirPin, LOW);
}


void StepperMotor::moveTo(int32_t position)
{
    targetPosition = position;
    isMoving = (currentPosition != targetPosition);
}

void StepperMotor::moveToPosition(uint8_t position)
{
    position = position % TOTAL_POSITIONS;
    minutes = position;

    int32_t targetSteps = ((int32_t)position * STEPS_PER_REVOLUTION) / TOTAL_POSITIONS;

    Serial.printf("Déplacement vers la position %d (pas %d)\n", position, targetSteps);

    moveTo(targetSteps);
}

void StepperMotor::update()
{
    if (!isMoving)
        return;

    // Calculer la différence de pas
    int32_t stepsToMove = targetPosition - currentPosition;

    if (stepsToMove == 0)
    {
        isMoving = false;
        return;
    }

    // Déterminer la direction optimale (chemin le plus court sur l'horloge)
    bool direction;
    int32_t absSteps;

    if (abs(stepsToMove) <= STEPS_PER_REVOLUTION / 2)
    {
        // Chemin direct est le plus court
        direction = (stepsToMove > 0); // HIGH = avant, LOW = arrière
        absSteps = abs(stepsToMove);
    }
    else
    {
        // Chemin inverse est plus court
        direction = (stepsToMove < 0);
        absSteps = STEPS_PER_REVOLUTION - abs(stepsToMove);
    }

    // Configurer la direction
    digitalWrite(dirPin, direction ? HIGH : LOW);
    delayMicroseconds(10);

    // Effectuer tous les pas
    for (int32_t i = 0; i < absSteps; i++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(STEP_DELAY_US);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(STEP_DELAY_US); // Délai entre les pas

        // Mettre à jour la position
        if (direction)
        {
            currentPosition++;
        }
        else
        {
            currentPosition--;
        }
    }

    // Normaliser la position si nécessaire (pour rester dans 0-STEPS_PER_REVOLUTION)
    currentPosition = currentPosition % STEPS_PER_REVOLUTION;
    if (currentPosition < 0)
    {
        currentPosition += STEPS_PER_REVOLUTION;
    }

    isMoving = false;
}

uint8_t StepperMotor::getCurrentClockPosition() const
{
    int32_t pos = currentPosition;
    return (uint8_t)(pos % TOTAL_POSITIONS);
}

void StepperMotor::stop()
{
    targetPosition = currentPosition;
    isMoving = false;
}

void StepperMotor::setCurrentPosition(int32_t position)
{
    currentPosition = position;
    targetPosition = position;
    isMoving = false;
}

// Fonction de homing bloquante - amène le moteur à la position 0
void StepperMotor::homeToZero()
{
    Serial.println("Homing en cours...");

    // Aller à la position 0
    moveTo(0);

    

    // Attendre que le mouvement soit terminé
    while (isMoving)
    {
        update();
    }

    // Restaurer la vitesse d'origine

    Serial.printf("Homing terminé - Position 0 atteinte : %d \n", minutes);
}

// ============================================
// StepperController Implementation
// ============================================

StepperController::StepperController()
{
    motor1 = new StepperMotor(MOTOR1_STEP_PIN, MOTOR1_DIR_PIN);
    motor2 = new StepperMotor(MOTOR2_STEP_PIN, MOTOR2_DIR_PIN);
    motorTaskHandle = NULL;
}

StepperController::~StepperController()
{
    // Arrêter la tâche proprement
    if (motorTaskHandle != NULL)
    {
        vTaskDelete(motorTaskHandle);
        motorTaskHandle = NULL;
    }

    delete motor1;
    delete motor2;
}

void StepperController::begin()
{
    // Configuration des pins de microstepping
    pinMode(M0_PIN, OUTPUT);
    pinMode(M1_PIN, OUTPUT);
    pinMode(M2_PIN, OUTPUT);

    // Configuration pour 1/1 microstepping
    // M0=HIGH, M1=HIGH, M2=LOW = 1/16
    digitalWrite(M0_PIN, LOW); // HIGH
    digitalWrite(M1_PIN, LOW); // HIGH
    digitalWrite(M2_PIN, LOW);

    // Configuration du pin ENABLE (LOW = activé)
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);

    // Initialisation des moteurs
    motor1->begin();
    motor2->begin();

    Serial.println("Stepper Controller initialisé");
    Serial.printf("Microstepping: 1/%d\n", MICROSTEPS);
    Serial.printf("Pas par position: %d\n", STEPS_PER_REVOLUTION / TOTAL_POSITIONS);

    // Séquence de homing - retour à la position 0
    Serial.println("\n=== Séquence de Homing ===");

    Serial.print("Moteur 1 (Heures): ");
    motor1->homeToZero();

    Serial.print("Moteur 2 (Minutes): ");
    motor2->homeToZero();

    xTaskCreatePinnedToCore(
        motorTask,              // Fonction de la tâche
        "MotorTask",            // Nom (pour debug)
        4096,                   // Taille de stack (4KB suffisant)
        this,                   // Paramètre passé (pointeur vers cette instance)
        2,                      // Priorité (2 = haute, WiFi est à 1)
        &motorTaskHandle,       // Handle de la tâche
        0                       // Core 0 (Core 1 = WiFi)
    );

    Serial.println("Tâche moteur démarrée sur Core 0");

}

void StepperController::motorTask(void *parameter)
{
    StepperController *controller = (StepperController *)parameter;
    
    Serial.printf("Tâche moteur démarrée sur Core %d\n", xPortGetCoreID());
    
    while (true)
    {
        // Appeler update() en continu
        controller->update();
        
        // Yield au scheduler (évite de monopoliser le core)
        // Si aucun moteur ne bouge, cette pause évite de consommer du CPU inutilement
        vTaskDelay(1);  // 1 tick = 1ms minimum
    }
}


void StepperController::update()
{
    motor1->update();
    motor2->update();
}

void StepperController::setMotor1Position(uint8_t position)
{
    motor1->moveToPosition(position);
}

void StepperController::setMotor2Position(uint8_t position)
{
    motor2->moveToPosition(position);
}

uint8_t StepperController::getMotor1Position() const
{
    return motor1->getCurrentClockPosition();
}

uint8_t StepperController::getMotor2Position() const
{
    return motor2->getCurrentClockPosition();
}

uint8_t StepperController::getMotor1Minutes() const
{
    return motor1->getCurrentMinutes();
}

uint8_t StepperController::getMotor2Minutes() const
{
    return motor2->getCurrentMinutes();
}

bool StepperController::isMotor1Moving() const
{
    return motor1->getIsMoving();
}

bool StepperController::isMotor2Moving() const
{
    return motor2->getIsMoving();
}

void StepperController::stopAll()
{
    motor1->stop();
    motor2->stop();
}

void StepperController::enableMotors(bool enable)
{
    digitalWrite(ENABLE_PIN, enable ? LOW : HIGH);
}