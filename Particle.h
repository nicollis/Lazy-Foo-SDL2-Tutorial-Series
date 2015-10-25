#pragma once
#ifndef _OGE_PARTICLE_H_
#define _OGE_PARTICLE_H_

#include "OTexture.h"

class Particle
{
public:
	//Initalize position and animation
	Particle(int x, int y);

	//Initalize textures
	static bool init(SDL_Renderer *renderer);

	//Free static members
	static void free();

	//Show the particle
	void render();

	//Checks if the particle is dead
	bool isDead();


private:
	//Offsets
	int mPosX, mPosY;

	//Current frame of animation
	int mFrame;

	//Type of particle
	OTexture *mTexture;

	//Shimmer Texture to make particles shine
	OTexture *mShimmer;

	//Static texture references
	static OTexture sRedParticle;
	static OTexture sBlueParticle;
	static OTexture sGreenParticle;
	static OTexture sShimmer;
};
#endif