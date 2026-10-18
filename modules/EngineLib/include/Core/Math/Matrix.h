// Core/Math/Matrix.h
#pragma once

#include <SimpleMath/SimpleMath.h>

#include "Core/Core.h"

class FVector;
class FQuat;

class FMatrix
{
private:
	using NativeMatrix = DirectX::SimpleMath::Matrix;

public:
	FMatrix() = default;
	FMatrix(const float matrix[4][4]);
	FMatrix(NativeMatrix matrix);

	/* Methods */
	[[nodiscard]] FMatrix GetTransposed() const;
	void Transpose();
	[[nodiscard]] TResult<FMatrix, FString> GetInverse() const;
	[[nodiscard]] TResult<void, FString> Invert();

	[[nodiscard]] static FMatrix Translation(const FVector& translation);
	[[nodiscard]] static FMatrix Rotation(const FQuat& rotation);
	[[nodiscard]] static FMatrix Scale(const FVector& scale);

	/* Operators */
	float* operator[](std::size_t row);
	const float* operator[](std::size_t row) const;

	[[nodiscard]] FMatrix operator+(const FMatrix& other) const;
	[[nodiscard]] FMatrix operator-(const FMatrix& other) const;
	[[nodiscard]] FMatrix operator*(const FMatrix& other) const;
	[[nodiscard]] FMatrix operator*(float scalar) const;
	[[nodiscard]] FMatrix operator/(float scalar) const;
	friend FMatrix operator*(float scalar, const FMatrix& matrix);

	FMatrix& operator+=(const FMatrix& other);
	FMatrix& operator-=(const FMatrix& other);
	FMatrix& operator*=(const FMatrix& other);
	FMatrix& operator*=(float scalar);
	FMatrix& operator/=(float scalar);

	static const FMatrix Identity;

private:
	TResult<NativeMatrix, FString> getInverseMatrix() const;

private:
	NativeMatrix mMatrix = NativeMatrix::Identity;
};