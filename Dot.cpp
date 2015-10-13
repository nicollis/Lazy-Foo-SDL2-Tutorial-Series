#include "Dot.h"

Dot::Dot(int x, int y, int screen_width, int screen_height)
{
	//Initialize of Offsets
	mPosX = x;
	mPosY = y;

	//Create the necessary SDL_Rects
	mColliders.resize(11);

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	mScreenWidth = screen_width;
	mScreenHeight = screen_height;

	//Initialize the collision boxes' width and height
	mColliders[0].w = 6;
	mColliders[0].h = 1;

	mColliders[1].w = 10;
	mColliders[1].h = 1;

	mColliders[2].w = 14;
	mColliders[2].h = 1;

	mColliders[3].w = 16;
	mColliders[3].h = 2;

	mColliders[4].w = 18;
	mColliders[4].h = 2;

	mColliders[5].w = 20;
	mColliders[5].h = 6;

	mColliders[6].w = 18;
	mColliders[6].h = 2;

	mColliders[7].w = 16;
	mColliders[7].h = 2;

	mColliders[8].w = 14;
	mColliders[8].h = 1;

	mColliders[9].w = 10;
	mColliders[9].h = 1;

	mColliders[10].w = 6;
	mColliders[10].h = 1;

	//Initialize colliders relative to position
	shiftColliders();
}

void Dot::handleEvent(SDL_Event &e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}//end key sym swithc
	}//end if key down
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}//end key sym swithc
	}//end else if key up
}//end handle event method

void Dot::move(std::vector<SDL_Rect>& otherColliders)
{
	//Move the dot left or right
	mPosX += mVelX;
	shiftColliders();

	//If the dot went to far to hte left or right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > mScreenWidth) || 
		checkCollision(mColliders, otherColliders))
	{
		//Move back
		mPosX -= mVelX;
		shiftColliders();
	}

	//Move the dot up or down
	mPosY += mVelY;
	shiftColliders();

	//If the dot went to far to up or down
	if ((mPosY < 0) || (mPosY + DOT_HEIGHT > mScreenHeight) ||
		checkCollision(mColliders, otherColliders))
	{
		//Move back
		mPosY -= mVelY;
		shiftColliders();
	}
}// end move method 

void Dot::render(OTexture& dotTexture)
{
	//Show the dot
	dotTexture.render(mPosX, mPosY);
}//end render method

void Dot::shiftColliders()
{
	//The row offset
	int r = 0;

	//Go though the dot's collision boxes
	for (int set = 0; set < mColliders.size(); ++set)
	{
		//Center the collision box
		mColliders[set].x = mPosX + (DOT_WIDTH - mColliders[set].w) / 2;

		//Set the collision box at its row offset
		mColliders[set].y = mPosY + r;

		//Move the row offset down the height of the collision box
		r += mColliders[set].h;
	}
}

std::vector<SDL_Rect>& Dot::getColliders()
{
	return mColliders;
}