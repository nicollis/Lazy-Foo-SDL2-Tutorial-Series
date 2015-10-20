/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>

#include "OWindow.h"
#include "OTexture.h"

//The dimensions of the level;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
OWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Screen Texture
OTexture gSceneTexture;

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
		if (!gWindow.init("SDL_Resizable Windows!"))
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			sucess = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				sucess = false;
			}
			else
			{
				//initalize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
			}// end else create renderer
		}//end else
	}//end else
	return sucess;
}//end init

bool loadMedia() 
{
	bool success = true;

	
	//Initalize texture
	gSceneTexture = OTexture(gRenderer);

	if (!gSceneTexture.loadFromFile("35/window.png"))
	{
		printf("Failed to load image from file!\n");
		success = false;
	}
	

	return success;
}//end loadMedia

void close()
{
	

	//Free loaded images
	gSceneTexture.free();

	//Distory the window and renderer
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

			//While application is running
			while (!quit)
			{

				//Render text flag
				bool renderText = false;

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User request quite
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}//end if
					
					//Handle window events
					gWindow.handleEvent(e);
				}//end while

				//Only draw when not minimized
				if (!gWindow.isMinimized())
				{
					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render text texture
					gSceneTexture.render(
						(gWindow.getWidth() - gSceneTexture.getWidth()) / 2,
						(gWindow.getHeight() - gSceneTexture.getHeight()) / 2);

					//Update Screen
					SDL_RenderPresent(gRenderer);
				}
				
			}//end main loop
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func