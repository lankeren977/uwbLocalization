#ifndef LOCATION_H
#define LOCATION_H

#include <iostream>
#include <math.h>
using namespace std;

static const double ZERO = 1e-9;

typedef struct vec2d vec2d;
struct vec2d
{
    double x;
    double y;
};

typedef struct circle circle;
struct circle
{
    double x;
    double y;
    double r;
};

vec2d trilateration(const vec2d *anchorArray, const int *radius, const int count);
vec2d selectPoint(const vec2d* points,const circle circle);
bool isOutsideCircle(const vec2d point, const circle circle);
void insect(vec2d* &insect, const circle circle1, const circle circle2);
vec2d optimizeByCentroid(const vec2d *points);

#endif