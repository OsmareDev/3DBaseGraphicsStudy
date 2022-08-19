#include "Window.h"

Window Window::main_window;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            Window::GetInstance().window_running = false;
        } break;
        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(Window::GetInstance().window, &rect);
            Window::GetInstance().window_width = rect.right - rect.left;
            Window::GetInstance().window_height = rect.bottom - rect.top;

            Window::GetInstance().RecalculateMemory();
        }
        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

void Window::RecalculateMemory()
{
    bufferSize = window_width * window_height * sizeof(unsigned int);

    // si ya habia memoria reservada se libera
    if (Window::GetInstance().memory) VirtualFree(memory, 0, MEM_RELEASE);

    memory = VirtualAlloc(
        0,
        bufferSize,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = window_width;
    bitmap_info.bmiHeader.biHeight = window_height;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
}

void Window::StartLoop()
{
    RunningLoop();
}

Window::Window()
{
    // de momento parece que sirve
    hInstance = GetModuleHandle(NULL);
    nCmdShow = 10;
    
    RecalculateMemory();

    DefineClass();
    WindowCreate();
}

void Window::DefineClass()
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) { MessageBoxW(nullptr, L"Error registrando la clase", L"Error registrando la clase", MB_OK); }
}

void Window::WindowCreate()
{
    window = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        CLASS_NAME,                     // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // pos.x, pos.y, width, height
        0, 0, window_width, window_height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (window == NULL) { MessageBoxW(nullptr, L"Error creando ventana", L"Error creando ventana", MB_OK); }
    else { ShowWindow(window, nCmdShow); hdc = GetDC(window); window_running = true; }
}

void Window::RunningLoop()
{
    // Message Loop
    MSG msg = { };

    while (window_running)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Draw Loop
        Render();

        StretchDIBits(
            hdc,
            0,
            0,
            window_width,
            window_height,
            0,
            0,
            window_width,
            window_height,
            memory,
            &bitmap_info,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }
}

void Window::PaintPixel(const int& x, const int& y, const uint32_t& color) const 
{
    uint32_t* pixel = (uint32_t*)memory + (y * window_width) + x;
    *pixel = color;
}
