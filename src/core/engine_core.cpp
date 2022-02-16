#include <cmath>
#include <iostream>

#include "engine_core.h"

EngineCore::EngineCore(float rate):
	rate(rate),
	delta(0.0f),
	timeScale(1.0f),
	prevTimeScale(1.0f),
	m_now(std::chrono::system_clock::now()),
	m_then(m_now)
{
}

void EngineCore::Pause()
{
	prevTimeScale = timeScale;
	timeScale = 0.0f;
}

void EngineCore::Resume()
{
	timeScale = prevTimeScale;
}

void EngineCore::CoreStart()
{
	Started();

	while (true)
		CoreUpdate();

	CoreStop();
}

void EngineCore::Started()
{
}

void EngineCore::CoreStop()
{
	Stopping();
}

void EngineCore::Stopping()
{
}

void EngineCore::CoreUpdate()
{
	// update time
	m_then = m_now;
	m_now = std::chrono::system_clock::now();
	std::chrono::duration<float> newDelta = (m_now - m_then);// / 1000.0f;

	//std::cout << delta << '\n';

	// calculate our delta and cap our frame rate
	if (delta > 0.0f)
		delta -= newDelta.count() * timeScale;
	else
	{
		// abs will mitigate negative frame rates, which add to the delta, causing CPU stress
		// time scale must factor in the whole equation to be valid, ie. if we pause,
		// delta must = 0
		delta = (std::abs(delta) + (1.0f / rate) - newDelta.count()) * timeScale;

		// keep user updates constrained to rate
		Updated(delta);
	}
}

void EngineCore::Updated(float)
{
}
