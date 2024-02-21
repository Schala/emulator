#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "engine_core.h"

typedef std::vector<SDL_Color> TPixels;


class CSprite
{
public:
	// Constructs the sprite, given a width and height
	CSprite(size_t, size_t);

	// Retrieve the pixel by index
	SDL_Color operator[](size_t) const;

	// Clears all pixels to black
	void Clear();

	void Draw(SDL_Renderer *, int, int) const;

	// Retrieve the pixel at x, y
	SDL_Color Get(size_t, size_t) const;

	// Set the pixel at x, y
	void Set(size_t, size_t, SDL_Color);

	// Set all pixels
	void Set(const TPixels &);
private:
	size_t m_h;
	size_t m_w;
	TPixels m_pixels;
};


class CEngine : public CEngineCore
{
public:
	static constexpr SDL_Color Black = { 0, 0, 0, SDL_ALPHA_OPAQUE };

	CEngine(float);
	virtual ~CEngine();
	void Start();
	void Stop();
protected:
	SDL_Renderer *m_rend;

	virtual void Resized(int, int);
private:
	SDL_Window *m_win;
	SDL_Event m_evt;

	void Update();
};
