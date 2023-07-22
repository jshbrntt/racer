// FILE: "entity.cpp"

#include "entity.h"
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

void Entity::draw(Point parentPosition)
{
  // LOAD IDENTITY MATRIX:
  // glLoadIdentity();

  // IF TEXTURE EXISTS ENABLE TEXTURING:
  if (texture != NULL)
  {
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_TEXTURE_2D);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // SDL_Rect fillRect;
    // fillRect.x = 0;
    // fillRect.y = 0;
    // fillRect.w = (int)width;
    // fillRect.h = (int)height;
    // SDL_RenderCopyEx(renderer,
    //                  texture,
    //                  NULL,
    //                  &fillRect,
    //                  (double)angle,
    //                  NULL,
    //                  SDL_FLIP_NONE);

    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
    for (Point point : verts) {
      if (point.x < minX) minX = point.x;
      if (point.x > maxX) maxX = point.x;
      if (point.y < minY) minY = point.y;
      if (point.y > maxY) maxY = point.y;
    }
    int width = maxX - minX;
    int height = maxY - minY;

    SDL_Rect rectangle;
    rectangle.x = minX + position.x + parentPosition.x;
    rectangle.y = minY + position.y + parentPosition.y;
    rectangle.w = maxX - minX;
    rectangle.h = maxY - minY;
    SDL_Point center{minX * -1, minY * -1};
    SDL_RenderCopyEx(renderer, this->texture, NULL, &rectangle, angle, &center, SDL_FLIP_NONE);

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

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(renderer, &wireframe[0], wireframe.size());

    // Convert to SDL_Point array
    // SDL_Point points[orientedShape.size() + 1];
    // transform(orientedShape.begin(),
    //           orientedShape.end(),
    //           points,
    //           [](const auto &point) {
    //             const int x = (int)point.x;
    //             const int y = (int)point.y;
    //             return SDL_Point{x, y};
    //           });

    // Add first point as also the final point
    // points[orientedShape.size()] = SDL_Point{(int)orientedShape[0].x, (int)orientedShape[0].y};

    // // Offset points by entity position
    // for (int i = 0; i < orientedShape.size() + 1; i++)
    // {
    //   points[i].x += (int)position.x;
    //   points[i].y += (int)position.y;
    // }

    // // Draw oriented shape.
    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // SDL_RenderDrawLines(renderer, points, shape.size() + 1);

    // // TRANSLATE
    // glTranslatef(parentPosition.x + position.x, parentPosition.y + position.y, 0.0f);

    // // ROTATE:
    // glRotatef(angle, 0.0f, 0.0f, 1.0f);

    // // DRAW POLYGON FROM VERTS:
    // if (verts.size() >= 3)
    // {
    //   // BEGIN DRAWING POLYGON:
    //   glBegin(GL_POLYGON);
    //   for (int i = 0; i != verts.size(); i++)
    //   {
    //     // GETTING CURRENT VERT:
    //     Point vert = verts[i];

    //     // MAP TEXTURE IF IT EXISTS:
    //     if (texture != 0)
    //     {
    //       // MAP TEXTURE RELATIVE TO VERTS:
    //       Point texturePosition = Point(0, 0);
    //       if (vert.x > 0)
    //         texturePosition.x = 1;
    //       if (vert.y > 0)
    //         texturePosition.y = 1;

    //       // APPLYING POSITION:
    //       glTexCoord2f(texturePosition.x, texturePosition.y);
    //     }
    //     // DRAW CURRENT VERT:
    //     glVertex2f(vert.x, vert.y);
    //   }
    //   glEnd();
    // }

    // // IF TEXTURE EXISTS DISABLE TEXTURING:
    // if (texture != 0)
    // {
    //   glDisable(GL_TEXTURE_2D);
    //   glDisable(GL_BLEND);
    // }
  }
}

void Entity::highlight(Point parentPosition, glRGB color, bool fill)
{
  // // LOAD IDENTITY MATRIX:
  // glLoadIdentity();

  // // TRANSLATE:
  // glTranslatef(parentPosition.x + position.x, parentPosition.y + position.y, 0.0f);

  // // ROTATE:
  // glRotatef(angle, 0.0f, 0.0f, 1.0f);

  // // DRAW POLYGON FROM VERTS:
  // if (verts.size() >= 3)
  // {
  //   // HIGHLIGHT IN RED:
  //   glColor3f(color.red, color.blue, color.green);

  //   // BEGIN DRAWING POLYGON / LINE_LOOP:
  //   fill ? glBegin(GL_POLYGON) : glBegin(GL_LINE_LOOP);
  //   for (int i = 0; i != shape.size(); i++)
  //   {
  //     // GETTING CURRENT VERT:
  //     Point vert = shape[i];

  //     // DRAW CURRENT VERT:
  //     glVertex2f(vert.x, vert.y);
  //   }
  //   glEnd();
  // }

  // // DRAW ORIGIN OF THE SHAPE:
  // glPointSize(10.0f);
  // glBegin(GL_POINTS);
  // glVertex2f(0, 0);
  // glEnd();
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
