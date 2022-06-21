#include "Window.h"
#include <sstream>

//Window::WindowClass Window::WindowClass::wndClass;


Window::WindowClass::WindowClass()
	: hInst(GetModuleHandle(nullptr)),
	wndName("DirectX 3D Window Lesson")
{
	WNDCLASSEX wc{ sizeof(wc) };

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetWndClassName();

	RegisterClassEx(&wc);
}

const char* Window::WindowClass::GetWndClassName() const
{
	return wndName;
}

HINSTANCE Window::WindowClass::GetInstance() const
{
	return hInst;
}


Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetWndClassName(), GetInstance());
}

Window::Window(int width, int height, const char* WindowName)
{
	RECT rc;

	rc.left = 100;
	rc.right = width - rc.left;
	rc.top = 100;
	rc.bottom = height + rc.top;

	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0);

	hwnd = CreateWindow(wndClass.GetWndClassName(), WindowName, WS_CAPTION
		| WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, nullptr, nullptr, wndClass.GetInstance(), this);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hwnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);

		Window* pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleMsgThunk));
		return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText(hwnd, "Respected");
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Exception::Exception(const int line, const char* file, HRESULT hr) noexcept
	: CustomException(line, file),
	_hr{hr}
{

}

const char* Window::Exception::what() const noexcept
{
	std::stringstream ss;

	ss << GetType() << std::endl
		<< "[ERROR CODE] " << GetErrorCode() << std::endl
		<< "[DESCRIPTION] " << GetErrorString() << std::endl
		<< _GetOriginalStr();

	whatBuff = ss.str();

	return whatBuff.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Window exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (!nMsgLen)
		return "Undentifided error code";

	std::string errorStr = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorStr;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return _hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(_hr);
}
