// FILE: "player.h"

#include "global.h"

#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

class Player : public Car
{
public:
  vector<Point> playerVerts();
  vector<Point> playerShape();
  SDL_Texture *playerTexture();
  Player();
  Player(Point position, float angle = 0);
  void update();
  void checkCollisions();
};

#endif
