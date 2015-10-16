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

TTF_Font* gFont;

//Font Textures
OFontTexture gPromptTextTexture = NULL;
OFontTexture gInputTextTexture = NULL;

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

	gFont = TTF_OpenFont("22/lazy.ttf", 20);
	if (gFont == NULL)
	{
		printf("Failed to load font file!");
		success = false;
	}

	//Provide texture with renderer
	gPromptTextTexture = OFontTexture(gRenderer, gFont);
	gInputTextTexture = OFontTexture(gRenderer, gFont);

	if (!gPromptTextTexture.loadFromRenderedText("Enter Text:", { 0,0,0 }))
	{
		printf("Failed to load from Rendered Text!");
		success = false;
	}
	

	return success;
}//end loadMedia

void close()
{
	//Free loaded images
	gPromptTextTexture.free();
	gInputTextTexture.free();
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

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 0xFF };

			//The current input text
			std::string inputText = "Some Text";
			gInputTextTexture.loadFromRenderedText(inputText, textColor);

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
						//Handle backspace
						if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
						{
							//lop off character
							inputText.pop_back();
							renderText = true;
						}
						//Handle copy
						else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
						{
							SDL_SetClipboardText(inputText.c_str());
						}
						//Handle paste
						else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
						{
							inputText = SDL_GetClipboardText();
							renderText = true;
						}
					}//end if else key event
					//Special text input event
					else if (e.type == SDL_TEXTINPUT)
					{
						//Not copy or paste
						if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') &&
							(e.text.text[0] == 'v' || e.text.text[0] == 'V') &&
							SDL_GetModState() & KMOD_CTRL))
						{
							//Append character
							inputText += e.text.text;
							renderText = true;
						}
					}
				}
					
					
				//Render text if needed
				if (renderText)
				{
					//Text is not empty
					if (inputText != "")
					{
						//render new text
						gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
					}
					//Text is empty
					else
					{
						//Render space texture
						gInputTextTexture.loadFromRenderedText(" ", textColor);
					}
				}

					

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render Textures
				gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
				gInputTextTexture.render((SCREEN_WIDTH - gInputTextTexture.getWidth()) / 2, gPromptTextTexture.getHeight());

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