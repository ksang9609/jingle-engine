// RenderInterface/PrimitiveRenderData.h
#pragma once

#include "Core/Math/MathTypes.h"

enum class EPrimitiveType
{
	None,
	Sphere,
	Cube
};

struct FPrimitiveRenderData
{
	FMatrix WorldMatrix;
	EPrimitiveType PrimitiveType;
};