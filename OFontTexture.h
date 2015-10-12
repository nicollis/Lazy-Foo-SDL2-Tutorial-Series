#pragma once
#ifndef _OGE_FONT_TEXTURE_H_
#define _OGE_FONT_TEXTURE_H_

#include <SDL.h>
#include <SDL_ttf.h>

#include "OTexture.h"

class OFontTexture : public OTexture
{
public:
	//de/construtors
	OFontTexture(SDL_Renderer *renderer = NULL, TTF_Font* font = NULL);
	~OFontTexture();

	//Gets/Sets font for object
	void setFont(TTF_Font *font);
	TTF_Font* getFont();

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Creates image from font string with provided font
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, 
		TTF_Font *font);

private:
	//Choosen font
	TTF_Font *mFont = NULL;

};

#endif