#include "pch.h"
#include "Proc2DMeshGenerator.h"

namespace
{
	constexpr float kPi = std::numbers::pi_v<float>;
	constexpr float kTwoPi = 2.0f * kPi;
	constexpr float kStartAngle = kPi * 0.5f;

	[[nodiscard]] float nonNegative(float value)
	{
		return std::max(value, 0.0f);
	}

	[[nodiscard]] glm::vec2 radialNormal(const glm::vec2& position)
	{
		const float lengthSquared = glm::dot(position, position);
		if (lengthSquared <= 0.0f)
			return { 0.0f, 0.0f };

		return position / std::sqrt(lengthSquared);
	}

	[[nodiscard]] glm::vec2 calculateUV(
		const glm::vec2& position,
		const glm::vec2& halfExtents
	)
	{
		glm::vec2 uv{ 0.5f, 0.5f };

		if (halfExtents.x > 0.0f)
			uv.x = position.x / (2.0f * halfExtents.x) + 0.5f;
		if (halfExtents.y > 0.0f)
			uv.y = position.y / (2.0f * halfExtents.y) + 0.5f;

		return uv;
	}

	[[nodiscard]] GL2DMeshData createTriangleFan(
		const std::vector<glm::vec2>& perimeter,
		const glm::vec2& halfExtents
	)
	{
		GL2DMeshData data;
		if (perimeter.size() < 3)
			return data;

		data.vertices.reserve(perimeter.size() + 1);
		data.indices.reserve(perimeter.size() * 3);

		data.vertices.push_back({
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			{ 0.5f, 0.5f }
			});

		for (const glm::vec2& position : perimeter)
		{
			data.vertices.push_back({
				position,
				radialNormal(position),
				calculateUV(position, halfExtents)
				});
		}

		for (uint32_t i = 0; i < perimeter.size(); ++i)
		{
			const uint32_t current = i + 1;
			const uint32_t next =
				static_cast<uint32_t>((i + 1) % perimeter.size()) + 1;

			data.indices.push_back(0);
			data.indices.push_back(current);
			data.indices.push_back(next);
		}

		data.primitiveMode = GL_TRIANGLES;
		return data;
	}

	[[nodiscard]] std::vector<glm::vec2> createEllipsePerimeter(
		uint32_t segments,
		float radiusX,
		float radiusY
	)
	{
		std::vector<glm::vec2> perimeter;
		perimeter.reserve(segments);

		for (uint32_t i = 0; i < segments; ++i)
		{
			const float angle =
				kStartAngle + kTwoPi *
				static_cast<float>(i) / static_cast<float>(segments);

			perimeter.push_back({
				std::cos(angle) * radiusX,
				std::sin(angle) * radiusY
				});
		}

		return perimeter;
	}
}

namespace Proc2DMeshGenerator
{
	GL2DMeshData createCircle(uint32_t segments, float radius)
	{
		radius = nonNegative(radius);
		return createEllipse(segments, radius, radius);
	}

	GL2DMeshData createSquare(float width, float height)
	{
		width = nonNegative(width);
		height = nonNegative(height);

		const float halfWidth = width * 0.5f;
		const float halfHeight = height * 0.5f;

		GL2DMeshData data;
		data.vertices = {
			{
				{ -halfWidth, -halfHeight },
				radialNormal({ -halfWidth, -halfHeight }),
				{ 0.0f, 0.0f }
			},
			{
				{ halfWidth, -halfHeight },
				radialNormal({ halfWidth, -halfHeight }),
				{ 1.0f, 0.0f }
			},
			{
				{ halfWidth, halfHeight },
				radialNormal({ halfWidth, halfHeight }),
				{ 1.0f, 1.0f }
			},
			{
				{ -halfWidth, halfHeight },
				radialNormal({ -halfWidth, halfHeight }),
				{ 0.0f, 1.0f }
			}
		};

		data.indices = {
			0, 1, 2,
			0, 2, 3
		};
		data.primitiveMode = GL_TRIANGLES;
		return data;
	}

