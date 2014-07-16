#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "context.h"

#define EMPTY_CELL 0

#define MAX_VALUE log2(2048)
#define MAX_VALUE_STRING "2048"

void blit_all(struct context_t *context);

void generate_new_cell(struct context_t *context, int value);

int play(struct context_t *context);

int move_cell(struct context_t *context, int x, int y, int vecteur[2]);

int move(struct context_t *context, enum side_e side);

void ask_for_screenshot(struct context_t *context);

SDL_bool check_if_max(struct context_t *context);

int get_max_value(const struct context_t *context);

SDL_bool isover(struct context_t *context);

#endif
