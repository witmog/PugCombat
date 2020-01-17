#include <game.h>
#include <Windows.h>
#include <windowsx.h>

global Platform platform;

typedef struct BitmapBuffer BitmapBuffer;
struct BitmapBuffer 
{
	BITMAPINFO info;
	void *data;
	i32 width;
	i32 height;
	u32 stride;		
};
global BitmapBuffer global_buffer;

void
platform_set_pixel(i32 x, i32 y, Color color)
{
	if (((color & (0xFF << 24) >> 24) != 0) &&
	    (x >= 0 && x < global_buffer.width) && 
	    (y >= 0 && y < global_buffer.height))
	{
		i32 *bitmap = (i32 *)global_buffer.data;
		bitmap[global_buffer.width*y+x] = color;
	}
}

internal void
win32_display_buffer(HDC device_context,
		     BitmapBuffer *buffer,
		     u32 width, u32 height)
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

internal void
win32_resize_buffer(BitmapBuffer *buffer, u32 width, u32 height)
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

	u32 buffer_data_size = buffer->width*buffer->height * BYTES_PER_PIXEL;
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
			platform.running = false;
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
			platform.left_mouse_down = true;
		} break;
		case WM_LBUTTONUP:
		{
			platform.left_mouse_down = false;
		} break;
		case WM_RBUTTONDOWN:
		{
			platform.right_mouse_down = true;
		} break;
		case WM_RBUTTONUP:
		{
			platform.right_mouse_down = false;
		} break;
		default:
		{
			result = DefWindowProc(window, msg, wparam, lparam);
		}
	}
	return result;
}


i32 WINAPI
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR     cmd_line,
        i32       show_cmd)
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

	DWORD style = WS_SIZEBOX | WS_CAPTION;
	RECT window_rect = {0};
	window_rect.right = platform.screen_width;
	window_rect.bottom = platform.screen_height;

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
	ShowWindow(window, show_cmd);

	platform.running = true;
	game_init(&platform);
	while (platform.running)
	{
		MSG msg = {0};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				platform.running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		// Note: update cursor
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
