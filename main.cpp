#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "stb_image_write.h"
#include "voronoi.h"

float max(float val1, float val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

float euler(Vector2 pos1, Vector2 pos2) {
    return (pos2 - pos1).magnitude();
}
float sqrtAvr(Vector2 pos1, Vector2 pos2) {
    return sqrt(abs(pos2.x - pos1.x)*abs(pos2.y - pos1.y));
}
float manhatten(Vector2 pos1, Vector2 pos2) {
    return abs(pos2.x - pos1.x) + abs(pos2.y - pos1.y);
}
float chebyshev(Vector2 pos1, Vector2 pos2) {
    Vector2 res = pos2 - pos1;
    return max(abs(res.x), abs(res.y));
}

int main()
{
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

    char* blPic = (char*)std::malloc(height * width * 3);

    char pictureEuler[height * width * 3];
    colorPartsVoronoi(points, pictureEuler, width, height, &euler);
    blPic = colorPartsToBL(pictureEuler, width, height, Color(0, 0, 0));

    char pictureManhatten[height * width * 3];
    colorPartsVoronoi(points, pictureManhatten, width, height, &manhatten);
    char* mblPic = colorPartsToBL(pictureManhatten, width, height, Color(255, 0, 0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (getColor(blPic, x, y, width) == Color(255, 255, 255))
                setColor(blPic, x, y, width, getColor(mblPic, x, y, width));
            else
                setColor(blPic, x, y, width, getColor(blPic, x, y, width) + getColor(mblPic, x, y, width));
        }
    }
    
    char pictureChebyshev[height * width * 3];
    colorPartsVoronoi(points, pictureChebyshev, width, height, &chebyshev);
    mblPic = colorPartsToBL(pictureChebyshev, width, height, Color(0, 255, 0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (getColor(blPic, x, y, width) == Color(255, 255, 255))
                setColor(blPic, x, y, width, getColor(mblPic, x, y, width));
            else
                setColor(blPic, x, y, width, getColor(blPic, x, y, width) + getColor(mblPic, x, y, width));
        }
    }

    stbi_write_png("voronoi.png", width, height, 3, blPic, width * 3);
    return 0;
}