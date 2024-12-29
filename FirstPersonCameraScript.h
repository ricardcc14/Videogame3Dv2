#pragma once

#include "Script.h"
//#include "TimerScript.h"
#include "LightScript.h"

using namespace std;

class FirstPersonCameraScript : public Script
{

    using Script::Script;

public:
    FirstPersonCameraScript(GLFWwindow* window, World* world, Entity* ent, LightScript* lightScript) : Script(window, world, ent)
    {
        this->lightScript = lightScript;
    }

    void startScript() override;

    void tickScript(float deltaTime) override;

private:

    glm::vec3 direction = glm::vec3(1., 0., 0.);

    bool firstClick = true;
    float speed = 0.01f;
    float sensitivity = 100.0f;
    bool holdingObj = false;
    Entity* objHolded = nullptr;
    float holdingZoom = 10.0f;
    LightScript* lightScript;
    
  
    //TimerScript timerScript;
    
};