#pragma once
#ifndef _OGE_BUTTON_H_
#define _OGE_BUTTON_H_

#include <SDL2/SDL.h>
#include "OTexture.h"

enum OButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_MOUSE_TOTAL = 4
};


class OButton
{
public:
	//de/constructor
	OButton(int width = 0, int height = 0);
	~OButton();

	//Sets top left position
	void setPosition(int x, int y);

	void setWidth(int width);

	void setHeight(int height);

	OButtonSprite getCurrentSprite();

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Shows button sprite
	void render(OTexture* texture, SDL_Rect* clip = NULL);

private:
	//top left positon
	SDL_Point mPosition;

	//width and height of button
	int mWidth;
	int mHeight;

	//Currently used global sprite
	OButtonSprite mCurrentSprite;

};
#endif