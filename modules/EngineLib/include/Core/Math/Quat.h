// Core/Math/Quat.h
#pragma once

#include <SimpleMath/SimpleMath.h>

class FRotator;
class FQuat
{
private:
	using NativeQuaternion = DirectX::SimpleMath::Quaternion;
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

	static FQuat Slerp(const FQuat& start, const FQuat& end, float alpha);

	/* Operators */
	[[nodiscard]] FQuat operator*(const FQuat& other) const;
	FQuat& operator*=(const FQuat& other);


	static const FQuat Identity;

private:
	FQuat(const NativeQuaternion& q);
	NativeQuaternion toNative() const;
};
