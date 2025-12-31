#pragma once
#include <vector>
#include <glm.hpp>
#include "ObjLoader.h"
#include "ISerializable.h"
class Shader;

struct Vertex {
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Normal;
};
static_assert(sizeof(Vertex) == 32, "Vertex struct size must be 32 bytes");

class Mesh : public ISerializable
{
public:
	Mesh(const float* meshVertices, size_t vertexSize, unsigned int* indices, size_t indexSize);
	Mesh(DotsRendering::ObjData objData);
	void SetPrivates(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices, int newIndicesSize);

	~Mesh();
	void Draw(Shader* shader);

#pragma region Serialization, Saving and Loading
	bool WriteTo(std::ofstream& outFile) const override;
	bool ReadFrom(std::ifstream& inFile) override;

	void SaveToFile(const std::string& filePath);
	Mesh* LoadFromFile(const std::string& filePath);
#pragma endregion

private:
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	int IndicesSize = 0;

protected:
	std::string meshName; 
	unsigned int VBO, VAO, EBO;
	unsigned int textureID = 0;
};