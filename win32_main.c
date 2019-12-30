#include <Windows.h>

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
	

}
