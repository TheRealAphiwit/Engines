#include "Mesh.h"
#include <glad\glad.h>
#include <glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include <iostream>

Mesh::Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize)
{
	IndicesSize = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, aVertexSize, someVertices, GL_STATIC_DRAW);

	EBO = 0;
	if (someIndices && aIndexSize > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndexSize, someIndices, GL_STATIC_DRAW);
	}

	// FIX: Use new glEnableVertexArrayAttrib later!
	// We're setting up 2 vertex attributes that uses different channels
	/*glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)( 3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	constexpr GLsizei stride = 8 * sizeof(float);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Mesh::Mesh(DotsRendering::ObjData objData)
{
	myVertices.resize(objData.vertices.size());

	for (size_t i = 0; i < objData.vertices.size(); ++i)
	{
		myVertices[i].Position = objData.vertices[i];
		myVertices[i].TexCoords = (i < objData.textCoords.size()) ? objData.textCoords[i] : glm::vec2(0.0f);
		myVertices[i].Normal = (i < objData.normals.size()) ? objData.normals[i] : glm::vec3(0, 1, 0);
	}

	myIndices = std::move(objData.indices);
	IndicesSize = (int)myIndices.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		myVertices.size() * sizeof(Vertex),
		myVertices.data(),
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		myIndices.size() * sizeof(unsigned int),
		myIndices.data(),
		GL_STATIC_DRAW
	);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(1);

	// Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::SetPrivates(std::vector<Vertex> newVertices, std::vector<unsigned int> newIndices, int newIndicesSize)
{
	myVertices = newVertices;
	myIndices = newIndices;
	IndicesSize = newIndicesSize;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader* shader)
{
	shader->Use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	shader->SetInt(0, "Default"); // new

	if (IndicesSize > 0)
		glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, nullptr);
	else
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(myVertices.size()));

	glBindVertexArray(0);
}

bool Mesh::WriteTo(std::ofstream& outFile) const
{
	if (!outFile) return false;

	// Serialize vertices
	size_t verticesSize = myVertices.size();
	outFile.write(reinterpret_cast<const char*>(&verticesSize), sizeof(verticesSize));
	outFile.write(reinterpret_cast<const char*>(myVertices.data()), verticesSize * sizeof(Vertex));

	// Serialize indices
	size_t indicesSize = myIndices.size();
	outFile.write(reinterpret_cast<const char*>(&indicesSize), sizeof(indicesSize));
	outFile.write(reinterpret_cast<const char*>(myIndices.data()), indicesSize * sizeof(unsigned int));

	return outFile.good();
}

bool Mesh::ReadFrom(std::ifstream& inFile)
{
	if (!inFile) return false;

	// Deserialize vertices
	size_t verticesSize = 0;
	inFile.read(reinterpret_cast<char*>(&verticesSize), sizeof(verticesSize));
	myVertices.resize(verticesSize);
	inFile.read(reinterpret_cast<char*>(myVertices.data()), verticesSize * sizeof(Vertex));

	// Deserialize indices
	size_t indicesSize = 0;
	inFile.read(reinterpret_cast<char*>(&indicesSize), sizeof(indicesSize));
	myIndices.resize(indicesSize);
	inFile.read(reinterpret_cast<char*>(myIndices.data()), indicesSize * sizeof(unsigned int));

	return inFile.good();
}

void Mesh::SaveToFile(const std::string& filePath)
{
	std::ofstream outFile(filePath, std::ios::binary);

	if (!outFile)
	{
		throw std::runtime_error("Failed to open file for saving: " + filePath);
	}

	if (!WriteTo(outFile))
	{
		throw std::runtime_error("Failed to write to file: " + filePath);
	}

	outFile.close();
}

Mesh* Mesh::LoadFromFile(const std::string& filePath)
{
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile)
	{
		throw std::runtime_error("Failed to open file for loading: " + filePath);
	}

	if (!ReadFrom(inFile))
	{
		throw std::runtime_error("Failed to read from file: " + filePath);
	}

	inFile.close();
	return this;
}
