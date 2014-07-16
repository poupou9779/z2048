#include "context.h"

int load_context(struct context_t *context, const char *path_context)
{
    int i, j;
    char path_tileset[LEN_MAX];
    FILE *f = fopen(path_context, "r");
    FILE *file_best_score = fopen("data\\b_s.bin", "rb");
    fscanf(f, "%d %d", &context->nb_cells_w, &context->nb_cells_h);
    fscanf(f, "%s", path_tileset);
    context->board = malloc(sizeof(*context->board)*context->nb_cells_h);
    for(i = 0; i < context->nb_cells_h; ++i)
        context->board[i] = malloc(sizeof(**context->board)*context->nb_cells_w);
    if(load_tileset(&context->tileset, path_tileset) == 0)
        return 0;
    context->has_reached_max = SDL_FALSE;
    context->screen = SDL_SetVideoMode(context->nb_cells_w*context->tileset.tab_pos[0].w,
                                       context->nb_cells_h*context->tileset.tab_pos[0].h,
                                       32,
                                       SDL_HWSURFACE|SDL_DOUBLEBUF);
    if(context->screen == NULL)
    {
        fputs("Unable to create screen\n", stderr);
        free_tileset(&context->tileset);
        return 0;
    }
    for(i = 0; i < context->nb_cells_h; ++i)
        for(j = 0; j < context->nb_cells_w; ++j)
            context->board[i][j] = 0;
    context->score = 0;
    fread(&context->best_score, sizeof(int), 1, file_best_score);
    fclose(file_best_score);
    fclose(f);
    SDL_WM_SetIcon(SDL_LoadBMP("data\\icon.bmp"), NULL);
    return 1;
}

void free_context(struct context_t *context)
{
    SDL_FreeSurface(context->screen);
    free_tileset(&context->tileset);
}
