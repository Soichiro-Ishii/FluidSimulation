#include"pch.h"

#include "CHRONO.h"

double CHRONO::timeStart() {
	m_start = glfwGetTime();
	return m_start;
}
double CHRONO::timeEnd() {
	m_end = glfwGetTime();
	return m_end;
}
double CHRONO::getElapsed() const {
	return m_end - m_start;
}
