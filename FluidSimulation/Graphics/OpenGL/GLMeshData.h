#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include<vector>
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 uv;
};
struct GLMeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	GLenum primitiveMode = GL_TRIANGLES;
};