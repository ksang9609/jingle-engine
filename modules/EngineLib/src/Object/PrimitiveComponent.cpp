// Object/PrimitiveComponent.cpp
#include "Object/PrimitiveComponent.h"

PrimitiveComponent::PrimitiveComponent()
	: SceneComponent()
	, mPrimitiveType(EPrimitiveType::None)
{
}

/* ===== Methods ===== */
FPrimitiveRenderData PrimitiveComponent::GetRenderData() const
{
	FPrimitiveRenderData RenderData{
		.WorldMatrix = GetWorldMatrix(),
		.PrimitiveType = mPrimitiveType
	};

	return RenderData;
}

/* ===== Getters / Setters ===== */
EPrimitiveType PrimitiveComponent::GetPrimitiveType() const
{
	return mPrimitiveType;
}

void PrimitiveComponent::SetPrimitiveType(EPrimitiveType InPrimitiveType)
{
	mPrimitiveType = InPrimitiveType;
}
