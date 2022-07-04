#include "Window.h"
#include <sstream>
#include "resource.h"

Window::WindowClass::WindowClass()
	: hInst(GetModuleHandle(nullptr)),
	wndName("DirectX 3D Window Lesson")
{
	WNDCLASSEX wc{ sizeof(wc) };

	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.hInstance = GetInstance();
	wc.lpszClassName = GetWndClassName();

	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32, 32, 0));

	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0));

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
	: width{ width }, height{ height }
{
	RECT rc;
	rc.left = 100;
	rc.right = width + rc.left;
	rc.top = 100;
	rc.bottom = height + rc.top;

	int centerX = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int centerY = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	if (!AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0))
		throw HWND_LAST_ERROR();

	hwnd = CreateWindow(wndClass.GetWndClassName(), WindowName, WS_CAPTION
		| WS_SYSMENU | WS_MINIMIZEBOX, centerX, centerY, rc.right - rc.left,
		rc.bottom - rc.top, nullptr, nullptr, wndClass.GetInstance(), this);

	if (hwnd == nullptr)
		throw HWND_LAST_ERROR();

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hwnd);
}

Window::~Window()
{
	DestroyWindow(hwnd);
}

void Window::SetTitle(const std::string& title)
{
	if (!SetWindowText(hwnd, title.c_str()))
		throw HWND_LAST_ERROR();
}

Graphics& Window::Gfx()
{
	return *pGfx;
}

HWND Window::_GetHWND() const noexcept
{
	return hwnd;
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
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

	case WM_KILLFOCUS:
	{
		keybd.ClearState();
		break;
	}

	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);

			if (!mouse.IsInWindow())
			{
				SetCapture(hwnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
				mouse.OnMouseMove(pt.x, pt.y);
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}

		break;
	}

	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}


	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}

	case WM_KEYDOWN:
		if (wParam == 'F')
			SetWindowText(hwnd, "Respected");

	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keybd.AutorepeatIsEnable())
			keybd.KeyOnPressed(static_cast<unsigned char>(wParam));
		break;

	case WM_SYSKEYUP:
		keybd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_KEYUP:
		keybd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		keybd.OnChar(static_cast<unsigned char>(wParam));
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Exception::Exception(const int line, const char* file, HRESULT hr) noexcept
	: CustomException(line, file),
	_hr{ hr }
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

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return _hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, _hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

	if (!nMsgLen)
		return "Undentifided error code";

	std::string errorStr = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorStr;
}
