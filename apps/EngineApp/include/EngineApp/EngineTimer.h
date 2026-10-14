// EngineApp/EngineTimer.h
#pragma once

#include <windows.h>

#include "Core/Core.h"

class FEngineTimer final
{
public:
	FEngineTimer(int targetFPS = 0);
	~FEngineTimer() = default;

	void StartFrame();
	void WaitForNextFrame();

	void SetTargetFPS(uint32 targetFPS);

	float64 GetDeltaTimeMS() const { return mDeltaTimeMS; }

private:
	LARGE_INTEGER mStartTimeTick;
	LARGE_INTEGER mFrequency; // Ticks per second

	uint32 mTargetFPS; // 0 for unlimited FPS
	float64 mTargetFrameTimeMS;

	float64 mDeltaTimeMS;
};