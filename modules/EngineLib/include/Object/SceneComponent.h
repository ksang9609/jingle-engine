// Object/SceneComponent.h
#pragma once

#include "Object/Object.h"

#include "Core/Math/MathTypes.h"

class SceneComponent : public UObject
{
public:
	SceneComponent();
	virtual ~SceneComponent() = default;

	/* Methods */
	FMatrix GetWorldMatrix() const;

	/* Getter / Setters */
	const FVector& GetRelativeLocation() const;
	void SetRelativeLocation(const FVector& location);
	const FQuat& GetRelativeRotation() const;
	void SetRelativeRotation(const FQuat& rotation);
	void SetRelativeRotation(const FRotator& eulerAngles);
	const FVector& GetRelativeScale() const;
	void SetRelativeScale(const FVector& scale);

private:
	FVector mRelativeLocation;
	FQuat mRelativeRotation;
	FVector mRelativeScale;
};