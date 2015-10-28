#include "render.hpp"
#include <iostream>
#include "shader.hpp"
#include "controls.hpp"

GLFWwindow* window;

std::vector<Track> track;
std::vector<GLuint> vertexbuffers;

GLuint VertexArrayID;
GLuint programID;
GLuint MatrixID;
GLuint minZ;
GLuint maxZ;
GLuint trackID;

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
    track = arg;
    vertexbuffers.resize(track.size());

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
    window = glfwCreateWindow(1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
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

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Map.vertexshader", "Map.fragmentshader");

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    minZ = glGetUniformLocation(programID, "minZ");
    maxZ = glGetUniformLocation(programID, "maxZ");
    trackID = glGetUniformLocation(programID, "trackID");

    glGenBuffers(track.size(), &vertexbuffers[0]);
    for (unsigned int i = 0; i < track.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, track[i].coords.size() * sizeof (glm::vec3), &track[i].coords[0], GL_STATIC_DRAW);
    }

    Controls::initial = glm::vec3(
            (Map::minVertex.x + Map::maxVertex.x) / 2,
            (Map::minVertex.y + Map::maxVertex.y) / 2,
            std::max(((Map::maxVertex.x - Map::minVertex.x)),
            ((Map::maxVertex.y - Map::minVertex.y))));


    printf("\ninitial: x= %f, y=%f, z=%f", Controls::initial.x, Controls::initial.y, Controls::initial.z);
    printf("\nmin: x= %f, y=%f, z=%f", Map::minVertex.x, Map::minVertex.y, Map::minVertex.z);
    printf("\nmax: x= %f, y=%f, z=%f", Map::maxVertex.x, Map::maxVertex.y, Map::maxVertex.z);
    
    Controls::reset();
    return 0;
}

void Render::Frame() {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);


    glUniform1f(minZ, Map::minVertex.z);
    glUniform1f(maxZ, Map::maxVertex.z);


    Controls::computeMatricesFromInputs(window);

    glm::mat4 ProjectionMatrix = Controls::getProjectionMatrix();
    glm::mat4 ViewMatrix = Controls::getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);

    for (unsigned int i = 0; i < track.size(); i++) {

        glUniform2f(trackID, (float) i, (float) track.size());

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[i]);
        glVertexAttribPointer(
                0.0f, // attribute. No particular reason for 0.0f, but must match the layout in the shader.
                3, // size
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0.0f, // stride
                (void*) 0 // array buffer offset
                );

        glDrawArrays(GL_LINE_STRIP, 0.0f, track[i].coords.size());

    }
    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

}

void Render::CleanUp() {
    // Cleanup VBO and shader
    for (unsigned int i = 0; i < track.size(); i++) {
        glDeleteBuffers(1, &vertexbuffers[i]);
    }
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}
