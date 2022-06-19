#include <Windows.h>
#include <sstream>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{


	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	char* Args, int nCmdShow)
{
	const char* pClassName = "D3DClass";


	WNDCLASSEX wc{ sizeof(wc) };

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = pClassName;

	if (!RegisterClassEx(&wc))
		return -1;

	HWND mainWnd = CreateWindowEx(0, pClassName, "D3D",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200, 640, 480, nullptr,
		nullptr, hInstance, nullptr);

	ShowWindow(mainWnd, SW_SHOW);

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