// FILE: "car.cpp"

#include "car.h"

Car::Car() : Entity() {}
Car::Car(
    vector<Point> verts,
    vector<Point> shape,
    Point position,
    float angle,
    SDL_Texture *texture) : Entity(verts,
                                   shape,
                                   position,
                                   angle,
                                   texture)
{
  // INITIALIZING CAR PROPERTIES:
  angularVelocity = 0;
  angularMagnitude = 0.4;
  angularFriction = 0.86;
  linearVelocity = Point(0, 0);
  linearMagnitude = 1;
  linearFriction = 0.9;
  lapState = 0;
  immobilized = true;
  currentLap = 0;
  color = glRGB();
  turboMagnitude = 1.5;
  turboOn = false;
  turboTimer = 0;
  turboTimeOut = 3000;
  flameDist = 0;
  flameOffset = 38;

  // CREATING FLAME ENTITY:
  vector<Point> flameBox = getBox(28, 28, Point(-14, -14));
  Point flamePosition(position.x + 40 * cos(angle * (M_PI / 180)),
                      position.y + 40 * sin(angle * (M_PI / 180)));
  SDL_Texture *flameTexture = loadTexture("../textures/fx/flame.png");
  flame = Entity(flameBox, flameBox, flamePosition, angle, flameTexture);
}
void Car::accelerate()
{
  if (!immobilized)
  {
    // SELECT MAGNITUDE:
    float magnitude = turboOn ? turboMagnitude : linearMagnitude;

    // MOVING CAR IN DIRECTION OF THE ANGLE:
    linearVelocity.x += cos(angle * (M_PI / 180)) * magnitude;
    linearVelocity.y += sin(angle * (M_PI / 180)) * magnitude;
  }
}
void Car::reverse()
{
  if (!immobilized)
  {
    // SELECT MAGNITUDE:
    float magnitude = turboOn ? turboMagnitude : linearMagnitude;

    // MOVING CAR IN THE OPPOSITE DIRECTION OF THE ANGLE:
    linearVelocity.x -= cos(angle * (M_PI / 180)) * magnitude;
    linearVelocity.y -= sin(angle * (M_PI / 180)) * magnitude;
  }
}
void Car::turnLeft()
{
  if (!immobilized)
  {
    // CHECK THE CAR IS MOVING BEFORE IT'S ALLOWED TO TURN:
    if (getAverageSpeed() >= 2)
    {
      // TURNING THE CAR TO THE LEFT:
      angularVelocity += angularMagnitude;
    }
  }
}
void Car::turnRight()
{
  if (!immobilized)
  {
    // CHECK THE CAR IS MOVING BEFORE IT'S ALLOWED TO TURN:
    if (getAverageSpeed() >= 2)
    {
      // TURNING THE CAR TO THE RIGHT:
      angularVelocity -= angularMagnitude;
    }
  }
}
void Car::turbo()
{
  if (!immobilized)
  {
    // IF TURBO ISN'T ALREADY ON:
    if (!turboOn)
    {
      turboTimer = clock();
      turboOn = true;
    }
  }
}

void Car::update()
{
  // CALLING SUPER FUNCTION:
  Entity::update();

  // CHECK COLLISIONS:
  checkCollisions();

  // UPDATE CURRENT LAP:
  updateCurrentLap();

  // IF CAR HAS REACHED LAP LIMIT IMMOBILIZE IT:
  if (currentLap == lapLimit)
    immobilized = true;

  // UPDATE TILE VARIABLES:
  if (currentTileIndex != track->getTileIndex(position))
  {
    // UPDATE CURRENT TILE INDEX:
    currentTileIndex = track->getTileIndex(position);

    // UPDATE PROXIMITY TILE INDEXES:
    proximityTiles.clear();
    proximityTiles.push_back(currentTileIndex);                                      // CENTER
    proximityTiles.push_back(Point(currentTileIndex.x - 1, currentTileIndex.y - 1)); // BOTTOM-LEFT
    proximityTiles.push_back(Point(currentTileIndex.x - 1, currentTileIndex.y));     // LEFT
    proximityTiles.push_back(Point(currentTileIndex.x - 1, currentTileIndex.y + 1)); // TOP-LEFT
    proximityTiles.push_back(Point(currentTileIndex.x, currentTileIndex.y + 1));     // TOP
    proximityTiles.push_back(Point(currentTileIndex.x + 1, currentTileIndex.y + 1)); // TOP-RIGHT
    proximityTiles.push_back(Point(currentTileIndex.x + 1, currentTileIndex.y));     // RIGHT
    proximityTiles.push_back(Point(currentTileIndex.x + 1, currentTileIndex.y - 1)); // BOTTOM-RIGHT
    proximityTiles.push_back(Point(currentTileIndex.x, currentTileIndex.y - 1));     // BOTTOM
  }

  // APPLYING LINEAR VELOCITY / FRICTION:
  position += linearVelocity;
  linearVelocity *= linearFriction;

  // APPLYING ANGULAR VELOCITY / FRICTION:
  angle += angularVelocity;
  angularVelocity *= angularFriction;

  // CLAMPING ANGLE BETWEEN 180 AND -180:
  angle = (angle >= 180) ? (angle - 360) : (angle <= -180) ? (angle + 360)
                                                           : angle;

  // IF TURBO IS ON:
  if (turboOn)
  {
    // HEADING:
    float heading = atan2(linearVelocity.y, linearVelocity.x) * (180 / M_PI);
    bool forward = (heading / abs(heading)) == (angle / abs(angle));

    // IF THE CAR IS TRAVELLING FORWARDS:
    if (forward)
      flameDist += ((abs(linearVelocity.x) + abs(linearVelocity.y)) / 6);

    // APPLY DECAY TO FLAME DISTANCE:
    flameDist *= linearFriction;

    // IF CAR TRAVELLING FORWARD:
    flame.angle = angle;

    // UPDATE FLAME ANGLE / POSITION:
    flame.position = Point(position.x - (flameDist + flameOffset) * cos(flame.angle * (M_PI / 180)),
                           position.y - (flameDist + flameOffset) * sin(flame.angle * (M_PI / 180)));

    // CHECK IF TURBO HAS TIMED OUT:
    if ((clock() - turboTimer) >= turboTimeOut)
    {
      turboOn = false;
    }
  }
}

