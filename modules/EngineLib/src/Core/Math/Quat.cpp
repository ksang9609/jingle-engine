// Core/Math/Quat.cpp
#include "Core/Math/Quat.h"

#include <algorithm>
#include <cassert>  
#include <cmath>
#include <numbers>

#include "Core/Math/Rotator.h"

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