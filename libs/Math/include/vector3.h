#pragma once

#include <iostream>
#include <math.h>
#include <string>
#include <cassert>

#define PI 3.141592653589793f

namespace Maths
{
	class Vector3
	{
	public:
		// Public members
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		// Static members
		const static Vector3 zero;
		const static Vector3 one;
		const static Vector3 right;
		const static Vector3 left;
		const static Vector3 up;
		const static Vector3 down;
		const static Vector3 forward;
		const static Vector3 back;

		// Constructors
		Vector3();
		Vector3(float _x, float _y, float _z);
		Vector3(float f);
		Vector3(const Vector3& copy);
		// Destructor
		~Vector3() = default;

		// Operators
		Vector3& operator=(const Vector3& copy);
		float& operator[](int index);
		float& operator[](char coord);
		float& operator[](const std::string& coord);
		float operator[](int index) const;
		float operator[](char coord) const;
		float operator[](const std::string& coord) const;

		Vector3 operator+(float f) const;
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3 operator-() const;
		Vector3 operator*(float f) const;
		Vector3& operator*=(float f);
		Vector3 operator/(float f) const;
		Vector3& operator/=(float f);
		void operator++(int i);
		void operator--(int i);
		float operator*(const Vector3& v) const;

		bool operator==(const Vector3& v) const;
		bool operator!=(const Vector3& v) const;
		bool operator<(const Vector3& v) const;
		bool operator<=(const Vector3& v) const;
		bool operator>(const Vector3& v) const;
		bool operator>=(const Vector3& v) const;

		// Getters 
		float X() const;
		float Y() const;
		float Z() const;

		// Setters 
		float& X();
		float& Y();
		float& Z();

		// Public methods
		float DotProduct(const Vector3& v) const;
		Vector3 CrossProduct(const Vector3& v) const;
		Vector3 Opposite() const;
		Vector3 ElementWiseProduct(const Vector3& v) const;
		float GetMagnitude() const;
		float Norm() const;
		float SquaredNorm() const;

		Vector3 Add(const Vector3& v) const;
		Vector3& Add(const Vector3& v);
		Vector3 Scale(float f) const;
		Vector3& Scale(float f);
		Vector3 Normalize() const;
		Vector3& Normalize();

		// Static methods
		static Vector3 MidPoint(const Vector3& p1, const Vector3& p2);
		static float Distance(const Vector3& p1, const Vector3& p2);
		static float AngleBetweenTwoVector3(const Vector3& v1, const Vector3& v2);
		static Vector3 GenerateRandomUnitVector3();
	};

	std::ostream& operator<<(std::ostream& os, const Vector3& v);
	std::string operator+(const std::string& str, const Vector3& v);
	std::string& operator+=(std::string& str, const Vector3& v);

	Vector3 operator*(float f, const Vector3& v);
	Vector3 operator/(float f, const Vector3& v);
	Vector3 operator+(const Vector3& v1, const Vector3& v2);
	Vector3 operator+(float f, const Vector3& v);
	Vector3 operator-(const Vector3& v1, const Vector3& v2);
	Vector3 operator-(float f, const Vector3& v);
}
