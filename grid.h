#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

typedef struct
{
	int width;
	int height;
	char **data;
} grid;

grid *init_grid(int w, int h);
void free_grid(grid *g);
void print_grid(grid *g);
int **generate_nav_map(grid *g);

#endif
