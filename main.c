#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "grid.h"
#include "a_star.h"

int main(int argc, char **argv)
{
	grid *g = init_grid(20, 20);
	int **g_navmap = generate_nav_map(g);

	int sX = 5;
	int sY = 5;
	int fX = 12;
	int fY = 11;

	int **p = ASTAR_get_path(g_navmap, g->height, g->width, sX, sY, fX, fY);

	printf("Path steps (x, y):\n");
	int i = 0;
	while (!(p[i][0] == END_OF_PATH && p[i][0] == END_OF_PATH))
	{
		putchar('\t');
		printf("(%d, %d)\n", p[i][0], p[i][1]);
		if (!(p[i][0] == fX && p[i][1] == fY))
			g->data[p[i][1]][p[i][0]] = '.';
		i++;
	}
	print_grid(g);

	free_grid(g);
	return 0;
}
