#include "Sphere.h"
#include <vector>
#include <glm.hpp>

Sphere::Sphere(float radius, int rings, int sectors) : Mesh(nullptr, 0, nullptr, 0)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;

	// Generate vertices by rings and sectors - rings go from top to bottom & sectors go around the sphere
    for (int r = 0; r <= rings; ++r)
    {
        float v = (float)r / rings;
        float phi = v * PI;

        for (int s = 0; s <= sectors; ++s)
        {
            float u = (float)s / sectors;
            float theta = u * 2.0f * PI;

            glm::vec3 pos(radius * sin(phi) * cos(theta), radius * cos(phi), radius * sin(phi) * sin(theta));

            glm::vec3 normal = glm::normalize(pos);

            vertices.push_back({pos, normal, glm::vec2(u, v)});
        }
    }

	// Generate indices
    for (int r = 0; r < rings; ++r)
    {
        for (int s = 0; s < sectors; ++s)
        {
            int cur = r * (sectors + 1) + s;
            int next = cur + sectors + 1;

            indices.push_back(cur);
            indices.push_back(next);
            indices.push_back(cur + 1);

            indices.push_back(cur + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

	SetPrivates(vertices, indices, (int)indices.size());
}
