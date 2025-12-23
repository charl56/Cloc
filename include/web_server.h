#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include "stepper_controller.h"
#include "laser_controller.h"

class WebServer {
private:
    AsyncWebServer* server;
    StepperController* stepperController;
    LaserController* laserController;
    
    void setupRoutes();
    
public:
    WebServer(StepperController* controller, LaserController* laserCtrl);
    ~WebServer();
    
    void begin();
};

#endif // WEB_SERVER_H