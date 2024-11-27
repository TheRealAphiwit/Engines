#pragma once
#include <vector>
#include <glm.hpp>
#include <string>

class Mesh;

namespace DotsRendering
{
	struct ObjData
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> textCoords;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
	};

	bool LoadOBJ(const std::string& filename, ObjData& outData);
	Mesh* LoadObjMesh(const std::string& filename);
}