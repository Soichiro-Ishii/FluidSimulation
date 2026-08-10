#pragma once

#include "GL2DMeshData.h"

#include <cstdint>

namespace Proc2DMeshGenerator
{
	// すべて原点中心、反時計回りの塗りつぶしメッシュを生成する。
	[[nodiscard]] GL2DMeshData createCircle(
		uint32_t segments = 32,
		float radius = 1.0f
	);

	[[nodiscard]] GL2DMeshData createSquare(
		float width = 1.0f,
		float height = 1.0f
	);

	[[nodiscard]] GL2DMeshData createStar(
		uint32_t points = 5,
		float outerRadius = 1.0f,
		float innerRadius = 0.5f
	);

	[[nodiscard]] GL2DMeshData createEllipse(
		uint32_t segments = 32,
		float radiusX = 1.0f,
		float radiusY = 0.5f
	);

	[[nodiscard]] GL2DMeshData createTriangle(
		float width = 1.0f,
		float height = 1.0f
	);

	[[nodiscard]] GL2DMeshData createRegularPolygon(
		uint32_t sides = 3,
		float radius = 1.0f
	);

	[[nodiscard]] GL2DMeshData createRing(
		uint32_t segments = 32,
		float outerRadius = 1.0f,
		float innerRadius = 0.5f
	);
}
