#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Matrix4 newLightToWorld, const Cartesian3 newColor, const float newIntensity): Light(newLightToWorld)
{
	color = newColor;
	intensity = newIntensity;
	// Calculate the direction by multiplying the light to world matrix
	// The direction can be modified, then, by the lightToWorld matrix
	direction = lightToWorld * Cartesian3(0.0f, 0.0f, -1.0f);
}
