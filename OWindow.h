#pragma once
#ifndef _OGE_Window_H_
#define _OGE_Window_H_

#include <string>
#include <sstream>
#include <SDL.h>

class OWindow
{
public:
	OWindow();

	//Creates window
	bool init(std::string windowName, int screenWidth = 640, int screenHeight = 480);

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

private:
	//Window data
	SDL_Window* mWindow;

	//Copy of Renderer generated
	SDL_Renderer* mRenderer;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
};
#endif