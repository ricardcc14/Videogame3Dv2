#include "BlockScript.h"


BlockScript::BlockScript(GLFWwindow* window, World* world, Entity* entity, float mass)
    :Script(window, world, entity), mass(mass), velocity(0.f), acceleration(0.f), forces(0.f), onPlatform(false) {}

void BlockScript::startScript()
{
    velocity = glm::vec3(0.f);
    forces = glm::vec3(0.f);
    acceleration = glm::vec3(0.f);
    onPlatform = false;
}

void BlockScript::tickScript(float deltaTime)
{
    acceleration = forces / mass;
    velocity += acceleration * deltaTime;

    ComponentHandle<Transform3D> transform = entity->get<Transform3D>();
    transform->position += velocity * deltaTime;

    forces = glm::vec3(0.f);

    applyGravity();
    

}

bool BlockScript::isColliding(Entity* other_ent) {
    ComponentHandle<Transform3D> myTransform = entity->get<Transform3D>();
    ComponentHandle<CubeCollider> myCollider = entity->get<CubeCollider>();

    ComponentHandle<Transform3D> otherTransform = other_ent->get<Transform3D>();
    ComponentHandle<CubeCollider> otherCollider = other_ent->get<CubeCollider>();


    glm::vec3 myPos = myTransform->position;
    glm::vec3 otherPos = otherTransform->position;

    glm::vec3 myHalfSize = glm::vec3(myCollider->width, myCollider->height, myCollider->length) * 0.5f;
    glm::vec3 otherHalfSize = glm::vec3(otherCollider->width, otherCollider->height, otherCollider->length) * 0.5f;

    return (abs(myPos.x - otherPos.x) <= (myHalfSize.x + otherHalfSize.x) &&
        abs(myPos.y - otherPos.y) <= (myHalfSize.y + otherHalfSize.y) &&
        abs(myPos.z - otherPos.z) <= (myHalfSize.z + otherHalfSize.z));
}

void BlockScript::applyGravity() {
    forces += (0.f, -9.8f, 0.f);
}

float BlockScript::getMass() {
    return mass;
}