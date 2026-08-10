#pragma once

class CHRONO
{
	double m_start = 0.0f;
	double m_end = 0.0f;
public:
	CHRONO() = default;
	double timeStart();
	double timeEnd();
	double getElapsed() const;
	[[nodiscard]] double start() const {
		return m_start;
	}
	[[nodiscard]] double end() const {
		return m_end;
	}
};