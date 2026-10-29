// Object/PrimitiveComponent.h
#pragma once

#include "Object/SceneComponent.h"

#include "RenderInterface/PrimitiveRenderData.h"

class PrimitiveComponent : public SceneComponent
{
public:
	PrimitiveComponent();
	virtual ~PrimitiveComponent() = default;

	/* Methods */
	FPrimitiveRenderData GetRenderData() const;

	/* Getters / Setters */
	EPrimitiveType GetPrimitiveType() const;
	void SetPrimitiveType(EPrimitiveType InPrimitiveType);

private:
	EPrimitiveType mPrimitiveType;
};