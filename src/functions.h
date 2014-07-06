#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "context.h"

#define EMPTY_CELL 0

void blit_all(struct context_t *context);

void generate_new_cell(struct context_t *context, int value);

int play(struct context_t *context);

int move_cell(struct context_t *context, int x, int y, int vecteur[2]);

int move(struct context_t *context, enum side_e side);

SDL_bool isover(const struct context_t *context);

#endif
