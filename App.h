#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
	App();

	int Go();

private:
	void DoFrame();
	Timer _timer;
	
	Window _wnd;
};

