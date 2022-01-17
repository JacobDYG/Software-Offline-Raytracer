// RaytraceTexturedObject
// This class extends TexturedObject with specific functionality required for raytracing, such as guaranteeing triangles
// WHILE THIS CLASS GUARANTEES TRIANGLES, IN ITS CURRENT STATE, THE OBJ IS NOT GUARANTEED TO BE CORRECT

#pragma once
#include "TexturedObject.h"

struct TriangularFace
{
    unsigned int v0, v1, v2;
};

class RaytraceTexturedObject :
    public TexturedObject
{
public:
    // Constructor calls base class for now
    RaytraceTexturedObject();

    // Override reading to automatically triangulate
    bool ReadObjectStream(std::istream& geometryStream, std::istream& textureStream);
private:
    // Always stored as triangles for RT
    std::vector<TriangularFace> triangles;

    // Convert to triangles if neccasary (assuming convex polygons)
    bool initTriangles();
};

