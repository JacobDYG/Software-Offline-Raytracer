// Ray class
#pragma once

#include <Cartesian3.h>
class Ray
{
	// Minimal class for representing a ray
protected:
	Cartesian3 origin;
	Cartesian3 direction;
public:
	// Constructors: default, inplace, and copy
	Ray() : origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, -1.0f) {};
	Ray(const Cartesian3& otherOrigin, const Cartesian3& otherDirection) : origin(otherOrigin), direction(otherDirection) {};
	Ray(Ray& other) : origin(other.getOrigin()), direction(other.getDirection()) {};

	// Getters
	Cartesian3 getOrigin() const { return origin; };
	Cartesian3 getDirection() const { return direction; };
	Cartesian3 getHalfLine() const { return (origin + direction); };
};

// stream output
std::ostream& operator << (std::ostream& outStream, const Ray& value);