#include "tileset.h"
#include <string.h>
#include <stdio.h>

void get_non_commented_line(FILE *f, char *buffer, int buffer_length)
{
    char *tmp;
	do
		tmp = fgets(buffer, buffer_length, f);
	while(*buffer == COMMENT_CHAR && tmp != NULL);
	if(strrchr(buffer, '\n') != NULL)
        *strrchr(buffer, '\n') = '\0';
}

int load_tileset(struct tileset_t *tileset, const char *path)
{
	char buffer[LEN_MAX];
	int i, j;
	FILE *f = fopen(path, "r");
	if(f == NULL)
		return 0;
    rewind(f);
	/*nb_tiles_x*/
	get_non_commented_line(f, buffer, LEN_MAX);
	sscanf(buffer, "%d %d", &tileset->nb_tiles_w, &tileset->nb_tiles_h);

	/*path and im*/
	get_non_commented_line(f, buffer, LEN_MAX);
	strcpy(tileset->path, buffer);
	tileset->im = SDL_LoadBMP(tileset->path);
	if(tileset->im == NULL)
	{
	    fputs("Unable to load tileset image\n", stderr);
		fclose(f);
		return 0;
	}

	/*tab_pos*/
	tileset->tab_pos = malloc(tileset->nb_tiles_w*tileset->nb_tiles_h*sizeof(*tileset->tab_pos));
	if(tileset->tab_pos == NULL)
	{
	    fputs("Unable to malloc tileset->tab_pos\n", stderr);
		fclose(f);
		SDL_FreeSurface(tileset->im);
		return 0;
	}
	for(i = 0; i < tileset->nb_tiles_h; ++i)
	{
		for(j = 0; j < tileset->nb_tiles_w; ++j)
		{
			tileset->tab_pos[tileset->nb_tiles_w*i + j].h = tileset->im->h/tileset->nb_tiles_h;
			tileset->tab_pos[tileset->nb_tiles_w*i + j].w = tileset->im->w/tileset->nb_tiles_w;
			tileset->tab_pos[tileset->nb_tiles_w*i + j].y = i*tileset->im->h/tileset->nb_tiles_h;
			tileset->tab_pos[tileset->nb_tiles_w*i + j].x = j*tileset->im->w/tileset->nb_tiles_w;
		}
	}

	/*tab_coll*/
	get_non_commented_line(f, buffer, LEN_MAX);
	if(*buffer == '0')
		tileset->tab_coll = NULL;
	else
	{
		tileset->tab_coll = malloc(tileset->nb_tiles_w*tileset->nb_tiles_h*sizeof(*tileset->tab_coll));
		if(tileset->tab_coll == NULL)
		{
		    fputs("Unable to malloc tileset->tab_coll\n", stderr);
			fclose(f);
			SDL_FreeSurface(tileset->im);
			free(tileset->tab_pos);
			return 0;
		}
		for(i = 0; i < tileset->nb_tiles_h; ++i)
		{
			for(j = 0; j < tileset->nb_tiles_w; ++i)
			{
				get_non_commented_line(f, buffer, LEN_MAX);
				tileset->tab_coll[tileset->nb_tiles_h*i + j] = *buffer == '1' ? SDL_TRUE : SDL_FALSE;
			}
		}
	}
	return 1;
}

void free_tileset(struct tileset_t *tileset)
{
    SDL_FreeSurface(tileset->im);
    free(tileset->tab_pos);
    free(tileset->tab_coll);
}

