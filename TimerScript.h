#include "Script.h"



class TimerScript : public Script
{

    using Script::Script;

public:

    TimerScript(GLFWwindow* window, World* world, Entity* ent, Entity* digitEnt[4]) : Script(window, world, ent)
    {
        digitEntities[0] = digitEnt[0];
        digitEntities[1] = digitEnt[1];
        digitEntities[2] = digitEnt[2];
        digitEntities[3] = digitEnt[3];
    }
    
    void startScript() override;

    void tickScript(float deltaTime) override;



private:
    
    float currentTime = 180.f;
    float tickRate = 1.f;
    float timeAccumulated = 0.f;
    Entity* digitEntities[4];
    bool running = false;
    void updateDisplay();
    std::vector<const char*> textures = {
        "Textures/7seg_0.png",
        "Textures/7seg_1.png",
        "Textures/7seg_2.png",
        "Textures/7seg_3.png",
        "Textures/7seg_4.png",
        "Textures/7seg_5.png",
        "Textures/7seg_6.png",
        "Textures/7seg_7.png",
        "Textures/7seg_8.png",
        "Textures/7seg_9.png"
    };
};