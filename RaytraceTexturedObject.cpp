#include "RaytraceTexturedObject.h"

RaytraceTexturedObject::RaytraceTexturedObject() : TexturedObject::TexturedObject()
{
}

bool RaytraceTexturedObject::ReadObjectStream(std::istream& geometryStream, std::istream& textureStream)
{
	// Call base class' read and then triangulate
	if (TexturedObject::ReadObjectStream(geometryStream, textureStream))
	{
		initTriangles();
		return true;
	}
	return false;
}

// Triangulate if neccasary. Returns true if any triangulation took place.
bool RaytraceTexturedObject::initTriangles()
{
	bool trianglesGenerated = false;
	for (auto& face : faceVertices)
	{
		if (face.size() > 3)
		{
			// Triangulate
			trianglesGenerated = true;

			// Implement as neccasary... for now be lazy and use the first three vertices
			TriangularFace triangle;
			triangle.v0 = face[0];
			triangle.v1 = face[1];
			triangle.v2 = face[2];
			triangles.push_back(triangle);
		}
		else if (face.size() == 3)
		{
			// Check there are 3 vertices available, just in case...
			TriangularFace triangle;
			triangle.v0 = face[0];
			triangle.v1 = face[1];
			triangle.v2 = face[2];

			triangles.push_back(triangle);
		}
	}
	return trianglesGenerated;
}
