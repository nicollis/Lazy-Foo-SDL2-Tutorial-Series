#pragma once

#include <SDL.h>
#include <vector>

#include "OTexture.h"
#include "Util.h"

class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;

	//Initialize the variables
	Dot(int levelWidth = 1280, int levelHeight = 960);

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event &e);

	//Moves the dot
	void move();

	//Shows the dot on the screen
	void render(OTexture& dotTexture);

	//Position accessors
	int getPosX();
	int getPosY();

private:
	//The X and Y offset of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Screen size to keep dot in box
	int mLevelWidth, mLevelHeight;
};