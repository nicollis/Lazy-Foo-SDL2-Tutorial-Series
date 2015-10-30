#include "Particle.h"

Particle::Particle(int x, int y)
{
	//Set offsets
	mPosX = x - 5 + (rand() % 25);
	mPosY = y - 5 + (rand() % 25);

	//Initialize animation
	mFrame = rand() % 5;

	//Set texture randomly
	switch ((rand() % 3))
	{
	case 0: mTexture = &sRedParticle; break;
	case 1: mTexture = &sBlueParticle; break;
	case 2: mTexture = &sGreenParticle; break;
	}
}//end Particle constructor

//Static initalizer of textures
bool Particle::init(SDL_Renderer *renderer)
{
	bool success = true;

	SDL_Color colorKey = { 0x00, 0xFF, 0xFF };

	if (!sRedParticle.textureIsLoaded())
	{
		sRedParticle.setRenderer(renderer);
		sBlueParticle.setRenderer(renderer);
		sGreenParticle.setRenderer(renderer);
		sShimmer.setRenderer(renderer);

		//Load red texture
		if (!sRedParticle.loadFromFile("38_particle_engines/red.bmp", &colorKey))
		{
			printf("Failed to load red texture!\n");
			success = false;
		}

		//Load green texture
		if (!sGreenParticle.loadFromFile("38_particle_engines/green.bmp", &colorKey))
		{
			printf("Failed to load green texture!\n");
			success = false;
		}

		//Load blue texture
		if (!sBlueParticle.loadFromFile("38_particle_engines/blue.bmp", &colorKey))
		{
			printf("Failed to load blue texture!\n");
			success = false;
		}

		//Load shimmer texture
		if (!sShimmer.loadFromFile("38_particle_engines/shimmer.bmp", &colorKey))
		{
			printf("Failed to load shimmer texture!\n");
			success = false;
		}

		//Set texture transparency
		sRedParticle.setAlpha(192);
		sGreenParticle.setAlpha(192);
		sBlueParticle.setAlpha(192);
		sShimmer.setAlpha(192);
	}
	else
	{
		printf("Warning: Particles have already been initliazed.\n");
	}

	return success;
}
//Static deinitalizer of textures
void Particle::free()
{
	if (sRedParticle.textureIsLoaded())
	{
		sRedParticle.free();
		sBlueParticle.free();
		sGreenParticle.free();
		sShimmer.free();
	}
	else
	{
		printf("Warning: Particle.Free() already fired!\n");
	}
}

void Particle::render()
{
	//Show the image
	mTexture->render(mPosX, mPosY);

	//Show shimmer
	if (mFrame % 2 == 0)
	{
		sShimmer.render(mPosX, mPosY);
	}

	//Animate
	mFrame++;
}

bool Particle::isDead()
{
	return mFrame > 10;
}


//Static values initalizaton
OTexture Particle::sRedParticle = OTexture();
OTexture Particle::sBlueParticle = OTexture();
OTexture Particle::sGreenParticle = OTexture();
OTexture Particle::sShimmer = OTexture();