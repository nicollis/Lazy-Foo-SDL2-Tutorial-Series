#include "Dot.h"

Dot::Dot(bool effects)
{

	mBox.x = 0;
	mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;


	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	mShowParticles = effects;

	if (mShowParticles)
	{
		//Initialize particles
		for (int i = 0; i < TOTAL_PARTICLES; ++i)
		{
			particles[i] = new Particle(mBox.x, mBox.y);
		}
	}
}

Dot::~Dot()
{
	if (mShowParticles)
	{
		//Delete particles
		for (int i = 0; i < TOTAL_PARTICLES; ++i)
		{
			delete particles[i];
		}
	}
}

//Static initalizer of textures
bool Dot::init(SDL_Renderer *renderer)
{
	bool success = true;

	SDL_Color colorKey = { 0x00, 0xFF, 0xFF };

	if (!sDotTexture.textureIsLoaded())
	{
		sDotTexture.setRenderer(renderer);

		//Load dot texture
		if (!sDotTexture.loadFromFile("38_particle_engines/dot.bmp", &colorKey))
		{
			printf("Failed to load dot texture!\n");
			success = false;
		}

		//Initalize particles
		if (!Particle::init(renderer))
		{
			printf("Failed to initalize particle engine!\n");
			success = false;
		}
	}
	else
	{
		printf("Warning: Dots have already been initliazed.\n");
	}

	return success;
}

//Static deinitalizer of textures
void Dot::free()
{
	if (sDotTexture.textureIsLoaded())
	{
		sDotTexture.free();
		Particle::free();
	}
	else
	{
		printf("Warning: Dot.Free() already fired!\n");
	}
}

void Dot::handleEvent(SDL_Event &e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}//end key sym swithc
	}//end if key down
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}//end key sym swithc
	}//end else if key up
}//end handle event method

void Dot::move(float timeStamp)
{
	//Move the dot left or right
	mBox.x += mVelX * timeStamp;

	//If the dot went too far to the left or right
	if (mBox.x < 0)
	{
		mBox.x = 0;
	}
	else if (mBox.x > SCREEN_WIDTH - DOT_WIDTH)
	{
		mBox.x = SCREEN_WIDTH - DOT_WIDTH;
	}

	//Move the dot up or down
	mBox.y += mVelY * timeStamp;

	//If the dot went too far up or down
	if (mBox.y < 0)
	{
		mBox.y = 0;
	}
	else if (mBox.y > SCREEN_HEIGHT - DOT_HEIGHT)
	{
		mBox.y = SCREEN_HEIGHT - DOT_HEIGHT;
	}
}// end move method 

void Dot::setCamera(SDL_Rect &camera)
{
	//Center the camera over the dot
	camera.x = (mBox.x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (mBox.y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Dot::render()
{
	//Show the dot
	sDotTexture.render(mBox.x, mBox.y);
	if (mShowParticles) { renderParticles(); }
}//end render method

void Dot::render(SDL_Rect &camera)
{
	//Show the dot
	sDotTexture.render(mBox.x - camera.x, mBox.y - camera.y);
	if (mShowParticles) { renderParticles(); }
}//end render method

float Dot::getPosX()
{
	return mBox.x;
}

float Dot::getPosY()
{
	return mBox.y;
}

void Dot::renderParticles()
{
	//Go though particles
	for (int i = 0; i < TOTAL_PARTICLES; ++i)
	{
		//Delete and replace dead particles
		if (particles[i]->isDead())
		{
			delete particles[i];
			particles[i] = new Particle(mBox.x, mBox.y);
		}
	}

	//Show particles
	for (int i = 0; i < TOTAL_PARTICLES; ++i)
	{
		particles[i]->render();
	}
}

OTexture Dot::sDotTexture = OTexture();