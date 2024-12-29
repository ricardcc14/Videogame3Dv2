#pragma once

#include "Script.h"

class BlockScript : public Script
{

    using Script::Script;

public:
    BlockScript(GLFWwindow* window, World* world, Entity* entity, float mass);
    void startScript() override;

    void tickScript(float deltaTime) override;
    
    float getMass();

    glm::vec2 getVelocity() const;
    void setVelocity(const glm::vec3& newVelocity);

    void applyGravity();

    bool isColliding(Entity* otherEntity);

private:

    float mass;
    glm::vec3 velocity;        
    glm::vec3 acceleration;   
    glm::vec3 forces;         
    bool onPlatform;
};