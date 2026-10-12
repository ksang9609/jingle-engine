// Core/Math/Vector.cpp

#include "Core/Math/Vector.h"

#include <cmath>

float FVector::LengthSquared() const
{
	return X * X + Y * Y + Z * Z;
}

float FVector::Length() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

FVector FVector::operator+(const FVector& v) const
{
	return { X + v.X, Y + v.Y, Z + v.Z };
}

FVector FVector::operator-(const FVector& v) const
{
	return { X - v.X, Y - v.Y, Z - v.Z };
}

FVector FVector::operator*(const float& f) const
{
	return { X * f, Y * f, Z * f };
}

FVector FVector::operator/(const float& f) const
{
	return { X / f, Y / f, Z / f };
}

FVector FVector::Normalized() const
{
	float len = Length();
	return { X / len, Y / len, Z / len };
}

float FVector::dot(const FVector& v) const
{
	return X * v.X + Y * v.Y + Z * v.Z;
}

float FVector::dot(const FVector& vA, const FVector& vB)
{
	return vA.X * vB.X + vA.Y * vB.Y + vA.Z * vB.Z;
}

static FVector operator*(const float& f, const FVector& v)
{
	return { v.X * f, v.Y * f, v.Z * f };
}
