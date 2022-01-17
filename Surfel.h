// Represents a surface element

#pragma once
#include <Cartesian3.h>

struct TexCoord
{
	float u = 0.0f;
	float v = 0.0f;
};

class Surfel
{
public:
	Cartesian3 position;
	TexCoord uv;
	Cartesian3 normal;

	// Safe constructor
	Surfel();
};

