// FILE: "entity.cpp"

#include "entity.hpp"
#include <algorithm>

Entity::Entity()
{
  // INITIALIZING DEFAULT ENTITY:
  position = Point();
  angle = 0;
  texture = 0;
}

Entity::Entity(vector<Point> verts,
               vector<Point> shape,
               Point position,
               float angle,
               SDL_Texture *texture)
{
  // PARSING PARAMETERS:
  this->verts = verts;
  this->shape = shape;
  this->position = position;
  this->angle = angle;
  this->texture = texture;
  this->highlighted = false;

  // INITIALIZING PROJECTION OBJECTS:
  Point xProj(FLT_MAX, -FLT_MAX);
  Point yProj(FLT_MAX, -FLT_MAX);

  // ITERATING THROUGH VERTICES:
  for (int i = 0; i != shape.size(); i++)
  {
    // GET CURRENT VERTICE:
    Point vert = shape[i];

    // PROJECTING CURRENT VERTICE AGAINST X AND Y AXIS:
    xProj = Point(min(vert.x, xProj.x), max(vert.x, xProj.y));
    yProj = Point(min(vert.y, yProj.x), max(vert.y, yProj.y));
  }

  // ASSIGNING WIDTH AND HEIGHT:
  this->width = xProj.y - xProj.x;
  this->height = yProj.y - yProj.x;
}

Point rotatePoint(
    Point point,
    float angle,
    Point center = Point(0, 0))
{
  float sine = sin(angle);
  float cosine = cos(angle);

  // translate point back to origin:
  point.x -= center.x;
  point.y -= center.y;

  // rotate point
  Point rotatedPoint = Point(
      point.x * cosine - point.y * sine,
      point.x * sine + point.y * cosine);

  // translate point back:
  rotatedPoint.x += center.x;
  rotatedPoint.y += center.y;
  return rotatedPoint;
}

SDL_Color HSVtoRGB(double h, double s, double v)
{
  double c = v * s;
  double x = c * (1 - std::abs(std::fmod(h / 60.0, 2) - 1));
  double m = v - c;

  double r, g, b;
  if (h >= 0 && h < 60)
  {
    r = c;
    g = x;
    b = 0;
  }
  else if (h >= 60 && h < 120)
  {
    r = x;
    g = c;
    b = 0;
  }
  else if (h >= 120 && h < 180)
  {
    r = 0;
    g = c;
    b = x;
  }
  else if (h >= 180 && h < 240)
  {
    r = 0;
    g = x;
    b = c;
  }
  else if (h >= 240 && h < 300)
  {
    r = x;
    g = 0;
    b = c;
  }
  else
  {
    r = c;
    g = 0;
    b = x;
  }

  return {
      static_cast<Uint8>((r + m) * 255),
      static_cast<Uint8>((g + m) * 255),
      static_cast<Uint8>((b + m) * 255),
      SDL_ALPHA_OPAQUE};
}

