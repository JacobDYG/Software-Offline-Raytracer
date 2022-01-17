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

bool Triangle::intersection(Ray ray, float& t)
{
    // Compute the normal of the plane this triangle lies on
    Cartesian3 v0v1 = v1 - v0;
    Cartesian3 v0v2 = v2 - v0;
    // Get normal with cross product. Doesn't need normalisation
    Cartesian3 planeNormal = v0v1.cross(v0v2);

    // Check if ray and plane are parallel
    float planeNDotRayD = planeNormal.dot(ray.getDirection());
    if (abs(planeNDotRayD) < 1e-8) return false; // 0 or very close, therefore consider parallel

    // Calculate D (dist from origin to plane, on a line parallel to the plane's normal)
    float planeD = planeNormal.dot(v0);

    // Compute t, intersection distance along ray
    float distT = (planeNormal.dot(ray.getOrigin()) + planeD) / planeNDotRayD;
    // Check if the plane is behind the ray
    if (distT < 0) return false;

    // Compute the intersection point
    Cartesian3 intersectionPoint = ray.getOrigin() + distT * ray.getDirection();

    // Inside out test
    Cartesian3 c;   // Vector perpendicular to triangle's plane

    // Edge 0
    Cartesian3 edge0 = v1 - v0;
    Cartesian3 pv0 = intersectionPoint - v0;
    c = edge0.cross(pv0);
    if (planeNormal.dot(c) < 0) return false; // P is on the right of this edge

    // Edge 1
    Cartesian3 edge1 = v2 - v1;
    Cartesian3 pv1 = intersectionPoint - v1;
    c = edge1.cross(pv1);
    if (planeNormal.dot(c) < 0) return false; // P is on the right of this edge

    // Edge 2
    Cartesian3 edge2 = v0 - v2;
    Cartesian3 pv2 = intersectionPoint - v2;
    c = edge2.cross(pv2);
    if (planeNormal.dot(c) < 0) return false; // P is on the right of this edge

    // Update t
    t = distT;

    return true;
}

// stream output for triangle
std::ostream& operator << (std::ostream& outStream, const Triangle& value)
{ // stream output
    outStream << "v0: " << value.getV0() << ", v1: " << value.getV1() << ", v2: " << value.getV2();
    return outStream;
}