// Core/Math/MathUtils.h

#include <numbers>

namespace FMath
{
	inline constexpr float PI = std::numbers::pi_v<float>;
	inline constexpr float DEG_TO_RAD = PI / 180.0f;
	inline constexpr float RAD_TO_DEG = 180.0f / PI;

	constexpr float DegreesToRadians(float degrees)
	{
		return degrees * DEG_TO_RAD;
	}
	
	constexpr float RadiansToDegrees(float radians)
	{
		return radians * RAD_TO_DEG;
	}
}