#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include "stepper_controller.h"

class WebServer {
private:
    AsyncWebServer* server;
    StepperController* stepperController;
    
    void setupRoutes();
    String getIndexHTML();
    
public:
    WebServer(StepperController* controller);
    ~WebServer();
    
    void begin();
};

#endif // WEB_SERVER_H