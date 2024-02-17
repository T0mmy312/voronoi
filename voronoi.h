#pragma once

#ifndef _VORONOI_H_
#define _VORONOI_H_

#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "vectors.h"
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

//? ----------------------------------------------------------------------------------------------------
//? Distance calculation funktions
//? ----------------------------------------------------------------------------------------------------

float eulerDist(Vector2 pos1, Vector2 pos2) {
    return (pos2 - pos1).magnitude();
}
float sqrtAvrDist(Vector2 pos1, Vector2 pos2) {
    return sqrt(abs(pos2.x - pos1.x)*abs(pos2.y - pos1.y));
}
float manhattenDist(Vector2 pos1, Vector2 pos2) {
    return abs(pos2.x - pos1.x) + abs(pos2.y - pos1.y);
}
float chebyshevDist(Vector2 pos1, Vector2 pos2) {
    Vector2 res = pos2 - pos1;
    return max(abs(res.x), abs(res.y));
}

//? ----------------------------------------------------------------------------------------------------
//? Edge class
//? ----------------------------------------------------------------------------------------------------

class Edge
{
public:
    Edge(Vector2 _a, Vector2 _b) {
        a = _a;
        b = _b;
    }
    ~Edge() {}

    Vector2 a;
    Vector2 b;
};

//? ----------------------------------------------------------------------------------------------------
//? Intersection classes
//? ----------------------------------------------------------------------------------------------------

class intersection 
{
public:
    intersection(Vector2 _point = Vector2(), bool _valid = false) {
        point = _point;
        valid = _valid;
    }
    ~intersection() {}

    Vector2 point;
    bool valid;
};

class doublei // double intersection
{
public:
    doublei(intersection _i1 = intersection(), intersection _i2 = intersection()) {
        i1 = _i1;
        i2 = _i2;
    }
    ~doublei() {}

    intersection i1;
    intersection i2;
};

//? ----------------------------------------------------------------------------------------------------
//? Parabular class
//? ----------------------------------------------------------------------------------------------------

class Parabular
{
public:
    Parabular(float _a = 0, float _b = 0, float _c = 0) {
        a = _a;
        b = _b;
        c = _c;
    }
    Parabular(Vector2 seed, float sweepLineY) {
        float twoSweepYDifSeedY = 2 * (sweepLineY - seed.y);
        a = 1 / twoSweepYDifSeedY;
        b = (2 * seed.x) / twoSweepYDifSeedY;
        c = (sweepLineY*sweepLineY - seed.x*seed.x - seed.y*seed.y) / twoSweepYDifSeedY;
    }
    ~Parabular() {}

    // formular for a Parabular y = a*x^2 + b*x + c
    float a;
    float b;
    float c;

    float calc(float x) {
        return a * x*x + b * x + c;
    }

    bool operator==(Parabular other) {
        return a == other.a && b == other.b && c == other.c;
    }
};

//! may not work, not yet tested
doublei intersect(Parabular p1, Parabular p2) {
    float twoA = 2 * (p1.a - p2.a);
    if (twoA == 0)
        return doublei();
    float sqrtCon = sq(p1.b - p2.b) - 4 * (p1.a - p2.a) * (p1.c - p2.c);
    if (sqrtCon < 0)
        return doublei();
    if (sqrtCon == 0) {
        float x = (-(p1.b - p2.b)) / twoA;
        return doublei(intersection(Vector2(x, p1.calc(x)), true), intersection());
    }
    float x1 = (-(p1.b - p2.b) + sqrt(sqrtCon)) / twoA;
    float x2 = (-(p1.b - p2.b) - sqrt(sqrtCon)) / twoA;
    return doublei(intersection(Vector2(x1, p1.calc(x1)), true), intersection(Vector2(x2, p1.calc(x2)), true));
}

std::vector<int> parMin(std::vector<Parabular> pars, int xStart, int xEnd) { // [xStart, xEnd)
    std::vector<int> ret = {};
    if (pars.empty())
        return {};
    for (int x = xStart; x < xEnd; x++) {
        int min = (int)pars[0].calc(x);
        for(int i = 1; i < pars.size(); i++) {
            int val = (int)pars[i].calc(x);
            if (val < min)
                min = val;
        }
        ret.push_back(min);
    }
    return ret;
}

//? ----------------------------------------------------------------------------------------------------
//? Color class
//? ----------------------------------------------------------------------------------------------------

class Color
{
public:
    Color(int _r = 0, int _g = 0, int _b = 0) {
        r = _r;
        g = _g;
        b = _b;
    }
    ~Color() {}

    int r;
    int g;
    int b;

    bool operator==(Color other) {
        return r == other.r && g == other.g &&  b == other.b;
    }
    Color operator+(Color other) {
        return Color(clamp(r + other.r, 0, 255), clamp(g + other.g, 0, 255), clamp(b + other.b, 0, 255));
    }
    Color operator*(float other) {
        return Color(clamp(r * other, 0, 255), clamp(g * other, 0, 255), clamp(b * other, 0, 255));
    }
};

