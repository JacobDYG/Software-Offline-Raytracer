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
		// Start with transformed vertices equal to vertices
		transformedVertices.resize(vertices.size());
		transformedNormals.resize(normals.size());
		for (size_t i = 0; i < vertices.size(); i++)
		{
			transformedVertices[i] = Homogeneous4(vertices[i]);
		}
		for (size_t i = 0; i < normals.size(); i++)
		{
			transformedNormals[i] = normals[i];
		}
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
		Triangle triangle(	transformedVertices[indexedTriangularFace.v0].Point(),
							transformedVertices[indexedTriangularFace.v1].Point(), 
							transformedVertices[indexedTriangularFace.v2].Point());
		
		// Initialise closest triangle to infinity
		float t = std::numeric_limits<float>::infinity();
		float u, v;
		Surfel surfel;

		// Test intersection of this triangle, update values if it is closer
		if (triangle.intersection(ray, t, u, v) && t < tNear)
		{
			tNear = t;
			// Create surfel
			surfel.position = ray.getOrigin() + t * ray.getDirection();
			// Barycentric coordinates
			float beta = u;
			float gamma = v;
			float alpha = 1.0f - beta - gamma;
			// Interpolate normals
			surfel.normal = alpha * transformedNormals[indexedTriangularFace.vn0] + 
							beta * transformedNormals[indexedTriangularFace.vn1] +
							gamma * transformedNormals[indexedTriangularFace.vn2];
			// Interpolate texture coord
			surfel.u =	alpha * textureCoords[indexedTriangularFace.vt0].x +
						beta * textureCoords[indexedTriangularFace.vt1].x +
						gamma * textureCoords[indexedTriangularFace.vt2].x;
			surfel.v =	alpha * textureCoords[indexedTriangularFace.vt0].y +
						beta * textureCoords[indexedTriangularFace.vt1].y +
						gamma * textureCoords[indexedTriangularFace.vt2].y;

			surfelOut = surfel;
			intersection = true;
		}
	}
	return intersection;
}

void RaytraceTexturedObject::calculateTransformations(RenderParameters* renderParameters)
{
	// Create transformation matrix
	Matrix4 transformationMat;
	transformationMat.SetIdentity();

	// Local transformations
	transformationMat = transformationMat * objectWorldMatrix;

	// World transformations
	// Visual translation first, -1 in z so image plane can be at 0
	transformationMat = transformationMat * Matrix4::TranslationMultMat(Cartesian3(renderParameters->xTranslate, renderParameters->yTranslate, -1.0f));
	// Rotation
	transformationMat = transformationMat * renderParameters->rotationMatrix;

	// Apply additional requested params
	float scale = renderParameters->zoomScale;
	if (renderParameters->scaleObject)
	{
		scale /= objectSize;
	}
	if (renderParameters->centreObject)
	{
		transformationMat = transformationMat * Matrix4::TranslationMultMat(Cartesian3(-centreOfGravity.x * scale, -centreOfGravity.y * scale, -centreOfGravity.z * scale));
	}

	// Transform all vertices
	for (size_t i = 0; i < vertices.size(); i++)
	{
		transformedVertices[i] = transformationMat * (Homogeneous4(scale * vertices[i]));
	}

	// Transform all normals
	for (size_t i = 0; i < normals.size(); i++)
	{
		transformedNormals[i] = renderParameters->rotationMatrix * normals[i];
	}
}

// Triangulate if neccasary. Returns true if any triangulation took place.
bool RaytraceTexturedObject::initTriangles()
{
	bool trianglesGenerated = false;
	//for (auto& face : faceVertices)
	for (size_t i = 0; i < faceVertices.size(); i++)
	{
		if (faceVertices[i].size() > 3)
		{
			// Triangulate
			trianglesGenerated = true;

			// Implement as neccasary... for now be lazy and use the first three vertices
			IndexedTriangularFace triangle;
			triangle.v0 = faceVertices[i][0];
			triangle.v1 = faceVertices[i][1];
			triangle.v2 = faceVertices[i][2];
			triangle.vn0 = faceNormals[i][0];
			triangle.vn1 = faceNormals[i][1];
			triangle.vn2 = faceNormals[i][2];
			triangle.vt0 = faceTexCoords[i][0];
			triangle.vt1 = faceTexCoords[i][1];
			triangle.vt2 = faceTexCoords[i][2];
			triangles.push_back(triangle);
		}
		else if (faceVertices[i].size() == 3)
		{
			// Check there are 3 vertices available, just in case...
			IndexedTriangularFace triangle;
			triangle.v0 = faceVertices[i][0];
			triangle.v1 = faceVertices[i][1];
			triangle.v2 = faceVertices[i][2];
			triangle.vn0 = faceNormals[i][0];
			triangle.vn1 = faceNormals[i][1];
			triangle.vn2 = faceNormals[i][2];
			triangle.vt0 = faceTexCoords[i][0];
			triangle.vt1 = faceTexCoords[i][1];
			triangle.vt2 = faceTexCoords[i][2];

			triangles.push_back(triangle);
		}
	}
	return trianglesGenerated;
}
