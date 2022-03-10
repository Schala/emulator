#ifndef _CORE_ENGINE_H
#define _CORE_ENGINE_H

#include <SDL2/SDL.h>
#include <vector>

#include "engine_core.h"

class Sprite
{
public:
	// Constructs the sprite, given a width and height
	Sprite(size_t, size_t);

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
	void Set(const std::vector<SDL_Color>);
private:
	size_t m_h;
	size_t m_w;
	std::vector<SDL_Color> m_pixels;
};

class Engine : public EngineCore
{
public:
	static constexpr SDL_Color BLACK = { 0, 0, 0, SDL_ALPHA_OPAQUE };

	Engine(float);
	virtual ~Engine();
	void Start();
	void Stop();
protected:
	SDL_Renderer *renderer;

	virtual void Resized(int, int);
private:
	SDL_Window *m_window;
	SDL_Event m_event;

	void Update();
};

#endif // _CORE_ENGINE_H
