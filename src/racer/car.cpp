// FILE: "car.cpp"

#include "car.h"
#include <algorithm>

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
  speed = 0;
  speedAcceleration = 0.01f;
  turboSpeedAcceleration = 0.02f;
  speedDeceleration = 0.986f;
  steer = 0;
  steerAcceleration = 0.1f;
  steerDeceleration = 0.9f;

  // angularVelocity = 0;
  // angularMagnitude = 0.4;
  // angularFriction = 0.86;
  // linearVelocity = Point(0, 0);
  // linearMagnitude = 0.2;
  // linearFriction = 0.98;
  lapState = 0;
  immobilized = true;
  currentLap = 0;
  color = glRGB();
  // turboMagnitude = 4;
  turboOn = false;
  turboTimer = 0;
  turboTimeOut = 3000;
  flameDist = 0;
  flameOffset = 38;

  turningCurve.push_back(Point(0, 0));
  turningCurve.push_back(Point(0.1, 1.7));
  turningCurve.push_back(Point(0.5, 0.7));
  turningCurve.push_back(Point(1, 0.7));

  // CREATING FLAME ENTITY:
  vector<Point> flameBox = getBox(28, 28, Point(-14, -14));
  Point flamePosition(position.x + 40 * cos(angle * (M_PI / 180)),
                      position.y + 40 * sin(angle * (M_PI / 180)));
  SDL_Texture *flameTexture = loadTexture("assets/racer/fx/flame.png");
  flame = Entity(flameBox, flameBox, flamePosition, angle, flameTexture);
}

float factorial(float num)
{
  if (num <= 1)
  {
    return 1;
  }
  float fac = 1;
  for (float i = 1; i <= num; i++)
  {
    fac *= i;
  }
  return fac;
}

float choose(float a, float b)
{
  return factorial(a) / (factorial(b) * factorial(a - b));
}

Point bezier(vector<Point> &pts, float t)
{
  Point p;
  size_t len = pts.size();
  for (size_t i = 0; i < len; i++)
  {
    p += pts[i] * choose(len - 1, i) * pow(1 - t, len - 1 - i) * pow(t, i);
  }
  return p;
}

