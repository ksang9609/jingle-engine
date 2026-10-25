// Core/Math/Matrix.cpp
#include "Core/Math/Matrix.h"

#include <cassert>

#include "Core/Math/Vector.h"
#include "Core/Math/Quat.h"
#include "Core/Math/MathUtils.h"

const FMatrix FMatrix::Identity = FMatrix(NativeMatrix::Identity);

FMatrix::FMatrix(const float (&matrix)[4][4])
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
FMatrix FMatrix::MakeTranslationMatrix(const FVector& translation)
{
	return FMatrix(NativeMatrix::CreateTranslation(
		translation.X, translation.Y, translation.Z));
}

// | r00 r01 r02 0 |
// | r10 r11 r12 0 |
// | r20 r21 r22 0 |
// | 0   0   0   1 |
// rotation must be normalized
FMatrix FMatrix::MakeRotationMatrix(const FQuat& rotation)
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
FMatrix FMatrix::MakeScaleMatrix(const FVector& scale)
{
	return FMatrix(NativeMatrix::CreateScale(
		scale.X, scale.Y, scale.Z));
}

// M = S * R * T (row-vector convention)
// | sx*r00 sx*r01 sx*r02 0 |
// | sy*r10 sy*r11 sy*r12 0 |
// | sz*r20 sz*r21 sz*r22 0 |
// | tx     ty     tz     1 |
FMatrix FMatrix::MakeSRTMatrix(
	const FVector& translation, 
	const FQuat& rotation, 
	const FVector& scale)
{
	using NativeQuaternion = DirectX::SimpleMath::Quaternion;

	const NativeQuaternion q(
		rotation.X, rotation.Y, rotation.Z, rotation.W);

	// R:
	// | r00 r01 r02 0 |
	// | r10 r11 r12 0 |
	// | r20 r21 r22 0 |
	// | 0   0   0   1 |
	NativeMatrix result = NativeMatrix::CreateFromQuaternion(q);

	// S * R:
	// | sx 0  0  0 |   | r00 r01 r02 0 |   | sx*r00 sx*r01 sx*r02 0 |
	// | 0  sy 0  0 | * | r10 r11 r12 0 | = | sy*r10 sy*r11 sy*r12 0 |
	// | 0  0  sz 0 |   | r20 r21 r22 0 |   | sz*r20 sz*r21 sz*r22 0 |
	// | 0  0  0  1 |   | 0   0   0   1 |   | 0      0      0      1 |
	for (int column = 0; column < 3; ++column)
	{
		result.m[0][column] *= scale.X;
		result.m[1][column] *= scale.Y;
		result.m[2][column] *= scale.Z;
	}

	// (S * R) * T:
	// | sx*r00 sy*r01 sz*r02 0 |	| 1  0  0  0 |   | sx*r00 sy*r01 sz*r02 0 |
	// | sx*r10 sy*r11 sz*r12 0 | *	| 0  1  0  0 | = | sx*r10 sy*r11 sz*r12 0 |
	// | sx*r20 sy*r21 sz*r22 0 |	| 0  0  1  0 |   | sx*r20 sy*r21 sz*r22 0 |
	// | 0      0      0      1 |	| tx ty tz 1 |   | tx     ty     tz     1 |
	result.m[3][0] = translation.X;
	result.m[3][1] = translation.Y;
	result.m[3][2] = translation.Z;

	return result;
}

// Re-define the axis vectors from UE to DirectX coordinate system:
// UE:		X (forward), Y (right), Z (up)
// DirectX:	X (right), Y (up), Z (forward)
FMatrix FMatrix::MakeViewMatrix(const FVector& eyePosition, const FQuat& rotation)
{
	const FVector up = rotation.GetUpVector();
	const FVector right = rotation.GetRightVector();
	const FVector forward = rotation.GetForwardVector();

	// | r_x  u_x  f_x  0 |
	// | r_y  u_y  f_y  0 |
	// | r_z  u_z  f_z  0 |
	// | -pr  -pu  -pf  1 |
	return FMatrix({
		{ right.X, up.X, forward.X, 0.0f },
		{ right.Y, up.Y, forward.Y, 0.0f },
		{ right.Z, up.Z, forward.Z, 0.0f },
		{ -eyePosition.Dot(right), -eyePosition.Dot(up), -eyePosition.Dot(forward), 1.0f }
		});
}

FMatrix FMatrix::MakePerspectiveProjectionMatrix(
	float fovYDeg,
	float aspectRatio,
	float nearPlane,
	float farPlane)
{
	const float fovRad = FMath::DegreesToRadians(fovYDeg);
	const float scaleY = 1.0f / std::tan(fovRad / 2.0f);
	const float scaleX = scaleY / aspectRatio;
	// | scaleX  0       0                    0 |
	// | 0       scaleY  0                    0 |
	// | 0       0       far/(far-near)       1 |
	// | 0       0       -near*far/(far-near) 0 |
	return FMatrix({
		{ scaleX, 0.0f, 0.0f, 0.0f },
		{ 0.0f, scaleY, 0.0f, 0.0f },
		{ 0.0f, 0.0f, farPlane / (farPlane - nearPlane), 1.0f },
		{ 0.0f, 0.0f, -nearPlane * farPlane / (farPlane - nearPlane), 0.0f }
		});
}

FMatrix FMatrix::MakeOrthographicProjectionMatrix(
	float fovYDeg,
	float aspectRatio,
	float orthoDistance,
	float nearPlane,
	float farPlane)
{
	const float fovRad = FMath::DegreesToRadians(fovYDeg);

	// h = 2d * tan(fov/2)
	const float height = 2 * orthoDistance * std::tan(fovRad / 2);
	const float width = height * aspectRatio;

	// | 2/width  0        0                    0 |
	// | 0        2/height 0                    0 |
	// | 0        0        1/(far-near)         0 |
	// | 0        0        -near/(far-near)     1 |
	return FMatrix({
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f / (farPlane - nearPlane), 0.0f },
		{ 0.0f, 0.0f, -nearPlane / (farPlane - nearPlane), 1.0f }
		});
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
