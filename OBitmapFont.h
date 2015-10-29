#pragma once
#ifndef _OGE_BITMAP_FONT_H_
#define _OGE_BITMAP_FONT_H_

#include <SDL.h>
#include "OTexture.h"

class OBitmapFont
{
public:
	//Default constuctor
	OBitmapFont();

	//Generates the font
	bool buildFont(OTexture *bitmap);

	//Show the text
	void renderText(int x, int y, std::string text);

private:
	//The font texture
	OTexture* mBitmap;
	
	//The individual characters in the surface
	SDL_Rect mChars[256];

	//Spacing variables
	int mNewLine, mSpace;
};
#endif