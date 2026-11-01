// Core/Math/Camera.h
#include "Core/Math/Camera.h"

#include "Core/Math/MathUtils.h"

FCamera::FCamera()
	: mCameraPosition(FVector::Zero)
	, mCameraRotation(FQuat::Identity)
	, mFovDeg(90.0f)
	, mNearPlane(0.1f)
	, mFarPlane(1000.0f)
	, mOrthoDistance(10.0f)
	, mProjectionType(ECameraProjectionType::Perspective)
{
}

/* ===== Methods ===== */
// Re-define the axis vectors from UE to DirectX coordinate system:
// UE:		X (forward), Y (right), Z (up)
// DirectX:	X (right), Y (up), Z (forward)
FMatrix FCamera::GetViewMatrix() const
{
	return FMatrix::MakeViewMatrix(mCameraPosition, mCameraRotation);
}

FMatrix FCamera::GetProjectionMatrix(float aspectRatio) const
{
	switch (mProjectionType)
	{
	case ECameraProjectionType::Perspective:
		return FMatrix::MakePerspectiveProjectionMatrix(
			mFovDeg,
			aspectRatio,
			mNearPlane,
			mFarPlane);
	case ECameraProjectionType::Orthographic:
		return FMatrix::MakeOrthographicProjectionMatrix(
			mFovDeg,
			aspectRatio,
			mOrthoDistance,
			mNearPlane,
			mFarPlane);
	default:
		assert(false && "Invalid projection type");
		return FMatrix::Identity;
	}
}

FCamera& FCamera::LookDirection(const FVector& forward)
{
	mCameraRotation.LookDirection(forward);
	return *this;
}

FCamera& FCamera::LookAt(const FVector& target)
{
	mCameraRotation.LookAt(mCameraPosition, target);
	return *this;
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
