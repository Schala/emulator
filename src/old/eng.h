#ifndef _EMU_ENG_H
#define _EMU_ENG_H

#include <SDL2/SDL.h>

// A 2D array of pixels representing an image
typedef struct _SPRITE
{
	size_t w; // width
	size_t h; // height
	SDL_Color *pixels;
} SPRITE;

// Allocates new sprite
SPRITE * sprite_alloc(size_t, size_t);

// Deallocates sprite
void sprite_free(SPRITE *);

// Sprite width times height in pixels
static inline size_t sprite_size(const SPRITE *sprite)
{
	return sprite->w * sprite->h;
}

// Grabs pixel at 2D coordinate in sprite's pixel array
static inline SDL_Color * sprite_get_pixel(SPRITE *sprite, size_t x, size_t y)
{
	return &sprite->pixels[(y * sprite->h) + x];
}

// Sets pixel at 2D coordinate in sprite's pixel array
static inline void sprite_set_pixel(SPRITE *sprite, size_t x, size_t y, SDL_Color color)
{
	sprite->pixels[(y * sprite->h) + x] = color;
}

// Logical engine data
typedef struct _ENG
{
	uint32_t now;
	uint32_t then;
	float delta;
} ENG;

// Allocate new engine
ENG * eng_alloc();

// Updates engine clock, returning delta time
float eng_clock(ENG *);

// Deallocate engine
void eng_free(ENG *);


#endif // _EMU_ENG_H
