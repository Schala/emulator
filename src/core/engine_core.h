#pragma once

#include <chrono>

// Non-graphical functionality of our engine
class CEngineCore
{
public:
	CEngineCore(float);
	void CoreStart();
	void Stop();
	void Pause();
	void Resume();
protected:
	float m_rate;
	float m_delta;
	float m_timeScale;
	float m_prevTimeScale;

	void CoreUpdate();
	virtual void Started();
	virtual void Stopping();
	virtual void Updated(float);
private:
	std::chrono::system_clock::time_point m_now;
	std::chrono::system_clock::time_point m_then;
};
