/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>

#include "OTexture.h"
#include "OTimer.h"
#include "OFontTexture.h"

//The dimensions of the level;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Data points
const int TOTAL_DATA = 10;
Sint32 gData[TOTAL_DATA];

//Font
TTF_Font* gFont;

//Font Textures
OFontTexture gPromptTextTexture = NULL;
OFontTexture gDataTextures[TOTAL_DATA];

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

	//Text rendering color
	SDL_Color textColor = { 0,0,0,0xFF };
	SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

	

	//Open file for reading in binary
	SDL_RWops* file = SDL_RWFromFile("33/nums.bin", "r+b");
	//File does not exist
	if (file == NULL)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

		//Create file for writing
		file = SDL_RWFromFile("33/nums.bin", "w+b");
		if (file != NULL)
		{
			printf("New file created!\n");

			//Initalize data
			for (int i = 0; i < TOTAL_DATA; ++i)
			{
				gData[i] = 0;
				SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
			}//end for

			//Close file handler
			SDL_RWclose(file);
		}//end if file not nill
		else
		{
			printf("Error: Unable to create file! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
	}//end if file equils null
	else
	{
		//Load data
		printf("Reading file...!\n");
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
		}
		//Close file handler
		SDL_RWclose(file);
	}//end else 

	gFont = TTF_OpenFont("33/lazy.ttf", 20);
	if (gFont == NULL)
	{
		printf("Failed to load font file!");
		success = false;
	}
	else
	{
		gPromptTextTexture = OFontTexture(gRenderer, gFont);
		if (!gPromptTextTexture.loadFromRenderedText("Enter Data:", textColor))
		{
			printf("Failed to render prompt text!\n");
			success = false;
		}
	}


	//Initalize data texturs
	gDataTextures[0] = OFontTexture(gRenderer, gFont);
	gDataTextures[0].loadFromRenderedText(std::to_string((_Longlong)gData[0]), highlightColor);
	for (int i = 1; i < TOTAL_DATA; ++i)
	{
		gDataTextures[i] = OFontTexture(gRenderer, gFont);
		gDataTextures[i].loadFromRenderedText(std::to_string((_Longlong)gData[i]), textColor);
	}
	

	return success;
}//end loadMedia

void close()
{
	//Open data for writing
	SDL_RWops* file = SDL_RWFromFile("33/nums.bin", "w+b");
	if (file != NULL)
	{
		//Save data 
		for (int i = 0; i < TOTAL_DATA; ++i)
		{
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		}

		//Close file handler
		SDL_RWclose(file);
	}
	else
	{
		printf("Error: unable to save file! %s\n", SDL_GetError());
	}

	//Free loaded images
	for (int i = 0; i < TOTAL_DATA; ++i)
	{
		gDataTextures[i].free();
	}

	//Free loaded images
	gPromptTextTexture.free();
	TTF_CloseFont(gFont);
	gFont = NULL;
	

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL; 
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

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 0xFF };
			SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

			//Current input point
			int currentData = 0;

			//Enable Text Input
			SDL_StartTextInput();

			//The background scrolling offset
			int scrollingOffset = 0;

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
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
							//Previous data entry
						case SDLK_UP:
							//Rerender previous entry input point
							gDataTextures[currentData].loadFromRenderedText(std::to_string((_Longlong)gData[currentData]), textColor);
							--currentData;
							if (currentData < 0)
							{
								currentData = TOTAL_DATA - 1;
							}

							//Rerender current entry input point
							gDataTextures[currentData].loadFromRenderedText(std::to_string((_Longlong)gData[currentData]), highlightColor);
							break;

							//Next data entry
						case SDLK_DOWN:
							//Rerender previous entry input point
							gDataTextures[currentData].loadFromRenderedText(std::to_string((_Longlong)gData[currentData]), textColor);
							++currentData;
							if (currentData == TOTAL_DATA)
							{
								currentData = 0;
							}

							//Rerender current entry input point
							gDataTextures[currentData].loadFromRenderedText(std::to_string((_Longlong)gData[currentData]), highlightColor);
							break;
							//Decrement input point
						case SDLK_LEFT:
							--gData[currentData];
							gDataTextures[currentData].loadFromRenderedText(std::to_string((_Longlong)gData[currentData]), highlightColor);
							break;

							//Increment input point
						case SDLK_RIGHT:
							++gData[currentData];
							gDataTextures[currentData].loadFromRenderedText(std::to_string((_Longlong)gData[currentData]), highlightColor);
							break;
						}//end switch
					}//end if key down
				}//end while

					

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render Textures
				gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
				for (int i = 0; i < TOTAL_DATA; ++i)
				{
					gDataTextures[i].render((SCREEN_WIDTH - gDataTextures[i].getWidth()) / 2, gPromptTextTexture.getHeight() + gDataTextures[0].getHeight() * i);
				}

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