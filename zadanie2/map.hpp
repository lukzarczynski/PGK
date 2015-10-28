#ifndef GAME_HPP
#define GAME_HPP
#include <vector>
#include <glm/glm.hpp>
#include "track.hpp"

class Map {
private:
    Map() = delete; // static class
    static Track readFile(char * arg);
public:
    static std::vector<Track> InitMap(int argc, char * argv[]);
    static glm::vec3 minVertex;
    static glm::vec3 maxVertex;
};

#endif
