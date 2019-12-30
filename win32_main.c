#include <stdbool.h>
#include <Windows.h>
static bool running;

int WINAPI
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR     cmd_line,
        int       show_cmd)
{
	WNDCLASSEX wc = {0};	
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = "PugCombat";

	RegisterClassEx(&wc);
	
	HWND window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
				     wc.lpszClassName,
				     "Pug Combat",
				     WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
				     0, 0,
				     800, 600,
				     NULL, NULL, 
				     instance,
				     NULL);
	if (!window)
	{
		return 1;
	}
	ShowWindow(window, show_cmd);

	running = true;
	while (running)
	{
		MSG msg = {0};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
	

}
