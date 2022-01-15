// Class for ray trace specific state and functions
#include "Raytracer.h"

// Standard libraries
#define _USE_MATH_DEFINES
#include <math.h>

// Utilities
#include "Cartesian3.h"

// RT Specific
#include "Ray.h"


// Constructor
Raytracer::Raytracer(RGBAImage* newFrameBuffer)
{
	// Set framebuffer pointer
	frameBuffer = newFrameBuffer;
}

// Main ray tracing routine
void Raytracer::raytrace()
{
	// Cast a ray for every pixel
	// For rows
	for (size_t row = 0; row < (*frameBuffer).height; row++)
	{
		// For columns
		for (size_t col = 0; col < (*frameBuffer).width; col++)
		{
			// Convert rows and columns to NDC
			// note that range used is [0:1] compared to [-1:1] for rasterisation
			float rowNdc = ((float)row + 0.5f) / (float)(*frameBuffer).height;
			float colNdc = ((float)col + 0.5f) / (float)(*frameBuffer).width;

			// Convert to screen space for image plane
			float rowScreen = 2.0f * rowNdc - 1.0f;
			float colScreen = 1.0f - 2.0f * colNdc;

			// Convert to camera space, accounting for aspect ratio, and field of view
			float fovRadians = M_PI / 2.0f;
			float rowCamera = rowScreen * ((*frameBuffer).width / (*frameBuffer).height) * tan(fovRadians / 2.0f);
			float colCamera = colScreen * tan(fovRadians / 2.0f);

			// Calculate a ray through the image plane
			Cartesian3 rayOrigin(0.0f, 0.0f, 0.0f);
			Cartesian3 rayDirection(0.0f, 0.0f, 0.0f);

			// Depends on projection mode ortho = true;
			if (projectionMode == RT_ORTHO)
			{
				rayDirection = Cartesian3(rowCamera, colCamera, -1.0f) - Cartesian3(rowCamera, colCamera, 0.0f);
			}
			else
			{
				rayDirection = Cartesian3(rowCamera, colCamera, -1.0f) - rayOrigin;
			}

			// Normalise to get direction vector
			rayDirection = rayDirection.unit();

			// Initialise a ray
			Ray ray(rayOrigin, rayDirection);
		}
	}
	std::cout << "Done!" << std::endl;
}