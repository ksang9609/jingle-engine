// Core/Math/Vector.h
#pragma once

class FVector
{
public:
	float X;
	float Y;
	float Z;

	FVector(float x, float y, float z);
	FVector();
	FVector(float n);

	float LengthSquared() const;
	float Length() const;

	FVector operator+(const FVector& v) const;
	FVector operator-(const FVector& v) const;
	FVector operator*(const float& f) const;
	FVector operator/(const float& f) const;

	void Normalize();
	[[nodiscard]] FVector Normalized() const;

	float Dot(const FVector& v) const;
	static float Dot(const FVector& vA, const FVector& vB);

	static FVector Zero;
	static FVector One;

private:

};

using FVector3 = FVector;

static FVector operator*(const float& f, const FVector& v);

class FVector4
{
public:
	float X;
	float Y;
	float Z;
	float W;

	FVector4(float x, float y, float z, float w);
	FVector4();
	FVector4(float n);
	
	float LengthSquared() const;
	float Length() const;
	float Length3() const;


	FVector4 operator+(const FVector4& v) const;
	FVector4 operator-(const FVector4& v) const;
	FVector4 operator*(const float& f) const;
	FVector4 operator/(const float& f) const;

	void Normalize();
	[[nodiscard]] FVector4 GetNormalized() const;

	float Dot(const FVector4& v) const;
	static float Dot(const FVector4& vA, const FVector4& vB);

	static FVector4 Zero;
	static FVector4 One;
};