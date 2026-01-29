#include "ray.h"
#include "scene.h"

#include <DebugLog/debug.h>

using namespace Maths;


Ray::Ray(Vector3 _origin, Vector3 _direction)
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

bool Ray::DoesRayIntersectWithScene(const Ray& _ray, const std::vector<Entity*>& _entities, HitData* _hitData)
{
    HitData h;
    float minDistanceHit = INFINITY;
    bool bHasFoundValidHit = false;

    // Iterate through all entities.
    for (size_t i = 0; i < _entities.size(); i++)
    {
        Vector3 offset = _entities[i]->transform.position;
        const Mesh* currMesh = _entities[i]->GetMesh();

        // Iterate through all the triangles of the current entity's mesh.
        for (uint32_t j = 0; j < currMesh->GetIndexCount(); j+=3)
        {
            // Ensure we have a valid triangle.
            size_t vertexCount = currMesh->GetVertexCount();
            uint32_t i1 = currMesh->GetIndices()[j];
            uint32_t i2 = currMesh->GetIndices()[j+1];
            uint32_t i3 = currMesh->GetIndices()[j+2];

            if (i1 >= vertexCount || i2 >= vertexCount || i3 >= vertexCount)
            {
                LOG_APP("/!\\ Tried to access a triangle with vertices out of bounds. Skipping...")
                continue;
            }

            // Get the 3 vertices of the triangle.
            h.triangle[0] = &currMesh->GetVertices()[i1];
            h.triangle[1] = &currMesh->GetVertices()[i2];
            h.triangle[2] = &currMesh->GetVertices()[i3];
            // Apply transform to the triangle.
            Vector3 positions[3] = { h.triangle[0]->position + offset, h.triangle[1]->position + offset, h.triangle[2]->position + offset };

            // Check collision with the infinite plane formed by this triangle.
            if (DoesRayIntersectWithInfiniteTriPlane(_ray, positions[0], positions[1], positions[2], &h.hitPoint))
            {
                float newDistance = (_ray.origin - h.hitPoint).Norm();
                if (newDistance > minDistanceHit)
                    continue;
                // If the point is closer than our last recorded point, check if it is inside the desired triangle.
                if (IsPointInTriangle(h.hitPoint, positions[0], positions[1], positions[2], &h.baryCoords))
                {
                    minDistanceHit = newDistance;
                    *_hitData = h;
                    bHasFoundValidHit = true;
                }
            }
        }
    }

    return bHasFoundValidHit;
}

Vector3 Ray::LaunchRay(const Ray& _ray, const std::vector<Entity*>& _entities)
{
    // Check for collision with any element (entity) in the scene.
    HitData hit;
    if (DoesRayIntersectWithScene(_ray, _entities, &hit))
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
    return *this;
}
