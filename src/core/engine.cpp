#include <cmath>
#include <iostream>

#include "engine.h"

Sprite::Sprite(size_t w, size_t h):
	m_h(h),
	m_w(w),
	m_pixels(std::vector<SDL_Color>(w * h, Engine::Black))
{
}

SDL_Color & Sprite::operator[](size_t index) const
{
	return m_pixels.at(index);
}

SDL_Color & Sprite::Get(size_t x, size_t y) const
{
	return m_pixels.at((y * m_h) + x);
}

void Sprite::Set(size_t x, size_t y, SDL_Color color)
{
	m_pixels[(y * m_h) + x] = color;
}


// Engine

Engine::Engine(float fps):
	m_fps(fps),
	m_delta(0.0f),
	m_timeScale(1.0f),
	m_prevTimeScale(1.0f)
{
	m_now = m_then = SDL_GetTicks64();
	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer);
}

Engine::~Engine()
{
	if (m_renderer) SDL_DestroyRenderer(m_renderer);
	if (m_window) SDL_DestroyWindow(m_window);
}

void Engine::Pause()
{
	m_prevTimeScale = m_timeScale;
	m_timeScale = 0.0f;
}

void Engine::Resume()
{
	m_timeScale = m_prevTimeScale;
}

int Engine::Start()
{
	int rcode = SDL_Init(SDL_INIT_VIDEO);
	if (rcode != 0) return rcode;

	float time = 0.0f;
	Started();

	while (true)
	{
		if (SDL_PollEvent(&m_event))
		{
			if (m_event.type == SDL_WINDOWEVENT)
			{
				switch (m_event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						Stop();
						break;

					case SDL_WINDOWEVENT_RESIZED:
					{
						int w, h;
						SDL_GetWindowSize(m_window, &w, &h);
						Resized(w, h);
					}
						break;
				}
			}
		}

		Update();
	}

	return rcode;
}

void Engine::Resized(int, int)
{
}

void Engine::Started()
{
}

void Engine::Stop()
{
	Stopping();
	SDL_Quit();
}

void Engine::Stopping()
{
}

void Engine::Update()
{
	// update time
	m_then = m_now;
	m_now = SDL_GetTicks64();
	float newDelta = (m_now - m_then) / 1000.0f;

	// calculate our delta and cap our frame rate
	if (m_delta > 0.0f)
		m_delta -= newDelta * m_timeScale;
	else
	{
		// abs will mitigate negative frame rates, which add to the delta, causing CPU stress
		// time scale must factor in the whole equation to be valid, ie. if we pause,
		// delta must = 0
		m_delta = (std::abs(m_delta) + (1.0f / m_fps) - newDelta) * m_timeScale;

		// let the user do their thing BEFORE we present
		// also, keep user updates constrained to frame updates
		Updated(m_delta);
		SDL_RenderPresent(m_renderer);

		std::cout << m_delta << '\n';
	}
}

void Engine::Updated(float)
{
}