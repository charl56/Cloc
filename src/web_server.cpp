#include "web_server.h"
#include "laser_controller.h"
#include "config.h"
#include <SPIFFS.h>

WebServer::WebServer(StepperController *controller, LaserController *laserCtrl)
    : stepperController(controller), laserController(laserCtrl)
{
    server = new AsyncWebServer(WEB_SERVER_PORT);
}

WebServer::~WebServer()
{
    delete server;
}

void WebServer::begin()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("Erreur: Impossible de monter SPIFFS");
    }
    else
    {
        Serial.println("SPIFFS monté avec succès");
    }
    setupRoutes();
    server->begin();
    Serial.printf("Serveur web démarré sur le port %d\n", WEB_SERVER_PORT);
    Serial.printf("Begin webserver, position : %d \n", stepperController->getMotor1Minutes());

}

void WebServer::setupRoutes()
{

    // Static files
    server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    // API: Get status
    server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest *request)
               {
        String json = "{";
        json += "\"motor1\":" + String(stepperController->getMotor1Minutes()) + ",";
        json += "\"motor2\":" + String(stepperController->getMotor2Minutes()) + ",";
        json += "\"motor1Moving\":" + String(stepperController->isMotor1Moving() ? "true" : "false") + ",";
        json += "\"motor2Moving\":" + String(stepperController->isMotor2Moving() ? "true" : "false");
        json += "}";
        
        Serial.printf("/api/status, position : %d \n", String(stepperController->getMotor1Minutes()));

        request->send(200, "application/json", json); });

        

    // API: Set motor 1 position
    server->on("/api/motor1", HTTP_POST, [this](AsyncWebServerRequest *request)
               {
        if (request->hasParam("position", true)) {
            int pos = request->getParam("position", true)->value().toInt();
            if (pos >= 0 && pos < TOTAL_POSITIONS) {
                stepperController->setMotor1Position(pos);
                request->send(200, "application/json", "{\"success\":true}");
            } else {
                request->send(400, "application/json", "{\"error\":\"Invalid position\"}");
            }
        } else {
            request->send(400, "application/json", "{\"error\":\"Missing position parameter\"}");
        } });

    // API: Set motor 2 position
    server->on("/api/motor2", HTTP_POST, [this](AsyncWebServerRequest *request)
               {
        if (request->hasParam("position", true)) {
            int pos = request->getParam("position", true)->value().toInt();
            if (pos >= 0 && pos < TOTAL_POSITIONS) {
                stepperController->setMotor2Position(pos);
                request->send(200, "application/json", "{\"success\":true}");
            } else {
                request->send(400, "application/json", "{\"error\":\"Invalid position\"}");
            }
        } else {
            request->send(400, "application/json", "{\"error\":\"Missing position parameter\"}");
        } });

    // API: Stop all motors
    server->on("/api/stop", HTTP_POST, [this](AsyncWebServerRequest *request)
               {
        stepperController->stopAll();
        request->send(200, "application/json", "{\"success\":true}"); });

    // API: Set laser 1 position
    server->on("/api/laser/{id}", HTTP_POST, [this](AsyncWebServerRequest *request)
               {
        AsyncWebParameter* p = request->getParam("id");
        String laserId = p->value();

        int id = laserId.toInt();

        switch(id){
            case 1:
                laserController->getLaser1()->toggle();
                break;
            case 2:
                laserController->getLaser2()->toggle();
                break;
            default:
                request->send(400, "application/json", "{\"error\":\"Invalid laser ID\"}");
                return;
        }
        
        request->send(200, "application/json", "{\"success\":true}"); });
}
