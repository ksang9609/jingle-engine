// Object/SceneComponent.cpp
#include "Object/SceneComponent.h"

SceneComponent::SceneComponent()
	: UObject()
	, mRelativeLocation(FVector::Zero)
	, mRelativeRotation(FQuat::Identity)
	, mRelativeScale(FVector::One)
{
}

/* ===== Methods ===== */
FMatrix SceneComponent::GetWorldMatrix() const
{
	return FMatrix::MakeSRTMatrix(
		mRelativeLocation, 
		mRelativeRotation, 
		mRelativeScale);
}

/* ===== Getter / Setters ===== */
const FVector& SceneComponent::GetRelativeLocation() const
{
	return mRelativeLocation;
}

void SceneComponent::SetRelativeLocation(const FVector& location)
{
	mRelativeLocation = location;
}

const FQuat& SceneComponent::GetRelativeRotation() const
{
	return mRelativeRotation;
}

void SceneComponent::SetRelativeRotation(const FQuat& rotation)
{
	mRelativeRotation = rotation;
}

void SceneComponent::SetRelativeRotation(const FRotator& eulerAngles)
{
	mRelativeRotation = eulerAngles.ToQuat();
}

const FVector& SceneComponent::GetRelativeScale() const
{
	return mRelativeScale;
}

void SceneComponent::SetRelativeScale(const FVector& scale)
{
	mRelativeScale = scale;
}
