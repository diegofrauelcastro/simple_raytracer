#include "vector2.h"

#include <iostream>
#include <math.h>
#include <string>
#include <cassert>
using namespace Maths;

const Vector2 Vector2::zero = Vector2();
const Vector2 Vector2::one = Vector2(1.f, 1.f);
const Vector2 Vector2::right = Vector2(1.f, 0.f);
const Vector2 Vector2::left = Vector2(- 1.f, 0.f);
const Vector2 Vector2::up = Vector2(1.f, 0.f);
const Vector2 Vector2::down = Vector2(-1.f, 0.f);

Vector2::Vector2()
	: x(0.f)
	, y(0.f)
{
}

Vector2::Vector2(float _x, float _y)
	: x(_x)
	, y(_y)
{
}

Vector2::Vector2(float f)
	: x(f)
	, y(f)
{
}

Vector2::Vector2(const Vector2& copy)
	: x(copy.x)
	, y(copy.y)
{
}

Vector2& Vector2::operator=(const Vector2& copy)
{
	x = copy.x;
	y = copy.y;
	return *this;
}

float& Vector2::operator[](int index)
{
	assert(index == 0 || index == 1);

	if (index == 0)
		return x;
	else
		return y;
}

float& Vector2::operator[](char coord)
{
	assert(coord == 'x' || coord == 'y');

	if (coord == 'x')
		return x;
	else
		return y;
}

float& Vector2::operator[](const std::string& coord)
{
	assert(coord == "x" || coord == "y");

	if (coord == "x")
		return x;
	else
		return y;
}

float Vector2::operator[](int index) const
{
	assert(index == 0 || index == 1);

	if (index == 0)
		return x;
	else
		return y;
}

float Vector2::operator[](char coord) const
{
	assert(coord == 'x' || coord == 'y');

	if (coord == 'x')
		return x;
	else
		return y;
}

float Vector2::operator[](const std::string& coord) const
{
	assert(coord == "x" || coord == "y");

	if (coord == "x")
		return x;
	else
		return y;
}

