#include "OButton.h"

OButton::OButton(int width, int height)
{
	mPosition.x = 0;
	mPosition.y = 0;

	mWidth = width;
	mHeight = height;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

OButton::~OButton()
{}

void OButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}//end setPosition

void OButton::setWidth(int width)
{
	mWidth = width;
}

void OButton::setHeight(int height)
{
	mHeight = height;
}

OButtonSprite OButton::getCurrentSprite()
{
	return mCurrentSprite;
}

void OButton::handleEvent(SDL_Event* e)
{
	//if mouse event happened
	if (e->type == SDL_MOUSEMOTION || 
		e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Cheak if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		if (x > mPosition.x + mWidth)
		{
			inside = false;
		}
		//Mouse is above the button
		if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse is below the button
		if (y > mPosition.y + mHeight)
		{
			inside = false;
		}

		//Mouse is outside the button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside the button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}//end e->type switch
		}//end if else mouse inside 
	}//end if is mouse event
}//end method handleEvent

void OButton::render(OTexture* texture, SDL_Rect* clip)
{
	//show current button spite
	texture->render(mPosition.x, mPosition.y, clip);
}