#pragma once
#ifndef _OGE_TIMER_H_
#define _OGE_TIMER_H_

#include <SDL.h>

class OTimer
{
public:
	//Initalize varables
	OTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer is started
	Uint32 mStartTicks;

	//The ticks stored w hen the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};
#endif