// FILE: "entity.h"

#include "global.h"

#ifndef ENTITY_H
#define ENTITY_H

using namespace std;

class Entity
{
protected:
  vector<Point> verts;
  vector<Point> shape;
  SDL_Texture *texture;

public:
  float width;
  float height;
  Point position;
  float angle;
  Entity();
  Entity(vector<Point> verts,
         vector<Point> shape,
         Point position = Point(0, 0),
         float angle = 0,
         SDL_Texture *texture = NULL);
  virtual void draw(Point parentPosition = Point(0, 0));
  void highlight(Point parentPosition = Point(0, 0),
                 glRGB color = glRGB(1.0f, 1.0f, 1.0f),
                 bool fill = false);
  virtual void update();
  vector<Point> getOrientedShape();
  bool collides(Entity other);
  Point MTV(Entity other);
};

#endif
