#include "App.h"
#include <sstream>

App::App() :_wnd(800, 300, "D3D love")
{

}

int App::Go()
{
	MSG msg;
	BOOL bResult = 0;

	while ((bResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//DoFrame();

		while (!_wnd.mouse.IsEmpty())
		{
			const Mouse::Event e = _wnd.mouse.Read();

			switch (e.GetType())
			{
			case Mouse::Event::Type::Leave:
				_wnd.SetTitle("Gone!");
				break;

			case Mouse::Event::Type::Enter:
				_wnd.SetTitle("Entered");
				break;

			case Mouse::Event::Type::Move:
				{
					std::stringstream ss;

					ss << e.GetPosX() << " " << e.GetPosY();

					_wnd.SetTitle(ss.str());
					break;
				}
			}
		}
	}

	if (bResult == -1)
		throw HWND_LAST_ERROR();

	return msg.wParam;
}

void App::DoFrame()
{
	const float t = _timer.Peek();
	std::stringstream ss;
	ss << "Time ellapsed: " << std::setprecision(1) << std::fixed << t;

	_wnd.SetTitle(ss.str().c_str());
}
