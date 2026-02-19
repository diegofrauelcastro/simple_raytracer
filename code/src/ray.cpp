#include "ray.h"
#include "scene.h"
#include "mesh_renderer_component.h"
#include "mesh.h"

#include <DebugLog/debug.h>

using namespace Maths;


Ray::Ray(const Vector3& _origin, const Vector3& _direction)
	: origin(_origin),
	direction(_direction)
{
}

Ray::Ray(const Ray& _copy)
    : origin(_copy.origin)
    , direction(_copy.direction)
{
}

Vector3 Ray::GetPointInRay(float _t) const
{
    return origin + (_t * direction);
}

bool Ray::DoesRayIntersectWithScene(const Ray& _ray, const std::vector<MeshRendererComponent*>& _entities, HitData* _hitData)
{
    HitData h;
    float minDistanceHit = INFINITY;
    bool hasFoundValidHit = false;

    // Iterate through all entities.
    for (size_t i = 0; i < _entities.size(); i++)
    {
		// Get the mesh and transform the ray into the entity's local space.
        const Mesh& currMesh = _entities[i]->GetMesh();
		Matrix4 w2oMat = _entities[i]->GetTransform().GetWorldToObjectMatrix();
		Ray rayInEntitySpace = Ray(
            w2oMat * Vector4(_ray.origin, 1.f),
            (w2oMat * Vector4(_ray.direction, 0.f)).Normalize()
        );

		// Check if the ray intersects with the current entity's mesh (in local space). If it does, the hit data is stored in the reference passed as a parameter, also in local space.
		if (DoesRayIntersectWithMeshInLocalSpace(rayInEntitySpace, currMesh, &h))
		{
			// If the point is closer than our last recorded point, store it in the reference passed.
			if (h.distanceFromRayOrigin < minDistanceHit)
			{
                // Record the new hit point.
				minDistanceHit = h.distanceFromRayOrigin;
				*_hitData = h;
				hasFoundValidHit = true;

				// Transform the hit point back to world space.
				_hitData->hitPoint = _entities[i]->GetTransform().GetObjectToWorldMatrix() * Vector4(_hitData->hitPoint, 1.f);

				// Transform the triangle vertices back to world space.
                for (int j = 0; j < 3; j++)
                    _hitData->triangle[j] = new Vertex(
                        _entities[i]->GetTransform().GetObjectToWorldMatrix() * Vector4(_hitData->triangle[j]->position, 1.f),
                        _entities[i]->GetTransform().GetNormalMatrix() * Vector4(_hitData->triangle[j]->normal, 0.f),
						_hitData->triangle[j]->color
                    );
			}
		}
    }

    return hasFoundValidHit;
}

Vector3 Ray::LaunchRay(const Ray& _ray, const Scene& _sceneToRender)
{
    // Check for collision with any element (entity) in the scene.
    HitData hit;
    if (DoesRayIntersectWithScene(_ray, _sceneToRender.GetMeshRenderersFrameCache(), &hit))
    {
        // Calculate color using the hit point data.
        Vertex newVertex = CreateInterpolatedVertexFromHitData(hit);
        return newVertex.color;
    }

    // If no collision at all, show the sky color (gradient).
    else
    {
	    float a = 0.5f * (_ray.GetDirection().y + 1.0f);
	    Vector3 color01 = (1.0f - a) * Vector3(1.0f, 1.0f, 1.0f) + a * Vector3(0.5f, 0.7f, 1.0f);
	    return color01 * (float)255;
    }
}

bool Ray::IsPointInTriangle(const Vector3& _point, const Vector3& _triA, const Vector3& _triB, const Vector3& _triC, Vector3* _storedBaryCoords)
{
    // Vectors from A
    Vector3 v0 = _triC - _triA;
    Vector3 v1 = _triB - _triA;
    Vector3 v2 = _point - _triA;

    // Compute dot products
    float dot00 = v0.DotProduct(v0);
    float dot01 = v0.DotProduct(v1);
    float dot02 = v0.DotProduct(v2);
    float dot11 = v1.DotProduct(v1);
    float dot12 = v1.DotProduct(v2);

    // Compute barycentric coordinates
    float denom = dot00 * dot11 - dot01 * dot01;
    if (fabs(denom) < 0.0001f)
        return false;
    float invDenom = 1.f / denom;
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    // Store the barycentric coordinates in the reference passed.
    *_storedBaryCoords = Vector3(u, v, 1.0f - u - v);

    // Check if point is in triangle
    return (u >= 0.f) && (v >= 0.f) && (u + v <= 1.f);
}

