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
Raytracer::Raytracer(RGBAImage* newFrameBuffer, RaytraceTexturedObject* objectIn, std::vector<Light*>* lightsIn, RenderParameters* newRenderParameters)
{
	// Set framebuffer pointer
	frameBuffer = newFrameBuffer;
	// and render obj
	object = objectIn;
	lights = lightsIn;
	// and params
	renderParameters = newRenderParameters;
}

Cartesian3 Raytracer::castRay(Ray ray)
{
	// For now, return white if there was an intersection, otherwise return ray direction as color
	float t = std::numeric_limits<float>::infinity();
	Surfel surfel;
	if (object->intersect(ray, t, surfel))
	{
		Cartesian3 color(0.7f, 0.7f, 0.7f);
		if (renderParameters->useLighting)
		{
			color = Cartesian3(0.0f, 0.0f, 0.0f);
			for (auto& light : *lights)
			{
				// Ambient first
				color = color + Cartesian3(renderParameters->ambient * light->color * light->intensity);
				// Direction returned is calculated by a subclass of light, so directional and point are handled implicitly
				Cartesian3 lightDirection = light->getDirection(surfel);
				Cartesian3 surfaceNormal = surfel.normal;
				float diffuseAmount = surfaceNormal.dot(lightDirection);
				if (diffuseAmount > 0.0f)
				{
					color = color + Cartesian3(renderParameters->diffuse * diffuseAmount * light->color * light->intensity);
				}

				// Finally, specular
				// 'Camera' is at world origin
				Cartesian3 eyeVec = Cartesian3(0.0f, 0.0f, 0.0f) - Cartesian3(0.0f, 0.0f, 0.0f);
				Cartesian3 bisector = ((eyeVec + lightDirection) / 2.0f).unit();
				// Calculate specular
				// Check if the dot product is negative before raising to exponent, to avoid negatives becoming positives
				float dotProduct = surfaceNormal.dot(bisector);
				if (dotProduct < 0.0f)
				{
					dotProduct = 0.0f;
				}
				float specularAmount = pow(dotProduct, renderParameters->specularExponent);
				// If there is any specular, add it to the light
				if (specularAmount > 0.0f)
				{
					color = color + (Cartesian3(renderParameters->specular * light->color * light->intensity) * specularAmount);
				}
			}
		}
		if (renderParameters->texturedRendering)
		{
			// Convert to discrete texture coords
			RGBAImage* texture = &(object->texture);
			int texCol = std::round(surfel.u * texture->width);
			int texRow = std::round(surfel.v * texture->height);
			float red = (float)((*texture)[texRow][texCol]).red / 255.0f;
			float green = (float)((*texture)[texRow][texCol]).green / 255.0f;
			float blue = (float)((*texture)[texRow][texCol]).blue / 255.0f;

			if (renderParameters->textureModulation)
			{
				color = Cartesian3(red * color[0], green * color[1], red * color[2]);
			}
			else
			{
				color = Cartesian3(red, green, blue);
			}
		}
		return color;
	}

	// Return direction as colour
	Cartesian3 rayDirection(ray.getDirection());
	Cartesian3 rayDirectionColor = (rayDirection + Cartesian3(1.0f, 1.0f, 1.0f) * 0.5f);
	return rayDirectionColor;
}

// Main ray tracing routine
void Raytracer::raytrace()
{
	// Calculate transformations for all objects
	object->calculateTransformations(renderParameters);

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
				rayOrigin = Cartesian3(colScreen, rowScreen, 0.0f);
				rayDirection = Cartesian3(colScreen, rowScreen, -1.0f) - Cartesian3(colScreen, rowScreen, 0.0f);
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