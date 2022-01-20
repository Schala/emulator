#include <stdlib.h>

#include "eng.h"

SPRITE * sprite_alloc(size_t w, size_t h)
{
	SPRITE *sprite = (SPRITE *)malloc(sizeof(SPRITE));
	sprite->w = w;
	sprite->h = h;
	sprite->pixels = (SDL_Color *)calloc(w * h, sizeof(SDL_Color));

	return sprite;
}

void sprite_free(SPRITE *sprite)
{
	free(sprite->pixels);
	free(sprite);
	sprite = NULL;
}

ENG * eng_alloc()
{
	ENG *eng = (ENG *)malloc(sizeof(ENG));
	eng->now = eng->then = SDL_GetTicks();

	return eng;
}

float eng_clock(ENG *eng)
{
	if (!eng) return 0.0f;

	eng->then = eng->now;
	eng->now = SDL_GetTicks();
	eng->delta = (eng->now - eng->then) / 1000.0f;

	return eng->delta;
}

void eng_free(ENG *eng)
{
	if (!eng) return;

	free(eng);
	eng = NULL;
}