Vector2 Vector2::operator+(const float f) const
{
	return Vector2(x + f, y + f);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	Add(v);
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	Add(-v);
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator*(float f) const
{
	return Scale(f);
}

Vector2& Vector2::operator*=(float f)
{
	Scale(f);
	return *this;
}

Vector2 Vector2::operator/(float f) const
{
	assert(f != 0.f);
	return Vector2(x / f, y / f);
}

Vector2& Vector2::operator/=(float f)
{
	assert(f != 0.f);
	x /= f;
	y /= f;
	return *this;
}

void Vector2::operator++(int i)
{
	(void)i;
	float length = GetMagnitude();
	assert(length > 0);
	x *= (length + 1) / length;
	y *= (length + 1) / length;
}

void Vector2::operator--(int i)
{
	(void)i;
	float length = GetMagnitude();
	assert(length > 0);
	x *= (length - 1) / length;
	y *= (length - 1) / length;
}

float Vector2::operator*(const Vector2& v) const
{
	return DotProduct(v);
}

bool Vector2::operator==(const Vector2& v) const
{
	return GetMagnitude() == v.GetMagnitude();
}

bool Vector2::operator!=(const Vector2& v) const
{
	return GetMagnitude() != v.GetMagnitude();
}

bool Vector2::operator<(const Vector2& v) const
{
	return GetMagnitude() < v.GetMagnitude();
}

bool Vector2::operator<=(const Vector2& v) const
{
	return GetMagnitude() <= v.GetMagnitude();
}

bool Vector2::operator>(const Vector2& v) const
{
	return GetMagnitude() > v.GetMagnitude();
}

bool Vector2::operator>=(const Vector2& v) const
{
	return GetMagnitude() >= v.GetMagnitude();
}

float Vector2::X() const
{
	return x;
}

float Vector2::Y() const
{
	return y;
}

float& Vector2::X()
{
	return x;
}

float& Vector2::Y()
{
	return y;
}

float Vector2::DotProduct(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

float Vector2::CrossProduct(const Vector2& v) const
{
	return x * v.y - y * v.x;
}

Vector2 Vector2::ElementWiseProduct(const Vector2& v) const
{
	return Vector2(x * v.x, y * v.y);
}

float Vector2::GetMagnitude() const
{
	return sqrt(x * x + y * y);
}

float Vector2::Norm() const
{
	return GetMagnitude();
}

float Vector2::SquaredNorm() const
{
	return x * x + y * y;
}

Vector2 Vector2::Add(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2& Vector2::Add(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::Scale(float f) const
{
	return Vector2(x * f, y * f);
}

Vector2& Vector2::Scale(float f)
{
	x *= f;
	y *= f;
	return *this;
}

Vector2 Vector2::Opposite() const
{
	return -(*this);
}

Vector2 Vector2::Normalize() const
{
	float length = GetMagnitude();
	assert(length > 0);
	return Vector2(x / length, y / length);
}

Vector2& Vector2::Normalize()
{
	float length = GetMagnitude();
	assert(length > 0);
	x /= length;
	y /= length;
	return *this;
}

Vector2 Vector2::Rotate(float radAngle, const Vector2& origin) const
{
	float newX = cosf(radAngle) * (x - origin.x) - sinf(radAngle) * (y - origin.y) + origin.x;
	float newY = sinf(radAngle) * (x - origin.x) + cosf(radAngle) * (y - origin.y) + origin.y;
	return Vector2(newX, newY);
}

Vector2& Vector2::Rotate(float radAngle, const Vector2& origin)
{
	x = cosf(radAngle) * (x - origin.x) - sinf(radAngle) * (y - origin.y) + origin.x;
	y = sinf(radAngle) * (x - origin.x) + cosf(radAngle) * (y - origin.y) + origin.y;
	return *this;
}

Vector2 Vector2::MidPoint(const Vector2& p1, const Vector2& p2)
{
	return (p1 + p2) / 2;
}

float Vector2::Distance(const Vector2& p1, const Vector2& p2)
{
	return (p2 - p1).GetMagnitude();
}

float Vector2::AngleBetweenTwoVector2(const Vector2& v1, const Vector2& v2)
{
	float productNorms = v1.GetMagnitude() * v2.GetMagnitude();
	// Avoid division by zero
	if (productNorms < 0.001)
		return 0;
	float dotProduct = v1.DotProduct(v2);
	float angle = acosf(dotProduct / productNorms);
	return isnan(angle) ? 0 : (angle*180/PI);
}
float Vector2::AngleRadBetweenTwoVector2(const Vector2& v1, const Vector2& v2)
{
	float productNorms = v1.GetMagnitude() * v2.GetMagnitude();
	// Avoid division by zero
	if (productNorms < 0.001)
		return 0;
	float dotProduct = v1.DotProduct(v2);
	float angle = acosf(dotProduct / productNorms);
	return isnan(angle) ? 0 : angle;
}

std::ostream& Maths::operator<<(std::ostream& os, const Vector2& v)
{
	return os << v.X() << " ; " << v.Y();
}

std::string Maths::operator+(const std::string& str, const Vector2& v)
{
	std::string result = str;
	result += "x = ";
	result += std::to_string(v.X());
	result += ", y = ";
	result += std::to_string(v.Y());
	return result;
}

std::string& Maths::operator+=(std::string& str, const Vector2& v)
{
	str += "x = ";
	str += std::to_string(v.X());
	str += ", y = ";
	str += std::to_string(v.Y());
	return str;
}

Vector2 Maths::operator*(float f, const Vector2& v)
{
	return Vector2(f * v.X(), f * v.Y());
}

Vector2 Maths::operator/(float f, const Vector2& v)
{
	assert(v.X() != 0.f);
	assert(v.Y() != 0.f);
	return Vector2(f / v.X(), f / v.Y());
}

Vector2 Maths::operator+(const Vector2& v1, const Vector2& v2)
{
	return v1.Add(v2);
}

Vector2 Maths::operator+(float f, const Vector2& v)
{
	return Vector2(f + v.x, f + v.y);
}

Vector2 Maths::operator-(const Vector2& v1, const Vector2& v2)
{
	return v1.Add(-v2);
}

Vector2 Maths::operator-(float f, const Vector2& v)
{
	return Vector2(f - v.x, f - v.y);
}
