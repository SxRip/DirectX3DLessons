#include <Windows.h>
#include <sstream>
#include <string>
#include "Window.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	char* Args, int nCmdShow)
{
	Window wnd(900, 500, "D3D love");

	MSG msg;
	BOOL bResult = 0;

	while ((bResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (bResult == -1)
		return -1;
	else
		return msg.wParam;
}