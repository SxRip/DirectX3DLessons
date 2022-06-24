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
	}
}

void App::DoFrame()
{
	const float t = _timer.Peek();
	std::stringstream ss;
	ss << "Time ellapsed: " << std::setprecision(1) << std::fixed << t;

	_wnd.SetTitle(ss.str().c_str());
}
