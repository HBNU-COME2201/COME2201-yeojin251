#pragma once
#include <cmath>

typedef struct {
    double x;
    double y;
} Pos;

inline double DEG2RAD(double deg)
{
    return deg * M_PI/180.0;
}

inline double CALC_DIST(const Pos& pos1, const Pos& pos2)
{
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}