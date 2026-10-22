// Core/Math/Camera.h
#include "Core/Math/Camera.h"

FCamera::FCamera()
	: mCameraPosition(FVector::Zero)
	, mCameraRotation(FQuat::Identity)
	, mFovDeg(90.0f)
	, mNearPlane(0.1f)
	, mFarPlane(1000.0f)
	, mProjectionType(ECameraProjectionType::Perspective)
{
}

/* ===== Methods ===== */
FMatrix FCamera::GetViewMatrix() const
{
	return FMatrix::Identity;
}

/* ===== Getters/Setters ===== */
FVector FCamera::GetPosition() const
{
	return mCameraPosition;
}

FCamera& FCamera::SetPosition(const FVector& position)
{
	mCameraPosition = position;
	return *this;
}

FQuat FCamera::GetRotation() const
{
	return mCameraRotation;
}

FCamera& FCamera::SetRotation(const FQuat& rotation)
{
	mCameraRotation = rotation;
	return *this;
}

FCamera& FCamera::SetRotation(const FRotator& rotation)
{
	mCameraRotation = rotation.ToQuat();
	return *this;
}

float FCamera::GetFovDeg() const
{
	return mFovDeg;
}

FCamera& FCamera::SetFovDeg(float fovDeg)
{
	mFovDeg = fovDeg;
	return *this;
}

float FCamera::GetNearPlane() const
{
	return mNearPlane;
}

FCamera& FCamera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
	return *this;
}

float FCamera::GetFarPlane() const
{
	return mFarPlane;
}

FCamera& FCamera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
	return *this;
}

ECameraProjectionType FCamera::GetProjectionType() const
{
	return mProjectionType;
}

FCamera& FCamera::SetProjectionType(ECameraProjectionType projectionType)
{
	mProjectionType = projectionType;
	return *this;
}
