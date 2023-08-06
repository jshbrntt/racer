// FILE: "enemy.h"

#include "global.h"

#ifndef ENEMY_H
#define ENEMY_H

using namespace std;

class Enemy : public Car
{
private:
  float rotationDeadzone;
  void runAI();

public:
  int currentNode;
  vector<Point> enemyVerts();
  vector<Point> enemyShape();
  SDL_Texture *enemyTexture();
  Enemy();
  Enemy(
      Point position,
      float rotation = 0);
  void update();
};

#endif
