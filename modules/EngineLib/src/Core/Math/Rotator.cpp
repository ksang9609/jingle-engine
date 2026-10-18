// Core/Math/Rotator.cpp
#include "Core/Math/Rotator.h"

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
