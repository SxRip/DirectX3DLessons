#include "App.h"
#include <sstream>

App::App() :_wnd(800, 300, "D3D love")
{

}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
			return *ecode;

		DoFrame();
		Sleep(1);
	}
}

void App::DoFrame()
{
	std::stringstream ss;
	ss << "Time ellapsed: " << std::setprecision(1) << std::fixed << _timer.Peek();

	_wnd.SetTitle(ss.str());
}
