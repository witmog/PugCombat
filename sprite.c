#include <sprite.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void
sprite_draw(i32 x, i32 y, Sprite *sprite)
{
	for (u32 j = 0; j < sprite->height; ++j)
	{
		for (u32 i = 0; i < sprite->width; ++i)
		{
			platform_set_pixel(i+x, j+y, sprite->data[j*sprite->width + i]);	
		}
	}
}

void
fill_circle_in_sprite(i32 x, i32 y,
		      Sprite *sprite,
		      u32 radius,
		      Color color)
{
	for (i32 i = radius; i > 1; --i)
	{
		for (u32 angle = 0; angle < 360; ++angle)
		{
			i32 i = (i32)(x + radius * (sin(angle) * cos(angle)));
			i32 j = (i32)(y + radius * (sin(angle) * sin(angle)));
			if ((i >= 0 && i < sprite->width) && 
			    (j >= 0 && j < sprite->height))
			{
				sprite->data[i + j * sprite->width] = color;
			}
		}
		--radius;
	}
}

void
sprite_to_disk(char *filepath, Sprite *sprite)
{
	FILE *file = fopen(filepath, "wb");
	if (!file) return;
	fwrite(&sprite->width, sizeof(u32), 1, file);
	fwrite(&sprite->height, sizeof(u32), 1, file);
	fwrite(sprite->data, sizeof(u32), sprite->width*sprite->height, file);
	fclose(file);	
}

Sprite
sprite_from_file(char *filepath)
{
	FILE *file = fopen(filepath, "rb");

	u32 width;
	u32 height;
	fread(&width, sizeof(width), 1, file);
	fread(&height, sizeof(height), 1, file);
	u32 pixel_count = width * height;
	Color *data = malloc(sizeof(Color) * pixel_count);
	fread(data, sizeof(Color), pixel_count, file);
	fclose(file);

	Sprite result;
	result.width = width;
	result.height = height;
	result.data = data;

	return result;
}
