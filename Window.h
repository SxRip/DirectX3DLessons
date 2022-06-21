#pragma once
#include <Windows.h>
#include "CustomException.h"

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
	class Exception : public CustomException
	{
	public:
		Exception(const int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;

		//std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT _hr;
	};

	Window(int width, int height, const char* WindowName);
	~Window();

	Window(const Window&) = delete;
	Window operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	//int width, height;
	HWND hwnd;
	WindowClass wndClass;
};

#define HWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr);
#define HWND_LAST_ERROR() Window::Exception(__LINE__, __FILE__, GetLastError());