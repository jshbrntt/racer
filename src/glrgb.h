// FILE: "glrgb.h"

#include "global.h"

#ifndef	GLRGB_H
#define	GLRGB_H

using namespace std;

class glRGB
{
public:
	glRGB();
	glRGB(float red, float green, float blue);
	float red;
	float green;
	float blue;
};

#endif