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
FMatrix FCamera::GetViewMatrix() const
{
	const FVector up = mCameraRotation.GetUpVector();
	const FVector right = mCameraRotation.GetRightVector();
	const FVector forward = mCameraRotation.GetForwardVector();

	// | f_x  r_x  u_x  0 |
	// | f_y  r_y  u_y  0 |
	// | f_z  r_z  u_z  0 |
	// | -pf  -pr  -pu  1 |
	return FMatrix({
		{ forward.X, right.X, up.X, 0.0f },
		{ forward.Y, right.Y, up.Y, 0.0f },
		{ forward.Z, right.Z, up.Z, 0.0f },
		{ -mCameraPosition.Dot(forward), -mCameraPosition.Dot(right), -mCameraPosition.Dot(up), 1.0f }
		});
}

FMatrix FCamera::GetProjectionMatrix(float aspectRatio) const
{
	switch (mProjectionType)
	{
	case ECameraProjectionType::Perspective:
		return getPerspectiveProjectionMatrix(aspectRatio);
	case ECameraProjectionType::Orthographic:
		return getOrthographicProjectionMatrix(aspectRatio);
	default:
		assert(false && "Invalid projection type");
		return FMatrix::Identity;
	}
}

FMatrix FCamera::getPerspectiveProjectionMatrix(float aspectRatio) const
{
	const float fovRad = FMath::DegreesToRadians(mFovDeg);
	const float scaleY = 1.0f / std::tan(fovRad / 2.0f);
	const float scaleX = scaleY / aspectRatio;

	// | scaleX  0       0                    0 |
	// | 0       scaleY  0                    0 |
	// | 0       0       far/(far-near)       1 |
	// | 0       0       -near*far/(far-near) 0 |
	return FMatrix({
		{ scaleX, 0.0f, 0.0f, 0.0f },
		{ 0.0f, scaleY, 0.0f, 0.0f },
		{ 0.0f, 0.0f, mFarPlane / (mFarPlane - mNearPlane), 1.0f },
		{ 0.0f, 0.0f, -mNearPlane * mFarPlane / (mFarPlane - mNearPlane), 0.0f }
		});
}

FMatrix FCamera::getOrthographicProjectionMatrix(float aspectRatio) const
{
	const float fovRad = FMath::DegreesToRadians(mFovDeg);

	// h = 2d * tan(fov/2)
	const float height = 2 * mOrthoDistance * std::tan(fovRad / 2);
	const float width = height * aspectRatio;

	// | 2/width  0        0                    0 |
	// | 0        2/height 0                    0 |
	// | 0        0        1/(far-near)         0 |
	// | 0        0        -near/(far-near)     1 |
	return FMatrix({
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f / (mFarPlane - mNearPlane), 0.0f },
		{ 0.0f, 0.0f, -mNearPlane / (mFarPlane - mNearPlane), 1.0f }
		});
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
