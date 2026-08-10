#include "pch.h"
#include "ProcMeshGenerator.h"

namespace ProcMeshGenerator
{
	GLMeshData createQuad(float width, float height)
	{
		GLMeshData data;

		const float halfWidth = width * 0.5f;
		const float halfHeight = height * 0.5f;

		/*
		 * XY平面上の四角形。
		 * 現在のカメラ側である-Z方向を正面にする。
		 */
		const glm::vec3 normal = { 0.0f, 0.0f, -1.0f };
		const glm::vec3 tangent = { 1.0f, 0.0f, 0.0f };

		data.vertices = {
			{
				{ -halfWidth, -halfHeight, 0.0f },
				normal,
				tangent,
				{ 0.0f, 0.0f }
			},
			{
				{ halfWidth, -halfHeight, 0.0f },
				normal,
				tangent,
				{ 1.0f, 0.0f }
			},
			{
				{ halfWidth, halfHeight, 0.0f },
				normal,
				tangent,
				{ 1.0f, 1.0f }
			},
			{
				{ -halfWidth, halfHeight, 0.0f },
				normal,
				tangent,
				{ 0.0f, 1.0f }
			}
		};

		// -Z方向から見て反時計回り
		data.indices = {
			0, 2, 1,
			0, 3, 2
		};

		data.primitiveMode = GL_TRIANGLES;

		return data;
	}

