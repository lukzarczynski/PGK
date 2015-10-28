#include "render.hpp"
#include <iostream>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "controls.hpp"

glm::vec3 Controls::initial;
static glm::vec3 position = Controls::initial;
static float speed = 3.0f; // 3 units / second
static float scaleSpeed = 0.1f; // 10% / second

glm::vec3 Controls::getPosition() {
    return position;
}

void Controls::reset() {
    position = Controls::initial;
}

void Controls::computeTranslateAndScale() {

    static double lastTime = Render::GetTime();
    double currentTime = Render::GetTime();
    float deltaTime = float(currentTime - lastTime);

    float moveSpeed = speed * deltaTime;
    if (Controls::getPosition().z < 1.0f) {
        moveSpeed *= Controls::getPosition().z;
    }

    // Move forward
    if (Render::IsKeyPressed(GLFW_KEY_A)) {
        position.z = position.z + (position.z * scaleSpeed * deltaTime);
        position.x = position.x + (position.x * scaleSpeed * deltaTime);
        position.y = position.y + (position.y * scaleSpeed * deltaTime);
    }
    // Move backward
    if (Render::IsKeyPressed(GLFW_KEY_B)) {
        position.z = position.z - (position.z * scaleSpeed * deltaTime);
        position.x = position.x - (position.x * scaleSpeed * deltaTime);
        position.y = position.y - (position.y * scaleSpeed * deltaTime);
    }
    //move up
    if (Render::IsKeyPressed(GLFW_KEY_UP)) {
        position.y -= moveSpeed;
    }
    // Move down
    if (Render::IsKeyPressed(GLFW_KEY_DOWN)) {
        position.y += moveSpeed;
    }
    // Strafe right
    if (Render::IsKeyPressed(GLFW_KEY_RIGHT)) {
        position.x -= moveSpeed;
    }
    // Strafe left
    if (Render::IsKeyPressed(GLFW_KEY_LEFT)) {
        position.x += moveSpeed;
    }
    // reset 
    if (Render::IsKeyPressed(GLFW_KEY_R)) {
        Controls::reset();
    }

    lastTime = currentTime;
}