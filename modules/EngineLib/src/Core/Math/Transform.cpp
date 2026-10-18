// Core/Math/Transform.cpp
#include "Core/Math/Transform.h"

#include "Core/Core.h"

const FTransform FTransform::Identity = FTransform(FVector::Zero, FQuat::Identity, FVector::One);

FTransform::FTransform(FVector translation, FQuat rotation, FVector scale)
	: mTranslation(translation), mRotation(rotation), mScale(scale)
{
}

FTransform::FTransform(FVector translation, FRotator rotation, FVector scale)
	: mTranslation(translation), mRotation(rotation.ToQuat()), mScale(scale)
{
}

FTransform::FTransform()
	: mTranslation(FVector::Zero), mRotation(FQuat::Identity), mScale(FVector::One)
{
}

/* ===== Methods ===== */

// M = S * R * T (row-vector convention)
// | sx*r00 sx*r01 sx*r02 0 |
// | sy*r10 sy*r11 sy*r12 0 |
// | sz*r20 sz*r21 sz*r22 0 |
// | tx     ty     tz     1 |
FMatrix FTransform::GetWorldMatrix() const
{
	using namespace DirectX::SimpleMath;

	const Quaternion rotation(
		mRotation.X, mRotation.Y, mRotation.Z, mRotation.W);

	// R:
	// | r00 r01 r02 0 |
	// | r10 r11 r12 0 |
	// | r20 r21 r22 0 |
	// | 0   0   0   1 |
	Matrix result = Matrix::CreateFromQuaternion(rotation);

	// S * R:
	// | sx 0  0  0 |   | r00 r01 r02 0 |   | sx*r00 sx*r01 sx*r02 0 |
	// | 0  sy 0  0 | * | r10 r11 r12 0 | = | sy*r10 sy*r11 sy*r12 0 |
	// | 0  0  sz 0 |   | r20 r21 r22 0 |   | sz*r20 sz*r21 sz*r22 0 |
	// | 0  0  0  1 |   | 0   0   0   1 |   | 0      0      0      1 |
	for (int column = 0; column < 3; ++column)
	{
		result.m[0][column] *= mScale.X;
		result.m[1][column] *= mScale.Y;
		result.m[2][column] *= mScale.Z;
	}

	// (S * R) * T:
	// | sx*r00 sy*r01 sz*r02 0 |	| 1  0  0  0 |   | sx*r00 sy*r01 sz*r02 0 |
	// | sx*r10 sy*r11 sz*r12 0 | *	| 0  1  0  0 | = | sx*r10 sy*r11 sz*r12 0 |
	// | sx*r20 sy*r21 sz*r22 0 |	| 0  0  1  0 |   | sx*r20 sy*r21 sz*r22 0 |
	// | 0      0      0      1 |	| tx ty tz 1 |   | tx     ty     tz     1 |
	result.m[3][0] = mTranslation.X;
	result.m[3][1] = mTranslation.Y;
	result.m[3][2] = mTranslation.Z;

	return result;
}

// TODO: Optimize 
TResult<FMatrix, FString> FTransform::GetInverseWorldMatrix() const
{
	return GetWorldMatrix().GetInverse();
}

/* ===== Getters / Setters ===== */
const FVector& FTransform::GetTranslation() const
{
	return mTranslation;
}

void FTransform::SetTranslation(const FVector& translation)
{
	mTranslation = translation;
}

const FQuat& FTransform::GetRotation() const
{
	return mRotation;
}

void FTransform::SetRotation(const FQuat& rotation)
{
	mRotation = rotation;
}

void FTransform::SetRotation(const FRotator& rotation)
{
	mRotation = rotation.ToQuat();
}

const FVector& FTransform::GetScale() const
{
	return mScale;
}

void FTransform::SetScale(const FVector& scale)
{
	mScale = scale;
}
