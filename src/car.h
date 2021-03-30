// FILE: "car.h"

#include "global.h"

#ifndef CAR_H
#define CAR_H

using namespace std;

class Car : public Entity
{
protected:
  // PROTECTED FIELDS:
  float angularVelocity;
  float angularMagnitude;
  float angularFriction;
  Entity flame;
  Point flamePosition;
  float flameDist;
  float flameOffset;
  float linearMagnitude;
  float turboMagnitude;
  float turboTimer;
  float turboTimeOut;
  bool turboOn;
  glRGB color;
  int lapState;
  vector<Point> proximityTiles;
  // PROTECTED FUNCTIONS:
  void accelerate();
  void reverse();
  void turnLeft();
  void turnRight();
  void turbo();
  void updateCurrentLap();
  virtual void checkCollisions();
  float getAverageSpeed();

public:
  // PUBLIC CONSTRUCTORS:
  Car();
  Car(
      vector<Point> verts,
      vector<Point> shape,
      Point position,
      float rotation = 0,
      SDL_Surface *texture = NULL);
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
