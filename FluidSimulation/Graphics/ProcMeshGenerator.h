#pragma once
#include "GLMesh.h"
#include <cstdint>

namespace ProcMeshGenerator
{
	GLMeshData createQuad(
		float width = 1.0f,
		float height = 1.0f
	);

	GLMeshData createCube(
		float size = 1.0f
	);

	GLMeshData createSphere(
		float radius = 1.0f,
		uint32_t slices = 32,
		uint32_t stacks = 16
	);

	GLMeshData createPlane(
		float width = 1.0f,
		float depth = 1.0f,
		uint32_t xSegments = 1,
		uint32_t zSegments = 1
	);
	GLMeshData createScreen();
}