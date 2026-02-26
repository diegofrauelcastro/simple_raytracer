#pragma once

#include "vectornd.h"
#include "vector2.h"
#include "matrix3.h"
#include "matrix4.h"
#include <vector>

namespace Maths
{
	class MatrixMxN
	{
	public:
		// Public member
		std::vector<float> matrix;
		unsigned int m;
		unsigned int n;

		// Constructors
		MatrixMxN() = delete;
		MatrixMxN(unsigned int _m, unsigned int _n, float f = 0.f);
		MatrixMxN(Vector2 dimensions, float f = 0.f);
		MatrixMxN(const MatrixMxN& copy);
		MatrixMxN(const Maths::Matrix3& m3);
		MatrixMxN(const Maths::Matrix4& m4);
		// Destructor
		~MatrixMxN() = default;

		// Operators
		MatrixMxN operator*(const MatrixMxN& m) const;
		VectorND operator*(const VectorND& v) const;

		MatrixMxN& operator=(const MatrixMxN& copy);
		bool operator==(const MatrixMxN& other) const;
		float& operator[](unsigned int index);
		float operator[](unsigned int index) const;

		// Getters
		float GetAt(unsigned int row, unsigned int col) const;
		Vector2 GetDimensions() const;
		// Setter
		void SetAt(unsigned int row, unsigned int col, float val);

		// Public methods
		VectorND GetDiagonal() const;
		float GetTrace() const;
		MatrixMxN Opposite() const;

		MatrixMxN Transpose() const;
		MatrixMxN& Transpose();
		MatrixMxN Add(const MatrixMxN& m2) const;
		MatrixMxN& Add(const MatrixMxN& m2);
		MatrixMxN Scale(float f) const;
		MatrixMxN& Scale(float f);
		MatrixMxN Multiply(const MatrixMxN& m2) const;
		MatrixMxN& Multiply(const MatrixMxN& m2);
		VectorND Multiply(const VectorND& v2) const;
		MatrixMxN GaussJordan() const;
		MatrixMxN& GaussJordan();
		MatrixMxN& Augment(const MatrixMxN& other);
		MatrixMxN GetAugment(const MatrixMxN& other) const;
		MatrixMxN& Inverse();
		MatrixMxN GetInverse() const;

		static MatrixMxN Identity(unsigned int _m);
	};

	std::ostream& operator<<(std::ostream& os, const MatrixMxN& m);
}
