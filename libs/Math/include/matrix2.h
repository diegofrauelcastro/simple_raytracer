#pragma once

#include "vector2.h"
#include <vector>
#include <cassert>

#define PI 3.141592653589793f

namespace Maths
{
	class Matrix2
	{
	public:
		// Public member
		float m[4];

		// Constructors
		Matrix2(float _m[4]);
		Matrix2(float f = 0.f);
		Matrix2(const Matrix2& copy);
		Matrix2(const Vector2& line1, const Vector2& line2);
		// Destructor
		~Matrix2() = default;

		// Operators
		Matrix2 operator*(const Matrix2& m) const;
		Vector2 operator*(const Vector2& v) const;

		Matrix2& operator=(const Matrix2& copy);
		bool operator==(const Matrix2& other) const;
		float& operator[](unsigned int index);
		float operator[](unsigned int index) const;

		// Getters
		float GetAt(unsigned int row, unsigned int col) const;
		// Setter
		void SetAt(unsigned int row, unsigned int col, float val);

		// Public methods
		Vector2 GetDiagonal() const;
		float GetTrace() const;
		float Det() const;
		Matrix2 Opposite() const;

		Matrix2 Transpose() const;
		Matrix2& Transpose();
		Matrix2 Add(const Matrix2& m2) const;
		Matrix2& Add(const Matrix2& m2);
		Matrix2 Scale(float f) const;
		Matrix2& Scale(float f);
		Matrix2 Multiply(const Matrix2& m2) const;
		Matrix2& Multiply(const Matrix2& m2);
		Vector2 Multiply(const Vector2& v2) const;
		Matrix2 GaussJordan() const;
		Matrix2& GaussJordan();

		// Static methods
		static Matrix2 Identity();
		static Matrix2 CreateScaleMatrix(const Vector2& scale);
		static Matrix2 CreateRotationMatrix(float angle);
		static Vector2 RotatePointAroundAnchor(float angleRad, const Vector2 p, const Vector2 anchor = Vector2(0.f, 0.f));
	};

	std::ostream& operator<<(std::ostream& os, const Matrix2& m);
}
