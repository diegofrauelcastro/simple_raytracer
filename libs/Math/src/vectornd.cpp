#include "vectornd.h"
using namespace Maths;

VectorND::VectorND(unsigned int _numDimensions, float f)
	: numDimension(_numDimensions)
	, values()
{
	values = std::vector<float>(numDimension, f);
}

VectorND::VectorND(const VectorND& copy)
	: numDimension(copy.numDimension)
	, values(copy.values)
{
}

VectorND& VectorND::operator=(const VectorND& copy)
{
	numDimension = copy.numDimension;
	values = copy.values;
	return *this;
}

float& VectorND::operator[](unsigned int index)
{
	assert(index >= 0 && index < numDimension);
	return values[index];
}

float VectorND::operator[](unsigned int index) const
{
	assert(index >= 0 && index < numDimension);
	return values[index];
}

VectorND VectorND::operator+(const float f) const
{
	VectorND res(*this);
	for (float val : res.values)
		val += f;
	return res;
}

VectorND& VectorND::operator+=(const VectorND& v)
{
	Add(v);
	return *this;
}

VectorND& VectorND::operator-=(const VectorND& v)
{
	Add(-v);
	return *this;
}

VectorND VectorND::operator-() const
{
	VectorND res(*this);
	for (float val : res.values)
		val = -val;
	return res;
}

VectorND VectorND::operator*(float f) const
{
	return Scale(f);
}

VectorND& VectorND::operator*=(float f)
{
	Scale(f);
	return *this;
}

VectorND VectorND::operator/(float f) const
{
	assert(fabs(f) > 0.000001f);
	VectorND res(*this);
	for (float val : res.values)
		val /= f;
	return res;
}

VectorND& VectorND::operator/=(float f)
{
	assert(fabs(f) > 0.000001f);
	for (float val : values)
		val /= f;
	return *this;
}

void VectorND::operator++(int i)
{
	(void)i;
	float length = GetMagnitude();
	assert(fabs(length) > 0.000001f);
	for (float val : values)
		val *= (length + 1) / length;
}

void VectorND::operator--(int i)
{
	(void)i;
	float length = GetMagnitude();
	assert(fabs(length) > 0.000001f);
	for (float val : values)
		val *= (length - 1) / length;
}

float VectorND::operator*(const VectorND& v) const
{
	return DotProduct(v);
}

bool VectorND::operator==(const VectorND& v) const
{
	return GetMagnitude() == v.GetMagnitude();
}

bool VectorND::operator!=(const VectorND& v) const
{
	return GetMagnitude() != v.GetMagnitude();
}

bool VectorND::operator<(const VectorND& v) const
{
	return GetMagnitude() < v.GetMagnitude();
}

bool VectorND::operator<=(const VectorND& v) const
{
	return GetMagnitude() <= v.GetMagnitude();
}

bool VectorND::operator>(const VectorND& v) const
{
	return GetMagnitude() > v.GetMagnitude();
}

bool VectorND::operator>=(const VectorND& v) const
{
	return GetMagnitude() >= v.GetMagnitude();
}

float VectorND::DotProduct(const VectorND& v) const
{
	float res = 0;
	assert(v.numDimension == numDimension);
	for (unsigned int i = 0; i < numDimension; i++)
		res += values[i]*v[i];
	return res;
}

VectorND VectorND::ElementWiseProduct(const VectorND& v) const
{
	VectorND res = v;
	assert(v.numDimension == numDimension);
	for (unsigned int i = 0; i < numDimension; i++)
		res[i] *= values[i];
	return res;
}

float VectorND::GetMagnitude() const
{
	return sqrt(SquaredNorm());
}

float VectorND::SquaredNorm() const
{
	float res = 0;
	for (float val : values)
		res += val * val;
	return res;
}

VectorND VectorND::Add(const VectorND& v) const
{
	VectorND res = *this;
	assert(v.numDimension == numDimension);
	for (unsigned int i = 0; i < numDimension; i++)
		res[i] += v[i];
	return res;
}

VectorND& VectorND::Add(const VectorND& v)
{
	assert(v.numDimension == numDimension);
	for (unsigned int i = 0; i < numDimension; i++)
		values[i] += v[i];
	return *this;
}

VectorND VectorND::Scale(float f) const
{
	VectorND res = *this;
	for (unsigned int i = 0; i < numDimension; i++)
		res[i] *= f;
	return res;
}

VectorND& VectorND::Scale(float f)
{
	for (unsigned int i = 0; i < numDimension; i++)
		values[i] *= f;
	return *this;
}

VectorND VectorND::Opposite() const
{
	return -(*this);
}

VectorND VectorND::Normalize() const
{
	VectorND res = *this;
	float length = GetMagnitude();
	assert(fabs(length) > 0.000001f);
	for (float val : res.values)
		val /= length;
	return res;
}

VectorND& VectorND::Normalize()
{
	float length = GetMagnitude();
	assert(fabs(length) > 0.000001f);
	for (float val : values)
		val *= (length - 1) / length;
	return *this;
}

VectorND VectorND::MidPoint(const VectorND& p1, const VectorND& p2)
{
	return (p1 + p2) / 2;
}

float VectorND::Distance(const VectorND& p1, const VectorND& p2)
{
	return (p2 - p1).GetMagnitude();
}

float VectorND::GetAt(unsigned int index) const
{
	return values[index];
}

unsigned int VectorND::GetDimension() const
{
	return numDimension;
}

void VectorND::SetAt(unsigned int index, float value)
{
	values[index] = value;
}

std::ostream& Maths::operator<<(std::ostream& os, const VectorND& v)
{
	std::stringstream res;
	for (unsigned int i = 0; i < v.GetDimension(); i++)
	{
		res << v[i];
		if (i < v.GetDimension() - 1)
			res << " ; ";
	}
	return os << res.str();
}

VectorND Maths::operator*(float f, const VectorND& v)
{
	VectorND res = v;
	for (unsigned int i = 0; i < res.GetDimension(); i++)
		res[i] = res[i] * f;
	return res;
}

VectorND Maths::operator/(float f, const VectorND& v)
{
	VectorND res = v;
	for (unsigned int i = 0; i < res.GetDimension(); i++)
	{
		assert(fabs(res[i]) > 0.000001f);
		res[i] = f / res[i];
	}
	return res;
}

VectorND Maths::operator+(const VectorND& v1, const VectorND& v2)
{
	return v1.Add(v2);
}

VectorND Maths::operator+(float f, const VectorND& v)
{
	VectorND res = v;
	for (unsigned int i = 0; i < res.GetDimension(); i++)
		res[i] = f + res[i];
	return res;
}

VectorND Maths::operator-(const VectorND& v1, const VectorND& v2)
{
	return v1.Add(-v2);
}

VectorND Maths::operator-(float f, const VectorND& v)
{
	VectorND res = v;
	for (unsigned int i = 0; i < res.GetDimension(); i++)
		res[i] = f - res[i];
	return res;
}
