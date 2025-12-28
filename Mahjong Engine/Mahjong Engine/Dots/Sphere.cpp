#include "Sphere.h"
#include <vector>
#include <glm.hpp>
#include <iostream>

static float vertices[] =
{
    // Top pole
     0.0f,  0.5f,  0.0f,   0.0f,  1.0f,  0.0f,   0.5f, 0.0f,

     // Ring 1
      0.3536f,  0.3536f,  0.0000f,   0.7071f,  0.7071f,  0.0f,   0.0f, 0.25f,
      0.2500f,  0.3536f,  0.2500f,   0.5f,     0.7071f, 0.5f,   0.125f, 0.25f,
      0.0000f,  0.3536f,  0.3536f,   0.0f,     0.7071f, 0.7071f,0.25f, 0.25f,
     -0.2500f,  0.3536f,  0.2500f,  -0.5f,     0.7071f, 0.5f,   0.375f,0.25f,
     -0.3536f,  0.3536f,  0.0000f,  -0.7071f,  0.7071f, 0.0f,   0.5f,  0.25f,
     -0.2500f,  0.3536f, -0.2500f,  -0.5f,     0.7071f,-0.5f,   0.625f,0.25f,
      0.0000f,  0.3536f, -0.3536f,   0.0f,     0.7071f,-0.7071f,0.75f, 0.25f,
      0.2500f,  0.3536f, -0.2500f,   0.5f,     0.7071f,-0.5f,   0.875f,0.25f,

      // Ring 2 (equator)
       0.5f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.5f,
       0.3536f, 0.0f, 0.3536f,  0.7071f, 0.0f, 0.7071f, 0.125f, 0.5f,
       0.0f,  0.0f,  0.5f,   0.0f, 0.0f, 1.0f,   0.25f, 0.5f,
      -0.3536f, 0.0f, 0.3536f, -0.7071f, 0.0f, 0.7071f, 0.375f,0.5f,
      -0.5f,  0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,   0.5f, 0.5f,
      -0.3536f, 0.0f,-0.3536f, -0.7071f, 0.0f,-0.7071f, 0.625f,0.5f,
       0.0f,  0.0f, -0.5f,   0.0f, 0.0f,-1.0f,   0.75f, 0.5f,
       0.3536f, 0.0f,-0.3536f,  0.7071f, 0.0f,-0.7071f, 0.875f,0.5f,

       // Bottom pole
        0.0f, -0.5f,  0.0f,   0.0f, -1.0f, 0.0f,   0.5f, 1.0f
};

// ---------------------------------------

static unsigned int indices[] =
{
    // Top cap
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 5,
    0, 5, 6,
    0, 6, 7,
    0, 7, 8,

    // Middle
    1, 9, 2,   2, 9,10,
    2,10, 3,   3,10,11,
    3,11, 4,   4,11,12,
    4,12, 5,   5,12,13,
    5,13, 6,   6,13,14,
    6,14, 7,   7,14,15,
    7,15, 8,   8,15,16,

    // Bottom cap
    9,17,10,
    10,17,11,
    11,17,12,
    12,17,13,
    13,17,14,
    14,17,15,
    15,17,16
};

// ---------------------------------------

Sphere::Sphere() : Mesh(vertices, sizeof(vertices), indices, sizeof(indices)){}

// Old version
//Sphere::Sphere(float radius, int rings, int sectors) : Mesh(nullptr, 0, nullptr, 0)
//{
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//
//    const float PI = 3.14159265359f;
//
//	// Generate vertices by rings and sectors - rings go from top to bottom & sectors go around the sphere
//    for (int r = 0; r <= rings; ++r)
//    {
//        float v = (float)r / rings;
//        float phi = v * PI;
//
//        for (int s = 0; s <= sectors; ++s)
//        {
//            float u = (float)s / sectors;
//            float theta = u * 2.0f * PI;
//
//            glm::vec3 pos(radius * sin(phi) * cos(theta), radius * cos(phi), radius * sin(phi) * sin(theta));
//
//            glm::vec3 normal = glm::normalize(pos);
//
//            vertices.push_back({pos, normal, glm::vec2(u, v)});
//        }
//    }
//
//	// Generate indices
//    for (int r = 0; r < rings; ++r)
//    {
//        for (int s = 0; s < sectors; ++s)
//        {
//            int cur = r * (sectors + 1) + s;
//            int next = cur + sectors + 1;
//
//            indices.push_back(cur);
//            indices.push_back(next);
//            indices.push_back(cur + 1);
//
//            indices.push_back(cur + 1);
//            indices.push_back(next);
//            indices.push_back(next + 1);
//        }
//    }
//
//	SetPrivates(vertices, indices, (int)indices.size()); // This is not enough - we need to set up VBO, VAO, EBO as well
//}
