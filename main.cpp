/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>

#include "OWindow.h"
#include "Dot.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//The window we'll be rendering to
OWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

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
			gRenderer = gWindow.getRenderer();
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

	//Initalize Dots to be created
	if (!Dot::init(gRenderer))
	{
		printf("Failed to initalize Dot Objects!\n");
		success = false;
	}

	//Initalize Tiles
	if (!Tile::init(gRenderer))
	{
		printf("Failed to initalize Tile Objects!\n");
		success = false;
	}

	
	
	return success;
}//end loadMedia

void close()
{
	Tile::free();
	Dot::free();

	gWindow.free();
	gRenderer = NULL;

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

			//The dot that will be moving on the screen
			Dot dot = Dot(true);

			//Level Camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

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
					
					//Handle dot events
					dot.handleEvent(e);
				}//end while

				//Move the dot
				dot.move(Tile::TileSet);
				dot.setCamera(camera);

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render objects
				for (int i = 0; i < Tile::TOTAL_TILES; ++i)
				{
					Tile::TileSet[i]->render(camera);
				}
				
				dot.render(camera);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}//end main loop
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func