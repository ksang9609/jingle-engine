// Object/PrimitiveComponent.cpp
#include "Object/PrimitiveComponent.h"

UPrimitiveComponent::UPrimitiveComponent()
	: USceneComponent()
	, mPrimitiveType(EPrimitiveType::None)
{
}

/* ===== Methods ===== */
FPrimitiveRenderData UPrimitiveComponent::GetRenderData() const
{
	FPrimitiveRenderData RenderData{
		.WorldMatrix = GetWorldMatrix(),
		.PrimitiveType = mPrimitiveType
	};

	return RenderData;
}

/* ===== Getters / Setters ===== */
EPrimitiveType UPrimitiveComponent::GetPrimitiveType() const
{
	return mPrimitiveType;
}

void UPrimitiveComponent::SetPrimitiveType(EPrimitiveType InPrimitiveType)
{
	mPrimitiveType = InPrimitiveType;
}
