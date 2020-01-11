#include <game.h>
#include <stdbool.h>
#include <Windows.h>
static bool running;

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
	if ((x < 0 || x > global_buffer.width) || 
	    (y < 0 || y > global_buffer.height)) 
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
win32_resize_buffer(BitmapBuffer *buffer, int width, int height)
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
			running = false;
		} break;
		case WM_SIZE:
		{
			RECT client;
			GetClientRect(window, &client);
			int unsigned width = client.right - client.left;
			int unsigned height = client.bottom - client.top;
			win32_resize_buffer(&global_buffer, width, height);	
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

	DWORD style = WS_SIZEBOX | WS_CAPTION;
	RECT dimensions = {0};
	dimensions.right = 800;
	dimensions.bottom = 600;
	AdjustWindowRectEx(&dimensions, style, 0, 0);

	int unsigned window_width = dimensions.right - dimensions.left;
	int unsigned window_height = dimensions.bottom - dimensions.top;
	
	win32_resize_buffer(&global_buffer, dimensions.right, dimensions.bottom);
	
	HWND window = CreateWindowEx(0,
				     wc.lpszClassName,
				     "Pug Combat",
				     style,
				     0, 0,
				     window_width, window_height,
				     0, 0, 
				     instance,
				     0);
	if (!window)
	{
		return 1;
	}
	game_init();
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

		game_loop();

		HDC device_context = GetDC(window);
		RECT client_rect;
		GetClientRect(window, &client_rect);
		win32_display_buffer(device_context, &global_buffer, client_rect.right, client_rect.bottom);	
		ReleaseDC(window, device_context);
	}
	return 0;
	

}
