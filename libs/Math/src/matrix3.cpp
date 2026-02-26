#include "matrix3.h"
using namespace Maths;

Matrix3::Matrix3(float _m[9])
{
	for (int i = 0; i < 9; i++)
		m[i] = _m[i];
}

Matrix3::Matrix3(float f)
{
	for (int i = 0; i < 9; i++)
		m[i] = f;
}

Matrix3::Matrix3(const Matrix3& copy)
{
	for (int i = 0; i < 9; i++)
		m[i] = copy[i];
}

Matrix3::Matrix3(const Vector3& line1, const Vector3& line2, const Vector3& line3)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector3 currentV = line3;
			if (i == 0)
				currentV = line1;
			else if (i == 1)
				currentV = line2;
			SetAt(i, j, currentV[j]);
		}
	}
}

Matrix3 Matrix3::operator*(const Matrix3& m) const
{
	return Multiply(m);
}

Vector3 Matrix3::operator*(const Vector3& v) const
{
	return Multiply(v);
}

Matrix3& Matrix3::operator=(const Matrix3& copy)
{
	for (int i = 0; i < 9; i++)
		m[i] = copy[i];
	return *this;
}

bool Matrix3::operator==(const Matrix3& other) const
{
	bool isEqual = true;
	for (unsigned int i = 0; i < 9 && isEqual; i++)
	{
		if (fabs(m[i] - other[i]) > 0.0000001f)
			isEqual = false;
	}
	return isEqual;
}

float& Matrix3::operator[](unsigned int index)
{
	assert(index < 9);
	return m[index];
}

float Matrix3::operator[](unsigned int index) const
{
	assert(index < 9);
	return m[index];
}

float Matrix3::GetAt(unsigned int row, unsigned int col) const
{
	assert(row < 3 && col < 3);
	return m[row * 3 + col];
}

void Matrix3::SetAt(unsigned int row, unsigned int col, float val)
{
	assert(row < 3 && col < 3);
	m[row * 3 + col] = val;
}

Vector3 Matrix3::GetDiagonal() const
{
	return Vector3(m[0], m[4], m[8]);
}

float Matrix3::GetTrace() const
{
	Vector3 diag = GetDiagonal();
	float res = 0.f;
	for (int i = 0; i < 3; i++)
		res += diag[i];
	return res;
}

float Matrix3::Det() const
{
	float det1 = m[4] * m[5] - m[5] * m[7];
	float det2 = m[3] * m[8] - m[5] * m[6];
	float det3 = m[3] * m[7] - m[4] * m[6];
	return m[0] * det1 - m[1] * det2 + m[2] * det3;
}

Matrix3 Matrix3::Opposite() const
{
	Matrix3 tempM = *this;
	for (unsigned int i = 0; i < 9; i++)
		tempM[i] = -tempM[i];
	return tempM;
}

Matrix3 Matrix3::Transpose() const
{
	Matrix3 res(0.f);
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
			res.SetAt(j, i, GetAt(i, j));
	}
	return res;
}

Matrix3& Matrix3::Transpose()
{
	Matrix3 tempM(0.f);
	for (unsigned int i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
			tempM.SetAt(j, i, GetAt(i, j));
	}
	(*this) = tempM;
	return *this;
}

Matrix3 Matrix3::Add(const Matrix3& m2) const
{
	Matrix3 res = m2;
	for (unsigned int i = 0; i < 9; i++)
		res[i] = m[i] + res[i];
	return res;
}

Matrix3& Matrix3::Add(const Matrix3& m2)
{
	for (unsigned int i = 0; i < 9; i++)
		m[i] += m2[i];
	return *this;
}

Matrix3 Matrix3::Scale(float f) const
{
	Matrix3 res = *this;
	for (unsigned int i = 0; i < 9; i++)
		res[i] *= f;
	return res;
}

Matrix3& Matrix3::Scale(float f)
{
	for (unsigned int i = 0; i < 9; i++)
		m[i] *= f;
	return *this;
}

Matrix3 Matrix3::GaussJordan() const
{
	Matrix3 copy = *this;
	return copy.GaussJordan();
}

