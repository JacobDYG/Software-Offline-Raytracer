// Class for ray trace specific state and functions
#include "Raytracer.h"

// Standard libraries
#define _USE_MATH_DEFINES
#include <math.h>

// Utilities
#include "Cartesian3.h"

// RT Specific
#include "Geometry.h"

// Constructor
Raytracer::Raytracer(RGBAImage* newFrameBuffer)
{
	// Set framebuffer pointer
	frameBuffer = newFrameBuffer;
}

Cartesian3 Raytracer::castRay(Ray ray)
{
	// For now, return ray direction as color
	Cartesian3 rayDirection(ray.getDirection());
	Cartesian3 rayDirectionColor = (rayDirection + Cartesian3(1.0f, 1.0f, 1.0f) * 0.5f);
	return rayDirectionColor;
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
			float colNdc = ((float)col + 0.5f) / (float)(*frameBuffer).width;
			float rowNdc = ((float)row + 0.5f) / (float)(*frameBuffer).height;

			// Convert to screen space for image plane
			float colScreen = 2.0f * colNdc - 1.0f;
			float rowScreen = 2.0f * rowNdc - 1.0f;

			// Convert to camera space, accounting for aspect ratio, and field of view
			float fovRadians = 90.0f * (M_PI / 2.0f);
			float aspectRatio = (float)((*frameBuffer).width) / (float)((*frameBuffer).height);
			float colCamera = colScreen * aspectRatio * tan(fovRadians / 2.0f);
			float rowCamera = rowScreen * tan(fovRadians / 2.0f);

			// Calculate a ray through the image plane
			Cartesian3 rayOrigin(0.0f, 0.0f, 0.0f);
			Cartesian3 rayDirection(0.0f, 0.0f, 0.0f);

			// Depends on projection mode ortho = true;
			if (projectionMode == RT_ORTHO)
			{
				rayDirection = Cartesian3(colCamera, rowCamera, -1.0f) - Cartesian3(colCamera, rowCamera, 0.0f);
			}
			else
			{
				rayDirection = Cartesian3(colCamera, rowCamera, -1.0f) - rayOrigin;
			}

			// Normalise to get direction vector
			rayDirection = rayDirection.unit();

			// Initialise a ray
			Ray ray(rayOrigin, rayDirection);
			// Cast the ray
			Cartesian3 rayDirectionColor = castRay(ray);
			RGBAValue hitColor(rayDirectionColor.x * 255.0f, rayDirectionColor.y * 255.0f, rayDirectionColor.z * 255.0f, 1.0f);
			(*frameBuffer)[row][col] = hitColor;
		}
	}
}