#pragma once
#ifndef _OGE_TEXTURE_H_
#define _OGE_TEXTURE_H_

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>

class OTexture
{
public:
	//de/contructor
	OTexture(SDL_Renderer* renderer);
	~OTexture();

	//Load image at specified path
	bool loadFromFile(std::string path);

	//Deaccocates Texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, 
		double angle = 0.0, SDL_Point* center = NULL, 
		SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Get image dimensions
	int getWidth();
	int getHeight();

	void setWidth(int w);
	void setHeight(int h);

protected:
	//hardware texture
	SDL_Texture* mTexture;
	SDL_Renderer* mRenderer;

private:
	//Image dimensions
	int mHeight;
	int mWidth;
};
#endif