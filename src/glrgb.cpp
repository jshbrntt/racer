// FILE: "glrgb.cpp"

#include "glrgb.h"

glRGB::glRGB()
{
	this->red	= 1.0f;
	this->blue	= 1.0f;
	this->green	= 1.0f;
}
glRGB::glRGB(float red, float green, float blue)
{
	this->red	= red;
	this->green	= green;
	this->blue	= blue;
}