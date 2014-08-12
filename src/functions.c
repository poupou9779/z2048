#include "functions.h"

#define NEW_CELL_VALUE_2 1
#define NEW_CELL_VALUE_4 2

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void blit_all(struct context_t *context)
{
    int i, j;
    SDL_Rect dstrect;
    dstrect.h = context->tileset.tab_pos[0].h;
    dstrect.w = context->tileset.tab_pos[0].w;
    for(i = 0; i < context->nb_cells_h; ++i)
    {
        dstrect.y = i*dstrect.h;
        for(j = 0; j < context->nb_cells_w; ++j)
        {
            dstrect.x = j*dstrect.w;
            SDL_BlitSurface(context->tileset.im, &context->tileset.tab_pos[context->board[i][j]], context->screen, &dstrect);
        }
    }
}

void generate_new_cell(struct context_t *context, int value)
{
    int x, y;
    do
    {
        x = rand()%context->nb_cells_w;
        y = rand()%context->nb_cells_h;
    } while(context->board[y][x] != 0);
    context->board[y][x] = value;
}

SDL_bool isover(const struct context_t *context)
{
    int i, j, k,
        vx[] = {-1,  0,  0, +1,},
        vy[] = { 0, -1, +1,  0};
    for(i = 0; i < context->nb_cells_h; ++i)
        for(j = 0; j < context->nb_cells_w; ++j)
            if(context->board[i][j] == (context->tileset.nb_tiles_w*context->tileset.nb_tiles_h)-1)
                return SDL_TRUE;
    for(i = 0; i < context->nb_cells_h; ++i)
        for(j = 0; j < context->nb_cells_w; ++j)
            if(context->board[i][j] == EMPTY_CELL)
                return SDL_FALSE;
    /*here, all cells are occupied with a value in [2, 1024].*/
    for(i = 0; i < context->nb_cells_h; ++i)
        for(j = 0; j < context->nb_cells_w; ++j)
            for(k = 0; k < 4; ++k)
                /*If there is still an available move*/
                if((i + vy[k] >= 0 && i + vy[k] < context->nb_cells_w) && (j + vx[k] >= 0 && j + vx[k] < context->nb_cells_h) &&
                            context->board[i+vy[k]][j+vx[k]] == context->board[i][j])
                    return SDL_FALSE;
    return SDL_TRUE;
}

int play(struct context_t *context)
{
    char title[LEN_MAX];
    FILE *file_best_score = fopen("data\\b_s.bin", "rb+");
    int best_score_tmp;
    SDL_Event e;
    generate_new_cell(context, rand()%100 < PROBABILITY_4 ? NEW_CELL_VALUE_4 : NEW_CELL_VALUE_2);
    do
    {
        sprintf(title, "SDL_z2048\t : current score : %d           best score : %d", context->score, MAX(context->best_score, context->score));
        SDL_WM_SetCaption(title, NULL);
        blit_all(context);
        SDL_Flip(context->screen);
        do
        {
            do
                SDL_WaitEvent(&e);
            while((e.type != SDL_QUIT && e.type != SDL_KEYDOWN) ||
                  (e.key.keysym.sym != SDLK_LEFT && e.key.keysym.sym != SDLK_RIGHT &&
                   e.key.keysym.sym != SDLK_UP && e.key.keysym.sym != SDLK_DOWN && e.key.keysym.sym != SDLK_ESCAPE));
            if(e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
                return 0;
        } while(move(context, e.key.keysym.sym - SDLK_UP) == 0);
        blit_all(context);
        SDL_Flip(context->screen);
        SDL_Delay(50);
        generate_new_cell(context, rand()%100 < PROBABILITY_4 ? NEW_CELL_VALUE_4 : NEW_CELL_VALUE_2);
    } while(!isover(context));
    blit_all(context);
    SDL_Flip(context->screen);
    SDL_Delay(1500);
    fread(&best_score_tmp, sizeof(int), 1, file_best_score);
    rewind(file_best_score);
    if(context->score > best_score_tmp)
        fwrite(&context->score, sizeof(int), 1, file_best_score);
    fclose(file_best_score);
    return 1;
}

int move_cell(struct context_t *context, int x, int y, int vecteur[2])
{
    int tmp_x = x,
        tmp_y = y;
    do
    {
        tmp_x += vecteur[0];
        tmp_y += vecteur[1];
    } while(tmp_x < context->nb_cells_w && tmp_x >= 0 && tmp_y < context->nb_cells_h && tmp_y >= 0 && context->board[tmp_y][tmp_x] == 0);
    if(tmp_x < context->nb_cells_w && tmp_x >= 0 && tmp_y < context->nb_cells_h && tmp_y >= 0)
    {
        if(context->board[tmp_y][tmp_x] == context->board[y][x])
        {
            ++context->board[tmp_y][tmp_x];
            context->board[y][x] = 0;
            context->score += 2 << (context->board[tmp_y][tmp_x] - 1);
            return 1;
        }
        else
        {
            if(tmp_y - vecteur[1] != y || tmp_x - vecteur[0] != x)
            {
                context->board[tmp_y - vecteur[1]][tmp_x - vecteur[0]] = context->board[y][x];
                context->board[y][x] = 0;
                return 1;
            }
        }
    }
    else if(tmp_y - vecteur[1] != y || tmp_x - vecteur[0] != x)
    {
        context->board[tmp_y - vecteur[1]][tmp_x - vecteur[0]] = context->board[y][x];
        context->board[y][x] = 0;
        return 1;
    }
    return 0;
}

#define UNKNOWN 0
int move(struct context_t *context, enum side_e direction)
{
    int n_moves = 0,
        i, j;
    int d[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}},
        j_initial[] = {1, UNKNOWN, UNKNOWN, 1},
        j_final[] = {UNKNOWN, -1, -1, UNKNOWN},
        j_iterator[] = {+1, -1, -1, +1};
    j_initial[1] = context->nb_cells_h-2; j_initial[2] = context->nb_cells_w-2;
    j_final[0] = context->nb_cells_h;       j_final[3] = context->nb_cells_w;
    for(i = 0; i < ((direction == LEFT || direction == RIGHT) ? context->nb_cells_h : context->nb_cells_w); ++i)
    {
        for(j = j_initial[direction]; j != j_final[direction]; j += j_iterator[direction])
        {
            int y = (direction == LEFT || direction == RIGHT) ? i : j,
                x = (direction == LEFT || direction == RIGHT) ? j : i;
            if(context->board[y][x] != EMPTY_CELL)
                n_moves += move_cell(context, x, y, d[direction]);
        }
    }
    return n_moves;
}