	GLMeshData createCube(float size)
	{
		GLMeshData data;

		const float h = size * 0.5f;

		/*
		 * 面ごとに独立した法線・Tangent・UVを持つため、
		 * 6面 × 4頂点の24頂点。
		 */
		data.vertices = {
			// Front (+Z)
			{
				{ -h, -h, h },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ h, -h, h },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 0.0f }
			},
			{
				{ h, h, h },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f }
			},
			{
				{ -h, h, h },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f }
			},

			// Back (-Z)
			{
				{ h, -h, -h },
				{ 0.0f, 0.0f, -1.0f },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ -h, -h, -h },
				{ 0.0f, 0.0f, -1.0f },
				{ -1.0f, 0.0f, 0.0f },
				{ 1.0f, 0.0f }
			},
			{
				{ -h, h, -h },
				{ 0.0f, 0.0f, -1.0f },
				{ -1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f }
			},
			{
				{ h, h, -h },
				{ 0.0f, 0.0f, -1.0f },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f }
			},

			// Right (+X)
			{
				{ h, -h, h },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ h, -h, -h },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 1.0f, 0.0f }
			},
			{
				{ h, h, -h },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 1.0f, 1.0f }
			},
			{
				{ h, h, h },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 0.0f, 1.0f }
			},

			// Left (-X)
			{
				{ -h, -h, -h },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ -h, -h, h },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f }
			},
			{
				{ -h, h, h },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f }
			},
			{
				{ -h, h, -h },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 0.0f, 1.0f }
			},

			// Top (+Y)
			{
				{ -h, h, h },
				{ 0.0f, 1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ h, h, h },
				{ 0.0f, 1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 0.0f }
			},
			{
				{ h, h, -h },
				{ 0.0f, 1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f }
			},
			{
				{ -h, h, -h },
				{ 0.0f, 1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f }
			},

			// Bottom (-Y)
			{
				{ -h, -h, -h },
				{ 0.0f, -1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ h, -h, -h },
				{ 0.0f, -1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 0.0f }
			},
			{
				{ h, -h, h },
				{ 0.0f, -1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f }
			},
			{
				{ -h, -h, h },
				{ 0.0f, -1.0f, 0.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f }
			}
		};

		data.indices.reserve(36);

		for (uint32_t face = 0; face < 6; ++face)
		{
			const uint32_t base = face * 4;

			data.indices.insert(
				data.indices.end(),
				{
					base + 0, base + 1, base + 2,
					base + 0, base + 2, base + 3
				}
			);
		}

		data.primitiveMode = GL_TRIANGLES;

		return data;
	}

	GLMeshData createSphere(
		float radius,
		uint32_t slices,
		uint32_t stacks
	)
	{
		GLMeshData data;

		slices = std::max(slices, 3u);
		stacks = std::max(stacks, 2u);
		radius = std::max(radius, 0.0f);

		const uint32_t verticesPerRow = slices + 1;

		data.vertices.reserve(
			static_cast<size_t>(stacks + 1) *
			static_cast<size_t>(verticesPerRow)
		);

		// 極の縮退三角形を除いたインデックス数
		data.indices.reserve(
			static_cast<size_t>(slices) *
			static_cast<size_t>(stacks - 1) *
			6
		);

		constexpr float pi = std::numbers::pi_v<float>;
		constexpr float twoPi = 2.0f * pi;

		for (uint32_t stack = 0; stack <= stacks; ++stack)
		{
			const float v =
				static_cast<float>(stack) /
				static_cast<float>(stacks);

			const float phi = v * pi;

			const float sinPhi = std::sin(phi);
			const float cosPhi = std::cos(phi);

			for (uint32_t slice = 0; slice <= slices; ++slice)
			{
				const float u =
					static_cast<float>(slice) /
					static_cast<float>(slices);

				const float theta = u * twoPi;

				const float sinTheta = std::sin(theta);
				const float cosTheta = std::cos(theta);

				const glm::vec3 normal = {
					sinPhi * cosTheta,
					cosPhi,
					sinPhi * sinTheta
				};

				/*
				 * UVではUを1-uに反転している。
				 * したがってTangentはthetaが減少する方向。
				 */
				const glm::vec3 tangent = {
					sinTheta,
					0.0f,
					-cosTheta
				};

				data.vertices.push_back({
					normal * radius,
					normal,
					tangent,
					{ 1.0f - u, 1.0f - v }
					});
			}
		}

		for (uint32_t stack = 0; stack < stacks; ++stack)
		{
			for (uint32_t slice = 0; slice < slices; ++slice)
			{
				const uint32_t topLeft =
					stack * verticesPerRow + slice;

				const uint32_t bottomLeft =
					(stack + 1) * verticesPerRow + slice;

				const uint32_t topRight = topLeft + 1;
				const uint32_t bottomRight = bottomLeft + 1;

				// 南極側ではこの三角形が縮退する
				if (stack != stacks - 1)
				{
					data.indices.push_back(topLeft);
					data.indices.push_back(bottomRight);
					data.indices.push_back(bottomLeft);
				}

				// 北極側ではこの三角形が縮退する
				if (stack != 0)
				{
					data.indices.push_back(topLeft);
					data.indices.push_back(topRight);
					data.indices.push_back(bottomRight);
				}
			}
		}

		data.primitiveMode = GL_TRIANGLES;

		return data;
	}

	GLMeshData createPlane(
		float width,
		float depth,
		uint32_t xSegments,
		uint32_t zSegments
	)
	{
		GLMeshData data;

		xSegments = std::max(xSegments, 1u);
		zSegments = std::max(zSegments, 1u);

		const uint32_t verticesPerRow = xSegments + 1;

		data.vertices.reserve(
			static_cast<size_t>(xSegments + 1) *
			static_cast<size_t>(zSegments + 1)
		);

		data.indices.reserve(
			static_cast<size_t>(xSegments) *
			static_cast<size_t>(zSegments) *
			6
		);

		const glm::vec3 normal = { 0.0f, 1.0f, 0.0f };
		const glm::vec3 tangent = { 1.0f, 0.0f, 0.0f };

		for (uint32_t z = 0; z <= zSegments; ++z)
		{
			const float v =
				static_cast<float>(z) /
				static_cast<float>(zSegments);

			const float positionZ = (v - 0.5f) * depth;

			for (uint32_t x = 0; x <= xSegments; ++x)
			{
				const float u =
					static_cast<float>(x) /
					static_cast<float>(xSegments);

				const float positionX = (u - 0.5f) * width;

				data.vertices.push_back({
					{ positionX, 0.0f, positionZ },
					normal,
					tangent,
					{ u, 1.0f - v }
					});
			}
		}

		for (uint32_t z = 0; z < zSegments; ++z)
		{
			for (uint32_t x = 0; x < xSegments; ++x)
			{
				const uint32_t topLeft =
					z * verticesPerRow + x;

				const uint32_t bottomLeft =
					(z + 1) * verticesPerRow + x;

				const uint32_t topRight = topLeft + 1;
				const uint32_t bottomRight = bottomLeft + 1;

				// +Y方向から見て反時計回り
				data.indices.push_back(topLeft);
				data.indices.push_back(bottomLeft);
				data.indices.push_back(bottomRight);

				data.indices.push_back(topLeft);
				data.indices.push_back(bottomRight);
				data.indices.push_back(topRight);
			}
		}

		data.primitiveMode = GL_TRIANGLES;

		return data;
	}

	GLMeshData createScreen() {
		//フルスクリーントライアングルを生成する
		std::vector<Vertex> vertices = {
			{
				{ -1.0f, -1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f }
			},
			{
				{ 3.0f, -1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 2.0f, 0.0f }
			},
			{
				{ -1.0f, 3.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 2.0f }
			}
		};
		std::vector<unsigned int> indices = { 0, 1, 2 };
		GLMeshData data;
		data.vertices = vertices;
		data.indices = indices;
		return data;
	}
}