#include "vector3.h"
using namespace Maths;

const Vector3 Vector3::zero = Vector3();
const Vector3 Vector3::right = Vector3(1.f, 0.f, 0.f);
const Vector3 Vector3::left = Vector3(-1.f, 0.f, 0.f);
const Vector3 Vector3::up = Vector3(0.f, 1.f, 0.f);
const Vector3 Vector3::down = Vector3(0.f, -1.f, 0.f);
const Vector3 Vector3::forward = Vector3(0.f, 0.f, 1.f);
const Vector3 Vector3::back = Vector3(0.f, 0.f, -1.f);

Vector3::Vector3()
	: x(0.f)
	, y(0.f)
	, z(0.f)
{
}

Vector3::Vector3(float _x, float _y, float _z)
	: x(_x)
	, y(_y)
	, z(_z)
{
}

Vector3::Vector3(float f)
	: x(f)
	, y(f)
	, z(f)
{
}

Vector3::Vector3(const Vector3& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
{
}

Vector3& Vector3::operator=(const Vector3& copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
	return *this;
}

float& Vector3::operator[](int index)
{
	assert(index == 0 || index == 1 || index == 2);

	if (index == 0)
		return x;
	else if (index == 1)
		return y;
	else
		return z;
}

float& Vector3::operator[](char coord)
{
	assert(coord == 'x' || coord == 'y' || coord == 'z');

	if (coord == 'x')
		return x;
	else if (coord == 'y')
		return y;
	else
		return z;
}

float& Vector3::operator[](const std::string& coord)
{
	assert(coord == "x" || coord == "y" || coord == "z");

	if (coord == "x")
		return x;
	else if (coord == "y")
		return y;
	else
		return z;
}

float Vector3::operator[](int index) const
{
	assert(index == 0 || index == 1 || index == 2);

	if (index == 0)
		return x;
	else if (index == 1)
		return y;
	else
		return z;
}

float Vector3::operator[](char coord) const
{
	assert(coord == 'x' || coord == 'y' || coord == 'z');

	if (coord == 'x')
		return x;
	else if (coord == 'y')
		return y;
	else
		return z;
}

float Vector3::operator[](const std::string& coord) const
{
	assert(coord == "x" || coord == "y" || coord == "z");

	if (coord == "x")
		return x;
	else if (coord == "y")
		return y;
	else
		return z;
}

Vector3 Vector3::operator+(const float f) const
{
	return Vector3(x + f, y + f, z + f);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	Add(v);
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	Add(-v);
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator*(float f) const
{
	return Scale(f);
}

Vector3& Vector3::operator*=(float f)
{
	Scale(f);
	return *this;
}

Vector3 Vector3::operator/(float f) const
{
	assert(f != 0.f);
	return Vector3(x / f, y / f, z / f);
}

Vector3& Vector3::operator/=(float f)
{
	assert(f != 0.f);
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

void Vector3::operator++(int i)
{
	(void)i;
	float length = GetMagnitude();
	assert(length > 0);
	x *= (length + 1) / length;
	y *= (length + 1) / length;
	z *= (length + 1) / length;
}

void Vector3::operator--(int i)
{
	(void)i;
	float length = GetMagnitude();
	assert(length > 0);
	x *= (length - 1) / length;
	y *= (length - 1) / length;
	z *= (length - 1) / length;
}

float Vector3::operator*(const Vector3& v) const
{
	return DotProduct(v);
}

bool Vector3::operator==(const Vector3& v) const
{
	return GetMagnitude() == v.GetMagnitude();
}

bool Vector3::operator!=(const Vector3& v) const
{
	return GetMagnitude() != v.GetMagnitude();
}

bool Vector3::operator<(const Vector3& v) const
{
	return GetMagnitude() < v.GetMagnitude();
}

bool Vector3::operator<=(const Vector3& v) const
{
	return GetMagnitude() <= v.GetMagnitude();
}

bool Vector3::operator>(const Vector3& v) const
{
	return GetMagnitude() > v.GetMagnitude();
}

bool Vector3::operator>=(const Vector3& v) const
{
	return GetMagnitude() >= v.GetMagnitude();
}

float Vector3::X() const
{
	return x;
}

float Vector3::Y() const
{
	return y;
}

float Vector3::Z() const
{
	return z;
}

float& Vector3::X()
{
	return x;
}

float& Vector3::Y()
{
	return y;
}

float& Vector3::Z()
{
	return z;
}

float Vector3::DotProduct(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::CrossProduct(const Vector3& v) const
{
	return Vector3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

Vector3 Vector3::ElementWiseProduct(const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

float Vector3::GetMagnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::Norm() const
{
	return GetMagnitude();
}

float Vector3::SquaredNorm() const
{
	return x * x + y * y + z * z;
}

Vector3 Vector3::Add(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3& Vector3::Add(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 Vector3::Scale(float f) const
{
	return Vector3(x * f, y * f, z * f);
}

Vector3& Vector3::Scale(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector3 Vector3::Opposite() const
{
	return -(*this);
}

Vector3 Vector3::Normalize() const
{
	float length = GetMagnitude();
	assert(length > 0);
	return Vector3(x / length, y / length, z / length);
}

Vector3& Vector3::Normalize()
{
	float length = GetMagnitude();
	assert(length > 0);
	x /= length;
	y /= length;
	z /= length;
	return *this;
}

Vector3 Vector3::MidPoint(const Vector3& p1, const Vector3& p2)
{
	return (p1 + p2) / 2;
}

float Vector3::Distance(const Vector3& p1, const Vector3& p2)
{
	return (p2 - p1).GetMagnitude();
}

float Vector3::AngleBetweenTwoVector3(const Vector3& v1, const Vector3& v2)
{
	float productNorms = v1.GetMagnitude() * v2.GetMagnitude();
	// Avoid division by zero
	if (productNorms < 0.001)
		return 0;
	float dotProduct = v1.DotProduct(v2);
	float angle = acosf(dotProduct / productNorms);
	return isnan(angle) ? 0 : angle;
}

std::ostream& Maths::operator<<(std::ostream& os, const Vector3& v)
{
	return os << v.X() << " ; " << v.Y() << " ; " << v.Z();
}

std::string Maths::operator+(const std::string& str, const Vector3& v)
{
	std::string result = str;
	result += "x = ";
	result += std::to_string(v.X());
	result += ", y = ";
	result += std::to_string(v.Y());
	result += ", z = ";
	result += std::to_string(v.Z());
	return result;
}

std::string& Maths::operator+=(std::string& str, const Vector3& v)
{
	str += "x = ";
	str += std::to_string(v.X());
	str += ", y = ";
	str += std::to_string(v.Y());
	str += ", z = ";
	str += std::to_string(v.Z());
	return str;
}

Vector3 Maths::operator*(float f, const Vector3& v)
{
	return Vector3(f * v.X(), f * v.Y(), f * v.Z());
}

Vector3 Maths::operator/(float f, const Vector3& v)
{
	assert(v.X() != 0.f);
	assert(v.Y() != 0.f);
	assert(v.Z() != 0.f);
	return Vector3(f / v.X(), f / v.Y(), f / v.Z());
}

Vector3 Maths::operator+(const Vector3& v1, const Vector3& v2)
{
	return v1.Add(v2);
}

Vector3 Maths::operator+(float f, const Vector3& v)
{
	return Vector3(f + v.x, f + v.y, f + v.z);
}

Vector3 Maths::operator-(const Vector3& v1, const Vector3& v2)
{
	return v1.Add(-v2);
}

Vector3 Maths::operator-(float f, const Vector3& v)
{
	return Vector3(f - v.x, f - v.y, f - v.z);
}
