#include <cmath>
#include <iostream>

#include "engine_core.h"

CEngineCore::CEngineCore(float rate):
	m_rate(rate),
	m_delta(0.0f),
	m_timeScale(1.0f),
	m_prevTimeScale(1.0f)
{
	m_then = m_now = std::chrono::system_clock::now();
}


void CEngineCore::Pause()
{
	m_prevTimeScale = m_timeScale;
	m_timeScale = 0.0f;
}


void CEngineCore::Resume()
{
	m_timeScale = m_prevTimeScale;
}


void CEngineCore::CoreStart()
{
	Started();

	while (true)
		CoreUpdate();

	CoreStop();
}


void CEngineCore::Started()
{
}


void CEngineCore::CoreStop()
{
	Stopping();
}


void CEngineCore::Stopping()
{
}


void CEngineCore::CoreUpdate()
{
	// update time
	m_then = m_now;
	m_now = std::chrono::system_clock::now();
	std::chrono::duration<float> delta = m_now - m_then;// / 1000.0f;

	//std::cout << delta << '\n';

	// calculate our delta and cap our frame rate
	if (m_delta > 0.0f)
		m_delta -= delta.count() * m_timeScale;
	else
	{
		// abs will mitigate negative frame rates, which add to the delta, causing CPU stress
		// time scale must factor in the whole equation to be valid, ie. if we pause,
		// delta must = 0
		m_delta = (std::abs(m_delta) + (1.0f / rate) - delta.count()) * m_timeScale;

		// keep user updates constrained to rate
		Updated(m_delta);
	}
}


void CEngineCore::Updated(float)
{
}
