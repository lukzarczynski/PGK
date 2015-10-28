#include "render.hpp"
#include <iostream>
#include "shader.hpp"
#include "controls.hpp"

GLFWwindow* window;

std::vector<Track> tracks;
std::vector<GLuint> vertexbuffers;

GLuint VertexArrayID;
GLuint programID;
GLuint posVec;
GLuint minZ;
GLuint maxZ;
GLuint blue;

double Render::GetTime() {
    return glfwGetTime();
}

bool Render::IsKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Render::IsWindowClosed() {
    return (glfwWindowShouldClose(window) == 1);
}

float map(float x, float a, float b, float ap, float bp) {
    return ((x - a) / (b - a)) * (bp - ap) + ap;
}

int Render::Init(std::vector<Track> arg) {
    tracks = arg;
    vertexbuffers.resize(tracks.size());

    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(680, 480, "Zadanie 2 - GPS", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Map.vertexshader", "Map.fragmentshader");

    // Get a handle for our "MVP" uniform
    posVec = glGetUniformLocation(programID, "posVec");
    blue = glGetUniformLocation(programID, "blue");

    // scale 
    for (unsigned int i = 0; i < tracks.size(); i++) {
        for (unsigned int j = 0; j < tracks[i].coords.size(); j++) {
            tracks[i].coords[j].z = map(tracks[i].coords[j].z, Map::minVertex.z, Map::maxVertex.z, 0.0f, 2.0f);
        }
    }

    //Bind buffers
    glGenBuffers(tracks.size(), &vertexbuffers[0]);
    for (unsigned int i = 0; i < tracks.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, tracks[i].coords.size() * sizeof (glm::vec3), &tracks[i].coords[0], GL_STATIC_DRAW);
    }

    float xlen = Map::maxVertex.x - Map::minVertex.x;
    float ylen = Map::maxVertex.y - Map::minVertex.y;

    float scale = 1 / (std::max(xlen, ylen) / 2);
    float translateX = -(((xlen / 2) + Map::minVertex.x) * scale);
    float translateY = -(((ylen / 2) + Map::minVertex.y) * scale);

    Controls::initial = glm::vec3(translateX, translateY, scale);

    //    printf("\ninitial: x= %f, y=%f, z=%f", Controls::initial.x, Controls::initial.y, Controls::initial.z);
    //    printf("\nmin: x= %f, y=%f, z=%f", Map::minVertex.x, Map::minVertex.y, Map::minVertex.z);
    //    printf("\nmax: x= %f, y=%f, z=%f", Map::maxVertex.x, Map::maxVertex.y, Map::maxVertex.z);

    Controls::reset();
    return 0;
}

void Render::Frame() {
    //    printf("\npos: x= %f, y=%f, z=%f", Controls::getPosition().x, Controls::getPosition().y, Controls::getPosition().z);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programID);

    Controls::computeTranslateAndScale();
    glUniform3fv(posVec, 1, &Controls::getPosition()[0]);

    glEnableVertexAttribArray(0);

    for (unsigned int i = 0; i < tracks.size(); i++) {

        glUniform1f(blue, (float) (i / tracks.size()));

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[i]);
        glVertexAttribPointer(
                0, // attribute.
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0.0f, // stride
                (void*) 0 // array buffer offset
                );

        glDrawArrays(GL_LINE_STRIP, 0, tracks[i].coords.size());

    }
    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

}

void Render::CleanUp() {
    // Cleanup VBO and shader
    for (unsigned int i = 0; i < tracks.size(); i++) {
        glDeleteBuffers(1, &vertexbuffers[i]);
    }
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}
