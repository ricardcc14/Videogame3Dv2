#include "TimerScript.h"

void TimerScript::startScript()
{
    
}

void TimerScript::tickScript(float deltaTime)
{
    
    if (!running && (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)) {
       running = true;
    }

    if (running) {

        timeAccumulated += deltaTime * 0.001f;

        if (timeAccumulated >= tickRate) {
            timeAccumulated -= tickRate;
    
            if (currentTime > 0.0f) {
                currentTime -= 1.0f;
                updateDisplay();
            }
            else {
                running = false;
                updateDisplay();
            }

            
            
        }
    }
}
void TimerScript::updateDisplay() {
    int totalSec = static_cast<int>(currentTime);
    int min = totalSec / 60;
    int sec = totalSec % 60;

    int digit1 = min / 10;
    int digit2 = min % 10;
    int digit3 = sec / 10;
    int digit4 = sec % 10;

    ComponentHandle<Sprite> digit1_sprite = digitEntities[0]->get<Sprite>();
    ComponentHandle<Sprite> digit2_sprite = digitEntities[1]->get<Sprite>();
    ComponentHandle<Sprite> digit3_sprite = digitEntities[2]->get<Sprite>();
    ComponentHandle<Sprite> digit4_sprite = digitEntities[3]->get<Sprite>();

    digit1_sprite->filepath = textures[digit1];
    digit2_sprite->filepath = textures[digit2];
    digit3_sprite->filepath = textures[digit3];
    digit4_sprite->filepath = textures[digit4];

}
