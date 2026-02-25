#include "ray.h"
#include "scene.h"
#include "mesh_renderer_component.h"
#include "light_component.h"
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

bool Ray::DoesRayIntersectWithScene(const Ray& _ray, const std::vector<MeshRendererComponent*>& _meshRenderers, HitData* _hitData)
{
    float minDistanceHit = INFINITY;
    bool hasFoundValidHit = false;

    // Iterate through all entities with a mesh renderer component.
    for (size_t i = 0; i < _meshRenderers.size(); i++)
    {
        HitData hit;

		// Get the mesh and transform the ray into the entity's local space.
        const Mesh& currMesh = _meshRenderers[i]->GetMesh();
		Matrix4 w2oMat = _meshRenderers[i]->GetTransform().GetWorldToObjectMatrix();
		Ray rayInEntitySpace = Ray(
            w2oMat * Vector4(_ray.origin, 1.f),
            (w2oMat * Vector4(_ray.direction, 0.f)).Normalize()
        );

        // First check if ray intersects with AABB to discard unnecessary mesh tests.
        if (!DoesRayIntersectWithAABB(rayInEntitySpace, currMesh.GetBoundingBox()))
			continue;

		// Check if the ray intersects with the current entity's mesh (in local space). If it does, the hit data is stored in the reference passed as a parameter, also in local space.
		if (DoesRayIntersectWithMeshInLocalSpace(rayInEntitySpace, currMesh, &hit))
		{
			// If the point is closer than our last recorded point, store it in the reference passed.
			if (hit.distanceFromRayOrigin < minDistanceHit)
			{
                // Record the new hit point.
                hasFoundValidHit = true;
                minDistanceHit = hit.distanceFromRayOrigin;
                if (_hitData)
                {
                    // Retrieve the latest hit data.
                    *_hitData = hit;
                    _hitData->material = &_meshRenderers[i]->GetMaterial();

                    // Get the interpolated local space vertex that was hit (position, normal, color).
                    _hitData->interpolatedVertex = CreateInterpolatedVertexFromHitData(*_hitData);
                    _hitData->interpolatedVertex.position = _meshRenderers[i]->GetTransform().GetObjectToWorldMatrix() * Vector4(_hitData->interpolatedVertex.position, 1.f);
                    _hitData->interpolatedVertex.normal = _meshRenderers[i]->GetTransform().GetNormalMatrix() * Vector4(_hitData->interpolatedVertex.normal, 0.f);

                    // Transform the hit point back to world space.
                    _hitData->hitPoint = _hitData->interpolatedVertex.position;
                }
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
			return Color::red+Color::blue;

        // Calculate color using the hit point data.
		Color matColor = hit.material->albedo;
        Color albedo = hit.interpolatedVertex.color * matColor;

        // Lighting.
        //Color lighting = ComputeLightingAtPoint(hit.interpolatedVertex, _sceneToRender);

        // Next ray values.
        Color recursiveColor = Color::black;
        Vector3 newOrigin = hit.interpolatedVertex.position + 0.001f * hit.interpolatedVertex.normal; // Slight offset to avoid self-intersection.
        switch (hit.material->GetType())
        {
            // For diffuse materials, we will simply bounce the ray in a random direction on the hemisphere of the hit point normal.
            case MaterialType::DIFFUSE:
            {
                // Get the color from the ray bounced from the hit point.
                Vector3 newDirection = Vector3::GenerateRandomOnHemisphere(hit.interpolatedVertex.normal);
                float cosTheta = fmax(newDirection.DotProduct(hit.interpolatedVertex.normal), 0.0f);
                recursiveColor = LaunchRayRecursively(Ray(newOrigin, newDirection), _sceneToRender, _currentRecursionDepth + 1, _maxRecursionDepth) * cosTheta * 2.f;
                break;
            }
            // For metallic materials, we will reflect the ray on the hit point normal.
            case MaterialType::METALLIC:
            {
                const Vector3 incident = _ray.direction.Normalize();
                Vector3 reflectedDir = incident - 2.f * incident.DotProduct(hit.interpolatedVertex.normal) * hit.interpolatedVertex.normal;

                // Avoid reflecting below surface
                if (reflectedDir.DotProduct(hit.interpolatedVertex.normal) <= 0)
                    recursiveColor = Color::black;
                recursiveColor = LaunchRayRecursively(Ray(newOrigin, reflectedDir), _sceneToRender, _currentRecursionDepth + 1, _maxRecursionDepth);
                break;
            }
        }
       

        //Color finalColor = recursiveColor * lighting * albedo;
        Color finalColor = recursiveColor * albedo;
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
        // Store hit data.
        if (_storedHitData)
        {
            _storedHitData->hitPoint = hitPoint;
            _storedHitData->baryCoords = Vector3(u, v, 1.f - u - v);
            _storedHitData->distanceFromRayOrigin = Vector3::Distance(_ray.origin, hitPoint);
        }
	}

    return res;
}

Color Ray::ComputeLightingAtPoint(const Vertex& _hitVertex, const Scene& _sceneToRender)
{
	Color finalColor = Color::black;
    Vector3 shadowRayOrigin = _hitVertex.normal + 0.001f;
    Vector3 normal = _hitVertex.normal.Normalize();
	std::vector<LightComponent*> lights = _sceneToRender.GetLightComponentsFrameCache();
    for (size_t i = 0; i < lights.size(); i++)
    {
        // Compute a shadow ray for each light.
        Vector3 lightDir = (lights[i]->GetTransform().GetPosition() - _hitVertex.position);
		float lightDistance = lightDir.GetMagnitude();
		lightDir.Normalize();
		Ray shadowRay(shadowRayOrigin, lightDir);

        float NdotL = fmax(normal.DotProduct(lightDir), 0.0f);
        if (NdotL <= 0.0f)
            continue;

        // If the shadow ray hits something before finding the light, this light doesn't contribute to the final color.
		HitData shadowHit;
		bool hasHitSomething = DoesRayIntersectWithScene(shadowRay, _sceneToRender.GetMeshRenderersFrameCache(), &shadowHit);
		if (hasHitSomething && shadowHit.distanceFromRayOrigin < lightDistance)
            continue;

		// Otherwise, add the contribution of this light to the final color.
		finalColor += lights[i]->GetColor() * NdotL * lights[i]->GetIntensity();
	}

	return finalColor;
}

Color Ray::LaunchRay(const Ray& _ray, const Scene& _sceneToRender, int _maxRecursionDepth)
{
	return LaunchRayRecursively(_ray, _sceneToRender, 0, _maxRecursionDepth);
}

bool Ray::DoesRayIntersectWithMeshInLocalSpace(const Ray& _ray, const Mesh& _mesh, HitData* _h)
{
	bool hasFoundValidHit = false;
	float minDistanceHit = INFINITY;

    // Iterate through all the triangles of the current entity's mesh.
    for (uint32_t j = 0; j < _mesh.GetIndexCount(); j += 3)
    {
        HitData cachedData;

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
        cachedData.triangle[0] = &_mesh.GetVertices()[i1];
        cachedData.triangle[1] = &_mesh.GetVertices()[i2];
        cachedData.triangle[2] = &_mesh.GetVertices()[i3];
        Vector3 positions[3] = { cachedData.triangle[0]->position, cachedData.triangle[1]->position, cachedData.triangle[2]->position };

        // Check collision with the infinite plane formed by this triangle.
        if (DoesRayIntersectWithTriangle(_ray, positions[0], positions[1], positions[2], &cachedData))
        {
            if (cachedData.distanceFromRayOrigin > minDistanceHit)
                continue;

            minDistanceHit = cachedData.distanceFromRayOrigin;
            hasFoundValidHit = true;
            // Copy hit data.
            if (_h) *_h = cachedData;
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
    //newVertex.position = _hitData.hitPoint;
	newVertex.position.x = _hitData.triangle[0]->position.x * bCoord.z + _hitData.triangle[1]->position.x * bCoord.x + _hitData.triangle[2]->position.x * bCoord.y;
	newVertex.position.y = _hitData.triangle[0]->position.y * bCoord.z + _hitData.triangle[1]->position.y * bCoord.x + _hitData.triangle[2]->position.y * bCoord.y;
	newVertex.position.z = _hitData.triangle[0]->position.z * bCoord.z + _hitData.triangle[1]->position.z * bCoord.x + _hitData.triangle[2]->position.z * bCoord.y;
    
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
    , interpolatedVertex(_copy.interpolatedVertex)
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
    interpolatedVertex = _copy.interpolatedVertex;
    baryCoords = _copy.baryCoords;
	distanceFromRayOrigin = _copy.distanceFromRayOrigin;
	material = _copy.material;
    return *this;
}
