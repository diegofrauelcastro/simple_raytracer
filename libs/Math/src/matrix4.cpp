#include "matrix4.h"
using namespace Maths;

Matrix4::Matrix4()
{
	for (int i = 0; i < 16; i++)
		m[i] = 0;
}

Matrix4::Matrix4(float _m[16])
{
	for (int i = 0; i < 16; i++)
		m[i] = _m[i];
}

Matrix4::Matrix4(Vector4& line1, Vector4& line2, Vector4& line3, Vector4& line4)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Vector4& currentV = line4;
			if (i == 0)
				currentV = line1;
			else if (i == 1)
				currentV = line2;
			else if (i == 2)
				currentV = line3;
			m[4 * i + j] = currentV[j];
		}
	}
}

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
	return Multiply(m);
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
	return Multiply(v);
}

Matrix4& Matrix4::operator=(const Matrix4& copy)
{
	for (int i = 0; i < 16; i++)
		m[i] = copy[i];
	return *this;
}

float& Matrix4::operator[](int index)
{
	assert(0 <= index && index < 16);
	return m[index];
}

float Matrix4::operator[](int index) const
{
	assert(0 <= index && index < 16);
	return m[index];
}

float Matrix4::GetAt(unsigned int row, unsigned int col) const
{
	assert(row < 4 && col < 4);
	return m[row * 4 + col];
}

void Matrix4::SetAt(unsigned int row, unsigned int col, float val)
{
	assert(row < 4 && col < 4);
	m[row * 4 + col] = val;
}

Vector4 Matrix4::GetDiagonal() const
{
	return Vector4(m[0], m[5], m[10], m[15]);
}

float Matrix4::GetTrace() const
{
	Vector4 diag = GetDiagonal();
	return diag.x + diag.y + diag.z + diag.w;
}

Matrix4 Matrix4::Opposite() const
{
	Matrix4 tempM = *this;
	for (int i = 0; i < 16; i++)
		tempM[i] = -tempM[i];
	return tempM;
}

float Matrix4::Det() const
{
	float line1 = m[3]*m[6]*m[9]*m[12] + m[0]*m[5]*m[10]*m[15] - m[0]*m[5]*m[11]*m[14] - m[0]*m[6]*m[9]*m[15];
	float line2 = m[0]*m[6]*m[11]*m[13] + m[0]*m[7]*m[9]*m[14] - m[0]*m[7]*m[10]*m[13] - m[1]*m[4]*m[10]*m[15];
	float line3 = m[1]*m[4]*m[11]*m[14] + m[1]*m[6]*m[8]*m[15] - m[1]*m[6]*m[11]*m[12] - m[1]*m[7]*m[8]*m[14];
	float line4 = m[1]*m[7]*m[10]*m[12] + m[2]*m[4]*m[9]*m[15] - m[2]*m[4]*m[11]*m[13] - m[2]*m[5]*m[8]*m[15];
	float line5 = m[2]*m[5]*m[11]*m[12] + m[2]*m[7]*m[8]*m[13] - m[2]*m[7]*m[9]*m[12] - m[3]*m[4]*m[9]*m[14];
	float line6 = m[3]*m[4]*m[10]*m[13] + m[3]*m[5]*m[8]*m[14] - m[3]*m[5]*m[10]*m[12] - m[3]*m[6]*m[8]*m[13];
	return line1 + line2 + line3 + line4 + line5 + line6;
}

Matrix4 Matrix4::Transpose() const
{
	Matrix4 res = Matrix4();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; i++)
		{
			res[4*i + j] = m[4*j + i];
		}
	}
	return res;
}

Matrix4& Matrix4::Transpose()
{
	Matrix4 tempM = Matrix4();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			tempM[4 * i + j] = m[4 * j + i];
	}
	(*this) = tempM;
	return *this;
}

Matrix4 Matrix4::Add(const Matrix4& m2) const
{
	Matrix4 res = m2;
	for (int i = 0; i < 16; i++)
		res[i] += m[i];
	return res;
}

Matrix4& Matrix4::Add(const Matrix4& m2)
{
	for (int i = 0; i < 16; i++)
		m[i] += m2[i];
	return *this;
}

Matrix4 Matrix4::Scale(float f) const
{
	Matrix4 res = *this;
	for (int i = 0; i < 16; i++)
		res[i] *= f;
	return res;
}

Matrix4& Matrix4::Scale(float f)
{
	for (int i = 0; i < 16; i++)
		m[i] *= f;
	return *this;
}

Matrix4 Matrix4::GaussJordan() const
{
	Matrix4 copy = *this;
	return copy.GaussJordan();
}

