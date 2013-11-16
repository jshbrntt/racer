// FILE: "hud.h"

#include "global.h"

#ifndef	HUD_H
#define	HUD_H

using namespace std;

class HUD
{
private:
	vector<Entity> entities;
	int raceWon;
	float start;
	float timer;
	int seconds;
public:
	HUD();
	void draw();
};

#endif