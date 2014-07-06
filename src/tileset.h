#ifndef TILESET_H
#define TILESET_H

#include <SDL/SDL.h>

#define LEN_MAX 128
#define COMMENT_CHAR '#'

struct tileset_t
{
	char path[LEN_MAX];
	SDL_Surface *im;
	int nb_tiles_w,
		nb_tiles_h;
	SDL_Rect *tab_pos;
	SDL_bool *tab_coll;
};

void get_non_commented_line(FILE *f, char *buffer, int buffer_length);

void free_tileset(struct tileset_t *tileset);

int load_tileset(struct tileset_t *tileset, const char *path);
/** THE FILE IN path MUST BE A *.tst FILE WITH :
<nb_tiles_w> <nb_tiles_h>
<path> #less than 128 characters in path !
<<boolean value in {0, 1} that tells if collision is needed> if so, a boolean value on each line>

#example for a tileset that has only 1 line with 9 tiles no matter the collisions and has a bmp on tileset.bmp:
9 1
tileset.bmp
0

#another example for a tileset that has 3 lines with 5 tiles each with important collisions management
#and the same path for the image :
3 5
tileset.bmp
1
#this line is ignored because it starts with a '#'
#warning ! a comment cannot be more than 128 characters long otherwise undefined behaviour will happen
#here 15 numbers will be on a different line each to tell whether or not this tile may collide
0
0
0
0
1
1
1
0
1
1
1
0
1
0
1
**/

#endif
