#include "DataStream.h"

DataStream::DataStream()
{
	mImages[0] = NULL;
	mImages[1] = NULL;
	mImages[2] = NULL;
	mImages[3] = NULL;

	mCurrentImage = 0;
	mDelayFrames = 4;
}

bool DataStream::loadMedia()
{
	bool success = true;

	for (int i = 0; i < 4; ++i)
	{
		char path[64] = "";
		sprintf_s(path, "42_texture_streaming/foo_walk_%d.png", i);

		SDL_Surface* loadedSurface = IMG_Load(path);
		if (loadedSurface == NULL)
		{
			printf("Unable to load %s! SDL_image error: %s\n", path, IMG_GetError());
			success = false;
		}
		else
		{
			mImages[i] = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, NULL);
		}

		SDL_FreeSurface(loadedSurface);
	}

	return success;
}

void DataStream::free()
{
	for (int i = 0; i < 4; ++i)
	{
		SDL_FreeSurface(mImages[i]);
	}
}

void* DataStream::getBuffer()
{
	--mDelayFrames;
	if (mDelayFrames == 0)
	{
		++mCurrentImage;
		mDelayFrames = 4;
	}

	if (mCurrentImage == 4)
	{
		mCurrentImage = 0;
	}

	return mImages[mCurrentImage]->pixels;
}