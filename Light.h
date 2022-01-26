// Base class for all light types
#pragma once

// Utils
#include <Matrix4.h>

// RT Specific
#include <Surfel.h>

class Light
{
public:
	// Attributes
	Matrix4 lightToWorld;
	Cartesian3 color;
	float intensity;

	// Constructor
	Light(const Matrix4 newLightToWorld) : lightToWorld(newLightToWorld), color(1.0f, 1.0f, 1.0f), intensity(1.0f) {};

	// For getting direction to a surfel
	virtual Cartesian3 getDirection(Surfel surfel) = 0;

	// Update the lightToWorld matrix
	void replaceLightToWorld(const Matrix4 newLightToWorld) { lightToWorld = newLightToWorld; };
	void multLightToWorld(const Matrix4 multMat) { lightToWorld = lightToWorld * multMat; };
};

