#pragma once
#include <vector>
#include <glm.hpp>
#include "ObjLoader.h"
class Shader;

#pragma region Old Texture Implementation
// class Texture;
// We no longer need Texture ref here since that'll be in the 'Virtual Object'  
#pragma endregion

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TextCoords;
};

class Mesh
{
	// friend class DotsRendering::OptmizedLoader;

public:
	Mesh(const float* meshVertices, size_t vertexSize, unsigned int* indices, size_t indexSize);
	Mesh(DotsRendering::ObjData objData);

	~Mesh();
	void Draw(Shader* shader);

private:
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	int IndicesSize = 0;

protected:
	std::string meshName; // This should be able to be overwritten by its children
	unsigned int VBO, VAO, EBO;
};