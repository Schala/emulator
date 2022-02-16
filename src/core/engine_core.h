#ifndef _CORE_ENGINE_CORE_H
#define _CORE_ENGINE_CORE_H

#include <chrono>

// Non-graphical functionality of our engine
class EngineCore
{
public:
	EngineCore(float);
	void CoreStart();
	void CoreStop();
	void Pause();
	void Resume();
protected:
	float rate;
	float delta;
	float timeScale;
	float prevTimeScale;

	void CoreUpdate();
	virtual void Started();
	virtual void Stopping();
	virtual void Updated(float);
private:
	std::chrono::system_clock::time_point m_now;
	std::chrono::system_clock::time_point m_then;
};

#endif // _CORE_ENGINE_CORE_H
