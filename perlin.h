#pragma once

#ifndef _PERLIN_H_
#define _PERLIN_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <functional>
#include "vectors.h"

// this is a funktion that just generates pseudo random vectors, but gets the same result for the same value
double hashCornerToAngle(Vector2 corner) {
    // Combine x and y into a single value
    size_t combined = std::hash<double>()(corner.x) ^ std::hash<double>()(corner.y);
    
    // Scale the combined hash to the range [0, 2*pi]
    double scaled = static_cast<double>(combined) / static_cast<double>(std::numeric_limits<size_t>::max());
    scaled *= 2 * M_PI; // 2 * pi
    
    return scaled;
}

class PerlinNoise2
{
public:
    PerlinNoise2(float _gridSize, int _layers) {
        gridSize = _gridSize;
        layers = _layers;
    }
    ~PerlinNoise2() {}

    float gridSize;
    int layers;

    double val(Vector2 pos)
    {
        double result = 0;
        for (int layer = 0; layer < layers; layer++) 
        {
            double layerGridSize = gridSize / pow(2, layer);
            double layerWeight = 1 / pow(2, layer);

            double x0 = ((int)(pos.x / layerGridSize)) * layerGridSize; // bottom left corner x
            double y0 = ((int)(pos.y / layerGridSize)) * layerGridSize; // bottom left corner y
            double x1 = x0 + layerGridSize; // top right corner x
            double y1 = y0 + layerGridSize; // top right corner y

            Vector2 s0 = angleToVec(hashCornerToAngle(Vector2(x0, y1)), 1);
            Vector2 s1 = angleToVec(hashCornerToAngle(Vector2(x1, y1)), 1);
            Vector2 s2 = angleToVec(hashCornerToAngle(Vector2(x0, y0)), 1);
            Vector2 s3 = angleToVec(hashCornerToAngle(Vector2(x1, y0)), 1);

            Vector2 pd0 = pos - Vector2(x0, y1);
            Vector2 pd1 = pos - Vector2(x1, y1);
            Vector2 pd2 = pos - Vector2(x0, y0);
            Vector2 pd3 = pos - Vector2(x1, y0);

            // these are the interpolation weights for the x and y corners
            double ix = (pos.x - x0) / layerGridSize;
            double iy = (pos.y - y0) / layerGridSize;

            double sc0 = s0 * pd0;
            double sc1 = s1 * pd1;
            double ix0 = smoothstep1(sc0, sc1, ix); // interpolated value of the top two corners

            double sc2 = s2 * pd2;
            double sc3 = s3 * pd3;
            double ix1 = smoothstep1(sc2, sc3, ix); // interpolated value of the bottom two corners

            double layerResult = smoothstep1(ix1, ix0, iy);
            result += layerResult * layerWeight;
        }
        return result;
    }
};

#endif