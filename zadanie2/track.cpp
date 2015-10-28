#include "track.hpp"

Track::Track(std::vector<glm::vec3> arg) : coords(arg) {
    max = glm::vec3(-1000.0f, -1000.0f, -1000.0f);
    min = glm::vec3(1000.0f, 1000.0f, 1000.0f);

    for (unsigned int i = 0; i < coords.size(); i++) {
        if (coords[i].x > max.x) {
            max.x = coords[i].x;
        }
        if (coords[i].y > max.y) {
            max.y = coords[i].y;
        }
        if (coords[i].z > max.z) {
            max.z = coords[i].z;
        }
        if (coords[i].x < min.x) {
            min.x = coords[i].x;
        }
        if (coords[i].y < min.y) {
            min.y = coords[i].y;
        }
        if (coords[i].z < min.z) {
            min.z = coords[i].z;
        }
    }
};