void Entity::draw(Point parentPosition)
{
  int minX = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::min();
  int minY = std::numeric_limits<int>::max();
  int maxY = std::numeric_limits<int>::min();
  for (Point point : verts)
  {
    if (point.x < minX)
      minX = point.x;
    if (point.x > maxX)
      maxX = point.x;
    if (point.y < minY)
      minY = point.y;
    if (point.y > maxY)
      maxY = point.y;
  }
  int width = maxX - minX;
  int height = maxY - minY;

  SDL_Rect rectangle;
  rectangle.x = minX + position.x + parentPosition.x;
  rectangle.y = minY + position.y + parentPosition.y;
  rectangle.w = maxX - minX;
  rectangle.h = maxY - minY;
  SDL_Point center{minX * -1, minY * -1};

  if (texture)
  {
    SDL_RenderCopyEx(renderer, this->texture, NULL, &rectangle, angle, &center, SDL_FLIP_NONE);
  }

  if (debug) {
    vector<Point> orientedShape = this->getOrientedShape(parentPosition);
    vector<SDL_Point> wireframe;
    std::transform(
        orientedShape.begin(),
        orientedShape.end(),
        std::back_inserter(wireframe),
        [](const Point &point)
        {
          return SDL_Point{(int)point.x, (int)point.y};
        });
    wireframe.push_back(SDL_Point{(int)orientedShape[0].x, (int)orientedShape[0].y});
    SDL_SetRenderDrawColor(renderer, highlighted ? 255 : 0, highlighted ? 0 : 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(renderer, &wireframe[0], wireframe.size());
  }
}

void Entity::highlight(Point parentPosition, glRGB color, bool fill)
{
  highlighted = fill;
}

void Entity::update() {}

vector<Point> Entity::getOrientedShape(Point parentPosition = Point())
{
  // CONVERTING ANGLE TO RADIANS:
  float radians = angle * (M_PI / 180);

  // INITIALIZING ORIENTATED SHAPE:
  vector<Point> orientedShape = shape;

  // ITERATING THROUGH VERTICES:
  for (int i = 0; i < (int)shape.size(); i++)
  {
    // ROTATING / TRANSLATING VERTICES:
    orientedShape[i].x = (shape[i].x * cos(radians)) - (shape[i].y * sin(radians)) + position.x + parentPosition.x;
    orientedShape[i].y = (shape[i].x * sin(radians)) + (shape[i].y * cos(radians)) + position.y + parentPosition.y;
  }
  return orientedShape;
}
bool Entity::collides(Entity other)
{
  // GETTING ORIENTED BOUNDING SHAPE:
  vector<Point> shape1 = this->getOrientedShape();
  vector<Point> shape2 = other.getOrientedShape();

  // GETTING NUMBER OF FACES FOR EACH ENTITY:
  int total1 = shape1.size();
  int total2 = shape2.size();

  // FOR EACH FACE RELATIVE TO THIS ENTITY:
  for (int i = 0; i < total1; i++)
  {
    // GET CURRENT FACE:
    Point face(shape1[i].x - shape1[(i + 1) % total1].x, shape1[i].y - shape1[(i + 1) % total1].y);

    // GET CURRENT AXIS PERPENDICULAR TO THE CURRENT FACE:
    Point axis(-face.y, face.x);

    // NORMALIZE THE AXIS:
    axis /= sqrt(axis.x * axis.x + axis.y * axis.y);

    // GET MAXIMUM AND MINIMUM POSSIBLE VALUES OF A FLOAT:
    Point overlap1(FLT_MAX, -FLT_MAX);
    Point overlap2(FLT_MAX, -FLT_MAX);

    // GET THE MAXIMUM AND MINIMUM VALUES OF THIS ENTITY RELATIVE TO THE AXIS:
    for (int j = 0; j < total1; j++)
    {
      float proj1 = ((axis.x * shape1[j].x) + (axis.y * shape1[j].y)) / ((axis.x * axis.x) + (axis.y * axis.y));
      overlap1.x = min(proj1, overlap1.x);
      overlap1.y = max(proj1, overlap1.y);
    }

    // GET THE MAXIMUM AND MINIMUM VALUES OF THE OTHER ENTITY RELATIVE TO THE AXIS:
    for (int j = 0; j < total2; j++)
    {
      float proj2 = ((axis.x * shape2[j].x) + (axis.y * shape2[j].y)) / ((axis.x * axis.x) + (axis.y * axis.y));
      overlap2.x = min(proj2, overlap2.x);
      overlap2.y = max(proj2, overlap2.y);
    }

    // EXIT IF THE PROJECTIONS DO NOT OVERLAP:
    if (overlap1.x > overlap2.y || overlap1.y < overlap2.x)
    {
      return false;
    }
  }

  // IF ON ALL AXIS AN OVERLAP OCCURS THEN THE ENTITIES ARE INTERSECTING:
  return true;
}
Point Entity::MTV(Entity other)
{
  // GETTING ORIENTED BOUNDING SHAPE:
  vector<Point> shape1 = this->getOrientedShape();
  vector<Point> shape2 = other.getOrientedShape();

  // GETTING NUMBER OF FACES FOR EACH ENTITY:
  int total1 = shape1.size();
  int total2 = shape2.size();

  // INITIALIZING OVERLAP VARIABLE:
  bool positive = true;
  float overlap = FLT_MAX;
  Point smallest = Point(0, 0);

  // FOR EACH FACE RELATIVE TO THIS ENTITY:
  for (int i = 0; i < total1; i++)
  {
    // GET CURRENT FACE:
    Point face(shape1[i].x - shape1[(i + 1) % total1].x, shape1[i].y - shape1[(i + 1) % total1].y);

    // NORMALIZE THE FACE:
    face /= sqrt(face.x * face.x + face.y * face.y);

    // GET CURRENT AXIS PERPENDICULAR TO THE CURRENT FACE:
    Point axis(-face.y, face.x);

    // GET MAXIMUM AND MINIMUM POSSIBLE VALUES OF A FLOAT:
    Point overlap1(FLT_MAX, -FLT_MAX);
    Point overlap2(FLT_MAX, -FLT_MAX);

    // GET THE MAXIMUM AND MINIMUM VALUES OF THIS ENTITY RELATIVE TO THE AXIS:
    for (int j = 0; j < total1; j++)
    {
      float proj1 = ((axis.x * shape1[j].x) + (axis.y * shape1[j].y)) / ((axis.x * axis.x) + (axis.y * axis.y));
      overlap1.x = min(proj1, overlap1.x);
      overlap1.y = max(proj1, overlap1.y);
    }

    // GET THE MAXIMUM AND MINIMUM VALUES OF THE OTHER ENTITY RELATIVE TO THE AXIS:
    for (int j = 0; j < total2; j++)
    {
      float proj2 = ((axis.x * shape2[j].x) + (axis.y * shape2[j].y)) / ((axis.x * axis.x) + (axis.y * axis.y));
      overlap2.x = min(proj2, overlap2.x);
      overlap2.y = max(proj2, overlap2.y);
    }

    // GETTING OVERLAP INTERVAL:
    Point interval(max(overlap1.x, overlap2.x), min(overlap1.y, overlap2.y));

    // GETTING LENGTH OF THE INTERVAL:
    float newOverlap = max(interval.x, interval.y) - min(interval.x, interval.y);

    // CHECK TO SEE IF IT'S THE SMALLEST OVERLAP SO FAR IF SO REPLACE:
    if (newOverlap < overlap)
    {
      overlap = newOverlap;
      smallest = axis;
      if (overlap1.x < overlap2.x)
      {
        positive = true;
      }
      else
      {
        positive = false;
      }
    }
  }

  // CHANGING THE POLARITY OF THE OVERLAP APPROPRIATELY:
  overlap = positive ? overlap : -overlap;

  // IF ON ALL AXIS AN OVERLAP OCCURS THEN THE ENTITIES ARE INTERSECTING:
  return smallest * overlap;
}
