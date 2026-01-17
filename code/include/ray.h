#pragma once

#include "vector3.h"

class Ray
{
private:
	Maths::Vector3 origin;
	Maths::Vector3 direction;

public:
	Ray(Maths::Vector3 _origin = Maths::Vector3::zero, Maths::Vector3 _direction = Maths::Vector3::forward);
	~Ray() = default;


	/// METHODS ///

	Maths::Vector3 GetPointInRay(float _t) const;
	bool IsPointInTriangle(const Maths::Vector3& _point, const Maths::Vector3& _triA, const Maths::Vector3& _triB, const Maths::Vector3& _triC, Maths::Vector3& _storedBaryCoords) const;
	bool DoesRayIntersectWithInfiniteTriPlane(const Maths::Vector3& _triA, const Maths::Vector3& _triB, const Maths::Vector3& _triC, Maths::Vector3& _storedIntersectionPoint) const;
	static Maths::Vector3 LaunchRay(Ray& _ray);


	/// GETTERS ///

	Maths::Vector3 GetOrigin()const { return origin; }
	Maths::Vector3 GetDirection()const { return direction; }
};