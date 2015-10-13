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
	static const int DOT_VEL = 1;

	//Initialize the variables
	Dot(int x, int y, int screen_width = 640, int screen_height = 480);

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event &e);

	//Moves the dot and check for collision
	void move(std::vector<SDL_Rect>& otherColliders);

	//Shows the dot on the screen
	void render(OTexture& dotTexture);

	//Gets the collision boxes
	std::vector<SDL_Rect>& getColliders();

private:
	//The X and Y offset of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Screen size to keep dot in box
	int mScreenWidth, mScreenHeight;

	//Dot's collision box
	std::vector<SDL_Rect> mColliders;

	//Moves the collision boxes relative to the dot's offset
	void shiftColliders();
};