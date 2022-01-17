// Includes all geometry related classes
#pragma once

#include <Cartesian3.h>
#include <Matrix4.h>
#include <Surfel.h>
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

// stream output for ray
std::ostream& operator << (std::ostream& outStream, const Ray& value);

class Triangle
{
	// Minimal class for representing a triangle
private:
	// Attributes
	Cartesian3 v0, v1, v2;
public:
	// Constructors
	Triangle();
	Triangle(Cartesian3 inV0, Cartesian3 inV1, Cartesian3 inV2);

	// If triangle needs to be transformed to some other coordinate space
	void triMultMat(Matrix4 matrix);

	// Test intersection with a ray
	bool intersection(Ray ray, float& t, Surfel& surfelOut);

	// Getters
	Cartesian3 getV0() const { return v0; };
	Cartesian3 getV1() const { return v1; };
	Cartesian3 getV2() const { return v2; };
};

// stream output for triangle
std::ostream& operator << (std::ostream& outStream, const Triangle& value);
