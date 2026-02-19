#pragma once

#include "vector3.h"
#include <vector>

// Forward declarations.
class Scene;
class MeshRendererComponent;
class Mesh;
struct Vertex;

struct HitData
{
	Maths::Vector3 hitPoint;
	Maths::Vector3 baryCoords;
	const Vertex* triangle[3] = {nullptr, nullptr, nullptr};
	float distanceFromRayOrigin = INFINITY;

	HitData() = default;
	HitData(const HitData& _copy);
	~HitData() = default;
	HitData& operator=(const HitData& _copy);
};

class Ray
{
private:
	/// RAY PROPERTIES ///

	Maths::Vector3 origin;
	Maths::Vector3 direction;

	
	/// CLASS HELPERS ///

	static bool DoesRayIntersectWithScene(const Ray& _ray, const std::vector<MeshRendererComponent*>& _entities, HitData* _storedHitData);
	static bool IsPointInTriangle(const Maths::Vector3& _point, const Maths::Vector3& _triA, const Maths::Vector3& _triB, const Maths::Vector3& _triC, Maths::Vector3* _storedBaryCoords);
	static bool DoesRayIntersectWithInfiniteTriPlane(const Ray& _ray, const Maths::Vector3& _triA, const Maths::Vector3& _triB, const Maths::Vector3& _triC, Maths::Vector3* _storedIntersectionPoint);
	static bool DoesRayIntersectWithMeshInLocalSpace(const Ray& _ray, const Mesh& _mesh, HitData* _storedHitData);
	static Vertex CreateInterpolatedVertexFromHitData(const HitData& _hitData);

public:
	/// CONSTRUCTOR & DESTRUCTOR ///

	Ray(const Maths::Vector3& _origin = Maths::Vector3::zero, const Maths::Vector3& _direction = Maths::Vector3::forward);
	Ray(const Ray& _copy);
	~Ray() = default;


	/// METHODS ///

	Maths::Vector3 GetPointInRay(float _t) const;
	static Maths::Vector3 LaunchRay(const Ray& _ray, const Scene& _sceneToRender);


	/// GETTERS ///

	const Maths::Vector3& GetOrigin() const { return origin; }
	const Maths::Vector3& GetDirection() const { return direction; }
};