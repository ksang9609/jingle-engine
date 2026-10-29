// Object/PrimitiveComponent.h
#pragma once

#include "Object/SceneComponent.h"

#include "RenderInterface/PrimitiveRenderData.h"

class UPrimitiveComponent : public USceneComponent
{
public:
	UPrimitiveComponent();
	virtual ~UPrimitiveComponent() = default;

	/* Methods */
	FPrimitiveRenderData GetRenderData() const;

	/* Getters / Setters */
	EPrimitiveType GetPrimitiveType() const;
	void SetPrimitiveType(EPrimitiveType InPrimitiveType);

private:
	EPrimitiveType mPrimitiveType;
};