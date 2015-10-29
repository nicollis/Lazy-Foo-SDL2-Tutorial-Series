#pragma once
#ifndef _Data_Stream_H_
#define _Data_Stream_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

class DataStream
{
public:
	//Initalizes internals
	DataStream();

	//Loads inital data
	bool loadMedia();

	//Deallocator
	void free();

	//Gets current frame data
	void* getBuffer();

private:
	//Internal data
	SDL_Surface *mImages[4];
	int mCurrentImage;
	int mDelayFrames;
};
#endif