// Core/Math/Matrix.cpp
#include "Core/Math/Matrix.h"

#include <cassert>

#include "Core/Math/Vector.h"
#include "Core/Math/Quat.h"

const FMatrix FMatrix::Identity = FMatrix(NativeMatrix::Identity);

FMatrix::FMatrix(const float matrix[4][4])
{
	for (std::size_t i = 0; i < 4; ++i)
	{
		for (std::size_t j = 0; j < 4; ++j)
		{
			mMatrix.m[i][j] = matrix[i][j];
		}
	}
}

FMatrix::FMatrix(NativeMatrix matrix)
	: mMatrix(matrix)
{
}

/* ===== Methods ===== */
FMatrix FMatrix::GetTransposed() const
{
	return FMatrix(mMatrix.Transpose());
}

void FMatrix::Transpose()
{
	mMatrix = mMatrix.Transpose();
}

TResult<FMatrix, FString> FMatrix::GetInverse() const
{
	auto result = getInverseMatrix();

	if (!result)
	{
		return TErr(std::move(result.error()));
	}

	return FMatrix(result.value());
}

TResult<void, FString> FMatrix::Invert()
{
	auto result = getInverseMatrix();

	if (!result)
	{
		return TErr(std::move(result.error()));
	}	

	mMatrix = result.value();
	return {};
}

TResult<FMatrix::NativeMatrix, FString> FMatrix::getInverseMatrix() const
{
	using namespace DirectX;

	const XMMATRIX source = XMLoadFloat4x4(&mMatrix);

	XMVECTOR determinant;
	const XMMATRIX inverse = XMMatrixInverse(&determinant, source);

	const float det = XMVectorGetX(determinant);

	if (!std::isfinite(det) || det == 0.0f)
	{
		return TErr(FString("Matrix is singular or has an invalid determinant."));
	}

	if (XMMatrixIsNaN(inverse) || XMMatrixIsInfinite(inverse))
	{
		return TErr(FString("Matrix inversion resulted in NaN values."));
	}

	NativeMatrix result;
	XMStoreFloat4x4(&result, inverse);

	return result;
}

// | 1  0  0  0 |
// | 0  1  0  0 |
// | 0  0  1  0 |
// | tx ty tz 1 |
FMatrix FMatrix::Translation(const FVector& translation)
{
	return FMatrix(NativeMatrix::CreateTranslation(
		translation.X, translation.Y, translation.Z));
}

// | r00 r01 r02 0 |
// | r10 r11 r12 0 |
// | r20 r21 r22 0 |
// | 0   0   0   1 |
// rotation must be normalized
FMatrix FMatrix::Rotation(const FQuat& rotation)
{
	const DirectX::SimpleMath::Quaternion q(
		rotation.X, rotation.Y, rotation.Z, rotation.W
	);

	return FMatrix(NativeMatrix::CreateFromQuaternion(q));
}

// | sx 0  0  0 |
// | 0  sy 0  0 |
// | 0  0  sz 0 |
// | 0  0  0  1 |
FMatrix FMatrix::Scale(const FVector& scale)
{
	return FMatrix(NativeMatrix::CreateScale(
		scale.X, scale.Y, scale.Z));
}

/* ===== Operators ===== */
float* FMatrix::operator[](std::size_t row)
{
	assert(row < 4);
	return mMatrix.m[row];
}

const float* FMatrix::operator[](std::size_t row) const
{
	assert(row < 4);
	return mMatrix.m[row];
}

FMatrix FMatrix::operator+(const FMatrix& other) const
{
	return FMatrix(mMatrix + other.mMatrix);
}

FMatrix FMatrix::operator-(const FMatrix& other) const
{
	return FMatrix(mMatrix - other.mMatrix);
}

FMatrix FMatrix::operator*(const FMatrix& other) const
{
	return FMatrix(mMatrix * other.mMatrix);
}

FMatrix FMatrix::operator*(float scalar) const
{
	return FMatrix(mMatrix * scalar);
}

FMatrix FMatrix::operator/(float scalar) const
{
	return FMatrix(mMatrix / scalar);
}


FMatrix operator*(float scalar, const FMatrix& matrix)
{
	return FMatrix(scalar * matrix.mMatrix);
}

FMatrix& FMatrix::operator+=(const FMatrix& other)
{
	mMatrix += other.mMatrix;
	return *this;
}

FMatrix& FMatrix::operator-=(const FMatrix& other)
{
	mMatrix -= other.mMatrix;
	return *this;
}

FMatrix& FMatrix::operator*=(const FMatrix& other)
{
	mMatrix *= other.mMatrix;
	return *this;
}

FMatrix& FMatrix::operator*=(float scalar)
{
	mMatrix *= scalar;
	return *this;
}

FMatrix& FMatrix::operator/=(float scalar)
{
	mMatrix /= scalar;
	return *this;
}
