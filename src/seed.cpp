/*
 * Seed.cpp
 * Created : 2018-17-11 23:03:22
 * Author: David Patry
 */

#include "seed.h"

//default constructor
Seed::Seed() :
center_x(0), center_y(0)
{
	
}

Seed::~Seed()
{
	
}

void Seed::setCenter(float x, float y)
{
	center_x=x;
	center_y=y;
}

int Seed::getPosX()
{	
	return center_x;
}
int Seed::getPosY()
{	
	return center_Y;
}

