#pragma once
#ifndef _OGE_UTIL_H_
#define _OGE_UTIL_H_

#include <SDL.h>
#include <vector>

//A circle Structure
struct Circle
{
	int x, y;
	int r;
};

//Box collion detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Per Pixel collision detector
bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b);

//Circle/Circle collision detector
bool checkCollision(Circle& a, Circle& b);

//Circle/Box collision detector
bool checkCollision(Circle& a, SDL_Rect& b);

//Calculates distance squared between two points
double distanceSquared(int x1, int y1, int x2, int y2);

#endif