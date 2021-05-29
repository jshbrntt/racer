// FILE: "player.cpp"

#include "player.h"

vector<Point> Player::playerVerts()
{
  // DEFAULT PLAYER VERTICES:
  vector<Point> verts = getBox(84, 40, Point(-52, -20));

  //  RETURNING VERTICES:
  return verts;
}

vector<Point> Player::playerShape()
{
  // DEFAULT PLAYER SHAPE:
  vector<Point> shape;

  // SHAPE:
  shape.push_back(Point(-40, -20));
  shape.push_back(Point(-52, -10));
  shape.push_back(Point(-52,  10));
  shape.push_back(Point(-40,  20));
  shape.push_back(Point(20,   20));
  shape.push_back(Point(32,   10));
  shape.push_back(Point(32,  -10));
  shape.push_back(Point(20,  -20));

  // RETURNING SHAPE:
  return shape;
}

SDL_Texture *Player::playerTexture()
{
  // IMPORTING PLAYER TEXTURE:
  return loadTexture("../textures/cars/red_car.png");
}

Player::Player() : Car()
{
  // INITALIZING COLOR:
  color = glRGB(1.0f, 0.0f, 0.0f);
}

Player::Player(
    Point position,
    float angle) : Car(playerVerts(),
                       playerShape(),
                       position,
                       angle,
                       playerTexture())
{
  // INITALIZING COLOR:
  color = glRGB(1.0f, 0.0f, 0.0f);
}

void Player::update()
{
  // CALLING SUPER FUNCTION:
  Car::update();

  // CHECKING KEY PRESSES:
  if (KEYS['w'] || KEYS['W'])
  {
    player->accelerate();
  }
  if (KEYS['s'] || KEYS['S'])
  {
    player->reverse();
  }
  if (KEYS['a'] || KEYS['A'])
  {
    player->turnRight();
  }
  if (KEYS['d'] || KEYS['D'])
  {
    player->turnLeft();
  }
}

void Player::checkCollisions()
{
  // CALLING SUPER METHOD:
  Car::checkCollisions();

  // ASSUME COLLISION IS FALSE:
  bool collision = false;

  // ASSUME PROXIMITY IS FALSE:
  bool proximity = false;

  // CHECK IF ENEMY IS WITHIN PROXIMITY:
  for (int i = 0; i < (int)player->proximityTiles.size(); i++)
  {
    if (player->proximityTiles[i] == enemy->currentTileIndex)
    {
      proximity = true;
      break;
    }
  }

  // IF PROXIMITY IS TRUE:
  if (proximity)
  {
#if COLLISION == 1
    // CHECKING FOR A COLLISION:
    collision = this->collides(*enemy) && enemy->collides(*this);
#if RESPONSE == 1
    // OBB VS OBB COLLISIONS:
    if (collision)
    {
      // STORE VELOCITIES:
      Point thisVel = this->linearVelocity;
      Point enemyVel = enemy->linearVelocity;

      // GET MINIMUM TRANSLATION VECTOR:
      Point MTV = enemy->MTV(*this) / 2;
      this->position += MTV;
      enemy->position -= MTV;

      // SWAP VELOCITIES:
      this->linearVelocity = enemyVel;
      enemy->linearVelocity = thisVel;

      // APPLYING FRICTION:
      this->linearVelocity *= this->linearFriction;
      enemy->linearVelocity *= enemy->linearFriction;
    }
#endif
#endif
#if DEBUG == 1
    // HIGHLIGHT TILE ENEMY IS ON:
    track->map[enemy->currentTileIndex.y][enemy->currentTileIndex.x].highlight(track->position, color, false);
#endif
  }
}
