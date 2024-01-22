#pragma once
#include "Prerequisites.h"

class Window
{
public:
	Window();
	~Window();

	HRESULT
		init(HINSTANCE hInstance, int nCmdShow, WNDPROC windproc);

	void
		update();

	void
		render();

	void
		Destroy();
public:
	HWND   m_hWnd = nullptr;
	unsigned m_width;
	unsigned m_height;
private:
	HINSTANCE                           m_hInst = nullptr;
	RECT m_rect;
	std::string m_windowName = "Life Engine";
};