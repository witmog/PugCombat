#include "chunk.h"

#include "game.h"
#include "blit.h"

static int dir = 0;
// Returns -1 on failure.
int
GetChunkAt(int x, int y)
{
	int i = (x/CHUNK_WIDTH);
	int j = (y/CHUNK_HEIGHT);
	if (i >= WORLD_WIDTH || i < 0 || j >= WORLD_HEIGHT || j < 0)
	{
		return -1;
	}
	return WORLD_WIDTH*j + i;
}

// Returns -1 on failure.
int
GetBlockAt(int x, int y)
{

	int i = x % CHUNK_WIDTH;
	int j = y % CHUNK_HEIGHT;
	if (i >= CHUNK_WIDTH || i < 0 || j >= CHUNK_HEIGHT || j < 0)
	{
		return -1;
	}
	return CHUNK_WIDTH*j + i;
}


static block_t*
CreateChunk(int width, int height)
{
	block_t *chunk = malloc(sizeof(block_t) * width*height);
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{

			chunk[width*j + i] = NEW_BLOCK(BLOCKTYPE_EMPTY, 0);

		}
	}
	return chunk;
}

static bool
HasBlock(block_t **world, int x, int y, enum BlockType type)
{
	int chunk_location;
	int block_location;
	if (((chunk_location = GetChunkAt(x, y)) != -1) &&
	    ((block_location = GetBlockAt(x, y)) != -1) &&
	    (BLOCK_TYPE(world[chunk_location][block_location]) == type))
	{
		return true;
	}
	return false;

}

static bool
MoveBlock(block_t **world, block_t *current, int x, int y)
{
	int chunk_location;
	int block_location;
	if (((chunk_location = GetChunkAt(x, y)) != -1) &&
	    ((block_location = GetBlockAt(x, y)) != -1) &&
	    (BLOCK_TYPE(world[chunk_location][block_location]) == BLOCKTYPE_EMPTY))
	{
		world[chunk_location][block_location] = NEW_BLOCK(BLOCK_TYPE(*current), 1);
		*current = NEW_BLOCK(BLOCKTYPE_EMPTY, 1);

		return true;
	}
	return false;

}


static void
UpdateChunk(block_t **world, int x, int y)
{


	block_t *chunk = world[WORLD_WIDTH*y + x];
	for (int j = CHUNK_HEIGHT-1; j >= 0; --j)
	{
		for (int i = 0; i < CHUNK_WIDTH; ++i)
		{
			block_t *current = chunk + (CHUNK_WIDTH*j + i);

			if ((BLOCK_TYPE(*current) == BLOCKTYPE_EMPTY) ||
			    (BLOCK_TYPE(*current) == BLOCKTYPE_SOLID) ||
			    (BLOCK_UPDATE(*current))) 
			{
				continue;
			}

			int world_x = x*CHUNK_WIDTH + i;
			int world_y = y*CHUNK_HEIGHT + j;

			if (BLOCK_TYPE(*current) == BLOCKTYPE_WATER)
			{
				if (dir == 0)
				{

					if (MoveBlock(world, current, world_x, world_y+1))
					{
					}
					else if (HasBlock(world, world_x+1, world_y, BLOCKTYPE_EMPTY) ||
						 HasBlock(world, world_x+1, world_y+1, BLOCKTYPE_EMPTY))
					{
						dir = 1;
					}
					else if (HasBlock(world, world_x-1, world_y, BLOCKTYPE_EMPTY) ||
						 HasBlock(world, world_x-1, world_y+1, BLOCKTYPE_EMPTY))
					{
						dir = -1;
					}
					}
				else if (dir == 1)
				{
					if (MoveBlock(world, current, world_x+1, world_y+1))
					{
					}
					else if (MoveBlock(world, current, world_x+1, world_y))
					{
					}
					else
					{
						dir = 0;
					}
							
					}
				else if (dir == -1)
				{
					if (MoveBlock(world, current, world_x-1, world_y+1))
					{
					}
					else if (MoveBlock(world, current, world_x-1, world_y))
					{
					}
					else
					{
						dir = 0;
					}
							
				}
			}
		}
	}

}

static color_t
GetBlockColor(enum BlockType type)
{
	switch (type)
	{
		case BLOCKTYPE_WATER:
			{
				return 0xffffffff;
			} break;
		case BLOCKTYPE_SOLID:
			{
				return 0xff9b7653;
			} break;
		default:
			{
				return 0x0;
				break;
			}
	}
}

static void
RenderChunk(block_t *chunk, int x, int y)
{
	for (int j = 0; j < CHUNK_HEIGHT; ++j)
	{
		for (int i = 0; i < CHUNK_WIDTH; ++i)
		{

			enum BlockType type = BLOCK_TYPE(chunk[CHUNK_WIDTH*j + i]);
			if (type != BLOCKTYPE_EMPTY)
			{
				DrawPoint(i+x, j+y, GetBlockColor(type));

				chunk[CHUNK_WIDTH*j + i] = NEW_BLOCK(type, 0);

			}
		}
	}
}


block_t **
CreateWorld(int width, int height)
{
	block_t **world = (block_t**)malloc(sizeof(block_t*) * width*height);

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			world[width*j + i] = CreateChunk(CHUNK_WIDTH, CHUNK_HEIGHT);
		}
	}
	return world;
}

void
DeleteWorld(block_t **world)
{
	if (world == NULL)
	{
		log_warn("Attempted to delete uninitialized world, aborting.");
		return;
	}

	for (int i = 0; i < WORLD_WIDTH*WORLD_HEIGHT; ++i)
	{
		free(world[i]);
	}
	free(world);
}

void
UpdateWorld(block_t **world)
{
	for (int j = WORLD_HEIGHT-1; j >= 0; --j)
	{
		for (int i = 0; i < WORLD_WIDTH; ++i)
		{
			UpdateChunk(world, i, j);
		}
	}
}

void
RenderWorld(block_t **world)
{
	for (int j = 0; j < WORLD_HEIGHT; ++j)
	{
		for (int i = 0; i < WORLD_WIDTH; ++i)
		{
			RenderChunk(world[WORLD_WIDTH*j + i], i*CHUNK_WIDTH, j*CHUNK_HEIGHT);
		}
	}
}


