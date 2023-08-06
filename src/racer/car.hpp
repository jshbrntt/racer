// FILE: "car.hpp"

#include "global.hpp"

#ifndef CAR_H
#define CAR_H

using namespace std;

class Car : public Entity
{
protected:
  // PROTECTED FIELDS:
  float speed;
  float speedAcceleration;
  float turboSpeedAcceleration;
  float speedDeceleration;
  float steer;
  float steerAcceleration;
  float steerDeceleration;
  Entity flame;
  Point flamePosition;
  float flameDist;
  float flameOffset;
  float turboTimer;
  float turboTimeOut;
  bool turboOn;
  glRGB color;
  int lapState;
  vector<Point> proximityTiles;
  vector<Point> turningCurve;
  // PROTECTED FUNCTIONS:
  void accelerate();
  void reverse();
  void turnLeft();
  void turnRight();
  void turbo();
  void updateCurrentLap();
  virtual void checkCollisions();

public:
  // PUBLIC CONSTRUCTORS:
  Car();
  Car(
      vector<Point> verts,
      vector<Point> shape,
      Point position,
      float rotation = 0,
      SDL_Texture *texture = NULL);
  // PUBLIC FIELDS:
  Point currentTileIndex;
  Point linearVelocity;
  float linearFriction;
  bool immobilized;
  int currentLap;
  // PUBLIC FUNCTIONS:
  void update();
  void draw(Point parentPosition);
};

#endif
