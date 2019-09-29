// FILE: "global.h"

#ifndef	GLOBAL_H
#define	GLOBAL_H

// DEBUG MACROS:
#define DEBUG		1
#define COLLISION	1
#define RESPONSE	1
#define AI			1

// SYSTEM MACROS:
#define _USE_MATH_DEFINES

// SYSTEM HEADERS:
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glut.h"	// The OpenGL Utility Toolkit (GLUT),	http://user.xmission.com/~nate/glut.html
#include "SOIL.h"	// Simple OpenGL Image Library,			http://www.lonesock.net/soil.html
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <SOIL/SOIL.h>
#include <float.h>
#endif

// GAME HEADERS:
#include "point.h"	// Multi-purpose Textures,				http://www.mptextures.com/
#include "glrgb.h"
#include "entity.h"
#include "menu.h"
#include "hud.h"
#include "car.h"
#include "player.h"
#include "enemy.h"
#include "track.h"

// ENGINE VARIABLES:
extern const float S_WIDTH;
extern const float S_HEIGHT;
extern bool LEFTMOUSE;
extern int	MOUSE_X;
extern int MOUSE_Y;
extern vector<bool> KEYS;

// GAME VARIABLES:
extern Track* track;
extern Player* player;
extern Enemy* enemy;
extern vector<Car*> cars;
extern int lapLimit;
extern bool raceStarted;
extern bool resetGame;
extern bool gameRun;
extern bool pause;

// GLOBAL FUNCTIONS:
GLuint PNGtoGLtexture(const char *filename);
int roundUp(int numToRound, int multiple);
vector<Point> getBox(float width, float height, Point position = Point(0, 0));
vector<Point> getPoly(int sides, float radius, Point position = Point(0, 0));
void renderBitmapString(float x, float y, void* font, const char* string);

#endif