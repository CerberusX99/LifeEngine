#pragma once
#include "Prerequisites.h" 

// Window class responsible for managing the application window
class Window
{
public:
    // Default constructor and destructor
    Window();
    ~Window();

    // Initialize the window with the specified instance handle, show command, and window procedure
    HRESULT init(HINSTANCE hInstance, int nCmdShow, WNDPROC windproc);

    void update();

    void render();

    void Destroy();

public:
    HWND m_hWnd = nullptr; // Handle to the window
    unsigned m_width; // Width of the window
    unsigned m_height; // Height of the window

private:
    HINSTANCE m_hInst = nullptr; // Instance handle
    RECT m_rect; // Rectangle representing the window
    std::string m_windowName = "Life Engine"; // Name of the window
};
