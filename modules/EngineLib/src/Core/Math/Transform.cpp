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
	return FMatrix::MakeSRTMatrix(mTranslation, mRotation, mScale);
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
