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

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 640;
const int LEVEL_HEIGHT = 480;

//The window we'll be rendering to
OWindow gWindow;

//Test our callback function
Uint32 callback(Uint32 interval, void* param);

OTexture gSplashScreenTexture;

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

Uint32 callback(Uint32 interval, void* param)
{
	//print callback message
	printf("Callback called back with message: %s\n", (char*)param);

	return 0;
}

bool init()
{
	//Initialization flag;
	bool sucess = true;

	//Initalize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER /*| SDL_INIT_AUDIO*/) < 0)
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

	gSplashScreenTexture.setRenderer(gWindow.getRenderer());

	if (!gSplashScreenTexture.loadFromFile("45_timer_callbacks/splash.png"))
	{
		printf("Issue initalizing Splash Screen!\n");
		success = false;
	}
	
	return success;
}//end loadMedia

void close()
{
	gSplashScreenTexture.free();

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

			//Set callback 
			SDL_TimerID timerID = SDL_AddTimer(3 * 1000, callback, "3 seconds waited!");

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
				
				//Clear screen
				SDL_SetRenderDrawColor(gWindow.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gWindow.getRenderer());

				//Render  Dot
				gSplashScreenTexture.render(0, 0);

				//Update screen
				SDL_RenderPresent(gWindow.getRenderer());
			}//end main loop

			//Remove timer in case the call back was not called
			SDL_RemoveTimer(timerID);
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func