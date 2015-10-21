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

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Focuses on window
	void focus();

	//Shows windows contents
	void render();

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown();

private:
	//Window data
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	int mWindowID;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mShown;
};
#endif