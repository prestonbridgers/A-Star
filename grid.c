#include <stdio.h>
#include <stdlib.h>

#include "grid.h"

#define WALL_TILE '#'
#define GROUND_TILE ' '
#define START_TILE 'S'
#define FINISH_TILE 'F'

grid *init_grid(int w, int h)
{
	grid *g = calloc(1, sizeof(grid));

	g->width = w;
	g->height = h;

	g->data = calloc(h, sizeof(char*));
	for (int i = 0; i < h; i++)
	{
		g->data[i] = calloc(w, sizeof(char));
		for (int j = 0; j < w; j++)
		{
			g->data[i][j] = GROUND_TILE;

			if (i == 0 || i == h - 1 ||
			    j == 0 || j == w - 1)
			g->data[i][j] = WALL_TILE;
		}
	}

	g->data[5][5] = START_TILE;
	g->data[11][12] = FINISH_TILE;

	
	g->data[7][6] = WALL_TILE;
	g->data[7][7] = WALL_TILE;
	g->data[7][10] = WALL_TILE;
	g->data[8][6] = WALL_TILE;
	g->data[8][7] = WALL_TILE;
	//g->data[8][8] = WALL_TILE;
	g->data[8][9] = WALL_TILE;
	g->data[8][10] = WALL_TILE;
	g->data[9][9] = WALL_TILE;
	g->data[9][10] = WALL_TILE;

	return g;
}

void free_grid(grid *g)
{
	for (int i = 0; i < g->height; i++)
		free(g->data[i]);
	free(g->data);
	free(g);
}

void print_grid(grid *g)
{
	for (int i = 0; i < g->height; i++)
	{
		for (int j = 0; j < g->width; j++)
		{
			putchar(g->data[i][j]);
		}
		putchar('\n');
	}
}

int **generate_nav_map(grid *g)
{
	int **map = calloc(g->height, sizeof(int*));
	for (int i = 0; i < g->height; i++)
	{
		map[i] = calloc(g->width, sizeof(int));
		for (int j = 0; j < g->width; j++)
		{
			char c = g->data[i][j];
	
			switch (c)
			{
				case WALL_TILE:
					map[i][j] = 1;
					break;
				default:
					map[i][j] = 0;
					break;
			}
		}
	}

	return map;
}

