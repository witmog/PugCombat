#include <Windows.h>
#include <windowsx.h>
#include "game.h"
#include "logger.h"

#include "game.c"

static struct Platform win32_platform;
static struct Rect game_area;
static struct PixelBuffer pixel_buffer;

struct PixelBuffer 
{
    BITMAPINFO info;
    void *pixels;
    int width;
    int height;
    int pitch;		
    int pixel_width;	
};

struct Rect
{
    int x;
    int y;
    int width;
    int height;
};

void
Platform_SetPixel(int x, int y, color_t color)
{
    if ((x >= 0 && x < win32_platform.game_width && x < pixel_buffer.width) && 
	(y >= 0 && y < win32_platform.game_height && y < pixel_buffer.height))
    {
	color_t *buffer = (color_t *)pixel_buffer.pixels;
	buffer[pixel_buffer.width*y + x] = color;
    }
}

static void
Win32_DisplayBuffer(HDC device_context,
		    struct PixelBuffer *buffer,
		    struct Rect game_area)
{
    StretchDIBits(device_context,
                  // destination
                  0, 0,
                  game_area.width, game_area.height,
                  // source
                  0, 0,
                  buffer->width, buffer->height,
                  buffer->pixels,
                  &buffer->info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

static struct Rect
Win32_ResizeBuffer(struct PixelBuffer *buffer, int width, int height)
{
    if (buffer->pixels)
    {
	ZeroMemory(buffer->pixels, buffer->width*buffer->height*BYTES_PER_PIXEL);
	VirtualFree(buffer->pixels, 0, MEM_RELEASE);
    }
    buffer->width = width;
    buffer->height = height;
    buffer->pitch = width*BYTES_PER_PIXEL;
	
    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = -buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;
    buffer->info.bmiHeader.biSizeImage = 0;
    buffer->info.bmiHeader.biClrUsed = 0;

    buffer->pixel_width = MAX(1, width/win32_platform.game_width);

    struct Rect game_area;
    game_area.width = buffer->width*buffer->pixel_width;
    game_area.height = buffer->height*buffer->pixel_width;

    size_t buffer_data_size = buffer->width*buffer->height * BYTES_PER_PIXEL;
    buffer->pixels = VirtualAlloc(NULL, buffer_data_size, MEM_COMMIT, PAGE_READWRITE);

    return game_area;

}

LRESULT WINAPI
Win32_ProcessMessages(HWND   window,
		      UINT   msg,
		      WPARAM wparam,
		      LPARAM lparam)
{
    LRESULT result = 0;
    switch (msg)
    {
    case WM_CLOSE:
    {
	win32_platform.running = false;
    } break;
    case WM_SIZE:
    {
	RECT client_rect;
	GetClientRect(window, &client_rect);
	game_area = Win32_ResizeBuffer(&pixel_buffer, 
				       client_rect.right,
				       client_rect.bottom);	
    } break;
    case WM_LBUTTONDOWN:
    {
	win32_platform.left_mouse_down = true;
    } break;
    case WM_LBUTTONUP:
    {
	win32_platform.left_mouse_down = false;
    } break;
    case WM_RBUTTONDOWN:
    {
	win32_platform.right_mouse_down = true;
    } break;
    case WM_RBUTTONUP:
    {
	win32_platform.right_mouse_down = false;
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
        LPSTR cmd_line,
        int show_cmd)
{
    WNDCLASSEX wc = {0};	
    wc.cbSize = sizeof(WNDCLASSEX); 
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Win32_ProcessMessages;
    wc.hInstance = instance;
    wc.lpszClassName = "Pug Combat";
    RegisterClassEx(&wc);

    win32_platform.game_width = 320;
    win32_platform.game_height = 180;

    DWORD style = WS_SIZEBOX | WS_CAPTION | WS_BORDER | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;
    RECT window_rect = {0};
    window_rect.right = 800;
    window_rect.bottom = 600;

    AdjustWindowRectEx(&window_rect, style, 0, 0);

    HWND window = CreateWindowEx(0,
				 wc.lpszClassName,
				 "Pug Combat",
				 style,
				 CW_USEDEFAULT, CW_USEDEFAULT,
				 window_rect.right,
				 window_rect.bottom,
				 0, 0, 
				 instance,
				 0);
    if (!window)
    {
	LOG_FATAL("Failed to create window");
	return 1;
    }
    game_area = Win32_ResizeBuffer(&pixel_buffer, window_rect.right, window_rect.bottom);
    ShowWindow(window, show_cmd);

    win32_platform.running = true;
    InitGame(&win32_platform);

    while (win32_platform.running)
    {
	MSG msg = {0};
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
	    if (msg.message == WM_QUIT)
	    {
		win32_platform.running = false;
	    }
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
		

	{
	    POINT pt;
	    GetCursorPos(&pt);		
	    ScreenToClient(window, &pt);
	    win32_platform.mouse_x = (pt.x) / pixel_buffer.pixel_width;
	    win32_platform.mouse_y = (pt.y) / pixel_buffer.pixel_width;;
	}

	UpdateGame();


	HDC device_context = GetDC(window);
	Win32_DisplayBuffer(device_context,
			    &pixel_buffer,
			    game_area); 
	ReleaseDC(window, device_context);
    }
    return 0;
	

}
