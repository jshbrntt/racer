// FILE: "point.h"

#include "global.h"

#ifndef POINT_H
#define POINT_H

using namespace std;

class Point
{
  friend ostream &operator<<(ostream &os, const Point &a);
  friend Point operator+(const Point &a, const Point &b);
  friend Point operator-(const Point &a, const Point &b);
  friend Point operator*(const Point &a, const Point &b);
  friend Point operator/(const Point &a, const Point &b);
  friend Point operator+(const Point &a, const float &b);
  friend Point operator-(const Point &a, const float &b);
  friend Point operator*(const Point &a, const float &b);
  friend Point operator/(const Point &a, const float &b);
  friend Point &operator+=(Point &a, const Point &b);
  friend Point &operator-=(Point &a, const Point &b);
  friend Point &operator*=(Point &a, const Point &b);
  friend Point &operator/=(Point &a, const Point &b);
  friend bool operator==(const Point &a, const Point &b);
  friend bool operator!=(const Point &a, const Point &b);
  friend Point &operator+=(Point &a, const float &b);
  friend Point &operator-=(Point &a, const float &b);
  friend Point &operator*=(Point &a, const float &b);
  friend Point &operator/=(Point &a, const float &b);

public:
  float x;
  float y;
  Point();
  Point(float x, float y);
};

#endif
