#pragma once

#include "Script.h"

class LightScript : public Script
{

    using Script::Script;

public:
   
    void startScript() override;

    void tickScript(float deltaTime) override;

    void openLight();
    float accTime = 0.f;
    float brightness = 1.f;
    
private:
  
    float minBrightness = 0.01f;
    
    bool running = false;
    bool reset = false;
   
};