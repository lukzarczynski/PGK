#ifndef CONTROLS_HPP
#define CONTROLS_HPP

class Controls {
public:
    static glm::vec3 initial;
    static void computeMatricesFromInputs(GLFWwindow* window);
    static glm::mat4 getViewMatrix();
    static glm::mat4 getProjectionMatrix();
    static void reset();
};

#endif