Matrix3& Matrix3::GaussJordan()
{
	int r = -1;
	// j is the column
	for (unsigned int j = 0; j < 3; j++)
	{
		// We look for the index k (the row) of the maximum num in this column. i is the current row.
		float max = 0; int k = 0;
		for (unsigned int i = r + 1; r + 1 < 3 && i < 3; i++)
		{
			if (fabs(GetAt(i, j)) > fabs(max))
			{
				k = i;
				max = GetAt(i, j);
			}
		}

		// We operate based on the pivot m[k,j] <=> m[3*k + j] here
		float pivot = GetAt(k, j);
		if (pivot != 0 && r + 1 < 3)
		{
			r++;
			// Normalization of the row by the pivot
			for (unsigned int tempCol = 0; tempCol < 3; tempCol++)
				SetAt(k, tempCol, GetAt(k, tempCol) / pivot);

			// Swapping k row and r row if they are not the same number
			if (k != r)
			{
				// Storing k row temporarily
				std::vector<float> temp;
				for (unsigned int tempI = 0; tempI < 3; tempI++)
					temp.push_back(GetAt(k, tempI));
				// Storing r row into k row
				for (unsigned int tempI = 0; tempI < 3; tempI++)
					SetAt(k, tempI, GetAt(r, tempI));
				// Inject temporary k row into r row
				for (unsigned int tempI = 0; tempI < 3; tempI++)
					SetAt(r, tempI, temp[tempI]);
			}
			for (unsigned int i = 0; i < 3; i++)
			{
				// Subtract (row r * m[3*i + j]) from row i
				if (i != r)
				{
					float factor = GetAt(i, j);
					for (unsigned int tempCol = 0; tempCol < 3; tempCol++)
						SetAt(i, tempCol, GetAt(i, tempCol) - GetAt(r, tempCol) * factor);
				}
			}
		}
	}
	return *this;
}

Matrix3 Matrix3::Multiply(const Matrix3& m2) const
{
	Matrix3 res = *this;
	return res.Multiply(m2);
}

Matrix3& Matrix3::Multiply(const Matrix3& m2)
{
	Matrix3 mRes(0.f);
	for (unsigned int i1 = 0; i1 < 3; i1++)
	{
		for (unsigned int j2 = 0; j2 < 3; j2++)
		{
			for (unsigned int commonI = 0; commonI < 3; commonI++)
			{
				float resValue = GetAt(i1, commonI) * m2.GetAt(commonI, j2);
				mRes.SetAt(i1, j2, mRes.GetAt(i1, j2) + resValue);
			}
		}
	}
	(*this) = mRes;
	return *this;
}

Vector3 Matrix3::Multiply(const Vector3& v2) const
{
	Vector3 vRes(0.f);
	std::vector<float> resValues;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			vRes[i] += GetAt(i, j) * v2[j];
	}
	return vRes;
}

std::ostream& Maths::operator<<(std::ostream& os, const Matrix3& m)
{
	for (unsigned int i = 0; i < 9; i++)
	{
		if (i % 3 == 0)
			os << "\n";
		os << m[i] << " ";
	}
	return os;
}

Matrix3 Matrix3::Identity()
{
	Matrix3 id(0.f);
	for (unsigned int i = 0; i < 3; i++)
		id.SetAt(i, i, 1);
	return id;
}

Matrix3 Matrix3::CreateTranslationMatrix(const Vector2& translation)
{
	Matrix3 translationMat = Matrix3::Identity();
	translationMat[2] = translation.x;
	translationMat[5] = translation.y;
	return translationMat;
}

Matrix3 Matrix3::CreateScaleMatrix(const Vector2& scale)
{
	Matrix3 scaleMat = Matrix3::Identity();
	scaleMat[0] = scale.x;
	scaleMat[4] = scale.y;
	return scaleMat;
}

Matrix3 Matrix3::CreateXRotationMatrix(float angle)
{
	angle *= PI / 180;
	Matrix3 rotMat = Matrix3::Identity();
	rotMat[4] = cosf(angle); rotMat[5] = -sinf(angle);
	rotMat[7] = sinf(angle); rotMat[8] = cosf(angle);
	return rotMat;
}

Matrix3 Matrix3::CreateYRotationMatrix(float angle)
{
	angle *= PI / 180;
	Matrix3 rotMat = Matrix3::Identity();
	rotMat[0] = cosf(angle); rotMat[2] = sinf(angle);
	rotMat[6] = -sinf(angle); rotMat[8] = cosf(angle);
	return rotMat;
}

Matrix3 Matrix3::CreateZRotationMatrix(float angle)
{
	angle *= PI / 180;
	Matrix3 rotMat = Matrix3::Identity();
	rotMat[0] = cosf(angle); rotMat[1] = -sinf(angle);
	rotMat[3] = sinf(angle); rotMat[4] = cosf(angle);
	return rotMat;
}
