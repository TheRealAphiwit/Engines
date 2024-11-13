#pragma once
class Shader;
class Texture;

class Mesh
{
public:
	Mesh(const float* meshVertices, size_t vertexSize, unsigned int* indices, size_t indexSize);
	~Mesh();
	void Draw(Shader* shader);

protected:
	unsigned int VBO, VAO, EBO;
};