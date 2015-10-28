#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "track.hpp"
#include "map.hpp"

class Render {
private:
    Render() = delete; // static class
public:
    static int Init(std::vector<Track> arg);
    static void Frame();
    static void CleanUp();
    static double GetTime();
    static bool IsKeyPressed(int key);
    static bool IsWindowClosed();
};


#endif //RENDER_HPP
