#pragma once

#include <iostream>
#include <math.h>
#include <string>
#include <cassert>

#define PI 3.141592653589793f

namespace Maths
{
	class Vector2
	{
	public:
		// Public members
		float x = 0.f;
		float y = 0.f;

		// Static members
		const static Vector2 zero;
		const static Vector2 one;
		const static Vector2 up;
		const static Vector2 down;
		const static Vector2 left;
		const static Vector2 right;

		// Constructors
		Vector2();
		Vector2(float _x, float _y);
		Vector2(float f);
		Vector2(const Vector2& copy);
		// Destructor
		~Vector2() = default;

		// Operators
		Vector2& operator=(const Vector2& copy);
		float& operator[](int index);
		float& operator[](char coord);
		float& operator[](const std::string& coord);
		float operator[](int index) const;
		float operator[](char coord) const;
		float operator[](const std::string& coord) const;

		Vector2 operator+(float f) const;
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2 operator-() const;
		Vector2 operator*(float f) const;
		Vector2& operator*=(float f);
		Vector2 operator/(float f) const;
		Vector2& operator/=(float f);
		void operator++(int i);
		void operator--(int i);
		float operator*(const Vector2& v) const;

		bool operator==(const Vector2& v) const;
		bool operator!=(const Vector2& v) const;
		bool operator<(const Vector2& v) const;
		bool operator<=(const Vector2& v) const;
		bool operator>(const Vector2& v) const;
		bool operator>=(const Vector2& v) const;

		// Getters 
		float X() const;
		float Y() const;

		// Setters 
		float& X();
		float& Y();

		// Public methods
		float DotProduct(const Vector2& v) const;
		float CrossProduct(const Vector2& v) const;
		Vector2 Opposite() const;
		Vector2 ElementWiseProduct(const Vector2& v) const;
		float GetMagnitude() const;
		float Norm() const;
		float SquaredNorm() const;

		Vector2 Add(const Vector2& v) const;
		Vector2& Add(const Vector2& v);
		Vector2 Scale(float f) const;
		Vector2& Scale(float f);
		Vector2 Normalize() const;
		Vector2& Normalize();
		Vector2 Rotate(float radAngle, const Vector2& origin = Vector2::zero) const;
		Vector2& Rotate(float radAngle, const Vector2& origin = Vector2::zero);

		// Static methods
		static Vector2 MidPoint(const Vector2& p1, const Vector2& p2);
		static float Distance(const Vector2& p1, const Vector2& p2);
		static float AngleRadBetweenTwoVector2(const Vector2& v1, const Vector2& v2);
		static float AngleBetweenTwoVector2(const Vector2& v1, const Vector2& v2);
	};


	std::ostream& operator<<(std::ostream& os, const Vector2& v);
	std::string operator+(const std::string& str, const Vector2& v);
	std::string& operator+=(std::string& str, const Vector2& v);

	Vector2 operator*(float f, const Vector2& v);
	Vector2 operator/(float f, const Vector2& v);
	Vector2 operator+(const Vector2& v1, const Vector2& v2);
	Vector2 operator+(float f, const Vector2& v);
	Vector2 operator-(const Vector2& v1, const Vector2& v2);
	Vector2 operator-(float f, const Vector2& v);
}
