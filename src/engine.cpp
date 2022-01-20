#include "engine.h"

Sprite::Sprite(size_t w, size_t h):
	m_h(h),
	m_w(w),
	m_pixels(std::vector<SDL_Color>({ 0, 0, 0, SDL_ALPHA_OPAQUE }, w * h)
{
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

Engine::Engine():
	m_timeScale(1.0f),
	m_prevTimeScale(1.0f),
	m_now(std::chrono::system_clock::now()),
	m_then(std::chrono::system_clock::now()),
{
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

	while (SDL_WaitEvent(&m_event))
	{
		if (m_event.type == SDL_WINDOWEVENT)
		{
			switch (m_event.window.event)
			{
				case SDL_WINDOWEVENT_CLOSE:
					Stop();
					goto exitloop;
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

exitloop:
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
	// update delta time
	m_then = m_now;
	m_now = std::chrono::system_clock::now();
	std::chrono::duration<float> delta = m_now - m_then;
	Update(delta.count() * m_timeScale);

	SDL_RenderPresent(m_renderer);
}

void Engine::Updated(float)
{
}
