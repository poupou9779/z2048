#include <SDL/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "context.h"
#include "functions.h"

#define PATH_CONTEXT "data\\context.ctx"
/*#define RESET_SCORE*/

int main(int ac, char **av)
{
#ifdef RESET_SCORE
    FILE *file_best_score = fopen("data\\b_s.bin", "wb+");
    int n = 0;
    rewind(file_best_score);
    fwrite(&n, sizeof(int), 1, file_best_score);
    fclose(file_best_score);
#else
    struct context_t context;
    if(load_context(&context, PATH_CONTEXT) == 0)
    {
        fputs("Unable to load context", stderr);
        return EXIT_FAILURE;
    }
    srand((unsigned int)time(NULL));

    SDL_EnableKeyRepeat(0, 0);
    play(&context);
    SDL_SaveBMP(context.screen, "data\\last_screen.bmp");

    free_context(&context);
    SDL_Delay(500);
#endif
    return EXIT_SUCCESS;
    (void)ac;
    (void)av;
}