void Car::accelerate()
{
  if (!immobilized)
  {
    speed += turboOn ? turboSpeedAcceleration : speedAcceleration;
    // SELECT MAGNITUDE:
    // float magnitude = turboOn ? turboMagnitude : linearMagnitude;

    // MOVING CAR IN DIRECTION OF THE ANGLE:
    // linearVelocity.x += cos(angle * (M_PI / 180)) * magnitude;
    // linearVelocity.y += sin(angle * (M_PI / 180)) * magnitude;
  }
}
void Car::reverse()
{
  if (!immobilized)
  {
    speed -= turboOn ? turboSpeedAcceleration : speedAcceleration;
    // SELECT MAGNITUDE:
    // float magnitude = turboOn ? turboMagnitude : linearMagnitude;

    // MOVING CAR IN THE OPPOSITE DIRECTION OF THE ANGLE:
    // linearVelocity.x -= cos(angle * (M_PI / 180)) * magnitude;
    // linearVelocity.y -= sin(angle * (M_PI / 180)) * magnitude;
  }
}
void Car::turnLeft()
{
  if (!immobilized)
  {
    steer += steerAcceleration * bezier(turningCurve, abs(speed)).y;
    // CHECK THE CAR IS MOVING BEFORE IT'S ALLOWED TO TURN:
    // if (getAverageSpeed() >= 2)
    // {
    // TURNING THE CAR TO THE LEFT:
    // angularVelocity += angularMagnitude * ((getAverageSpeed() * 1.4) - getAverageSpeed());
    // SDL_Log("Speed: %f\n", getAverageSpeed());
    // }
  }
}
void Car::turnRight()
{
  if (!immobilized)
  {
    steer -= steerAcceleration * bezier(turningCurve, abs(speed)).y;
    // CHECK THE CAR IS MOVING BEFORE IT'S ALLOWED TO TURN:
    // if (getAverageSpeed() >= 2)
    // {
    // TURNING THE CAR TO THE RIGHT:
    // angularVelocity -= angularMagnitude * ((getAverageSpeed() * 1.4) - getAverageSpeed());
    // }
  }
}
void Car::turbo()
{
  if (!immobilized)
  {
    // IF TURBO ISN'T ALREADY ON:
    if (!turboOn)
    {
      turboTimer = SDL_GetTicks();
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
    proximityTiles.push_back(Point(currentTileIndex.x - 1, currentTileIndex.y - 1)); // TOP-LEFT
    proximityTiles.push_back(Point(currentTileIndex.x - 1, currentTileIndex.y));     // LEFT
    proximityTiles.push_back(Point(currentTileIndex.x - 1, currentTileIndex.y + 1)); // BOTTOM-LEFT
    proximityTiles.push_back(Point(currentTileIndex.x, currentTileIndex.y + 1));     // BOTTOM
    proximityTiles.push_back(Point(currentTileIndex.x + 1, currentTileIndex.y + 1)); // BOTTOM-RIGHT
    proximityTiles.push_back(Point(currentTileIndex.x + 1, currentTileIndex.y));     // RIGHT
    proximityTiles.push_back(Point(currentTileIndex.x + 1, currentTileIndex.y - 1)); // TOP-RIGHT
    proximityTiles.push_back(Point(currentTileIndex.x, currentTileIndex.y - 1));     // TOP
  }

  // CLAMP
  speed = clamp(speed, -1.0f, 1.0f);
  steer = clamp(steer, -1.0f, 1.0f);

  // PASSIVE DECELERATION
  speed *= speedDeceleration;
  steer *= steerDeceleration;

  // SDL_Log("Speed: %f\nSteer: %f\n\n", speed, steer);

  // APPLYING LINEAR VELOCITY / FRICTION:
  position.x += (speed * cos(angle * (M_PI / 180))) * 10;
  position.y += (speed * sin(angle * (M_PI / 180))) * 10;
  angle += steer * 3;

  // APPLYING ANGULAR VELOCITY / FRICTION:
  // angle += angularVelocity;
  // angularVelocity *= angularFriction;

  // CLAMPING ANGLE BETWEEN 180 AND -180:
  angle = (angle >= 180) ? (angle - 360) : (angle <= -180) ? (angle + 360)
                                                           : angle;

  // IF TURBO IS ON:
  if (turboOn)
  {
    // HEADING:
    float turboLeft = 1 - ((SDL_GetTicks() - turboTimer) / turboTimeOut);

    // IF THE CAR IS TRAVELLING FORWARDS:w
    flameDist = (20 * speed) * turboLeft;

    // IF CAR TRAVELLING FORWARD:
    flame.angle = angle;

    // UPDATE FLAME ANGLE / POSITION:
    // flame.position = Point(position.x, position.y);
    flame.position = Point(position.x - (flameOffset + flameDist) * cos(flame.angle * (M_PI / 180)),
                           position.y - (flameOffset + flameDist) * sin(flame.angle * (M_PI / 180)));

    // CHECK IF TURBO HAS TIMED OUT:
    if ((SDL_GetTicks() - turboTimer) >= turboTimeOut)
    {
      turboOn = false;
    }
  }

  // SDL_Log("\nSPEED: %f\nSTEER: %f\n\n", speed, steer);
}

vector<SDL_Point> trail;

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
    if (turboOn) {
      flame.draw(parentPosition);
    }

    // DRAWING CAR:
    Entity::draw(parentPosition);
  }

  // SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
  // vector<SDL_Point> sdlPoints;
  // for (float t = 0; t <= 1; t += 0.01)
  // {
  //   Point curvePoint = bezier(turningCurve, t);
  //   curvePoint.x *= 300;
  //   curvePoint.y *= 300;
  //   // SDL_Log("%f %f %f", t, curvePoint.x, curvePoint.y);
  //   sdlPoints.push_back(SDL_Point{(int)curvePoint.x, (int)curvePoint.y});
  // }
  // SDL_Point* curvePoints = &sdlPoints[0];
  // SDL_RenderDrawLines(renderer, curvePoints, sdlPoints.size());

  // trail.push_back(SDL_Point{(int)position.x, (int)position.y});
  // if (trail.size() > 500) {
  //   trail.erase(trail.begin());
  // }

  // vector<SDL_Point> screenspaceTrail;
  // auto transformFunction = [&parentPosition](SDL_Point point) { return SDL_Point{point.x + (int)parentPosition.x, point.y + (int)parentPosition.y}; };
  // std::transform(trail.begin(), trail.end(), std::back_inserter(screenspaceTrail), transformFunction);

  // SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  // SDL_RenderDrawLines(renderer, &screenspaceTrail[0], trail.size());
}

void Car::updateCurrentLap()
{
  // UPDATING CURRENT LAP:
  switch (lapState)
  {
  case 0:
    lapState = ((position.x > 2600) && (position.y > 1400)) ? 1 : 0;
    break;
  case 1:
    lapState = ((position.x > 5000) && (position.y > 2000)) ? 2 : 1;
    break;
  case 2:
    lapState = ((position.x < 3000) && (position.y < 2800)) ? 3 : 2;
    break;
  case 3:
    lapState = ((position.x < 800) && (position.y < 1500)) ? 4 : 3;
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
        // this->linearVelocity += MTV * 2;

        // SLOW CAR DOWN BY APPLYING FRICTION:
        // this->linearVelocity *= this->linearFriction;
        break;
      }
    }
#endif
#endif
#if DEBUG == 1
    // HIGHLIGHTING COLLISION POLYGONS:
    tileEntity.highlighted = this->highlighted = collision;
#endif
  }
}

// float Car::getAverageSpeed()
// {
//   return ((abs(linearVelocity.x) + abs(linearVelocity.y)) / 2);
// }