//? ----------------------------------------------------------------------------------------------------
//? color funktions
//? ----------------------------------------------------------------------------------------------------

Color intColor(int val) {
    Color ret;
    ret.b = val % 255;
    val /= 255;
    ret.g = val % 255;
    val /= 255;
    ret.r = val % 255;
    return ret;
}

void setColor(char picture[], int x, int y, int width, Color color) {
    picture[y * width * 3 + x * 3] = color.r;
    picture[y * width * 3 + x * 3 + 1] = color.g;
    picture[y * width * 3 + x * 3 + 2] = color.b;
}

Color getColor(char picture[], int x, int y, int width) {
    Color ret = Color(0, 0, 0);
    ret.r = picture[y * width * 3 + x * 3];
    ret.g = picture[y * width * 3 + x * 3 + 1];
    ret.b = picture[y * width * 3 + x * 3 + 2];
    return ret;
}

void fillColor(char picture[], int width, int height, Color color) {
    for (int row = 0; row < height; row++)
        for (int col = 0; col < width; col++)
            setColor(picture, col, row, width, color);
}

bool surroundEq(char picture[], int x, int y, int width, int height, Color currColor) {
    bool rightEdge = x == width - 1;
    bool leftEdge = x == 0;
    bool topEdge = y == 0;
    bool bottomEdge = y == height - 1;
    return  (!topEdge ? (getColor(picture, x, y - 1, width) == currColor) : true) && 
            (!rightEdge && !topEdge ? (getColor(picture, x + 1, y - 1, width) == currColor) : true) && 
            (!rightEdge  ? (getColor(picture, x + 1, y, width) == currColor) : true) &&
            (!rightEdge && !bottomEdge ? (getColor(picture, x + 1, y + 1, width) == currColor) : true) &&
            (!bottomEdge ? (getColor(picture, x, y + 1, width) == currColor) : true) &&
            (!bottomEdge && !leftEdge ? (getColor(picture, x - 1, y + 1, width) == currColor) : true) &&
            (!leftEdge ? (getColor(picture, x - 1, y, width) == currColor) : true) &&
            (!leftEdge && !topEdge ? (getColor(picture, x - 1, y - 1, width) == currColor) : true);
}

//? ----------------------------------------------------------------------------------------------------
//? generate points
//? ----------------------------------------------------------------------------------------------------

std::vector<Vector2> generateRandPoints(int width, int height, int amount, int minDistance) 
{
    std::vector<Vector2> points = {};

    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> widthDist(0, width);
    std::uniform_int_distribution<std::mt19937::result_type> heightDist(0, height);

    for (int i = 0; i < amount;) {
        Vector2 pos(widthDist(rng), heightDist(rng));
        bool space = true;
        for (int j = 0; j < points.size(); j++) {
            if ((points[j] - pos).magnitude() < minDistance) {
                space = false;
                break;
            }
        }
        if (!space)
            continue;
        points.push_back(pos);
        i++;
    }

    return points;
}

//! isn't working properly
std::vector<Vector2> wrapPoints(std::vector<Vector2> seeds, int width, int height) 
{
    std::vector<Vector2> points = {};

    std::vector<Vector2> duplicatePoints = {};
    for (int i = 0; i < seeds.size(); i++) {
        duplicatePoints.push_back(Vector2(seeds[i].x, seeds[i].y - height));
        duplicatePoints.push_back(Vector2(seeds[i].x + width, seeds[i].y - height));
        duplicatePoints.push_back(Vector2(seeds[i].x + width, seeds[i].y));
        duplicatePoints.push_back(Vector2(seeds[i].x + width, seeds[i].y + height));
        duplicatePoints.push_back(Vector2(seeds[i].x, seeds[i].y + height));
        duplicatePoints.push_back(Vector2(seeds[i].x - width, seeds[i].y + height));
        duplicatePoints.push_back(Vector2(seeds[i].x - width, seeds[i].y));
        duplicatePoints.push_back(Vector2(seeds[i].x - width, seeds[i].y - height));
    }
    for (int i = 0; i < duplicatePoints.size(); i++)
        points.push_back(duplicatePoints[i]);
    
    return points;
}

//? ----------------------------------------------------------------------------------------------------
//? voronoi implementation
//? ----------------------------------------------------------------------------------------------------

/*
// this is an implementation of Fortune's algorithm
// top left and bottom right are the boundaries
// TODO: Check how to find line Edges think it something to do with seting the sweep line to a point
// the intersection of all the Parabulars is the min of all of the Parabulars
std::vector<Edge> voronoi(std::vector<Vector2> seeds, Vector2 topLeft, Vector2 bottomRight)
{
    float sweepY = topLeft.y;
}
*/