	GL2DMeshData createStar(
		uint32_t points,
		float outerRadius,
		float innerRadius
	)
	{
		points = std::max(points, 3u);
		outerRadius = nonNegative(outerRadius);
		innerRadius = std::clamp(innerRadius, 0.0f, outerRadius);

		const uint32_t perimeterCount = points * 2;
		std::vector<glm::vec2> perimeter;
		perimeter.reserve(perimeterCount);

		for (uint32_t i = 0; i < perimeterCount; ++i)
		{
			const float radius = (i % 2 == 0) ? outerRadius : innerRadius;
			const float angle =
				kStartAngle + kTwoPi *
				static_cast<float>(i) / static_cast<float>(perimeterCount);

			perimeter.push_back({
				std::cos(angle) * radius,
				std::sin(angle) * radius
				});
		}

		return createTriangleFan(
			perimeter,
			{ outerRadius, outerRadius }
		);
	}

	GL2DMeshData createEllipse(
		uint32_t segments,
		float radiusX,
		float radiusY
	)
	{
		segments = std::max(segments, 3u);
		radiusX = nonNegative(radiusX);
		radiusY = nonNegative(radiusY);

		return createTriangleFan(
			createEllipsePerimeter(segments, radiusX, radiusY),
			{ radiusX, radiusY }
		);
	}

	GL2DMeshData createTriangle(float width, float height)
	{
		width = nonNegative(width);
		height = nonNegative(height);

		const float halfWidth = width * 0.5f;
		const float halfHeight = height * 0.5f;

		GL2DMeshData data;
		const glm::vec2 bottomLeft{ -halfWidth, -halfHeight };
		const glm::vec2 bottomRight{ halfWidth, -halfHeight };
		const glm::vec2 top{ 0.0f, halfHeight };

		data.vertices = {
			{ bottomLeft, radialNormal(bottomLeft), { 0.0f, 0.0f } },
			{ bottomRight, radialNormal(bottomRight), { 1.0f, 0.0f } },
			{ top, radialNormal(top), { 0.5f, 1.0f } }
		};
		data.indices = { 0, 1, 2 };
		data.primitiveMode = GL_TRIANGLES;
		return data;
	}

	GL2DMeshData createRegularPolygon(uint32_t sides, float radius)
	{
		sides = std::max(sides, 3u);
		radius = nonNegative(radius);

		return createTriangleFan(
			createEllipsePerimeter(sides, radius, radius),
			{ radius, radius }
		);
	}

	GL2DMeshData createRing(
		uint32_t segments,
		float outerRadius,
		float innerRadius
	)
	{
		segments = std::max(segments, 3u);
		outerRadius = nonNegative(outerRadius);
		innerRadius = std::clamp(innerRadius, 0.0f, outerRadius);

		GL2DMeshData data;
		data.vertices.reserve(static_cast<size_t>(segments) * 2);
		data.indices.reserve(static_cast<size_t>(segments) * 6);

		for (uint32_t i = 0; i < segments; ++i)
		{
			const float angle =
				kStartAngle + kTwoPi *
				static_cast<float>(i) / static_cast<float>(segments);
			const glm::vec2 direction{ std::cos(angle), std::sin(angle) };

			const glm::vec2 outerPosition = direction * outerRadius;
			const glm::vec2 innerPosition = direction * innerRadius;

			data.vertices.push_back({
				outerPosition,
				direction,
				calculateUV(outerPosition, { outerRadius, outerRadius })
				});
			data.vertices.push_back({
				innerPosition,
				direction,
				calculateUV(innerPosition, { outerRadius, outerRadius })
				});
		}

		for (uint32_t i = 0; i < segments; ++i)
		{
			const uint32_t next = (i + 1) % segments;
			const uint32_t outerCurrent = i * 2;
			const uint32_t innerCurrent = outerCurrent + 1;
			const uint32_t outerNext = next * 2;
			const uint32_t innerNext = outerNext + 1;

			data.indices.insert(
				data.indices.end(),
				{
					outerCurrent, outerNext, innerNext,
					outerCurrent, innerNext, innerCurrent
				}
			);
		}

		data.primitiveMode = GL_TRIANGLES;
		return data;
	}
}
