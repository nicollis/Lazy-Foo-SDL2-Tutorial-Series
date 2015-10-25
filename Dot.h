#pragma once

#include <SDL.h>

#include "OTexture.h"
#include "Particle.h"
#include "Util.h"
#include "Tile.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

//Particle count
const int TOTAL_PARTICLES = 20;

class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 10;

	//Initialize the variables
	Dot(bool effects = false);

	//Deallocates particles
	~Dot();

	//Initalize textures
	static bool init(SDL_Renderer *renderer);

	//Free static members
	static void free();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event &e);

	//Moves the dot
	void move(Tile *tiles[]);

	//Centers the camera on the dot
	void setCamera(SDL_Rect &camera);

	//Shows the dot on the screen
	void render(SDL_Rect &camera);

	//Position accessors
	int getPosX();
	int getPosY();

private:
	//Collision detection
	SDL_Rect mBox;

	//The particles
	Particle* particles[TOTAL_PARTICLES];

	//Show the particles
	bool mShowParticles;
	void renderParticles();

	//The velocity of the dot
	int mVelX, mVelY;

	static OTexture sDotTexture;
};