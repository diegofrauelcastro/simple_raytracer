#pragma once

#include "vector3.h"

namespace Maths
{
	class Vector4
	{
	public:
		// Public members
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float w = 0.f;

		// Static members
		const static Vector4 zero;

		// Constructors
		Vector4();
		Vector4(float _x, float _y, float _z, float _w);
		Vector4(const Vector3& vec3, float _w = 1.0f);
		Vector4(float f);
		Vector4(const Vector4& copy);
		// Destructor
		~Vector4() = default;

		// Operators
		operator Vector3();
		Vector4& operator=(const Vector4& copy);
		float& operator[](int index);
		float& operator[](char coord);
		float& operator[](const std::string& coord);
		float operator[](int index) const;
		float operator[](char coord) const;
		float operator[](const std::string& coord) const;

		Vector4 operator+(float f) const;
		Vector4& operator+=(const Vector4& v);
		Vector4& operator-=(const Vector4& v);
		Vector4 operator-() const;
		Vector4 operator*(float f) const;
		Vector4& operator*=(float f);
		Vector4 operator/(float f) const;
		Vector4& operator/=(float f);
		void operator++(int i);
		void operator--(int i);
		float operator*(const Vector4& v) const;

		bool operator==(const Vector4& v) const;
		bool operator!=(const Vector4& v) const;
		bool operator<(const Vector4& v) const;
		bool operator<=(const Vector4& v) const;
		bool operator>(const Vector4& v) const;
		bool operator>=(const Vector4& v) const;

		// Getters 
		float X() const;
		float Y() const;
		float Z() const;
		float W() const;

		// Setters 
		float& X();
		float& Y();
		float& Z();
		float& W();

		// Public methods
		float DotProduct(const Vector4& v) const;
		Vector4 Opposite() const;
		Vector4 ElementWiseProduct(const Vector4& v) const;
		float GetMagnitude() const;
		float Norm() const;
		float SquaredNorm(bool homogenize = false) const;

		Vector4 Add(const Vector4& v) const;
		Vector4& Add(const Vector4& v);
		Vector4 Scale(float f) const;
		Vector4& Scale(float f);
		Vector4 Normalize() const;
		Vector4& Normalize();
		void Homogenize();

		// Static methods
		static Vector4 MidPoint(const Vector4& p1, const Vector4& p2);
		static float Distance(const Vector4& p1, const Vector4& p2);
	};

	std::ostream& operator<<(std::ostream& os, const Vector4& v);
	std::string operator+(const std::string& str, const Vector4& v);
	std::string& operator+=(std::string& str, const Vector4& v);

	Vector4 operator*(float f, const Vector4& v);
	Vector4 operator/(float f, const Vector4& v);
	Vector4 operator+(const Vector4& v1, const Vector4& v2);
	Vector4 operator+(float f, const Vector4& v);
	Vector4 operator-(const Vector4& v1, const Vector4& v2);
	Vector4 operator-(float f, const Vector4& v);
}
