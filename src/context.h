#ifndef CONTEXT_H
#define CONTEXT_H

#include "tileset.h"

enum side_e {UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3};

struct context_t
{
    struct tileset_t tileset;
    int **board;
    SDL_Surface *screen;
    int score,
        best_score,
        nb_cells_w,
        nb_cells_h;
    SDL_bool has_reached_max;
};

int load_context(struct context_t *context, const char *path_tielset);

void free_context(struct context_t *context);

#endif
