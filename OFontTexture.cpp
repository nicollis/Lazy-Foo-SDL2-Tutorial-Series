#include "OFontTexture.h"

OFontTexture::OFontTexture(SDL_Renderer * renderer, TTF_Font* font)
	: OTexture(renderer)
{
	setFont(font);
}

OFontTexture::~OFontTexture()
{
	mFont = NULL;
}

void OFontTexture::setFont(TTF_Font *font)
{
	mFont = font;
}

TTF_Font* OFontTexture::getFont()
{
	return mFont;
}

//Create texture from string using set font
bool OFontTexture::loadFromRenderedText(std::string textureText,
	SDL_Color textColor)
{
	return loadFromRenderedText(textureText, textColor, getFont());
}

//Create texture from string using temp font
bool OFontTexture::loadFromRenderedText(std::string textureText,
	SDL_Color textColor, TTF_Font* font)
{
	//Get rid of any preexisting texture
	free();

	//Render Text Surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unabled to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to creat texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image deminsons
			setWidth(textSurface->w);
			setHeight(textSurface->h);
		}
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}