#pragma once

#include <vector>
#include <cassert>
#include <math.h>
#include <string>
#include <ostream>
#include <sstream>

namespace Maths
{
	class VectorND
	{
	public:
		// Constructors
		VectorND() = delete;
		VectorND(unsigned int _numDimensions, float f = 0.f);
		VectorND(const VectorND& copy);
		// Destructor
		~VectorND() = default;

		// Operators
		VectorND& operator=(const VectorND& copy);
		float& operator[](unsigned int index);
		float operator[](unsigned int index) const;

		VectorND operator+(float f) const;
		VectorND& operator+=(const VectorND& v);
		VectorND& operator-=(const VectorND& v);
		VectorND operator-() const;
		VectorND operator*(float f) const;
		VectorND& operator*=(float f);
		VectorND operator/(float f) const;
		VectorND& operator/=(float f);
		void operator++(int i);
		void operator--(int i);
		float operator*(const VectorND& v) const;

		bool operator==(const VectorND& v) const;
		bool operator!=(const VectorND& v) const;
		bool operator<(const VectorND& v) const;
		bool operator<=(const VectorND& v) const;
		bool operator>(const VectorND& v) const;
		bool operator>=(const VectorND& v) const;

		// Getters
		float GetAt(unsigned int index) const;
		unsigned int GetDimension() const;

		// Setter
		void SetAt(unsigned int index, float value);

		// Public methods
		float DotProduct(const VectorND& v) const;
		VectorND Opposite() const;
		VectorND ElementWiseProduct(const VectorND& v) const;
		float GetMagnitude() const;
		float SquaredNorm() const;

		VectorND Add(const VectorND& v) const;
		VectorND& Add(const VectorND& v);
		VectorND Scale(float f) const;
		VectorND& Scale(float f);
		VectorND Normalize() const;
		VectorND& Normalize();

		// Static methods
		static VectorND MidPoint(const VectorND& p1, const VectorND& p2);
		static float Distance(const VectorND& p1, const VectorND& p2);
	private:
		// private members
		std::vector<float> values;
		unsigned int numDimension;
	};

	std::ostream& operator<<(std::ostream& os, const VectorND& v);

	VectorND operator*(float f, const VectorND& v);
	VectorND operator/(float f, const VectorND& v);
	VectorND operator+(const VectorND& v1, const VectorND& v2);
	VectorND operator+(float f, const VectorND& v);
	VectorND operator-(const VectorND& v1, const VectorND& v2);
	VectorND operator-(float f, const VectorND& v);
}
