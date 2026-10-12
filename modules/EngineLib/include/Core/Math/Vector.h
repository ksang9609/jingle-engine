// Core/Math/Vector.h
#pragma once
class FVector
{
public:
	float X;
	float Y;
	float Z;

	inline FVector(float x, float y, float z) 
		: X(x), Y(y), Z(z) {}
	inline FVector() : X(0), Y(0), Z(0) {}
	inline FVector(float n) 
		: X(n), Y(n), Z(n) {}

	float LengthSquared() const;
	float Length() const;

	FVector operator+(const FVector& v) const;
	FVector operator-(const FVector& v) const;
	FVector operator*(const float& f) const;
	FVector operator/(const float& f) const;

	FVector Normalized() const;

	float dot(const FVector& v) const;
	static float dot(const FVector& vA, const FVector& vB);

private:

};

static FVector operator*(const float& f, const FVector& v);