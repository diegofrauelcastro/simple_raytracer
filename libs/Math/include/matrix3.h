#pragma once

#include "vector3.h"
#include "vector2.h"
#include <vector>
#include <cassert>

namespace Maths
{
	class Matrix3
	{
	public:
		// Public member
		float m[9];

		// Constructors
		Matrix3(float _m[9]);
		Matrix3(float f = 0.f);
		Matrix3(const Matrix3& copy);
		Matrix3(const Vector3& line1, const Vector3& line2, const Vector3& line3);
		// Destructor
		~Matrix3() = default;

		// Operators
		Matrix3 operator*(const Matrix3& m) const;
		Vector3 operator*(const Vector3& v) const;

		Matrix3& operator=(const Matrix3& copy);
		bool operator==(const Matrix3& other) const;
		float& operator[](unsigned int index);
		float operator[](unsigned int index) const;

		// Getters
		float GetAt(unsigned int row, unsigned int col) const;
		// Setter
		void SetAt(unsigned int row, unsigned int col, float val);

		// Public methods
		Vector3 GetDiagonal() const;
		float GetTrace() const;
		float Det() const;
		Matrix3 Opposite() const;

		Matrix3 Transpose() const;
		Matrix3& Transpose();
		Matrix3 Add(const Matrix3& m2) const;
		Matrix3& Add(const Matrix3& m2);
		Matrix3 Scale(float f) const;
		Matrix3& Scale(float f);
		Matrix3 Multiply(const Matrix3& m2) const;
		Matrix3& Multiply(const Matrix3& m2);
		Vector3 Multiply(const Vector3& v2) const;
		Matrix3 GaussJordan() const;
		Matrix3& GaussJordan();

		static Matrix3 Identity();
		static Matrix3 CreateTranslationMatrix(const Vector2& translation);
		static Matrix3 CreateScaleMatrix(const Vector2& scale);
		static Matrix3 CreateXRotationMatrix(float angle);
		static Matrix3 CreateYRotationMatrix(float angle);
		static Matrix3 CreateZRotationMatrix(float angle);
	};

	std::ostream& operator<<(std::ostream& os, const Matrix3& m);
}
