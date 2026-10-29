// Object/SceneComponent.cpp
#include "Object/SceneComponent.h"

USceneComponent::USceneComponent()
	: UObject()
	, mRelativeLocation(FVector::Zero)
	, mRelativeRotation(FQuat::Identity)
	, mRelativeScale(FVector::One)
{
}

/* ===== Methods ===== */
FMatrix USceneComponent::GetWorldMatrix() const
{
	return FMatrix::MakeSRTMatrix(
		mRelativeLocation, 
		mRelativeRotation, 
		mRelativeScale);
}

/* ===== Getter / Setters ===== */
const FVector& USceneComponent::GetRelativeLocation() const
{
	return mRelativeLocation;
}

void USceneComponent::SetRelativeLocation(const FVector& location)
{
	mRelativeLocation = location;
}

const FQuat& USceneComponent::GetRelativeRotation() const
{
	return mRelativeRotation;
}

void USceneComponent::SetRelativeRotation(const FQuat& rotation)
{
	mRelativeRotation = rotation;
}

void USceneComponent::SetRelativeRotation(const FRotator& eulerAngles)
{
	mRelativeRotation = eulerAngles.ToQuat();
}

const FVector& USceneComponent::GetRelativeScale() const
{
	return mRelativeScale;
}

void USceneComponent::SetRelativeScale(const FVector& scale)
{
	mRelativeScale = scale;
}
