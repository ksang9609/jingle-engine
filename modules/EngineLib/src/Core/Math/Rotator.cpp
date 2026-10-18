// Core/Math/Rotator.cpp
#include "Core/Math/Rotator.h"

#include <cmath>
#include <numbers>

#include "Core/Math/Quat.h"
#include "Core/Math/Vector.h"

FRotator::FRotator(float roll, float pitch, float yaw)
	: Roll(roll)
	, Pitch(pitch)
	, Yaw(yaw)
{
}

FRotator::FRotator(const FVector& euler)
	: Roll(euler.X)
	, Pitch(euler.Y)
	, Yaw(euler.Z)
{
}

FRotator::FRotator()
	: Roll(0.0f)
	, Pitch(0.0f)
	, Yaw(0.0f)
{
}

FQuat FRotator::ToQuat() const
{
    constexpr double halfDegToRad =
        std::numbers::pi_v<double> / 360.0;

    const double roll = Roll * halfDegToRad;
    const double pitch = Pitch * halfDegToRad;
    const double yaw = Yaw * halfDegToRad;

    const double sr = std::sin(roll);
    const double cr = std::cos(roll);
    const double sp = std::sin(pitch);
    const double cp = std::cos(pitch);
    const double sy = std::sin(yaw);
    const double cy = std::cos(yaw);

    return FQuat(
        static_cast<float>(cr * sp * sy - sr * cp * cy),
        static_cast<float>(-cr * sp * cy - sr * cp * sy),
        static_cast<float>(cr * cp * sy - sr * sp * cy),
        static_cast<float>(cr * cp * cy + sr * sp * sy)
    );
}