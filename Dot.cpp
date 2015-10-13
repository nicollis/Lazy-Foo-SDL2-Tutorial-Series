#include "Dot.h"

Dot::Dot(int x, int y, int screen_width, int screen_height)
{
	//Initialize of Offsets
	mPosX = x;
	mPosY = y;

	//Create the necessary SDL_Rects
	mColliders.r = DOT_WIDTH / 2;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	mScreenWidth = screen_width;
	mScreenHeight = screen_height;

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

void Dot::move(SDL_Rect &square, Circle& circle)
{
	//Move the dot left or right
	mPosX += mVelX;
	shiftColliders();

	//If the dot went to far to hte left or right
	if ((mPosX - mColliders.r < 0) || (mPosX + mColliders.r > mScreenWidth) || 
		checkCollision(mColliders, square) || checkCollision(mColliders, circle))
	{
		//Move back
		mPosX -= mVelX;
		shiftColliders();
	}

	//Move the dot up or down
	mPosY += mVelY;
	shiftColliders();

	//If the dot went to far to up or down
	if ((mPosY - mColliders.r < 0) || (mPosY + mColliders.r > mScreenHeight) ||
		checkCollision(mColliders, square) || checkCollision(mColliders, circle))
	{
		//Move back
		mPosY -= mVelY;
		shiftColliders();
	}
}// end move method 

void Dot::render(OTexture& dotTexture)
{
	//Show the dot
	dotTexture.render(mPosX - mColliders.r, mPosY - mColliders.r);
}//end render method

void Dot::shiftColliders()
{
	//Align collider to center of dot
	mColliders.x = mPosX;
	mColliders.y = mPosY;
}

Circle& Dot::getColliders()
{
	return mColliders;
}