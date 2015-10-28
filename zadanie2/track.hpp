#ifndef TRACK_HPP
#define	TRACK_HPP

#include <vector>
#include <glm/glm.hpp>

class Track {
public:
    Track(std::vector<glm::vec3> coords);
    std::vector<glm::vec3> coords;
    glm::vec3 min;
    glm::vec3 max;
};


#endif	/* GPS_HPP */

