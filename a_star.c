#include <stdio.h>
#include <stdlib.h>

#include "a_star.h"

// Struct Declarations
typedef struct
{
	int x;
	int y;
} vector2;

typedef struct _cell_data
{
	float g;
	float h;
	float f;
	int source_x;
	int source_y;
} cell_data;

// Prototypes
vector2 *update_adj_tiles(vector2 current_tile);
cell_data calc_cell_data(cell_data **data, vector2 start, vector2 current, vector2 cell, vector2 finish);

int **ASTAR_get_path(int **navmap, int height, int width, int start_x, int start_y, int finish_x, int finish_y)
{
	// Vars
	vector2 start = (vector2) {start_x, start_y};
	vector2 finish = (vector2) {finish_x, finish_y};
	vector2 current_tile = start;
	vector2 *adj_tiles = calloc(NUM_ADJ_TILES, sizeof(vector2));
	vector2 *checked_tiles = calloc(width * height, sizeof(vector2));
	cell_data **c_data;
	int **correct_path;

	// Forcing startx and starty to be passable
	navmap[start_y][start_x] = 0;

	// Init checked_tiles
	checked_tiles[0] = (vector2) {current_tile.x, current_tile.y};
	int numCheckedTiles = 1;
	
	// Init c_data
	c_data = calloc(height, sizeof(cell_data*));
	for (int i = 0; i < height; i++)
	{
		c_data[i] = calloc(width, sizeof(cell_data));
		for (int j = 0; j < width; j++)
			c_data[i][j] = (cell_data) {0, 9999, 9999};
	}

	// Init correct_path
	correct_path = calloc(height * width, sizeof(int*));
	for (int i = 0; i < height * width; i++)
	{
		correct_path[i] = calloc(2, sizeof(int));
		correct_path[i][0] = END_OF_PATH;
		correct_path[i][1] = END_OF_PATH;
	}

	// Start logic loop
	while (!(current_tile.x == finish.x && current_tile.y == finish.y))
	{
		// Update adjacent tiles
		free(adj_tiles);
		adj_tiles = update_adj_tiles(current_tile);

		// Calculate cell_data for tiles adjacent to current
		for (int i = 0; i < NUM_ADJ_TILES; i++)
		{
			int adj_x = adj_tiles[i].x;
			int adj_y = adj_tiles[i].y;

			if (navmap[adj_y][adj_x] != 1)
				c_data[adj_y][adj_x] = calc_cell_data(c_data, start, current_tile, adj_tiles[i], finish);
		}

		// 
		short tile_checked = 0;
		float best_tile = c_data[0][0].f;
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				// If current col and row is in checked_tiles, flag it as checked and skip over it
				tile_checked = 0;
				for (int k = 0; k < numCheckedTiles; k++)
				{
					if (checked_tiles[k].x == col && checked_tiles[k].y == row)
						tile_checked = 1;
				}
				if (tile_checked) continue;

				if (c_data[row][col].f < best_tile)
				{
					best_tile = c_data[row][col].f;
					current_tile.x = col;
					current_tile.y = row;
				}
				else if (c_data[row][col].f == best_tile)
				{
					if (c_data[row][col].h < c_data[current_tile.y][current_tile.x].h)
					{
						best_tile = c_data[row][col].f;
						current_tile.x = col;
						current_tile.y = row;
					}
				}
			}
		}

		checked_tiles[numCheckedTiles] = (vector2) {current_tile.x, current_tile.y};
		numCheckedTiles++;
	}

	// Filling correct_path with steps to finish
	vector2 path = (vector2) {finish.x, finish.y};
	int path_step_count = 0;
	while (!(path.x == start.x && path.y == start.y))
	{
		int source_x = c_data[path.y][path.x].source_x;
		int source_y = c_data[path.y][path.x].source_y;

		correct_path[path_step_count][0] = path.x;
		correct_path[path_step_count][1] = path.y;

		path.x =	source_x;
		path.y =	source_y;
		path_step_count++;
	}

	// Reversing the path so it starts as move (start + 1) and
	// ends at (finish)
	int n = path_step_count - 1;
	for (int i = 0; i < n; i++)
	{
		int tempx = correct_path[i][0];
		int tempy = correct_path[i][1];
		
		correct_path[i][0] = correct_path[n][0];
		correct_path[i][1] = correct_path[n][1];

		correct_path[n][0] = tempx;
		correct_path[n][1] = tempy;

		n--;
	}

	return correct_path;
}

vector2 *update_adj_tiles(vector2 current_tile)
{
	vector2 *temp_tiles = calloc(8, sizeof(vector2));

	temp_tiles[0] = (vector2) {current_tile.x - 1, current_tile.y - 1};
	temp_tiles[1] = (vector2) {current_tile.x    , current_tile.y - 1};
	temp_tiles[2] = (vector2) {current_tile.x + 1, current_tile.y - 1};
	temp_tiles[3] = (vector2) {current_tile.x - 1, current_tile.y    };
	temp_tiles[4] = (vector2) {current_tile.x + 1, current_tile.y    };
	temp_tiles[5] = (vector2) {current_tile.x - 1, current_tile.y + 1};
	temp_tiles[6] = (vector2) {current_tile.x    , current_tile.y + 1};
	temp_tiles[7] = (vector2) {current_tile.x + 1, current_tile.y + 1};
	
	return temp_tiles;
}

cell_data calc_cell_data(cell_data **data, vector2 start, vector2 current, vector2 cell, vector2 finish)
{
	float g = 0;
	float h = 0;
	float f;
	vector2 cell_copy = (vector2) {cell.x, cell.y};

	if (current.x < cell.x && current.y < cell.y ||
		 current.x < cell.x && current.y > cell.y ||
		 current.x > cell.x && current.y < cell.y ||
		 current.x > cell.x && current.y > cell.y)
		g = data[current.y][current.x].g + 14;
	else
		g = data[current.y][current.x].g + 10;
	
	while (!(cell.x == finish.x && cell.y == finish.y))
	{
		if      (cell.x < finish.x && cell.y < finish.y)
		{
			cell.x++;
			cell.y++;
			h += 14;
		}
		else if (cell.x < finish.x && cell.y > finish.y)
		{
			cell.x++;
			cell.y--;
			h += 14;
		}
		else if (cell.x > finish.x && cell.y < finish.y)
		{
			cell.x--;
			cell.y++;
			h += 14;
		}
		else if (cell.x > finish.x && cell.y > finish.y)
		{
			cell.x--;
			cell.y--;
			h += 14;
		}
		else if (cell.x < finish.x)
		{
			cell.x++;
			h += 10;
		}
		else if (cell.x > finish.x)
		{
			cell.x--;
			h += 10;
		}
		else if (cell.y < finish.y)
		{
			cell.y++;
			h += 10;
		}
		else if (cell.y > finish.y)
		{
			cell.y--;
			h += 10;
		}
	}

	f = g + h;

	if (f < data[cell_copy.y][cell_copy.x].f)
		return (cell_data) {g, h, f, current.x, current.y};
	else
		return data[cell_copy.y][cell_copy.x];
}

