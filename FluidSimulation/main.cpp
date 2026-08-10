#include "pch.h"
#include"FluidSimApp.h"


int main() {
	FluidSimApp app;
	int result = app.run(1920, 1080, "FluidSimulation", false);
	return result;

}
