// Class for ray trace specific state and functions
#pragma once
#include <RGBAImage.h>

// Raytrace specific
#include "Geometry.h"

// Constants
// Rendering modes
const unsigned int RT_ORTHO = 0;
const unsigned int RT_PERSPECTIVE = 1;

class Raytracer
{
private:
	// Image to render to
	RGBAImage* frameBuffer;

	// Rendering options
	unsigned int projectionMode = RT_ORTHO;

	// Internal ray tracing methods
	Cartesian3 castRay(Ray ray);
public:
	// Constructor
	Raytracer(RGBAImage* newFrameBuffer);

	// Main ray tracing routine
	void raytrace();

	// Getters and setters
	const unsigned int getProjectionMode() { return projectionMode; };
	void setProjectionOrtho() { projectionMode = RT_ORTHO; };
	void setProjectionPerspective() { projectionMode = RT_PERSPECTIVE; };
};