// FILE: "point.cpp"

#include "point.hpp"

Point::Point()
{
  this->x = 0;
  this->y = 0;
}
Point::Point(float x, float y)
{
  this->x = x;
  this->y = y;
}
ostream &operator<<(ostream &os, const Point &a)
{
  os << "[" << a.x << "," << a.y << "]";
  return os;
}
Point operator+(const Point &a, const Point &b)
{
  return Point(a.x + b.x, a.y + b.y);
}
Point operator-(const Point &a, const Point &b)
{
  return Point(a.x - b.x, a.y - b.y);
}
Point operator*(const Point &a, const Point &b)
{
  return Point(a.x * b.x, a.y * b.y);
}
Point operator/(const Point &a, const Point &b)
{
  return Point(a.x / b.x, a.y / b.y);
}
Point operator+(const Point &a, const float &b)
{
  return Point(a.x + b, a.y + b);
}
Point operator-(const Point &a, const float &b)
{
  return Point(a.x - b, a.y - b);
}
Point operator*(const Point &a, const float &b)
{
  return Point(a.x * b, a.y * b);
}
Point operator/(const Point &a, const float &b)
{
  return Point(a.x / b, a.y / b);
}
Point &operator+=(Point &a, const Point &b)
{
  a.x += b.x;
  a.y += b.y;
  return a;
}
Point &operator-=(Point &a, const Point &b)
{
  a.x -= b.x;
  a.y -= b.y;
  return a;
}
Point &operator*=(Point &a, const Point &b)
{
  a.x *= b.x;
  a.y *= b.y;
  return a;
}
Point &operator/=(Point &a, const Point &b)
{
  a.x /= b.x;
  a.y /= b.y;
  return a;
}
bool operator==(const Point &a, const Point &b)
{
  return ((a.x == b.x) && (a.y == b.y));
}
bool operator!=(const Point &a, const Point &b)
{
  return ((a.x != b.x) || (a.y != b.y));
}
Point &operator+=(Point &a, const float &b)
{
  a.x += b;
  a.y += b;
  return a;
}
Point &operator-=(Point &a, const float &b)
{
  a.x -= b;
  a.y -= b;
  return a;
}
Point &operator*=(Point &a, const float &b)
{
  a.x *= b;
  a.y *= b;
  return a;
}
Point &operator/=(Point &a, const float &b)
{
  a.x /= b;
  a.y /= b;
  return a;
}
