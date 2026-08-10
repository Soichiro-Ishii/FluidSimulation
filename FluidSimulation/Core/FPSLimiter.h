#pragma once

#include <Windows.h>

class FPSLimiter
{
private:
	HANDLE m_timer = nullptr;

	double m_targetFPS = 0.0;
	double m_frameDuration = 0.0;
	double m_nextFrameTime = 0.0;

	bool m_started = false;
	bool m_timerResolutionRaised = false;
public:
	FPSLimiter();
	~FPSLimiter();

	FPSLimiter(const FPSLimiter&) = delete;
	FPSLimiter& operator=(const FPSLimiter&) = delete;

	void setTargetFPS(double targetFPS);
	void begin();

	void wait();

	[[nodiscard]]
	double targetFPS() const;
};