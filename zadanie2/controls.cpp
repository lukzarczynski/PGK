// Include GLFW
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"




// Initial position : on +Z
glm::vec3 Controls::initial = glm::vec3(51.1f, 16.9f, 1);

static glm::vec3 position = Controls::initial;
// Initial horizontal angle : toward -Z
static float horizontalAngle = 3.14f;
// Initial vertical angle : none
static float verticalAngle = 0.0f;
// Initial Field of View
static float initialFoV = 45.0f;
static glm::mat4 ViewMatrix;
static glm::mat4 ProjectionMatrix;

static float speed = 3.0f; // 3 units / second

glm::mat4 Controls::getViewMatrix() {
    return ViewMatrix;
}

glm::mat4 Controls::getProjectionMatrix() {
    return ProjectionMatrix;
}

void Controls::reset() {
    position = Controls::initial;
}

void Controls::computeMatricesFromInputs(GLFWwindow* window) {

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
            );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
            );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    //move up
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        position += up * deltaTime * speed;
    }
    // Move down
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position -= up * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }
    // reset 
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        Controls::reset();
    }

    float FoV = initialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
            position, // Camera is here
            position + direction, // and looks here : at the same position, plus "direction"
            up // Head is up (set to 0,-1,0 to look upside-down)
            );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}