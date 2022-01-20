// Directional light

#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    // Attributes
    Cartesian3 direction;

    // Constructor
    DirectionalLight(const Matrix4 newLightToWorld, const Cartesian3 newColor = Cartesian3(1.0f, 1.0f, 1.0f), const float newIntensity = 1.0f);
};

