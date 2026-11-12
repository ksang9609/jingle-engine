// Core/Math/Quat.cpp
#include "Core/Math/Quat.h"

#include <algorithm>
#include <cassert>  
#include <cmath>
#include <numbers>

#include "Core/Math/Rotator.h"
#include "Core/Math/Vector.h"
#include "Core/Math/MathUtils.h"

const FQuat FQuat::Identity = FQuat(0.0f, 0.0f, 0.0f, 1.0f);

FQuat::FQuat()
	: X(0.0f)
	, Y(0.0f)
	, Z(0.0f)
	, W(1.0f)
{
}

FQuat::FQuat(float x, float y, float z, float w)
	: X(x)
	, Y(y)
	, Z(z)
	, W(w)
{
}

FQuat::FQuat(const NativeQuaternion& q)
	: X(q.x)
	, Y(q.y)
	, Z(q.z)
	, W(q.w)
{
}

FQuat::NativeQuaternion FQuat::toNative() const
{
	return NativeQuaternion(X, Y, Z, W);
}

FRotator FQuat::ToRotator() const
{
	constexpr double radToDeg =
		180.0 / std::numbers::pi_v<double>;

	double x = X;
	double y = Y;
	double z = Z;
	double w = W;

	const double length =
		std::sqrt(x * x + y * y + z * z + w * w);

	assert(std::isfinite(length) && length > 0.0);

	x /= length;
	y /= length;
	z /= length;
	w /= length;

	const double sinPitch =
		std::clamp(2.0 * (x * z - w * y), -1.0, 1.0);

	double roll;
	double pitch;
	double yaw;

	if (std::abs(sinPitch) >= 1.0 - 1e-6)
	{
		// Pitch가 ±90도에 가까우면
		// Roll과 Yaw를 독립적으로 결정할 수 없음.
		// 여기서는 Roll을 0으로 선택.
		roll = 0.0;
		pitch = std::copysign(
			std::numbers::pi_v<double> / 2.0, sinPitch);
		yaw = 2.0 * std::atan2(z, w);
	}
	else
	{
		pitch = std::asin(sinPitch);

		yaw = std::atan2(
			2.0 * (w * z + x * y),
			1.0 - 2.0 * (y * y + z * z));

		roll = std::atan2(
			-2.0 * (w * x + y * z),
			1.0 - 2.0 * (x * x + y * y));
	}

	return FRotator(
		static_cast<float>(std::remainder(roll * radToDeg, 360.0)),
		static_cast<float>(pitch * radToDeg),
		static_cast<float>(std::remainder(yaw * radToDeg, 360.0))
	);
}

FQuat FQuat::GetNormalized() const
{
	NativeQuaternion q = toNative();
	q.Normalize();

	return FQuat(q);
}

void FQuat::Normalize()
{
	*this = GetNormalized();
}

void FQuat::LookDirection(const FVector& forward)
{
	LookDirection(forward, FVector::Up);
}

void FQuat::LookDirection(const FVector& forward, const FVector& up)
{
	auto result = tryMakeLookDirectionQuat(forward, up);
	if (!result)
	{
		// Keep the current quaternion unchanged if the input is invalid
		return;
	}
	else
	{
		*this = result.value();
	}
}

void FQuat::LookAt(const FVector& eyePosition, const FVector& target)
{
	LookAt(eyePosition, target, FVector::Up);
}

void FQuat::LookAt(const FVector& eyePosition, const FVector& target, const FVector& up)
{
	const FVector forward = (target - eyePosition).GetNormalized();
	LookDirection(forward, up);
}

FQuat FQuat::MakeLookDirectionQuat(const FVector& forward)
{
	return MakeLookDirectionQuat(forward, FVector::Up);
}

FQuat FQuat::MakeLookDirectionQuat(const FVector& forward, const FVector& up)
{
	auto result = tryMakeLookDirectionQuat(forward, up);
	if (!result)
	{
		return FQuat::Identity;
	}
	return result.value();
}

