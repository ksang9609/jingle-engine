// Core/Math/Vector.cpp

#include "Core/Math/Vector.h"

#include <cmath>

FVector FVector::Zero = FVector(0, 0, 0);
FVector FVector::One = FVector(1, 1, 1);

FVector::FVector(float x, float y, float z)
	: X(x)
	, Y(y)
	, Z(z) 
{
}

FVector::FVector()
	: X(0)
	, Y(0)
	, Z(0)
{
}

FVector::FVector(float n)
	: X(n)
	, Y(n)
	, Z(n)
{
}

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

void FVector::Normalize()
{
	float len = Length();
	X /= len;
	Y /= len;
	Z /= len;
}

FVector FVector::Normalized() const
{
	float len = Length();
	return FVector{ X / len, Y / len, Z / len };
}

float FVector::Dot(const FVector& v) const
{
	return X * v.X + Y * v.Y + Z * v.Z;
}

float FVector::Dot(const FVector& vA, const FVector& vB)
{
	return vA.X * vB.X + vA.Y * vB.Y + vA.Z * vB.Z;
}

FVector operator*(const float& f, const FVector& v)
{
	return FVector{ v.X * f, v.Y * f, v.Z * f };
}


/* ===== FVector4 ===== */

FVector4 FVector4::Zero = FVector4(0, 0, 0, 1);
FVector4 FVector4::One = FVector4(1, 1, 1, 1);

FVector4::FVector4(float x, float y, float z, float w)
	: X(x)
	, Y(y)
	, Z(z)
	, W(w)
{
}

FVector4::FVector4()
	: X(0)
	, Y(0)
	, Z(0)
	, W(0)
{
}

FVector4::FVector4(float n)
	: X(n)
	, Y(n)
	, Z(n)
	, W(n)
{
}

float FVector4::LengthSquared() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

float FVector4::Length() const
{
	return sqrt(X * X + Y * Y + Z * Z + W * W);
}

float FVector4::Length3() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

FVector4 FVector4::operator+(const FVector4& v) const
{
	return { X + v.X, Y + v.Y, Z + v.Z, W + v.W };
}

FVector4 FVector4::operator-(const FVector4& v) const
{
	return { X - v.X, Y - v.Y, Z - v.Z, W - v.W };
}

FVector4 FVector4::operator*(const float& f) const
{
	return { X * f, Y * f, Z * f, W * f };
}

FVector4 FVector4::operator/(const float& f) const
{
	return { X / f, Y / f, Z / f, W / f };
}

void FVector4::Normalize()
{
	float len = Length();
	X /= len;
	Y /= len;
	Z /= len;
	W /= len;
}

FVector4 FVector4::GetNormalized() const
{
	float len = Length();
	return FVector4{ X / len, Y / len, Z / len, W / len };
}

float FVector4::Dot(const FVector4& v) const
{
	return X * v.X + Y * v.Y + Z * v.Z + W * v.W;
}

float FVector4::Dot(const FVector4& vA, const FVector4& vB)
{
	return vA.X * vB.X + vA.Y * vB.Y + vA.Z * vB.Z + vA.W * vB.W;
}