Matrix4& Matrix4::GaussJordan()
{
	int r = -1;
	// j is the column
	for (int j = 0; j < 4; j++)
	{
		// We look for the index k (the row) of the maximum num in this column. i is the current row.
		int k = 4 * (r+1) + j;
		for (int i = r+1; i < 4; i++)
		{
			if (fabs(m[4*i + j]) > fabs(m[k]))
				k = 4*i + j;
		}
		k = k / 4;

		// We operate based on the pivot m[k,j] <=> m[4k + j] here
		if (m[4 * k + j] != 0)
		{
			r++;
			// Normalization of the row by the pivot
			float pivot = m[4 * k + j];
			for (int tempCol = 0; tempCol < 4; tempCol++)
				m[4 * k + tempCol] /= pivot;

			// Swapping k row and r row if they are not the same number
			if (k != r)
			{
				// Storing k row
				float temp[4] = { m[4 * k], m[4 * k + 1] , m[4 * k + 2] , m[4 * k + 3] };
				// Storing r row into k row
				m[4*k] = m[4*r]; m[4*k+1] = m[4*r+1]; m[4*k+2] = m[4*r+2]; m[4*k+3] = m[4*r+3];
				// Inject temporary k row into r row
				m[4*r] = temp[0]; m[4*r+1] = temp[1]; m[4*r+2] = temp[2]; m[4*r+3] = temp[3];
			}
			for (int i = 0; i < 4; i++)
			{
				// Subtract (row r * m[4*i + j]) from row i
				if (i != r)
				{
					float factor = m[4 * i + j];
					for (int tempCol = 0; tempCol < 4; tempCol++)
						m[4*i+tempCol] -= m[4*r+tempCol] * factor;
				}
			}
		}
	}
	return *this;
}

// https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix - Answer by @willnode (Jun 8, 2017)
Matrix4& Matrix4::Inverse()
{
	float A2323 = m[10] * m[15] - m[11] * m[14];
	float A1323 = m[9] * m[15] - m[11] * m[13];
	float A1223 = m[9] * m[14] - m[10] * m[13];
	float A0323 = m[8] * m[15] - m[11] * m[12];
	float A0223 = m[8] * m[14] - m[10] * m[12];
	float A0123 = m[8] * m[13] - m[9] * m[12];
	float A2313 = m[6] * m[15] - m[7] * m[14];
	float A1313 = m[5] * m[15] - m[7] * m[13];
	float A1213 = m[5] * m[14] - m[6] * m[13];
	float A2312 = m[6] * m[11] - m[7] * m[10];
	float A1312 = m[5] * m[11] - m[7] * m[9];
	float A1212 = m[5] * m[10] - m[6] * m[9];
	float A0313 = m[4] * m[15] - m[7] * m[12];
	float A0213 = m[4] * m[14] - m[6] * m[12];
	float A0312 = m[4] * m[11] - m[7] * m[8];
	float A0212 = m[4] * m[10] - m[6] * m[8];
	float A0113 = m[4] * m[13] - m[5] * m[12];
	float A0112 = m[4] * m[9] - m[5] * m[8];

	float det = m[0] * (m[5] * A2323 - m[6] * A1323 + m[7] * A1223)
		- m[1] * (m[4] * A2323 - m[6] * A0323 + m[7] * A0223)
		+ m[2] * (m[4] * A1323 - m[5] * A0323 + m[7] * A0123)
		- m[3] * (m[4] * A1223 - m[5] * A0223 + m[6] * A0123);
	det = 1 / det;

	Matrix4 res = *this;
	res[0] = det *   (m[5] * A2323 - m[6] * A1323 + m[7] * A1223);
	res[1] = det *  -(m[1] * A2323 - m[2] * A1323 + m[3] * A1223);
	res[2] = det *   (m[1] * A2313 - m[2] * A1313 + m[3] * A1213);
	res[3] = det *  -(m[1] * A2312 - m[2] * A1312 + m[3] * A1212);
	res[4] = det *  -(m[4] * A2323 - m[6] * A0323 + m[7] * A0223);
	res[5] = det *   (m[0] * A2323 - m[2] * A0323 + m[3] * A0223);
	res[6] = det *  -(m[0] * A2313 - m[2] * A0313 + m[3] * A0213);
	res[7] = det *   (m[0] * A2312 - m[2] * A0312 + m[3] * A0212);
	res[8] = det *   (m[4] * A1323 - m[5] * A0323 + m[7] * A0123);
	res[9] = det *  -(m[0] * A1323 - m[1] * A0323 + m[3] * A0123);
	res[10] = det *  (m[0] * A1313 - m[1] * A0313 + m[3] * A0113);
	res[11] = det * -(m[0] * A1312 - m[1] * A0312 + m[3] * A0112);
	res[12] = det * -(m[4] * A1223 - m[5] * A0223 + m[6] * A0123);
	res[13] = det *  (m[0] * A1223 - m[1] * A0223 + m[2] * A0123);
	res[14] = det * -(m[0] * A1213 - m[1] * A0213 + m[2] * A0113);
	res[15] = det *  (m[0] * A1212 - m[1] * A0212 + m[2] * A0112);
	for (int i = 0; i < 16; i++)
		m[i] = res[i];
	return *this;
}

Matrix4 Matrix4::Inverse() const
{
	return (*this).GetInverse();
}

