#include "OTexture.h"

OTexture::OTexture() {}

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

bool OTexture::loadFromFile(std::string path, SDL_Color* colorKey)
{
	//Get rid of preexisiting texture
	free();

	mEditabled = false;

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
		if (colorKey != NULL)
		{
			SDL_SetColorKey(loadedSurface, SDL_TRUE,
				SDL_MapRGB(loadedSurface->format,
					colorKey->r, colorKey->g, colorKey->b));
		}

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

bool OTexture::loadEditableFromFile(std::string path, SDL_Window* window)
{
	//Get rid of preexisting texture
	free();

	mEditabled = true;

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(window)->format, NULL);
		if (formattedSurface == NULL)
		{
			printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture(mRenderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
			if (newTexture == NULL)
			{
				printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Lock texture for manipulation
				SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);

				//Copy loaded/formatted surface pixels
				memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

				//Unlock texture to update
				SDL_UnlockTexture(newTexture);
				mPixels = NULL;

				//Get image dimensions
				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface(formattedSurface);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool OTexture::textureIsLoaded()
{
	return mTexture != NULL;
}

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

	if (SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip))
	{
		printf("Error rendering texture! SDL Error %s\n", SDL_GetError());
	}
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

void OTexture::setRenderer(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

SDL_Renderer* OTexture::getRenderer()
{
	return mRenderer;
}

bool OTexture::lockTexture()
{
	bool success = true;

	//Texture is not editable
	if (!mEditabled)
	{
		printf("Texture is not editable!\n");
		success = false;
	}
	//Texture is already locked
	else if (mPixels != NULL)
	{
		printf("Texture is already locked!\n");
		success = false;
	}
	//Lock texture
	else
	{
		if (SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0)
		{
			printf("Unable to lock texture! %s\n", SDL_GetError());
			success = false;
		}
	}
	return success;
}

bool OTexture::unlockTexture()
{
	bool success = true;

	//Texture is not editable 
	if (!mEditabled)
	{
		printf("Texture is not editable!\n");
		success = false;
	}
	//Texture is not locked
	else if (mPixels == NULL)
	{
		printf("Texture is not locked!\n");
		success = false;
	}
	//Unlock Texture
	else
	{
		SDL_UnlockTexture(mTexture);
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* OTexture::getPixels()
{
	return mPixels;
}

int OTexture::getPitch()
{
	return mPitch;
}

bool OTexture::isEditable()
{
	return mEditabled;
}