#pragma once

#include "vector4.h"
#include <cassert>

namespace Maths
{
	class Matrix4
	{
	public:
		// Public member
		float m[16];

		// Constructors
		Matrix4();
		Matrix4(float _m[16]);
		Matrix4(Vector4& line1, Vector4& line2, Vector4& line3, Vector4& line4);
		// Destructor
		~Matrix4() = default;

		// Operators
		Matrix4 operator*(const Matrix4& m) const;
		Vector4 operator*(const Vector4& v) const;

		Matrix4& operator=(const Matrix4& copy);
		float& operator[](int index);
		float operator[](int index) const;

		// Getters
		float GetAt(unsigned int row, unsigned int col) const;
		// Setter
		void SetAt(unsigned int row, unsigned int col, float val);

		// Public methods
		Vector4 GetDiagonal() const;
		float GetTrace() const;
		float Det() const;
		Matrix4 Opposite() const;

		Matrix4 Transpose() const;
		Matrix4& Transpose();
		Matrix4 Add(const Matrix4& m2) const;
		Matrix4& Add(const Matrix4& m2);
		Matrix4 Scale(float f) const;
		Matrix4& Scale(float f);
		Matrix4 Multiply(const Matrix4& m2) const;
		Matrix4& Multiply(const Matrix4& m2);
		Vector4 Multiply(const Vector4& v2) const;
		Matrix4 GaussJordan() const;
		Matrix4& GaussJordan();
		Matrix4& Inverse();
		Matrix4 Inverse() const;
		Matrix4 GetInverse() const;

		// Static methods
		static Matrix4 Identity();
		static Matrix4 CreateTransformMatrix(const Vector3& rotation, const Vector3& position, const Vector3& scale);
		static Matrix4 CreateTranslationMatrix(const Vector3& translation);
		static Matrix4 CreateScaleMatrix(const Vector3& scale);
		static Matrix4 CreateXRotationMatrix(float angle);
		static Matrix4 CreateYRotationMatrix(float angle);
		static Matrix4 CreateZRotationMatrix(float angle);
		static Vector3 RotatePointAroundThreeAxis(Vector3& anglesRadPerAxis, const Vector3& p, const Vector3 anchor = Vector3(0.f, 0.f, 0.f));
		static Vector4 RotatePointAroundThreeAxis(Vector3& anglesRadPerAxis, const Vector4& p, const Vector3 anchor = Vector3(0.f, 0.f, 0.f));
		static Matrix4 GetPerspectiveMatrix(unsigned int width, unsigned int height, float near, float far, float fov);
	};

	std::ostream& operator<<(std::ostream& os, const Matrix4& m);
}
