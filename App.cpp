#include "App.h"
#include "Timer.h"
#include <sstream>

App::App() :_wnd(800, 300, "D3D love")
{

}

int App::Go()
{
	while (true)
	{
		if (const auto& ecode = _wnd.ProcessMessages())
			return ecode.value();

		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin(_timer.Peek()) / 2.0f + 0.5f;
	_wnd.Gfx().EndFrame();
	_wnd.Gfx().ClearBuffer(0.3f, 1.f, c);
}
