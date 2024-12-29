#pragma once

#include "Script.h"

class LightScript : public Script
{

    using Script::Script;

public:
   
    void startScript() override;

    void tickScript(float deltaTime) override;

    void openLight();
    
private:
  
    float minBrightness = 0.01f;
    
    bool running = false;
    bool reset = false;
   
};