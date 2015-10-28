#include "map.hpp"
#include <iostream>
#include <algorithm>
#include "render.hpp"
#include <string.h>
#include <fstream> 
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

#define WIERSZ_SIZE 1000

glm::vec3 Map::minVertex(10000.0f, 10000.0f, 10000.0f);
glm::vec3 Map::maxVertex(-10000.0f, -10000.0f, -10000.0f);

std::vector<Track> Map::InitMap(int argc, char * argv[]) {
    std::vector<Track> data;
    for (int i = 1; i < argc; i++) {
        data.push_back(Map::readFile(argv[i]));

        if (data[i-1].max.x > maxVertex.x) {
            maxVertex.x = data[i-1].max.x;
        }
        if (data[i-1].max.y > maxVertex.y) {
            maxVertex.y = data[i-1].max.y;
        }
        if (data[i-1].max.z > maxVertex.z) {
            maxVertex.z = data[i-1].max.z;
        }
        if (data[i-1].min.x < minVertex.x) {
            minVertex.x = data[i-1].min.x;
        }
        if (data[i-1].min.y < minVertex.y) {
            minVertex.y = data[i-1].min.y;
        }
        if (data[i-1].min.z < minVertex.z) {
            minVertex.z = data[i-1].min.z;
        }

    }
    printf("%d", argc);
    return data;
};

Track Map::readFile(char * arg) {
    std::vector<glm::vec3> oneFileData;

    FILE * infile;
    char wiersz [100];
    char *p;
    unsigned char latlon = 0;
    infile = fopen(arg, "r");
    if (infile == NULL) printf("Error opening file %s\n",arg);
    else {
        while (fgets(wiersz, 100, infile) != NULL) {
            float lat, lon, ele;
            if (strstr(wiersz, "<trkpt ")) {
                sscanf(strstr(wiersz, "lat=\"") + 5, "%f", &lat);
                sscanf(strstr(wiersz, "lon=\"") + 5, "%f", &lon);
                latlon = 1;
            } else if ((p = strstr(wiersz, "<ele>"))) {
                sscanf(p + 5, "%f</ele>", &ele);
                if (latlon == 1) {
                    glm::vec3 newVec(lat, lon, ele);
                    oneFileData.push_back(newVec);
                    latlon = 0;
                }
            }
        }
        fclose(infile);
    }
    return Track(oneFileData);
};
