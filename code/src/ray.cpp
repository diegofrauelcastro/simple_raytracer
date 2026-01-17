#include "ray.h"

using namespace Maths;

Ray::Ray(Vector3 _origin, Vector3 _direction)
	: origin(_origin),
	direction(_direction)
{
}

Vector3 Ray::GetPointInRay(float _t) const
{
	return origin + (_t*direction);
}

Vector3 Ray::LaunchRay(Ray& _ray)
{
	float a = 0.5f * (_ray.GetDirection().y + 1.0f);
	Vector3 color01 = (1.0f - a) * Vector3(1.0f, 1.0f, 1.0f) + a * Vector3(0.5f, 0.7f, 1.0f);
	return color01 * (float)255;
}
