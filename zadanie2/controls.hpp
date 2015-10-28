#ifndef CONTROLS_HPP
#define CONTROLS_HPP

class Controls {
public:
    static glm::vec3 initial;
    static void computeTranslateAndScale();
    static glm::vec3 getPosition();
    static void reset();
};

#endif