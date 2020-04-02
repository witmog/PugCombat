static struct Platform linux_platform;
#include <SDL2/SDL.h>

#include <stdlib.h>
#include "logger.h"
#include "game.h"

#include "game.c"


struct PixelBuffer
{
    int width;
    int height;
    int pitch;
    uint32_t *pixels;
    int pixel_length;
    SDL_Texture *texture;
};

static struct PixelBuffer pixel_buffer;



void
Platform_SetPixel(int x, int y, uint32_t color)
{
    if ((x < pixel_buffer.width && x >= 0 && x < linux_platform.game_width) &&
	(y < pixel_buffer.height && y >= 0 && y < linux_platform.game_height))
    {
	pixel_buffer.pixels[pixel_buffer.width*y + x] = color;
    }
}


static SDL_Rect
Linux_ResizeBuffer(SDL_Renderer *renderer, struct PixelBuffer *buffer, int width, int height)
{
    SDL_RenderClear(renderer);
    if (buffer->texture)
    {
	SDL_DestroyTexture(buffer->texture);
    }
    if (buffer->pixels)
    {
	free(buffer->pixels);
    }

    buffer->texture = SDL_CreateTexture(renderer,
					SDL_PIXELFORMAT_ARGB8888,
					SDL_TEXTUREACCESS_STREAMING,
					width,
					height);


    buffer->pixels = malloc(width*height*BYTES_PER_PIXEL);
    buffer->pixel_length = MAX(1, width/linux_platform.game_width);
    buffer->pitch = BYTES_PER_PIXEL*width;
    buffer->width = width;
    buffer->height = height;

    SDL_Rect game_area_offset;
    game_area_offset.x = MAX(0, (buffer->width - linux_platform.game_width*buffer->pixel_length) / 2);
    game_area_offset.y = MAX(0, (buffer->height - linux_platform.game_height*buffer->pixel_length) / 2);
    game_area_offset.w = buffer->width*buffer->pixel_length;
    game_area_offset.h = buffer->height*buffer->pixel_length;
    return game_area_offset;
}

static void
Linux_DisplayBuffer(SDL_Renderer* renderer,
		    struct PixelBuffer *buffer,
		    SDL_Rect game_area_offset)
{
    // Note: returns 0 on success
    if (SDL_UpdateTexture(buffer->texture,
			  NULL,
			  buffer->pixels,
			  buffer->pitch))
    {
	LOG_ERROR("Failed to update pixel data.\nRemind the dev to use SDL_GetError for details.");
    }

    SDL_RenderCopy(renderer,
		   buffer->texture,
		   NULL,
		   &game_area_offset);
    SDL_RenderPresent(renderer);
	
}

int
main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
	LOG_FATAL("Failed to init SDL");
	return -1;
    }
    SDL_Window *window = SDL_CreateWindow("Pug Combat",
					  SDL_WINDOWPOS_UNDEFINED,
					  SDL_WINDOWPOS_UNDEFINED,
					  800, 600,
					  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    linux_platform.game_width = 320;
    linux_platform.game_height = 180;
    SDL_Rect game_area_offset;
    game_area_offset = Linux_ResizeBuffer(renderer, &pixel_buffer, 800, 600);

    LOG_TRACE("Init game area %d x %dpx",
	      linux_platform.game_width,
	      linux_platform.game_height);
    InitGame(&linux_platform);

    linux_platform.running = true;
    while (linux_platform.running)
    {

	int mouse_x;
	int mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	linux_platform.mouse_x = (mouse_x - game_area_offset.x) / pixel_buffer.pixel_length;
	linux_platform.mouse_y = (mouse_y - game_area_offset.y) / pixel_buffer.pixel_length;
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
	    linux_platform.left_mouse_down = true;
	}
	else
	{
	    linux_platform.left_mouse_down = false;
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
	    linux_platform.right_mouse_down = true;
	}
	else
	{
	    linux_platform.right_mouse_down = false;
	}
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
	    switch (event.type)
	    {
	    case SDL_QUIT:
	    {
		linux_platform.running = false;
	    } break;
	    case SDL_KEYDOWN:
	    {
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
		    linux_platform.running = false;
		}
	    } break;
	    case SDL_WINDOWEVENT:
	    {
		if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
		    game_area_offset = Linux_ResizeBuffer(renderer,
							  &pixel_buffer, 
							  event.window.data1,
							  event.window.data2);
	       
		}
	    } break;
	    }
	}
	memset(pixel_buffer.pixels,
	       0,
	       pixel_buffer.width*pixel_buffer.height*BYTES_PER_PIXEL);
	UpdateGame();

	Linux_DisplayBuffer(renderer, &pixel_buffer, game_area_offset);

    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
