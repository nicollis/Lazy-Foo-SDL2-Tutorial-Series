#include "OTexture.h"

OTexture::OTexture(SDL_Renderer* renderer)
{
	mRenderer = renderer;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

OTexture::~OTexture()
{
	free();
	mRenderer = NULL;
}

bool OTexture::loadFromFile(std::string path)
{
	//Get rid of preexisiting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load iamge at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}//end if else newTexture created

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}//end if else loadedSurface

	mTexture = newTexture;
	return mTexture != NULL;
}//end loadFromFile

void OTexture::free()
{
	//Free texture if it exist
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}//end free

void OTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void OTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void OTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void OTexture::render(int x, int y, SDL_Rect* clip,
	double angle, SDL_Point* center,
	SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}//end render

int OTexture::getWidth()
{
	return mWidth;
}

int OTexture::getHeight()
{
	return mHeight;
}

void OTexture::setWidth(int w)
{
	mWidth = w;
}

void OTexture::setHeight(int h)
{
	mHeight = h;
}