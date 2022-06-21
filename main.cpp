#include <Windows.h>
#include <sstream>
#include <string>
#include "Window.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	char* Args, int nCmdShow)
{
	try
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
	catch (const CustomException& ex)
	{
		MessageBox(nullptr, ex.what(), ex.GetType(), MB_ICONERROR);
	}
	catch (const std::exception& ex)
	{
		MessageBox(nullptr, ex.what(), "Standart exception", MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, "No data about an error", "Error", MB_ICONERROR);
	}

	return -1;
}