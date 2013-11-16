// FILE: "player.h"

#include "global.h"

#ifndef	PLAYER_H
#define	PLAYER_H

using namespace std;

class Player: public Car
{
public:
	vector<Point> playerVerts();
	vector<Point> playerShape();
	GLuint playerTexture();
	Player();
	Player(Point position, float angle = 0);
	void update();
	void checkCollisions();
};

#endif