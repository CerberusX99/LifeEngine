#pragma once
#include "Prerequisites.h"

class Window {
public:
    // Constructor de la clase Window.
    Window();

    // Destructor de la clase Window.
    ~Window();

    // Inicializa la ventana con los par�metros dados.
    // Retorna un c�digo de error HRESULT.
    HRESULT init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

    // Actualiza la ventana (puede implementarse seg�n sea necesario).
    void update();

    // Renderiza el contenido de la ventana (puede implementarse seg�n sea necesario).
    void render();

    // Destruye la ventana y libera los recursos asociados.
    void destroy();

public:
    HWND  m_hWnd = nullptr;  // Handle de la ventana.
    unsigned int m_width;    // Ancho de la ventana.
    unsigned int m_height;   // Alto de la ventana.

private:
    HINSTANCE m_hInst = nullptr;  // Instancia de la aplicaci�n.
    RECT m_rect;                  // Estructura RECT que representa el rect�ngulo de la ventana.
    std::string m_windowName = "LifeEngine";  // Nombre de la ventana.
};
