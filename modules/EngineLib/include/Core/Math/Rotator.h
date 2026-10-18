// Core/Math/Rotator.h
#pragma once

#include <SimpleMath/SimpleMath.h>
#include "Core/Math/Quat.h"

class FVector;

class FRotator
{
public:
	float Roll;		// X-axis
	float Pitch;	// Y-axis
	float Yaw;		// Z-axis

	FRotator(float roll, float pitch, float yaw);
	FRotator(const FVector& euler);
	FRotator();


private:

};