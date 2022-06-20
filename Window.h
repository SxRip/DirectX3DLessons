#pragma once
#include <Windows.h>

class Window
{
	class WindowClass
	{
	public:
		const char* GetWndClassName() const;
		HINSTANCE GetInstance() const;

		WindowClass();
		~WindowClass();
	private:
		WindowClass(const WindowClass&) = delete;
		WindowClass operator=(const WindowClass&) = delete;

		const char* wndName;
		//static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:

	Window(int width, int height, const char* WindowName);
	~Window();

	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int width, height;
	HWND hwnd;
	WindowClass wndClass;
};