void Car::draw(Point parentPosition)
{
  // CHECKING CAR IS ONSCREEN:
  Point relative = position + track->position;

  //USE TO DEMONSTRATE CLIPPING WORKS:
  int offset = 0;

  // CLIPPING CAR IF OUTSIDE VIEWPORT:
  if ((relative.x >= offset - track->tileSize && relative.x <= S_WIDTH + track->tileSize - offset) &&
      (relative.y >= offset - track->tileSize && relative.y <= S_HEIGHT + track->tileSize - offset))
  {
    // IF TURBO IS ON, DRAWING FLAME UNDER CAR:
    if (turboOn)
      flame.draw(parentPosition);

    // DRAWING CAR:
    Entity::draw(parentPosition);
  }
}

void Car::updateCurrentLap()
{
  // UPDATING CURRENT LAP:
  switch (lapState)
  {
  case 0:
    lapState = ((position.x >= 2800) && (position.y >= 800)) ? 1 : 0;
    break;
  case 1:
    lapState = ((position.x >= 2800) && (position.y <= 800)) ? 2 : 1;
    break;
  case 2:
    lapState = ((position.x <= 2600) && (position.y <= 800)) ? 3 : 2;
    break;
  case 3:
    lapState = ((position.x <= 2600) && (position.y >= 2100)) ? 4 : 3;
    break;
  case 4:
    currentLap++;
    lapState = 0;
    break;
  }
}
void Car::checkCollisions()
{
  // ASSUME THE COLLISION IS FALSE:
  bool collision = false;

  // MAP SIZE:
  int mapWidth = track->map[0].size();
  int mapHeight = track->map.size();

  // STORING CURRENT TILE INDEX:
  Point tileIndex;

  // STORING CURRENT TILE TYPE:
  int tileType;

  // STORING CURRENT TILE ENTITY:
  Entity tileEntity;

  // ITERATE THROUGH PROXIMITY TILES:
  for (int i = 0; i != proximityTiles.size(); i++)
  {
    // CURRENT TILE INDEX:
    tileIndex = proximityTiles[i];

    // EXCLUDE IRRELEVANT INDEXES:
    if (tileIndex.x < 0)
      continue;
    if (tileIndex.x > mapWidth - 1)
      continue;
    if (tileIndex.y < 0)
      continue;
    if (tileIndex.y > mapHeight - 1)
      continue;

    // CURRENT TILE TYPE:
    tileType = track->tiles[tileIndex.y][tileIndex.x];

    // EXCLUDE IRRELEVANT TILES:
    if (tileType < 3)
      continue;

    // CURRENT TILE ENTITY:
    tileEntity = track->map[tileIndex.y][tileIndex.x];

#if COLLISION == 1
    // CHECKING FOR A COLLISION:
    collision = this->collides(tileEntity) && tileEntity.collides(*this);
#if RESPONSE == 1
    // IF A COLLISION OCCURS:
    if (collision)
    {
      switch (tileType)
      {
      // TURBO TILE:
      case 12:
        // TRIGGER TURBO:
        turbo();
        break;
      // EVERY OTHER TILE:
      default:
        // GETTING MINIMUM TRANSLATION VECTOR:
        Point MTV = tileEntity.MTV(*this);

        // SEPARATE THE CAR FROM THE TILE USING THE MINIMUM TRANSLATION VECTOR::
        this->position += MTV;

        // MAKE CAR BOUNCE OFF THE WALL:
        this->linearVelocity += MTV * 2;

        // SLOW CAR DOWN BY APPLYING FRICTION:
        this->linearVelocity *= this->linearFriction;
        break;
      }
    }
#endif
#endif
#if DEBUG == 1
    // HIGHLIGHTING COLLISION POLYGONS:
    collision ? tileEntity.highlight(track->position, color, true) : tileEntity.highlight(track->position, color, false);
    collision ? this->highlight(track->position, color, true) : this->highlight(track->position, color, false);
#endif
  }
}

float Car::getAverageSpeed()
{
  return ((abs(linearVelocity.x) + abs(linearVelocity.y)) / 2);
}
