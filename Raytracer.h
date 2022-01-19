// Class for ray trace specific state and functions
#pragma once
#include <RGBAImage.h>

// Raytrace specific
#include "Geometry.h"
#include "RaytraceTexturedObject.h"

// Constants
// Rendering modes
const unsigned int RT_ORTHO = 0;
const unsigned int RT_PERSPECTIVE = 1;

class Raytracer
{
private:
	// Pointer to object
	RaytraceTexturedObject* object;
	// and to render params
	RenderParameters* renderParameters;

	// Image to render to
	RGBAImage* frameBuffer;

	// Rendering options
	unsigned int projectionMode = RT_ORTHO;

	// Internal ray tracing methods
	Cartesian3 castRay(Ray ray);
public:
	// Constructor
	Raytracer(RGBAImage* newFrameBuffer, RaytraceTexturedObject* object, RenderParameters* newRenderParameters);

	// Main ray tracing routine
	void raytrace();

	// Getters and setters
	const unsigned int getProjectionMode() { return projectionMode; };
	void setProjectionOrtho() { projectionMode = RT_ORTHO; };
	void setProjectionPerspective() { projectionMode = RT_PERSPECTIVE; };
};