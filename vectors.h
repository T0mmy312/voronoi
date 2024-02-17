#pragma once

#ifndef _VECTORS_H_
#define _VECTORS_H_

#include <math.h>

//? ----------------------------------------------------------------------------------------------------
//? Sinple math funktions
//? ----------------------------------------------------------------------------------------------------

float sq(float x) {
    return x*x;
}

float clamp(float x, float min, float max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

float max(float val1, float val2) {
    if (val1 >= val2)
        return val1;
    return val2;
}

// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is linear
double lerp(double val1, double val2, double w) {
    return val1 * (1 - w) + val2 * w;
}
// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is a polynomial with little incline, bigger incline and little incline again 1 to 6 indicates longer little incline (0 is lerp)
double smoothstep1(double val1, double val2, double w) {
    double scale = -2 * w * w * w + 3 * w * w;
    return val1 * (1 - scale) + val2 * scale;
}
// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is a polynomial with little incline, bigger incline and little incline again 1 to 6 indicates longer little incline (0 is lerp)
double smoothstep2(double val1, double val2, double w) {
    double scale = 6*w*w*w*w*w - 15*w*w*w*w + 10*w*w*w;
    return val1 * (1 - scale) + val2 * scale;
}
// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is a polynomial with little incline, bigger incline and little incline again 1 to 6 indicates longer little incline (0 is lerp)
double smoothstep3(double val1, double val2, double w) {
    double scale = -20*pow(w, 7) + 70*pow(w, 6) - 84*pow(w, 5) + 35*pow(w, 4);
    return val1 * (1 - scale) + val2 * scale;
}
// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is a polynomial with little incline, bigger incline and little incline again 1 to 6 indicates longer little incline (0 is lerp)
double smoothstep4(double val1, double val2, double w) {
    double scale = 70*pow(w, 9) - 315*pow(w, 8) + 540*pow(w, 7) - 420*pow(w, 6) + 126*pow(w, 5);
    return val1 * (1 - scale) + val2 * scale;
}
// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is a polynomial with little incline, bigger incline and little incline again 1 to 6 indicates longer little incline (0 is lerp)
double smoothstep5(double val1, double val2, double w) {
    double scale = -252*pow(w, 11) + 1386*pow(w, 10) - 3080*pow(w, 9) + 3465*pow(w, 8) - 1980*pow(w, 7) + 462*pow(w, 6);
    return val1 * (1 - scale) + val2 * scale;
}
// returns a value between val1 and val2 depending on w (weight 0 - 1) 0 is val1 and 1 val2
// this is a polynomial with little incline, bigger incline and little incline again 1 to 6 indicates longer little incline (0 is lerp)
double smoothstep6(double val1, double val2, double w) {
    double scale = 924*pow(w, 13) - 6006*pow(w, 12) + 16380*pow(w, 11) - 24024*pow(w, 10) + 20020*pow(w, 9) - 9009*pow(w, 8) + 1716*pow(w, 7);
    return val1 * (1 - scale) + val2 * scale;
}


//? ----------------------------------------------------------------------------------------------------
//? Vector2 class
//? ----------------------------------------------------------------------------------------------------

class Vector2 
{
public:
    Vector2(float _x = 0, float _y = 0) {
        x = _x;
        y = _y;
    }
    ~Vector2() {}

    float x;
    float y;

    float magnitude() {
        return sqrt(x*x + y*y);
    }
    Vector2 unitVec() {
        float mag = magnitude();
        return Vector2(x / mag, y / mag);
    }

    Vector2 operator+(Vector2 other) {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator-(Vector2 other) {
        return Vector2(x - other.x, y - other.y);
    }
    float operator*(Vector2 other) {
        return x * other.x + y * other.y;
    }
    Vector2 operator*(float other) {
        return Vector2(x * other, y * other);
    }
    Vector2 operator/(float other) {
        return Vector2(x / other, y / other);
    }
};

//? ----------------------------------------------------------------------------------------------------
//? Vector3 class
//? ----------------------------------------------------------------------------------------------------

class Vector3 
{
public:
    Vector3(float _x = 0, float _y = 0, float _z = 0) {
        x = _x;
        y = _y;
        z = _z;
    }
    ~Vector3() {}

    float x;
    float y;
    float z;

    float magnitude() {
        return sqrt(x*x + y*y + z*z);
    }
    Vector3 unitVec() {
        float mag = magnitude();
        return Vector3(x / mag, y / mag, z / mag);
    }

    Vector3 operator+(Vector3 other) {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator-(Vector3 other) {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    float operator*(Vector3 other) {
        return x * other.x + y * other.y + z * other.z;
    }
    Vector3 operator*(float other) {
        return Vector3(x * other, y * other, z * other);
    }
    Vector3 operator/(float other) {
        return Vector3(x / other, y / other, z / other);
    }
};

//? ----------------------------------------------------------------------------------------------------
//? Vector funktions
//? ----------------------------------------------------------------------------------------------------

Vector3 crossProd(Vector3 a, Vector3 b) {
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double angle(Vector2 a, Vector2 b) { // angle in rads (returns -1 if one of the magnitudes is 0)
    double aMag = a.magnitude();
    double bMag = b.magnitude();
    if (bMag == 0 || aMag == 0)
        return -1;
    return acos((a * b) / (aMag * bMag));
}
double angle(Vector3 a, Vector3 b) { // angle in rads (returns -1 if one of the magnitudes is 0)
    double aMag = a.magnitude();
    double bMag = b.magnitude();
    if (bMag == 0 || aMag == 0)
        return -1;
    return acos((a * b) / (aMag * bMag));
}

Vector2 angleToVec(double angle, double magnitude) { // gets the 2d vector for a angle (in rads) and magnitude
    return Vector2(magnitude * cos(angle), magnitude * sin(angle));
}

#endif