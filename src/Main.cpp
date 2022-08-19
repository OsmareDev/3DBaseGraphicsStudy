#include "Render.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Window::GetInstance().SetRender(Render::Renderize);
    Window::GetInstance().StartLoop();

    return 0;
}