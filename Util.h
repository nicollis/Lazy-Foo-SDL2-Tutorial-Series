#pragma once
#ifndef _OGE_UTIL_H_
#define _OGE_UTIL_H_

#include <SDL.h>
#include <vector>

//Box collion detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Per Pixel collision detector
bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b);

#endif