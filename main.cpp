/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>

#include "OWindow.h"
#include "Dot.h"
#include "OTexture.h"
#include "DataStream.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 640;
const int LEVEL_HEIGHT = 480;

//The window we'll be rendering to
OWindow gWindow;

//Texture
OTexture gTargetTexture;

//Starts up SDL and creates a window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface(std::string path);

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

bool init()
{
	//Initialization flag;
	bool sucess = true;

	//Initalize SDL
	if (SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_AUDIO*/) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		sucess = false;
	}//end if
	else
	{
		//Set texture filtering to liner
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create Window
		if (!gWindow.init("Tile Engine!"))
		{
			printf("Window could not be created!");
			sucess = false;
		}
		else
		{
			
#ifdef _SDL_IMAGE_H
			//initialize PNG loading
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags)& imageFlags))
			{
				printf("SDL_image could not initialize! SDL_Image Error: %s\n", IMG_GetError());
				sucess = false;
			}//end image init else
#endif
#ifdef _SDL_TTF_H
				//Initialize SDL_ttf
			if (TTF_Init() == -1)
			{
				printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
				sucess = false;
			}
#endif

			/*//Initalize SDL_mixer
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				printf("SDL_mixer could not initalize! SDL_mixer Error: %s\n", Mix_GetError());
				sucess = false;
			}*/
		}//end else
	}//end else
	return sucess;
}//end init

bool loadMedia() 
{
	bool success = true;

	gTargetTexture.setRenderer(gWindow.getRenderer());

	//Load blank texture
	if (!gTargetTexture.createBlank(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_TEXTUREACCESS_TARGET))
	{
		printf("Failed to create texture!\n");
		success = false;
	}
	
	return success;
}//end loadMedia

void close()
{
	gTargetTexture.free();

	gWindow.free();

	//Quit SDL subsystems
#ifdef _SDL_TTF_H 
	TTF_Quit();
#endif
#ifdef _SDL_IMAGE_H
	IMG_Quit();
#endif
	SDL_Quit();
}//end close

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initalize!\n");
	}//end if
	else
	{
		//Load Media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}//end if
		else
		{
			// Main Loop Flag
			bool quit = false;

			//Event Handler
			SDL_Event e;

			//Rotation variables
			double angle = 0;
			SDL_Point screenCenter = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User request quite
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}//end if
					
					
				}//end while

				//rotate
				angle += 2;
				if (angle > 360)
				{
					angle -= 360;
				}

				//Set self as render target
				gTargetTexture.setAsRenderTarget();

				//Clear screen
				SDL_SetRenderDrawColor(gWindow.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gWindow.getRenderer());

				//Render red filled quad
				SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
				SDL_SetRenderDrawColor(gWindow.getRenderer() , 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(gWindow.getRenderer(), &fillRect);

				//Render green outlined quad
				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor(gWindow.getRenderer(), 0x00, 0xFF, 0x00, 0xFF);
				SDL_RenderDrawRect(gWindow.getRenderer(), &outlineRect);

				//Draw blue horizontal line
				SDL_SetRenderDrawColor(gWindow.getRenderer(), 0x00, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawLine(gWindow.getRenderer(), 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor(gWindow.getRenderer(), 0xFF, 0xFF, 0x00, 0xFF);
				for (int i = 0; i < SCREEN_HEIGHT; i += 4)
				{
					SDL_RenderDrawPoint(gWindow.getRenderer(), SCREEN_WIDTH / 2, i);
				}

				//Reset render target
				SDL_SetRenderTarget(gWindow.getRenderer(), NULL);

				//Show rendered to texture 
				gTargetTexture.render(0, 0, NULL, angle, &screenCenter);

				//Update screen
				SDL_RenderPresent(gWindow.getRenderer());
			}//end main loop
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func