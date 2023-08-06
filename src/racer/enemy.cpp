// FILE: "enemy.cpp"

#include "enemy.hpp"

vector<Point> Enemy::enemyVerts()
{
  // DEFAULT ENEMY SHAPE:
  vector<Point> verts;

  // VERTS:
  verts.push_back(Point(-52, -20));
  verts.push_back(Point(-52,  20));
  verts.push_back(Point(32,   20));
  verts.push_back(Point(32,  -20));

  //  RETURNING VERTS:
  return verts;
}

vector<Point> Enemy::enemyShape()
{
  // DEFAULT ENEMY SHAPE:
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

SDL_Texture *Enemy::enemyTexture()
{
  // IMPORTING PLAYER TEXTURE:
  return loadTexture("assets/cars/green_car.png");
}

Enemy::Enemy() : Car()
{
  // INITALIZING COLOR:
  color = glRGB(0.0f, 0.0f, 1.0f);
}

Enemy::Enemy(
    Point position,
    float rotation) : Car(enemyVerts(),
                          enemyShape(),
                          position,
                          rotation,
                          enemyTexture())
{
  // INITIALIZE CURRENT NODE:
  this->currentNode = 0;

  // INITIALIZE ROTATION DEADZONE:
  rotationDeadzone = 20;

  // INITALIZING COLOR:
  color = glRGB(0.0f, 0.0f, 1.0f);
}

void Enemy::update()
{
  // CALLING SUPER FUNCTION:
  Car::update();

  // RUN ARTIFICIAL INTELLIGENCE:
  runAI();
}

// SELECTED NODE INDEX:
Point nodeIndex;
int previousNode = -1;

void Enemy::runAI()
{
#if AI == 1
  // GET CURRENT AI NODE:
  Point trackNodeIndex = track->aiMap[currentNode];

  // GET CURRENT TURBO NODE:
  Point turboNodeIndex = track->turboMap[currentNode];

  // RANDOM TURBO SELECTION:
  if (currentNode != previousNode)
  {
    nodeIndex = trackNodeIndex;
    if (debug) {
      SDL_Log("CURRENT: %i, PREVIOUS: %i\n", currentNode, previousNode);
      SDL_Log("TRACK: %.f, %.f, TURBO: %.f, %.f\n", trackNodeIndex.x, trackNodeIndex.y, turboNodeIndex.x, turboNodeIndex.y);
    }
    if (track->tiles[turboNodeIndex.y][turboNodeIndex.x] == 12)
    {
      // RANDOM 1 / 0:
      int targetTurboTile = (rand() % 2);
      if (debug) {
        SDL_Log("RAND: %i\n", targetTurboTile);
      }
      if (targetTurboTile == 1)
      {
        nodeIndex = turboNodeIndex;
      }
    }
    if (debug) {
      SDL_Log("NODE: %.f, %.f, \n\n", nodeIndex.x, nodeIndex.y);
    }
  }

  // UPDATE PREVIOUS NODE INDEX:
  previousNode = currentNode;

  // IF AT NEXT NODE:
  if (currentTileIndex == nodeIndex)
  {
    currentNode++;
    currentNode = currentNode % (int)track->aiMap.size();
  }

  // CURRENT TILE ENTITY OBJECT:
  Entity tileEnt = track->map[nodeIndex.y][nodeIndex.x];

  // FIND THE DESIRED ANGLE TO FACE THE CURRENT TILE ENTITY:
  float desiredAngle = atan2((tileEnt.position.y + (track->tileSize / 2) - position.y),
                             (tileEnt.position.x + (track->tileSize / 2) - position.x)) *
                       (180 / M_PI);

  // IF ROTATION IS LARGER THAN A FULL TURN:
  if (abs(angle - desiredAngle) >= 180)
  {
    // ADJUST THE DESIRED ANGLE:
    if (angle < 0)
      desiredAngle -= 360;
    if (angle > 0)
      desiredAngle += 360;
  }

  // ROTATE THE CAR TOWARDS THE TILE:
  if (angle < (desiredAngle + (rotationDeadzone / 2)))
    turnLeft();
  if (angle > (desiredAngle - (rotationDeadzone / 2)))
    turnRight();

  // MOVE ENEMY CAR:
  accelerate();
#endif
}
