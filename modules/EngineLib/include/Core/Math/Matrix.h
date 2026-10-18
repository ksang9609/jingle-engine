// Core/Math/Matrix.h
#pragma once

#include <SimpleMath/SimpleMath.h>

#include "Core/Core.h"

class FMatrix
{
private:
	using Matrix4x4 = DirectX::SimpleMath::Matrix;

public:
	FMatrix() = default;
	FMatrix(const float matrix[4][4]);
	FMatrix(Matrix4x4 matrix);

	/* Methods */
	[[nodiscard]] FMatrix GetTransposed() const;
	void Transpose();
	[[nodiscard]] TResult<FMatrix, FString> GetInverse() const;
	[[nodiscard]] TResult<void, FString> Invert();

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
	TResult<Matrix4x4, FString> getInverseMatrix() const;

private:
	Matrix4x4 mMatrix = Matrix4x4::Identity;
};