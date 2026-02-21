#pragma once
#include "vector3.h"

class Color
{
public:
	// Public members

	float r = 0.f;
	float g = 0.f;
	float b = 0.f;
	float a = 1.f;

	// Static members

	const static Color black;
	const static Color white;
	const static Color red;
	const static Color green;
	const static Color blue;


	// Constructors

	Color();
	Color(float _r = 0.f, float _g = 0.f, float _b = 0.f, float _a = 1.f);
	Color(float _f, float _a);
	Color(const Color& _copy);


	// Destructor

	~Color() = default;


	// Operators

	Color& operator=(const Color& _copy);
	Color operator+(float _f) const;
	Color& operator+=(const Color& _c);
	Color operator+(const Color& _c) const;
	Color operator*(const Color& _c) const;
	Color& operator*=(const Color& _c);
	Color operator-() const;
	Color operator*(float _f) const;
	Color& operator*=(float _f);
	Color operator/(float _f) const;
	Color& operator/=(float f);
	bool operator==(const Color& _c) const;
	bool operator!=(const Color& _c) const;
	Maths::Vector3 ToByteVector3() const { return Maths::Vector3(fmin(255.f, fmax(0.f, r * 255.f)), fmin(255.f, fmax(0.f, g * 255.f)), fmin(255.f, fmax(0.f, b * 255.f))); }
};