// Includes all geometry related classes
#include "Geometry.h"

// stream output for ray
std::ostream& operator << (std::ostream& outStream, const Ray& value)
{ // stream output
    outStream << "Ray Origin: " << value.getOrigin() << " Ray Direction: " << value.getDirection();
    return outStream;
}

// Init to safe values
Triangle::Triangle()
{
    Triangle(Cartesian3(), Cartesian3(), Cartesian3());
}

// Set values
Triangle::Triangle(Cartesian3 inV0, Cartesian3 inV1, Cartesian3 inV2)
{
    v0 = inV0;
    v1 = inV1;
    v2 = inV2;
}

void Triangle::triMultMat(Matrix4 matrix)
{
    v0 = matrix * v0;
    v1 = matrix * v1;
    v2 = matrix * v2;
}

// stream output for triangle
std::ostream& operator << (std::ostream& outStream, const Triangle& value)
{ // stream output
    outStream << "v0: " << value.getV0() << ", v1: " << value.getV1() << ", v2: " << value.getV2();
    return outStream;
}