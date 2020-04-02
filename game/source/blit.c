#include "blit.h"

void
DrawPoint(int x, int y, color_t color)
{
    Platform_SetPixel(x, y, color);
}

void
DrawRectangle(int x, int y, int width, int height, color_t color)
{
    for (int j = 0; j < height; ++j)
    {
	for (int i = 0; i < width; ++i)
	{

	    Platform_SetPixel(x + i, y + j, color);
	}
    }
}

void
DrawCircle(int x, int y, int radius, color_t color)
{
    for (int i = radius; i > 1; --i)
    {
	for (int angle = 0; angle < 360; ++angle)
	{
	    int i = (int)(x + radius*(sin(angle) *
				      cos(angle)));
	    int j = (int)(y + radius*(sin(angle) *
				      sin(angle)));
	    Platform_SetPixel(i, j, color);
	}
	--radius;
    }
}


static void
DrawLineBelow(int x0, int y0, int x1, int y1, color_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int j = 1;
    if (dy < 0)
    {
	j = -1;
	dy = -dy;
    }

    int d = 2*dy - dx;
    int y = y0;

    for (int x = x0; x < x1; ++x)
    {
	Platform_SetPixel(x, y, color);
	if (d > 0)
	{
	    y = y + j;
	    d = d - 2*dx;
	}
	d = d + 2*dy;
    }
}

static void
DrawLineAbove(int x0, int y0, int x1, int y1, color_t color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int xi = 1;
    if (dx < 0)
    {
	xi = -1;
	dx = -dx;
    }
    int d = 2*dx - dy;
    int x = x0;

    for (int y = y0; y < y1; ++y)
    {
	Platform_SetPixel(x, y, color);

	if (d > 0)
	{
	    x = x + xi;
	    d = d - 2*dy;
	}
	d = d + 2*dx;
    }
}
/* Using Bresenham algorithm */
void
DrawLine(int x0, int y0, int x1, int y1, color_t color)
{
    if (abs(y1 - y0) < abs(x1 - x0))
    {
	if (x0 > x1)
	{
	    DrawLineBelow(x1, y1, x0, y0, color);
	}
	else
	{
	    DrawLineBelow(x0, y0, x1, y1, color);
	}
    }
    else
    {
	if (y0 > y1)
	{
	    DrawLineAbove(x1, y1, x0, y0, color);
	}
	else
	{
	    DrawLineAbove(x0, y0, x1, y1, color);
	}
    }
}
