/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 640;
const int LEVEL_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//Starts up SDL and creates a window
bool init();

//Initializes matrices and clear color
bool initGL();

//Input handler
void handleKeys(unsigned char key, int x, int y);

//Per frame update
void update();

//Renders quad to the screen
void render();

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface(std::string path);

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

void handleKeys(unsigned char key, int x, int y)
{
	//toggle quad
	if (key == 'q')
	{
		gRenderQuad = !gRenderQuad;
	}
}

void update()
{
	//No per frame update needed
}

void render()
{
	//Clear color buffer 
	glClear(GL_COLOR_BUFFER_BIT);

	//Render quad
	if (gRenderQuad)
	{
		glBegin(GL_QUADS);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.5f, -0.5f);
			glVertex2f(0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);
		glEnd();
	}
}

bool init()
{
	//Initialization flag;
	bool sucess = true;

	//Initalize SDL
	if (SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_TIMER /*| SDL_INIT_AUDIO*/) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		sucess = false;
	}//end if
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		//Create Window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			sucess = false;
		}
		else
		{

			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				sucess = false;
			}
			else
			{
				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
					sucess = false;
				}
				//Initalize OpenGL
				if (!initGL())
				{
					printf("Unable to Initalize OpenGL!\n");
					sucess = false;
				}
			}
			
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

bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initalize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initalizing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initalizing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//Initalize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initalizing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	return success;
}

bool loadMedia() 
{
	bool success = true;
	
	return success;
}//end loadMedia

void close()
{
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

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

			//enable text input
			SDL_StartTextInput();

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
					else if (e.type == SDL_TEXTINPUT)
					{
						int x = 0, y = 0;
						SDL_GetMouseState(&x, &y);
						handleKeys(e.text.text[0], x, y);
					}
				}//end while
				
				//Render quad
				render();

				//Update screen
				SDL_GL_SwapWindow(gWindow);
			}//end main loop

			SDL_StopTextInput();
		}//end else
	}//end if

	//Free resources and close SDL
	close();
	system("pause");
	return 0;
}// end main func