// this is an stb_image_write picture (rgb)
int voronoiBW(char picture[], int pictureWidth, int pictureHeight, std::vector<Vector2> seeds, Vector2 topLeft, Vector2 bottomRight, Color lineColor = Color(0, 0, 0), Color backgroundColor = Color(255, 255, 255)) // black white picture of the voronoi diagram
{
    fillColor(picture, pictureWidth, pictureHeight, backgroundColor);
    std::vector<Parabular> pars = {}; // list of parabulars
    for (int sweepY = topLeft.y; sweepY <= bottomRight.y; sweepY++) {
        pars.clear();
        pars.shrink_to_fit();
        for (int i = 0; i < seeds.size(); i++) {
            if (seeds[i].y >= sweepY) 
                pars.push_back(Parabular(seeds[i], sweepY));
        }
        for (int i = 0; i < pars.size(); i++) {
            for (int j = i + 1; j < pars.size(); j++) { // j = i + 1 is because it has intersected with the ones before already
                if (pars[i] == pars[j]) // checks if its the same
                    continue;
                doublei intersections = intersect(pars[i], pars[j]);
                if (intersections.i1.valid) {
                    if (intersections.i1.point.y > 0 && intersections.i1.point.y < pictureHeight &&
                        intersections.i1.point.x > 0 && intersections.i1.point.x < pictureWidth) {
                        setColor(picture, (int)intersections.i1.point.x, (int)intersections.i1.point.y, pictureWidth, lineColor);
                    }
                }
                if (intersections.i2.valid) {
                    if (intersections.i2.point.y > 0 && intersections.i2.point.y < pictureHeight &&
                        intersections.i2.point.x > 0 && intersections.i2.point.x < pictureWidth) {
                        setColor(picture, (int)intersections.i2.point.x, (int)intersections.i2.point.y, pictureWidth, lineColor);
                    }
                }
            }
        }
    }
    return 0;
}

int voronoiMin(char picture[], int pictureWidth, int pictureHeight, std::vector<Vector2> seeds, Vector2 topLeft, Vector2 bottomRight, Color lineColor = Color(0, 0, 0), Color backgroundColor = Color(255, 255, 255)) // black white picture of the voronoi diagram
{
    fillColor(picture, pictureWidth, pictureHeight, backgroundColor);
    std::vector<Parabular> pars = {}; // list of parabulars
    for (int sweepY = topLeft.y; sweepY <= bottomRight.y; sweepY++) {
        pars.clear();
        pars.shrink_to_fit();
        for (int i = 0; i < seeds.size(); i++) {
            if (seeds[i].y >= sweepY) 
                pars.push_back(Parabular(seeds[i], sweepY));
        }
        std::vector<int> minArray = parMin(pars, topLeft.x, bottomRight.x);
        // now we just need to place black pixels at local min
        for(int i = 0; i < minArray.size(); i++) {
            if (i == 0) {
                if (i == minArray.size() - 1)
                    break;
                if (minArray[i + 1] > minArray[i])
                    setColor(picture, topLeft.x + i, minArray[i], pictureWidth, lineColor);
            }
            else if (i == minArray.size() - 1) {
                if (minArray[i - 1] > minArray[i])
                    setColor(picture, topLeft.x + i, minArray[i], pictureWidth, lineColor);
            }
            else {
                if (minArray[i - 1] > minArray[i] && minArray[i + 1] > minArray[i])
                    setColor(picture, topLeft.x + i, minArray[i], pictureWidth, lineColor);
            }
            
        }
    }
    return 0;
}

int colorPartsVoronoi(std::vector<Vector2> seeds, char picture[], int pictureWidth, int pictureHeight, float (*distance)(Vector2, Vector2))
{
    fillColor(picture, pictureWidth, pictureHeight, Color(255, 255, 255));
    for (int y = 0; y < pictureHeight; y++)
    {
        for (int x = 0; x < pictureWidth; x++)
        {
            int minI = 0;
            float minDist = (*distance)(seeds[0], Vector2(x, y));
            for (int i = 1; i < seeds.size(); i++) {
                float dist = (*distance)(seeds[i], Vector2(x, y));
                if (dist < minDist) {
                    minI = i;
                    minDist = dist;
                }
            }
            setColor(picture, x, y, pictureWidth, intColor(minI));
        }
    }
    return 0;
}

char* colorPartsToBL(char picture[], int width, int height, Color lineColor = Color(0, 0, 0)) // returns a pointer to a picture
{
    char* blPic = (char*)std::malloc(width * height * 3);
    fillColor(blPic, width, height, Color(255, 255, 255));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color curr = getColor(picture, x, y, width);
            if (!surroundEq(picture, x, y, width, height, curr))
                setColor(blPic, x, y, width, lineColor);
        }
    }
    return blPic;
}

void writeImage(const char filename[], char picture[], int width, int height) {
    stbi_write_png(filename, width, height, 3, picture, width * 3);
}

#endif