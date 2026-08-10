#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include<vector>
struct Vertex2D {
	glm::vec2 position;
	glm::vec2 normal;	//2Dでライティングしたい時用
	glm::vec2 uv;
};
struct GL2DMeshData {
	std::vector<Vertex2D> vertices;
	std::vector<uint32_t> indices;
	GLenum primitiveMode = GL_TRIANGLES;
};