FQuat FQuat::MakeLookAtQuat(const FVector& eyePosition, const FVector& target)
{
	return MakeLookAtQuat(eyePosition, target, FVector::Up);
}

FQuat FQuat::MakeLookAtQuat(const FVector& eyePosition, const FVector& target, const FVector& up)
{
	const FVector forward = (target - eyePosition).GetNormalized();
	return MakeLookDirectionQuat(forward, up);
}

FQuat FQuat::MakeFromAxisAngle(const FVector& axis, float angleDeg)
{
	NativeVector nativeAxis(axis.X, axis.Y, axis.Z);
	return FQuat(
		NativeQuaternion::CreateFromAxisAngle(
			nativeAxis, FMath::DegreesToRadians(angleDeg)));
}

TResult<FQuat, FString> FQuat::tryMakeLookDirectionQuat(const FVector& forward, const FVector& up)
{
	constexpr float epsilonSquared = 1e-8f;

	NativeVector forwardVec(forward.X, forward.Y, forward.Z);
	NativeVector upVec(up.X, up.Y, up.Z);

	// Return identity quaternion if forward vector is too small
	const float forwardLengthSquared = forwardVec.LengthSquared();
	if (!std::isfinite(forwardLengthSquared) ||
		forwardLengthSquared < epsilonSquared)
	{
		return TErr<FString>(FString("Forward vector is too small or invalid."));
	}
	forwardVec.Normalize();

	// Replace up vector to world up (0, 0, 1) if up vector is not valid
	const float upLengthSquared = upVec.LengthSquared();
	if (!std::isfinite(upLengthSquared) ||
		upLengthSquared < epsilonSquared)
	{
		upVec = NativeVector::UnitZ;
	}
	else
	{
		upVec.Normalize();
	}

	NativeVector rightVec = upVec.Cross(forwardVec);

	if (rightVec.LengthSquared() <= epsilonSquared)
	{
		upVec = (std::abs((forwardVec.z)) < 0.9f)
			? NativeVector::UnitZ
			: NativeVector::UnitY;

		rightVec = upVec.Cross(forwardVec);
	}
	rightVec.Normalize();

	// Recompute up vector to ensure orthogonality
	upVec = forwardVec.Cross(rightVec);
	upVec.Normalize();

	const NativeMatrix rotation(forwardVec, rightVec, upVec);

	NativeQuaternion q =
		NativeQuaternion::CreateFromRotationMatrix(rotation);

	q.Normalize();
	return FQuat(q);
}

FVector FQuat::GetAxisVector(EAxis axis) const
{
	const NativeQuaternion rotation = toNative();

	NativeVector result;
	switch (axis)
	{
	case EAxis::X:
		result = NativeVector::Transform(NativeVector::UnitX, rotation);
		break;
	case EAxis::Y:
		result = NativeVector::Transform(NativeVector::UnitY, rotation);
		break;
	case EAxis::Z:
		result = NativeVector::Transform(NativeVector::UnitZ, rotation);
		break;
	default:
		assert(false && "Invalid axis");
		return FVector::Zero;
	}

	return FVector{ result.x, result.y, result.z };
}

FVector FQuat::GetForwardVector() const
{
	return GetAxisVector(EAxis::X);
}

FVector FQuat::GetRightVector() const
{
	return GetAxisVector(EAxis::Y);
}

FVector FQuat::GetUpVector() const
{
	return GetAxisVector(EAxis::Z);
}

FQuat FQuat::operator*(const FQuat& other) const
{
	const NativeQuaternion lhs = toNative();
	const NativeQuaternion rhs = other.toNative();

	const auto result = lhs * rhs;

	return FQuat(result);
}

FQuat& FQuat::operator*=(const FQuat& other)
{
	*this = *this * other;
	return *this;
}

FQuat FQuat::Slerp(const FQuat& start, const FQuat& end, float alpha)
{
	const NativeQuaternion lhs = start.toNative();
	const NativeQuaternion rhs = end.toNative();

	const NativeQuaternion result = NativeQuaternion::Slerp(lhs, rhs, alpha);

	return FQuat(result);
}