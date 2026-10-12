// Rendering/Primitives/Triangle.h
#pragma once

#include "Rendering/VertexLayout.h"

inline constexpr FVertexSimple TRIANGLE_VERTICES[] =
{
	{ 0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
	{ 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
	{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
};