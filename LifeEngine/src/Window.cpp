#include "Windows.h"
#include "Window.h"

Window::Window()
{
}

Window::~Window()
{
}

// La clase Window se utiliza para inicializar la ventana.
//  Toma como parámetros el identificador de instancia de la aplicacion 
// (hInstance), el comando de visualizacion (nCmdShow) y el procedimiento 
// de ventana (wndproc)
//Se registra la clase Ventana
//Se crea la ventana con las dimensiones y el estilo especificado
//Se ajusta el tamaño de la ventana para que se incluya el marco
//Se obtinen las dimensiones de la ventana y se actualizan las variables
HRESULT Window::init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc)
{
    m_hInst = hInstance;
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
 
    RECT rc = { 0, 0, 640, 480 };
    m_rect = rc;
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    m_hWnd = CreateWindow("TutorialWindowClass", 
             "PLOPLO-Engine", 
             WS_OVERLAPPEDWINDOW,
             CW_USEDEFAULT, 
             CW_USEDEFAULT, 
        m_rect.right - m_rect.left,
        m_rect.bottom - m_rect.top,
             nullptr, 
             nullptr, 
             m_hInst,
             nullptr);
    if (!m_hWnd)
        return E_FAIL;

    ShowWindow(m_hWnd, nCmdShow);

    
    GetClientRect(m_hWnd, &m_rect);
    m_width = m_rect.right - m_rect.left;
    m_height = m_rect.bottom - m_rect.top;

    return S_OK;
}

void Window::update()
{
}

void Window::render()
{
}

void Window::destroy()
{
}

