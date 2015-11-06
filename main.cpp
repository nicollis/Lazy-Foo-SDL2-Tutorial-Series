/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_thread.h>
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

OTexture gSplashScreenTexture;

//Our worker thread function
int producer(void* data);
int consumer(void* data);
void produce();
void consume();

//the protective mutex
SDL_mutex *gBufferLock = NULL;

//The conditions
SDL_cond *gCanProduce = NULL;
SDL_cond *gCanConsume = NULL;


//Data access semaphore
SDL_SpinLock gDataLock = 0;

//The data buffer
int gData = -1;

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

int producer(void *data)
{
	printf("\nProducer started....\n");

	//Seed thread random
	srand(SDL_GetTicks());

	//Produce
	for (int i = 0; i < 5; ++i)
	{
		//Wait
		SDL_Delay(rand() % 1000);

		//Produce
		produce();
	}

	printf("\nProducer Finished!\n");

	return 0;
}

int consumer(void *data)
{
	printf("\nConsumer Started....\n");

	//Seed random thread 
	srand(SDL_GetTicks());

	for (int i =0 ; i < 5; ++i)
	{
		//Wait
		SDL_Delay(rand() % 1000);

		//Consume
		consume();
	}

	printf("\nConsumer finished!\n");

	return 0;
}

void produce()
{
	//Lock
	SDL_LockMutex(gBufferLock);

	//If the buffer is full
	if (gData != -1)
	{
		//Wait for buffer to be cleared
		printf("\nProducer encountered full buffer, waiting for consumer to empty buffer...\n");
		SDL_CondWait(gCanProduce, gBufferLock);
	}

	//Fill and show buffer
	gData = rand() % 255;
	printf("\nProduced %d\n", gData);

	//Unlock
	SDL_UnlockMutex(gBufferLock);

	//Signal consumer
	SDL_CondSignal(gCanConsume);
}

void consume()
{
	//Lock
	SDL_LockMutex(gBufferLock);

	//If the buffer is empty
	if (gData == -1)
	{
		//Wait for the buffer to be filled
		printf("\nConsumer encountereed empty buffer, waiting for producer to fill buffer...\n");
		SDL_CondWait(gCanConsume, gBufferLock);
	}

	//Show and empty buffer
	printf("\nConsumed %d\n", gData);
	gData = -1;

	//Unlock
	SDL_UnlockMutex(gBufferLock);

	//Signal Producer
	SDL_CondSignal(gCanProduce);
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

	//Create Mutex
	gBufferLock = SDL_CreateMutex();

	//Create conditions
	gCanProduce = SDL_CreateCond();
	gCanConsume = SDL_CreateCond();

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

	//Destroy the mutex
	SDL_DestroyMutex(gBufferLock);
	gBufferLock = NULL;

	SDL_DestroyCond(gCanConsume);
	SDL_DestroyCond(gCanProduce);
	gCanConsume = NULL;
	gCanProduce = NULL;

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

			//Run the thread
			srand(SDL_GetTicks());
			SDL_Thread *threadA = SDL_CreateThread(producer, "Thread A", (void*)"Thread A");
			SDL_Delay(16 + rand() % 32);
			SDL_Thread *threadB = SDL_CreateThread(consumer, "Thread B", (void*)"Thread B");

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
			SDL_WaitThread(threadA, NULL);
			SDL_WaitThread(threadB, NULL);
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func