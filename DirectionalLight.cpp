#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Matrix4 newLightToWorld, const Cartesian3 newColor, const float newIntensity): Light(newLightToWorld)
{
	color = newColor;
	intensity = newIntensity;
}

Cartesian3 DirectionalLight::getDirection(Surfel surfel)
{
	// Calculate the direction by multiplying the light to world matrix
	// The direction can be modified, then, by the lightToWorld matrix
	// Directional lights don't care about the position of the surfel, so just return the direction
	return (lightToWorld * Homogeneous4(0.0f, 0.0f, 1.0f, 1.0f)).Point();
}
