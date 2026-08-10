#include "pch.h"
#include "FPSLimiter.h"
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

FPSLimiter::FPSLimiter()
{
	m_timer = CreateWaitableTimerExW(
		nullptr,
		nullptr,
		CREATE_WAITABLE_TIMER_HIGH_RESOLUTION,
		TIMER_ALL_ACCESS
	);

	if (m_timer == nullptr) {
		const DWORD error = GetLastError();

		spdlog::warn(
			"High-resolution waitable timer unavailable: {}",
			error
		);

		if (timeBeginPeriod(1) == TIMERR_NOERROR) {
			m_timerResolutionRaised = true;
		}

		m_timer = CreateWaitableTimerW(
			nullptr,
			FALSE,
			nullptr
		);
	}
}

FPSLimiter::~FPSLimiter()
{
	if (m_timer != nullptr) {
		CloseHandle(m_timer);
		m_timer = nullptr;
	}

	if (m_timerResolutionRaised) {
		timeEndPeriod(1);
		m_timerResolutionRaised = false;
	}
}

void FPSLimiter::setTargetFPS(double targetFPS)
{
	m_targetFPS = targetFPS;

	if (targetFPS > 0.0) {
		m_frameDuration = 1.0 / targetFPS;
	}
	else {
		m_frameDuration = 0.0;
	}

	m_started = false;
}

void FPSLimiter::begin()
{
	if (m_targetFPS <= 0.0 || m_started) {
		return;
	}

	m_nextFrameTime = glfwGetTime() + m_frameDuration;
	m_started = true;
}

void FPSLimiter::wait()
{
	if (m_targetFPS <= 0.0 || !m_started) {
		return;
	}

	constexpr double spinTime = 0.001;
	// 最後の1msはbusy wait

	while (true) {
		const double now = glfwGetTime();
		const double remaining = m_nextFrameTime - now;

		if (remaining <= 0.0) {
			break;
		}

		if (remaining > spinTime && m_timer != nullptr) {
			const double waitSeconds = remaining - spinTime;

			LARGE_INTEGER dueTime;

			// 相対時間。100ns単位で負数を指定
			dueTime.QuadPart = -static_cast<LONGLONG>(
				waitSeconds * 10'000'000.0
				);

			if (SetWaitableTimer(
				m_timer,
				&dueTime,
				0,
				nullptr,
				nullptr,
				FALSE))
			{
				WaitForSingleObject(m_timer, INFINITE);
			}
			else {
				break;
			}
		}
		else {
			while (glfwGetTime() < m_nextFrameTime) {
				// busy wait
			}

			break;
		}
	}

	m_nextFrameTime += m_frameDuration;

	const double now = glfwGetTime();

	if (now - m_nextFrameTime > m_frameDuration) {
		m_nextFrameTime = now + m_frameDuration;
	}
}

double FPSLimiter::targetFPS() const
{
	return m_targetFPS;
}