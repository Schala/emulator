#include <algorithm>
#include <cmath>
#include <format
#include <stdexcept>

#include "engine.h"

// CSprite

CSprite::CSprite(size_t w, size_t h):
	m_h(h),
	m_w(w),
	m_pixels(std::vector<SDL_Color>(w * h, CEngine::BLACK))
{
}


SDL_Color CSprite::operator[](size_t index) const
{
	return m_pixels.at(index);
}


void CSprite::Clear()
{
	std::fill(m_pixels.begin(), m_pixels.end(), CEngine::BLACK);
}


void CSprite::Draw(SDL_Renderer *renderer, int x, int y) const
{
	for (size_t row = 0; row < m_h; row++)
		for (size_t col = 0; col < m_w; col++)
		{
			SDL_Color c = m_pixels.at((row * m_w) + col);

			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
			SDL_RenderDrawPoint(renderer, x + col, y + row);
		}
}


SDL_Color CSprite::Get(size_t x, size_t y) const
{
	return m_pixels.at((y * m_h) + x);
}


void CSprite::Set(size_t x, size_t y, SDL_Color color)
{
	m_pixels[(y * m_h) + x] = color;
}


void CSprite::Set(const std::vector<SDL_Color> pixels)
{
	m_pixels = pixels;
}


// CEngine

CEngine::CEngine(float rate): CEngineCore(rate)
{
	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &m_win, &m_rend);
}


CEngine::~CEngine()
{
	if (m_rend) SDL_DestroyRenderer(m_rend);
	if (m_win) SDL_DestroyWindow(m_win);
}


void CEngine::Start()
{
	int rcode = SDL_Init(SDL_INIT_VIDEO);
	if (rcode != 0)
		throw std::runtime_error(std::format("Failed to initialise SDL video, error code {}", rcode));

	Started();

	while (true)
	{
		if (SDL_PollEvent(&m_evt))
		{
			if (m_evt.type == SDL_WINDOWEVENT)
			{
				switch (m_evt.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						Stop();
						break;

					case SDL_WINDOWEVENT_RESIZED:
					{
						int w, h;
						SDL_GetWindowSize(m_win, &w, &h);
						Resized(w, h);
						break;
					}

					default: ;
				}
			}
		}

		Update();
	}

	Stop();
}


void CEngine::Resized(int, int)
{
}


void CEngine::Stop()
{
	Stopping();
	SDL_Quit();
}


void CEngine::Update()
{
	CoreUpdate();
	SDL_RenderPresent(m_rend);
}
