// Core/Math/Camera.h
#pragma once

#include "Core/Math/MathTypes.h"

enum class ECameraProjectionType
{
	Perspective,
	Orthographic
};

class FCamera
{
public:
	FCamera();

	/* Methods */
	FMatrix GetViewMatrix() const;
	FMatrix GetProjectionMatrix(float aspectRatio) const;

	FCamera& LookDirection(const FVector& forward);
	FCamera& LookAt(const FVector& target);

	/* Getters/Setters */
	FVector GetPosition() const;
	FCamera& SetPosition(const FVector& position);
	FQuat GetRotation() const;
	FCamera& SetRotation(const FQuat& rotation);
	FCamera& SetRotation(const FRotator& rotation);

	float GetFovDeg() const;
	FCamera& SetFovDeg(float fovDeg);
	float GetNearPlane() const;
	FCamera& SetNearPlane(float nearPlane);
	float GetFarPlane() const;
	FCamera& SetFarPlane(float farPlane);
	ECameraProjectionType GetProjectionType() const;
	FCamera& SetProjectionType(ECameraProjectionType projectionType);

private:
	
private:
	FVector mCameraPosition;
	FQuat mCameraRotation;

	float mFovDeg; // Vertical Field of View in degrees
	float mNearPlane;
	float mFarPlane;
	float mOrthoDistance; // Distance from camera to near plane for orthographic projection
	ECameraProjectionType mProjectionType;
};