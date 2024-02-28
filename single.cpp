#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "stb_image_write.h"
#include "voronoi.h"

int main() {
    const int width = 800;
    const int height = 800;
    int numOfPoints = 100;
    int minDistance = 0;

    std::vector<Vector2> seeds = generateRandPoints(width, height, numOfPoints, minDistance);

    char picture[width * height * 3];
    colorPartsVoronoi(seeds, picture, width, height, &parabularDist);
    char* blPic = colorPartsToBL(picture, width, height, Color(0, 0, 0));
    writeImage("singleVoroni.png", blPic, width, height);
}