#include "ray.h"
#include "scene.h"
#include "mesh_renderer_component.h"
#include "mesh.h"
#include "material.h"

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

    // Iterate through all entities with a mesh renderer component.
    for (size_t i = 0; i < _entities.size(); i++)
    {
		// Get the mesh and transform the ray into the entity's local space.
        const Mesh& currMesh = _entities[i]->GetMesh();
		Matrix4 w2oMat = _entities[i]->GetTransform().GetWorldToObjectMatrix();
		Ray rayInEntitySpace = Ray(
            w2oMat * Vector4(_ray.origin, 1.f),
            (w2oMat * Vector4(_ray.direction, 0.f)).Normalize()
        );

        // First check if ray intersects with AABB to discard unnecessary mesh tests.
        if (!DoesRayIntersectWithAABB(rayInEntitySpace, currMesh.GetBoundingBox()))
			continue;

		// Check if the ray intersects with the current entity's mesh (in local space). If it does, the hit data is stored in the reference passed as a parameter, also in local space.
		if (DoesRayIntersectWithMeshInLocalSpace(rayInEntitySpace, currMesh, &h))
		{
			// If the point is closer than our last recorded point, store it in the reference passed.
			if (h.distanceFromRayOrigin < minDistanceHit)
			{
                // Record the new hit point.
				minDistanceHit = h.distanceFromRayOrigin;
				*_hitData = h;
				_hitData->material = &_entities[i]->GetMaterial();
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

Color Ray::LaunchRayRecursively(const Ray& _ray, const Scene& _sceneToRender, int _currentRecursionDepth, int _maxRecursionDepth)
{
    if (_currentRecursionDepth >= _maxRecursionDepth)
        return Color::black;

    // Check for collision with any element (entity) in the scene.
	HitData hit;
    if (DoesRayIntersectWithScene(_ray, _sceneToRender.GetMeshRenderersFrameCache(), &hit))
    {
        if (hit.material == nullptr)
			return Color::black;

        // Calculate color using the hit point data.
        Vertex newVertex = CreateInterpolatedVertexFromHitData(hit);
		Color matColor = hit.material->albedo;
        Color albedo = newVertex.color * matColor;

        // Next ray values.
		Color recursiveColor = Color::black;
        newVertex.normal.Normalize();
        Vector3 newOrigin = newVertex.position + 0.001f * newVertex.normal; // Slight offset to avoid self-intersection.
        switch (hit.material->GetType())
		{
            // For diffuse materials, we will simply bounce the ray in a random direction on the hemisphere of the hit point normal.
            case MaterialType::DIFFUSE:
            {
                // Get the color from the ray bounced from the hit point.
                Vector3 newDirection = Vector3::GenerateRandomOnHemisphere(newVertex.normal);
                float cosTheta = fmax(newDirection.DotProduct(newVertex.normal), 0.0f);
                recursiveColor = LaunchRayRecursively(Ray(newOrigin, newDirection), _sceneToRender, _currentRecursionDepth + 1, _maxRecursionDepth) * cosTheta * 2.f;
                break;
            }
            // For metallic materials, we will reflect the ray on the hit point normal.
            case MaterialType::METALLIC:
            {
                const Vector3 incident = _ray.direction.Normalize();
                Vector3 reflectedDir = incident - 2.f * incident.DotProduct(newVertex.normal) * newVertex.normal;

                // Avoid reflecting below surface
                if (reflectedDir.DotProduct(newVertex.normal) <= 0)
                    recursiveColor = Color::black;
                recursiveColor = LaunchRayRecursively(Ray(newOrigin, reflectedDir), _sceneToRender, _currentRecursionDepth + 1, _maxRecursionDepth);
                break;
            }
        }

        // Final color.
		Color finalColor = albedo * recursiveColor;
        return finalColor;
    }
    // If no collision at all, show the sky color (gradient).
    else
    {
        float a = 0.5f * (_ray.GetDirection().y + 1.0f);
		Color color01 =  (Color::white * (1.0f - a)) + (Color(0.5f, 0.7f, 1.0f) * a);
        return color01;
	}
}

bool Ray::DoesRayIntersectWithAABB(const Ray& _ray, const AABB& _aabb)
{
	Maths::Vector3 boxMin = _aabb.min;
	Maths::Vector3 boxMax = _aabb.max;

	// Using the "slab method" for ray-AABB intersection.
    float tMin = 0.f;
    float tMax = INFINITY;
    for (int i = 0; i < 3; i++)
    {
        if (fabs(_ray.direction[i]) < 0.00001f)
        {
			// Parallel to slab. No hit if origin not within slab.
            if (_ray.origin[i] < boxMin[i] || _ray.origin[i] > boxMax[i])
                return false;
        }
        else
        {
            // Compute intersection t value of ray with near and far plane of slab
            float invD = 1.f / _ray.direction[i];
            float t0 = (boxMin[i] - _ray.origin[i]) * invD;
            float t1 = (boxMax[i] - _ray.origin[i]) * invD;
            if (invD < 0.f)
                std::swap(t0, t1);
            tMin = fmax(tMin, t0);
            tMax = fmin(tMax, t1);
            if (tMax <= tMin)
                return false;
        }
    }
	return true;
}

// Möller and Trumbore, "Fast, Minimum Storage Ray-Triangle Intersection", Journal of Graphics Tools, vol. 2, 1997, p. 21–28
bool Ray::DoesRayIntersectWithTriangle(const Ray& _ray, const Vector3& _triA, const Vector3& _triB, const Vector3& _triC, HitData* _storedHitData)
{
    if (!_storedHitData)
		return false;

    Vector3 E1 = _triB - _triA;
    Vector3 E2 = _triC - _triA;
    
    Vector3 N = E1.CrossProduct(E2);
    float det = -_ray.direction.DotProduct(N);
    float invdet = 1.0f / det;
    Vector3 AO = _ray.origin - _triA;
    Vector3 DAO = AO.CrossProduct(_ray.direction);

    float u = E2.DotProduct(DAO) * invdet;
    float v = -E1.DotProduct(DAO) * invdet;
    float t = AO.DotProduct(N) * invdet;
    bool res = (det >= 0.00001f && t >= 0.0f && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f);
    if (res)
    {
		Vector3 hitPoint = _ray.GetPointInRay(t);
        _storedHitData->hitPoint = hitPoint;
        _storedHitData->baryCoords = Vector3(u, v, 1.f - u - v);
        _storedHitData->distanceFromRayOrigin = Vector3::Distance(_ray.origin, hitPoint);
	}

    return res;
}

Color Ray::LaunchRay(const Ray& _ray, const Scene& _sceneToRender, int _maxRecursionDepth)
{
	return LaunchRayRecursively(_ray, _sceneToRender, 0, _maxRecursionDepth);
}

bool Ray::DoesRayIntersectWithMeshInLocalSpace(const Ray& _ray, const Mesh& _mesh, HitData* _h)
{
    if (!_h)
        return false;

	// TODO Optimization : Here test if the ray intersects the bounding box of the mesh before testing all the triangles.

	bool hasFoundValidHit = false;
	float minDistanceHit = INFINITY;

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
        if (DoesRayIntersectWithTriangle(_ray, positions[0], positions[1], positions[2], _h))
        {
            if (_h->distanceFromRayOrigin > minDistanceHit)
                continue;

            minDistanceHit = _h->distanceFromRayOrigin;
            hasFoundValidHit = true;
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
    
    // Interpolate color and clamp it between 0.f and 1.f .
	newVertex.color = Color::black;
	newVertex.color.r = _hitData.triangle[0]->color.r * bCoord.z + _hitData.triangle[1]->color.r * bCoord.x + _hitData.triangle[2]->color.r * bCoord.y;
	newVertex.color.g = _hitData.triangle[0]->color.g * bCoord.z + _hitData.triangle[1]->color.g * bCoord.x + _hitData.triangle[2]->color.g * bCoord.y;
	newVertex.color.b = _hitData.triangle[0]->color.b * bCoord.z + _hitData.triangle[1]->color.b * bCoord.x + _hitData.triangle[2]->color.b * bCoord.y;

    newVertex.color.r = fmin(1.f, newVertex.color.r); newVertex.color.r = fmax(0.f, newVertex.color.r);
    newVertex.color.g = fmin(1.f, newVertex.color.g); newVertex.color.g = fmax(0.f, newVertex.color.g);
    newVertex.color.b = fmin(1.f, newVertex.color.b); newVertex.color.b = fmax(0.f, newVertex.color.b);

    // Interpolate normal and keep it normalized.
    newVertex.normal = _hitData.triangle[0]->normal * bCoord.z + _hitData.triangle[1]->normal * bCoord.x + _hitData.triangle[2]->normal * bCoord.y;
    newVertex.normal = newVertex.normal.Normalize();

    return newVertex;
}

HitData::HitData(const HitData& _copy)
    : hitPoint(_copy.hitPoint)
    , baryCoords(_copy.baryCoords)
	, distanceFromRayOrigin(_copy.distanceFromRayOrigin)
	, material(_copy.material)
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
	material = _copy.material;
    return *this;
}
