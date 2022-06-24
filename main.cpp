#include "App.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	char* Args, int nCmdShow)
{
	try
	{
		return App{}.Go();
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