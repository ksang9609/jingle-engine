// Core/Math/Transform.h
#pragma once

#include "Core/Math/MathTypes.h"

class FTransform
{
public:
	FTransform(FVector translation, FQuat rotation, FVector scale);
	FTransform(FVector translation, FRotator rotation, FVector scale);
	FTransform();

	/* Methods */
	FMatrix GetWorldMatrix() const;
	TResult<FMatrix, FString> GetInverseWorldMatrix() const;

	/* Getters / Setters */
	const FVector& GetTranslation() const;
	void SetTranslation(const FVector& translation);

	const FQuat& GetRotation() const;
	void SetRotation(const FQuat& rotation);
	void SetRotation(const FRotator& rotation);

	const FVector& GetScale() const;
	void SetScale(const FVector& scale);

	static const FTransform Identity;

private:
	FVector mTranslation;
	FQuat mRotation;
	FVector mScale;
};