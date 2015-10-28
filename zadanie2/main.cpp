#include <cstdlib>
#include <ctime>
#include <iostream>
#include "map.hpp"
#include "render.hpp"
#include "track.hpp"

// Cursor coordinates. These are also used by Render::Frame()... :(
unsigned int selection_x = 0, selection_y = 0;

int main(int argc, char * argv[]) {

    // Reset game stats.
    std::vector<Track> files = Map::InitMap(argc, argv);

    // Prepare the renderer.
    int n = Render::Init(files);
    if (n) return n;

    // This is the main loop.
    do {
        // Draw a single frame.
        Render::Frame();
        // If the ESC key was pressed or the window was closed, break the loop
    } while (!Render::IsKeyPressed(GLFW_KEY_ESCAPE) && !Render::IsWindowClosed());

    // Delete buffers, remove the window.
    Render::CleanUp();

    return 0;
}