bool Ray::DoesRayIntersectWithInfiniteTriPlane(const Ray& _ray, const Vector3& _triA, const Vector3& _triB, const Vector3& _triC, Vector3* _storedIntersectionPoint)
{
    Vector3 u = _triB - _triA;
    Vector3 v = _triC - _triA;
    Vector3 n0 = u.CrossProduct(v).Normalize();
    float denom = n0.DotProduct(_ray.direction);
    *_storedIntersectionPoint = Vector3::zero;

    // If the line is strictly parallel to the plane, it never intersects.
    if (fabs(denom) < 0.00001f)
        return false;

    // Calculate the necessary t to intersect. If t is negative, the plane intersects but in the opposite direction of the ray, which we don't care.
    float t = n0.DotProduct(_triA - _ray.origin) / denom;
    if (t < 0.00001f)
        return false;

    // Otherwise, we store the intersection point.
    *_storedIntersectionPoint = _ray.origin + t * _ray.direction;
    return true;
}

bool Ray::DoesRayIntersectWithMeshInLocalSpace(const Ray& _ray, const Mesh& _mesh, HitData* _h)
{
    if (!_h)
        return false;

	bool hasFoundValidHit = false;

    // Iterate through all the triangles of the current entity's mesh.
    for (uint32_t j = 0; j < _mesh.GetIndexCount(); j += 3)
    {
        // Ensure we have a valid triangle.
        size_t vertexCount = _mesh.GetVertexCount();
        uint32_t i1 = _mesh.GetIndices()[j];
        uint32_t i2 = _mesh.GetIndices()[j + 1];
        uint32_t i3 = _mesh.GetIndices()[j + 2];

        if (i1 >= vertexCount || i2 >= vertexCount || i3 >= vertexCount)
        {
            LOG_APP("/!\\ Tried to access a triangle with vertices out of bounds. Skipping...")
            continue;
        }

        // Get the 3 vertices of the triangle.
        _h->triangle[0] = &_mesh.GetVertices()[i1];
        _h->triangle[1] = &_mesh.GetVertices()[i2];
        _h->triangle[2] = &_mesh.GetVertices()[i3];
        Vector3 positions[3] = { _h->triangle[0]->position, _h->triangle[1]->position, _h->triangle[2]->position };

        // Check collision with the infinite plane formed by this triangle.
        if (DoesRayIntersectWithInfiniteTriPlane(_ray, positions[0], positions[1], positions[2], &_h->hitPoint))
        {
            float newDistance = (_ray.origin - _h->hitPoint).Norm();
			if (newDistance > _h->distanceFromRayOrigin)
                continue;
            // If the point is closer than our last recorded point, check if it is inside the desired triangle.
            if (IsPointInTriangle(_h->hitPoint, positions[0], positions[1], positions[2], &_h->baryCoords))
            {
				hasFoundValidHit = true;
                _h->distanceFromRayOrigin = newDistance;
            }
        }
    }
	return hasFoundValidHit;
}

Vertex Ray::CreateInterpolatedVertexFromHitData(const HitData& _hitData)
{
    Vertex newVertex;
    Vector3 bCoord = _hitData.baryCoords;

    // We skip position, as we already know it from the hitPoint.
    // ... //
    
    // Interpolate color and clamp it between 0.f and 255.f .
    newVertex.color = _hitData.triangle[0]->color * bCoord.z + _hitData.triangle[1]->color * bCoord.x + _hitData.triangle[2]->color * bCoord.y;
    newVertex.color.x = fmin(255.f, newVertex.color.x); newVertex.color.x = fmax(0.f, newVertex.color.x);
    newVertex.color.y = fmin(255.f, newVertex.color.y); newVertex.color.y = fmax(0.f, newVertex.color.y);
    newVertex.color.z = fmin(255.f, newVertex.color.z); newVertex.color.z = fmax(0.f, newVertex.color.z);

    // Interpolate normal and keep it normalized.
    newVertex.normal = _hitData.triangle[0]->normal * bCoord.z + _hitData.triangle[1]->normal * bCoord.x + _hitData.triangle[2]->normal * bCoord.y;
    newVertex.normal = newVertex.normal.Normalize();

    return newVertex;
}

HitData::HitData(const HitData& _copy)
    : hitPoint(_copy.hitPoint)
    , baryCoords(_copy.baryCoords)
	, distanceFromRayOrigin(_copy.distanceFromRayOrigin)
{
    triangle[0] = _copy.triangle[0];
    triangle[1] = _copy.triangle[1];
    triangle[2] = _copy.triangle[2];
}

HitData& HitData::operator=(const HitData& _copy)
{
    triangle[0] = _copy.triangle[0];
    triangle[1] = _copy.triangle[1];
    triangle[2] = _copy.triangle[2];
    hitPoint = _copy.hitPoint;
    baryCoords = _copy.baryCoords;
	distanceFromRayOrigin = _copy.distanceFromRayOrigin;
    return *this;
}
