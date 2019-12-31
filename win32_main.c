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
	int bytes_per_pixel = 4;
	buffer->width = width;
	buffer->height = height;
	buffer->stride = buffer->width*bytes_per_pixel;
	
	buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
	buffer->info.bmiHeader.biWidth = buffer->width;
	buffer->info.bmiHeader.biHeight = -buffer->height;
	buffer->info.bmiHeader.biPlanes = 1;
	buffer->info.bmiHeader.biBitCount = 32;
	buffer->info.bmiHeader.biCompression = BI_RGB;
	buffer->info.bmiHeader.biSizeImage = 0;
	buffer->info.bmiHeader.biClrUsed = 0;

	int buffer_data_size = buffer->width*buffer->height * 4;
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
	win32_resize_buffer(&global_buffer, 800, 600);
	
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
		HDC device_context = GetDC(window);

		{
			unsigned char *row = (unsigned char *)global_buffer.data;
			for (int y = 0; y < global_buffer.height; ++y)
			{
					
				int *pixel = (int *)row; 
				for (int x = 0; x < global_buffer.width; ++x)
				{
					
					*pixel++ = (255 << 16) | (255 << 8) | (0);
				}
				row += global_buffer.stride;
			}
		}

		win32_display_buffer(device_context, &global_buffer, 800, 600);	
		ReleaseDC(window, device_context);
	}
	return 0;
	

}
