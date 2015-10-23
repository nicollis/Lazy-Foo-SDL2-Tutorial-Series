#include "Particle.h"

Particle::Particle(int x, int y, OTexture* particles, OTexture* shimmer)
{
	//Set offsets
	mPosX = x - 5 + (rand() % 25);
	mPosY = y - 5 + (rand() % 25);

	//Initialize animation
	mFrame = rand() % 5;

	//Set type
	mTexture = &particles[(rand() % 3)];
	mShimmer = shimmer;
}//end Particle constructor

void Particle::render()
{
	//Show the image
	mTexture->render(mPosX, mPosY);

	//Show shimmer
	if (mFrame % 2 == 0)
	{
		mShimmer->render(mPosX, mPosY);
	}

	//Animate
	mFrame++;
}

bool Particle::isDead()
{
	return mFrame > 10;
}