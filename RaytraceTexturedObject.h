// RaytraceTexturedObject
// This class extends TexturedObject with specific functionality required for raytracing, such as guaranteeing triangles
// WHILE THIS CLASS GUARANTEES TRIANGLES, IN ITS CURRENT STATE, THE OBJ IS NOT GUARANTEED TO BE CORRECT

#pragma once

// Custom classes
#include "TexturedObject.h"
#include "Matrix4.h"
// RT Specific
#include "Geometry.h"
#include "Surfel.h"

struct IndexedTriangularFace
{
    unsigned int v0, v1, v2;
};

class RaytraceTexturedObject :
    public TexturedObject
{
private:
    // Always stored as triangles for RT
    std::vector<IndexedTriangularFace> triangles;

    // Matrix for translating this object
    Matrix4 objectWorldMatrix;

    // Convert to triangles if neccasary (assuming convex polygons)
    bool initTriangles();
public:
    // Constructor calls base class for now
    RaytraceTexturedObject();

    // Override reading to automatically triangulate
    bool ReadObjectStream(std::istream& geometryStream, std::istream& textureStream);

    // Test ray intersection
    bool intersect(Ray ray, float& tNear, Surfel& surfelOut, RenderParameters* renderParameters);
};

