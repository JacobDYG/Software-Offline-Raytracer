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

// Test intersection with a ray
// Tests against input ray, returns true if there was an intersection, and writes the nearest intersection to tNear and surfelOut
bool RaytraceTexturedObject::intersect(Ray ray, float& tNear, Surfel& surfelOut)
{
	bool intersection = false;
	for (auto& indexedTriangularFace : triangles)
	{
		// Get triangle positions to test against
		const Cartesian3 v0Pos = vertices[indexedTriangularFace.v0];
		const Cartesian3 v1Pos = vertices[indexedTriangularFace.v1];
		const Cartesian3 v2Pos = vertices[indexedTriangularFace.v2];
		Triangle triangle(v0Pos, v1Pos, v2Pos);
		
		// Initialise closest triangle to infinity
		float t = std::numeric_limits<float>::infinity();
		Surfel surfel;

		// Test intersection of this triangle, update values if it is closer
		if (triangle.intersection(ray, t) && t < tNear)
		{
			tNear = t;
			surfelOut = surfel;
			intersection = true;
		}
	}
	return intersection;
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
			IndexedTriangularFace triangle;
			triangle.v0 = face[0];
			triangle.v1 = face[1];
			triangle.v2 = face[2];
			triangles.push_back(triangle);
		}
		else if (face.size() == 3)
		{
			// Check there are 3 vertices available, just in case...
			IndexedTriangularFace triangle;
			triangle.v0 = face[0];
			triangle.v1 = face[1];
			triangle.v2 = face[2];

			triangles.push_back(triangle);
		}
	}
	return trianglesGenerated;
}
