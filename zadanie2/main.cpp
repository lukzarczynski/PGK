#include <cstdlib>
#include <ctime>
#include <iostream>
#include "map.hpp"
#include "render.hpp"
#include "track.hpp"

int main(int argc, char * argv[]) {

    std::vector<Track> files = Map::InitMap(argc, argv);

    int n = Render::Init(files);
    if (n) return n;

    do {
        Render::Frame();
    } while (!Render::IsKeyPressed(GLFW_KEY_ESCAPE) && !Render::IsWindowClosed());

    Render::CleanUp();

    return 0;
}
