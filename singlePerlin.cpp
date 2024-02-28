#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "desmos.h"
#include "stb_image_write.h"
#include "vectors.h"
#include "perlin.h"
#include "voronoi.h"

int main()
{
    const int width = 800;
    const int height = 800;
    const float widthReal = 25;
    float pixlToxy = widthReal / ((float)width);

    PerlinNoise2 perlin(1, 1);

    char picture[width * height * 3];
    fillColor(picture, width, height, Color(0, 0, 0));

    int py = 0;
    for (float y = 0; y < height * pixlToxy && py < height; y += pixlToxy) 
    {
        int px = 0;
        for (float x = 0; x < widthReal && px < width; x += pixlToxy)
        {
            double val = (perlin.val(Vector2(x, y)) + 1) / 2;
            setColor(picture, px, py, width, Color(255, 255, 255) * val);
            px++;
        }
        py++;
    }

    writeImage("singlePerlin.png", picture, width, height);
}