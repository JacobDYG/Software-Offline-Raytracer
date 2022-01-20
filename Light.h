// Base class for all light types
#pragma once

#include <Matrix4.h>

class Light
{
public:
	// Attributes
	Matrix4 lightToWorld;
	Cartesian3 color;
	float intensity;

	// Constructor
	Light(const Matrix4 newLightToWorld) : lightToWorld(newLightToWorld), color(1.0f, 1.0f, 1.0f), intensity(1.0f) {};
};

