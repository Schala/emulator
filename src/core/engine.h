#ifndef _CORE_ENGINE_H
#define _CORE_ENGINE_H

#include <SDL2/SDL.h>
#include <vector>

class Sprite
{
public:
	// Constructs the sprite, given a width and height
	Sprite(size_t, size_t);

	// Retrieve the pixel by index
	inline SDL_Color & operator[](size_t) const;

	// Retrieve the pixel at x, y
	inline SDL_Color & Get(size_t, size_t) const;

	// Set the pixel at x, y
	inline void Set(size_t, size_t, SDL_Color);
private:
	size_t m_h;
	size_t m_w;
	std::vector<SDL_Color> m_pixels;
};

class Engine
{
public:
	static constexpr SDL_Color Black = { 0, 0, 0, SDL_ALPHA_OPAQUE };

	Engine(float);
	virtual ~Engine();
	void Pause();
	void Resume();
	int Start();
	void Stop();
protected:
	float m_fps;
	float m_delta;
	float m_timeScale;
	float m_prevTimeScale;
	SDL_Renderer *m_renderer;

	virtual void Resized(int, int);
	virtual void Started();
	virtual void Stopping();
	virtual void Updated(float);
private:
	SDL_Window *m_window;
	uint64_t m_now;
	uint64_t m_then;
	SDL_Event m_event;

	void Update();
};

#endif // _CORE_ENGINE_H