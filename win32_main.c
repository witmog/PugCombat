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

}
