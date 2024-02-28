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

#define str(val) std::to_string(val)

int main() {
    DesFile file("perlinHashTest.html");

    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 25; x++) {
            double alpha = hashCornerToAngle(Vector2(x, y));
            Vector2 vec = angleToVec(alpha, 0.5);
            file.addExpr("X = ("+str(x)+","+str(y)+") + ("+str(vec.x)+","+str(vec.y)+") * t", "black");
        }
    }
}