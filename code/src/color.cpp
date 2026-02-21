#include "color.h"
#include <corecrt_math.h>


// Static
const Color Color::black = Color(0.f, 0.f, 0.f);
const Color Color::white = Color(1.f, 1.f, 1.f);
const Color Color::red = Color(1.f, 0.f, 0.f);
const Color Color::green = Color(0.f, 1.f, 0.f);
const Color Color::blue = Color(0.f, 0.f, 1.f);

Color::Color()
	: r(0.f), g(0.f), b(0.f), a(1.f)
{
}

Color::Color(float _r, float _g, float _b, float _a)
	: r(_r), g(_g), b(_b)
{
}

Color::Color(float _f, float _a)
	: r(_f), g(_f), b(_f), a(_a)
{
}

Color::Color(const Color& _copy)
	: r(_copy.r), g(_copy.g), b(_copy.b), a(_copy.a)
{
}

Color& Color::operator=(const Color& _copy)
{
	r = _copy.r;
	g = _copy.g;
	b = _copy.b;
	return *this;
}

Color Color::operator+(float _f) const
{
	Color newColor = Color(r + _f, g + _f, b + _f, 1.f);
	if (newColor.r > 1.f || newColor.g > 1.f || newColor.b > 1.f)
	{
		newColor.r = fmin(1.f, newColor.r); newColor.r = fmax(0.f, newColor.r);
		newColor.g = fmin(1.f, newColor.g); newColor.g = fmax(0.f, newColor.g);
		newColor.b = fmin(1.f, newColor.b); newColor.b = fmax(0.f, newColor.b);
	}
	return newColor;
}

Color& Color::operator+=(const Color& _c)
{
	r += _c.r;
	g += _c.g;
	b += _c.b;
	a += _c.a;
	if (r > 1.f || g > 1.f || b > 1.f)
	{
		r = fmin(1.f, r); r = fmax(0.f, r);
		g = fmin(1.f, g); g = fmax(0.f, g);
		b = fmin(1.f, b); b = fmax(0.f, b);
	}
	return *this;
}

Color Color::operator+(const Color& _c) const
{
	Color newColor = Color(r + _c.r, g + _c.g, b + _c.b, a + _c.a);
	if (newColor.r > 1.f || newColor.g > 1.f || newColor.b > 1.f)
	{
		newColor.r = fmin(1.f, newColor.r); newColor.r = fmax(0.f, newColor.r);
		newColor.g = fmin(1.f, newColor.g); newColor.g = fmax(0.f, newColor.g);
		newColor.b = fmin(1.f, newColor.b); newColor.b = fmax(0.f, newColor.b);
	}
	return newColor;
}

Color Color::operator*(const Color& _c) const
{
	Color newColor = Color(r * _c.r, g * _c.g, b * _c.b, a * _c.a);
	if (newColor.r > 1.f || newColor.g > 1.f || newColor.b > 1.f)
	{
		newColor.r = fmin(1.f, newColor.r); newColor.r = fmax(0.f, newColor.r);
		newColor.g = fmin(1.f, newColor.g); newColor.g = fmax(0.f, newColor.g);
		newColor.b = fmin(1.f, newColor.b); newColor.b = fmax(0.f, newColor.b);
	}
	return newColor;
}

Color& Color::operator*=(const Color& _c)
{
	r *= _c.r;
	g *= _c.g;
	b *= _c.b;
	if (r > 1.f || g > 1.f || b > 1.f)
	{
		r = fmin(1.f, r); r = fmax(0.f, r);
		g = fmin(1.f, g); g = fmax(0.f, g);
		b = fmin(1.f, b); b = fmax(0.f, b);
	}
	return *this;
}

Color Color::operator-() const
{
	return Color(1.f-r, 1.f-g, 1.f-b);
}

Color Color::operator*(float _f) const
{
	_f = fmin(_f, 1.f);
	return Color(r * _f, g * _f, b * _f);
}

Color& Color::operator*=(float _f)
{
	_f = fmin(_f, 1.f);
	r *= _f;
	g *= _f;
	b *= _f;
	return *this;
}

Color Color::operator/(float _f) const
{
	if (_f == 0.f)
		return Color(1.f, 1.f, 1.f);

	_f = fmin(_f, 1.f);
	return Color(r / _f, g / _f, b / _f);
}

Color& Color::operator/=(float _f)
{
	if (_f == 0.f)
	{
		r = 1.f;
		g = 1.f;
		b = 1.f;
		return *this;
	}

	_f = fmin(_f, 1.f);
	r /= _f;
	g /= _f;
	b /= _f;
	return *this;
}

bool Color::operator==(const Color& _c) const
{
	return r == _c.r && g == _c.g && b == _c.b && a == _c.a;
}

bool Color::operator!=(const Color& _c) const
{
	return !(*this == _c);
}
