#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{
public:
	// Example values; radius = 0.5f, rings = 16, sectors = 32
    Sphere(float radius, int rings, int sectors);
};