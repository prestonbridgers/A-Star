#ifndef A_STAR_H_INCLUDED
#define A_STAR_H_INCLUDED

#define END_OF_PATH -999
#define NUM_ADJ_TILES 8

int **ASTAR_get_path(int **navmap, int height, int width, int start_x, int start_y, int finish_x, int finish_y);

#endif
