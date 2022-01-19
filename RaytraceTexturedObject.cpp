#include "RaytraceTexturedObject.h"

// For homogeneous coords
#include "Homogeneous4.h"

RaytraceTexturedObject::RaytraceTexturedObject() : TexturedObject::TexturedObject(), objectWorldMatrix(Matrix4::Identity())
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
bool RaytraceTexturedObject::intersect(Ray ray, float& tNear, Surfel& surfelOut, RenderParameters* renderParameters)
{
	// Create transformation matrix
	objectWorldMatrix.SetIdentity();

	// Visual translation first, -1 in z so image plane can be at 0
	objectWorldMatrix = objectWorldMatrix * Matrix4::TranslationMultMat(Cartesian3(renderParameters->xTranslate, renderParameters->yTranslate, -1.0f));
	// Rotation
	objectWorldMatrix = objectWorldMatrix * renderParameters->rotationMatrix;

	// Apply additional requested params
	if (renderParameters->scaleObject)
	{
		objectWorldMatrix.SetScale(renderParameters->zoomScale, renderParameters->zoomScale, renderParameters->zoomScale);
	}
	if (renderParameters->centreObject)
	{
		objectWorldMatrix.SetTranslation(Cartesian3(-centreOfGravity.x * renderParameters->zoomScale, -centreOfGravity.y * renderParameters->zoomScale, -centreOfGravity.z * renderParameters->zoomScale));
	}

	bool intersection = false;
	for (auto& indexedTriangularFace : triangles)
	{
		// Get triangle positions to test against
		Homogeneous4 v0Pos = objectWorldMatrix * Homogeneous4(vertices[indexedTriangularFace.v0]);
		Homogeneous4 v1Pos = objectWorldMatrix * Homogeneous4(vertices[indexedTriangularFace.v1]);
		Homogeneous4 v2Pos = objectWorldMatrix * Homogeneous4(vertices[indexedTriangularFace.v2]);
		Triangle triangle(v0Pos.Point(), v1Pos.Point(), v2Pos.Point());
		
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
