#include "Mesh.h"
#include <glad\glad.h>
#include <glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include <iostream>
#include "LightHandler.h"

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
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // Channel 0 handles position

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1); // Channel 1 handles UV

	// I need channel 2 -> handle in_Normal (shadows upadte: 2026-01-17)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(DotsRendering::ObjData objData)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	#pragma region Explaination
	// For every vertex; we're creating data for VBOs  
	#pragma endregion
	std::vector<float> vertexData;
	for (size_t i = 0; i < objData.vertices.size(); ++i) 
	{
		#pragma region Positions
		vertexData.push_back(objData.vertices[i].x);
		vertexData.push_back(objData.vertices[i].y);
		vertexData.push_back(objData.vertices[i].z);

		// Can possibly remove if()
		if (i < objData.textCoords.size())
		{
			vertexData.push_back(objData.textCoords[i].x);
			vertexData.push_back(objData.textCoords[i].y);
		}
		else
		{
			vertexData.push_back(0.0f);
			vertexData.push_back(0.0f);
		}
		#pragma endregion

		#pragma region Normals
		if (i < objData.normals.size())
		{
			vertexData.push_back(objData.normals[i].x);
			vertexData.push_back(objData.normals[i].y);
			vertexData.push_back(objData.normals[i].z);
		}
		else
		{
			vertexData.push_back(0.0f);
			vertexData.push_back(0.0f);
			vertexData.push_back(1.0f); // Default at 1 to point upwards
		}
		#pragma endregion
	}

	#pragma region VBO and EBO setup
	//VBO Creation
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	// Why repeat the following 3 times?
	#pragma region Explaination
	// We're setting up the vertex position, texture coords, and normals 
	#pragma endregion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objData.indices.size() * sizeof(unsigned int), objData.indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	IndicesSize = objData.indices.size();
	#pragma endregion
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
	/*
	glBindVertexArray(VAO);

	if (IndicesSize > 0)
	{
		glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(myVertices.size()));
	}

	glBindVertexArray(0);
	*/

	#pragma region Original Draw
	
	if (VAO == 0)
	{
		std::cout << "ERROR: Mesh has no VAO!\n";
		return;
	}

	// glBindVertexArray(VAO);

	//if (IndicesSize > 0)
	//{
	//	glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, nullptr);
	//}
	//else
	//{
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
	//}

	glBindVertexArray(VAO); 

	if (IndicesSize > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, (void*)0);

		glBindVertexArray(0);
	}

	if (EBO == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	
	#pragma endregion
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