Matrix4 Matrix4::GetInverse() const
{
	Matrix4 copy = *this;
	return copy.Inverse();
}

Matrix4 Matrix4::Multiply(const Matrix4& m2) const
{
	Matrix4 res = *this;
	return res.Multiply(m2);
}

Matrix4& Matrix4::Multiply(const Matrix4& m2)
{
	Matrix4 mRes;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			mRes[4*i + j] = m[4*i]*m2.m[j] + m[4*i+1]*m2.m[4+j] + m[4*i+2]*m2.m[8+j] + m[4*i+3]*m2.m[12+j];
	}
	(*this) = mRes;
	return *this;
}


Vector4 Matrix4::Multiply(const Vector4& v2) const
{
	Vector4 vRes;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			vRes[i] += v2[j] * m[4 * i + j];
	}
	return vRes;
}

std::ostream& Maths::operator<<(std::ostream& os, const Matrix4& m)
{
	for (int i = 0; i < 16; i++)
	{
		if (i % 4 == 0)
			os << "\n";
		os << m.m[i] << " ";
	}
	return os;
}

Matrix4 Matrix4::Identity()
{
	Matrix4 id;
	for (int i = 0; i < 4; i++)
		id[4 * i + i] = 1;
	return id;
}


Matrix4 Matrix4::CreateTransformMatrix(const Vector3& rotation, const Vector3& position, const Vector3& scale)
{
	Matrix4 mTranslate = CreateTranslationMatrix(position);
	Matrix4 mRotationY = CreateYRotationMatrix(rotation.y * (PI/180.f));
	Matrix4 mRotationX = CreateXRotationMatrix(rotation.x * (PI / 180.f));
	Matrix4 mRotationZ = CreateZRotationMatrix(rotation.z * (PI / 180.f));
	Matrix4 mScale = CreateScaleMatrix(scale);
	return mTranslate * mRotationY * mRotationX * mRotationZ * mScale;
}

Matrix4 Matrix4::CreateTranslationMatrix(const Vector3& translation)
{
	Matrix4 m = Matrix4::Identity();
	for (int i = 0; i < 3; i++)
		m[4*(i+1)-1] = translation[i];
	return m;
}

Matrix4 Matrix4::CreateScaleMatrix(const Vector3& scale)
{
	Matrix4 m = Matrix4::Identity();
	for (int i = 0; i < 3; i++)
		m[4*i+i] = scale[i];
	return m;
}

Matrix4 Matrix4::CreateXRotationMatrix(float angle)
{
	Matrix4 m = Matrix4::Identity();
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	m[5] = cosAngle; m[6] = -sinAngle;
	m[9] = sinAngle; m[10] = cosAngle;
	return m;
}

Matrix4 Matrix4::CreateYRotationMatrix(float angle)
{
	Matrix4 m = Matrix4::Identity();
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	m[0] = cosAngle; m[2] = sinAngle;
	m[8] = -sinAngle; m[10] = cosAngle;
	return m;
}

Matrix4 Matrix4::CreateZRotationMatrix(float angle)
{
	Matrix4 m = Matrix4::Identity();
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	m[0] = cosAngle; m[1] = -sinAngle;
	m[4] = sinAngle; m[5] = cosAngle;
	return m;
}

Vector3 Matrix4::RotatePointAroundThreeAxis(Vector3& anglesRadPerAxis, const Vector3& p, const Vector3 anchor)
{
	Vector4 vec4P(p);
	Vector4 result = Matrix4::RotatePointAroundThreeAxis(anglesRadPerAxis, vec4P, anchor);
	return Vector3(result.x, result.y, result.z);
}

Vector4 Matrix4::RotatePointAroundThreeAxis(Vector3& anglesRadPerAxis, const Vector4& p, const Vector3 anchor)
{
	Matrix4 matX = Matrix4::CreateXRotationMatrix(anglesRadPerAxis.x);
	Matrix4 matY = Matrix4::CreateYRotationMatrix(anglesRadPerAxis.y);
	Matrix4 matZ = Matrix4::CreateZRotationMatrix(anglesRadPerAxis.z);
	Matrix4 matTranslate = Matrix4::CreateTranslationMatrix(anchor);
	Matrix4 matTranslateInverse = Matrix4::CreateTranslationMatrix(-anchor);
	Matrix4 rotMatOrigin = matZ * matY * matX;
	Matrix4 rotMatAroundAnchor = matTranslate * rotMatOrigin * matTranslateInverse;
	return rotMatAroundAnchor * p;
}

Matrix4 Matrix4::GetPerspectiveMatrix(unsigned int width, unsigned int height, float near, float far, float fov) 
{
	Matrix4 result;

	float q = 1.0f / tan(0.5f * fov * PI / 180.f);
	float a = q / ((float)width/height);

	float b = (near + far) / (near - far);
	float c = (2.0f * near * far) / (near - far);

	result[0] = a;
	result[5] = q;
	result[10] = b;
	result[14] = -1.0f;
	result[11] = c;

	return result;
}
