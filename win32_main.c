#include <game.h>
#include <Windows.h>
#include <windowsx.h>

static Platform platform;

typedef struct 
{
	BITMAPINFO info;
	void *data;
	int width;
	int height;
	int stride;		
} BitmapBuffer;

static BitmapBuffer global_buffer;

void
platform_set_pixel(int x, int y, Color color)
{
	if ((x < 0 || x >= global_buffer.width) || 
	    (y < 0 || y >= global_buffer.height)) 
	{
		return;
	}
	((int *)global_buffer.data)[global_buffer.width*y+x] = color;
}

static void
win32_display_buffer(HDC device_context,
		     BitmapBuffer *buffer,
		     int width, int height)
{
	StretchDIBits(device_context,
		      // destination
		      0, 0,
		      width, height,
		      // source
		      0, 0,
		      buffer->width, buffer->height,
		      buffer->data,
		      &buffer->info,
		      DIB_RGB_COLORS,
		      SRCCOPY);
}

static void
win32_resize_buffer(BitmapBuffer *buffer, int unsigned width, int unsigned height)
{
	if (buffer->data)
	{
		VirtualFree(buffer->data, 0, MEM_RELEASE);
	}
	buffer->width = width;
	buffer->height = height;
	buffer->stride = buffer->width*BYTES_PER_PIXEL;
	
	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = buffer->width;
	buffer->info.bmiHeader.biHeight = -buffer->height;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = 32;
	buffer->info.bmiHeader.biCompression = BI_RGB;
	buffer->info.bmiHeader.biSizeImage = 0;
	buffer->info.bmiHeader.biClrUsed = 0;

	int buffer_data_size = buffer->width*buffer->height * BYTES_PER_PIXEL;
	buffer->data = VirtualAlloc(NULL, buffer_data_size, MEM_COMMIT, PAGE_READWRITE);


}

LRESULT WINAPI
win32_window_proc(HWND   window,
	   UINT   msg,
	   WPARAM wparam,
	   LPARAM lparam)
{
	LRESULT result = 0;
	switch (msg)
	{
		case WM_CLOSE:
		{
			platform.running = 0;
		} break;
		case WM_SIZE:
		{
			RECT client_rect;
			GetClientRect(window, &client_rect);
			platform.screen_width = client_rect.right;
			platform.screen_height = client_rect.bottom;
			win32_resize_buffer(&global_buffer, 
					    platform.screen_width,
					    platform.screen_height);	
		} break;
		case WM_LBUTTONDOWN:
		{
			platform.left_mouse_down = 1;
		} break;
		case WM_LBUTTONUP:
		{
			platform.left_mouse_down = 0;
		} break;
		case WM_RBUTTONDOWN:
		{
			platform.right_mouse_down = 1;
		} break;
		case WM_RBUTTONUP:
		{
			platform.right_mouse_down = 0;
		} break;
		default:
		{
			result = DefWindowProc(window, msg, wparam, lparam);
		}
	}
	return result;
}


int WINAPI
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR     cmd_line,
        int       show_cmd)
{
	WNDCLASSEX wc = {0};	
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = win32_window_proc;
	wc.hInstance = instance;
	wc.lpszClassName = "PugCombat";
	RegisterClassEx(&wc);

	platform.screen_width = 800;
	platform.screen_height = 600;

	RECT window_rect = {0};
	window_rect.right = platform.screen_width;
	window_rect.bottom = platform.screen_height;

	DWORD style = WS_SIZEBOX | WS_CAPTION;
	AdjustWindowRectEx(&window_rect, style, 0, 0);

	HWND window = CreateWindowEx(0,
				     wc.lpszClassName,
				     "Pug Combat",
				     style,
				     0, 0,
				     window_rect.right,
				     window_rect.bottom,
				     0, 0, 
				     instance,
				     0);
	if (!window)
	{
		return 1;
	}
	win32_resize_buffer(&global_buffer, platform.screen_width, platform.screen_height);
	game_init(&platform);
	ShowWindow(window, show_cmd);

	platform.running = 1;
	while (platform.running)
	{
		MSG msg = {0};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				platform.running = 0;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		{
			POINT pt;
			GetCursorPos(&pt);		
			ScreenToClient(window, &pt);
			platform.mouse_x = pt.x;
			platform.mouse_y = pt.y;
		}

		game_loop();


		HDC device_context = GetDC(window);
		win32_display_buffer(device_context,
				     &global_buffer,
				     platform.screen_width, 
				     platform.screen_height);	
		ReleaseDC(window, device_context);
	}
	return 0;
	

}
