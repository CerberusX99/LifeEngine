#include "Window.h"

// Default constructor for the Window class
Window::Window()
{
}

// Destructor for the Window class
Window::~Window()
{
}

// Initialize the window with the specified instance handle, show command, and window procedure
HRESULT Window::init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst; // m_hInst initialized 
    wcex.hIcon = LoadIcon(m_hInst, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    m_hInst = hInstance; // Assign hInstance to m_hInst
    RECT rc = { 0, 0, 640, 480 };
    m_rect = rc; // Store the window's rectangle
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_hWnd = CreateWindow("TutorialWindowClass",
        "Direct3D 11 Tutorial 7",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        m_rect.right - m_rect.left,
        m_rect.bottom - m_rect.top,
        nullptr,
        nullptr,
        m_hInst, // Use m_hInst as the instance handle
        nullptr);
    if (!m_hWnd)
        return E_FAIL;

    ShowWindow(m_hWnd, nCmdShow);

    //RECT rc;
    GetClientRect(m_hWnd, &m_rect); // Get the client area rectangle of the window
    m_width = m_rect.right - m_rect.left; // Calculate the width of the client area
    m_height = m_rect.bottom - m_rect.top; // Calculate the height of the client area

    return S_OK;
}

void Window::update()
{
}

void Window::render()
{
}

void Window::Destroy()
{
}
