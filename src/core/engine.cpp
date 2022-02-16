#include <cmath>
#include <fmt/core.h>
#include <stdexcept>

#include "engine.h"

Sprite::Sprite(size_t w, size_t h):
	m_h(h),
	m_w(w),
	m_pixels(std::vector<SDL_Color>(w * h, Engine::Black))
{
}

SDL_Color Sprite::operator[](size_t index) const
{
	return m_pixels.at(index);
}

SDL_Color Sprite::Get(size_t x, size_t y) const
{
	return m_pixels.at((y * m_h) + x);
}

void Sprite::Set(size_t x, size_t y, SDL_Color color)
{
	m_pixels[(y * m_h) + x] = color;
}


// Engine

Engine::Engine(float rate): EngineCore(rate)
{
	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &m_window, &renderer);
}

Engine::~Engine()
{
	if (renderer) SDL_DestroyRenderer(renderer);
	if (m_window) SDL_DestroyWindow(m_window);
}

void Engine::Start()
{
	int rcode = SDL_Init(SDL_INIT_VIDEO);
	if (rcode != 0)
		throw std::runtime_error(fmt::format("Failed to initialise SDL video, error code {}", rcode));

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

	Stop();
}

void Engine::Resized(int, int)
{
}

void Engine::Stop()
{
	CoreStop();
	SDL_Quit();
}

void Engine::Update()
{
	CoreUpdate();
	SDL_RenderPresent(renderer);
}
