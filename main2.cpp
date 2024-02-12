#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "stb_image_write.h"
#include "voronoi.h"

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());

    std::vector<Vector2> points = {};
    const int width = 800; // change this to change the width of the picture
    const int height = 800; // change this to change the height of the picture
    int amoutPoints;
    float minDist;

    std::cout << "points: ";
    std::cin >> amoutPoints;
    std::cout << "min distance between points: ";
    std::cin >> minDist;

    std::uniform_int_distribution<std::mt19937::result_type> widthDist(0, width);
    std::uniform_int_distribution<std::mt19937::result_type> heightDist(0, height);
    // dist(rng)

    // generates points with a min distance
    for (int i = 0; i < amoutPoints;) {
        Vector2 pos(widthDist(rng), heightDist(rng));
        bool space = true;
        for (int j = 0; j < points.size(); j++) {
            if ((points[j] - pos).magnitude() < minDist) {
                space = false;
                break;
            }
        }
        if (!space)
            continue;
        points.push_back(pos);
        i++;
    }

    // duplicate all the points so it wraps nicely
    std::vector<Vector2> duplicatePoints;
    for (int i = 0; i < points.size(); i++) {
        duplicatePoints.push_back(Vector2(points[i].x, points[i].y - height));
        duplicatePoints.push_back(Vector2(points[i].x + width, points[i].y - height));
        duplicatePoints.push_back(Vector2(points[i].x + width, points[i].y));
        duplicatePoints.push_back(Vector2(points[i].x + width, points[i].y + height));
        duplicatePoints.push_back(Vector2(points[i].x, points[i].y + height));
        duplicatePoints.push_back(Vector2(points[i].x - width, points[i].y + height));
        duplicatePoints.push_back(Vector2(points[i].x - width, points[i].y));
        duplicatePoints.push_back(Vector2(points[i].x - width, points[i].y - height));
    }
    for (int i = 0; i < duplicatePoints.size(); i++)
        points.push_back(duplicatePoints[i]);
    duplicatePoints.clear();
    duplicatePoints.shrink_to_fit();

    char picture[height * width * 3];

    colorPartsVoronoi(points, picture, width, height, &eulerDist);
    char* eblPic = colorPartsToBL(picture, width, height, Color(255, 0, 0));
    colorPartsVoronoi(points, picture, width, height, &manhattenDist);
    char* mblPic = colorPartsToBL(picture, width, height, Color(0, 255, 0));
    colorPartsVoronoi(points, picture, width, height, &chebyshevDist);
    char* cblPic = colorPartsToBL(picture, width, height, Color(0, 0, 255));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            setColor(picture, x, y, width, getColor(eblPic, x, y, width) + getColor(mblPic, x, y, width) + getColor(cblPic, x, y, width));
        }
    }
    
    Color add = Color(0, 0, 255) + Color(255, 0, 0);
    std::cout << add.r << ", " << add.g << ", " << add.b << std::endl;
    std::cout << clamp(-1, 0, 100) << ", " << clamp(101, 0, 100) << std::endl;

    stbi_write_png("voronoi2.png", width, height, 3, picture, width * 3);
}