// Core/Math/Quat.h
#pragma once

#include <SimpleMath/SimpleMath.h>

#include "Core/Core.h"
#include "Core/Math/MathEnums.h"

class FRotator;
class FVector;
class FQuat
{
private:
	using NativeVector = DirectX::SimpleMath::Vector3;
	using NativeQuaternion = DirectX::SimpleMath::Quaternion;
	using NativeMatrix = DirectX::SimpleMath::Matrix;
public:
	float X;
	float Y;
	float Z;
	float W;

	FQuat();
	FQuat(float x, float y, float z, float w);

	/* Methods */
	FRotator ToRotator() const;
	[[nodiscard]] FQuat GetNormalized() const;
	void Normalize();

	void LookDirection(const FVector& forward);	// use world up (0, 0, 1)
	void LookDirection(const FVector& forward, const FVector& up);
	void LookAt(const FVector& eyePosition, const FVector& target);			// use world up (0, 0, 1)
	void LookAt(const FVector& eyePosition, const FVector& target, const FVector& up);

	static FQuat MakeLookDirectionQuat(const FVector& forward);	// use world up (0, 0, 1)
	static FQuat MakeLookDirectionQuat(const FVector& forward, const FVector& up);
	static FQuat MakeLookAtQuat(const FVector& eyePosition, const FVector& target);	// use world up (0, 0, 1)
	static FQuat MakeLookAtQuat(const FVector& eyePosition, const FVector& target, const FVector& up);
	static FQuat MakeFromAxisAngle(const FVector& axis, float angleDeg);

	FVector GetAxisVector(EAxis axis) const;
	FVector GetForwardVector() const;
	FVector GetRightVector() const;
	FVector GetUpVector() const;

	static FQuat Slerp(const FQuat& start, const FQuat& end, float alpha);

	/* Operators */
	[[nodiscard]] FQuat operator*(const FQuat& other) const;
	FQuat& operator*=(const FQuat& other);


	static const FQuat Identity;

private:
	FQuat(const NativeQuaternion& q);
	NativeQuaternion toNative() const;

	static TResult<FQuat, FString> tryMakeLookDirectionQuat(const FVector& forward, const FVector& up);
};
