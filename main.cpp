/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>

#include "OTexture.h"
#include "OTimer.h"
#include "Dot.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Dot Texture
OTexture gDotTexture = NULL;

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
	}//end if
	else
	{
		//Create Window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			sucess = false;
		}//end if
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				sucess = false;
			}
			else
			{
				//initalize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//initialize PNG loading
				int imageFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imageFlags)& imageFlags))
				{
					printf("SDL_image could not initialize! SDL_Image Error: %s\n", IMG_GetError());
					sucess = false;
				}//end image init else

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
			}// end else create renderer
		}//end else
	}//end else
	return sucess;
}//end init

bool loadMedia() 
{
	bool success = true;

	//Provide texture with renderer
	gDotTexture = OTexture(gRenderer);

	if (!gDotTexture.loadFromFile("26/dot.bmp"))
	{
		printf("Failed to load Dot Texture!");
		success = false;
	}

	return success;
}//end loadMedia

void close()
{
	//Free loaded images
	gDotTexture.free();
	

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 
	gRenderer = NULL;

	//Quit SDL subsystems
#ifdef _SDL_TTF_H 
	TTF_Quit();
#endif
	IMG_Quit();
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

			//The dot what will be moving around on the screen
			Dot dot(0, 0);

			//The dot that will be collided against
			Dot otherDot(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

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
					else if (e.type == SDL_KEYDOWN)
					{
						
					}//end if else key event
					
					//Handle dot events
					dot.handleEvent(e);
				}// end event pool loop

				//Move the dot
				dot.move(otherDot.getColliders());

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render textures
				dot.render(gDotTexture);
				otherDot.render(gDotTexture);

				//Update Screen
				SDL_RenderPresent(gRenderer);
			}//end main loop
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func